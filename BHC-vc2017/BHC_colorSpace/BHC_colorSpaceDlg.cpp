
// BHC_colorSpaceDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "BHC_colorSpace.h"
#include "BHC_colorSpaceDlg.h"
#include "afxdialogex.h"
#include "BHC_exel.h"
#include <float.h>
#include <math.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>



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


// CBHC_colorSpaceDlg 대화 상자

// 측정 thread
UINT threadProcLoad(LPVOID lParam)
{
	CBHC_colorSpaceDlg *pMain = (CBHC_colorSpaceDlg*)lParam;

	while (TRUE)
	{
		//
		WaitForSingleObject(pMain->m_hEventLoad, INFINITE);


		//
		switch(pMain->m_indexLoad)
		{
		case _BHC_RGB_:
			pMain->LoadRgbData();
			break;

		case _BHC_LUMINANCE_:
			pMain->LoadLuminanceData();
			break;

		case _BHC_XYZ_:
			pMain->LoadXYZData();
			break;

		case _BHC_MAKE_HOBJ_LUMINANCE_:
			pMain->MakeHobjLuminance();
			break;

		default:
			break;
		}


		//
		ResetEvent(pMain->m_hEventLoad);
	}

	return 0;
}



CBHC_colorSpaceDlg::CBHC_colorSpaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBHC_colorSpaceDlg::IDD, pParent)
	, m_uiDataWidth(4896)
	, m_uiDataHeight(3264)
	, m_rdoTarget(1)
	, m_rdoImageDepth(0)
	, m_uiDiv_width(20)
	, m_uiDiv_height(20)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_szData = CSize(100, 100 );

	m_pImageViewer = NULL;

	m_hEventLoad = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hThreadLoad = AfxBeginThread(threadProcLoad, (LPVOID)this);

	QueryPerformanceFrequency(&m_counter_Freq);
	QueryPerformanceCounter(&m_globalEndCounter);
	QueryPerformanceCounter(&m_globalEndCounter);

	m_pWndShow = NULL;
}


// CreateEvent 때문에 여기에 위치시킴...
#include "HalconCpp.h"
using namespace HalconCpp;


void CBHC_colorSpaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DATA_WIDTH, m_uiDataWidth);
	DDX_Text(pDX, IDC_EDIT_DATA_HEIGHT, m_uiDataHeight);
	DDX_Text(pDX, IDC_EDIT_DIV_WIDTH, m_uiDiv_width);
	DDX_Text(pDX, IDC_EDIT_DIV_HEIGHT, m_uiDiv_height);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
	DDX_Control(pDX, IDC_LIST1, m_ctrlListMemo);
	DDX_Control(pDX, IDC_TAB1, m_ctrlTab);
	DDX_Radio(pDX, IDC_RADIO_TARGET_DEFAULT, m_rdoTarget);
	DDX_Radio(pDX, IDC_RADIO_IMAGE_8BIT, m_rdoImageDepth);
}

BEGIN_MESSAGE_MAP(CBHC_colorSpaceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_IMAGE_RGB, &CBHC_colorSpaceDlg::OnBnClickedButtonOpenImageRgb)
	ON_BN_CLICKED(IDC_BUTTON_SET_DATA_SIZE, &CBHC_colorSpaceDlg::OnBnClickedButtonSetDataSize)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_DATA_LUMINANCE, &CBHC_colorSpaceDlg::OnBnClickedButtonOpenDataLuminance)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_HOBJ_LUMINANCE, &CBHC_colorSpaceDlg::OnBnClickedButtonSaveHobjLuminance)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CBHC_colorSpaceDlg::OnBnClickedButtonTest)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CBHC_colorSpaceDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_DATA_XYZ, &CBHC_colorSpaceDlg::OnBnClickedButtonOpenDataXyz)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_HOBJ_LUMINANCE_MULTI, &CBHC_colorSpaceDlg::OnBnClickedButtonSaveHobjLuminanceMulti)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_TARGET_DEFAULT, IDC_RADIO_TARGET_LUMITOP, CBHC_colorSpaceDlg::OnRadioButtonTarget)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_IMAGE_8BIT, IDC_RADIO_IMAGE_16BIT, CBHC_colorSpaceDlg::OnRadioButtonImageDepth)
	ON_BN_CLICKED(IDC_BUTTON_GEN_GRAPH, &CBHC_colorSpaceDlg::OnBnClickedButtonGenGraph)
END_MESSAGE_MAP()


