// BHC_graph2D.cpp : 구현 파일입니다.
//

#include "stdafx.h"
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
	m_iEdgeSize	= 30;

	m_dX_min_world = -100;
	m_dX_max_world = 100;
	m_dY_min_world = -100;
	m_dY_max_world = 100;

	m_dX_min_local = -100;
	m_dX_max_local = 100;
	m_dY_min_local = -100;
	m_dY_max_local = 100;

	m_dStepX_sub	= 10;
	m_dStepY_sub	= 10;

	m_iRadius_point = 1;

	m_bUseFilter = false;
	m_iFilterIndex = 0;


	for(int i=-100 ; i<100 ; i++ )
	{
		stPtDisplay pt;
		pt.x = i;
		pt.y = i;
		pt.iRed = 0;
		pt.iGreen = (i+100)%255;
		pt.iBlue = 0;

		m_listPoint.push_back(pt);
	}

	m_iZoom = 0;

	m_ptLButtonDown = CPoint(0,0);
	m_ptCurrent		= CPoint(0,0);
	m_bLButtonDown	= false;

	m_pParent = NULL;
}

CBHC_graph2D::~CBHC_graph2D()
{
}


BEGIN_MESSAGE_MAP(CBHC_graph2D, CScrollView)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE( WM_INVALIDATE_VIWE, OnReceive )
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


	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0,255,0) );    // 녹색 펜 생성
	CPen* oldPen = pDC->SelectObject(&pen);

	pDC->SetBkMode(TRANSPARENT);

	// draw points
	for( std::list<stPtDisplay>::iterator ite = m_listPoint.begin() ; ite != m_listPoint.end() ; ite++ )
	{
		// filtering 적용 시
		if( m_bUseFilter==true && ite->iIndex != m_iFilterIndex )
		{
			continue;
		}


		// 좌표
		double x, y;
		ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, ite->x, ite->y, &x, &y);


		// 선택한 점에 대해서는 테두리를 그린다.
		if( ite->bSelect )
		{
			CPen pen_temp;
			pen_temp.CreatePen(PS_SOLID, 2, RGB( 0, 255, 0) ); 
			pDC->SelectObject(&pen_temp);

			// 그리기
			pDC->Ellipse(	(int)(x - m_iRadius_point - 5), 
							(int)(y - m_iRadius_point - 5), 
							(int)(x + m_iRadius_point + 5 ), 
							(int)(y + m_iRadius_point + 5 ) );
		}


		// 색설정
		CPen pen_temp;
		pen_temp.CreatePen(PS_SOLID, 3, RGB( ite->iRed, ite->iGreen, ite->iBlue) ); 
		pDC->SelectObject(&pen_temp);

		// 그리기
		pDC->Ellipse(	(int)(x - m_iRadius_point), 
						(int)(y - m_iRadius_point), 
						(int)(x + m_iRadius_point), 
						(int)(y + m_iRadius_point) );
	}


	// draw point of interest
	for( std::list<stPtDisplay>::iterator ite = m_listPoint2.begin() ; ite != m_listPoint2.end() ; ite++ )
	{
		// 좌표
		double x, y;
		ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, ite->x, ite->y, &x, &y);

		// 색설정
		CPen pen_temp;
		pen_temp.CreatePen(PS_SOLID, 1, RGB( ite->iRed, ite->iGreen, ite->iBlue) ); 
		pDC->SelectObject(&pen_temp);

		// 그리기
		pDC->Ellipse(	(int)(x - 3), 
						(int)(y - 3), 
						(int)(x + 3), 
						(int)(y + 3));
	}


	// Draw rect
	if( m_bLButtonDown )
	{
		// 색설정
		CPen pen_temp;
		pen_temp.CreatePen(PS_SOLID, 3, RGB( 0, 255, 0) ); 
		pDC->SelectObject(&pen_temp);

		pDC->SetBkMode(TRANSPARENT);

		// 그리기
		pDC->MoveTo(	min( m_ptLButtonDown.x, m_ptCurrent.x ), min( m_ptLButtonDown.y, m_ptCurrent.y ) );
		pDC->LineTo(	max( m_ptLButtonDown.x, m_ptCurrent.x ), min( m_ptLButtonDown.y, m_ptCurrent.y ) );
		pDC->LineTo(	max( m_ptLButtonDown.x, m_ptCurrent.x ), max( m_ptLButtonDown.y, m_ptCurrent.y ) );
		pDC->LineTo(	min( m_ptLButtonDown.x, m_ptCurrent.x ), max( m_ptLButtonDown.y, m_ptCurrent.y ) );
		pDC->LineTo(	min( m_ptLButtonDown.x, m_ptCurrent.x ), min( m_ptLButtonDown.y, m_ptCurrent.y ) );
	}

	pDC->SelectObject(oldPen);
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
	// real 좌표 기준 축 설정
	m_dX_min_world = dX_min;
	m_dX_max_world = dX_max;
	m_dY_min_world = dY_min;
	m_dY_max_world = dY_max;


