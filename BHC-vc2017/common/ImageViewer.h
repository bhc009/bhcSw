#pragma once

#include "StdAfx.h"
#include "BhImage.h"


// CImageViewer 뷰입니다.
#define WM_INVALIDATE_VIWE		WM_USER+1

#define _BHC_DIR_IMAGE_TO_VIEW_	0
#define _BHC_DIR_VIEW_TO_IMAGE_	1

// Main image와 sub image 구분용
#define _BHC_IMAGEVIEWER_MAIN_IMAGE_	0
#define _BHC_IMAGEVIEWER_SUB_IMAGE_		1

#ifdef UNICODE
#define BHC_MEMO(logformat, ...)\
	{ \
	char logbuf[2048]; \
	int ret = _snprintf_s(logbuf, 2048, _TRUNCATE, logformat, ##__VA_ARGS__); \
	int len = MultiByteToWideChar( CP_ACP, 0, logbuf, -1, NULL, 0 ); \
	WCHAR wBuf[2048]; \
	MultiByteToWideChar( CP_ACP, 0, logbuf, strlen(logbuf)+1, wBuf, len ); \
	OutputDebugString(wBuf); \
	};
#else
#define BHC_MEMO(logformat, ...)\
	{ \
	char logbuf[2048]; \
	int ret = _snprintf_s(logbuf, 2048, _TRUNCATE, logformat, ##__VA_ARGS__); \
	OutputDebugString(logbuf); \
	};
#endif


class CImageViewer : public CScrollView
{
	DECLARE_DYNCREATE(CImageViewer)

protected:
	CImageViewer();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CImageViewer();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	afx_msg LRESULT OnReceive( WPARAM, LPARAM );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP()


protected:

	CBhImage m_image;		// main image
	BOOL IsImage();

	CBhImage m_imageSub;	// main image
	void ResetSubImage();
	bool SetSubImage(int iWidth, int iHeight);
	bool m_bSubImage;

	CSize m_szWindow;
	int m_iZoom;	// zoom 2^x
	double m_dZoomFactor;

	BOOL ConvertCoordinate( double dX_in, double dY_in, double *pdX_out, double *pdY_out, int iDir );

	bool m_bIsRoi;		// ROI 여부
	int m_iPoiWidth;	// 관심점 표시 폭
	int m_iPoiHeight;	// 관심점 표시 높이

	int m_iImageToShow;

	// 윈도우 좌표
	CPoint m_ptLBtnDown_view;	
	CPoint m_ptLBtnDown_view_old;	
	CPoint m_ptLBtnUp_view;
	CPoint m_ptLBtnUp_view_old;

	// 영상 좌표
	CPoint m_ptLBtnDown_image;
	CPoint m_ptLBtnDown_image_old;
	CPoint m_ptLBtnUp_image;
	CPoint m_ptLBtnUp_image_old;

	// Roi
	CRect m_rtRoi_view;		// view 창에서 관심 영역 위치
	CRect m_rtRoi_image;	// image 에서 관심 영역 위치

	// 관심 point
	bool m_bIsInterest;			// 관심점 여부
	CPoint m_ptInterst_view;	// view 창에서 관심점 위치
	CPoint m_ptInterst_image;	// 영상에서 관심점 위치


	void invalidateView();

public:
	BOOL LoadImage(CString strFilePath, int iType = _BHC_IMAGEVIEWER_MAIN_IMAGE_);
	BOOL SetImage( BYTE *pData, int iWidth, int iHeight );
	BOOL SetColorImage( BYTE *pData, int iWidth, int iHeight );
	BOOL SetColorImage( int iWidth, int iHeight, BYTE *pData_red, BYTE *pData_green, BYTE *pData_blue);

	BYTE *GetImageBuffer(int iType = _BHC_IMAGEVIEWER_MAIN_IMAGE_);
	int GetImageWidth();
	int GetImageHeight();

	BOOL GetRoi(CRect *pRtRoi);
	BOOL GetPoi(CPoint *pt);
	BOOL GetPoi(CRect *pRtPoi);
	BOOL SetPoiSize( int iWidth, int iHeight );

	void SetDrawing(bool bDraw);

	void SetZoom(int iZoom);
	void ZoomIn();
	void ZoomOut();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


