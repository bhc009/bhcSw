#pragma once


#include "../BHC_featureAnalysis/resource.h"	// 수작업을 줄일 방법은?

#include "../common/grid/GridCtrl.h"
#include "../common/ImageViewer.h"


#define DEFAULT_FILE "d:\\lena_color.bmp"
//#define DEFAULT_FILE "d:\\BHC.bmp"


// CGridDialog 대화 상자입니다.

class CFeatureInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CFeatureInfoDlg)

public:
	CFeatureInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFeatureInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FEATURE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	void init();

	void SetSize(int iWidth, int iHeight);
	bool SetData( int x, int y, std::string strContent );

	void Open( std::string str );

	void Renew();

protected:
	CGridCtrl m_Grid;	// data view
	CImageViewer *m_pImageViewer;

	int m_iWidth;
	int m_iHeight;


	void stringToChar(std::string *str, TCHAR *ch, int iMaxSize );

public:
	afx_msg void OnNcDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
