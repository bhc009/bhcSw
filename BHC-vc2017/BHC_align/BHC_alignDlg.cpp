
// BHC_alignDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "BHC_align.h"
#include "BHC_alignDlg.h"
#include "afxdialogex.h"

#include "math.h"

#include "../common/BhImage.h"

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


// CBHC_alignDlg ��ȭ ����




CBHC_alignDlg::CBHC_alignDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBHC_alignDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBHC_alignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBHC_alignDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FORWARD_TRANSFORM, &CBHC_alignDlg::OnBnClickedButtonForwardTransform)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CBHC_alignDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CBHC_alignDlg::OnBnClickedButtonCalculate)
	ON_BN_CLICKED(IDC_BUTTON_BACKWARD_TRANSFORM, &CBHC_alignDlg::OnBnClickedButtonBackwardTransform)
	ON_BN_CLICKED(IDC_BUTTON_REVIEW, &CBHC_alignDlg::OnBnClickedButtonReview)
END_MESSAGE_MAP()


// CBHC_alignDlg �޽��� ó����

BOOL CBHC_alignDlg::OnInitDialog()
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


	m_dX[0] = -100;
	m_dX[1] = 100;
	m_dX[2] = 100;
	m_dX[3] = -100;

	m_dY[0] = 100;
	m_dY[1] = 100;
	m_dY[2] = -100;
	m_dY[3] = -100;

	m_dXX[0] = -105.845593067914;
	m_dXX[1] = 91.1159575345278;
	m_dXX[2] = 125.845593067914;
	m_dXX[3] = -71.1159575345278;

	m_dYY[0] = 86.1159575345278;
	m_dYY[1] = 120.845593067914;
	m_dYY[2] = -76.1159575345278;
	m_dYY[3] = -110.845593067914;

	CString str;
	str.Format(_T("%f"), m_dX[0]);
	SetDlgItemText(IDC_EDIT_X_1, str);
	str.Format(_T("%f"), m_dX[1]);
	SetDlgItemText(IDC_EDIT_X_2, str);
	str.Format(_T("%f"), m_dX[2]);
	SetDlgItemText(IDC_EDIT_X_3, str);
	str.Format(_T("%f"), m_dX[3]);
	SetDlgItemText(IDC_EDIT_X_4, str);

	str.Format(_T("%f"), m_dY[0]);
	SetDlgItemText(IDC_EDIT_Y_1, str);
	str.Format(_T("%f"), m_dY[1]);
	SetDlgItemText(IDC_EDIT_Y_2, str);
	str.Format(_T("%f"), m_dY[2]);
	SetDlgItemText(IDC_EDIT_Y_3, str);
	str.Format(_T("%f"), m_dY[3]);
	SetDlgItemText(IDC_EDIT_Y_4, str);

	str.Format(_T("%f"), m_dXX[0]);
	SetDlgItemText(IDC_EDIT_XX_1, str);
	str.Format(_T("%f"), m_dXX[1]);
	SetDlgItemText(IDC_EDIT_XX_2, str);
	str.Format(_T("%f"), m_dXX[2]);
	SetDlgItemText(IDC_EDIT_XX_3, str);
	str.Format(_T("%f"), m_dXX[3]);
	SetDlgItemText(IDC_EDIT_XX_4, str);

	str.Format(_T("%f"), m_dYY[0]);
	SetDlgItemText(IDC_EDIT_YY_1, str);
	str.Format(_T("%f"), m_dYY[1]);
	SetDlgItemText(IDC_EDIT_YY_2, str);
	str.Format(_T("%f"), m_dYY[2]);
	SetDlgItemText(IDC_EDIT_YY_3, str);
	str.Format(_T("%f"), m_dYY[3]);
	SetDlgItemText(IDC_EDIT_YY_4, str);


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CBHC_alignDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBHC_alignDlg::OnPaint()
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
HCURSOR CBHC_alignDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBHC_alignDlg::OnBnClickedButtonForwardTransform()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	double x = GetDlgItem_x();
	double y = GetDlgItem_y();


	arma::mat matX(3,1);
	matX(0,0)	= x;
	matX(1,0)	= y;
	matX(2,0)	= 1;


	arma::mat matB = m_matTansform * matX;


	SetDlgItem_xx( matB(0,0));
	SetDlgItem_yy( matB(1,0));


	return;





	// align mark�� ���� ��ǥ
