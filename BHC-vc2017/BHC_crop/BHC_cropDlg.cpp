
// BHC_cropDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "BHC_crop.h"
#include "BHC_cropDlg.h"
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


// CBHC_cropDlg 대화 상자




CBHC_cropDlg::CBHC_cropDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBHC_cropDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBHC_cropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBHC_cropDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CBHC_cropDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_FOLDER, &CBHC_cropDlg::OnBnClickedButtonLoadFolder)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_OUT, &CBHC_cropDlg::OnBnClickedButtonZoomOut)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_IN, &CBHC_cropDlg::OnBnClickedButtonZoomIn)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_FOLDER_ALL, &CBHC_cropDlg::OnBnClickedButtonLoadFolderAll)
	ON_BN_CLICKED(IDC_BUTTON_SET_ROI_SIZE, &CBHC_cropDlg::OnBnClickedButtonSetRoiSize)
END_MESSAGE_MAP()


// CBHC_cropDlg 메시지 처리기

BOOL CBHC_cropDlg::OnInitDialog()
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
	{
		CRect rtVeiwRegion;
		GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(&rtVeiwRegion);
		ScreenToClient(rtVeiwRegion);

		if (!m_pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
			rtVeiwRegion, this, AFX_IDW_PANE_FIRST, NULL))
		{
			TRACE0("Failed to create view window\n");
			return -1;
		}
	}

	// 고정 ROI 크기 설정
	m_pView->SetPoiSize(100, 100);
	SetDlgItemInt(IDC_EDIT_ROI_WIDTH, 100);
	SetDlgItemInt(IDC_EDIT_ROI_HEIGHT, 100);

	//
	SetDlgItemText(IDC_EDIT_KEY_WORD, _T(".bmp") );

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBHC_cropDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBHC_cropDlg::OnPaint()
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
HCURSOR CBHC_cropDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBHC_cropDlg::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("image files(*.bmp,*.tif,*.tiff)|*.bmp;*.tif;*.tiff|"),
		this);


	if (dlg.DoModal() == IDOK)
	{
		CString strFile;
		strFile = dlg.GetFolderPath() + "\\" + dlg.GetFileName();

		//m_pView->LoadImage(strFile);

		char cFile[1024];
		WideCharToMultiByte( CP_ACP, 0, strFile, -1, cFile, 1024, NULL, NULL );

		std::string strFile2;
		std::ostringstream strStream;
		strStream<<cFile;
		strFile2 = strStream.str();


		cv::Mat cvImage = cv::imread(strFile2, cv::IMREAD_COLOR | cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR );

		switch( cvImage.type() )
		{
		case CV_8UC1:	// 8bit color
			m_pView->SetImage( cvImage.data, cvImage.cols, cvImage.rows );
			break;

		case CV_8UC3:	// 8bit color
			m_pView->SetColorImage( cvImage.data, cvImage.cols, cvImage.rows );
			break;

		default:
			AfxMessageBox(_T("loading error"));
			return;
		}		

	}
}


void CBHC_cropDlg::OnBnClickedButtonLoadFolder()
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

	for (int i = 1; i < 999; i++)
	{
		CString strFileName;
		strFileName.Format(_T("%s\\%d.bmp"), pszPathname, i);

		CBhImage srcImg;
		if (!srcImg.Open(strFileName))
		{
			continue;
		}


		//
		CRect rtRoi;
		m_pView->GetRoi(&rtRoi);

		CBhImage dstImg;
		dstImg.Create( rtRoi.Width(), rtRoi.Height(), 8, 1 );


		BYTE *pSrc = srcImg.GetImage();
		BYTE *pDst = dstImg.GetImage();

		for (int y = 0; y < dstImg.GetHeight(); y++)
		{
			for (int x = 0; x < dstImg.GetWidth(); x++)
			{
				pDst[x + y*dstImg.GetWidthStep() ] = pSrc[rtRoi.left + x + (rtRoi.top + y)*srcImg.GetWidthStep() ];
			}
		}


		CString strFileName_dst;
		strFileName_dst.Format(_T("%s\\crop_%d.bmp"), pszPathname, i);
		dstImg.Save(strFileName_dst);
	}
}


