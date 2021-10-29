
// BHC_multiImageDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "BHC_multiImage.h"
#include "BHC_multiImageDlg.h"
#include "afxdialogex.h"

#define _TIMER_VIEW_		0
#define _TIMER_PROGRESS_	1

// ���� �б� ����
#include <iostream>
#include <fstream>
using namespace std;

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


// CBHC_multiImageDlg ��ȭ ����

// ���� thread
UINT threadProcInsp(LPVOID lParam)
{
	CBHC_multiImageDlg *pMain = (CBHC_multiImageDlg*)lParam;

	while (TRUE)
	{
		//
		WaitForSingleObject(pMain->m_hEventInsp, INFINITE);

		CString strDirectory;
		list<CString> listDir;

		if( pMain->SelectDirecctory(&strDirectory) )
		{
			pMain->GetDirectoryList(strDirectory, &listDir );

			for( list<CString>::iterator ite = listDir.begin() ; ite != listDir.end() ; ite++ )
			{
				CString strPath;
				strPath.Format(_T("%s\\%s"),strDirectory, *ite );
				pMain->InspectDirectory(strPath);
			}
		}

		//
		ResetEvent(pMain->m_hEventInsp);
	}

	return 0;
}




CBHC_multiImageDlg::CBHC_multiImageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBHC_multiImageDlg::IDD, pParent)
	, m_strFileName_pre(_T("No"))
	, m_strFileName_last(_T("00"))
	, m_iNumImage(108)
	, m_iNumViewer(5)
	, m_iZoom(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_iLightColor = _BHC_WHITE_;


	for( int i=0 ; i<_BHC_LIGHTING_CONDITION_NUM_ ; i++)
	{
		m_aLightingCondition[i] = _BHC_WHITE_;
	}

	for( int i=0 ; i<_BHC_LIGHTING_CONDITION_TABLE_NUM_ ; i++)
	{
		for( int k=0 ; k<_BHC_LIGHTING_CONDITION_NUM_ ; k++)
		{
			m_aLightingConditionTable[i][k] = 0;;
		}
	}

	m_dProgress_ratio = 0.0;

	m_imageProcessing[0].initialize(4096, 3072,3);

	m_hEventInsp = CreateEventW(NULL, TRUE, FALSE, NULL);
	m_hThreadInsp = AfxBeginThread(threadProcInsp, (LPVOID)this);

}

void CBHC_multiImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILE_NAME_1, m_strFileName_pre);
	DDX_Text(pDX, IDC_EDIT_FILE_NAME_2, m_strFileName_last);
	DDX_Control(pDX, IDC_COMBO1, m_comboData);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
	DDX_Control(pDX, IDC_LIST_MEMO, m_ctrlMemo);
	DDX_Control(pDX, IDC_COMBO_IMAGE_NUM, m_ctrImageNum_graph);
}

BEGIN_MESSAGE_MAP(CBHC_multiImageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_SET_PATH_1, &CBHC_multiImageDlg::OnBnClickedButtonSetPath1)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CBHC_multiImageDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SET_PATH_2, &CBHC_multiImageDlg::OnBnClickedButtonSetPath2)
	ON_BN_CLICKED(IDC_BUTTON_SET_PATH_3, &CBHC_multiImageDlg::OnBnClickedButtonSetPath3)
	ON_BN_CLICKED(IDC_BUTTON_SET_PATH_4, &CBHC_multiImageDlg::OnBnClickedButtonSetPath4)
	ON_BN_CLICKED(IDC_BUTTON_SET_PATH_5, &CBHC_multiImageDlg::OnBnClickedButtonSetPath5)
	ON_BN_CLICKED(IDC_BUTTON_SET_PATH_6, &CBHC_multiImageDlg::OnBnClickedButtonSetPath6)
	ON_BN_CLICKED(IDC_BUTTON_SET_PATH_7, &CBHC_multiImageDlg::OnBnClickedButtonSetPath7)
	ON_BN_CLICKED(IDC_BUTTON_SET_PATH_8, &CBHC_multiImageDlg::OnBnClickedButtonSetPath8)
	ON_BN_CLICKED(IDC_BUTTON_SET_ZOOM, &CBHC_multiImageDlg::OnBnClickedButtonSetZoom)
	ON_BN_CLICKED(IDC_BUTTON_SET_IMAGE_NUMBER, &CBHC_multiImageDlg::OnBnClickedButtonSetImageNumber)
	ON_BN_CLICKED(IDC_BUTTON_SET_IMAGE_VIEWER, &CBHC_multiImageDlg::OnBnClickedButtonSetImageViewer)
	ON_BN_CLICKED(IDC_BUTTON_SET_ZOOM_IN, &CBHC_multiImageDlg::OnBnClickedButtonSetZoomIn)
	ON_BN_CLICKED(IDC_BUTTON_SET_ZOOM_OUT, &CBHC_multiImageDlg::OnBnClickedButtonSetZoomOut)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_LIGHTING_CONDITION, &CBHC_multiImageDlg::OnBnClickedButtonLoadLightingCondition)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CBHC_multiImageDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_BEFORE, &CBHC_multiImageDlg::OnBnClickedButtonBefore)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_LIGHTING_CONDITION2, &CBHC_multiImageDlg::OnBnClickedButtonUpdateLightingCondition2)
	ON_BN_CLICKED(IDC_BUTTON_INSP_ROI, &CBHC_multiImageDlg::OnBnClickedButtonInspRoi)
	ON_BN_CLICKED(IDC_BUTTON_INSP, &CBHC_multiImageDlg::OnBnClickedButtonInsp)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_DATA, &CBHC_multiImageDlg::OnBnClickedButtonShowData)
	ON_BN_CLICKED(IDC_BUTTON_INSP_FOLDER, &CBHC_multiImageDlg::OnBnClickedButtonInspFolder)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CBHC_multiImageDlg �޽��� ó����

BOOL CBHC_multiImageDlg::OnInitDialog()
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

		CWnd *pViewDlg = GetDlgItemViewer(i);

		if( pViewDlg )
		{
			pViewDlg->GetWindowRect(&rtVeiwRegion);
			ScreenToClient(rtVeiwRegion);

			if (!m_pImageView[i]->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
				rtVeiwRegion, this, AFX_IDW_PANE_FIRST, NULL))
			{
				TRACE0("Failed to create view window\n");
				return -1;
			}

