// GraphDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GraphDlg.h"
#include "afxdialogex.h"


// CGraphDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGraphDlg, CDialog)

CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphDlg::IDD, pParent)
, m_dAxis_xMin(0)
, m_dAxis_xMax(0)
, m_dAxis_yMin(0)
, m_dAxis_yMax(0)
, m_dGrid_x(0)
, m_dGrid_y(0)
, m_iRadius_point(1)
, m_bFilter(FALSE)
, m_iIndex_filter(0)
{
	m_pGraph = NULL;
	m_bInit = false;
}

CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X_MIN, m_dAxis_xMin);
	DDX_Text(pDX, IDC_EDIT_X_MAX, m_dAxis_xMax);
	DDX_Text(pDX, IDC_EDIT_Y_MIN, m_dAxis_yMin);
	DDX_Text(pDX, IDC_EDIT_Y_MAX, m_dAxis_yMax);
	DDX_Text(pDX, IDC_EDIT_X_GRID, m_dGrid_x);
	DDX_Text(pDX, IDC_EDIT_Y_GRID, m_dGrid_y);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_iRadius_point);
	DDX_Check(pDX, IDC_CHECK_FILTER, m_bFilter);
	DDX_Text(pDX, IDC_EDIT_INDEX_FILTER, m_iIndex_filter);
}


BEGIN_MESSAGE_MAP(CGraphDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CGraphDlg::OnBnClickedCancel)
	ON_WM_NCDESTROY()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_SET, &CGraphDlg::OnBnClickedButtonSet)
END_MESSAGE_MAP()


// CGraphDlg 메시지 처리기입니다.


void CGraphDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialog::OnCancel();

	DestroyWindow();

}


void CGraphDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete this;
}


int CGraphDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRuntimeClass  *pObject;
	pObject = RUNTIME_CLASS(CBHC_graph2D);
	m_pGraph = (CBHC_graph2D*)pObject->CreateObject();


	return 0;
}

void CGraphDlg::init()
{
	if( m_bInit == true )
	{
		return;
	}

	m_bInit = true;

	CRect rtGraph;
	GetDlgItem(IDC_STATIC_GRAPH)->GetWindowRect(&rtGraph);
	ScreenToClient(rtGraph);

	if (!m_pGraph->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		rtGraph, this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return;
	}


	m_pGraph->SetSize(rtGraph.Width(), rtGraph.Height() );
}


void CGraphDlg::ClearData()
{
	m_pGraph->ClearData();
}


void CGraphDlg::AddData( double dx, double dy, int iRed, int iGreen, int iBlue, int iIndex )
{
	m_pGraph->AddData( dx, dy, iRed, iGreen, iBlue, iIndex );
}


void CGraphDlg::SetAxis( double dX_min, double dX_max, double dY_min, double dY_max, double dXGrid, double dYGrid )
{
	m_pGraph->SetRange( dX_min, dX_max, dY_min, dY_max);

	m_pGraph->SetGrid(dXGrid, dYGrid);

	m_dAxis_xMin	= dX_min;
	m_dAxis_xMax	= dX_max;
	m_dGrid_x		= dXGrid;

	m_dAxis_yMin	= dY_min;
	m_dAxis_yMax	= dY_max;
	m_dGrid_y		= dYGrid;

	UpdateData(FALSE);
}


void CGraphDlg::OnBnClickedButtonSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	// 
	m_pGraph->SetRange( m_dAxis_xMin, m_dAxis_xMax, m_dAxis_yMin, m_dAxis_yMax);

	m_pGraph->SetGrid(m_dGrid_x, m_dGrid_y);


	// size
	m_pGraph->SetPointRadius(m_iRadius_point);


	//
	m_pGraph->SetFilter( GetUseFilter(), GetFilterIndex() );
}


BOOL CGraphDlg::GetUseFilter()
{
	if( ((CButton*)GetDlgItem(IDC_CHECK_FILTER))->GetCheck()== TRUE )
	{
		return TRUE;
	}

	return FALSE;
}


int CGraphDlg::GetFilterIndex()
{
	CString strIndex;
	GetDlgItemText(IDC_EDIT_INDEX_FILTER, strIndex);

	return atoi((CStringA)strIndex);
}