// CBHC_colorSpaceDlg 메시지 처리기

BOOL CBHC_colorSpaceDlg::OnInitDialog()
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
		GetDlgItem(IDC_STATIC_VIEW_IMAGE)->GetWindowRect(&rtVeiwRegion);
		ScreenToClient(rtVeiwRegion);

		if (!m_pImageViewer->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
			rtVeiwRegion, this, AFX_IDW_PANE_FIRST, NULL))
		{
			TRACE0("Failed to create view window\n");
			return -1;
		}
	}

	// gen tab
	InitTabCtrl();

	// gen progress
	m_ctrlProgress.SetRange( 0, 100 );


	//
	SetTimer(1, 500, NULL);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBHC_colorSpaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBHC_colorSpaceDlg::OnPaint()
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
HCURSOR CBHC_colorSpaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBHC_colorSpaceDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 
	if( m_hThreadLoad )
	{
		delete m_hThreadLoad;
	}
	m_hThreadLoad = NULL;

}


int CBHC_colorSpaceDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRuntimeClass  *pObject;
	pObject = RUNTIME_CLASS(CImageViewer);
	m_pImageViewer = (CImageViewer*)pObject->CreateObject();


	return 0;
}


void CBHC_colorSpaceDlg::OnBnClickedButtonOpenImageRgb()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_indexLoad = _BHC_RGB_;

	SetEvent(m_hEventLoad);
}


void CBHC_colorSpaceDlg::OnBnClickedButtonSetDataSize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CBHC_colorSpaceDlg::OnBnClickedButtonOpenDataLuminance()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

//	LoadLuminanceData();

	m_indexLoad = _BHC_LUMINANCE_;

	SetEvent(m_hEventLoad);
}

BOOL CBHC_colorSpaceDlg::LoadLuminanceData()
{
	// dialog 
	CFileDialog dlg(TRUE,
					NULL,
					NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					_T("image files(*.csv,*.xlsx,*.txt)|*.csv;*.xlsx;*.txt"),
					this);


	CString strFile;
	if (dlg.DoModal() == IDOK)
	{
		strFile = dlg.GetFolderPath() + "\\" + dlg.GetFileName();
	} else {
		return FALSE;
	}

	LoadLuminanceData(strFile);


	return TRUE;
}


BOOL CBHC_colorSpaceDlg::LoadLuminanceData(CString strFileName)
{
	return TRUE;
}


BOOL CBHC_colorSpaceDlg::SaveLuminanceData(CString strFileName)
{
	memo(_T("Save data..."));

	// get data
	stColor *pXyz = m_colorInfo.GetXyz();


	// gen buffer
	HImage hiSave;
	hiSave.GenImageConst( "real", pXyz->iWidth, pXyz->iHeight );

	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;
	GetImagePointer1(hiSave, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	float *pHData = (float*)hv_Pointer.L();


	// copy
	for( int i=0 ; i<pXyz->iWidth * pXyz->iHeight ; i++)
	{
		pHData[i] = (float)(pXyz->pData[_BHC_COLOR_XYY_Y][i]);
	}


	CHAR strMB[1024];
	WideCharToMultiByte( CP_ACP, 0, strFileName, -1, strMB, 1024, NULL, NULL );

	hiSave.WriteImage("hobj", 0, strMB );


	memo(_T("Save data...OK"));


	return TRUE;
}


BOOL CBHC_colorSpaceDlg::LoadRgbData()
{
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("image files(*.bmp,*.tif)|*.bmp;*.tif|"),
		this);


	if (dlg.DoModal() == IDOK)
	{
		// file path
		CString strFile;
		strFile = dlg.GetFolderPath() + "\\" + dlg.GetFileName();

		// unicode to multi-byte
		char cFile[1024];
		WideCharToMultiByte( CP_ACP, 0, strFile, -1, cFile, 1024, NULL, NULL );

		// convert type
		std::string strFile2;
		std::ostringstream strStream;
		strStream<<cFile;
		strFile2 = strStream.str();

		// read file
		memo(_T("load image..."));
		
		cv::Mat cvImage = cv::imread(strFile2, cv::IMREAD_COLOR | cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR );
		
		memo(_T("load image...OK"));



		// 8bit 영상일 때
		memo(_T("Set data..."));

		switch( cvImage.type() )
		{
		case CV_8UC3:	// 8bit color
			m_colorInfo.SetRgb( cvImage.data, cvImage.cols, cvImage.rows );
			break;

		case CV_16UC3:
			m_colorInfo.SetRgb( (unsigned short*)cvImage.data, cvImage.cols, cvImage.rows );
			break;

		default:
			memo(_T("Set data...error - type"));
			return FALSE;
		}		
		
		memo(_T("Set data...OK"));


		// 16 bit 영상일 때


		//
		memo(_T("draw view image..."));
		
		MakeViewImage();
		
		memo(_T("draw view image...OK"));
	}


	return TRUE;
}


