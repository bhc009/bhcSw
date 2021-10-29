// ImageViewer.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
// #include "crop_BHC.h"
#include "ImageViewer.h"
#include <math.h>


// CImageViewer

IMPLEMENT_DYNCREATE(CImageViewer, CScrollView)

CImageViewer::CImageViewer()
{
	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_iZoom = 0;
	m_dZoomFactor = 1.0;


	m_bIsRoi			= false;
	m_ptLBtnDown_view	= CPoint(0,0);
	m_ptLBtnDown_view_old	= CPoint(0,0);
	m_ptLBtnUp_view		= CPoint(0,0);
	m_ptLBtnUp_view_old		= CPoint(0,0);

	m_ptLBtnDown_image	= CPoint(0,0);
	m_ptLBtnDown_image_old	= CPoint(0,0);
	m_ptLBtnUp_image	= CPoint(0,0);
	m_ptLBtnUp_image_old	= CPoint(0,0);


	m_rtRoi_view		= CRect(0,0,0,0);
	m_rtRoi_image		= CRect(0,0,0,0);


	m_bIsInterest		= false;
	m_ptInterst_view	= CPoint(0,0);
	m_ptInterst_image	= CPoint(0,0);

	OutputDebugString(_T("CImageViewer : ver 0.99"));
}

CImageViewer::~CImageViewer()
{
}


BEGIN_MESSAGE_MAP(CImageViewer, CScrollView)
	ON_MESSAGE( WM_INVALIDATE_VIWE, OnReceive )
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CImageViewer �׸����Դϴ�.

void CImageViewer::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImageViewer::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	if( IsImage() )
	{
		// ���� ǥ��
		m_image.Draw(pDC->m_hDC,
			0, 0,
			(int)(m_dZoomFactor * GetImageWidth()),
			(int)(m_dZoomFactor * GetImageHeight()));


		if (m_bIsRoi)
		{
			pDC->SetBkMode(TRANSPARENT);

			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(0,255,0) );    // ������ �� ����
			CPen* oldPen = pDC->SelectObject(&pen);

			pDC->SetTextColor(RGB(0, 255, 0));


			pDC->MoveTo(m_rtRoi_view.left, m_rtRoi_view.top);
			pDC->LineTo(m_rtRoi_view.right, m_rtRoi_view.top);
			pDC->LineTo(m_rtRoi_view.right, m_rtRoi_view.bottom);
			pDC->LineTo(m_rtRoi_view.left, m_rtRoi_view.bottom);
			pDC->LineTo(m_rtRoi_view.left, m_rtRoi_view.top);

			pDC->SelectObject(oldPen);
		}


		// ���� ����
		if( m_bIsInterest )
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB( 255, 0, 0) );    // ������ �� ����
			CPen* oldPen = pDC->SelectObject(&pen);

			int iRadius = 10;
			pDC->MoveTo(m_ptInterst_view.x - iRadius, m_ptInterst_view.y - iRadius );
			pDC->LineTo(m_ptInterst_view.x + iRadius, m_ptInterst_view.y - iRadius );
			pDC->LineTo(m_ptInterst_view.x + iRadius, m_ptInterst_view.y + iRadius );
			pDC->LineTo(m_ptInterst_view.x - iRadius, m_ptInterst_view.y + iRadius );
			pDC->LineTo(m_ptInterst_view.x - iRadius, m_ptInterst_view.y - iRadius );

			pDC->SelectObject(oldPen);

		}
	}
}


// CImageViewer �����Դϴ�.

