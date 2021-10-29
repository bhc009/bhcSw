#pragma once

#include "StdAfx.h"
#include "common\BhImage.h"


// CImageViewer 뷰입니다.

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


