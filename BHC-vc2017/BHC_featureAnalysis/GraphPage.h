#pragma once


#include "../common/BHC_graph2D.h"


// CGraphPage ��ȭ �����Դϴ�.

class CGraphPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGraphPage)

public:
	CGraphPage();
	virtual ~CGraphPage();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PAGE_GRAPH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()




public:
	void init(void);

	void SetMessage( CWnd *pTarget, UINT uiMessage);

	CBHC_graph2D* GetGraph(){ return m_pGraph;};


	void SetAxis( double dX_min, double dX_max, double dY_min, double dY_max, double dXGrid, double dYGrid );

protected:
	CBHC_graph2D *m_pGraph;

public:
	afx_msg void OnBnClickedButtonSet();
	double m_dAxisX_min;
	double m_dAxisX_max;
	double m_dAxisY_min;
	double m_dAxisY_max;
	UINT m_uiRadius;
	double m_dAxisX_grid;
	double m_dAxisY_grid;
};
