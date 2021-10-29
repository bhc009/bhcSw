#pragma once

#include "StdAfx.h"
#include "common\BhImage.h"


// CImageViewer ���Դϴ�.
#define WM_INVALIDATE_VIWE		WM_USER+1

#define _BHC_DIR_IMAGE_TO_VIEW_	0
#define _BHC_DIR_VIEW_TO_IMAGE_	1

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
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP()


protected:

	BOOL IsImage();
	CBhImage m_image;

	void ZoomIn();
	void ZoomOut();
	int m_iZoom;	// zoom 2^x
	double m_dZoomFactor;

	BOOL ConvertCoordinate( double dX_in, double dY_in, double *pdX_out, double *pdY_out, int iDir );

	bool m_bIsRoi;

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


	CRect m_rtRoi_view;
	CRect m_rtRoi_image;

	// ���� point
	bool m_bIsInterest;
	CPoint m_ptInterst_view;
	CPoint m_ptInterst_image;


	void invalidateView();

public:
	BOOL LoadImage(CString strFilePath);
	BOOL SetImage( int iWidth, int iHeight, BYTE *pData);

	BYTE *GetImageBuffer();
	int GetImageWidth();
	int GetImageHeight();

	BOOL GetRoi(CRect *pRtRoi);
	BOOL GetPoi(CPoint *pt);

	void SetDrawing(bool bDraw);

};