#ifdef _DEBUG
void CImageViewer::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CImageViewer::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CImageViewer �޽��� ó�����Դϴ�.
void CImageViewer::invalidateView()
{
	//
	// Update scroll size
	//
	if( IsImage() )
	{
		CSize sizeTotal;
		sizeTotal.cx = (LONG)(m_dZoomFactor * GetImageWidth());
		sizeTotal.cy = (LONG)(m_dZoomFactor * GetImageHeight());
		SetScrollSizes(MM_TEXT, sizeTotal);
	}


	//
	// update graphic positions
	//
	// roi position
	double x, y;
	// 1st point
	if( !ConvertCoordinate(m_ptLBtnDown_image.x, m_ptLBtnDown_image.y, &x, &y, _BHC_DIR_IMAGE_TO_VIEW_ ) )
	{
		return;
	}
	m_ptLBtnDown_view = CPoint( (int)x, (int)y );


	// 2nd point
	if( !ConvertCoordinate(m_ptLBtnUp_image.x, m_ptLBtnUp_image.y, &x, &y, _BHC_DIR_IMAGE_TO_VIEW_ ) )
	{
		return;
	}
	m_ptLBtnUp_view = CPoint( (int)x, (int)y );

	// roi
	m_rtRoi_view = CRect(	min(m_ptLBtnDown_view.x, m_ptLBtnUp_view.x),
							min(m_ptLBtnDown_view.y, m_ptLBtnUp_view.y),
							max(m_ptLBtnDown_view.x, m_ptLBtnUp_view.x),
							max(m_ptLBtnDown_view.y, m_ptLBtnUp_view.y));


	// point of interest
	if( !ConvertCoordinate(m_ptInterst_image.x, m_ptInterst_image.y, &x, &y, _BHC_DIR_IMAGE_TO_VIEW_ ) )
	{
		return;
	}
	m_ptInterst_view = CPoint( (int)x, (int)y );



	Invalidate();
}


//
BOOL CImageViewer::LoadImage(CString strFilePath)
{
// 	CHAR strMB[1024];
// 	WideCharToMultiByte( CP_ACP, 0, strFilePath, -1, strMB, 1024, NULL, NULL );
// 
// 	m_image.Open( strMB );
	m_image.Open( strFilePath );

	invalidateView();

	return TRUE;
}


BOOL CImageViewer::SetImage( int iWidth, int iHeight, BYTE *pData)
{
	m_image.SetImage(pData, iWidth, iHeight);

// 	invalidateView();
	SendMessage(WM_INVALIDATE_VIWE);

	return TRUE;
}


BYTE *CImageViewer::GetImageBuffer()
{
	return m_image.GetImage();
}


int CImageViewer::GetImageWidth()
{
	if (GetImageBuffer())
	{
		return m_image.GetWidth();
	}

	return 0;
}


int CImageViewer::GetImageHeight()
{
	if (GetImageBuffer())
	{
		return m_image.GetHeight();
	}

	return 0;
}


BOOL CImageViewer::GetRoi(CRect *pRtRoi) 
{ 
	// ������ ������ 
	if( IsImage() == FALSE )
	{
		return FALSE;
	}

	// 
	if( m_bIsRoi )	// ������ ROI�� ���� ��
	{
		*pRtRoi = m_rtRoi_image;
	} 
	else 	// ������ ROI�� ���� ��
	{
		*pRtRoi = CRect( 0, 0, GetImageWidth() - 1, GetImageHeight() - 1 );
	}


	return TRUE; 
};


BOOL CImageViewer::GetPoi(CPoint *pt)
{
	if( m_bIsInterest == false )
	{
		return FALSE;
	}


	*pt = m_ptInterst_image;


	return TRUE;
}


void CImageViewer::SetDrawing(bool bDraw)
{
//	m_bDraw = bDraw;

	invalidateView();
}

LRESULT CImageViewer::OnReceive(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your control notification handler code here

//	if( wParam == WM_INVALIDATE_VIWE )
	{
		invalidateView();
	}

	return 0;
}


void CImageViewer::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	// Save old position
	m_ptLBtnDown_view_old = m_ptLBtnDown_view;
	m_ptLBtnDown_image_old = m_ptLBtnDown_image;


	// window ��ǥ
	CPoint ptScroll = GetScrollPosition();
	m_ptLBtnDown_view = CPoint(	point.x + ptScroll.x, 
								point.y + ptScroll.y );

	// ���� ��ǥ
	m_ptLBtnDown_image = CPoint(	(int)(m_ptLBtnDown_view.x/m_dZoomFactor), 
									(int)(m_ptLBtnDown_view.y/m_dZoomFactor) );


	CScrollView::OnLButtonDown(nFlags, point);
}