// 			m_pImageView[i]->LoadImageW(_T("d:\\1_1.bmp"));
		}
	}


	// gen sub image viewer
	for( int i=0 ; i<_BHC_VIEW_NUMBER_ ; i++ )
	{
		CRect rtVeiwRegion;

		CWnd *pViewDlg = GetDlgItemSubViewer(i);

		if( pViewDlg )
		{
			pViewDlg->GetWindowRect(&rtVeiwRegion);
			ScreenToClient(rtVeiwRegion);

			if (!m_pImageView_sub[i]->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
				rtVeiwRegion, this, AFX_IDW_PANE_FIRST, NULL))
			{
				TRACE0("Failed to create view window\n");
				return -1;
			}

			// 			m_pImageView[i]->LoadImageW(_T("d:\\1_1.bmp"));
		}
	}


	//
	SetDlgItemInt(IDC_EDIT_VIEW_NUMBER, m_iNumViewer);
	SetDlgItemInt(IDC_EDIT_IMAGE_NUMBER, m_iNumImage);
	SetDlgItemInt(IDC_EDIT_ZOOM, m_iZoom);

	SetDlgItemText(IDC_EDIT_PATH_1, _T("D:\\CU\\GAP\\CU_GAP (0)_����\\191115_145202_(No code)_OK"));
	SetDlgItemText(IDC_EDIT_PATH_2, _T("D:\\CU\\GAP\\CU_GAP (1)\\191115_145414_(No code)_NG"));
	SetDlgItemText(IDC_EDIT_PATH_3, _T("D:\\CU\\GAP\\CU_GAP (2)\\191115_145556_(No code)_NG"));
	SetDlgItemText(IDC_EDIT_PATH_4, _T("D:\\CU\\GAP\\CU_GAP (3)\\191115_150207_(No code)_NG"));
	SetDlgItemText(IDC_EDIT_PATH_5, _T("D:\\CU\\GAP\\CU_GAP (4)\\191115_150337_(No code)_NG"));


	//
	CreateLightingView();


	//
	LoadLightingTable();


	m_comboData.SetCurSel(0);
	m_ctrImageNum_graph.SetCurSel(0);

	m_ctrlProgress.SetRange( 0, 100 );


	//
	SetTimer(_TIMER_VIEW_, 1000, NULL );
	SetTimer(_TIMER_PROGRESS_, 250, NULL );

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

CWnd *CBHC_multiImageDlg::GetDlgItemViewer(int i)
{
	switch(i)
	{
	case 0:
		return GetDlgItem(IDC_STATIC_VIEW_1);

	case 1:
		return GetDlgItem(IDC_STATIC_VIEW_2);

	case 2:
		return GetDlgItem(IDC_STATIC_VIEW_3);

	case 3:
		return GetDlgItem(IDC_STATIC_VIEW_4);

	case 4:
		return GetDlgItem(IDC_STATIC_VIEW_5);

	case 5:
		return GetDlgItem(IDC_STATIC_VIEW_6);

	case 6:
		return GetDlgItem(IDC_STATIC_VIEW_7);

	case 7:
		return GetDlgItem(IDC_STATIC_VIEW_8);

	}

	return NULL;
}


CWnd *CBHC_multiImageDlg::GetDlgItemSubViewer(int i)
{
	switch(i)
	{
	case 0:
		return GetDlgItem(IDC_STATIC_VIEW_SUB_1);

	case 1:
		return GetDlgItem(IDC_STATIC_VIEW_SUB_2);

	case 2:
		return GetDlgItem(IDC_STATIC_VIEW_SUB_3);

	case 3:
		return GetDlgItem(IDC_STATIC_VIEW_SUB_4);

	case 4:
		return GetDlgItem(IDC_STATIC_VIEW_SUB_5);

	case 5:
		return GetDlgItem(IDC_STATIC_VIEW_SUB_6);

	case 6:
		return GetDlgItem(IDC_STATIC_VIEW_SUB_7);

	case 7:
		return GetDlgItem(IDC_STATIC_VIEW_SUB_8);

	}

	return NULL;
}


void CBHC_multiImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBHC_multiImageDlg::OnPaint()
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
HCURSOR CBHC_multiImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CBHC_multiImageDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	for( int i=0 ; i<_BHC_VIEW_NUMBER_ ; i++ )
	{
		CRuntimeClass  *pObject;
		pObject = RUNTIME_CLASS(CImageViewer);
		m_pImageView[i] = (CImageViewer*)pObject->CreateObject();
	}


	for( int i=0 ; i<_BHC_VIEW_NUMBER_ ; i++ )
	{
		CRuntimeClass  *pObject;
		pObject = RUNTIME_CLASS(CImageViewer);
		m_pImageView_sub[i] = (CImageViewer*)pObject->CreateObject();
	}

	return 0;
}


void CBHC_multiImageDlg::OnBnClickedButtonLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	//
	GetFilePath();

	//
	LoadImages();

	//
	LoadColorImages();

	//
	UpdateLightingView();
}


void CBHC_multiImageDlg::SetFilePath(int i)
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


	m_strDirectory[i] = pszPathname;

// 	SetDlgItemText(IDC_EDIT_PATH_1 + i, m_strDirectory[i]);
	SetFilePath( i, m_strDirectory[i] );
}

void CBHC_multiImageDlg::SetFilePath(int i, CString strPath)
{
	SetDlgItemText(IDC_EDIT_PATH_1 + i, strPath);
}


bool CBHC_multiImageDlg::SelectDirecctory(CString *pStr)
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

	if (pidlBrowse == NULL)
	{
		return false;
	}

	SHGetPathFromIDList(pidlBrowse, pszPathname);


	*pStr = pszPathname;


	return true;
}


void CBHC_multiImageDlg::GetFilePath()
{
	for( int i=0 ; i<8 ; i++)
	{
		m_strDirectory[i] = GetFilePath(i);
	}
}


