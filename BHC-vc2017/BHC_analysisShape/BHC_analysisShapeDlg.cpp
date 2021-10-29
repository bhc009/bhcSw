
// BHC_analysisShapeDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "BHC_analysisShape.h"
#include "BHC_analysisShapeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CBHC_analysisShapeDlg ��ȭ ����




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


// CBHC_analysisShapeDlg �޽��� ó����

BOOL CBHC_analysisShapeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CBHC_analysisShapeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CBHC_analysisShapeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CBHC_analysisShapeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	CRuntimeClass  *pObject;
	pObject = RUNTIME_CLASS(CImageViewer);
	m_pImageViewer = (CImageViewer*)pObject->CreateObject();

	return 0;
}


void CBHC_analysisShapeDlg::OnBnClickedButtonLoadOrgImage()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strFilePath;

	if( GetFilePath( &strFilePath ) == false )
	{
		return;
	}


	m_pImageViewer->LoadImage(strFilePath, _BHC_IMAGEVIEWER_MAIN_IMAGE_);	// main image ����
}


void CBHC_analysisShapeDlg::OnBnClickedButtonLoadPrcImage()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strFilePath;

	if( GetFilePath( &strFilePath ) == false )
	{
		return;
	}


	m_pImageViewer->LoadImage(strFilePath, _BHC_IMAGEVIEWER_SUB_IMAGE_);	// ���� image ����
}


void CBHC_analysisShapeDlg::OnBnClickedButtonMakePrcImage()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//////////////////////////////////////////////////////////////////////////
//
//	OnBnClickedButtonAnalysis
//
//	���� ROI �������� feature ���� ���
//
//
//////////////////////////////////////////////////////////////////////////
void CBHC_analysisShapeDlg::OnBnClickedButtonAnalysis()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// mesh �˻� test
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileFind finder;

	BOOL bWorking = finder.FindFile(m_strDirectory+"/*.*");


	while(bWorking)
	{
		// ���� ����/������ �����Ѵٸ� true
		bWorking = finder.FindNextFile();

		// �����϶�
		if (finder.IsArchived())
		{
			CString fileName = finder.GetFileName();

			if (fileName == "." || fileName == "..")
			{
				continue;
			}

			//fileName = finder.GetFileTitle();

			//��ȯ�� char* ���� ����
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
//wchar_t ���� char ���� ����ȯ �Լ�
char * CBHC_analysisShapeDlg::ConvertWCtoC(wchar_t* str)
{
	//��ȯ�� char* ���� ����
	char* pStr;

	//�Է¹��� wchar_t ������ ���̸� ����
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	//char* �޸� �Ҵ�
	pStr = new char[strSize];

	//�� ��ȯ
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);

	return pStr;
}


