
// BHC_analysisShapeDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "BHC_analysisShape.h"
#include "BHC_analysisShapeDlg.h"
#include "afxdialogex.h"

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


// CBHC_analysisShapeDlg 대화 상자




CBHC_analysisShapeDlg::CBHC_analysisShapeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBHC_analysisShapeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBHC_analysisShapeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBHC_analysisShapeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_ORG_IMAGE, &CBHC_analysisShapeDlg::OnBnClickedButtonLoadOrgImage)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_PRC_IMAGE, &CBHC_analysisShapeDlg::OnBnClickedButtonLoadPrcImage)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_PRC_IMAGE, &CBHC_analysisShapeDlg::OnBnClickedButtonMakePrcImage)
	ON_BN_CLICKED(IDC_BUTTON_ANALYSIS, &CBHC_analysisShapeDlg::OnBnClickedButtonAnalysis)
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_LOAD, &CBHC_analysisShapeDlg::OnBnClickedButtonTemplateLoad)
	ON_BN_CLICKED(IDC_BUTTON__TEMPLATE_FIND, &CBHC_analysisShapeDlg::OnBnClickedButton)
	ON_BN_CLICKED(IDC_BUTTON_INSP_RIPPLE, &CBHC_analysisShapeDlg::OnBnClickedButtonInspRipple)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CBHC_analysisShapeDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_SET_DIRECTORY, &CBHC_analysisShapeDlg::OnBnClickedButtonSetDirectory)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CBHC_analysisShapeDlg::OnBnClickedButtonProcess)
END_MESSAGE_MAP()


// CBHC_analysisShapeDlg 메시지 처리기

BOOL CBHC_analysisShapeDlg::OnInitDialog()
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
	{
		CRect rtVeiwRegion;
		GetDlgItem(IDC_STATIC_IMAGE_VIEWER)->GetWindowRect(&rtVeiwRegion);
		ScreenToClient(rtVeiwRegion);

		if (!m_pImageViewer->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
			rtVeiwRegion, this, AFX_IDW_PANE_FIRST, NULL))
		{
			TRACE0("Failed to create view window\n");
			return -1;
		}


		m_pImageViewer->LoadImage(_T("d:\\BHC.bmp"));
	}

	//m_alg.Init_globalBuffer(8192, 20000);
	m_pyt.initialize("PyTest");


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBHC_analysisShapeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBHC_analysisShapeDlg::OnPaint()
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
HCURSOR CBHC_analysisShapeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CBHC_analysisShapeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRuntimeClass  *pObject;
	pObject = RUNTIME_CLASS(CImageViewer);
	m_pImageViewer = (CImageViewer*)pObject->CreateObject();

	return 0;
}


void CBHC_analysisShapeDlg::OnBnClickedButtonLoadOrgImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strFilePath;

	if( GetFilePath( &strFilePath ) == false )
	{
		return;
	}


	m_pImageViewer->LoadImage(strFilePath, _BHC_IMAGEVIEWER_MAIN_IMAGE_);	// main image 설정
}


void CBHC_analysisShapeDlg::OnBnClickedButtonLoadPrcImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strFilePath;

	if( GetFilePath( &strFilePath ) == false )
	{
		return;
	}


	m_pImageViewer->LoadImage(strFilePath, _BHC_IMAGEVIEWER_SUB_IMAGE_);	// 보조 image 설정
}


void CBHC_analysisShapeDlg::OnBnClickedButtonMakePrcImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


//////////////////////////////////////////////////////////////////////////
//
//	OnBnClickedButtonAnalysis
//
//	설정 ROI 영역에서 feature 정보 계산
//
//
//////////////////////////////////////////////////////////////////////////
void CBHC_analysisShapeDlg::OnBnClickedButtonAnalysis()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
/*
	//
	// get roi info
	//
	CRect rtRoi;
	m_pImageViewer->GetRoi( &rtRoi );


	//
	//
	//
	m_alg.SetShowImage(true);

	m_alg.ExtractFeature(	m_pImageViewer->GetImageBuffer(_BHC_IMAGEVIEWER_SUB_IMAGE_),
							m_pImageViewer->GetImageWidth(),
							m_pImageViewer->GetImageHeight(),
							rtRoi.left,
							rtRoi.top,
							rtRoi.right - 1,
							rtRoi.bottom - 1 );
							*/
}