CString CBHC_multiImageDlg::GetFilePath(int i)
{
	CString str(_T(""));

	switch(i)
	{
	case 0:
		GetDlgItemText(IDC_EDIT_PATH_1, str);
		break;
	case 1:
		GetDlgItemText(IDC_EDIT_PATH_2, str);
		break;
	case 2:
		GetDlgItemText(IDC_EDIT_PATH_3, str);
		break;
	case 3:
		GetDlgItemText(IDC_EDIT_PATH_4, str);
		break;
	case 4:
		GetDlgItemText(IDC_EDIT_PATH_5, str);
		break;
	case 5:
		GetDlgItemText(IDC_EDIT_PATH_6, str);
		break;
	case 6:
		GetDlgItemText(IDC_EDIT_PATH_7, str);
		break;
	case 7:
		GetDlgItemText(IDC_EDIT_PATH_8, str);
		break;
	}

	return str;
}


//////////////////////////////////////////////////////////////////////////
//
//	GetFilePath
//
//	UI �� ������ ���丮�� ��´�.
//
//
//////////////////////////////////////////////////////////////////////////
void CBHC_multiImageDlg::GetFilePath(int i, CString *pStrPath)
{
	GetDlgItemText(IDC_EDIT_PATH_1 + i, *pStrPath);
}




//////////////////////////////////////////////////////////////////////////
//
//	LoadImages
//
//
//	������ �ε��Ѵ�.
//
//		- UI ���� ���丮, ���ϸ��� �̿��Ͽ� �ε�
//		- 
//
//////////////////////////////////////////////////////////////////////////
void CBHC_multiImageDlg::LoadImages()
{
	CString strKeyIndex;
	GetDlgItemText(IDC_EDIT_FILE_NAME_2, strKeyIndex);

	int iKeyIndex = atoi((CStringA)strKeyIndex);

	for( int i=0 ; i<m_iNumViewer ; i++ )
	{
		// key word
		CString strKeyWord;
		strKeyWord.Format(_T("_%02d.bmp"), iKeyIndex);


		// find target file
		CString strFile;
		if( !FindTargetFile( GetFilePath(i), strKeyWord, &strFile ) )
		{
			break;
		}

		// load image
		m_pImageView[i]->LoadImage(strFile);
	}
}


//////////////////////////////////////////////////////////////////////////
//
//	LoadColorImages
//
//
//	�ռ� color ������ �����Ѵ�.
//
//////////////////////////////////////////////////////////////////////////
void CBHC_multiImageDlg::LoadColorImages()
{
	CString strKeyIndex;
	GetDlgItemText(IDC_EDIT_FILE_NAME_2, strKeyIndex);

	int iKeyIndex = atoi( (CStringA)strKeyIndex );


	// color ���� �� index ȹ��
	int iKeyIndex_red	= 0;
	int iKeyIndex_green	= 0;
	int iKeyIndex_blue	= 0;
	if( !GetIndex_red(iKeyIndex, &iKeyIndex_red) ||
		!GetIndex_green(iKeyIndex, &iKeyIndex_green) ||
		!GetIndex_blue(iKeyIndex, &iKeyIndex_blue) )
	{
		return;
	}

	CString strKeyIndex_red;
	if( iKeyIndex_red<10 )
	{
		strKeyIndex_red.Format(_T("0%d"), iKeyIndex_red);
	} else {
		strKeyIndex_red.Format(_T("%d"), iKeyIndex_red);
	}

	CString strKeyIndex_green;
	if( iKeyIndex_green<10 )
	{
		strKeyIndex_green.Format(_T("0%d"), iKeyIndex_green);
	} else {
		strKeyIndex_green.Format(_T("%d"), iKeyIndex_green);
	}

	CString strKeyIndex_blue;
	if( iKeyIndex_blue<10 )
	{
		strKeyIndex_blue.Format(_T("0%d"), iKeyIndex_blue);
	} else {
		strKeyIndex_blue.Format(_T("%d"), iKeyIndex_blue);
	}



	for( int i=0 ; i<m_iNumViewer ; i++ )
	{
		//
		// red
		//
		// key word
		CString strKeyWord_red;
		strKeyWord_red.Format(_T("_%s.bmp"), strKeyIndex_red);


		// find target file
		CString strFile_red;
// 		if( !FindTargetFile( m_strDirectory[i], strKeyWord_red, &strFile_red ) )
		if( !FindTargetFile( GetFilePath(i), strKeyWord_red, &strFile_red ) )
		{
			break;
		}

		// load image
		CBhImage imgRed;
		imgRed.Open(strFile_red);


		//
		// green
		//
		// key word
		CString strKeyWord_green;
		strKeyWord_green.Format(_T("_%s.bmp"), strKeyIndex_green);


		// find target file
		CString strFile_green;
// 		if( !FindTargetFile( m_strDirectory[i], strKeyWord_green, &strFile_green ) )
		if( !FindTargetFile(GetFilePath(i), strKeyWord_green, &strFile_green ) )
		{
			break;
		}

		// load image
		CBhImage imgGreen;
		imgGreen.Open(strFile_green);


		//
		// blue
		//
		// key word
		CString strKeyWord_blue;
		strKeyWord_blue.Format(_T("_%s.bmp"), strKeyIndex_blue);


		// find target file
		CString strFile_blue;
// 		if( !FindTargetFile( m_strDirectory[i], strKeyWord_blue, &strFile_blue ) )
		if( !FindTargetFile( GetFilePath(i), strKeyWord_blue, &strFile_blue ) )
		{
			break;
		}

		// load image
		CBhImage imgBlue;
		imgBlue.Open(strFile_blue);



		//
		// set image
		//
		m_pImageView_sub[i]->SetColorImage(	imgRed.GetWidth(), 
											imgRed.GetHeight(),
											imgRed.GetImage(),		// R
											imgGreen.GetImage(),	// G
											imgBlue.GetImage() );	// B
	}
}



//////////////////////////////////////////////////////////////////////////
//
//	FindTargetFile
//
//	�Է� ���� �������� �Է¹��� key�� �����ϴ� ���ϸ��� �����Ѵ�.
//
//
///////////////////////////////////////////////////////////////////////////
BOOL CBHC_multiImageDlg::FindTargetFile( CString strPath, CString strKey, CString *pStrFile )
{
	// �˻� ���� ����
	CString strPath_reshape;
	strPath_reshape.Format(_T("%s\\*"), strPath );

	CFileFind fFinder;
	BOOL bFindFile = fFinder.FindFile(strPath_reshape);


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
		if( iFind > 0 )
		{
			pStrFile->Format(_T("%s\\%s"), strPath, strFile_interest );

			return TRUE;
		}
	}


	return FALSE;
}



