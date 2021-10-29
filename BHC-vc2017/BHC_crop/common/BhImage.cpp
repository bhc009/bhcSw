// BhImage.cpp: implementation of the CBhImage class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "BhImage.h"

#include "io.h"
#include "fcntl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DIB_HEADER_MAKER ((WORD)('M' << 8) | 'B')//BM MARKER
#define DibNumColors(lpbi)      ((lpbi)->biClrUsed == 0 && (lpbi)->biBitCount <= 8 \
                                    ? (int)(1 << (int)(lpbi)->biBitCount)          \
                                    : (int)(lpbi)->biClrUsed)


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBhImage::CBhImage()
{
	m_pDib		= NULL;
	m_pImage	= NULL;
	m_bIsImage	= false;
	m_nDumy = 0;

	OutputDebugString(_T("BH's image... v2.0\n"));
}

CBhImage::~CBhImage()
{
	if( m_pDib )
	{
		delete[] m_pDib;
	}
	m_pDib = NULL;

	m_pImage = NULL;
}

void CBhImage::Init()
{
	if( m_pDib )
	{
		delete[] m_pDib;
	}
	m_pDib = NULL;

	m_pImage = NULL;
}

void* CBhImage::Create(int width, int height, int bpp, int imageType)
{
	//.. initialize buffers
	Init();

	//.. 1. Set header
	m_infoHeader.biSize		= sizeof(BITMAPINFOHEADER);
    m_infoHeader.biWidth	= width;			// fill in width from parameter
    m_infoHeader.biHeight	= -height;			// fill in height from parameter
    m_infoHeader.biPlanes	= 1;				// must be 1
    m_infoHeader.biBitCount	= (WORD)bpp;		// from parameter
    m_infoHeader.biCompression	= BI_RGB; 
    m_infoHeader.biClrImportant	= 0;
	m_infoHeader.biXPelsPerMeter = 0;			//See SetXDPI
	m_infoHeader.biYPelsPerMeter = 0;			//See SetYDPI
/*	int ndumy = width%4;
	((4-ndumy)==4)? ndumy=0 : ndumy=4-ndumy;
	m_widthStep = width + ndumy;
    m_infoHeader.biSizeImage	= m_widthStep * height;*/
	
	// set the correct bpp value
    switch (bpp)
	{
        case 1:
            m_infoHeader.biClrUsed = 2;
			break;

        case 4:
            m_infoHeader.biClrUsed = 16; 
			break;

        case 8:
            m_infoHeader.biClrUsed = 256;
			m_nDumy = width%4;
			((4-m_nDumy)==4)? m_nDumy=0 : m_nDumy=4-m_nDumy;
			m_widthStep = width + m_nDumy;
			break;

        default:
            m_infoHeader.biClrUsed = 0;
 			m_nDumy = (3*width)%4;
			((4-m_nDumy)==4)? m_nDumy=0 : m_nDumy=4-m_nDumy;
			m_widthStep = 3*width + m_nDumy;
   }
    m_infoHeader.biSizeImage	= m_widthStep * height;


	//.. 2. Set dib
	m_pDib = malloc(GetSize()); // allocate memory block to store our bitmap
    if (!m_pDib)
	{
		return NULL;
	}


	//.. 3. Clear & Set the palette
	RGBQUAD *lpPal;
    lpPal = GetPalette();	// ������ �ѱ��� �� ������ �Ʒ�ó�� 0-255������ GRAY VALUE�� ��������.. ���� �̰� ���� ���� ����ߴµ�.. �̺κ��� ������ �ǽŴٸ� �̤�
    for(int i = 0; i < int(m_infoHeader.biClrUsed); i++)
    {
        lpPal[i].rgbRed = i;
        lpPal[i].rgbGreen = i;
        lpPal[i].rgbBlue = i;
        lpPal[i].rgbReserved = i;
    }

    // use our bitmap info structure to fill in first part of
    // our DIB with the BITMAPINFOHEADER
	BITMAPINFOHEADER*  lpbi;
	lpbi = (BITMAPINFOHEADER*)(m_pDib);
    *lpbi = m_infoHeader;

	m_pImage = GetBits();


	//
	m_bIsImage = true;

	return m_pDib;
}