// 	// window 좌표 기준 축 설정
// 	m_dX_min_local = m_iEdgeSize;
// 	m_dX_max_local = m_szTotal.cx - m_iEdgeSize;
// 	m_dY_min_local = m_szTotal.cy - m_iEdgeSize;
// 	m_dY_max_local = m_iEdgeSize;


	PostMessage(WM_INVALIDATE_VIWE);
//	Invalidate();
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

	PostMessage(WM_INVALIDATE_VIWE);
//	Invalidate();
}


//////////////////////////////////////////////////////////////////////////
//
// Graph 영역 설정 (화면 기준)
//
//////////////////////////////////////////////////////////////////////////
void CBHC_graph2D::SetSize( int x, int y )
{
	m_szTotal = CSize(x, y);


	// window 좌표 기준 축 설정
	m_dX_min_local = m_iEdgeSize;
	m_dX_max_local = m_szTotal.cx - m_iEdgeSize;
	m_dY_min_local = m_szTotal.cy - m_iEdgeSize;
	m_dY_max_local = m_iEdgeSize;


	PostMessage(WM_INVALIDATE_VIWE);
//	Invalidate(TRUE);
}


void CBHC_graph2D::SetPointRadius( int iRadius )
{
	m_iRadius_point = iRadius;

	PostMessage(WM_INVALIDATE_VIWE);
//	Invalidate(TRUE);
}


void CBHC_graph2D::Reset(  )
{
	for( std::list<stPtDisplay>::iterator ite = m_listPoint.begin() ; ite != m_listPoint.end() ; ite++ )
	{
		ite->bSelect = false;
	}

	PostMessage(WM_INVALIDATE_VIWE);
}


std::list<unsigned int> CBHC_graph2D::GetIndex_selection()
{
	std::list<unsigned int> listIndex;

	for( std::list<stPtDisplay>::iterator ite = m_listPoint.begin() ; ite != m_listPoint.end() ; ite++ )
	{
		if( ite->bSelect )
		{
			listIndex.push_back(ite->iIndex);
		}
	}

	return listIndex;
}


void CBHC_graph2D::SetFilter( bool bUse, int iIndex )
{
	m_bUseFilter = bUse;
	m_iFilterIndex = iIndex;

	PostMessage(WM_INVALIDATE_VIWE);
//	Invalidate(TRUE);
}


void CBHC_graph2D::SetMessage( CWnd *pWnd, UINT uiMessage )
{
	m_pParent = pWnd;
	m_uiMessage = uiMessage;
}


void CBHC_graph2D::ClearData()
{
	m_listPoint.clear();
	m_listPoint2.clear();
}


void CBHC_graph2D::AddData(double x, double y)
{
	stPtDisplay pt;
	pt.x = x;
	pt.y = y;

	m_listPoint.push_back(pt);
}


