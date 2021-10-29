
// BHC_featureAnalysisDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "BHC_featureAnalysis.h"
#include "BHC_featureAnalysisDlg.h"
#include "afxdialogex.h"

#include "../common/BHC_exel.h"

#define DEFAULT_FILE "d:\\lena_color.bmp"
// #define DEFAULT_FILE "d:\\BHC.bmpBHC.bmp"


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


// CBHC_featureAnalysisDlg 대화 상자




CBHC_featureAnalysisDlg::CBHC_featureAnalysisDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBHC_featureAnalysisDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBHC_featureAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// 	DDX_Control(pDX, IDC_STATIC_DATA, m_Grid);             // associate the grid window with a C++ object

	DDX_Control(pDX, IDC_COMBO_TARGET, m_cbType_in);
	DDX_Control(pDX, IDC_COMBO_AXIS_X, m_cbAxis_x);
	DDX_Control(pDX, IDC_COMBO_AXIS_Y, m_cbAxis_y);
	DDX_Control(pDX, IDC_COMBO_RESULT, m_cbType_out);
	DDX_Control(pDX, IDC_COMBO_IMAGE_PATH, m_cbImageFilePath);
}

BEGIN_MESSAGE_MAP(CBHC_featureAnalysisDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CBHC_featureAnalysisDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SET_AXIS, &CBHC_featureAnalysisDlg::OnBnClickedButtonSetAxis)
	ON_BN_CLICKED(IDC_BUTTON_DRAW_GRAPH, &CBHC_featureAnalysisDlg::OnBnClickedButtonDrawGraph)
	ON_BN_CLICKED(IDC_BUTTON_SET_INTEREST, &CBHC_featureAnalysisDlg::OnBnClickedButtonSetInterest)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CBHC_featureAnalysisDlg::OnBnClickedButtonCalculate)
	ON_MESSAGE(WM_GRAPH_SELECTION , OnUserFunc)
	ON_BN_CLICKED(IDC_BUTTON_RESET_INTEREST2, &CBHC_featureAnalysisDlg::OnBnClickedButtonResetInterest2)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_RULE, &CBHC_featureAnalysisDlg::OnBnClickedButtonApplyRule)
	ON_BN_CLICKED(IDC_BUTTON_LEARN, &CBHC_featureAnalysisDlg::OnBnClickedButtonLearn)
	ON_BN_CLICKED(IDC_BUTTON_PREDICT, &CBHC_featureAnalysisDlg::OnBnClickedButtonPredict)
END_MESSAGE_MAP()


// CBHC_featureAnalysisDlg 메시지 처리기

BOOL CBHC_featureAnalysisDlg::OnInitDialog()
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

	// image veiwer
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

		m_pImageViewer->LoadImage(_T(DEFAULT_FILE));
	}

	// graph viewer
	{
		CRect rtVeiwRegion;
		GetDlgItem(IDC_STATIC_2D_GRAPH)->GetWindowRect(&rtVeiwRegion);
		ScreenToClient(rtVeiwRegion);

		m_graphPage.Create(IDD_PAGE_GRAPH, this);
		m_graphPage.SetWindowPos(NULL, rtVeiwRegion.left, rtVeiwRegion.top, rtVeiwRegion.Width(), rtVeiwRegion.Height(), SWP_SHOWWINDOW | SWP_NOZORDER );

		m_graphPage.init();

		m_graphPage.SetMessage(this, WM_GRAPH_SELECTION);
	}

	// data viewer
	{
		CRect rtVeiwRegion;
		GetDlgItem(IDC_STATIC_DATA)->GetWindowRect(&rtVeiwRegion);

		ScreenToClient(rtVeiwRegion);

		m_Grid.Create(rtVeiwRegion, this, IDC_STATIC_DATA);

		initGrid();
	}

	//
#ifdef _USE_PYTHON_
	{
//		m_py.initialize("PyTest");
		m_py.initialize("PyML");
	}
#endif


	SetDlgItemText(IDC_EDIT_TARGET, _T("Dent"));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBHC_featureAnalysisDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBHC_featureAnalysisDlg::OnPaint()
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
HCURSOR CBHC_featureAnalysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CBHC_featureAnalysisDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	{
		CRuntimeClass  *pObject;
		pObject = RUNTIME_CLASS(CImageViewer);
		m_pImageViewer = (CImageViewer*)pObject->CreateObject();
	}


