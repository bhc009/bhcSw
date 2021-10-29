
// BHC_multiLightingDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "BHC_multiLighting.h"
#include "BHC_multiLightingDlg.h"
#include "afxdialogex.h"

// #include "../common/fmt/format.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBHC_multiLightingDlg 대화 상자




CBHC_multiLightingDlg::CBHC_multiLightingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBHC_multiLightingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_iZoom = 0;
	m_strFilePath = _T("d:\\");
	m_strFilePath = _T("D:\\data\\외관검사\\20200313-BM-color\\BM 결함\\CG-1 (추가 시료)\\1-3-2 (BM SC)");
// 	m_strFilePath = _T("d:\\test");

	m_iStep = 25;

	for( int i=0 ; i<_BHC_VIEW_NUMBER_ ; i++ )
	{
		m_pImageViewer[i] = NULL;
	}
}

void CBHC_multiLightingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBHC_multiLightingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SET_PATH, &CBHC_multiLightingDlg::OnBnClickedButtonSetPath)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMAGES, &CBHC_multiLightingDlg::OnBnClickedButtonLoadImages)
	ON_BN_CLICKED(IDC_BUTTON_GO_BEFORE, &CBHC_multiLightingDlg::OnBnClickedButtonGoBefore)
	ON_BN_CLICKED(IDC_BUTTON_GO_NEXT, &CBHC_multiLightingDlg::OnBnClickedButtonGoNext)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_OUT, &CBHC_multiLightingDlg::OnBnClickedButtonZoomOut)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_IN, &CBHC_multiLightingDlg::OnBnClickedButtonZoomIn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_INSPECTION, &CBHC_multiLightingDlg::OnBnClickedButtonInspection)
END_MESSAGE_MAP()


// CBHC_multiLightingDlg 메시지 처리기

BOOL CBHC_multiLightingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// gen image viewer
	for( int i=0 ; i<_BHC_VIEW_NUMBER_ ; i++ )
	{
		CRect rtVeiwRegion;

		CWnd *pViewDlg = GetDlgItemImageViewer(i);

		if( pViewDlg )
		{
			pViewDlg->GetWindowRect(&rtVeiwRegion);
			ScreenToClient(rtVeiwRegion);

			if (!m_pImageViewer[i]->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
				rtVeiwRegion, this, AFX_IDW_PANE_FIRST, NULL))
			{
				TRACE0("Failed to create view window\n");
				return -1;
			}

// 			m_pImageViewer[i]->LoadImage(_T("d:\\data\\sample.bmp"));
		}
	}



	//
	SetDlgItemInt(IDC_EDIT_ZOOM, m_iZoom);	// zoom
	SetDlgItemText(IDC_EDIT_FILE_PATH, m_strFilePath);
	SetDlgItemText(IDC_EDIT_KEY_WORD, _T("001"));

	SetTimer(NULL, 1000,NULL );

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBHC_multiLightingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBHC_multiLightingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBHC_multiLightingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBHC_multiLightingDlg::OnBnClickedButtonSetPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetFilePath();
}


void CBHC_multiLightingDlg::OnBnClickedButtonLoadImages()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LoadImages();
}


void CBHC_multiLightingDlg::OnBnClickedButtonGoBefore()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 
	// set key
	//
	CString strKey;
	GetKeyWord(&strKey);

	int iIndex_old = atoi((CStringA)strKey);

	int iIndex_new = iIndex_old - m_iStep;

	if( iIndex_new < 0 )
	{
		iIndex_new = 200 - 25;
	}

	SetKeyWord(iIndex_new);



	//
	//
	//
	LoadImages();
}


void CBHC_multiLightingDlg::OnBnClickedButtonGoNext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//
	//
	//
	CString strKey;
	GetKeyWord(&strKey);

	int iIndex_old = atoi((CStringA)strKey);

	int iIndex_new = iIndex_old + m_iStep;

	if( iIndex_new >= 200 )
	{
		iIndex_new = 1;
	}

	SetKeyWord(iIndex_new);


	//
	//
	//
	LoadImages();
}


int CBHC_multiLightingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	for( int i=0 ; i<_BHC_VIEW_NUMBER_ ; i++ )
	{
		CRuntimeClass  *pObject;
		pObject = RUNTIME_CLASS(CImageViewer);
		m_pImageViewer[i] = (CImageViewer*)pObject->CreateObject();
	}

	return 0;
}


CWnd *CBHC_multiLightingDlg::GetDlgItemImageViewer(int i)
{
	if( i<24 )
	{
		return GetDlgItem(IDC_STATIC_IMAGE_VIEWER_1 + i);
	}

	return NULL;
}


void CBHC_multiLightingDlg::SetZoom()
{

}


BOOL CBHC_multiLightingDlg::GetZoom()
{
	return FALSE;
}



void CBHC_multiLightingDlg::SetFilePath()
{
	ITEMIDLIST *pidlBrowse;
	TCHAR       pszPathname[MAX_PATH];
	BROWSEINFO  BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select Directory");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);
	if (pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
	}
	
	
	m_strFilePath = pszPathname;

	SetDlgItemText(IDC_EDIT_FILE_PATH, m_strFilePath);
}


BOOL CBHC_multiLightingDlg::GetFilePath(CString *pStrPath)
{
	GetDlgItemText(IDC_EDIT_FILE_PATH, *pStrPath);

	return TRUE;
}


BOOL CBHC_multiLightingDlg::GetKeyWord( CString *pStrKey )
{
	GetDlgItemText(IDC_EDIT_KEY_WORD, *pStrKey);

	return TRUE;
}


