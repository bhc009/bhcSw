// GraphPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "BHC_featureAnalysis.h"
#include "GraphPage.h"
#include "afxdialogex.h"


// CGraphPage ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CGraphPage, CPropertyPage)

CGraphPage::CGraphPage()
	: CPropertyPage(CGraphPage::IDD)
	, m_dAxisX_min(0)
	, m_dAxisX_max(100)
	, m_dAxisY_min(0)
	, m_dAxisY_max(100)
	, m_uiRadius(1)
	, m_dAxisX_grid(0)
	, m_dAxisY_grid(0)
{
	m_pGraph = NULL;
}

CGraphPage::~CGraphPage()
{
}

void CGraphPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X_MIN, m_dAxisX_min);
	DDX_Text(pDX, IDC_EDIT_X_MAX, m_dAxisX_max);
	DDX_Text(pDX, IDC_EDIT_Y_MIN, m_dAxisY_min);
	DDX_Text(pDX, IDC_EDIT_Y_MAX, m_dAxisY_max);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_uiRadius);
	DDX_Text(pDX, IDC_EDIT_X_GRID, m_dAxisX_grid);
	DDX_Text(pDX, IDC_EDIT_Y_GRID, m_dAxisY_grid);
}


BEGIN_MESSAGE_MAP(CGraphPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CGraphPage::OnBnClickedButtonSet)
END_MESSAGE_MAP()


// CGraphPage �޽��� ó�����Դϴ�.

void CGraphPage::init(void)
{
	if( m_pGraph== NULL )
	{
		{
			CRuntimeClass  *pObject;
			pObject = RUNTIME_CLASS(CBHC_graph2D);
			m_pGraph = (CBHC_graph2D*)pObject->CreateObject();
		}




		CRect rtVeiwRegion;
		GetDlgItem(IDC_GRID_VIEW)->GetWindowRect(&rtVeiwRegion);
		ScreenToClient(rtVeiwRegion);

		if (!m_pGraph->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
			rtVeiwRegion, this, AFX_IDW_PANE_FIRST, NULL))
		{
			TRACE0("Failed to create view window\n");
		}

		m_pGraph->SetSize(rtVeiwRegion.Width(), rtVeiwRegion.Height());
	}
}



//////////////////////////////////////////////////////////////////////////
//
// ���콺 ���ʹ�ư Ŭ���� �߻� �޼������
//
//
//////////////////////////////////////////////////////////////////////////
void CGraphPage::SetMessage( CWnd *pTarget, UINT uiMessage)
{
	if(m_pGraph==NULL)
	{
		return;
	}


	m_pGraph->SetMessage(pTarget, uiMessage );
}


void CGraphPage::OnBnClickedButtonSet()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	// 
	m_pGraph->SetRange( m_dAxisX_min, m_dAxisX_max, m_dAxisY_min, m_dAxisY_max);


	m_pGraph->SetGrid(m_dAxisX_grid, m_dAxisY_grid);


	// size
	m_pGraph->SetPointRadius(m_uiRadius);


	//
// 	m_pGraph->SetFilter( GetUseFilter(), GetFilterIndex() );

}


void CGraphPage::SetAxis( double dX_min, double dX_max, double dY_min, double dY_max, double dXGrid, double dYGrid )
{
	m_pGraph->SetRange( dX_min, dX_max, dY_min, dY_max);

	m_pGraph->SetGrid(dXGrid, dYGrid);

	m_dAxisX_min	= dX_min;
	m_dAxisX_max	= dX_max;
	m_dAxisX_grid	= dXGrid;

	m_dAxisY_min	= dY_min;
	m_dAxisY_max	= dY_max;
	m_dAxisY_grid	= dYGrid;

	UpdateData(FALSE);
}