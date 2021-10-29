// BhImage.h: interface for the CBhImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BHIMAGE_H__373CCD7B_9DC5_477B_92C0_6CB9EA369FA1__INCLUDED_)
#define AFX_BHIMAGE_H__373CCD7B_9DC5_477B_92C0_6CB9EA369FA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _GRAY_	0
#define _RGB_	1

class CBhImage  
{
public:
	void*	Create( int width, int height, int bpp, int imageType);

	BOOL	Open(CString fileName);
// 	BOOL	Open(LPCSTR fileName);
	void	Save(CString strFileName);
	void	Copy(CBhImage *img);

	void	Draw( HDC hdc, int x, int y );							// �̹����� �ѷ��ش�.
	void	Draw( HDC hdc, int x, int y, int width, int height );	// �־��� ���� ���̿� �°� �̹����� �ѷ��ش�.
	void	Invalidate();

// 	BYTE*	GetImage() { return m_pImageBuffer; };	// �̹����� ���� ������
	BYTE*	GetImage() { return m_pImage; };	// �̹����� ���� ������
	int		GetColor();
	BYTE	GetPixelGrey(int x, int y);		// �̹��� �ȼ��� ���� ��´�(Gray).
	RGBQUAD GetPixelColor(int x, int y);
	CSize	GetImageSize();					// �̹����� ���۸� ��´�.
	long	GetSize();						// �̹����� �����ϴ� ������ ũ�⸦ ��´�(�̹��� ũ��ʹ� �ٸ� �� �ִ�).
	int		GetHeight();					// �̹����� ���̸� ��´�.
	int		GetWidth();						// �̹����� ���� ��´�.
	int		GetWidthStep();					// �̹��� ������ ���� ��´�.
	BOOL	GetROI( BYTE *pImg, CSize szImg, CRect rtROI );
	BOOL	SetROI( BYTE *pImg, CSize szImg, CRect rtROI );

	void	SetImage(BYTE value);								// �־��� ������ ���۸� ä���.
	void	SetImage(BYTE *pImage, int iWidth, int iHeight);	// �־��� ���۷� �̹����� ä���.
	void	SetColorImage(BYTE *pImage, int iWidth, int iHeight);	// �־��� ���۷� �̹����� ä���.
	void	SetImage(BYTE *pImage, CSize szImage );	// �־��� ���۷� �̹����� ä���.
	void	SetPixelGrey(int x, int y, BYTE value);				// 

	BOOL	IsValid();					// �̹����� �ε��Ǿ� �ִ°�?
	BOOL	IsValidPoint(int x, int y);	// (x,y)�� �̹��� �� �����ϴ� ���ΰ�?
	void	Cut( int iStartX, int iStartY, int iWidth, int iHeight );
	
	CBhImage();
	virtual ~CBhImage();
protected:
	void	Init();					// ���� �ʱ�ȭ
	BOOL	Decode();

// 	void	FlipImage();			// ������ ����ϴ� �̹��� ���۷� DIP�� ������ �ű��.
	void	FlipImage(BYTE *pSrc, int width, int height, BYTE *pDst);			// 
// 	void	UnFlipImage();			// ���� ��� �̹��� ������ ������ DIP�� �ű��.

	BYTE*		GetBits();			// �̹��� ���ۿ� �ش��ϴ� ������ �����͸� �����´�.
	DWORD		GetPaletteSize();	// �ȷ�Ʈ�� ũ�⸦ ��´�.
	RGBQUAD*	GetPalette() const;	// �ȷ�Ʈ�� �����͸� ��´�.

protected:
	void				*m_pDib;			// contains the header, the palette, the pixels
    BITMAPINFOHEADER    m_infoHeader;		// standard header
	BYTE				*m_pImage;			// the pixels
// 	BYTE				*m_pImageBuffer;	// addition - ������ ���̴� �̹����� ������ ������ �迭
	int					m_widthStep;		// valid width
	char				m_strFileName[1024];
	bool				m_bIsImage;

	int m_nDumy;
};

#endif // !defined(AFX_BHIMAGE_H__373CCD7B_9DC5_477B_92C0_6CB9EA369FA1__INCLUDED_)