BOOL CBHC_multiLightingDlg::SetKeyWord( int iKey )
{
	CString strKeyWord;

	if( iKey<10 )
	{
		strKeyWord.Format(_T("00%d"), iKey);
	} else if( iKey<100 )
	{
		strKeyWord.Format(_T("0%d"), iKey);
	} else {
		strKeyWord.Format(_T("%d"), iKey);
	}


	//
	SetDlgItemText(IDC_EDIT_KEY_WORD, strKeyWord);


	return TRUE;
}


BOOL CBHC_multiLightingDlg::LoadImages()
{
	// key word
	CString strKeyWord_seed;
	if( !GetKeyWord(&strKeyWord_seed) )
	{
		return FALSE;
	}

	int iIndex_seed = atoi((CStringA)strKeyWord_seed);


	// path
	CString strPath;
	if( !GetFilePath(&strPath))
	{
		return FALSE;
	}


	// read
	for( int i=0 ; i<_BHC_VIEW_NUMBER_ - 1; i++ )
	{
		// key word
		CString strKeyWord;
		int iIndex_key = iIndex_seed + i;
		if( iIndex_key<10 )
		{
			strKeyWord.Format(_T("00%d.bmp"), iIndex_key);
		} else if( iIndex_key<100 )
		{
			strKeyWord.Format(_T("0%d.bmp"), iIndex_key);
		} else {
			strKeyWord.Format(_T("%d.bmp"), iIndex_key);
		}


		// find target file
		CString strFile;
		if( !FindTargetFile( strPath, strKeyWord, &strFile ) )
		{
			continue;
		}

		// load image
		if( m_pImageViewer[i] )
		{
			m_pImageViewer[i]->LoadImage(strFile);
		}
	}


	return TRUE;
}



void CBHC_multiLightingDlg::OnBnClickedButtonZoomOut()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDlgItemInt(IDC_EDIT_ZOOM, m_iZoom - 1);

	m_iZoom = GetDlgItemInt(IDC_EDIT_ZOOM);

	for( int i=0 ; i<_BHC_VIEW_NUMBER_ - 1; i++ )
	{
		m_pImageViewer[i]->SetZoom(m_iZoom);
	}

}


void CBHC_multiLightingDlg::OnBnClickedButtonZoomIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDlgItemInt(IDC_EDIT_ZOOM, m_iZoom + 1);

	m_iZoom = GetDlgItemInt(IDC_EDIT_ZOOM);

	for( int i=0 ; i<_BHC_VIEW_NUMBER_ - 1 ; i++ )
	{
		m_pImageViewer[i]->SetZoom(m_iZoom);
	}
}


void CBHC_multiLightingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint ptScroll = m_pImageViewer[0]->GetScrollPosition();

	for( int i=1 ; i<_BHC_VIEW_NUMBER_ - 1; i++ )
	{
		CPoint ptScroll_old = m_pImageViewer[i]->GetScrollPosition();

		if( ptScroll != ptScroll_old )
		{
			m_pImageViewer[i]->SetScrollPos(SB_HORZ, ptScroll.x);
			m_pImageViewer[i]->SetScrollPos(SB_VERT, ptScroll.y);

			m_pImageViewer[i]->SendMessage(WM_INVALIDATE_VIWE);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


//////////////////////////////////////////////////////////////////////////
//
//	FindTargetFile
//
//	입력 받은 폴더에서 입력받은 key를 포함하는 파일명을 검출한다.
//
//
///////////////////////////////////////////////////////////////////////////
BOOL CBHC_multiLightingDlg::FindTargetFile( CString strPath, CString strKey, CString *pStrFile )
{
	// 검색 폴더 선택
	CString strPath_reshape;
	strPath_reshape.Format(_T("%s\\*"), strPath );

	CFileFind fFinder;
	bool bFindFile = fFinder.FindFile(strPath_reshape);


	// 폴더내에서 파일 선택
	while (bFindFile)
	{
		bFindFile = fFinder.FindNextFile();

		// 디렉토리는 통과
		if( fFinder.IsDots() )
		{
			continue;
		}

		// 선택 파일
		CString strFile_interest = fFinder.GetFileName();

		// key 문자를 찾는다.
		int iFind = strFile_interest.Find(strKey);

		// key 문자를 찾으면 로딩 및 종료
		if( iFind >= 0 )
		{
			pStrFile->Format(_T("%s\\%s"), strPath, strFile_interest );

			return TRUE;
		}
	}


	return FALSE;
}

void CBHC_multiLightingDlg::OnBnClickedButtonInspection()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::string strFileName;
	std::ostringstream strStream;
	strStream<<"aaa"<<5;

	strFileName = strStream.str();


	//
	// Get image
	//
	int iWidth = m_pImageViewer[0]->GetImageWidth();
	int iHeight = m_pImageViewer[0]->GetImageHeight();

	int nImage = 8;
	BYTE **ppImages = new BYTE*[nImage];
	for( int i=0 ; i<nImage ; i++ )
	{
		BYTE *pImage_src = m_pImageViewer[i]->GetImageBuffer();

		ppImages[i] = new BYTE[3*iWidth*iHeight];
		memcpy( ppImages[i], pImage_src, 3*iWidth*iHeight);	// color
	}


	//
	//
	//
	CImageProcess_H alg;

// 	list<stDefectInfo> defectList;

// 	alg.Inspection( ppImages, iWidth, iHeight, &defectList );




	//
	//
	//
	for( int i=0 ; i<nImage ; i++)
	{
		delete[] ppImages[i];
	}
	delete[] ppImages;
}
