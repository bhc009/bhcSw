
// BHC_colorSpace.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CBHC_colorSpaceApp:
// �� Ŭ������ ������ ���ؼ��� BHC_colorSpace.cpp�� �����Ͻʽÿ�.
//

class CBHC_colorSpaceApp : public CWinApp
{
public:
	CBHC_colorSpaceApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CBHC_colorSpaceApp theApp;

extern double g_dProgress;