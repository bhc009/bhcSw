
// BHC_multiImageDlg.h : 헤더 파일
//

#pragma once

#include "../common/ImageViewer.h"
#include "../common/TitleWnd.h"

#include "../common/ImageProcess_H.h"
#include "afxwin.h"

#include "GraphDlg.h"
#include "afxcmn.h"


#define _BHC_VIEW_NUMBER_	8

#define _BHC_LIGHTING_CONDITION_NUM_		10
#define _BHC_LIGHTING_CONDITION_TABLE_NUM_	108

#define _BHC_WHITE_	7
#define _BHC_RED_	0
#define _BHC_GREEN_	1
#define _BHC_BLUE_	2

#define _BHC_ANLGLE_LOW_	4
#define _BHC_ANLGLE_MID__	2
#define _BHC_ANLGLE_HIGH_	0

#define _BHC_MID_XYZ_GRAPH_	0
#define _BHC_MID_HSV_GRAPH_	1
#define _BHC_MID_LUV_GRAPH_	2

#define _BHC_MID_ALL_DATA_GRAPH_			0
#define _BHC_MID_MAJOR_DATA_GRAPH_		1


// CBHC_multiImageDlg 대화 상자
class CBHC_multiImageDlg : public CDialogEx
{
// 생성입니다.
public:
	CBHC_multiImageDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BHC_MULTIIMAGE_DIALOG };

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSetPath1();
	afx_msg void OnBnClickedButtonSetPath2();
	afx_msg void OnBnClickedButtonSetPath3();
	afx_msg void OnBnClickedButtonSetPath4();
	afx_msg void OnBnClickedButtonSetPath5();
	afx_msg void OnBnClickedButtonSetPath6();
	afx_msg void OnBnClickedButtonSetPath7();
	afx_msg void OnBnClickedButtonSetPath8();
	afx_msg void OnBnClickedButtonSetZoom();
	afx_msg void OnBnClickedButtonSetImageNumber();
	afx_msg void OnBnClickedButtonSetImageViewer();
	afx_msg void OnBnClickedButtonSetZoomIn();
	afx_msg void OnBnClickedButtonSetZoomOut();
	afx_msg void OnBnClickedButtonLoadLightingCondition();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonBefore();
	afx_msg void OnBnClickedButtonUpdateLightingCondition2();
	afx_msg void OnBnClickedButtonInspRoi();
	afx_msg void OnBnClickedButtonInsp();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButtonShowData();
	afx_msg void OnBnClickedButtonInspFolder();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:


	HANDLE m_hEventInsp;
	CWinThread *m_hThreadInsp;	// 측정 스레드

	bool InspectDirectory( CString strDirectory );
	bool SelectDirecctory(CString *pStr);
	bool GetDirectoryList(CString strSrc, std::list<CString> *listFolder);


protected:
	//
	//
	//
	int m_iNumViewer;
	int m_iNumImage;
	int m_iZoom;

	CString m_strDirectory[_BHC_VIEW_NUMBER_];

	CString m_strFileName_pre;
	CString m_strFileName_last;

	void SetFilePath(int i);
	void SetFilePath(int i, CString strPath);

	void GetFilePath();
	CString GetFilePath(int i);
	void GetFilePath(int i, CString *pStrPath);
	BOOL FindTargetFile( CString strPath, CString strKey, CString *pStrFile );

	int GetImageIndex();
	void SetImageIndex(int iIndex);

	int GetMaxImageIndex();
	void SetMaxImageIndex(int i);


	//
	// Image view
	//
	CImageViewer *m_pImageView[_BHC_VIEW_NUMBER_];
	CImageViewer *m_pImageView_sub[_BHC_VIEW_NUMBER_];

	void LoadImages();
	void LoadColorImages();
	CWnd *GetDlgItemViewer(int i);
	CWnd *GetDlgItemSubViewer(int i);


	//
	// Lighting view	
	//
	int m_iLightColor;
	CTitleWnd m_ctrlLightHigh[8];
	CTitleWnd m_ctrlLightMidle[8];
	CTitleWnd m_ctrlLightLow[8];

	BOOL CreateLightingView();
	BOOL ResetLightingView();
	BOOL LoadLightingTable();
	BOOL UpdateLightingView();
	BOOL UpdateLightingView(int iIndex);



	// analysis
	CComboBox m_comboData;	// 색선택
	int GetDataType_graph();

	int GetDataNum_graph();

	bool GetDlgCheck_display();


	//
	// image processing
	//
	CImageProcess_H m_imageProcessing[_BHC_VIEW_NUMBER_];
	bool FindRoi();
	bool FindRoi(int i);
	bool ExtractFeature();
	bool ExtractFeature(int i);


	//
	// lighting condition table
	//
	int m_aLightingCondition[_BHC_LIGHTING_CONDITION_NUM_];
	int m_aLightingConditionTable[_BHC_LIGHTING_CONDITION_TABLE_NUM_][_BHC_LIGHTING_CONDITION_NUM_];

	BOOL GetIndex_red( int iIndex, int *piIndex );
	BOOL GetIndex_green( int iIndex, int *piIndex );
	BOOL GetIndex_blue( int iIndex, int *piIndex );


	//
	CProgressCtrl m_ctrlProgress;
	double m_dProgress_ratio;

	CListBox m_ctrlMemo;
	void Memo(CString str);

public:
	CComboBox m_ctrImageNum_graph;
};