// 	{
// 		CRuntimeClass  *pObject;
// 		pObject = RUNTIME_CLASS(CBHC_graph2D);
// 		m_pGraph = (CBHC_graph2D*)pObject->CreateObject();
// 	}


	return 0;
}



void CBHC_featureAnalysisDlg::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// get file path
	CString strFilePath;
	if( GetFilePath(&strFilePath) == false )
	{
		return;
	}


	// read data
	CBHC_exel exel;
	if( exel.Read(strFilePath) == false )
	{
		return;
	}


	// set data
	if( m_featureData.SetData(	exel.GetData(), exel.GetWidth(), exel.GetHeight() ) == false )
	{
		return;
	}


	// update data grid
	UpdateGrid();

	UpdateFeatureData();
}


void CBHC_featureAnalysisDlg::OnBnClickedButtonSetAxis()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//
	int iIndex_in	= m_cbType_in.GetCurSel();
	int iIndex_out	= m_cbType_out.GetCurSel();

	if( m_featureData.SetIndex_target(iIndex_in, iIndex_out) == false )
	{
		return;
	}


	//
	std::string strTargetName;
	GetTargetType(&strTargetName);
	m_featureData.SetTargetName(strTargetName);
}


void CBHC_featureAnalysisDlg::OnBnClickedButtonDrawGraph()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateGraph();
}


void CBHC_featureAnalysisDlg::OnBnClickedButtonSetInterest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 정보얻기
// 	std::list<unsigned int> index = m_pGraph->GetIndex_selection();
	std::list<unsigned int> index = m_graphPage.GetGraph()->GetIndex_selection();

	// 설정
	m_featureData.SetIndex_interest( &index );


	// 갱신
	UpdateGraph();
}


void CBHC_featureAnalysisDlg::OnBnClickedButtonResetInterest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_featureData.Reset();

	UpdateGraph();
// 	m_pGraph->Reset();
}