int CBHC_multiImageDlg::GetImageIndex()
{
	CString strKeyIndex;
	GetDlgItemText(IDC_EDIT_FILE_NAME_2, strKeyIndex);

	return atoi((CStringA)strKeyIndex);
}


void CBHC_multiImageDlg::SetImageIndex(int iIndex)
{
	CString str;
	str.Format(_T("%02d"), iIndex);
	SetDlgItemText(IDC_EDIT_FILE_NAME_2, str);
}


int CBHC_multiImageDlg::GetMaxImageIndex()
{
	return GetDlgItemInt(IDC_EDIT_IMAGE_NUMBER);
}


void CBHC_multiImageDlg::SetMaxImageIndex(int i)
{
	SetDlgItemInt(IDC_EDIT_IMAGE_NUMBER, i );
}


int CBHC_multiImageDlg::GetDataNum_graph()
{
	switch( m_ctrImageNum_graph.GetCurSel() )
	{
	case 0:
		return _BHC_MID_ALL_DATA_GRAPH_;

	case 1:
		return _BHC_MID_MAJOR_DATA_GRAPH_;

	default:
		return _BHC_MID_ALL_DATA_GRAPH_;

	}

	return _BHC_MID_ALL_DATA_GRAPH_;
}



void CBHC_multiImageDlg::OnBnClickedButtonSetPath1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetFilePath(0);
}


void CBHC_multiImageDlg::OnBnClickedButtonSetPath2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetFilePath(1);
}


void CBHC_multiImageDlg::OnBnClickedButtonSetPath3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetFilePath(2);
}


void CBHC_multiImageDlg::OnBnClickedButtonSetPath4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetFilePath(3);
}


void CBHC_multiImageDlg::OnBnClickedButtonSetPath5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetFilePath(4);
}


void CBHC_multiImageDlg::OnBnClickedButtonSetPath6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetFilePath(5);
}


void CBHC_multiImageDlg::OnBnClickedButtonSetPath7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetFilePath(6);
}


void CBHC_multiImageDlg::OnBnClickedButtonSetPath8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetFilePath(7);
}


void CBHC_multiImageDlg::OnBnClickedButtonSetZoom()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iZoom = GetDlgItemInt(IDC_EDIT_ZOOM);


	for( int i=0 ; i<m_iNumViewer ; i++ )
	{
		m_pImageView[i]->SetZoom(m_iZoom);
	}
}

void CBHC_multiImageDlg::OnBnClickedButtonSetZoomIn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetDlgItemInt(IDC_EDIT_ZOOM, m_iZoom + 1);

	m_iZoom = GetDlgItemInt(IDC_EDIT_ZOOM);

	for( int i=0 ; i<m_iNumViewer ; i++ )
	{
		m_pImageView[i]->SetZoom(m_iZoom);
	}

	for( int i=0 ; i<m_iNumViewer ; i++ )
	{
		m_pImageView_sub[i]->SetZoom(m_iZoom);
	}
}


void CBHC_multiImageDlg::OnBnClickedButtonSetZoomOut()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetDlgItemInt(IDC_EDIT_ZOOM, m_iZoom - 1);

	m_iZoom = GetDlgItemInt(IDC_EDIT_ZOOM);

	for( int i=0 ; i<m_iNumViewer ; i++ )
	{
		m_pImageView[i]->SetZoom(m_iZoom);
	}

	for( int i=0 ; i<m_iNumViewer ; i++ )
	{
		m_pImageView_sub[i]->SetZoom(m_iZoom);
	}
}


void CBHC_multiImageDlg::OnBnClickedButtonSetImageNumber()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iNumImage = GetMaxImageIndex();
}


void CBHC_multiImageDlg::OnBnClickedButtonSetImageViewer()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iNumViewer = GetDlgItemInt(IDC_EDIT_VIEW_NUMBER);
}



//////////////////////////////////////////////////////////////////////////
//
//	OnBnClickedButtonLoadLightingCondition
//
//	- ���� ���� �� index �б�
//
//////////////////////////////////////////////////////////////////////////
void CBHC_multiImageDlg::OnBnClickedButtonLoadLightingCondition()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	LoadLightingTable();
}




//////////////////////////////////////////////////////////////////////////
//
//	LoadLightingTable
//
//	���� ������ �������Ͽ��� �д´�.
//
//////////////////////////////////////////////////////////////////////////
BOOL CBHC_multiImageDlg::LoadLightingTable()
{
	CString strFilePath = _T("d:\\light.csv");

	ifstream is(strFilePath);
	char s[100];
	// 	wchar_t  s[100];

	is.getline(s, sizeof(s));	// 

	int iValue;
	for( int y=0 ; y<_BHC_LIGHTING_CONDITION_TABLE_NUM_ ; y++ )
	{
		for( int x=0 ; x<_BHC_LIGHTING_CONDITION_NUM_ - 1 ; x++ )
		{
			is.getline(s, sizeof(s), ',');

			iValue = atoi(s);

			m_aLightingConditionTable[y][x] = iValue;
		}

		is.getline(s, sizeof(s));
		iValue = atoi(s);

		m_aLightingConditionTable[y][_BHC_LIGHTING_CONDITION_NUM_ - 1] = iValue;
	}


	return TRUE;
}


void CBHC_multiImageDlg::OnBnClickedButtonNext()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//
	// Index ����
	//
	// ���簪
	CString strKeyWord;
	GetDlgItemText(IDC_EDIT_FILE_NAME_2, strKeyWord);

	int iIndex = atoi((CStringA)strKeyWord);


	// ���氪
	iIndex = (iIndex + 1)%m_iNumImage;

	CString strTarget;
	strTarget.Format(_T("%02d"), iIndex);


	// ����
	SetDlgItemText(IDC_EDIT_FILE_NAME_2, strTarget);
// 	SetDlgItemInt(IDC_EDIT_FILE_NAME_2, iIndex );


	//
	//
	//
	LoadImages();


	//
	//
	//
	LoadColorImages();


	//
	//
	//
	UpdateLightingView();
}