BOOL CBHC_colorSpaceDlg::LoadXYZData()
{
	//
	// read data
	//
	switch( m_rdoTarget )
	{
		case _BHC_TARGET_SYSTEM_DEFAULT_:
			return FALSE;
			break;

		case _BHC_TARGET_SYSTEM_RADIANT_:
			if( LoadXYZData_radiant() == FALSE )
			{
				return FALSE;
			}
			break;

		case _BHC_TARGET_SYSTEM_LUMITOP_:
			if( LoadXYZData_lumitop() == FALSE )
			{
				return FALSE;
			}
			break;

		default:
			return FALSE;
	}



	//
	memo(_T("Set image : start"));

	MakeViewImage();

	memo(_T("Set image : end"));


	// display


	return TRUE;
}


BOOL CBHC_colorSpaceDlg::LoadXYZData_default()
{
	return TRUE;
}


BOOL CBHC_colorSpaceDlg::LoadXYZData_radiant()
{
	// dialog 
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("image files(*.csv,*.xlsx,*.txt)|*.csv;*.xlsx;*.txt"),
		this);


	// 파일 위치, 이름 획득
	CString strFileName;
	CString strPath;
	if (dlg.DoModal() == IDOK)
	{
		strPath = dlg.GetFolderPath();
		strFileName = dlg.GetFileName();
	} else {
		return FALSE;
	}


	// 상위 디렉토리 획득
	int iRootPos = strPath.ReverseFind('\\');
	CString strRootPath;
	strRootPath = strPath.Left(iRootPos);


	// 
	double *pXyy[3];
	int iWidth, iHeight;

	for( int i=0 ; i<3 ; i++ )	// X, Y, Z 각각을 읽는다.
	{
		CString strFullName;

		if( i == 0 )
		{
			strFullName.Format(strRootPath + _T("\\Tristimulus X\\") + strFileName );
		}
		if( i == 1 )
		{
			strFullName.Format(strRootPath + _T("\\Tristimulus Y\\") + strFileName );
		}
		if( i == 2 )
		{
			strFullName.Format(strRootPath + _T("\\Tristimulus Z\\") + strFileName );
		}
		

		// read data
		memo(_T("Load data..."));

		CBHC_exel exel;
		exel.m_pProgess = &g_dProgress;

		exel.ReadRadient(strFullName);

		stRadient *pRadientData = exel.GetRadientData();
		

		// copy data
		iWidth = pRadientData->iWidth;
		iHeight = pRadientData->iHeight;

		pXyy[i] = new double[iWidth * iHeight ];

		memcpy( pXyy[i], pRadientData->pData, sizeof(double)*iWidth * iHeight);

		memo(_T("Load data...OK"));
	}


	//
	//
	//
	memo(_T("Set data..."));

	m_colorInfo.SetXYZ( pXyy[0], pXyy[1], pXyy[2], iWidth, iHeight );

	memo(_T("Set data...OK"));


	for( int i=0 ; i<3 ; i++ )
	{
		delete[] pXyy[i];
	}


	return TRUE;
}


BOOL CBHC_colorSpaceDlg::LoadXYZData_lumitop()
{
	// dialog 
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("image files(*.csv,*.xlsx,*.txt)|*.csv;*.xlsx;*.txt"),
		this);


	CString strFile;
	if (dlg.DoModal() == IDOK)
	{
		strFile = dlg.GetFolderPath() + "\\" + dlg.GetFileName();
	} else {
		return FALSE;
	}


	// read data
	memo(_T("Load data..."));

	CBHC_exel exel;
	exel.m_pProgess = &g_dProgress;

	exel.ReadLumitop(strFile);

	stLumitop *pLumitopData = exel.GetLumitopData();

	memo(_T("Load data...OK"));


	//
	memo(_T("Set data..."));

	m_colorInfo.SetXYZ(pLumitopData->pDataX, pLumitopData->pDataY, pLumitopData->pDataZ, pLumitopData->iWidth, pLumitopData->iHeight );

	memo(_T("Set data...OK"));


	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	MakeHobjLuminance