/*
	double dX_align_world[4] = {-100, 100, 100, -100};
	double dY_align_world[4] = {100, 100, -100, -100};

	// align 10��
// 	double dX_align_local[4] = {-105.845593067914, 91.1159575345278, 125.845593067914, -71.1159575345278};
// 	double dY_align_local[4] = {86.1159575345278, 120.845593067914, -76.1159575345278, -110.845593067914};
	// align 5��
	double dX_align_local[4] = {-98.3350440839404, 100.903895534409, 118.33504408394, -80.9038955344087};
	double dY_align_local[4] = {95.9038955344087, 113.33504408394, -85.9038955344087, -103.33504408394};
*/



// 	double dX_align_world[4] = {-1152.698, 1156.299, 1151.837, -1157.27};
// 	double dY_align_world[4] = {-1088.56, -1088.328, 1088.087, 1087.732};
// 	double dX_align_world[4] = {-1151.664, 1157.514, 1150.633, -1158.576};
// 	double dY_align_world[4] = {-1091.012, -1087.93, 1088.325, 1085.307};
// 	double dX_align_world[4] = {-1152.416, 1157.442, 1151.774, -1157.239};
// 	double dY_align_world[4] = {-1088.884, -1088.389, 1088.087, 1087.376};
	double dX_align_world[4] = {-1150.887, 1158.148, 1150.982, -1158.196};
	double dY_align_world[4] = {-1091.204, -1087.737, 1088.63, 1085.099};
	double dX_align_local[4] = {-1152.4, 1156.55, 1152.4, -1156.55};
	double dY_align_local[4] = {-1088, -1088, 1088, 1088};

	// align camera offset ���
	double dOffest_x[4] = {0, 0, 0, 0};
	double dOffest_y[4] = {0, 0, 0, 0};

	 for( int i=0 ; i<4 ; i++ )
	 {
		 dOffest_x[i] = dX_align_local[i] - dX_align_world[i];
		 dOffest_y[i] = dY_align_local[i] - dY_align_world[i];
	 }

	// align ��� ���˿�
	double dX_align_Test[4] = {0, 0, 0, 0};
	double dY_align_Test[4] = {0, 0, 0, 0};


	//
	//
	//
	double dTheta = 0.0;
	double dTx = 0.0;
	double dTy = 0.0;

	CalcAlign(	dOffest_x, 
				dOffest_y, 
				dX_align_world, 
				dY_align_world, 
				&dTheta, 
				&dTx, 
				&dTy, 
				dX_align_Test, 
				dY_align_Test );


	//
	CString str;
	str.Format(_T(" tx = %.5f\n ty=%.5f\n theta=%.5f\n\n real vs calc\n (%.2f, %.2f)  (%.2f, %.2f)\n (%.2f, %.2f)  (%.2f, %.2f)\n (%.2f, %.2f)  (%.2f, %.2f)\n (%.2f, %.2f)  (%.2f, %.2f)"), 
		dTx, dTy, dTheta,
		dX_align_local[0], dY_align_local[0], dX_align_Test[0], dY_align_Test[0],
		dX_align_local[1], dY_align_local[1], dX_align_Test[1], dY_align_Test[1],
		dX_align_local[2], dY_align_local[2], dX_align_Test[2], dY_align_Test[2],
		dX_align_local[3], dY_align_local[3], dX_align_Test[3], dY_align_Test[3] );

	AfxMessageBox(str);