bool CBHC_featureAnalysisDlg::GetFilePath(CString *pStrFileName)
{
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
// 		_T("image files(*.bmp,*.tif,*.tiff)|*.bmp;*.tif;*.tiff|"),
		_T("exel files(*.csv,*.tif,*.tiff)|*.csv|"),
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


//////////////////////////////////////////////////////////////////////////
//
//	GetTargetType
//
//	대상으로 하는 불량명을 UI로 부터 얻기
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_featureAnalysisDlg::GetTargetType( std::string *pStr )
{
	CString str;
	GetDlgItemText( IDC_EDIT_TARGET, str );
	
	char buf[2048];
	WideCharToMultiByte( CP_ACP, 0, str, -1, buf, 2048, NULL, NULL );


	*pStr = std::string(buf);


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	GetCheck_useFilePath
//
//	UI에서 정보가져오기
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_featureAnalysisDlg::GetCheck_useFilePath()
{
	if( ((CButton*)GetDlgItem(IDC_CHECK_IMAGE_PATH))->GetCheck()== TRUE )
	{
		return true;
	}

	return false;
}


void CBHC_featureAnalysisDlg::initGrid()
{
	// index 열/행 생성
	m_Grid.SetFixedColumnCount();
	m_Grid.SetFixedRowCount();

	// data 갯수 설정
	m_Grid.SetColumnCount();
	m_Grid.SetRowCount();

	// 색설정
	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

// 	m_Grid.SetFrameFocusCell(true);
// 
// 	m_Grid.SetFixedColumnSelection(TRUE);
// 	m_Grid.SetFixedRowSelection(TRUE);
	m_Grid.SetHeaderSort(TRUE);


	// set data
	m_Grid.SetCellType(1,1, RUNTIME_CLASS(CGridCell));
	m_Grid.SetItemText(1,1, _T("1"));
	m_Grid.SetItemState(1,1, m_Grid.GetItemState(1,1) & ~GVIS_READONLY);
	m_Grid.Invalidate();

}



//////////////////////////////////////////////////////////////////////////
//
//	UpdateGrid
//
//	현재 feature data를 graph에 입력한다.
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_featureAnalysisDlg::UpdateGrid()
{
	int iWidth	= m_featureData.GetWidth();
	int iHeight	= m_featureData.GetHeight();


		// make 
		int iWidth_grid		= iWidth + 1;
		int iHeight_grid	= iHeight + 1;
		m_Grid.SetColumnCount(iWidth_grid);
		m_Grid.SetRowCount(iHeight_grid);


	// set column index
	for( int x=0 ; x<iWidth ; x++ )
	{
		std::string str = m_featureData.GetColumns(x);

		WCHAR wBuf[2048];
		stringToChar(&str, wBuf, 2048 );

		m_Grid.SetItemText(0, x+1, (LPCTSTR)wBuf );
	}


	// set row index
	for( int y=0 ; y<iHeight ; y++ )
	{
		CString str;
		str.Format(_T("%d"), y );

		m_Grid.SetItemText(y+1, 0, str );
	}


	// set contents
	for( int y=0 ; y<iHeight ; y++ )
	{
		for( int x=0 ; x<iWidth ; x++ )
		{
			std::string str = m_featureData.GetContent(x,y);

			WCHAR wBuf[2048];
			stringToChar(&str, wBuf, 2048 );

			m_Grid.SetItemText(y+1,x+1, (LPCTSTR)wBuf );
		}
	}


	m_Grid.Invalidate();


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	UpdateGrid
//
//	현재 feature data를 graph에 입력한다.
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_featureAnalysisDlg::UpdateGraph()
{
	int iIndex_in	= m_cbType_in.GetCurSel();
	int iIndex_x	= m_cbAxis_x.GetCurSel();
	int iIndex_y	= m_cbAxis_y.GetCurSel();


	//
	// get data
	//

	std::vector<std::string> vecType;
	if( m_featureData.GetContent(iIndex_in, &vecType) == false )
	{
		return false;
	}

	std::vector<std::string> vecX;
	if( m_featureData.GetContent(iIndex_x, &vecX) == false )
	{
		return false;
	}

	std::vector<std::string> vecY;
	if( m_featureData.GetContent(iIndex_y, &vecY) == false )
	{
		return false;
	}

	std::string strTarget;
	GetTargetType(&strTarget);


	//
	// Set data
	//
// 	m_pGraph->ClearData();
	m_graphPage.GetGraph()->ClearData();

	double dAxis_x_min	= DBL_MAX;
	double dAxis_x_max	= -DBL_MAX;
	double dAxis_y_min	= DBL_MAX;
	double dAxis_y_max	= -DBL_MAX;

	for( int i=0 ; i<vecType.size() ; i++ )
	{
		std::string strType	= vecType[i];
		std::string strX	= vecX[i];
		std::string strY	= vecY[i];

		double dX = atof(strX.c_str());
		double dY = atof(strY.c_str());

		// 관심 정보만 올린다.
		if( m_featureData.CheckInterest(i) == false )
		{
			continue;
		}

		if( !strType.compare(strTarget))	
		{	// target 불량일 경우
// 			m_pGraph->AddData(dX, dY, 255, 0, 0, i);
			m_graphPage.GetGraph()->AddData(dX, dY, 255, 0, 0, i);
		} else {
// 			m_pGraph->AddData(dX, dY, 0, 0, 255, i);
			m_graphPage.GetGraph()->AddData(dX, dY, 0, 0, 255, i);
		}

		//
		if( dX > dAxis_x_max )
		{
			dAxis_x_max = dX;
		}
		if( dX < dAxis_x_min )
		{
			dAxis_x_min = dX;
		}
		if( dY > dAxis_y_max )
		{
			dAxis_y_max = dY;
		}
		if( dY < dAxis_y_min )
		{
			dAxis_y_min = dY;
		}
	}

// 	m_pGraph->SetRange(dAxis_x_min, dAxis_x_max, dAxis_y_min, dAxis_y_max);
	m_graphPage.SetAxis(dAxis_x_min, dAxis_x_max, dAxis_y_min, dAxis_y_max, 0.1 * (dAxis_x_max-dAxis_x_min), 0.1 * (dAxis_y_max-dAxis_y_min));
// 	m_graphPage.GetGraph()->SetRange(dAxis_x_min, dAxis_x_max, dAxis_y_min, dAxis_y_max);





	//////////////////////////////////////////////////////////////////////////
	// 사용 데이터 type
	//
	CGraphDlg *pGraphDlg = new CGraphDlg;
	pGraphDlg->Create(IDD_GRAPHDLG);
	pGraphDlg->init();

	pGraphDlg->ClearData();



	// 	for( int i=0 ; i<iImageNum ; i++ )
	for( int i=0 ; i<vecType.size() ; i++ )
	{
		std::string strType	= vecType[i];
		std::string strX	= vecX[i];
		std::string strY	= vecY[i];

		double dX = atof(strX.c_str());
		double dY = atof(strY.c_str());

		// 관심 정보만 올린다.
		if( m_featureData.CheckInterest(i) == false )
		{
			continue;
		}

		if( !strType.compare(strTarget))
		{
			pGraphDlg->AddData(dX, dY, 255, 0, 0, i);
		} else {
			pGraphDlg->AddData(dX, dY, 0, 0, 255, i);
		}
	}

	pGraphDlg->SetAxis(	dAxis_x_min, dAxis_x_max, 
		dAxis_y_min, dAxis_y_max, 
		10, 10 );


	pGraphDlg->ShowWindow(SW_SHOW);


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	ShowDefectInfo
//
//	불량(1개) 정보를 화면에 표시한다.
//
//
//	focus feature data 
//	load image file
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_featureAnalysisDlg::ShowDefectInfo(int iIndex)
{
	// at gird
	m_Grid.SetFocusCell(iIndex+1,0);


	CFeatureInfoDlg *pDlg = new CFeatureInfoDlg;
	pDlg->Create(IDD_DIALOG_FEATURE_INFO);
	pDlg->init();


	int iWidth	= 2;
	int iHeight	= m_featureData.GetWidth();

	pDlg->SetSize(iWidth, iHeight);

	// set column index : 첫번째열
	for( int x=0 ; x<iHeight ; x++ )
	{
		std::string str = m_featureData.GetColumns(x);

		pDlg->SetData(0, x,str);
	}


	// set contents : 두번째 열
	for( int x=0 ; x<iHeight ; x++ )
	{
		std::string str = m_featureData.GetContent(x,iIndex);

		pDlg->SetData(1, x, str);
	}


	// load image
	if( GetCheck_useFilePath() == true )
	{
		int iColIndex = m_cbImageFilePath.GetCurSel();

		std::string str = m_featureData.GetContent(iColIndex,iIndex);

		pDlg->Open(str);
	}

	pDlg->Renew();

	pDlg->ShowWindow(SW_SHOW);

	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	UpdateFeatureData
//
//	feature 선택 UI에 현재 feature data 정보를 반영한다.
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_featureAnalysisDlg::UpdateFeatureData()
{
	// defect 


	// axis 축
	std::vector<std::string>* pVecColum = m_featureData.GetColumns();
	
	m_cbType_in.Clear();
	m_cbType_out.Clear();
	m_cbAxis_x.Clear();
	m_cbAxis_y.Clear();
	m_cbImageFilePath.Clear();

	for( int i=0 ; i<pVecColum->size() ; i++ )
	{
		std::string str = (*pVecColum)[i];

		WCHAR wBuf[2048];
		stringToChar(&str, wBuf, 2048 );

		m_cbType_in.AddString((LPCTSTR)wBuf);
		m_cbType_out.AddString((LPCTSTR)wBuf);
		m_cbAxis_x.AddString((LPCTSTR)wBuf);
		m_cbAxis_y.AddString((LPCTSTR)wBuf);
		m_cbImageFilePath.AddString((LPCTSTR)wBuf);
	}

	m_cbType_in.SetCurSel(1);
	m_cbType_out.SetCurSel(2);
	m_cbAxis_x.SetCurSel(3);
	m_cbAxis_y.SetCurSel(4);
	m_cbImageFilePath.SetCurSel(0);


	return true;
}


void CBHC_featureAnalysisDlg::stringToChar(std::string *str, TCHAR *ch, int iMaxSize )
{
	std::vector<char> writable(str->begin(), str->end());
	writable.push_back('\0');
	char* ptr = &writable[0];


	int len = MultiByteToWideChar( CP_ACP, 0, ptr, -1, NULL, 0 );
// 	WCHAR wBuf[2048];
	MultiByteToWideChar( CP_ACP, 0, ptr, strlen(ptr)+1, ch, len );

}


void CBHC_featureAnalysisDlg::OnBnClickedButtonCalculate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iIndexTarget = m_cbType_in.GetCurSel();
	int iIndexResult = m_cbType_out.GetCurSel();

	if( m_featureData.SetIndex_target(iIndexTarget, iIndexResult) == false )
	{
		return;
	}

	int iAll, iDefect, iMiss, iOver;
	if( m_featureData.GetScore(&iAll, &iDefect, &iMiss, &iOver) == false )
	{
		return;
	}

	SetDlgItemInt(IDC_EDIT_COUNT_ALL,		iAll );
	SetDlgItemInt(IDC_EDIT_COUNT_DEFECT,	iDefect );
	SetDlgItemInt(IDC_EDIT_COUNT_MISS,		iMiss );
	SetDlgItemInt(IDC_EDIT_COUNT_OVER,		iOver );
}


LRESULT CBHC_featureAnalysisDlg::OnUserFunc(WPARAM wParam, LPARAM lParam)
{
	ShowDefectInfo(wParam);

	return 0;
}


void CBHC_featureAnalysisDlg::OnBnClickedButtonApplyRule()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	std::list<bool> listJudge;

	for( int i=0 ; i<m_featureData.GetHeight() ; i++ )
	{
		// 관심 데이터 얻기
		std::string str = m_featureData.GetContent(4, i);

		// 문자로 저장한 값들을 어떻게 가져다 쓸지 고민 필요함
		int iValue = atoi(str.c_str());

		// 샘플 : 판정
		bool bJudge = false;
		if( iValue <10 )
		{
			bJudge = true;
		}

		listJudge.push_back(bJudge);
	}

	// 판정값 data에 저장
	m_featureData.SetJudgment(&listJudge);

	// grid U 업데이트
	UpdateGrid();
}


void CBHC_featureAnalysisDlg::OnBnClickedButtonLearn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BHC_MEMO("CBHC_featureAnalysisDlg::OnBnClickedButtonLearn() > start");

#ifdef _USE_PYTHON_

	CLearningDialog *pLearningDlg = new CLearningDialog;
	pLearningDlg->Create(IDD_DIALOG_LEARNING);


	// init dialog
	pLearningDlg->init();


	// connet python module
#ifdef _USE_PYTHON_
	pLearningDlg->m_pPython = &m_py;
#endif


	// get data : column
	std::vector<std::string> vecCol;
	if (m_featureData.GetColumn(&vecCol) == false)
	{
		BHC_MEMO("CBHC_featureAnalysisDlg::OnBnClickedButtonLearn() > GetColumn > error");
		return;
	}


	// get data : contents
	int iWidth	= 0;
	int iHeight	= 0;
	std::vector<std::string> vecContent;
	if (m_featureData.GetContent( &vecContent, &iWidth, &iHeight ) == false)
	{
		BHC_MEMO("CBHC_featureAnalysisDlg::OnBnClickedButtonLearn() > GetContent > error");
		return;
	}


	// set data
	pLearningDlg->SetData(&vecCol, &vecContent, iWidth, iHeight);

	
	// show dialog
	pLearningDlg->ShowWindow(SW_SHOW);

#endif

	BHC_MEMO("CBHC_featureAnalysisDlg::OnBnClickedButtonLearn() > end");
}


void CBHC_featureAnalysisDlg::OnBnClickedButtonPredict()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
#ifdef _USE_PYTHON_

/*
	double *pA = new double[12];
	pA[0] = 0.0;		pA[1] = 0;
	pA[2] = 0.1;		pA[3] = 0;
	pA[4] = 0.2;		pA[5] = 0;
	pA[6] = 0.3;		pA[7] = 0;
	pA[8] = 0.4;		pA[9] = 0;
	pA[10] = 0.5;		pA[11] = 0;

	int *pB = new int[6];

	if (m_py.Predict(pA, 2, 6, pB) == false)
	{
		BHC_MEMO("OnBnClickedButtonPredict > error");
	}


	for (int i = 0; i < 6; i++)
	{
		BHC_MEMO("[%d]class = %d", i, pB[i]);
	}

	delete[] pA;
	delete[] pB;
*/
	

	//
	//
	//
	double *pA = new double[6];
	pA[0] = 0.0;
	pA[1] = 1.0;
	pA[2] = 2.0;
	pA[3] = 6.0;
	pA[4] = 7.0;
	pA[5] = 8.0;

	int *pB = new int[6];
	pB[0] = 0;
	pB[1] = 0;
	pB[2] = 0;
	pB[3] = 1;
	pB[4] = 1;
	pB[5] = 1;

	m_py.SetClassifier(_BHCPYTHON_CLASSIFIER_LOGISTIC_PROBA_);

	m_py.Fit(pA, pB, 1, 6);


	double *pX = new double[1];
	pX[0] = 3;

	int *pY = new int[2];
	pY[0] = 0;
	pY[1] = 0;

	m_py.Predict(pX, 1, 1, pY);

	int a = 0;


#endif
}
