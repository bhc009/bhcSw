
// BHC_align.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CBHC_alignApp:
// �� Ŭ������ ������ ���ؼ��� BHC_align.cpp�� �����Ͻʽÿ�.
//

class CBHC_alignApp : public CWinApp
{
public:
	CBHC_alignApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CBHC_alignApp theApp;