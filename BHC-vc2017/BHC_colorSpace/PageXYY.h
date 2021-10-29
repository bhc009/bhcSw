#pragma once

#include "BHC_graph2D.h"

// CPageXYY 대화 상자입니다.

class CPageXYY : public CDialogEx
{
	DECLARE_DYNAMIC(CPageXYY)

public:
	CPageXYY(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPageXYY();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PAGE_XYY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


public:
	CBHC_graph2D *m_pGraph;
	void init();
};