void CBHC_graph2D::AddData(double x, double y, int iRed, int iGreen, int iBlue, int iIndex )
{
	stPtDisplay pt;
	pt.x = x;
	pt.y = y;

	pt.iRed = iRed;
	pt.iGreen = iGreen;
	pt.iBlue = iBlue;

	pt.iIndex = iIndex;

	m_listPoint.push_back(pt);
}


void CBHC_graph2D::AddData2(double x, double y)
{
	stPtDisplay pt;
	pt.x = x;
	pt.y = y;

	m_listPoint2.push_back(pt);
}


void CBHC_graph2D::Draw_mainAxis(CDC* pDC)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 3, RGB(0,0,0) );    // 녹색 펜 생성
	CPen* oldPen = pDC->SelectObject(&pen);


	CFont font,*pOldFont;
	font.CreatePointFont(50*GetZoomRatio(),_T("Lucida Grande Bold"));
	pOldFont=(CFont*)pDC->SelectObject(&font);



	// x axis
	double x1, y1;
	ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, m_dX_min_world, m_dY_min_world, &x1, &y1);

	double x2, y2;
	ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, m_dX_max_world, m_dY_min_world, &x2, &y2);

	// line
	pDC->MoveTo((int)x1, (int)y1);
	pDC->LineTo((int)x2, (int)y2);


	// value
	CString str;
	str.Format(_T("%.2f"), m_dX_min_world );
	pDC->TextOut((int)x1+5, (int)y1+5, str);

	str.Format(_T("%.2f"), m_dX_max_world );
	pDC->TextOut((int)x2+5, (int)y2+5, str);



	//
	// y axis
	//
	ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, m_dX_min_world, m_dY_min_world, &x1, &y1);

	ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, m_dX_min_world, m_dY_max_world, &x2, &y2);

	// line
	pDC->MoveTo((int)x1, (int)y1);
	pDC->LineTo((int)x2, (int)y2);

	// value
	pDC->SetTextAlign(TA_RIGHT|TA_BOTTOM);

	str.Format(_T("%.1f"), m_dY_min_world );
	pDC->TextOut((int)x1-5, (int)y1-5, str);

	str.Format(_T("%.1f"), m_dY_max_world );
	pDC->TextOut((int)x2-5, (int)y2-5, str);


	pDC->SelectObject(oldPen);
}


void CBHC_graph2D::Draw_subAxis(CDC* pDC)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(200,200,200) );    // 회색 펜 생성
	CPen* oldPen = pDC->SelectObject(&pen);


	// x axis
	for (double x = m_dX_min_world ; x < m_dX_max_world ; x += m_dStepX_sub )
	{
		double x1, y1;
		ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, x, m_dY_min_world, &x1, &y1);

		double x2, y2;
		ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, x, m_dY_max_world, &x2, &y2);

		pDC->MoveTo((int)x1, (int)y1);
		pDC->LineTo((int)x2, (int)y2);
	}


	// y axis
	for (double y = m_dY_min_world ; y < m_dY_max_world ; y += m_dStepY_sub )
	{
		double x1, y1;
		ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, m_dX_min_world, y, &x1, &y1);

		double x2, y2;
		ConvertCoordinate(_BHC_DIR_WORLD_TO_LOCAL_, m_dX_max_world, y, &x2, &y2);

		pDC->MoveTo((int)x1, (int)y1);
		pDC->LineTo((int)x2, (int)y2);
	}


	pDC->SelectObject(oldPen);
}