void CImageViewer::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// Save old position
	m_ptLBtnUp_view_old = m_ptLBtnUp_view;
	m_ptLBtnUp_image_old = m_ptLBtnUp_image;


	// window ��ǥ
	CPoint ptScroll = GetScrollPosition();
	m_ptLBtnUp_view = CPoint(	point.x + ptScroll.x, 
								point.y + ptScroll.y );

	// Lbutton down ��ȿȭ
	if( m_ptLBtnDown_view == m_ptLBtnUp_view )
	{
		m_ptLBtnDown_view	= m_ptLBtnDown_view_old;
		m_ptLBtnDown_image	= m_ptLBtnDown_image_old;

		return;
	}

	m_rtRoi_view = CRect(	min(m_ptLBtnDown_view.x, m_ptLBtnUp_view.x),
							min(m_ptLBtnDown_view.y, m_ptLBtnUp_view.y),
							max(m_ptLBtnDown_view.x, m_ptLBtnUp_view.x),
							max(m_ptLBtnDown_view.y, m_ptLBtnUp_view.y));



	// ���� ��ǥ
	m_ptLBtnUp_image = CPoint(	(int)(m_ptLBtnUp_view.x/m_dZoomFactor), 
								(int)(m_ptLBtnUp_view.y/m_dZoomFactor) );

	m_rtRoi_image = CRect(	min(m_ptLBtnDown_image.x, m_ptLBtnUp_image.x),
							min(m_ptLBtnDown_image.y, m_ptLBtnUp_image.y),
							max(m_ptLBtnDown_image.x, m_ptLBtnUp_image.x),
							max(m_ptLBtnDown_image.y, m_ptLBtnUp_image.y));

	//
	m_bIsRoi = true;

	invalidateView();

	CScrollView::OnLButtonUp(nFlags, point);
}


void CImageViewer::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// window ��ǥ
	CPoint ptScroll = GetScrollPosition();
	m_ptInterst_view = CPoint(	point.x + ptScroll.x, 
		point.y + ptScroll.y );

	// ���� ��ǥ
	m_ptInterst_image = CPoint(	(int)(m_ptInterst_view.x/m_dZoomFactor), 
		(int)(m_ptInterst_view.y/m_dZoomFactor) );


	m_bIsInterest = true;


	//
	SendMessage(WM_INVALIDATE_VIWE);


	CScrollView::OnRButtonDown(nFlags, point);
}



BOOL CImageViewer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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


//////////////////////////////////////////////////////////////////////////
//
//	ZoomIn
//
//	
//////////////////////////////////////////////////////////////////////////
void CImageViewer::ZoomIn()
{
	if( m_iZoom>=10 )
	{
		return;
	}

	m_iZoom++;
	m_dZoomFactor = pow( 2.0, m_iZoom);


	//
	SendMessage(WM_INVALIDATE_VIWE);
}


//////////////////////////////////////////////////////////////////////////
//
//	ZoomOut
//
//
//////////////////////////////////////////////////////////////////////////
void CImageViewer::ZoomOut()
{
	if( m_iZoom <= -10 )
	{
		return;
	}

	m_iZoom--;
	m_dZoomFactor = pow( 2.0, m_iZoom);


	//
	SendMessage(WM_INVALIDATE_VIWE);
}


//////////////////////////////////////////////////////////////////////////
//
//	IsImage
//
//	�ε��� ������ �ִ°�?
//
//
//////////////////////////////////////////////////////////////////////////
BOOL CImageViewer::IsImage()
{
	if( GetImageBuffer() )
	{
		return TRUE;
	}

	return FALSE;
}



//////////////////////////////////////////////////////////////////////////
//
//	ConvertCoordnate
//
//
//
//////////////////////////////////////////////////////////////////////////
BOOL CImageViewer::ConvertCoordinate( double dX_in, double dY_in, double *pdX_out, double *pdY_out, int iDir )
{
// 	if( !IsImage() )
// 	{
// 		return FALSE;
// 	}


	if( iDir == _BHC_DIR_IMAGE_TO_VIEW_ )	// image to view
	{
		*pdX_out = dX_in * m_dZoomFactor;
		*pdY_out = dY_in * m_dZoomFactor;
	}
	else if( iDir == _BHC_DIR_VIEW_TO_IMAGE_ )	// view to image
	{
		*pdX_out = dX_in / m_dZoomFactor;
		*pdY_out = dY_in / m_dZoomFactor;
	}
	else
	{
		return FALSE;
	}


	return TRUE;
}