void CBHC_multiImageDlg::OnBnClickedButtonBefore()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//
	// Index ����
	//
	// ���� ��
	CString strKeyWord;
	GetDlgItemText(IDC_EDIT_FILE_NAME_2, strKeyWord);

	int iIndex = atoi((CStringA)strKeyWord);


	// ���� ��
	if(iIndex > 0)
	{
		iIndex--;
	} else {
		iIndex = m_iNumImage - 1;
	}

	CString strTarget;
	strTarget.Format(_T("%02d"), iIndex);


	// ����
	SetDlgItemText(IDC_EDIT_FILE_NAME_2, strTarget);


	//
	//
	//
	LoadImages();


	//
	//
	//
	LoadColorImages();


	//
	//
	//
	UpdateLightingView();
}


void CBHC_multiImageDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


	if( nIDEvent == _TIMER_VIEW_ )
	{
		CPoint ptScroll = m_pImageView[0]->GetScrollPosition();

		for( int i=1 ; i<m_iNumViewer ; i++ )
		{
			CPoint ptScroll_old = m_pImageView[i]->GetScrollPosition();

			if( ptScroll != ptScroll_old )
			{
				m_pImageView[i]->SetScrollPos(SB_HORZ, ptScroll.x);
				m_pImageView[i]->SetScrollPos(SB_VERT, ptScroll.y);

				m_pImageView[i]->SendMessage(WM_INVALIDATE_VIWE);
			}
		}

		for( int i=0 ; i<m_iNumViewer ; i++ )
		{
			CPoint ptScroll_old = m_pImageView_sub[i]->GetScrollPosition();

			if( ptScroll != ptScroll_old )
			{
				m_pImageView_sub[i]->SetScrollPos(SB_HORZ, ptScroll.x);
				m_pImageView_sub[i]->SetScrollPos(SB_VERT, ptScroll.y);

				m_pImageView_sub[i]->SendMessage(WM_INVALIDATE_VIWE);
			}
		}
	}


	//
	if( nIDEvent == _TIMER_PROGRESS_ )
	{
		m_ctrlProgress.SetPos( (int)(100 * m_dProgress_ratio) );
	}


	CDialogEx::OnTimer(nIDEvent);
}


void CBHC_multiImageDlg::OnBnClickedButtonUpdateLightingCondition2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateLightingView();
}

BOOL CBHC_multiImageDlg::UpdateLightingView()
{
	// INDEX ȹ��
	CString strIndex;

	GetDlgItemText(IDC_EDIT_FILE_NAME_2, strIndex);

	int iIndex = atoi((CStringA)strIndex);


	//
	if( !UpdateLightingView(iIndex) )
	{
		return FALSE;
	}


	return TRUE;
}


BOOL CBHC_multiImageDlg::UpdateLightingView(int iIndex)
{
	ResetLightingView();

	CTitleWnd *pWnd;

	// angle
	switch(m_aLightingConditionTable[iIndex][0])
	{
	case _BHC_ANLGLE_LOW_:
		pWnd = &(m_ctrlLightLow[0]);
		break;
	case _BHC_ANLGLE_MID__:
		pWnd = &(m_ctrlLightMidle[0]);
		break;
	case _BHC_ANLGLE_HIGH_:
		pWnd = &(m_ctrlLightHigh[0]);
		break;
	default:
		return FALSE;
	}


	// color
	COLORREF color;
	switch(m_aLightingConditionTable[iIndex][1])
	{
	case _BHC_RED_:
		color = RGB(255,0,0);
		break;
	case _BHC_GREEN_:
		color = RGB(0,255,0);
		break;
	case _BHC_BLUE_:
		color = RGB(0,0,255);
		break;
	case _BHC_WHITE_:
		color = RGB(255,255,255);
		break;
	default:
		return FALSE;
	}


	//
	for( int i=0 ; i<8 ; i++ )
	{
		if( m_aLightingConditionTable[iIndex][2+i] == 1 )
		{
			pWnd[i].SetBkColor(color);
		}
	}

	return TRUE;
}


BOOL CBHC_multiImageDlg::GetIndex_red( int iIndex, int *piIndex )
{
	//
	// ��ǥ�� ����
	//
	int aTarget[_BHC_LIGHTING_CONDITION_NUM_];

	// ���� ����
	for( int i=0 ; i<_BHC_LIGHTING_CONDITION_NUM_ ; i++ )
	{
		aTarget[i] = m_aLightingConditionTable[iIndex][i];
	}

	// ������ �Է�
	aTarget[1] = _BHC_RED_;


	//
	// index ����
	//
	bool bValid = true;
	for( int i=0 ; i<_BHC_LIGHTING_CONDITION_TABLE_NUM_ ; i++ )
	{
		bValid = true;
		for( int k=0 ; k<_BHC_LIGHTING_CONDITION_NUM_ ; k++ )
		{
			if( aTarget[k] != m_aLightingConditionTable[i][k] )
			{
				bValid = false;
			}
		}

		// ���ϴ� ������ ã���� ����
		if( bValid )
		{
			*piIndex = i;

			return TRUE;
		}
	}


	return FALSE;
}


BOOL CBHC_multiImageDlg::GetIndex_green( int iIndex, int *piIndex )
{
	//
	// ��ǥ�� ����
	//
	int aTarget[_BHC_LIGHTING_CONDITION_NUM_];

	// ���� ����
	for( int i=0 ; i<_BHC_LIGHTING_CONDITION_NUM_ ; i++ )
	{
		aTarget[i] = m_aLightingConditionTable[iIndex][i];
	}

	// ������ �Է�
	aTarget[1] = _BHC_GREEN_;


	//
	// index ����
	//
	bool bValid = true;
	for( int i=0 ; i<_BHC_LIGHTING_CONDITION_TABLE_NUM_ ; i++ )
	{
		bValid = true;
		for( int k=0 ; k<_BHC_LIGHTING_CONDITION_NUM_ ; k++ )
		{
			if( aTarget[k] != m_aLightingConditionTable[i][k] )
			{
				bValid = false;
			}
		}

		// ���ϴ� ������ ã���� ����
		if( bValid )
		{
			*piIndex = i;

			return TRUE;
		}
	}


	return FALSE;
}


