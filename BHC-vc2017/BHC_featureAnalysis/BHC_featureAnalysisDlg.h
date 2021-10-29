
// BHC_featureAnalysisDlg.h : 헤더 파일
//

#pragma once

#ifndef _BHC_
#define _BHC_

#ifdef UNICODE
#define BHC_MEMO(logformat, ...)\
			{ \
			char logbuf[2048]; \
			int ret = _snprintf_s(logbuf, 2048, _TRUNCATE, logformat, ##__VA_ARGS__); \
			int len = MultiByteToWideChar( CP_ACP, 0, logbuf, -1, NULL, 0 ); \
			WCHAR wBuf[2048]; \
			MultiByteToWideChar( CP_ACP, 0, logbuf, strlen(logbuf)+1, wBuf, len ); \
			OutputDebugString(wBuf); \
			};
#else
#define BHC_MEMO(logformat, ...)\
			{ \
			char logbuf[2048]; \
			int ret = _snprintf_s(logbuf, 2048, _TRUNCATE, logformat, ##__VA_ARGS__); \
			OutputDebugString(logbuf); \
			};
#endif
#endif


#include "../common/ImageViewer.h"
#include "../common/BHC_graph2D.h"
#include "../common/BHC_exel.h"
#include "FeatureData.h"

#include "../common/grid/GridCtrl.h"
#include "afxwin.h"

#include "GraphPage.h"

#include "../common/GraphDlg.h"
#include "GridDialog.h"

#include "../BhcPython/BhcPython.h"

#include "LearningDialog.h"

#define WM_GRAPH_SELECTION	(WM_USER + 1)

#define _USE_PYTHON_	// release mode


// CBHC_featureAnalysisDlg 대화 상자
class CBHC_featureAnalysisDlg : public CDialogEx
{
// 생성입니다.
public:
	CBHC_featureAnalysisDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BHC_FEATUREANALYSIS_DIALOG };

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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSetAxis();
	afx_msg void OnBnClickedButtonDrawGraph();
	afx_msg void OnBnClickedButtonSetInterest();
	afx_msg void OnBnClickedButtonCalculate();
	afx_msg LRESULT OnUserFunc(WPARAM wParam, LPARAM lParam);
DECLARE_MESSAGE_MAP()



protected:
	CImageViewer *m_pImageViewer;	// image viewer

	CFeatureData m_featureData;	// feature data

	CGridCtrl m_Grid;	// data view
	void initGrid();

	CGraphPage m_graphPage;	// graph UI

#ifdef _USE_PYTHON_
	CBhcPython m_py;
#endif



	// common UI
	CComboBox m_cbType_in;
	CComboBox m_cbType_out;
	CComboBox m_cbAxis_x;
	CComboBox m_cbAxis_y;
	CComboBox m_cbImageFilePath;


	bool GetTargetType( std::string *pStr );
	bool GetCheck_useFilePath();


	// common
	bool GetFilePath(CString *pStrFileName);

	bool UpdateFeatureData();	// update feature data to ui
	bool UpdateGrid();			// update feature data to grid
	bool UpdateGraph();
	bool ShowDefectInfo(int iIndex);


public:
	void stringToChar(std::string *str, TCHAR *ch, int iMaxSize );

	afx_msg void OnBnClickedButtonResetInterest2();
	afx_msg void OnBnClickedButtonApplyRule();
	afx_msg void OnBnClickedButtonLearn();
	afx_msg void OnBnClickedButtonPredict();
};