// 
// 	for(int i=0 ; i<4 ; i++ )
// 	{
// 		dx[i] = dX_align_local[i] - dX_align_world[i];
// 		dy[i] = dY_align_local[i] - dY_align_world[i];
// 	}
// 
// 
// 	double dTheta = 0.0;
// 	double dTx = 0.0;
// 	double dTy = 0.0;
// 	double dPhi = 3.14159265358979;
// 
// 	// calc Tx
// 	int nCnt = 0;
// 	double dSum = 0;
// 	for( int i=0 ; i<4 ; i++ )
// 	{
// 		dSum += dx[i];
// 		nCnt++;
// 	}
// 	dTx = dSum/nCnt;
// 
// 
// 	// calc Ty
// 	nCnt = 0;
// 	dSum = 0;
// 	for( int i=0 ; i<4 ; i++ )
// 	{
// 		dSum += dy[i];
// 		nCnt++;
// 	}
// 	dTy = dSum/nCnt;
// 
// 
// 	//
// 	int iCnt = 0;
// 	double dThetaSum = 0.0;
// 	for( int i=0 ; i<4 ; i++ )
// 	{
// 		int iIndex_next = ( i + 1 ) % 4;
// 
// 		// world ��ǥ�迡���� ����
// 		double x1 = dX_align_world[i];
// 		double y1 = dY_align_world[i];
// 		double x2 = dX_align_world[iIndex_next];
// 		double y2 = dY_align_world[iIndex_next];
// 		double dTheta_world = atan2( y2 - y1, x2 - x1 );
// 		if( dTheta_world < 0 )
// 		{
// 			dTheta_world = 2.0*dPhi + dTheta_world;
// 		}
// 
// 		// local ��ǥ�迡���� ����
// 		x1 = dX_align_local[i];
// 		y1 = dY_align_local[i];
// 		x2 = dX_align_local[iIndex_next];
// 		y2 = dY_align_local[iIndex_next];
// 		double dTheta_local = atan2( y2 - y1, x2 - x1 );
// 		if( dTheta_local < 0 )
// 		{
// 			dTheta_local = 2.0*dPhi + dTheta_local;
// 		}
// 
// 		//
// 		double dTheta_temp = dTheta_local - dTheta_world;
// 
// 		dThetaSum += dTheta_temp;
// 
// 		iCnt++;
// 	}
// 
// 	dTheta = dThetaSum / iCnt;
// 
// 	// test
// 	double aa = 50*cos(dTheta) - 50*sin(dTheta) + dTx;
// 	double bb = 50*sin(dTheta) + 50*cos(dTheta) + dTy;
// 
// 
// 	dTheta = 180.0 * dTheta / dPhi;
}


double CBHC_alignDlg::GetDlgItem_x()
{
	CString str;

	GetDlgItemText(IDC_EDIT_X, str );


	char cStr[1024];
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );

	double dValue = atof(cStr);


	return dValue;
}


double CBHC_alignDlg::GetDlgItem_y()
{
	CString str;

	GetDlgItemText(IDC_EDIT_Y, str );

	char cStr[1024];
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );

	double dValue = atof(cStr);


	return dValue;
}


double CBHC_alignDlg::GetDlgItem_xx()
{
	CString str;

	GetDlgItemText(IDC_EDIT_XX, str );

	char cStr[1024];
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );

	double dValue = atof(cStr);


	return dValue;
}


double CBHC_alignDlg::GetDlgItem_yy()
{
	CString str;

	GetDlgItemText(IDC_EDIT_YY, str );

	char cStr[1024];
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );

	double dValue = atof(cStr);


	return dValue;
}

void CBHC_alignDlg::SetDlgItem_x(double dValue)
{
	CString str;
	str.Format(_T("%f"), dValue);

	SetDlgItemText(IDC_EDIT_X, str);
}


void CBHC_alignDlg::SetDlgItem_y(double dValue)
{
	CString str;
	str.Format(_T("%f"), dValue);

	SetDlgItemText(IDC_EDIT_Y, str);
}


void CBHC_alignDlg::SetDlgItem_xx(double dValue)
{
	CString str;
	str.Format(_T("%f"), dValue);

	SetDlgItemText(IDC_EDIT_XX, str);
}


void CBHC_alignDlg::SetDlgItem_yy(double dValue)
{
	CString str;
	str.Format(_T("%f"), dValue);

	SetDlgItemText(IDC_EDIT_YY, str);
}





