
// BHC_multiImage.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CBHC_multiImageApp:
// �� Ŭ������ ������ ���ؼ��� BHC_multiImage.cpp�� �����Ͻʽÿ�.
//

class CBHC_multiImageApp : public CWinApp
{
public:
	CBHC_multiImageApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CBHC_multiImageApp theApp;