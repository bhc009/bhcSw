
// BHC_alignDlg.h : 헤더 파일
//

#pragma once


// #include "../common/ImageProcess_H.h"

#include <iostream>
#include <armadillo>
using namespace std;
using namespace arma;

// CBHC_alignDlg 대화 상자
class CBHC_alignDlg : public CDialogEx
{
// 생성입니다.
public:
	CBHC_alignDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BHC_ALIGN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonForwardTransform();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonCalculate();
	DECLARE_MESSAGE_MAP()

	
public:

	double GetDlgItem_x();
	double GetDlgItem_y();
	double GetDlgItem_xx();
	double GetDlgItem_yy();

	void SetDlgItem_x(double dValue);
	void SetDlgItem_y(double dValue);
	void SetDlgItem_xx(double dValue);
	void SetDlgItem_yy(double dValue);

protected:
	BOOL CalcAlign(double *pOffset_x_image, double *pOffset_y_image, double *pTargetPos_x_world, double *pTargetPos_y_world, double *pTheta, double *pTx, double *pTy, double *pTargetPos_x_local, double *pTargetPos_y_local );

	void GetInput();

	double m_dX[4];
	double m_dY[4];
	double m_dXX[4];
	double m_dYY[4];

	arma::mat m_matTansform;
	arma::mat m_matInverseTansform;

public:
	afx_msg void OnBnClickedButtonBackwardTransform();
	afx_msg void OnBnClickedButtonReview();
};
