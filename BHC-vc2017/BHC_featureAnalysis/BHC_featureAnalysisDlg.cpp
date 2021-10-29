
// BHC_featureAnalysisDlg.cpp : ���� ����
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


// CBHC_featureAnalysisDlg ��ȭ ����




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


// CBHC_featureAnalysisDlg �޽��� ó����

BOOL CBHC_featureAnalysisDlg::OnInitDialog()
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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CBHC_featureAnalysisDlg::OnPaint()
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
HCURSOR CBHC_featureAnalysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CBHC_featureAnalysisDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateGraph();
}


void CBHC_featureAnalysisDlg::OnBnClickedButtonSetInterest()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// �������
// 	std::list<unsigned int> index = m_pGraph->GetIndex_selection();
	std::list<unsigned int> index = m_graphPage.GetGraph()->GetIndex_selection();

	// ����
	m_featureData.SetIndex_interest( &index );


	// ����
	UpdateGraph();
}


void CBHC_featureAnalysisDlg::OnBnClickedButtonResetInterest2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
//	������� �ϴ� �ҷ����� UI�� ���� ���
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
//	UI���� ������������
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
	// index ��/�� ����
	m_Grid.SetFixedColumnCount();
	m_Grid.SetFixedRowCount();

	// data ���� ����
	m_Grid.SetColumnCount();
	m_Grid.SetRowCount();

	// ������
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
//	���� feature data�� graph�� �Է��Ѵ�.
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
//	���� feature data�� graph�� �Է��Ѵ�.
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

		// ���� ������ �ø���.
		if( m_featureData.CheckInterest(i) == false )
		{
			continue;
		}

		if( !strType.compare(strTarget))	
		{	// target �ҷ��� ���
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
	// ��� ������ type
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

		// ���� ������ �ø���.
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
//	�ҷ�(1��) ������ ȭ�鿡 ǥ���Ѵ�.
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

	// set column index : ù��°��
	for( int x=0 ; x<iHeight ; x++ )
	{
		std::string str = m_featureData.GetColumns(x);

		pDlg->SetData(0, x,str);
	}


	// set contents : �ι�° ��
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
//	feature ���� UI�� ���� feature data ������ �ݿ��Ѵ�.
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_featureAnalysisDlg::UpdateFeatureData()
{
	// defect 


	// axis ��
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	std::list<bool> listJudge;

	for( int i=0 ; i<m_featureData.GetHeight() ; i++ )
	{
		// ���� ������ ���
		std::string str = m_featureData.GetContent(4, i);

		// ���ڷ� ������ ������ ��� ������ ���� ��� �ʿ���
		int iValue = atoi(str.c_str());

		// ���� : ����
		bool bJudge = false;
		if( iValue <10 )
		{
			bJudge = true;
		}

		listJudge.push_back(bJudge);
	}

	// ������ data�� ����
	m_featureData.SetJudgment(&listJudge);

	// grid U ������Ʈ
	UpdateGrid();
}


void CBHC_featureAnalysisDlg::OnBnClickedButtonLearn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
