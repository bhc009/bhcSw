#pragma once


#include "../common/BHC_graph2D.h"

// CGraphDlg 대화 상자입니다.

class CGraphDlg : public CDialog
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	CGraphDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGraphDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GRAPHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNcDestroy();
	afx_msg void OnBnClickedButtonSet();




protected:
	CBHC_graph2D *m_pGraph;

	bool m_bInit;

	int m_iRadius_point;


	BOOL m_bFilter;
	int m_iIndex_filter;

	BOOL GetUseFilter();
	int GetFilterIndex();

public:
	void init();

	void ClearData();
	void AddData( double dx, double dy, int iRed, int iGreen, int iBlue, int iIndex );
	void SetAxis( double dX_min, double dX_max, double dY_min, double dY_max, double dXGrid, double dYGrid );

// 	double m_dGrid_x;
// 	double m_dAxis_yMin;
// 	double m_dAxis_yMax;
// 	double m_dGrid_y;
// 	double m_dAxis_xMin;
// 	double m_dAxis_xMax;
	double m_dAxis_xMin;
	double m_dAxis_xMax;
	double m_dAxis_yMin;
	double m_dAxis_yMax;
	double m_dGrid_x;
	double m_dGrid_y;
};
