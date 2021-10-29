// BhcVision.h : BhcVision DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.


// CBhcVisionApp
// 이 클래스의 구현을 보려면 BhcVision.cpp를 참조하십시오.
//

class CBhcVisionApp : public CWinApp
{
public:
	CBhcVisionApp();

// 재정의입니다.
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
// CImageProcess_h 숨기기
//
typedef void* pBhcVision;
extern "C" DLLTYPE_H pBhcVision CreateBhcVision();	// 인스턴스 생성

extern "C" DLLTYPE_H int BhcVision_test(pBhcVision pVision, int a, int b);	// 함수 호출