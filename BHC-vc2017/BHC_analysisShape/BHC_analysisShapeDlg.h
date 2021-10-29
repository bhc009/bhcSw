
// BHC_analysisShapeDlg.h : ��� ����
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



// CBHC_analysisShapeDlg ��ȭ ����
class CBHC_analysisShapeDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CBHC_analysisShapeDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_BHC_ANALYSISSHAPE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
