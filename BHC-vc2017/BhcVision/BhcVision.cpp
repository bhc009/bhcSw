// BhcVision.cpp : �ش� DLL�� �ʱ�ȭ ��ƾ�� �����մϴ�.
//

#include "stdafx.h"
#include "BhcVision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: �� DLL�� MFC DLL�� ���� �������� ��ũ�Ǿ� �ִ� ���
//		MFC�� ȣ��Ǵ� �� DLL���� ���������� ��� �Լ���
//		���� �κп� AFX_MANAGE_STATE ��ũ�ΰ�
//		��� �־�� �մϴ�.
//
//		��:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �Ϲ����� �Լ� ������ ���⿡ �ɴϴ�.
//		}
//
//		�� ��ũ�δ� MFC�� ȣ���ϱ� ����
//		�� �Լ��� �ݵ�� ��� �־�� �մϴ�.
//		��, ��ũ�δ� �Լ��� ù ��° ���̾�� �ϸ� 
//		��ü ������ �����ڰ� MFC DLL��
//		ȣ���� �� �����Ƿ� ��ü ������ ����Ǳ� ����
//		���;� �մϴ�.
//
//		�ڼ��� ������
//		MFC Technical Note 33 �� 58�� �����Ͻʽÿ�.
//

// CBhcVisionApp

BEGIN_MESSAGE_MAP(CBhcVisionApp, CWinApp)
END_MESSAGE_MAP()


// CBhcVisionApp ����

CBhcVisionApp::CBhcVisionApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CBhcVisionApp ��ü�Դϴ�.

CBhcVisionApp theApp;


// CBhcVisionApp �ʱ�ȭ

BOOL CBhcVisionApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


#include "ImageProcess_h.h"


extern "C" DLLTYPE_H pBhcVision CreateBhcVision()
{
 	CImageProcess_H *pAlg = new CImageProcess_H();
 
 	return (pBhcVision)pAlg;
}


extern "C" DLLTYPE_H int BhcVision_test(pBhcVision pVision, int a, int b)
{
	CImageProcess_H *pAlg = (CImageProcess_H*)pVision;

	pAlg->Test();

	return 0;

}