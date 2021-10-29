// PageXYY.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BHC_colorSpace.h"
#include "PageXYY.h"
#include "afxdialogex.h"


// CPageXYY 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPageXYY, CDialogEx)

CPageXYY::CPageXYY(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageXYY::IDD, pParent)
{
	m_pGraph = NULL;
}

CPageXYY::~CPageXYY()
{
}

void CPageXYY::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageXYY, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CPageXYY 메시지 처리기입니다.


int CPageXYY::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRuntimeClass  *pObject;
	pObject = RUNTIME_CLASS(CBHC_graph2D);
	m_pGraph = (CBHC_graph2D*)pObject->CreateObject();

	return 0;
}


void CPageXYY::init()
{
	{
		CRect rtVeiwRegion;
		GetDlgItem(IDC_STATIC_GRAPH)->GetWindowRect(&rtVeiwRegion);
		ScreenToClient(rtVeiwRegion);

		if (!m_pGraph->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
			rtVeiwRegion, this, AFX_IDW_PANE_FIRST, NULL))
		{
			TRACE0("Failed to create view window\n");
		}

		m_pGraph->SetSize(rtVeiwRegion.Width(), rtVeiwRegion.Height());
	}

}
