// ImageViewer.cpp : 구현 파일입니다.
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
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_szWindow.cx = 100;
	m_szWindow.cy = 100;
	m_iZoom = 0;
	m_dZoomFactor = 1.0;

	m_bSubImage		= false;

	m_iImageToShow	= _BHC_IMAGEVIEWER_MAIN_IMAGE_;

	m_bIsRoi		= false;
	m_iPoiWidth		= 100;
	m_iPoiHeight	= 100;

	m_ptLBtnDown_view		= CPoint(0,0);
	m_ptLBtnDown_view_old	= CPoint(0,0);
	m_ptLBtnUp_view			= CPoint(0,0);
	m_ptLBtnUp_view_old		= CPoint(0,0);

	m_ptLBtnDown_image		= CPoint(0,0);
	m_ptLBtnDown_image_old	= CPoint(0,0);
	m_ptLBtnUp_image		= CPoint(0,0);
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
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CImageViewer 그리기입니다.

void CImageViewer::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImageViewer::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	if( IsImage() )
	{
		// 영상 표시
		switch( m_iImageToShow )
		{
			case _BHC_IMAGEVIEWER_MAIN_IMAGE_:
				m_image.Draw(	pDC->m_hDC,
								0, 0,
								(int)(m_dZoomFactor * GetImageWidth()),
								(int)(m_dZoomFactor * GetImageHeight()));
				break;

			case _BHC_IMAGEVIEWER_SUB_IMAGE_:
				m_imageSub.Draw(pDC->m_hDC,
								0, 0,
								(int)(m_dZoomFactor * GetImageWidth()),
								(int)(m_dZoomFactor * GetImageHeight()));
				break;

			default:
				m_image.Draw(	pDC->m_hDC,
								0, 0,
								(int)(m_dZoomFactor * GetImageWidth()),
								(int)(m_dZoomFactor * GetImageHeight()));
		}


		if (m_bIsRoi)
		{
			pDC->SetBkMode(TRANSPARENT);

			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(0,255,0) );    // 빨간색 펜 생성
			CPen* oldPen = pDC->SelectObject(&pen);

			pDC->SetTextColor(RGB(0, 255, 0));


			pDC->MoveTo(m_rtRoi_view.left, m_rtRoi_view.top);
			pDC->LineTo(m_rtRoi_view.right, m_rtRoi_view.top);
			pDC->LineTo(m_rtRoi_view.right, m_rtRoi_view.bottom);
			pDC->LineTo(m_rtRoi_view.left, m_rtRoi_view.bottom);
			pDC->LineTo(m_rtRoi_view.left, m_rtRoi_view.top);

			pDC->SelectObject(oldPen);
		}


		// 관심 영역
		if( m_bIsInterest )
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB( 255, 0, 0) );    // 빨간색 펜 생성
			CPen* oldPen = pDC->SelectObject(&pen);

			int iHalfWidth	= m_dZoomFactor * m_iPoiWidth/2;
			int iHalfHeight	= m_dZoomFactor * m_iPoiHeight/2;
			pDC->MoveTo(m_ptInterst_view.x - iHalfWidth, m_ptInterst_view.y - iHalfHeight );
			pDC->LineTo(m_ptInterst_view.x + iHalfWidth, m_ptInterst_view.y - iHalfHeight );
			pDC->LineTo(m_ptInterst_view.x + iHalfWidth, m_ptInterst_view.y + iHalfHeight );
			pDC->LineTo(m_ptInterst_view.x - iHalfWidth, m_ptInterst_view.y + iHalfHeight );
			pDC->LineTo(m_ptInterst_view.x - iHalfWidth, m_ptInterst_view.y - iHalfHeight );

			pDC->SelectObject(oldPen);

		}
	}
}


// CImageViewer 진단입니다.

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


