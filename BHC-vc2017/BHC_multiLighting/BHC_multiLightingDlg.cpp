
// BHC_multiLightingDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "BHC_multiLighting.h"
#include "BHC_multiLightingDlg.h"
#include "afxdialogex.h"

// #include "../common/fmt/format.h"

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


// CBHC_multiLightingDlg ��ȭ ����




CBHC_multiLightingDlg::CBHC_multiLightingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBHC_multiLightingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_iZoom = 0;
	m_strFilePath = _T("d:\\");
	m_strFilePath = _T("D:\\data\\�ܰ��˻�\\20200313-BM-color\\BM ����\\CG-1 (�߰� �÷�)\\1-3-2 (BM SC)");
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


// CBHC_multiLightingDlg �޽��� ó����

BOOL CBHC_multiLightingDlg::OnInitDialog()
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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CBHC_multiLightingDlg::OnPaint()
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
HCURSOR CBHC_multiLightingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBHC_multiLightingDlg::OnBnClickedButtonSetPath()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetFilePath();
}


void CBHC_multiLightingDlg::OnBnClickedButtonLoadImages()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LoadImages();
}


void CBHC_multiLightingDlg::OnBnClickedButtonGoBefore()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetDlgItemInt(IDC_EDIT_ZOOM, m_iZoom - 1);

	m_iZoom = GetDlgItemInt(IDC_EDIT_ZOOM);

	for( int i=0 ; i<_BHC_VIEW_NUMBER_ - 1; i++ )
	{
		m_pImageViewer[i]->SetZoom(m_iZoom);
	}

}


void CBHC_multiLightingDlg::OnBnClickedButtonZoomIn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetDlgItemInt(IDC_EDIT_ZOOM, m_iZoom + 1);

	m_iZoom = GetDlgItemInt(IDC_EDIT_ZOOM);

	for( int i=0 ; i<_BHC_VIEW_NUMBER_ - 1 ; i++ )
	{
		m_pImageViewer[i]->SetZoom(m_iZoom);
	}
}


void CBHC_multiLightingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
//	�Է� ���� �������� �Է¹��� key�� �����ϴ� ���ϸ��� �����Ѵ�.
//
//
///////////////////////////////////////////////////////////////////////////
BOOL CBHC_multiLightingDlg::FindTargetFile( CString strPath, CString strKey, CString *pStrFile )
{
	// �˻� ���� ����
	CString strPath_reshape;
	strPath_reshape.Format(_T("%s\\*"), strPath );

	CFileFind fFinder;
	bool bFindFile = fFinder.FindFile(strPath_reshape);


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
		CString strFile_interest = fFinder.GetFileName();

		// key ���ڸ� ã�´�.
		int iFind = strFile_interest.Find(strKey);

		// key ���ڸ� ã���� �ε� �� ����
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
