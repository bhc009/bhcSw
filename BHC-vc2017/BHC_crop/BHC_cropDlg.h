
// BHC_cropDlg.h : ��� ����
//

#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../common/ImageViewer.h"

#include <list>

// CBHC_cropDlg ��ȭ ����
class CBHC_cropDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CBHC_cropDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_BHC_CROP_DIALOG };

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
