
// BHC_cropDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "BHC_crop.h"
#include "BHC_cropDlg.h"
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


// CBHC_cropDlg ��ȭ ����




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


// CBHC_cropDlg �޽��� ó����

BOOL CBHC_cropDlg::OnInitDialog()
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

	// ���� ROI ũ�� ����
	m_pView->SetPoiSize(100, 100);
	SetDlgItemInt(IDC_EDIT_ROI_WIDTH, 100);
	SetDlgItemInt(IDC_EDIT_ROI_HEIGHT, 100);

	//
	SetDlgItemText(IDC_EDIT_KEY_WORD, _T(".bmp") );

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CBHC_cropDlg::OnPaint()
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
HCURSOR CBHC_cropDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBHC_cropDlg::OnBnClickedButtonLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// �˻� ���� ����
	CString strPath_reshape;
	strPath_reshape.Format(_T("%s\\*"), pszPathname );

	CFileFind fFinder;
	bool bFindFile = fFinder.FindFile(strPath_reshape);

// 	CString strKey = _T(".bmp");
	CString strKey = GetDlgItem_keyWord();

	// ���������� ���� ����
	while (bFindFile)
	{
		bFindFile = fFinder.FindNextFile();

		// ���丮�� ���
		if( fFinder.IsDots() )
		{
			continue;
		}

		// ���� ����
		CString strFileName_interest = fFinder.GetFileName();

		// key ���ڸ� ã�´�.
		int iFind = strFileName_interest.Find(strKey);

		// key ���ڸ� ã���� �ε� �� ����
		if( iFind >= 0 )
		{
			CString strFile;
			strFile.Format(_T("%s\\%s"), pszPathname, strFileName_interest );


			// ���� ���� (BMP )
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

			// ���� ����(openCV)
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


			// ROI ��������
			CRect rtRoi;
			if( GetDlgItem_check_roi() )
			{
				if( m_pView->GetPoi(&rtRoi) == FALSE )
				{
					AfxMessageBox(_T("���� ROI �̼���"));

					return;
				}
			} else {
				m_pView->GetRoi(&rtRoi);
			}


			// ��� ���� �����
			CBhImage dstImg;
			dstImg.Create( rtRoi.Width(), rtRoi.Height(), 8, 1 );

			// ��� ���� ������ ����
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

			// ����
			CString strFileName_dst;
			strFileName_dst.Format(_T("%s\\crop_%s"), pszPathname, strFileName_interest);
			strFileName_dst.Replace(strKey, _T(".bmp"));
			dstImg.Save(strFileName_dst);
		}
	}


	AfxMessageBox(_T("�Ϸ�"));
}


int CBHC_cropDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	CRuntimeClass  *pObject;
	pObject = RUNTIME_CLASS(CImageViewer);
	m_pView = (CImageViewer*)pObject->CreateObject();

	return 0;
}


void CBHC_cropDlg::OnBnClickedButtonZoomOut()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pView->ZoomOut();
}


void CBHC_cropDlg::OnBnClickedButtonZoomIn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pView->SetPoiSize(	GetDlgItem_roi_width(),
							GetDlgItem_roi_height() );
}


CString CBHC_cropDlg::GetDlgItem_keyWord()
{
	CString str;

	GetDlgItemText(IDC_EDIT_KEY_WORD, str );


	return str;
}