//
//	Radiant data를 읽어서 저장하는 특별함수
//
//////////////////////////////////////////////////////////////////////////
BOOL CBHC_colorSpaceDlg::MakeHobjLuminance()
{
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


	CString strDirectory = pszPathname;


	//
	for( int i=0 ; i<27 ; i++ )
	{
		CString strFileName;
		if( i<9 )
		{
			strFileName.Format(_T("0%d"), i+1);
		} else
		{
			strFileName.Format(_T("%2d"), i+1);
		}

		LoadLuminanceData(strDirectory + "\\" + strFileName + _T(".txt") );

		SaveLuminanceData(strDirectory + "\\" + strFileName);
	}

	return TRUE;
}


void CBHC_colorSpaceDlg::memo(CString strMemo )
{
	QueryPerformanceCounter(&m_globalEndCounter);

	CString strTime;
	strTime.Format(_T("[%f sec]"), (double)(m_globalEndCounter.QuadPart - m_globalStartCounter.QuadPart) / (double)m_counter_Freq.QuadPart );

	m_ctrlListMemo.InsertString(0, strMemo + strTime);

	QueryPerformanceCounter(&m_globalStartCounter);
}


// BOOL CBHC_colorSpaceDlg::SetLuminaceData( int iWidth, int iHeight, double *pData )
// {
// 	return TRUE;
// }


// BOOL CBHC_colorSpaceDlg::SetXYZData( int iWidth, int iHeight, double *pData, int index )
// {
// 	m_XYZ.iWidth = iWidth;
// 	m_XYZ.iHeight = iHeight;
// 
// 
// 	switch( index )
// 	{
// 	case _BHC_XYZ_X_:
// 		{
// 			// gen buffer
// 			if( m_XYZ.pDataX )
// 			{
// 				delete[] m_XYZ.pDataX;
// 			}
// 			m_XYZ.pDataX = new double[iWidth*iHeight];
// 
// 
// 			// set data
// 			memcpy( m_XYZ.pDataX, pData, sizeof(double)*iWidth*iHeight );
// 
// 		}
// 		break;
// 
// 	case _BHC_XYZ_Y_:
// 		{
// 			// gen buffer
// 			if( m_XYZ.pDataY )
// 			{
// 				delete[] m_XYZ.pDataY;
// 			}
// 			m_XYZ.pDataY = new double[iWidth*iHeight];
// 
// 
// 			// set data
// 			memcpy( m_XYZ.pDataY, pData, sizeof(double)*iWidth*iHeight );
// 
// 		}
// 		break;
// 
// 	case _BHC_XYZ_Z_:
// 		{
// 			// gen buffer
// 			if( m_XYZ.pDataZ )
// 			{
// 				delete[] m_XYZ.pDataZ;
// 			}
// 			m_XYZ.pDataZ = new double[iWidth*iHeight];
// 
// 
// 			// set data
// 			memcpy( m_XYZ.pDataZ, pData, sizeof(double)*iWidth*iHeight );
// 
// 		}
// 		break;
// 
// 	default:
// 		return FALSE;
// 	}
// 
// 	return TRUE;
// }



