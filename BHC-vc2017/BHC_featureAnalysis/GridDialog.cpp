// GridDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BHC_featureAnalysis.h"
#include "GridDialog.h"
#include "afxdialogex.h"


// CGridDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFeatureInfoDlg, CDialog)

CFeatureInfoDlg::CFeatureInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFeatureInfoDlg::IDD, pParent)
{
	m_iWidth	= 0;
	m_iHeight	= 0;

	m_pImageViewer = NULL;
}

CFeatureInfoDlg::~CFeatureInfoDlg()
{
}

void CFeatureInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFeatureInfoDlg, CDialog)
	ON_WM_NCDESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CGridDialog 메시지 처리기입니다.
void CFeatureInfoDlg::init()
{
	{
		CRect rtVeiwRegion;
		GetDlgItem(IDC_STATIC_IMAGE_VIEWER)->GetWindowRect(&rtVeiwRegion);
		ScreenToClient(rtVeiwRegion);

		if (!m_pImageViewer->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
			rtVeiwRegion, this, AFX_IDW_PANE_FIRST, NULL))
		{
			TRACE0("Failed to create view window\n");
			return;
		}


// 		m_pImageViewer->LoadImage(_T("d:\\lena_color.bmp"));
		m_pImageViewer->LoadImage(_T(DEFAULT_FILE));
	}


	{
		CRect rtVeiwRegion;
		GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(&rtVeiwRegion);

		ScreenToClient(rtVeiwRegion);

		m_Grid.Create(rtVeiwRegion, this, IDC_STATIC_GRID);

		// 		initGrid();
	}

}


void CFeatureInfoDlg::SetSize(int iWidth, int iHeight)
{
	m_Grid.SetColumnCount(iWidth);
	m_Grid.SetRowCount(iHeight);


	// index 열/행 생성
	m_Grid.SetFixedColumnCount();
// 	m_Grid.SetFixedRowCount();


	// 색설정
	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));



	m_iWidth	= iWidth;
	m_iHeight	= iHeight;
}


bool CFeatureInfoDlg::SetData( int x, int y, std::string strContent )
{
	if( x >= m_iWidth || y>= m_iHeight || x<0 || y<0 )
	{
		return false;
	}

	std::string str = strContent;

	WCHAR wBuf[2048];
	stringToChar(&str, wBuf, 2048 );

	m_Grid.SetItemText( y, x, (LPCTSTR)wBuf );


	return true;
}


void CFeatureInfoDlg::Open( std::string str )
{
	std::string strPath_1 = str;

	WCHAR wBuf[2048];
	stringToChar(&strPath_1, wBuf, 2048 );

	CString strPath_2;
	strPath_2.Format(_T("%s"), wBuf);
	
	m_pImageViewer->LoadImage(strPath_2);
}


void CFeatureInfoDlg::Renew()
{
	m_Grid.Invalidate();
}


void CFeatureInfoDlg::stringToChar(std::string *str, TCHAR *ch, int iMaxSize )
{
	std::vector<char> writable(str->begin(), str->end());
	writable.push_back('\0');
	char* ptr = &writable[0];


	int len = MultiByteToWideChar( CP_ACP, 0, ptr, -1, NULL, 0 );
	// 	WCHAR wBuf[2048];
	MultiByteToWideChar( CP_ACP, 0, ptr, strlen(ptr)+1, ch, len );

}


void CFeatureInfoDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete this;
}


int CFeatureInfoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	{
		CRuntimeClass  *pObject;
		pObject = RUNTIME_CLASS(CImageViewer);
		m_pImageViewer = (CImageViewer*)pObject->CreateObject();
	}

	return 0;
}
