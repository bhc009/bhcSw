
// BHC_featureAnalysis.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
//	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CBHC_featureAnalysisApp:
// �� Ŭ������ ������ ���ؼ��� BHC_featureAnalysis.cpp�� �����Ͻʽÿ�.
//

class CBHC_featureAnalysisApp : public CWinApp
{
public:
	CBHC_featureAnalysisApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CBHC_featureAnalysisApp theApp;