// CImageViewer 메시지 처리기입니다.
void CImageViewer::invalidateView()
{
	//
	// Update scroll size
	//
// 	if( IsImage() )
// 	{
// 		CSize sizeTotal;
// 		sizeTotal.cx = (LONG)(m_dZoomFactor * GetImageWidth());
// 		sizeTotal.cy = (LONG)(m_dZoomFactor * GetImageHeight());
// 		SetScrollSizes(MM_TEXT, sizeTotal);
// 	}


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
BOOL CImageViewer::LoadImage(CString strFilePath, int iType )
{
#ifdef UNICODE
	switch( iType )
	{
	case _BHC_IMAGEVIEWER_MAIN_IMAGE_:
		if( m_image.Open( strFilePath ) == FALSE )
		{
			return FALSE;
		}

		ResetSubImage();	// 보조 영상은 무효화

		break;

	case _BHC_IMAGEVIEWER_SUB_IMAGE_:
		if( m_imageSub.Open( strFilePath ) == FALSE )
		{
			return FALSE;
		}

		if( SetSubImage(m_imageSub.GetWidth(), m_imageSub.GetHeight()) == false )
		{
			BHC_MEMO("error : SetSubImage > different size");
		}

		break;

	default:
		if( m_image.Open( strFilePath ) == FALSE )
		{
			return FALSE;
		}

		ResetSubImage();	// 보조 영상은 무효화
	}
#else
	CHAR strMB[1024];
	WideCharToMultiByte( CP_ACP, 0, strFilePath, -1, strMB, 1024, NULL, NULL );

	switch( iType )
	{
	case _BHC_IMAGEVIEWER_MAIN_IMAGE_:
		if( m_image.Open( strMB ) == FALSE )
		{
			return FALSE;
		}

		ResetSubImage();	// 보조 영상은 무효화

		break;

	case _BHC_IMAGEVIEWER_SUB_IMAGE_:
		if( m_imageSub.Open( strMB ) == FALSE )
		{
			return FALSE;
		}

		if( SetSubImage(m_imageSub.GetWidth(), m_imageSub.GetHeight()) == false )
		{
			BHC_MEMO("error : SetSubImage > different size");
		}

		break;

	default:
		if( m_image.Open( strMB ) == FALSE )
		{
			return FALSE;
		}

		ResetSubImage();	// 보조 영상은 무효화

	}
#endif


	CSize sizeTotal;
	sizeTotal.cx = (LONG)(m_dZoomFactor * GetImageWidth());
	sizeTotal.cy = (LONG)(m_dZoomFactor * GetImageHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);


	SendMessage(WM_INVALIDATE_VIWE);

	return TRUE;
}


BOOL CImageViewer::SetImage( BYTE *pData, int iWidth, int iHeight)
{
	m_image.SetImage(pData, iWidth, iHeight);


	ResetSubImage();	// 보조 영상은 무효화


	// invalidate view
	SendMessage(WM_INVALIDATE_VIWE);


	return TRUE;
}


BOOL CImageViewer::SetColorImage( BYTE *pData, int iWidth, int iHeight)
{
	m_image.SetColorImage(pData, iWidth, iHeight);


	m_bSubImage = false;	// 보조 영상은 무효화


	// invalidate view
	SendMessage(WM_INVALIDATE_VIWE);


	return TRUE;
}


BOOL CImageViewer::SetColorImage( int iWidth, int iHeight, BYTE *pData_red, BYTE *pData_green, BYTE *pData_blue)
{
	m_image.SetColorImage(pData_red, pData_green, pData_blue, iWidth, iHeight);


	ResetSubImage();	// 보조 영상은 무효화


	// invalidate view
	SendMessage(WM_INVALIDATE_VIWE);


	return TRUE;
}


BYTE *CImageViewer::GetImageBuffer(int iType)
{
	switch( iType )
	{
	case _BHC_IMAGEVIEWER_MAIN_IMAGE_:
		return m_image.GetImage();

	case _BHC_IMAGEVIEWER_SUB_IMAGE_:
		return m_imageSub.GetImage();
	}


	return m_image.GetImage();
}


int CImageViewer::GetImageWidth()
{
	if (GetImageBuffer())
	{
		//return m_image.GetWidth();
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
	// 영상이 없으면 
	if( IsImage() == FALSE )
	{
		return FALSE;
	}

	// 
	if( m_bIsRoi )	// 선정한 ROI가 있을 때
	{
		*pRtRoi = m_rtRoi_image;
	} 
	else 	// 선정한 ROI가 없을 때
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


BOOL CImageViewer::GetPoi(CRect *pRtPoi)
{
	if( m_bIsInterest == false )
	{
		return FALSE;
	}


	CRect rtRoi;
	rtRoi.left		= m_ptInterst_image.x - m_iPoiWidth/2;
	rtRoi.right		= m_ptInterst_image.x + m_iPoiWidth/2;
	rtRoi.top		= m_ptInterst_image.y - m_iPoiHeight/2;
	rtRoi.bottom	= m_ptInterst_image.y + m_iPoiHeight/2;

	*pRtPoi = rtRoi;


	return TRUE;
}


BOOL CImageViewer::SetPoiSize( int iWidth, int iHeight )
{
	m_iPoiWidth	 = iWidth;
	m_iPoiHeight = iHeight;


	SendMessage(WM_INVALIDATE_VIWE);

	return TRUE;
}



void CImageViewer::SetDrawing(bool bDraw)
{
//	m_bDraw = bDraw;

	invalidateView();
}


void CImageViewer::SetZoom(int iZoom)
{
	int iDir=0;
	if( iZoom > m_iZoom )
	{
		iDir = 1;
	} else {
		iDir = -1;
	}
	m_iZoom = iZoom;
	m_dZoomFactor = pow( 2.0, m_iZoom);

	CPoint ptScroll_old = GetScrollPosition();
	CPoint ptOldCenter = CPoint(	ptScroll_old.x + m_szWindow.cx/2,
									ptScroll_old.y + m_szWindow.cy/2);

	CSize sizeTotal;
	sizeTotal.cx = (LONG)(m_dZoomFactor * GetImageWidth());
	sizeTotal.cy = (LONG)(m_dZoomFactor * GetImageHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);


	// scroll
	CPoint ptSc;
	if(iDir==1)
	{
		ptSc.x = 2 * ptOldCenter.x - m_szWindow.cx/2;
		ptSc.y = 2 * ptOldCenter.y - m_szWindow.cy/2;
	} else {
		ptSc.x = 0.5 * ptOldCenter.x - m_szWindow.cx/2;
		ptSc.y = 0.5 * ptOldCenter.y - m_szWindow.cy/2;
	}


	// 예외처리
	if( ptSc.x + m_szWindow.cx >= sizeTotal.cx )
	{
		ptSc.x = sizeTotal.cx - m_szWindow.cx;
	}
	if( ptSc.x < 0 )
	{
		ptSc.x = 0;
	}

	if( ptSc.y + m_szWindow.cy >= sizeTotal.cy )
	{
		ptSc.y = sizeTotal.cy - m_szWindow.cy;
	}
	if( ptSc.y < 0 )
	{
		ptSc.y = 0;
	}


	SetScrollPos(SB_HORZ, ptSc.x );
	SetScrollPos(SB_VERT, ptSc.y );

	SendMessage(WM_INVALIDATE_VIWE);
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// Save old position
	m_ptLBtnDown_view_old = m_ptLBtnDown_view;
	m_ptLBtnDown_image_old = m_ptLBtnDown_image;


	// window 좌표
	CPoint ptScroll = GetScrollPosition();
	m_ptLBtnDown_view = CPoint(	point.x + ptScroll.x, 
								point.y + ptScroll.y );

	// 영상 좌표
	m_ptLBtnDown_image = CPoint(	(int)(m_ptLBtnDown_view.x/m_dZoomFactor), 
									(int)(m_ptLBtnDown_view.y/m_dZoomFactor) );


	CScrollView::OnLButtonDown(nFlags, point);
}


void CImageViewer::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// Lbutton down 무효화
	if( m_ptLBtnDown_view == point )
	{
		return;
	}


	// Save old position
	m_ptLBtnUp_view_old = m_ptLBtnUp_view;
	m_ptLBtnUp_image_old = m_ptLBtnUp_image;


	// window 좌표
	CPoint ptScroll = GetScrollPosition();
	m_ptLBtnUp_view = CPoint(	point.x + ptScroll.x, 
								point.y + ptScroll.y );


	m_rtRoi_view = CRect(	min(m_ptLBtnDown_view.x, m_ptLBtnUp_view.x),
							min(m_ptLBtnDown_view.y, m_ptLBtnUp_view.y),
							max(m_ptLBtnDown_view.x, m_ptLBtnUp_view.x),
							max(m_ptLBtnDown_view.y, m_ptLBtnUp_view.y));



	// 영상 좌표
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// window 좌표
	CPoint ptScroll = GetScrollPosition();
	m_ptInterst_view = CPoint(	point.x + ptScroll.x, 
		point.y + ptScroll.y );

	// 영상 좌표
	m_ptInterst_image = CPoint(	(int)(m_ptInterst_view.x/m_dZoomFactor), 
		(int)(m_ptInterst_view.y/m_dZoomFactor) );


	m_bIsInterest = true;


	//
	SendMessage(WM_INVALIDATE_VIWE);


	CScrollView::OnRButtonDown(nFlags, point);
}



BOOL CImageViewer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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


//////////////////////////////////////////////////////////////////////////
//
//	ZoomIn
//
//	
//////////////////////////////////////////////////////////////////////////
void CImageViewer::ZoomIn()
{
	if(IsImage()==FALSE)
	{
		return;
	}

	if( m_iZoom>=10 )
	{
		return;
	}


	SetZoom(m_iZoom+1);
}


//////////////////////////////////////////////////////////////////////////
//
//	ZoomOut
//
//
//////////////////////////////////////////////////////////////////////////
void CImageViewer::ZoomOut()
{
	if(IsImage()==FALSE)
	{
		return;
	}

	if( m_iZoom <= -10 )
	{
		return;
	}

	SetZoom(m_iZoom-1);
}


//////////////////////////////////////////////////////////////////////////
//
//	IsImage
//
//	로딩한 영상이 있는가?
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



void CImageViewer::ResetSubImage()
{
	m_bSubImage = false;

	m_iImageToShow = _BHC_IMAGEVIEWER_MAIN_IMAGE_;


	//
	SendMessage(WM_INVALIDATE_VIWE);
}


//////////////////////////////////////////////////////////////////////////
//
//	SetSubImage
//
//	보조 영상의 크기(폭, 높이)가 main 영상과 일치하면 등록한다.
//
//
//	ㅇ 
//		int iWidth, : 보조 영상의 폭
//		int iHeight	: 보조 영상의 높이
//
//////////////////////////////////////////////////////////////////////////
bool CImageViewer::SetSubImage(int iWidth, int iHeight)
{
	if( IsImage() == FALSE )
	{
		return false;
	}


	int iWidth_main		= GetImageWidth();	// main 영상의 폭
	int iHeight_main	= GetImageHeight();	// main 영상의 높이


	if( iWidth_main != iWidth || iHeight_main != iHeight )
	{
		return false;
	}

	m_bSubImage = true;


	return true;
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



void CImageViewer::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch( m_iImageToShow )
	{
	case _BHC_IMAGEVIEWER_MAIN_IMAGE_:
		if( m_bSubImage )	// 보조 영상이 있을 때,변경
		{
			m_iImageToShow = _BHC_IMAGEVIEWER_SUB_IMAGE_;
		}
		break;

	case _BHC_IMAGEVIEWER_SUB_IMAGE_:
		m_iImageToShow = _BHC_IMAGEVIEWER_MAIN_IMAGE_;
		break;
	}


	//
	SendMessage(WM_INVALIDATE_VIWE);


	CScrollView::OnLButtonDblClk(nFlags, point);
}


int CImageViewer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	m_szWindow.cx = lpCreateStruct->cx;
	m_szWindow.cy = lpCreateStruct->cy;

	return 0;
}
