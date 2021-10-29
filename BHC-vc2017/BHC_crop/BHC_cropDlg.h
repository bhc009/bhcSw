
// BHC_cropDlg.h : 헤더 파일
//

#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../common/ImageViewer.h"

#include <list>

// CBHC_cropDlg 대화 상자
class CBHC_cropDlg : public CDialogEx
{
// 생성입니다.
public:
	CBHC_cropDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BHC_CROP_DIALOG };

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
	afx_msg void OnBnClickedButtonLoadFolder();
	afx_msg void OnBnClickedButtonZoomOut();
	afx_msg void OnBnClickedButtonZoomIn();
	afx_msg void OnBnClickedButtonLoadFolderAll();
	afx_msg void OnBnClickedButtonSetRoiSize();

	DECLARE_MESSAGE_MAP()
public:



protected:
	CImageViewer* m_pView;

	std::list<CBhImage> m_imageList;


	bool GetDlgItem_check_roi();
	int GetDlgItem_roi_width();
	int GetDlgItem_roi_height();

	CString GetDlgItem_keyWord();

public:
};
