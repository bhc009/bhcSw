#pragma once

#include "BHC_graph2D.h"

// CPageXYY ��ȭ �����Դϴ�.

class CPageXYY : public CDialogEx
{
	DECLARE_DYNAMIC(CPageXYY)

public:
	CPageXYY(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPageXYY();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PAGE_XYY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


public:
	CBHC_graph2D *m_pGraph;
	void init();
};
