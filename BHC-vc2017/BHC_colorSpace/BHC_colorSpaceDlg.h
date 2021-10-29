
// BHC_colorSpaceDlg.h : 헤더 파일
//

#pragma once

#include "../common/ImageViewer.h"
#include "afxcmn.h"
#include "afxwin.h"

#include "PageXYY.h"
#include "PageLUV.h"
#include "ColorInfo.h"


#define _BHC_RGB_		0
#define _BHC_LUMINANCE_	1
#define _BHC_XYZ_		2
#define _BHC_MAKE_HOBJ_LUMINANCE_		3


// #define _BHC_XYZ_X_		0
// #define _BHC_XYZ_Y_		1
// #define _BHC_XYZ_Z_		2

#define _BHC_TARGET_SYSTEM_DEFAULT_		0
#define _BHC_TARGET_SYSTEM_RADIANT_		1
#define _BHC_TARGET_SYSTEM_LUMITOP_		2


struct stXYZ
{
	int iWidth;
	int iHeight;
	double *pDataX;
	double *pDataY;
	double *pDataZ;

	stXYZ()
	{
		iWidth = 0;
		iHeight = 0;
		pDataX = NULL;
		pDataY = NULL;
		pDataZ = NULL;
	}

	void free()
	{
		if( pDataX )
		{
			delete[] pDataX;
		}
		pDataX = NULL;

		if( pDataY )
		{
			delete[] pDataY;
		}
		pDataY = NULL;

		if( pDataZ )
		{
			delete[] pDataZ;
		}
		pDataZ = NULL;
	}
};


struct stXYY
{
	int iWidth;
	int iHeight;
	double *pDataX;
	double *pDataY;

	stXYY()
	{
		iWidth = 0;
		iHeight = 0;
		pDataX = NULL;
		pDataY = NULL;
	}

	void free()
	{
		if( pDataX )
		{
			delete[] pDataX;
		}
		pDataX = NULL;

		if( pDataY )
		{
			delete[] pDataY;
		}
		pDataY = NULL;
	}
};


struct stLuminace
{
	int iWidth;
	int iHeight;
	double *pData;

	stLuminace()
	{
		iWidth = 0;
		iHeight = 0;
		pData = NULL;
	}

	void free()
	{
		if( pData )
		{
			delete[] pData;
		}
		pData = NULL;
	}
};


// struct stRGB_16
// {
// 	int iWidth;
// 	int iHeight;
// 	unsigned short *pData;
// 
// 	stRGB_16()
// 	{
// 		iWidth = 0;
// 		iHeight = 0;
// 		pData = NULL;
// 	}
// 
// 	void free()
// 	{
// 		if( pData )
// 		{
// 			delete[] pData;
// 		}
// 		pData = NULL;
// 	}
// };


// struct stRGB_8
// {
// 	int iWidth;
// 	int iHeight;
// 	BYTE *pData;
// 
// 	stRGB_8()
// 	{
// 		iWidth = 0;
// 		iHeight = 0;
// 		pData = NULL;
// 	}
// 
// 	void free()
// 	{
// 		if( pData )
// 		{
// 			delete[] pData;
// 		}
// 		pData = NULL;
// 	}
// };


// CBHC_colorSpaceDlg 대화 상자
class CBHC_colorSpaceDlg : public CDialogEx
{
// 생성입니다.
public:
	CBHC_colorSpaceDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BHC_COLORSPACE_DIALOG };

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
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonOpenImageRgb();
	afx_msg void OnBnClickedButtonSetDataSize();
	afx_msg void OnBnClickedButtonOpenDataLuminance();
	afx_msg void OnBnClickedButtonSaveHobjLuminance();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonOpenDataXyz();
	afx_msg void OnBnClickedButtonSaveHobjLuminanceMulti();
	afx_msg void OnRadioButtonTarget(UINT iValue);
	afx_msg void OnRadioButtonImageDepth(UINT iValue);
	afx_msg void OnBnClickedButtonGenGraph();

	DECLARE_MESSAGE_MAP()


public:
	HANDLE m_hEventLoad;
	int m_indexLoad;


	// load data - rgb
	BOOL LoadRgbData();

	// load data - xyz
	BOOL LoadXYZData();


	// load data - lumi
	BOOL LoadLuminanceData();
	BOOL LoadLuminanceData(CString strFileName);
	BOOL SaveLuminanceData(CString strFileName);
	BOOL MakeHobjLuminance();



protected:
	CImageViewer *m_pImageViewer;

	// 한장 검사 스레드
	CWinThread *m_hThreadLoad;	// 측정 스레드

	//
	LARGE_INTEGER m_globalStartCounter, m_globalEndCounter, m_counter_Freq;


	// data
	CColorInfo m_colorInfo;
	UINT m_uiDataWidth;
	UINT m_uiDataHeight;
	CSize m_szData;


	// load data
	BOOL LoadXYZData_default();
	BOOL LoadXYZData_radiant();
	BOOL LoadXYZData_lumitop();


	// view window
	BOOL MakeViewImage();

	BOOL MakeGraph(int iType);
	bool GenGraph_xyy();
	bool GenGraph_luv();

	void memo(CString strMemo );


	// tab
	void InitTabCtrl();
	void ShowTabCtrl();


	// dlg item
	CWnd *m_pWndShow;

	CTabCtrl m_ctrlTab;
	CPageLUV m_pageLUV;
	CPageXYY m_pageXYY;

	CListBox m_ctrlListMemo;

	CProgressCtrl m_ctrlProgress;


	unsigned int m_uiDiv_width;
	unsigned int m_uiDiv_height;

	int m_rdoImageDepth;
	int m_rdoTarget;


public:


};
