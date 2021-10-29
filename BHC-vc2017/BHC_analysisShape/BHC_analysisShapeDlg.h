
// BHC_analysisShapeDlg.h : 헤더 파일
//

#pragma once

#include "../common/ImageViewer.h"
// #include "../common/ImageProcess_H.h"
#include "../BhcVision/ImageProcess_h.h"
#include "../BhcPython/BhcPython.h"

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



// CBHC_analysisShapeDlg 대화 상자
class CBHC_analysisShapeDlg : public CDialogEx
{
// 생성입니다.
public:
	CBHC_analysisShapeDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BHC_ANALYSISSHAPE_DIALOG };

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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButtonLoadOrgImage();
	afx_msg void OnBnClickedButtonLoadPrcImage();
	afx_msg void OnBnClickedButtonMakePrcImage();
	afx_msg void OnBnClickedButtonAnalysis();




protected:
	CImageViewer *m_pImageViewer;

	CImageProcess_H m_alg;

	bool GetFilePath(CString *pStrFileName);

	CString m_strDirectory;

	char * ConvertWCtoC(wchar_t* str);

	CBhcPython m_pyt;

public:
	afx_msg void OnBnClickedButtonTemplateLoad();
	afx_msg void OnBnClickedButton();
	afx_msg void OnBnClickedButtonInspRipple();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonSetDirectory();
	afx_msg void OnBnClickedButtonProcess();
};