//////////////////////////////////////////////////////////////////////////
//
//
//	x의 window 좌표 구하기
//			x - a
//	(d-c) x ----- + c
//          b - a
//		x
//		|
//	a-------------------------------b	world
//
//	c-----------------d					local
//
//
//
//////////////////////////////////////////////////////////////////////////
void CBHC_graph2D::ConvertCoordinate(int iDirection, double dX_in, double dY_in, double *pdX, double *pdY)
{
	// real 좌표계에서 축범위
	double dXRangeWorld = m_dX_max_world - m_dX_min_world;
	double dYRangeWorld = m_dY_max_world - m_dY_min_world;

	// view 좌표계에서 축범위
	double dXRangeLocal = pow((double)2, m_iZoom)*m_dX_max_local - pow((double)2, m_iZoom)*m_dX_min_local;
	double dYRangeLocal = pow((double)2, m_iZoom)*m_dY_max_local - pow((double)2, m_iZoom)*m_dY_min_local;

	double x, y;

	switch( iDirection )
	{
	case _BHC_DIR_WORLD_TO_LOCAL_:
		x = (dX_in - m_dX_min_world) * dXRangeLocal / dXRangeWorld + pow((double)2, m_iZoom)*m_dX_min_local;
		y = (dY_in - m_dY_min_world) * dYRangeLocal / dYRangeWorld + pow((double)2, m_iZoom)*m_dY_min_local;
		break;

	case _BHC_DIR_LOCAL_TO_WORLD_:
		x = (dX_in - pow((double)2, m_iZoom)*m_dX_min_local) * dXRangeWorld / dXRangeLocal + m_dX_min_world;
		y = (dY_in - pow((double)2, m_iZoom)*m_dY_min_local) * dYRangeWorld / dYRangeLocal + m_dY_min_world;
		break;
	}


	*pdX = x;
	*pdY = y;
}




BOOL CBHC_graph2D::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// zoom in
	if( zDelta > 0 )
	{
		ZoomIn();
	} 


	// zoom out
	if( zDelta < 0 )
	{
		ZoomOut();
	}


	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CBHC_graph2D::ZoomIn()
{
	if( m_iZoom > 5 )
	{
		return;
	}


	m_iZoom++;
	
	CPoint ptScroll_old = GetScrollPosition();


	CSize sizeTotal;
	sizeTotal.cx = (LONG)(GetZoomRatio() * m_szTotal.cx);
	sizeTotal.cy = (LONG)(GetZoomRatio() * m_szTotal.cy);
	SetScrollSizes(MM_TEXT, sizeTotal);


	// ptScroll_old.x + m_szTotal.cx/2 : 현재 중심의 위치(view 기준)
	SetScrollPos(SB_HORZ, max( 2 * (ptScroll_old.x + m_szTotal.cx/2 ) - m_szTotal.cx/2, 0 ) );
	SetScrollPos(SB_VERT, max( 2 * (ptScroll_old.y + m_szTotal.cy/2 ) - m_szTotal.cy/2, 0 ) );


	PostMessage(WM_INVALIDATE_VIWE);
//	Invalidate();
}


void CBHC_graph2D::ZoomOut()
{
	if( m_iZoom < 1 )
	{
		return;
	}


	m_iZoom--;

	CPoint ptScroll_old = GetScrollPosition();


	CSize sizeTotal;
	sizeTotal.cx = (LONG)(GetZoomRatio() * m_szTotal.cx);
	sizeTotal.cy = (LONG)(GetZoomRatio() * m_szTotal.cy);
	SetScrollSizes(MM_TEXT, sizeTotal);


	// ptScroll_old.x + m_szTotal.cx/2 : 현재 중심의 위치(view 기준)
	SetScrollPos(SB_HORZ, max( 0.5 * (ptScroll_old.x + m_szTotal.cx/2 ) - m_szTotal.cx/2, 0 ) );
	SetScrollPos(SB_VERT, max( 0.5 * (ptScroll_old.y + m_szTotal.cy/2 ) - m_szTotal.cy/2, 0 ) );


	PostMessage(WM_INVALIDATE_VIWE);
//	Invalidate();
}


double CBHC_graph2D::GetZoomRatio()
{
	return pow((double)2, m_iZoom);
}


void CBHC_graph2D::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint ptScroll = GetScrollPosition();

	m_ptLButtonDown = CPoint( ptScroll.x + point.x, ptScroll.y+ point.y );

	m_bLButtonDown = true;


	CScrollView::OnLButtonDown(nFlags, point);
}


