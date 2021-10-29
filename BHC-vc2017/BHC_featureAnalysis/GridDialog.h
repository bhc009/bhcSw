#pragma once


#include "../BHC_featureAnalysis/resource.h"	// ���۾��� ���� �����?

#include "../common/grid/GridCtrl.h"
#include "../common/ImageViewer.h"


#define DEFAULT_FILE "d:\\lena_color.bmp"
//#define DEFAULT_FILE "d:\\BHC.bmp"


// CGridDialog ��ȭ �����Դϴ�.

class CFeatureInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CFeatureInfoDlg)

public:
	CFeatureInfoDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFeatureInfoDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_FEATURE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