BOOL CBHC_multiImageDlg::GetIndex_blue( int iIndex, int *piIndex )
{
	//
	// ��ǥ�� ����
	//
	int aTarget[_BHC_LIGHTING_CONDITION_NUM_];

	// ���� ����
	for( int i=0 ; i<_BHC_LIGHTING_CONDITION_NUM_ ; i++ )
	{
		aTarget[i] = m_aLightingConditionTable[iIndex][i];
	}

	// ������ �Է�
	aTarget[1] = _BHC_BLUE_;


	//
	// index ����
	//
	bool bValid = true;
	for( int i=0 ; i<_BHC_LIGHTING_CONDITION_TABLE_NUM_ ; i++ )
	{
		bValid = true;
		for( int k=0 ; k<_BHC_LIGHTING_CONDITION_NUM_ ; k++ )
		{
			if( aTarget[k] != m_aLightingConditionTable[i][k] )
			{
				bValid = false;
			}
		}

		// ���ϴ� ������ ã���� ����
		if( bValid )
		{
			*piIndex = i;

			return TRUE;
		}
	}


	return FALSE;
}


BOOL CBHC_multiImageDlg::CreateLightingView()
{
	for( int i=0 ; i<8 ; i++ )
	{
		m_ctrlLightHigh[i].SubclassDlgItem(IDC_STATIC_LIGHT_HIGH_1 + i, this);
		m_ctrlLightHigh[i].SetTextColor(RGB(0,0,0));
		m_ctrlLightHigh[i].SetFontSize(8);
		m_ctrlLightHigh[i].SetBkColor(RGB(0,0,0));

		m_ctrlLightMidle[i].SubclassDlgItem(IDC_STATIC_LIGHT_MID_1 + i, this);
		m_ctrlLightMidle[i].SetTextColor(RGB(0,0,0));
		m_ctrlLightMidle[i].SetFontSize(8);
		m_ctrlLightMidle[i].SetBkColor(RGB(0,0,0));

		m_ctrlLightLow[i].SubclassDlgItem(IDC_STATIC_LIGHT_LOW_1 + i, this);
		m_ctrlLightLow[i].SetTextColor(RGB(0,0,0));
		m_ctrlLightLow[i].SetFontSize(8);
		m_ctrlLightLow[i].SetBkColor(RGB(0,0,0));

	}


	return TRUE;
}


BOOL CBHC_multiImageDlg::ResetLightingView()
{
	for( int i=0 ; i<8 ; i++ )
	{
		m_ctrlLightHigh[i].SetBkColor(RGB(0,0,0));

		m_ctrlLightMidle[i].SetBkColor(RGB(0,0,0));

		m_ctrlLightLow[i].SetBkColor(RGB(0,0,0));

	}


	return TRUE;
}



//////////////////////////////////////////////////////////////////////////
//
//	ROI �����ϱ�
//
//
//
//////////////////////////////////////////////////////////////////////////
void CBHC_multiImageDlg::OnBnClickedButtonInspRoi()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Memo(_T("inspect roi..."));

	for( int i=0 ; i<m_iNumViewer ; i++ )
	{
		FindRoi(i);
	}

	Memo(_T("inspect roi...ok"));
}


//////////////////////////////////////////////////////////////////////////
//
//	ROI �����ϱ�
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_multiImageDlg::FindRoi()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	for( int i=0 ; i<m_iNumViewer ; i++ )
	{
		if( FindRoi(i) == false )
		{
			return false;
		}
	}


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	ROI �����ϱ�
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_multiImageDlg::FindRoi(int i)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ����
	m_imageProcessing[i].SetShowImage( GetDlgCheck_display() );	
	m_imageProcessing[i].SetShowRatio( 0.25 );

	// �˻�
// 	if( !m_imageProcessing[i].FindRoi_batWeld(	m_pImageView[i]->GetImageBuffer(), 
// 												m_pImageView[i]->GetImageWidth(), 
// 												m_pImageView[i]->GetImageHeight(), 
// 												96,	// 64, 
// 												14000 - 6000,	// 10000 - 4000, 
// 												14000 + 6000 ) )// 10000 + 4000 ) )
// 	{
// 		return false;
// 	}


	if( !m_imageProcessing[i].FindRoi_batWeld(	m_pImageView[i]->GetImageBuffer(), 
		m_pImageView[i]->GetImageWidth(), 
		m_pImageView[i]->GetImageHeight(), 
		64, 
		10000 - 4000, 
		10000 + 4000 ) )
	{
		return false;
	}


	return true;
}


bool CBHC_multiImageDlg::ExtractFeature()
{
	for( int i=0 ; i<m_iNumViewer ; i++ )
	{
		if( ExtractFeature(i) == false )
		{
			return false;
		}
	}


	return true;
}


bool CBHC_multiImageDlg::ExtractFeature(int i)
{
	// �˻�
	if( !m_imageProcessing[i].Inspect_batWeld(	m_pImageView_sub[i]->GetImageBuffer(), 
												m_pImageView_sub[i]->GetImageWidth(), 
												m_pImageView_sub[i]->GetImageHeight()) )
	{
		return false;
	}


	return true;
}

//////////////////////////////////////////////////////////////////////////
//
//	�˻��ϱ�
//
//
//
//////////////////////////////////////////////////////////////////////////
void CBHC_multiImageDlg::OnBnClickedButtonInsp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Memo(_T("inspect image..."));

	for( int i=0 ; i<m_iNumViewer ; i++ )
	{
		// ����
		m_imageProcessing[i].SetShowImage( true );	
		m_imageProcessing[i].SetShowRatio( 0.25 );

		// �˻�
		m_imageProcessing[i].Inspect_batWeld(	m_pImageView_sub[i]->GetImageBuffer(), 
												m_pImageView_sub[i]->GetImageWidth(), 
												m_pImageView_sub[i]->GetImageHeight());
	}

	Memo(_T("inspect image...OK"));
}


int CBHC_multiImageDlg::GetDataType_graph()
{
	switch( m_comboData.GetCurSel() )
	{
	case 0:
		return _BHC_MID_XYZ_GRAPH_;
		break;

	case 1:
		return _BHC_MID_HSV_GRAPH_;
		break;

	case 2:
		return _BHC_MID_LUV_GRAPH_;
		break;

	default:
		return _BHC_MID_XYZ_GRAPH_;
	}
}


