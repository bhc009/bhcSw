
// BHC_multiLightingDlg.h : 헤더 파일
//

#pragma once

#include <sstream>
#include <iomanip>

#include "../common/ImageProcess_H.h"
#include "../common/ImageViewer.h"


#define _BHC_VIEW_NUMBER_ 25


// CBHC_multiLightingDlg 대화 상자
class CBHC_multiLightingDlg : public CDialogEx
{
// 생성입니다.
public:
	CBHC_multiLightingDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BHC_MULTILIGHTING_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSetPath();
	afx_msg void OnBnClickedButtonLoadImages();
	afx_msg void OnBnClickedButtonGoBefore();
	afx_msg void OnBnClickedButtonGoNext();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);






protected:

	CImageViewer *m_pImageViewer[_BHC_VIEW_NUMBER_];


	CWnd *GetDlgItemImageViewer(int i);


	void SetZoom();
	BOOL GetZoom();
	int m_iZoom;

	void SetFilePath();
	BOOL GetFilePath(CString *pStrPath);
	CString m_strFilePath;


	BOOL GetKeyWord( CString *pStrKey );
	BOOL SetKeyWord( int iKey );


	BOOL LoadImages();
	BOOL FindTargetFile( CString strPath, CString strKey, CString *pStrFile );


	int m_iStep;

public:
	afx_msg void OnBnClickedButtonZoomOut();
	afx_msg void OnBnClickedButtonZoomIn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonInspection();
};
