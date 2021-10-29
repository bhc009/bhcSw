// ImageViewer.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
// #include "crop_BHC.h"
#include "ImageViewer.h"


// CImageViewer

IMPLEMENT_DYNCREATE(CImageViewer, CScrollView)

CImageViewer::CImageViewer()
{
	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_bValidRoi = false;
}

CImageViewer::~CImageViewer()
{
}


BEGIN_MESSAGE_MAP(CImageViewer, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CImageViewer �׸����Դϴ�.

void CImageViewer::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImageViewer::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	if (GetImageBuffer())
	{
		// ���� ǥ��
		m_image.Draw(pDC->m_hDC,
			0, 0,
			GetImageWidth(),
			GetImageHeight());


		if (m_bValidRoi)
		{
			pDC->SetBkMode(TRANSPARENT);

			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(0,255,0) );    // ������ �� ����
			CPen* oldPen = pDC->SelectObject(&pen);

			pDC->SetTextColor(RGB(0, 255, 0));


			pDC->MoveTo(m_rtRoi.left, m_rtRoi.top);
			pDC->LineTo(m_rtRoi.right, m_rtRoi.top);
			pDC->LineTo(m_rtRoi.right, m_rtRoi.bottom);
			pDC->LineTo(m_rtRoi.left, m_rtRoi.bottom);
			pDC->LineTo(m_rtRoi.left, m_rtRoi.top);

			pDC->SelectObject(oldPen);
		}
	}
}


// CImageViewer �����Դϴ�.

#ifdef _DEBUG
void CImageViewer::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CImageViewer::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CImageViewer �޽��� ó�����Դϴ�.
void CImageViewer::invalidateView()
{
	CSize sizeTotal;
	sizeTotal.cx = GetImageWidth();
	sizeTotal.cy = GetImageHeight();
	SetScrollSizes(MM_TEXT, sizeTotal);

	Invalidate();
}



//
BOOL CImageViewer::LoadImage(CString strFilePath)
{
// 	CHAR strMB[1024];
// 	WideCharToMultiByte( CP_ACP, 0, strFilePath, -1, strMB, 1024, NULL, NULL );
// 
// 	m_image.Open( strMB );
	m_image.Open( strFilePath );

	invalidateView();

	return TRUE;
}

BYTE *CImageViewer::GetImageBuffer()
{
	return m_image.GetImage();
}

int CImageViewer::GetImageWidth()
{
	if (GetImageBuffer())
	{
		return m_image.GetWidth();
	}

	return 0;
}


int CImageViewer::GetImageHeight()
{
	if (GetImageBuffer())
	{
		return m_image.GetHeight();
	}

	return 0;
}


void CImageViewer::SetDrawing(bool bDraw)
{
//	m_bDraw = bDraw;

	invalidateView();
}

void CImageViewer::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	m_ptLBtnDown = point;

	CScrollView::OnLButtonDown(nFlags, point);
}


void CImageViewer::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	m_ptLBtnUp = point;

	CPoint ptScroll = GetScrollPosition();

	m_rtRoi = CRect(min(m_ptLBtnDown.x + ptScroll.x, m_ptLBtnUp.x + ptScroll.x),
					min(m_ptLBtnDown.y + ptScroll.y, m_ptLBtnUp.y + ptScroll.y),
					max(m_ptLBtnDown.x + ptScroll.x, m_ptLBtnUp.x + ptScroll.x),
					max(m_ptLBtnDown.y + ptScroll.y, m_ptLBtnUp.y + ptScroll.y));

	m_bValidRoi = true;

	invalidateView();

	CScrollView::OnLButtonUp(nFlags, point);
}
