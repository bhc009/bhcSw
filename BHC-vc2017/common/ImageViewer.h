#pragma once

#include "StdAfx.h"
#include "BhImage.h"


// CImageViewer ���Դϴ�.
#define WM_INVALIDATE_VIWE		WM_USER+1

#define _BHC_DIR_IMAGE_TO_VIEW_	0
#define _BHC_DIR_VIEW_TO_IMAGE_	1

// Main image�� sub image ���п�
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
	CImageViewer();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CImageViewer();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual void OnInitialUpdate();     // ������ �� ó���Դϴ�.

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

	bool m_bIsRoi;		// ROI ����
	int m_iPoiWidth;	// ������ ǥ�� ��
	int m_iPoiHeight;	// ������ ǥ�� ����

	int m_iImageToShow;

	// ������ ��ǥ
	CPoint m_ptLBtnDown_view;	
	CPoint m_ptLBtnDown_view_old;	
	CPoint m_ptLBtnUp_view;
	CPoint m_ptLBtnUp_view_old;

	// ���� ��ǥ
	CPoint m_ptLBtnDown_image;
	CPoint m_ptLBtnDown_image_old;
	CPoint m_ptLBtnUp_image;
	CPoint m_ptLBtnUp_image_old;

	// Roi
	CRect m_rtRoi_view;		// view â���� ���� ���� ��ġ
	CRect m_rtRoi_image;	// image ���� ���� ���� ��ġ

	// ���� point
	bool m_bIsInterest;			// ������ ����
	CPoint m_ptInterst_view;	// view â���� ������ ��ġ
	CPoint m_ptInterst_image;	// ���󿡼� ������ ��ġ


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