bool CBHC_multiImageDlg::GetDlgCheck_display()
{
	if( ((CButton*)GetDlgItem(IDC_CHECK_DISPLAY_PROCESSING))->GetCheck()== TRUE )
	{
		return true;
	}

	return false;
}



//////////////////////////////////////////////////////////////////////////
//
//	InspectDirectory
//
//	�Է��� ���丮���� �������Ͽ� ���ؼ� Ư¡�� ���� �� ����
//
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_multiImageDlg::InspectDirectory( CString strDirectory )
{
	Memo(_T("inspect directory..."));

	m_dProgress_ratio = 0.0;

	//
	int iNumViewer_old = m_iNumViewer;
	m_iNumViewer = 1;

	// ���� ����
	SetFilePath(0, strDirectory);
	

	// ���ϸ� ���� = ������ ���
	int iFind = strDirectory.ReverseFind(_T('\\'));
	if( iFind == -1 )
	{
		return false;
	}
	CString strFileName = strDirectory.Mid( iFind + 1 );


	//
	// ROI ����
	//
	// roi ����� ���� index ����
// 	SetImageIndex(8);	// Al
	SetImageIndex(26);	// Cu

	// ���� �ε�
	LoadImages();

	// ���� ���� ǥ��
	UpdateLightingView();

	// �˻�
	if( FindRoi() == false )
	{
		return false;
	}


	//
	// Ư¡�� ���� �� ����
	//

	// gen data file
	CStdioFile dataFile;
	CString strFile;
	strFile.Format( _T("d:\\test\\%s.csv"), strFileName );	

	if( !dataFile.Open( strFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText ) )
	{
		AfxMessageBox(_T("Can't make data file"));
// 		dataFile.Close();
		return false;
	}


	// ���� �Է�
	CString strContents;
	strContents.Format( _T("folder,index(image),index(welding),"));
	dataFile.WriteString(strContents);

	strContents.Format( _T("area,radius,"));
	dataFile.WriteString(strContents);

	strContents.Format( _T("Xxyz_mean,Yxyz_mean,Zxyz_mean,Xxyz_dev,Yxyz_dev,Zxyz_dev,"));
	dataFile.WriteString(strContents);

	strContents.Format( _T("Hhsv_mean,Shsv_mean,Vhsv_mean,Hhsv_dev,Shsv_dev,Vhsv_dev,"));
	dataFile.WriteString(strContents);

	strContents.Format( _T("Lluv_mean,Uluv_mean,Vluv_mean,Lluv_dev,Uluv_dev,Vluv_dev\n"));
	dataFile.WriteString(strContents);


	// inspect
	int nImage = GetMaxImageIndex();
// 	for( int i=0 ; i<nImage ; i++ )
	for( int i=26 ; i<27 ; i++ )
	{
		//
		SetImageIndex(i);

		//
		LoadImages();

		//
		LoadColorImages();

		//
		UpdateLightingView();

		//
		ExtractFeature(0);


		// save
		int nData = 0;
		stInfo_batWeld *pResult = m_imageProcessing[0].GetInfo_batWeld(&nData);
		
		for( int k=0 ; k<nData ; k++ )
		{
			// common info
			strContents.Format( _T("%s,%d,%d,"),
								strDirectory,
								i,	// ���� index
								k);
			dataFile.WriteString(strContents);

			// area
			strContents.Format( _T("%d,%f,"), pResult[k].iArea, pResult[k].dRadius);
			dataFile.WriteString(strContents);


			// xyz
			double dSum = pResult[k].dXyz_x_mean + pResult[k].dXyz_y_mean + pResult[k].dXyz_z_mean;
			strContents.Format( _T("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,"),
				pResult[k].dXyz_x_mean/dSum,		pResult[k].dXyz_y_mean/dSum,		pResult[k].dXyz_z_mean/dSum,
				pResult[k].dXyz_x_deviation/dSum,	pResult[k].dXyz_y_deviation/dSum,	pResult[k].dXyz_z_deviation/dSum );
			dataFile.WriteString(strContents);
			 
			// hsv
			strContents.Format( _T("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,"),
				pResult[k].dHsv_h_mean,			pResult[k].dHsv_s_mean,			pResult[k].dHsv_v_mean,
				pResult[k].dHsv_h_deviation,	pResult[k].dHsv_s_deviation,	pResult[k].dHsv_v_deviation );
			dataFile.WriteString(strContents);

			// luv
			strContents.Format( _T("%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\n"),
				pResult[k].dLuv_l_mean,			pResult[k].dLuv_u_mean,			pResult[k].dLuv_v_mean,
				pResult[k].dLuv_l_deviation,	pResult[k].dLuv_u_deviation,	pResult[k].dLuv_v_deviation	);
			dataFile.WriteString(strContents);
		}


		// 
		m_dProgress_ratio = (double)(i+1)/nImage;
// 		m_ctrlProgress.SetPos( (int)(100 * m_dProgress_ratio) );
	}

	dataFile.Close();

	m_iNumViewer = iNumViewer_old;

	Memo(_T("inspect directory...OK"));


	return true;
}



void CBHC_multiImageDlg::OnBnClickedButtonShowData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ��� ������ type
	int iDataType = GetDataType_graph();

	// ��� ���� ��
	int iTargetImageNum = GetDataNum_graph();


	//
	CGraphDlg *pGraph;
	pGraph = new CGraphDlg;
	pGraph->Create(IDD_GRAPHDLG);
	pGraph->init();

	pGraph->ClearData();

	// set data
// 	int iImageNum = 0;
// 	switch( iTargetImageNum )
// 	{
// 	case _BHC_MID_N_IMAGE_GRAPH_: // ���� �����϶�
// 		iImageNum = m_iNumViewer;
// 		break;
// 	case _BHC_MID_ONE_IMAGE_GRAPH_: // �� ������ ��
// 		iImageNum = 1;
// 		break;
// 	default:
// 		iImageNum = m_iNumViewer;
// 	}


