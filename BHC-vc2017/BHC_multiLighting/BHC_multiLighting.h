
// BHC_multiLighting.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CBHC_multiLightingApp:
// �� Ŭ������ ������ ���ؼ��� BHC_multiLighting.cpp�� �����Ͻʽÿ�.
//

class CBHC_multiLightingApp : public CWinApp
{
public:
	CBHC_multiLightingApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CBHC_multiLightingApp theApp;