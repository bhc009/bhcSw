
// BHC_crop.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CBHC_cropApp:
// �� Ŭ������ ������ ���ؼ��� BHC_crop.cpp�� �����Ͻʽÿ�.
//

class CBHC_cropApp : public CWinApp
{
public:
	CBHC_cropApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CBHC_cropApp theApp;