//////////////////////////////////////////////////////////////////////////
//
//	CalcAlign
//
//	�� ���� 
//		- align matrix ��� : theta, tx, ty
//
//	�� ����
//		double *pOffset_x_image : [in]Align camera ���󿡼� align mark�� ��ġ offset
//		double *pOffset_y_image : [in]Align camera ���󿡼� align mark�� ��ġ offset 
//		double *Target_x_world	: [in]����� align mark�� ��ġ 
//		double *pTarget_y_world	: [in]����� align mark�� ��ġ 
//		double *pTheta			: [out]theta
//		double *pTx				: [out]translation
//		double *pTy				: [out]translation
//		double *pTarget_x_local : [out]Align mark�� stage ��ǥ
//		double *pTarget_y_local : [out]Align mark�� stage ��ǥ
//
//
//////////////////////////////////////////////////////////////////////////
BOOL CBHC_alignDlg::CalcAlign(double *pOffset_x_image, double *pOffset_y_image, double *pTargetPos_x_world, double *pTargetPos_y_world, double *pTheta, double *pTx, double *pTy, double *pTargetPos_x_local, double *pTargetPos_y_local)
{
	double dResolution = 1.0;
	double dPhi = 3.14159265358979;


	//
	//
	//
	int iPointNum = 4;

	// align mark�� world ��ǥ
	double *pdX_align_world = new double[iPointNum];
	double *pdY_align_world = new double[iPointNum];

	// align mark�� local ��ǥ
	double *pdX_align_local = new double[iPointNum];
	double *pdY_align_local = new double[iPointNum];

	//
	double *pOffset_x_local = new double[iPointNum];
	double *pOffset_y_local = new double[iPointNum];

	//
	for( int i=0 ; i<iPointNum ; i++ )
	{
		// align mark ��ǥ (world coord)
		pdX_align_world[i] = pTargetPos_x_world[i];
		pdY_align_world[i] = pTargetPos_y_world[i];

		// align mark offset (local coord)
		pOffset_x_local[i] = pOffset_x_image[i]*dResolution;
		pOffset_y_local[i] = pOffset_y_image[i]*dResolution;

		// align mark ��ǥ (local coord)
		pdX_align_local[i] = pTargetPos_x_world[i] + pOffset_x_local[i];
		pdY_align_local[i] = pTargetPos_y_world[i] + pOffset_y_local[i];
	}


	//
	// Translation ���
	//
	double dTheta = 0.0;
	double dTx = 0.0;
	double dTy = 0.0;

	// calc Tx
	int nCnt = 0;
	double dSum = 0;
	for( int i=0 ; i<iPointNum ; i++ )
	{
		dSum += pOffset_x_local[i];
		nCnt++;
	}
	dTx = dSum/nCnt;


	// calc Ty
	nCnt = 0;
	dSum = 0;
	for( int i=0 ; i<iPointNum ; i++ )
	{
		dSum += pOffset_y_local[i];
		nCnt++;
	}
	dTy = dSum/nCnt;



	//
	// rotation ���
	//
	int iCnt = 0;
	double dThetaSum = 0.0;
	for( int i=0 ; i<iPointNum ; i++ )
	{
		int iIndex_next = ( i + 1 ) % iPointNum;

		// world ��ǥ�迡���� ����
		double x1 = pdX_align_world[i];
		double y1 = pdY_align_world[i];
		double x2 = pdX_align_world[iIndex_next];
		double y2 = pdY_align_world[iIndex_next];
		double dTheta_world = atan2( y2 - y1, x2 - x1 );
		if( dTheta_world < 0 )
		{
			dTheta_world = 2.0*dPhi + dTheta_world;
		}

		// local ��ǥ�迡���� ����
		x1 = pdX_align_local[i];
		y1 = pdY_align_local[i];
		x2 = pdX_align_local[iIndex_next];
		y2 = pdY_align_local[iIndex_next];
		double dTheta_local = atan2( y2 - y1, x2 - x1 );
		if( dTheta_local < 0 )
		{
			dTheta_local = 2.0*dPhi + dTheta_local;
		}

		//
		double dTheta_temp = dTheta_local - dTheta_world;

		dThetaSum += dTheta_temp;

		iCnt++;
	}

	dTheta = dThetaSum / iCnt;


	//
	//
	//
	for( int i=0 ; i<iPointNum ; i++ )
	{
		pTargetPos_x_local[i] = pdX_align_world[i]*cos(dTheta) - pdY_align_world[i]*sin(dTheta) + dTx;
		pTargetPos_y_local[i] = pdX_align_world[i]*sin(dTheta) + pdY_align_world[i]*cos(dTheta) + dTy;
	}


	// convert rad to degree
	dTheta = 180.0 * dTheta / dPhi;


	*pTx = dTx;
	*pTy = dTy;
	*pTheta = dTheta;


	//
	// free memory
	//
	// 
	delete[] pdX_align_world;
	delete[] pdY_align_world;

	// align mark�� local ��ǥ
	delete[] pdX_align_local;
	delete[] pdY_align_local;

	//
	delete[] pOffset_x_local;
	delete[] pOffset_y_local;


	return TRUE;
}