void CBHC_graph2D::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint ptScroll = GetScrollPosition();

	m_ptCurrent = CPoint( ptScroll.x + point.x, ptScroll.y+ point.y );

	if( m_bLButtonDown )
	{
		PostMessage(WM_INVALIDATE_VIWE);
	}

	CScrollView::OnMouseMove(nFlags, point);
}


void CBHC_graph2D::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bLButtonDown = false;


	CPoint ptScroll = GetScrollPosition();
	CPoint ptLButtonUp = CPoint( ptScroll.x + point.x, ptScroll.y+ point.y );
	
	if( m_ptLButtonDown != ptLButtonUp )	// 마우스 왼쪽버튼의 down위치와 up위치가 다를 때 처리
	{
		double dx_1, dx_2, dy_1, dy_2;

		ConvertCoordinate( _BHC_DIR_LOCAL_TO_WORLD_, min(m_ptLButtonDown.x, ptLButtonUp.x), min(m_ptLButtonDown.y, ptLButtonUp.y), &dx_1, &dy_1 );
		ConvertCoordinate( _BHC_DIR_LOCAL_TO_WORLD_, max(m_ptLButtonDown.x, ptLButtonUp.x), max(m_ptLButtonDown.y, ptLButtonUp.y), &dx_2, &dy_2 );

		double dx_min = min( dx_1, dx_2 );
		double dy_min = min( dy_1, dy_2 );
		double dx_max = max( dx_1, dx_2 );
		double dy_max = max( dy_1, dy_2 );

		for( std::list<stPtDisplay>::iterator ite = m_listPoint.begin() ; ite != m_listPoint.end() ; ite++ )
		{
			if( ite->x >= dx_min && ite->x <=dx_max && ite->y >=dy_min && ite->y <= dy_max )
			{
				ite->bSelect = true;
			}
		}

	}

	PostMessage(WM_INVALIDATE_VIWE);

	CScrollView::OnLButtonUp(nFlags, point);
}


void CBHC_graph2D::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// view 영역 기준 좌표로 변환
	CPoint ptScroll = GetScrollPosition();

	CPoint ptCur = CPoint(	point.x + ptScroll.x, 
		point.y + ptScroll.y );


	// wordl 좌표로 변환
	double dX = 0;
	double dY = 0;
	ConvertCoordinate(_BHC_DIR_LOCAL_TO_WORLD_, ptCur.x, ptCur.y, &dX, &dY );


	// 가장 가까운 점 찾기
	double dMinDistance = DBL_MAX;
	int iIndex_minDist = -1;
	int iIndex = 0;
	std::list<stPtDisplay>::iterator ite_minDist;
	bool bResult = false;

	for( std::list<stPtDisplay>::iterator ite = m_listPoint.begin() ; ite != m_listPoint.end() ; ite++, iIndex++ )
	{

		ite->bSelect = false;

		// filtering 적용 시
		if( m_bUseFilter==true && ite->iIndex != m_iFilterIndex )
		{
			continue;
		}

		// 거리
		double dDistance = sqrt( pow( dX - ite->x, 2) + pow( dY - ite->y, 2) );

		if( dDistance < dMinDistance )
		{
			dMinDistance = dDistance;
			iIndex_minDist = ite->iIndex;
			ite_minDist = ite;
			bResult = true;
		}
	}

	if( bResult ==true )
	{
		ite_minDist->bSelect = true;

		if( m_pParent)
		{
			m_pParent->PostMessage(m_uiMessage, iIndex_minDist, 0 );
		}
	}

// 	Invalidate();
	PostMessage(WM_INVALIDATE_VIWE);


	CScrollView::OnRButtonDown(nFlags, point);
}


LRESULT CBHC_graph2D::OnReceive(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your control notification handler code here

	//	if( wParam == WM_INVALIDATE_VIWE )
	{
		Invalidate();
	}

	return 0;
}