void CBHC_cropDlg::OnBnClickedButtonLoadFolderAll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//
	// select directory
	//
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


	//
	// find file & crop
	//
	// 검색 폴더 선택
	CString strPath_reshape;
	strPath_reshape.Format(_T("%s\\*"), pszPathname );

	CFileFind fFinder;
	bool bFindFile = fFinder.FindFile(strPath_reshape);

// 	CString strKey = _T(".bmp");
	CString strKey = GetDlgItem_keyWord();

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
		CString strFileName_interest = fFinder.GetFileName();

		// key 문자를 찾는다.
		int iFind = strFileName_interest.Find(strKey);

		// key 문자를 찾으면 로딩 및 종료
		if( iFind >= 0 )
		{
			CString strFile;
			strFile.Format(_T("%s\\%s"), pszPathname, strFileName_interest );


			// 영상 열고 (BMP )
// 			CBhImage srcImg;
// 			if (!srcImg.Open(strFile))
// 			{
// 				continue;
// 			}
// 
// 			if( (m_pView->GetImageWidth()	!= srcImg.GetWidth()) ||
// 				(m_pView->GetImageHeight()	!= srcImg.GetHeight()) )
// 			{
// 				continue;
// 			}

			// 영상 열고(openCV)
			// unicode to multi-byte
			char cFile[1024];
			WideCharToMultiByte( CP_ACP, 0, strFile, -1, cFile, 1024, NULL, NULL );

			// convert type
			std::string strFile2;
			std::ostringstream strStream;
			strStream<<cFile;
			strFile2 = strStream.str();

			// read file
			cv::Mat cvImage = cv::imread(strFile2, cv::IMREAD_GRAYSCALE | cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR );
			int aaa = CV_8UC1;


			// ROI 가져오고
			CRect rtRoi;
			if( GetDlgItem_check_roi() )
			{
				if( m_pView->GetPoi(&rtRoi) == FALSE )
				{
					AfxMessageBox(_T("고정 ROI 미설정"));

					return;
				}
			} else {
				m_pView->GetRoi(&rtRoi);
			}


			// 결과 영상 만들고
			CBhImage dstImg;
			dstImg.Create( rtRoi.Width(), rtRoi.Height(), 8, 1 );

			// 결과 영상에 데이터 쓰고
// 			BYTE *pSrc = srcImg.GetImage();
			BYTE *pSrc = cvImage.data;
			BYTE *pDst = dstImg.GetImage();

			for (int y = 0; y < dstImg.GetHeight(); y++)
			{
				for (int x = 0; x < dstImg.GetWidth(); x++)
				{
					pDst[x + y*dstImg.GetWidthStep() ] = pSrc[rtRoi.left + x + (rtRoi.top + y)*cvImage.cols ];
				}
			}

			// 저장
			CString strFileName_dst;
			strFileName_dst.Format(_T("%s\\crop_%s"), pszPathname, strFileName_interest);
			strFileName_dst.Replace(strKey, _T(".bmp"));
			dstImg.Save(strFileName_dst);
		}
	}


	AfxMessageBox(_T("완료"));
}


int CBHC_cropDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRuntimeClass  *pObject;
	pObject = RUNTIME_CLASS(CImageViewer);
	m_pView = (CImageViewer*)pObject->CreateObject();

	return 0;
}


void CBHC_cropDlg::OnBnClickedButtonZoomOut()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pView->ZoomOut();
}


void CBHC_cropDlg::OnBnClickedButtonZoomIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pView->ZoomIn();
}


bool CBHC_cropDlg::GetDlgItem_check_roi()
{
	if( ((CButton*)GetDlgItem(IDC_CHECK_ROI_SIZE))->GetCheck()== TRUE )
	{
		return true;
	}

	return false;
}


int CBHC_cropDlg::GetDlgItem_roi_width()
{
	return GetDlgItemInt(IDC_EDIT_ROI_WIDTH);
}


int CBHC_cropDlg::GetDlgItem_roi_height()
{
	return GetDlgItemInt(IDC_EDIT_ROI_HEIGHT);
}


void CBHC_cropDlg::OnBnClickedButtonSetRoiSize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pView->SetPoiSize(	GetDlgItem_roi_width(),
							GetDlgItem_roi_height() );
}


CString CBHC_cropDlg::GetDlgItem_keyWord()
{
	CString str;

	GetDlgItemText(IDC_EDIT_KEY_WORD, str );


	return str;
}