//////////////////////////////////////////////////////////////////////////
//
//	MakeViewImage
//
//	view Image 를 생성한다.
//
//////////////////////////////////////////////////////////////////////////
BOOL CBHC_colorSpaceDlg::MakeViewImage()
{
	int iWidth, iHeight;
	if( m_colorInfo.GetSize( &iWidth, &iHeight ) == false )
	{
		return FALSE;
	}


	stRgb *pRgb = m_colorInfo.GetRgb();
	if( pRgb == NULL )
	{
		return FALSE;
	}

// 	m_pImageViewer->SetImage(iWidth, iHeight, pRgb->pData_8[_BHC_COLOR_RGB_R], pRgb->pData_8[_BHC_COLOR_RGB_G], pRgb->pData_8[_BHC_COLOR_RGB_B] );

	m_pImageViewer->SetColorImage(pRgb->pData, pRgb->iWidth, pRgb->iHeight);


	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	MakeImageData
//
//	Image data를 생성한다.
//
//////////////////////////////////////////////////////////////////////////
// BOOL CBHC_colorSpaceDlg::MakeImageData(int iSource)
// {
// 	BYTE *pImage;
// 	double dMin = DBL_MAX;
// 	double dMax = DBL_MIN;
// 	double dRange;
// 
// 
// 	switch(iSource)
// 	{
// 	case _BHC_LUMINANCE_:
// 
// 		// range
// 		for( int y=0 ; y<m_luminance.iHeight ; y++ )
// 		{
// 			for( int x=0 ; x<m_luminance.iWidth ; x++ )
// 			{
// 				double dValue = m_luminance.pData[ x + y*m_luminance.iWidth ];
// 
// 				dMax = max( dValue, dMax );
// 				dMin = min( dValue, dMin );
// 			}
// 		}
// 
// 		dRange = dMax - dMin;
// 
// 		//
// 		pImage = new BYTE[m_luminance.iWidth * m_luminance.iHeight];
// 		for( int y=0 ; y<m_luminance.iHeight ; y++ )
// 		{
// 			for( int x=0 ; x<m_luminance.iWidth ; x++ )
// 			{
// 				// lum 값
// 				double dValue = m_luminance.pData[ x + y*m_luminance.iWidth ];
// 
// 				// byte로 변환
// 				BYTE gray = (BYTE)( 255.0 * ( dValue - dMin ) / dRange );
// 
// 				pImage[ x + y*m_luminance.iWidth ] = gray;
// 			}
// 		}
// 
// 		m_pImageViewer->SetImage(m_luminance.iWidth, m_luminance.iHeight, pImage);
// 
// 		delete[] pImage;
// 
// 		break;
// 
// 	case _BHC_RGB_:
// 		break;
// 
// 	case _BHC_XYZ_:
// 		// range
// 		for( int y=0 ; y<m_XYZ.iHeight ; y++ )
// 		{
// 			for( int x=0 ; x<m_XYZ.iWidth ; x++ )
// 			{
// 				double dValue = m_XYZ.pDataY[ x + y*m_XYZ.iWidth ];
// 
// 				dMax = max( dValue, dMax );
// 				dMin = min( dValue, dMin );
// 			}
// 		}
// 
// 		dRange = dMax - dMin;
// 
// 		//
// 		pImage = new BYTE[m_XYZ.iWidth * m_XYZ.iHeight];
// 		for( int y=0 ; y<m_XYZ.iHeight ; y++ )
// 		{
// 			for( int x=0 ; x<m_XYZ.iWidth ; x++ )
// 			{
// 				// lum 값
// 				double dValue = m_XYZ.pDataY[ x + y*m_XYZ.iWidth ];
// 
// 				// byte로 변환
// 				BYTE gray = (BYTE)( 255.0 * ( dValue - dMin ) / dRange );
// 
// 				pImage[ x + y*m_XYZ.iWidth ] = gray;
// 			}
// 		}
// 
// 		m_pImageViewer->SetImage(m_XYZ.iWidth, m_XYZ.iHeight, pImage);
// 
// 		delete[] pImage;
// 
// 		break;
// 
// 	default:
// 		AfxMessageBox(_T("Error : MakeImageData"));
// 		return FALSE;
// 	}
// 
// 	return TRUE;
// }


//////////////////////////////////////////////////////////////////////////
//
//	MakeColorData
//
//	XYZ data로 부터 xyY, LUV data를 생성한다.
//
//////////////////////////////////////////////////////////////////////////
// BOOL CBHC_colorSpaceDlg::MakeData_xyY()
// {
// 	// xyY
// 	m_XYY.iWidth = m_XYZ.iWidth;
// 	m_XYY.iHeight = m_XYZ.iHeight;
// 
// 	if( m_XYY.pDataX )
// 	{
// 		delete[] m_XYY.pDataX;
// 	}
// 	m_XYY.pDataX = new double[m_XYY.iWidth * m_XYY.iHeight];
// 
// 	if( m_XYY.pDataY )
// 	{
// 		delete[] m_XYY.pDataY;
// 	}
// 	m_XYY.pDataY = new double[m_XYY.iWidth * m_XYY.iHeight];
// 
// 
// 	for( int y=0 ; y<m_XYZ.iHeight ; y++ )
// 	{
// 		for( int x=0 ; x<m_XYZ.iWidth ; x++ )
// 		{
// 			int iPos = x + y*m_XYZ.iWidth;
// 
// 			double xx = m_XYZ.pDataX[iPos];
// 			double yy = m_XYZ.pDataY[iPos];
// 			double zz = m_XYZ.pDataZ[iPos];
// 
// 			m_XYY.pDataX[iPos] = xx/(xx+yy+zz);
// 			m_XYY.pDataY[iPos] = yy/(xx+yy+zz);
// 		}
// 	}
// 
// 
// 	return TRUE;
// }


//////////////////////////////////////////////////////////////////////////
//
//	MakeData_luminance
//
//	XYZ data로 부터 luminance 를 생성한다.
//
//////////////////////////////////////////////////////////////////////////
// BOOL CBHC_colorSpaceDlg::MakeData_luminance()
// {
// 	// xyY
// 	m_luminance.iWidth = m_XYZ.iWidth;
// 	m_luminance.iHeight = m_XYZ.iHeight;
// 
// 	if( m_luminance.pData )
// 	{
// 		delete[] m_luminance.pData;
// 	}
// 	m_luminance.pData = new double[m_luminance.iWidth * m_luminance.iHeight];
// 
// 
// 	for( int y=0 ; y<m_XYZ.iHeight ; y++ )
// 	{
// 		for( int x=0 ; x<m_XYZ.iWidth ; x++ )
// 		{
// 			int iPos = x + y*m_XYZ.iWidth;
// 
// 			double yy = m_XYZ.pDataY[iPos];
// 
// 			m_luminance.pData[iPos] = yy;
// 		}
// 	}
// 
// 
// 	return TRUE;
// }



//////////////////////////////////////////////////////////////////////////
//
//	MakeGraph_xyY
//
//	
//
//////////////////////////////////////////////////////////////////////////
// BOOL CBHC_colorSpaceDlg::MakeGraph_xyY()
// {
// 	//
// 	CRect rtRoi;
// 	if( m_pImageViewer->GetRoi(&rtRoi) == FALSE )
// 	{
// 		return FALSE;
// 	}
// 
// 
// 	int iDivWidth = GetDlgItemInt(IDC_EDIT_DIV_WIDTH);
// 	int iDivHeight = GetDlgItemInt(IDC_EDIT_DIV_HEIGHT);
// 
// 	int iStepWidth = rtRoi.Width()/iDivWidth;
// 	int iStepHeight = rtRoi.Height()/iDivHeight;;
// 
// 	//
// 	stColor *pXyy = m_colorInfo.GetXyy();
// 	int iWidth = pXyy->iWidth;
// 	int iHeight = pXyy->iHeight;
// 
// 
// 	// clear
// 	m_pageXYY.m_pGraph->ClearData();
// 
// 	// range of axis
// 	m_pageXYY.m_pGraph->SetRange( 0, 1.0, 0, 1.0 );
// 	m_pageXYY.m_pGraph->SetGrid(0.1, 0.1);
// 
// 
// 	// put data : all region
// 	for( int yIndex = 0 ; yIndex < iDivHeight ; yIndex++ )
// 	{
// 		for( int xIndex = 0 ; xIndex < iDivWidth ; xIndex++ )
// 		{
// 			double dSumX = 0.0;
// 			double dSumY = 0.0;
// 			int iCnt = 0;
// 			for( int y=rtRoi.top + yIndex*iStepHeight ; y<rtRoi.top + (yIndex+1)*iStepHeight ; y++ )
// 			{
// 				for( int x=rtRoi.left + xIndex*iStepWidth ; x<rtRoi.left + (xIndex+1)*iStepWidth ; x++ )
// 				{
// 					int iPos = x + y*iWidth;
// 
// 					dSumX += pXyy->pData[_BHC_COLOR_XYY_X][iPos];
// 					dSumY += pXyy->pData[_BHC_COLOR_XYY_Y][iPos];
// 
// 					iCnt++;
// 				}
// 			}
// 
// 			double dX_avg = dSumX/iCnt;
// 			double dY_avg = dSumY/iCnt;
// 
// 			m_pageXYY.m_pGraph->AddData(dX_avg, dY_avg);
// 		}
// 	}
// 
// 
// 
// 	//
// 	// Roi 영역의 값 입력
// 	//
// 	CPoint ptInterest;
// 	if( m_pImageViewer->GetPoi(&ptInterest) )
// 	{
// 		double dX_sum = 0.0;
// 		double dY_sum = 0.0;
// 		int iCnt = 0;
// 
// 		for( int y = max( ptInterest.y - iStepHeight/2, 0 ) ; y < min( ptInterest.y + iStepHeight/2, iHeight )  ; y++ )
// 		{
// 			for( int x = max( ptInterest.x - iStepWidth/2, 0 ) ; x < min( ptInterest.x + iStepWidth/2, iWidth ) ; x++ )
// 			{
// 				int iPos = x + y*iWidth;
// 
// 				dX_sum += pXyy->pData[_BHC_COLOR_XYY_X][iPos];
// 				dY_sum += pXyy->pData[_BHC_COLOR_XYY_Y][iPos];
// 
// 				iCnt++;
// 			}
// 		}
// 
// 		if( iCnt >0 )
// 		{
// 			double dX_avg = dX_sum/iCnt;
// 			double dY_avg = dY_sum/iCnt;
// 
// 			m_pageXYY.m_pGraph->AddData2(dX_avg, dY_avg);
// 		}
// 
// 	}
// 
// 
// 	m_pageXYY.Invalidate(TRUE);
// 
// 	return TRUE;
// }


//////////////////////////////////////////////////////////////////////////
//
//	MakeGraph
//
//	그래프 생성
//
//////////////////////////////////////////////////////////////////////////
BOOL CBHC_colorSpaceDlg::MakeGraph(int iType)
{
	GenGraph_xyy();

	return TRUE;
}


void CBHC_colorSpaceDlg::OnBnClickedButtonSaveHobjLuminance()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	SaveLuminanceData(_T("d:\\test"));

/*
	HImage hiSave;
	hiSave.GenImageConst( "real", m_luminance.iWidth, m_luminance.iHeight );

	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;
	GetImagePointer1(hiSave, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	float *pHData = (float*)hv_Pointer.L();

	for( int i=0 ; i<m_luminance.iWidth * m_luminance.iHeight ; i++)
	{
		pHData[i] = m_luminance.pData[i];
	}


	hiSave.WriteImage("hobj", 0, "d:\\test" );
*/

}