void CBHC_alignDlg::GetInput()
{
	CString str;
	char cStr[1024];


	// X
	GetDlgItemText(IDC_EDIT_X_1, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dX[0] = atof(cStr);

	GetDlgItemText(IDC_EDIT_X_2, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dX[1] = atof(cStr);

	GetDlgItemText(IDC_EDIT_X_3, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dX[2] = atof(cStr);

	GetDlgItemText(IDC_EDIT_X_4, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dX[3] = atof(cStr);


	// Y
	GetDlgItemText(IDC_EDIT_Y_1, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dY[0] = atof(cStr);

	GetDlgItemText(IDC_EDIT_Y_2, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dY[1] = atof(cStr);

	GetDlgItemText(IDC_EDIT_Y_3, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dY[2] = atof(cStr);

	GetDlgItemText(IDC_EDIT_Y_4, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dY[3] = atof(cStr);

	// XX
	GetDlgItemText(IDC_EDIT_XX_1, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dXX[0] = atof(cStr);

	GetDlgItemText(IDC_EDIT_XX_2, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dXX[1] = atof(cStr);

	GetDlgItemText(IDC_EDIT_XX_3, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dXX[2] = atof(cStr);

	GetDlgItemText(IDC_EDIT_XX_4, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dXX[3] = atof(cStr);


	// YY
	GetDlgItemText(IDC_EDIT_YY_1, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dYY[0] = atof(cStr);

	GetDlgItemText(IDC_EDIT_YY_2, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dYY[1] = atof(cStr);

	GetDlgItemText(IDC_EDIT_YY_3, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dYY[2] = atof(cStr);

	GetDlgItemText(IDC_EDIT_YY_4, str );
	WideCharToMultiByte( CP_ACP, 0, str, -1, cStr, 1024, NULL, NULL );
	m_dYY[3] = atof(cStr);

}


void CBHC_alignDlg::OnBnClickedButtonTest()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
// 	std::string strMemo;
// 	sprintf( (char*)strMemo.c_str(), "%s\\%d", "d:\\test", 1 );
/*
	CImageProcess_H alg;
	alg.SetShowImage(true);


	//
	alg.SetAlignPoint_world( 0, 0, 7000, 15000 );	// ���� 7 mm, ���� 15 mm


	//
	CBhImage img;
	
	img.Open(_T("d:\\test\\align_1.bmp"));
	alg.SetAlignImage_line_color( img.GetImage(), img.GetWidth(), img.GetHeight(), _BHC_IP_RIGHT_, 59.342, 57.818, _BHC_IP_NEGATIVE_, _BHC_IP_POSITIVE_, 0 );

	img.Open(_T("d:\\test\\align_2.bmp"));
	alg.SetAlignImage_line_color( img.GetImage(), img.GetWidth(), img.GetHeight(), _BHC_IP_BOTTOM_, 49.434, 66.984, _BHC_IP_NEGATIVE_, _BHC_IP_POSITIVE_, 0 );

	img.Open(_T("d:\\test\\align_3.bmp"));
	alg.SetAlignImage_line_color( img.GetImage(), img.GetWidth(), img.GetHeight(), _BHC_IP_RIGHT_, 107.73, 207.657, _BHC_IP_NEGATIVE_, _BHC_IP_POSITIVE_, 0 );

	img.Open(_T("d:\\test\\align_4.bmp"));
	alg.SetAlignImage_line_color( img.GetImage(), img.GetWidth(), img.GetHeight(), _BHC_IP_BOTTOM_, 119.185, 197.111, _BHC_IP_NEGATIVE_, _BHC_IP_POSITIVE_, 0 );


	//
	alg.CalcCoordinateMatrix();


	//
// 	double dx, dy;
// 	alg.GetCoodinate( alg.m_ptAlignKey_local[0].x, alg.m_ptAlignKey_local[0].y, &dx, &dy, _BHC_IP_LOCAL_TO_WORLD_ );
// 	alg.GetCoodinate( alg.m_ptAlignKey_local[1].x, alg.m_ptAlignKey_local[1].y, &dx, &dy, _BHC_IP_LOCAL_TO_WORLD_ );
*/
}


void CBHC_alignDlg::OnBnClickedButtonCalculate()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetInput();

	//////////////////////////////////////////////////////////////////////////
	//
	// forward transform matrix
	//
	arma::mat A(8,6);
	arma::mat B(8,1);


	// A matrix
	for( int i=0 ; i<4 ; i++ )
	{
		A(2*i, 0) = m_dX[i];
		A(2*i, 1) = m_dY[i];
		A(2*i, 2) = 1;
		A(2*i, 3) = 0;
		A(2*i, 4) = 0;
		A(2*i, 5) = 0;

		A(2*i + 1, 0) = 0;
		A(2*i + 1, 1) = 0;
		A(2*i + 1, 2) = 0;
		A(2*i + 1, 3) = m_dX[i];
		A(2*i + 1, 4) = m_dY[i];
		A(2*i + 1, 5) = 1;
	}


	// B matrix
	B(0,0)	= m_dXX[0];
	B(1,0)	= m_dYY[0];
	B(2,0)	= m_dXX[1];
	B(3,0)	= m_dYY[1];
	B(4,0)	= m_dXX[2];
	B(5,0)	= m_dYY[2];
	B(6,0)	= m_dXX[3];
	B(7,0)	= m_dYY[3];



	// solve matrix
 	arma::mat X = solve(A, B);


	arma::mat resultMat(2, 3);
	resultMat(0,0) = X(0,0);
	resultMat(0,1) = X(1,0);
	resultMat(0,2) = X(2,0);

	resultMat(1,0) = X(3,0);
	resultMat(1,1) = X(4,0);
	resultMat(1,2) = X(5,0);
	

	m_matTansform = resultMat;




	//////////////////////////////////////////////////////////////////////////
	//
	// backward transform matrix
	// A matrix
	for( int i=0 ; i<4 ; i++ )
	{
		A(2*i, 0) = m_dXX[i];
		A(2*i, 1) = m_dYY[i];
		A(2*i, 2) = 1;
		A(2*i, 3) = 0;
		A(2*i, 4) = 0;
		A(2*i, 5) = 0;

		A(2*i + 1, 0) = 0;
		A(2*i + 1, 1) = 0;
		A(2*i + 1, 2) = 0;
		A(2*i + 1, 3) = m_dXX[i];
		A(2*i + 1, 4) = m_dYY[i];
		A(2*i + 1, 5) = 1;
	}


	// B matrix
	B(0,0)	= m_dX[0];
	B(1,0)	= m_dY[0];
	B(2,0)	= m_dX[1];
	B(3,0)	= m_dY[1];
	B(4,0)	= m_dX[2];
	B(5,0)	= m_dY[2];
	B(6,0)	= m_dX[3];
	B(7,0)	= m_dY[3];



	// solve matrix
	X = solve(A, B);


	resultMat(0,0) = X(0,0);
	resultMat(0,1) = X(1,0);
	resultMat(0,2) = X(2,0);

	resultMat(1,0) = X(3,0);
	resultMat(1,1) = X(4,0);
	resultMat(1,2) = X(5,0);


	m_matInverseTansform = resultMat;
}


void CBHC_alignDlg::OnBnClickedButtonBackwardTransform()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	double x = GetDlgItem_xx();
	double y = GetDlgItem_yy();


	arma::mat matX(3,1);
	matX(0,0)	= x;
	matX(1,0)	= y;
	matX(2,0)	= 1;


	arma::mat matB = m_matInverseTansform * matX;


	SetDlgItem_x( matB(0,0));
	SetDlgItem_y( matB(1,0));


	return;
}


void CBHC_alignDlg::OnBnClickedButtonReview()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
/*	CImageProcess_H alg;

	int i = 3;


	//
	CBhImage img1;
// 	img1.Open(_T("D:\\data\\�ܰ��˻�\\20200408-����2\\1-.bmp"));
	img1.Open(_T("D:\\data\\�ܰ��˻�\\20200408-review_num2\\3-1.bmp"));

	alg.SetReviewImage( img1.GetImage(), img1.GetWidth(), img1.GetHeight(), 3, _BHC_IP_REVIEW_NORMAL_, i );


	//
	CBhImage img2;
// 	img2.Open(_T("D:\\data\\�ܰ��˻�\\20200408-����2\\2.bmp"));
	img2.Open(_T("D:\\data\\�ܰ��˻�\\20200408-review_num2\\3-2.bmp"));

	alg.SetReviewImage( img2.GetImage(), img2.GetWidth(), img2.GetHeight(), 1, _BHC_IP_REVIEW_SPECIAL_, i );


	//
	alg.InspectReview(i);*/
}
