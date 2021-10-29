// BHC_graph2D.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BHC_colorSpace.h"
#include "BHC_graph2D.h"


// CBHC_graph2D

IMPLEMENT_DYNCREATE(CBHC_graph2D, CScrollView)

CBHC_graph2D::CBHC_graph2D()
{
	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_szTotal = CSize(100,100 );
	m_iEdgeSize	= 10;

	m_dX_min = -100;
	m_dX_max = 100;
	m_dY_min = -100;
	m_dY_max = 100;

	m_dStepX_sub	= 10;
	m_dStepY_sub	= 10;


	for(int i=-100 ; i<100 ; i++ )
	{
		stPoint2D pt;
		pt.x = i;
		pt.y = i;

		m_listPoint.push_back(pt);
	}
}

CBHC_graph2D::~CBHC_graph2D()
{
}


BEGIN_MESSAGE_MAP(CBHC_graph2D, CScrollView)
END_MESSAGE_MAP()


// CBHC_graph2D 그리기입니다.

void CBHC_graph2D::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CBHC_graph2D::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	//
	Draw_mainAxis(pDC);


	Draw_subAxis(pDC);


	// draw points
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(0,255,0) );    // 녹색 펜 생성
		CPen* oldPen = pDC->SelectObject(&pen);

		pDC->SetBkMode(TRANSPARENT);

		for( std::list<stPoint2D>::iterator ite = m_listPoint.begin() ; ite != m_listPoint.end() ; ite++ )
		{
			double x, y;
			ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, ite->x, ite->y, &x, &y);

			pDC->Ellipse(	(int)(x - 3), 
							(int)(y - 3), 
							(int)(x + 3), 
							(int)(y + 3) );
		}

		pDC->SelectObject(oldPen);
	}


	// draw point of interest
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(255,0,0) );    // 녹색 펜 생성
		CPen* oldPen = pDC->SelectObject(&pen);

		for( std::list<stPoint2D>::iterator ite = m_listPoint2.begin() ; ite != m_listPoint2.end() ; ite++ )
		{
			double x, y;
			ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, ite->x, ite->y, &x, &y);

			pDC->Ellipse(	(int)(x - 3), 
							(int)(y - 3), 
							(int)(x + 3), 
							(int)(y + 3));
		}


		pDC->SelectObject(oldPen);
	}
}


// CBHC_graph2D 진단입니다.

#ifdef _DEBUG
void CBHC_graph2D::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CBHC_graph2D::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBHC_graph2D 메시지 처리기입니다.
//////////////////////////////////////////////////////////////////////////
//
// Graph 영역 설정 (data 기준)
//
//////////////////////////////////////////////////////////////////////////
void CBHC_graph2D::SetRange( double dX_min, double dX_max, double dY_min, double dY_max )
{
	m_dX_min = dX_min;
	m_dX_max = dX_max;
	m_dY_min = dY_min;
	m_dY_max = dY_max;
}


//////////////////////////////////////////////////////////////////////////
//
//	SetGrid
//
//	Graph 보조눈금 설정
//
//
//
//////////////////////////////////////////////////////////////////////////
void CBHC_graph2D::SetGrid( double dStepX, double dStepY )
{
	m_dStepX_sub = dStepX;
	m_dStepY_sub = dStepY;
}


//////////////////////////////////////////////////////////////////////////
//
// Graph 영역 설정 (화면 기준)
//
//////////////////////////////////////////////////////////////////////////
void CBHC_graph2D::SetSize( int x, int y )
{
	m_szTotal = CSize(x, y);

	Invalidate(TRUE);
}


void CBHC_graph2D::ClearData()
{
	m_listPoint.clear();
	m_listPoint2.clear();
}


void CBHC_graph2D::AddData(double x, double y)
{
	stPoint2D pt;
	pt.x = x;
	pt.y = y;

	m_listPoint.push_back(pt);
}


void CBHC_graph2D::AddData2(double x, double y)
{
	stPoint2D pt;
	pt.x = x;
	pt.y = y;

	m_listPoint2.push_back(pt);
}


void CBHC_graph2D::Draw_mainAxis(CDC* pDC)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 3, RGB(0,0,0) );    // 녹색 펜 생성
	CPen* oldPen = pDC->SelectObject(&pen);

	// x axis
	double x1, y1;
	ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, m_dX_min, 0, &x1, &y1);

	double x2, y2;
	ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, m_dX_max, 0, &x2, &y2);

	pDC->MoveTo((int)x1, (int)y1);
	pDC->LineTo((int)x2, (int)y2);


	// y axis
	ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, 0, m_dY_min, &x1, &y1);

	ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, 0, m_dY_max, &x2, &y2);

	pDC->MoveTo((int)x1, (int)y1);
	pDC->LineTo((int)x2, (int)y2);


	pDC->SelectObject(oldPen);
}


void CBHC_graph2D::Draw_subAxis(CDC* pDC)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(200,200,200) );    // 녹색 펜 생성
	CPen* oldPen = pDC->SelectObject(&pen);


	// x axis
	for (double x = m_dX_min ; x < m_dX_max ; x += m_dStepX_sub )
	{
		double x1, y1;
		ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, x, m_dY_min, &x1, &y1);

		double x2, y2;
		ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, x, m_dY_max, &x2, &y2);

		pDC->MoveTo((int)x1, (int)y1);
		pDC->LineTo((int)x2, (int)y2);
	}


	// y axis
	for (double y = m_dY_min ; y < m_dY_max ; y += m_dStepY_sub )
	{
		double x1, y1;
		ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, m_dX_min, y, &x1, &y1);

		double x2, y2;
		ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, m_dX_max, y, &x2, &y2);

		pDC->MoveTo((int)x1, (int)y1);
		pDC->LineTo((int)x2, (int)y2);
	}


	pDC->SelectObject(oldPen);
}


void CBHC_graph2D::ConvertCoordinate(int iDirection, double dX_in, double dY_in, double *pdX, double *pdY)
{
	double dXRangeWorld = m_dX_max - m_dX_min;
	double dYRangeWorld = m_dY_max - m_dY_min;

	double dXRangeLocal = m_szTotal.cx - 2*m_iEdgeSize;
	double dYRangeLocal = m_szTotal.cy - 2*m_iEdgeSize;

	double x, y;

	switch( iDirection )
	{
	case _BHC_DIR_WORLD_TO_LOCAL_:
		x = dXRangeLocal * (dX_in - m_dX_min)/dXRangeWorld + m_iEdgeSize;
		y = dYRangeLocal * (dY_in - m_dY_min)/dYRangeWorld + m_iEdgeSize;
		y = m_szTotal.cy - y;	// y는 역방향
		break;

	case _BHC_DIR_LOCAL_TO_WORLD_:
		break;
	}


	*pdX = x;
	*pdY = y;
}