void CBHC_colorSpaceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_ctrlProgress.SetPos((int)(g_dProgress * 100));

	CDialogEx::OnTimer(nIDEvent);
}


void CBHC_colorSpaceDlg::OnBnClickedButtonTest()
{



}


void CBHC_colorSpaceDlg::InitTabCtrl()
{
	m_ctrlTab.InsertItem(0, _T("xyY"));
	m_ctrlTab.InsertItem(1, _T("LUV"));


	CRect rtTab;
	m_ctrlTab.GetClientRect(&rtTab);

	m_pageXYY.Create(IDD_PAGE_XYY, &m_ctrlTab);
	m_pageXYY.SetWindowPos(NULL, 5, 25, rtTab.Width()-12, rtTab.Height()-33, SWP_SHOWWINDOW | SWP_NOZORDER );
	m_pWndShow = &m_pageXYY;

	m_pageLUV.Create(IDD_PAGE_LUV, &m_ctrlTab);
	m_pageLUV.SetWindowPos(NULL, 5, 25, rtTab.Width()-12, rtTab.Height()-33, SWP_SHOWWINDOW | SWP_NOZORDER );
	m_pWndShow = &m_pageLUV;

	// 선택 탭 정보얻기
	ShowTabCtrl();


	//
	m_pageXYY.init();
}