long CBhImage::GetSize()
{
	return m_infoHeader.biSize + m_infoHeader.biSizeImage + GetPaletteSize();
}

BYTE* CBhImage::GetBits()
{ 
	if (m_pDib)	
	{
		return ((BYTE*)m_pDib + *(DWORD*)m_pDib + GetPaletteSize()); 
	}
	
	return NULL;
}

DWORD CBhImage::GetPaletteSize()
{
	return (m_infoHeader.biClrUsed * sizeof(RGBQUAD));
}

RGBQUAD* CBhImage::GetPalette() const
{
	if ((m_pDib)&&(m_infoHeader.biClrUsed))
	{
		return (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
	}

	return NULL;
}

BOOL CBhImage::Open(CString fileName)
//BOOL CBhImage::Open(LPCSTR fileName)
{
// 	strcpy_s(m_strFileName,fileName);
// 	WideCharToMultiByte(CP_ACP,0,(LPCWSTR)fileName,fileName.GetLength(),m_strFileName,fileName.GetLength(),NULL,NULL);
	CHAR strMB[1024];
	WideCharToMultiByte( CP_ACP, 0, fileName, -1, strMB, 1024, NULL, NULL );
	WideCharToMultiByte( CP_ACP, 0, fileName, -1, m_strFileName, 1024, NULL, NULL );

// 	m_strFileName = strMB;
	
	if( !Decode() )
	{
		return FALSE;
	}


	return TRUE;
}

void CBhImage::Save(CString strFileName)
{
	DWORD len;
	int nColor = m_infoHeader.biClrUsed;
	
	HANDLE fd= CreateFile(strFileName,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	// Coding about BITMAPFILEHEADER
	BITMAPFILEHEADER   bfHeader;
    bfHeader.bfType		= DIB_HEADER_MAKER;//'BM'
	bfHeader.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + ((nColor)*sizeof(RGBQUAD));
	bfHeader.bfSize		= sizeof(bfHeader) + ((nColor-1)*sizeof(RGBQUAD)) + sizeof(BITMAPINFO) + m_infoHeader.biSizeImage;
	bfHeader.bfReserved1	= 0;
	bfHeader.bfReserved2	= 0;

	// 1. file header
	if(!WriteFile(fd,(LPSTR)&bfHeader,sizeof(bfHeader),&len,NULL))
	{
		return;
	}

	// 2. bitmap info+palette
	BITMAPINFOHEADER    infoHeader = m_infoHeader;		// standard header
	infoHeader.biHeight = abs(infoHeader.biHeight);		// ����...

// 	if(!WriteFile(fd,&m_infoHeader,sizeof(BITMAPINFOHEADER),&len,NULL))
	if(!WriteFile(fd,&infoHeader,sizeof(BITMAPINFOHEADER),&len,NULL))
	{
		return;
	}

	// 2.2 SetPallete
	BYTE        *lpPal;
    lpPal = (BYTE *)GetPalette();// ������ �ѱ��� �� ������ �Ʒ�ó�� 0-255������ GRAY VALUE�� ��������.. ���� �̰� ���� ���� ����ߴµ�.. �̺κ��� ������ �ǽŴٸ� �̤�
    DWORD offDest = 0;
    for(int i = 0; i < GetColor(); i++)
    {
        lpPal[offDest++] = (BYTE)i;
        lpPal[offDest++] = (BYTE)i;
        lpPal[offDest++] = (BYTE)i;
        lpPal[offDest++] = 0x00;
    }

	if(!WriteFile(fd,GetPalette(),GetPaletteSize(),&len,NULL))
	{
		return;
	}

	// 3. image data
	BYTE *pFilpImage = new BYTE[m_infoHeader.biSizeImage];
	FlipImage(m_pImage, GetWidthStep(), GetHeight(), pFilpImage);

	if(!WriteFile(fd, pFilpImage, m_infoHeader.biSizeImage,&len,NULL))
	{
		delete[] pFilpImage;

		return;
	}

	delete[] pFilpImage;


	CloseHandle(fd);
}

BOOL CBhImage::Decode()
{
	DWORD len;
	
	//////////////////////////////////////////////////////////////////////////
	//.. file open
#ifdef UNICODE
// 	HANDLE fd=CreateFile((LPCWSTR)m_strFileName,GENERIC_READ,
// 			0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	WCHAR strWC[1024];
	MultiByteToWideChar( CP_ACP, 0, m_strFileName, -1, strWC, 1024 );

	HANDLE fd=CreateFile(strWC,GENERIC_READ,
			0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
#else
	HANDLE fd=CreateFile(m_strFileName,GENERIC_READ,
		0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
#endif


	//////////////////////////////////////////////////////////////////////////
	//
	BITMAPFILEHEADER   bf;
    //if (hFile->Read(&bf,sizeof(bf),1)==0) throw "Not a BMP";
	if( !ReadFile(fd,&bf,sizeof(bf),&len,NULL) )
	{
		return FALSE;
	}


	//////////////////////////////////////////////////////////////////////////
	// infoHeader
    //if (fh->Read(pdib,sizeof(BITMAPINFOHEADER),1)==0) return false;
	if( !ReadFile(fd,&m_infoHeader,sizeof(BITMAPINFOHEADER),&len,NULL) )
	{
		return FALSE;
	}


	//////////////////////////////////////////////////////////////////////////
	//
	Create(m_infoHeader.biWidth,m_infoHeader.biHeight,m_infoHeader.biBitCount,0);


	//////////////////////////////////////////////////////////////////////////
	// pallete
	RGBQUAD *pRgb = GetPalette();
	//ReadFile(fd,(void*)pRgb,DibNumColors(&m_infoHeader) * sizeof(RGBTRIPLE),&len,NULL);
    if( !ReadFile(fd,(void*)pRgb,DibNumColors(&m_infoHeader) * sizeof(RGBQUAD),&len,NULL) )
	{
		return FALSE;
	}


	//////////////////////////////////////////////////////////////////////////
	// image Data
	BYTE *pTempBuff = new BYTE[m_infoHeader.biSizeImage];

	if( !ReadFile(fd, pTempBuff, m_infoHeader.biSizeImage,&len,NULL) )	// read in the pixels
	{
		return FALSE; 
	}

	// filp image data
	FlipImage(	pTempBuff, GetWidthStep(), GetHeight(), m_pImage);

	delete[] pTempBuff;


	//////////////////////////////////////////////////////////////////////////
	//..
	if( !CloseHandle(fd) )
	{
		return FALSE;
	}

	return TRUE;
}

void CBhImage::Draw(HDC hdc, int x, int y, int width, int height)
{
	SetStretchBltMode(	hdc, COLORONCOLOR );	
	StretchDIBits(	hdc, 
					x, y, width, height, 
					0, 0, GetWidth(), GetHeight(),
					m_pImage,
					(BITMAPINFO*)m_pDib,
					DIB_RGB_COLORS,
					SRCCOPY );

}

void CBhImage::Draw(HDC hdc, int x, int y)
{
	SetStretchBltMode( hdc, COLORONCOLOR );	
	SetDIBitsToDevice(	hdc, 
						x, y, GetWidth(), GetHeight(), 
						0, 0, 0, GetHeight(),
						m_pImage,
						(BITMAPINFO*)m_pDib,
						DIB_RGB_COLORS );
}

int CBhImage::GetWidth()
{
	return m_infoHeader.biWidth;
}

int CBhImage::GetHeight()
{
	return abs(m_infoHeader.biHeight);
}

CSize CBhImage::GetImageSize()
{
	return CSize( GetWidth(), GetHeight() );
}

int CBhImage::GetWidthStep()
{
	return m_widthStep;
}

int CBhImage::GetColor()
{
	switch(m_infoHeader.biBitCount) 
	{
		case 24:	// RGB
			return 0;

		case 8:		// Gray
			return 8;

		default:
			return -1;
	}
}

RGBQUAD CBhImage::GetPixelColor(int x, int y)
{
	RGBQUAD	rgb = { 0, 0, 0 };

	if( m_pDib )
	{
		rgb.rgbRed		= m_pImage[ 3*x+ 2 +y*m_widthStep ];
		rgb.rgbGreen	= m_pImage[ 3*x+ 1 +y*m_widthStep ];
		rgb.rgbBlue		= m_pImage[ 3*x+ 0 +y*m_widthStep ];
// 		rgb.rgbRed		= m_pImageBuffer[ 3*x+ 2 +y*3*GetWidth() ];
// 		rgb.rgbGreen	= m_pImageBuffer[ 3*x+ 1 +y*3*GetWidth() ];
// 		rgb.rgbBlue		= m_pImageBuffer[ 3*x+ 0 +y*3*GetWidth() ];
	}
		
	return rgb;
}

BYTE CBhImage::GetPixelGrey(int x, int y)
{
	if( m_pDib )
	{
		if( IsValidPoint(x, y) )
		{
			return m_pImage[ x + y*GetWidthStep() ];
// 			return m_pImageBuffer[ x + y*GetWidth() ];
		}
	}

	return 0;
}

void CBhImage::SetPixelGrey(int x, int y, BYTE value)
{
	if( m_pDib )
	{
		if( IsValidPoint(x, y) )
		{
			m_pImage[ x + y*GetWidthStep() ] = value;
// 			m_pImageBuffer[ x + y*GetWidth() ] = value;
		}
	}
}

BOOL CBhImage::IsValid()
{
	if( m_pDib )
	{
		return TRUE;
	}

	return FALSE;
}

void CBhImage::SetImage(BYTE value)
{
	if( m_pDib )
	{
		if( m_infoHeader.biBitCount==8 )
		{
			memset( m_pImage, value, sizeof(BYTE)*GetWidthStep()*GetHeight() );
// 			memset( m_pImageBuffer, value, sizeof(BYTE)*GetWidth()*GetHeight() );
		}
		else if( m_infoHeader.biBitCount==24 )
		{
			memset( m_pImage, value, sizeof(BYTE)*GetWidthStep()*GetHeight() );
// 			memset( m_pImageBuffer, value, sizeof(BYTE)*3*GetWidth()*GetHeight() );
		}
	}
}

void CBhImage::SetImage(BYTE *pImage, int iWidth, int iHeight)
{
	Create( iWidth, iHeight, 8, 0 );

	int iPosY_Src = 0;
	int iPosY_Dst = 0;
	for( int y=0 ; y<iHeight ; y++ )
	{
		memcpy( GetImage()+iPosY_Dst, pImage+iPosY_Src, iWidth );

		iPosY_Src += iWidth;
		iPosY_Dst += iWidth;
	}

	Invalidate();
}

void CBhImage::SetColorImage(BYTE *pImage, int iWidth, int iHeight)
{
	Create( iWidth, iHeight, 24, 0 );

	int iPosY_Src = 0;
	int iPosY_Dst = 0;
	for( int y=0 ; y<iHeight ; y++ )
	{
		memcpy( GetImage()+iPosY_Dst, pImage+iPosY_Src, 3*iWidth );

		iPosY_Src += 3*iWidth;
		iPosY_Dst += 3*iWidth;
	}

	Invalidate();
}

void CBhImage::SetImage(BYTE *pImage, CSize szImage )
{
	SetImage(pImage, szImage.cx, szImage.cy );
}

//////////////////////////////////////////////////////////////////////////
//
// ������ ���ϸ� �����´�.
//
//////////////////////////////////////////////////////////////////////////
// void CBhImage::FlipImage()
// {
// 	if( m_pDib )
// 	{
// 		// Gray
// 		if( m_infoHeader.biBitCount==8 )
// 		{
// 			if( m_pImageBuffer )
// 			{
// 				delete[] m_pImageBuffer;
// 			}
// 
// 			m_pImageBuffer = new BYTE[GetWidth()*GetHeight()];
// 
// 			int iPosY = 0;
// 			int iPosY_flip = GetWidth()*( GetHeight() - 1 );
// 			for( int y=0 ; y<GetHeight() ; y++ )
// 			{
// 				memcpy( m_pImageBuffer+iPosY_flip, m_pImage+iPosY, sizeof(BYTE)*GetWidth() );
// 
// 				iPosY		+= m_widthStep;
// 				iPosY_flip	-= GetWidth();
// 			}
// 		}
// 		// RGB
// 		else if( m_infoHeader.biBitCount==24 )
// 		{
// 			if( m_pImageBuffer )
// 			{
// 				delete[] m_pImageBuffer;
// 			}
// 
// 			m_pImageBuffer = new BYTE[3*GetWidth()*GetHeight()];
// 
// 			for( int y=0 ; y<GetHeight() ; y++ )
// 			{
// 				int yflip = GetHeight() - y - 1;
// 				for( int x=0 ; x<3*GetWidth() ; x++ )
// 				{
// 					m_pImageBuffer[x+yflip*3*GetWidth()] = m_pImage[ x + y*m_widthStep ];
// 				}
// 			}
// 		}
// 	}
// }


//////////////////////////////////////////////////////////////////////////
//
// ������ ���ϸ� �����´�.
//
//////////////////////////////////////////////////////////////////////////
void CBhImage::FlipImage(BYTE *pSrc, int width, int height, BYTE *pDst)
{
	for( int y=0 ; y<height ; y++ )
	{
		memcpy( pDst + (height - 1 - y)*width,
				pSrc + y*width,
				width);
	}
}


//////////////////////////////////////////////////////////////////////////
//
// ���� BMP�̹����� ������ ������Ʈ �Ѵ�.
//
//////////////////////////////////////////////////////////////////////////
// void CBhImage::UnFlipImage()
// {
// 	if( m_pDib )
// 	{
// 		// Gray
// 		if( m_infoHeader.biBitCount==8 )
// 		{
// 			int iPosY		= 0;
// 			int iPosY_flip	= m_widthStep*(GetHeight() - 1);
// 			for( int y=0 ; y<GetHeight() ; y++ )
// 			{
// 				memcpy( m_pImage+iPosY_flip, m_pImageBuffer+iPosY, GetWidth() );
// 				iPosY		+= GetWidth();
// 				iPosY_flip	-= m_widthStep;
// 			}
// 		}
// 		// RGB
// 		else if( m_infoHeader.biBitCount==24 )
// 		{
// 			for( int y=0 ; y<GetHeight() ; y++ )
// 			{
// 				int yflip = GetHeight() - y - 1;
// 				for( int x=0 ; x<3*GetWidth() ; x++ )
// 				{
// 					m_pImage[x+yflip*m_widthStep] = m_pImageBuffer[ x + y*3*GetWidth() ];
// 				}
// 			}
// 		}
// 	}
// }

//////////////////////////////////////////////////////////////////////////
//
// �� ������ ��ȿ�� Ȯ��
//
//////////////////////////////////////////////////////////////////////////
BOOL CBhImage::IsValidPoint(int x, int y)
{
	if( m_pDib )
	{
		if( x>=0 && x<GetWidth() && y>=0 && y<GetHeight() )
		{
			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//
// �̹��� ����
//
//////////////////////////////////////////////////////////////////////////
void CBhImage::Invalidate()
{
// 	if( m_pDib )
// 	{
// 		UnFlipImage();
// 	}
}


//////////////////////////////////////////////////////////////////////////
// copy image
void CBhImage::Copy(CBhImage *img)
{
	if( img->IsValid() )
	{
		Create( img->GetWidth(), img->GetHeight(), img->GetColor(), 0 );

		//memcpy( GetImage(), img->GetImage(), sizeof(BYTE)*img->GetWidthStep()*img->GetHeight() );
		memcpy( GetImage(), img->GetImage(), sizeof(BYTE)*img->GetWidth()*img->GetHeight() );

		Invalidate();
	}
}

//////////////////////////////////////////////////////////////////////////
//
// ROI���� ������ �������� �����.
//
//////////////////////////////////////////////////////////////////////////
void CBhImage::Cut( int iStartX, int iStartY, int iWidth, int iHeight )
{
	// *Only Gray
	int x, y, tempX, tempY;
	int iPosY, iPosY_Temp, iPos, iPos_Temp;

	//.. 1. Get Contents of ROI
	BYTE *pTemp = new BYTE[iWidth*iHeight];
	for( y=iStartY, tempY=0 ; y<iStartY+iHeight ; y++, tempY++ )
	{
		iPosY		= y*GetWidth();
		iPosY_Temp	= tempY*iWidth;
		for( x=iStartX, tempX=0 ; x<iStartX+iWidth ; x++, tempX++ )
		{
			iPos		= x + iPosY;
			iPos_Temp	= tempX + iPosY_Temp;
			pTemp[iPos_Temp] = GetImage()[iPos];
		}
	}

	//.. 2. Resize image
	Create( iWidth, iHeight, 8, 0 );

	//.. 3. Set ROI
	SetImage( pTemp, iWidth*iHeight );
//	for( y=0 ; y<iHeight ; y++ )
//	{
//		iPosY = y*iWidth;
//		for( x=0 ; x<iWidth ; x++ )
//		{
//			iPos = x + iPosY;
//			GetImage()[iPos] = pTemp[iPos];
//		}
//	}
//	Invalidate();
	
	delete[] pTemp;
}

//////////////////////////////////////////////////////////////////////////
//
// ROI������ data�� ��´�.
//
//////////////////////////////////////////////////////////////////////////
BOOL CBhImage::GetROI( BYTE *pImg, CSize szImg, CRect rtROI )
{
	if( szImg.cx!=rtROI.Width() || szImg.cy!=rtROI.Height() || 
		rtROI.left<0 || rtROI.top<0 || rtROI.right>GetWidth() || rtROI.bottom>GetHeight() )
	{
		return FALSE;
	}

	int x, y, xDst, yDst;
	int iPosY = rtROI.top * GetWidth();
	int iPosDstY = 0;
	for( yDst=0, y=rtROI.top ; y<rtROI.bottom ; yDst++, y++ )
	{
		for( xDst=0, x=rtROI.left ; x<rtROI.right ; xDst++, x++ )
		{
			pImg[ xDst + iPosDstY ] = GetImage()[x + iPosY ];
		}
		iPosY		+= GetWidth();
		iPosDstY	+= szImg.cx;
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
// ROI������ data�� �����Ѵ�.
//
//////////////////////////////////////////////////////////////////////////
BOOL CBhImage::SetROI( BYTE *pImg, CSize szImg, CRect rtROI )
{
	if( szImg.cx!=rtROI.Width() || szImg.cy!=rtROI.Height() || 
		rtROI.left<0 || rtROI.top<0 || rtROI.right>GetWidth() || rtROI.bottom>GetHeight() )
	{
		return FALSE;
	}

	int x, y, xDst, yDst;
	int iPosY = rtROI.top * GetWidth();
	int iPosDstY = 0;
	for( yDst=0, y=rtROI.top ; y<rtROI.bottom ; yDst++, y++ )
	{
		for( xDst=0, x=rtROI.left ; x<rtROI.right ; xDst++, x++ )
		{
			 GetImage()[x + iPosY ] = pImg[ xDst + iPosDstY ];
		}
		iPosY		+= GetWidth();
		iPosDstY	+= szImg.cx;
	}

	return TRUE;
}

