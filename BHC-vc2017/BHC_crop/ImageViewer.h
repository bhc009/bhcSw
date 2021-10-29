#pragma once

#include "StdAfx.h"
#include "common\BhImage.h"


// CImageViewer ���Դϴ�.

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

	DECLARE_MESSAGE_MAP()


protected:

	CBhImage m_image;


	bool m_bValidRoi;
	CPoint m_ptLBtnDown;
	CPoint m_ptLBtnUp;
	CRect m_rtRoi;

	void invalidateView();

public:
	BOOL LoadImage(CString strFilePath);

	BYTE *GetImageBuffer();
	int GetImageWidth();
	int GetImageHeight();

	CRect GetRoi() { return m_rtRoi; };

	void SetDrawing(bool bDraw);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