bool CBHC_analysisShapeDlg::GetFilePath(CString *pStrFileName)
{
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("image files(*.bmp,*.tif,*.tiff)|*.bmp;*.tif;*.tiff|"),
		this);


	if (dlg.DoModal() == IDOK)
	{
		*pStrFileName = dlg.GetFolderPath() + "\\" + dlg.GetFileName();
	}
	else
	{
		return false;
	}


	return true;
}


void CBHC_analysisShapeDlg::OnBnClickedButtonTemplateLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	// 
// 	m_alg.LoadTemplate();


	//
	m_alg.LoadTemplate(	"d:\\ROI_template.htp",
						"d:\\ROI_AA.bmp",
						"d:\\ROI_BM.bmp",
						"d:\\ROI_cHole.bmp",
						"d:\\ROI_sHole.bmp");

	return;
}


void CBHC_analysisShapeDlg::OnBnClickedButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

/*
	//
	// 1 result
	//
	CString strFile;
	strFile.Format(_T("d:\\test\\ROI\\image.bmp"));

	CBhImage img;
	img.Open(strFile);

	CBhImage imgOut;
	imgOut.Create(img.GetWidth(), img.GetHeight(), 8, 0 );


	m_alg.m_bSave = true;
	m_alg.FindTemplate( img.GetImage(), img.GetWidth(), img.GetHeight(), imgOut.GetImage() );


	strFile.Format(_T("d:\\test\\ROI\\result.bmp"));
	imgOut.Save(strFile);
*/


	//
	// N result
	//
	CString strFile;
	strFile.Format(_T("d:\\test\\ROI\\FindTemplate - input.bmp"));

	CBhImage img;
	if (img.Open(strFile) == FALSE)
	{
		AfxMessageBox(_T("loading error"));
		return;
	}


	int iWidth	= img.GetWidth();
	int iHeight	= img.GetHeight();

	BYTE *ppImages[10];
	for (int i = 0; i < 10; i++)
	{
		ppImages[i] = new BYTE[iWidth*iHeight];
	}


	m_alg.m_bSave = true;
	m_alg.FindTemplate(img.GetImage(), img.GetWidth(), img.GetHeight(), ppImages[0], ppImages[1], ppImages[2], ppImages[3]);


	for (int i = 0; i < 10; i++)
	{
		delete[] ppImages[i];
	}

}


void CBHC_analysisShapeDlg::OnBnClickedButtonInspRipple()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iCnt=0;

	if( m_alg.FindRipple( m_pImageViewer->GetImageBuffer(), m_pImageViewer->GetImageWidth(), m_pImageViewer->GetImageHeight(), 
		1300, 5800, 6800, 19300, 
		10, 800, 0.5,
		&iCnt) == true )
	{
		BHC_MEMO("Ripple count is %d", iCnt);
	}

}


void CBHC_analysisShapeDlg::OnBnClickedButtonTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_alg.m_bSave = true;
// 	if (m_alg.PitchCompare(m_pImageViewer->GetImageBuffer(), 
// 		m_pImageViewer->GetImageWidth(), 
// 		m_pImageViewer->GetImageHeight(),
// 		10,
// 		40,
// 		NULL) == true)
// 	{
// 		BHC_MEMO("Pitch compare is done");
// 	}



	//
	// Make template
	//
	// set roi info
//	std::list<int> *pListX = new std::list<int>[4];
//	std::list<int> *pListY = new std::list<int>[4];
//
//	for (int i = 0; i < 4; i++)
//	{
//		pListX[i].push_back(1616);
//		pListX[i].push_back(2720);
//		pListX[i].push_back(2720);
//		pListX[i].push_back(1616);
//
//		pListY[i].push_back(4217);
//		pListY[i].push_back(4217);
//		pListY[i].push_back(4691);
//		pListY[i].push_back(4691);
//	}
//
//	m_alg.MakeTemplate(m_pImageViewer->GetImageBuffer(),
//		m_pImageViewer->GetImageWidth(),
//		m_pImageViewer->GetImageHeight(),
//		pListX,
//		pListY,
//		2000,
//		6000,
//		80,
//		7000,
//		10);
//
//
//	delete[] pListX;
//	delete[] pListY;


	//
	// mesh 검사 test
	//
