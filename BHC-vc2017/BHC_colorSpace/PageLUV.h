#pragma once


// CPageLUV ��ȭ �����Դϴ�.

class CPageLUV : public CDialogEx
{
	DECLARE_DYNAMIC(CPageLUV)

public:
	CPageLUV(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPageLUV();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PAGE_LUV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
