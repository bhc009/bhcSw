// BhcVision.h : BhcVision DLL�� �⺻ ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CBhcVisionApp
// �� Ŭ������ ������ ������ BhcVision.cpp�� �����Ͻʽÿ�.
//

class CBhcVisionApp : public CWinApp
{
public:
	CBhcVisionApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


#ifdef BHC_VISION_EXPORT
#define DLLTYPE_H __declspec(dllexport)
#else
#define DLLTYPE_H __declspec(dllimport)
#endif




//
// CImageProcess_h �����
//
typedef void* pBhcVision;
extern "C" DLLTYPE_H pBhcVision CreateBhcVision();	// �ν��Ͻ� ����

extern "C" DLLTYPE_H int BhcVision_test(pBhcVision pVision, int a, int b);	// �Լ� ȣ��