void CBHC_colorSpaceDlg::ShowTabCtrl()
{
	// 현재 것 숨기기
	if( m_pWndShow )
	{
		m_pWndShow->ShowWindow(SW_HIDE);
		m_pWndShow = NULL;
	}

	// 선택 탭 정보얻기
	int iIndex = m_ctrlTab.GetCurSel();

	// 표시하기
	switch( iIndex )
	{
	case 0:
		m_pageXYY.ShowWindow(SW_SHOW);
		m_pageLUV.ShowWindow(SW_HIDE);
		m_pWndShow = &m_pageXYY;
		break;

	case 1:
		m_pageXYY.ShowWindow(SW_HIDE);
		m_pageLUV.ShowWindow(SW_SHOW);
		m_pWndShow = &m_pageLUV;
		break;

	}
}


//////////////////////////////////////////////////////////////////////////
//
//	GenGraph_xyy
//
//	xyy graph 생성
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_colorSpaceDlg::GenGraph_xyy()
{
	//
	CRect rtRoi;
	if( m_pImageViewer->GetRoi(&rtRoi) == FALSE )
	{
		return FALSE;
	}


	int iDivWidth = GetDlgItemInt(IDC_EDIT_DIV_WIDTH);
	int iDivHeight = GetDlgItemInt(IDC_EDIT_DIV_HEIGHT);

	int iStepWidth = rtRoi.Width()/iDivWidth;
	int iStepHeight = rtRoi.Height()/iDivHeight;;




	// clear
	m_pageXYY.m_pGraph->ClearData();

	// range of axis
	m_pageXYY.m_pGraph->SetRange( 0, 1.0, 0, 1.0 );
	m_pageXYY.m_pGraph->SetGrid(0.1, 0.1);


	// put data
	stColor *pXyy = m_colorInfo.GetXyy();
	stColor *pXyz = m_colorInfo.GetXyz();
	stRgb *pRgb = m_colorInfo.GetRgb();
	if( pXyy == NULL )
	{
		return false;
	}
	int iWidth = pXyy->iWidth;
	int iHeight = pXyy->iHeight;

	for( int yIndex = 0 ; yIndex < iDivHeight ; yIndex++ )
	{
		for( int xIndex = 0 ; xIndex < iDivWidth ; xIndex++ )
		{
			double dSumX = 0.0;
			double dSumY = 0.0;
			int iCnt = 0;
			for( int y=rtRoi.top + yIndex*iStepHeight ; y<rtRoi.top + (yIndex+1)*iStepHeight ; y++ )
			{
				for( int x=rtRoi.left + xIndex*iStepWidth ; x<rtRoi.left + (xIndex+1)*iStepWidth ; x++ )
				{
					int iPos = x + y*iWidth;

					dSumX += pXyy->pData[_BHC_COLOR_XYY_X][iPos];
					dSumY += pXyy->pData[_BHC_COLOR_XYY_Y][iPos];

					iCnt++;
				}
			}

			double dX_avg = dSumX/iCnt;
			double dY_avg = dSumY/iCnt;

			m_pageXYY.m_pGraph->AddData(dX_avg, dY_avg);
		}
	}



	//
	//
	//
	CPoint ptInterest;
	if( m_pImageViewer->GetPoi(&ptInterest) )
	{
		double dX_sum = 0.0;
		double dY_sum = 0.0;
		int iCnt = 0;

		for( int y = MAX( ptInterest.y - iStepHeight/2, 0 ) ; y < MIN( ptInterest.y + iStepHeight/2, iHeight )  ; y++ )
		{
			for( int x = MAX( ptInterest.x - iStepWidth/2, 0 ) ; x < MIN( ptInterest.x + iStepWidth/2, iWidth ) ; x++ )
			{
				int iPos = x + y*iWidth;

				dX_sum += pXyy->pData[_BHC_COLOR_XYY_X][iPos];
				dY_sum += pXyy->pData[_BHC_COLOR_XYY_Y][iPos];

				iCnt++;
			}
		}

		if( iCnt >0 )
		{
			double dX_avg = dX_sum/iCnt;
			double dY_avg = dY_sum/iCnt;

			m_pageXYY.m_pGraph->AddData2(dX_avg, dY_avg);
		}

	}


	m_pageXYY.Invalidate(TRUE);

	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	GenGraph_luv
//
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_colorSpaceDlg::GenGraph_luv()
{
	return true;
}


void CBHC_colorSpaceDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowTabCtrl();

	*pResult = 0;
}