// 	for( int i=0 ; i<iImageNum ; i++ )
	for( int i=0 ; i<m_iNumViewer ; i++ )
	{
		int nData=0;
		stInfo_batWeld *pData = m_imageProcessing[i].GetInfo_batWeld(&nData);

		int iRed = 0;
		int iGreen = 0;
		int iBlue = 0;
		

		for( int k=0 ; k<8 ; k++ )
		{
			// all data
			iRed	= 0;
			iGreen	= 0;
			iBlue	= 255;

			int iIndex = 10*i + k;	// index 
// 			switch( iTargetImageNum )
// 			{
// 			case _BHC_MID_ALL_DATA_GRAPH_: // ���� �����϶��� ���������...
// 				iIndex = 10*i + k;
// 				break;
// 
// 			case _BHC_MID_MAJOR_DATA_GRAPH_: // �� ������ ���� ���� blob ������...
// 				iIndex = k;
// 				break;
// 
// 			default:
// 				iIndex = 10*i + k;
// 			}


			// XYZ
			if( iDataType == _BHC_MID_XYZ_GRAPH_)
			{
				// ��ü �����͸� �׷����� �߰�
				if( iTargetImageNum == _BHC_MID_ALL_DATA_GRAPH_ )
				{
					std::list<double>::iterator ite_x = pData[k].listXyz_x.begin();
					std::list<double>::iterator ite_y = pData[k].listXyz_y.begin();
					std::list<double>::iterator ite_z = pData[k].listXyz_z.begin();
					for(  ; ite_x != pData[k].listXyz_x.end() ; ite_x++, ite_y++, ite_z++ )
					{
						double dx = *ite_x;
						double dy = *ite_y;
						double dz = *ite_z;
						double dSum = dx + dy + dz;

						pGraph->AddData( dx/dSum, dy/dSum, iRed, iGreen, iBlue, iIndex );
					}
				}


				// ��ǥ �����͸� �׷����� �߰�
				iRed	= 255;
				iGreen	= 0;
				iBlue	= 0;
				double dx = pData[k].dXyz_x_mean;
				double dy = pData[k].dXyz_y_mean;
				double dz = pData[k].dXyz_z_mean;
				double dSum = dx + dy + dz;

				pGraph->AddData( dx/dSum, dy/dSum, iRed, iGreen, iBlue, iIndex );
			}


			// XYZ
			if( iDataType == _BHC_MID_HSV_GRAPH_)
			{
				// ��ü �����͸� �׷����� �߰�
				if( iTargetImageNum == _BHC_MID_ALL_DATA_GRAPH_ )
				{
					std::list<double>::iterator ite_x = pData[k].listHsv_h.begin();
					std::list<double>::iterator ite_y = pData[k].listHsv_s.begin();
					std::list<double>::iterator ite_z = pData[k].listHsv_v.begin();
					for(  ; ite_x != pData[k].listHsv_h.end() ; ite_x++, ite_y++, ite_z++ )
					{
						double dh = *ite_x;
						double ds = *ite_y;
						double dv = *ite_z;

						pGraph->AddData( dh, ds, iRed, iGreen, iBlue, iIndex );
					}
				}


				// ��ǥ �����͸� �׷����� �߰�
				iRed	= 255;
				iGreen	= 0;
				iBlue	= 0;
				double dh = pData[k].dHsv_h_mean;
				double ds = pData[k].dHsv_s_mean;
				double dv = pData[k].dHsv_v_mean;

				pGraph->AddData( dh, ds, iRed, iGreen, iBlue, iIndex );
			}

			// LUV
			if( iDataType == _BHC_MID_LUV_GRAPH_)
			{
				// ��ü �����͸� �׷����� �߰�
				if( iTargetImageNum == _BHC_MID_ALL_DATA_GRAPH_ )
				{
					std::list<double>::iterator ite_x = pData[k].listLuv_l.begin();
					std::list<double>::iterator ite_y = pData[k].listLuv_u.begin();
					std::list<double>::iterator ite_z = pData[k].listLuv_v.begin();
					for(  ; ite_x != pData[k].listLuv_l.end() ; ite_x++, ite_y++, ite_z++ )
					{
						double dl = *ite_x;
						double du = *ite_y;
						double dv = *ite_z;

						pGraph->AddData( dl, du, iRed, iGreen, iBlue, iIndex );
					}
				}


				// ��ǥ �����͸� �׷����� �߰�
				iRed	= 255;
				iGreen	= 0;
				iBlue	= 0;
				double dl = pData[k].dLuv_l_mean;
				double du = pData[k].dLuv_u_mean;
				double dv = pData[k].dLuv_v_mean;

				pGraph->AddData( dl, du, iRed, iGreen, iBlue, iIndex );
			}
		}


		// set axis
		if( iDataType == _BHC_MID_XYZ_GRAPH_)
		{
			pGraph->SetAxis( 0.3, 0.5, 0.3, 0.5, 0.01, 0.01);
		}
		if( iDataType == _BHC_MID_HSV_GRAPH_)
		{
			pGraph->SetAxis( 0.0, 1.0, 0.0, 1.0, 0.1, 0.1);
		}
		if( iDataType == _BHC_MID_LUV_GRAPH_)
		{
			pGraph->SetAxis( 0.0, 1.0, 0.0, 1.0, 0.1, 0.1);
		}
	}



	pGraph->ShowWindow(SW_SHOW);
}


void CBHC_multiImageDlg::OnBnClickedButtonInspFolder()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetEvent(m_hEventInsp);
}


void CBHC_multiImageDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if( m_hThreadInsp )
	{
		delete m_hThreadInsp;
	}
	m_hThreadInsp = NULL;

}


void CBHC_multiImageDlg::Memo(CString str)
{
	m_ctrlMemo.InsertString(0, str);
}


bool CBHC_multiImageDlg::GetDirectoryList(CString strSrc, std::list<CString> *listFolder)
{
	// �˻� class
	CFileFind finder;


	// �Է� ���� ����
	CString strPath = strSrc + "\\*.*";
	if (!finder.FindFile(strPath))
	{
		return false;
	}


	//
	CString strFileName;
	BOOL bValid = TRUE;
	while (bValid)
	{
		bValid = finder.FindNextFile();

		// ������ ����
		if (finder.IsArchived())
		{

		}

		//  ������ ���
		if (finder.IsDirectory())
		{
			strFileName = finder.GetFileName();

			if (strFileName != "." && strFileName != "..")
			{
				listFolder->push_back(strFileName);
			}
		}
	}

	return true;
}