//	int iScore = 0;
//
//	if (m_alg.InspectMesh(m_pImageViewer->GetImageBuffer(),
//		m_pImageViewer->GetImageWidth(),
//		m_pImageViewer->GetImageHeight(),
//		5, &iScore))
//	{
//		CString strMsg;
//		strMsg.Format(_T("Score = %d"), iScore);
//		AfxMessageBox(strMsg);
//		//int a = iScore;
//	}


	//
	// boundary processing
	//
//	m_alg.m_bSave = false;
//	m_alg.InspectBoundary(m_pImageViewer->GetImageBuffer(),
//		m_pImageViewer->GetImageWidth(),
//		m_pImageViewer->GetImageHeight(),
//		NULL,
//		NULL,
//		100,
//		20,
//		20);


	//
	// global variable test
	//
/*	CImageProcess_H m_alg1;

	int iWidth = 0;
	int iHeight = 0;
	m_alg1.Get_globalBuffer_size(iWidth, iHeight);

	CString str;
	str.Format(_T("%d x %d"), iWidth, iHeight);
	AfxMessageBox(str);
*/


	//
	// resize test
	//
/*	int iWidth = 0;
	int iHeight = 0;
	m_alg.ResizeImage(m_pImageViewer->GetImageBuffer(),
		m_pImageViewer->GetImageWidth(),
		m_pImageViewer->GetImageHeight(),
		8192,
		20000,
		1.04959,
		1.04959,
		-498,
		2513,
		NULL);
*/


//	CBhcPython pyt_1;
//	pyt_1.initialize("PyTest");

//	CBhcPython pyt_2;
//	pyt_2.initialize("PyTest");

/*
	int a = pyt_1.Get_global();
//	int b = pyt_2.Get_global();

	pyt_1.Set_global(100);
	int c = pyt_1.Get_global();
*/


	{
		int a = m_pyt.Get_member(0);
		//	int b = pyt_2.Get_global();

		m_pyt.Set_member(0,100);
		int c = m_pyt.Get_member(0);


		BHC_MEMO("1st module : %d, %d, %d", a, 0, c);
	}

	{
		int a = m_pyt.Get_member(1);
		//	int b = pyt_2.Get_global();

		m_pyt.Set_member(1,200);
		int c = m_pyt.Get_member(1);


		BHC_MEMO("2nd module : %d, %d, %d", a, 0, c);
	}
}


void CBHC_analysisShapeDlg::OnBnClickedButtonSetDirectory()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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


	m_strDirectory = pszPathname;
}


void CBHC_analysisShapeDlg::OnBnClickedButtonProcess()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileFind finder;

	BOOL bWorking = finder.FindFile(m_strDirectory+"/*.*");


	while(bWorking)
	{
		// 다음 파일/폴더가 존재한다면 true
		bWorking = finder.FindNextFile();

		// 파일일때
		if (finder.IsArchived())
		{
			CString fileName = finder.GetFileName();

			if (fileName == "." || fileName == "..")
			{
				continue;
			}

			//fileName = finder.GetFileTitle();

			//반환할 char* 변수 선언
			char* pStrPath = ConvertWCtoC(m_strDirectory.GetBuffer());
			char* pStrName = ConvertWCtoC(fileName.GetBuffer());

			// make bmp images
			//m_alg.MakeBmpImage(pStrPath, pStrName);

			// 
			//m_alg.Preprocessing_median(pStrPath, pStrName);
			m_alg.Preprocessing_normalize(pStrPath, pStrName);
		}
	}
}

///////////////////////////////////////////////////////////////////////
//wchar_t 에서 char 로의 형변환 함수
char * CBHC_analysisShapeDlg::ConvertWCtoC(wchar_t* str)
{
	//반환할 char* 변수 선언
	char* pStr;

	//입력받은 wchar_t 변수의 길이를 구함
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	//char* 메모리 할당
	pStr = new char[strSize];

	//형 변환
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);

	return pStr;
}