void CBHC_colorSpaceDlg::OnBnClickedButtonOpenDataXyz()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_indexLoad = _BHC_XYZ_;

	SetEvent(m_hEventLoad);
}


//////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////
void CBHC_colorSpaceDlg::OnBnClickedButtonSaveHobjLuminanceMulti()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_indexLoad = _BHC_MAKE_HOBJ_LUMINANCE_;

	SetEvent(m_hEventLoad);


	/*//
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


	CString strDirectory = pszPathname;


	//
	for( int i=0 ; i<27 ; i++ )
	{
		CString strFileName;
		if( i<9 )
		{
			strFileName.Format(_T("0%d.txt"), i+1);
		} else
		{
			strFileName.Format(_T("%2d.txt"), i+1);
		}

		LoadLuminanceData(strDirectory + "\\" + strFileName );

		strFileName.Format(_T("%2d"), i+1);
		SaveLuminanceData(strDirectory + "\\" + strFileName);
	}*/

}


void CBHC_colorSpaceDlg::OnRadioButtonTarget(UINT iValue)
{
	UpdateData(TRUE);
}


void CBHC_colorSpaceDlg::OnRadioButtonImageDepth(UINT iValue)
{
	switch( iValue )
	{
	case IDC_RADIO_IMAGE_8BIT:
		m_rdoImageDepth = 0;
		break;

	case IDC_RADIO_IMAGE_16BIT:
		m_rdoImageDepth = 1;
		break;
	}
}


void CBHC_colorSpaceDlg::OnBnClickedButtonGenGraph()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MakeGraph(0);
}


