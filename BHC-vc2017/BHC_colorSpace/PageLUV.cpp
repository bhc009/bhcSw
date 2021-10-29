// PageLUV.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BHC_colorSpace.h"
#include "PageLUV.h"
#include "afxdialogex.h"


// CPageLUV 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPageLUV, CDialogEx)

CPageLUV::CPageLUV(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageLUV::IDD, pParent)
{

}

CPageLUV::~CPageLUV()
{
}

void CPageLUV::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageLUV, CDialogEx)
END_MESSAGE_MAP()


// CPageLUV 메시지 처리기입니다.
