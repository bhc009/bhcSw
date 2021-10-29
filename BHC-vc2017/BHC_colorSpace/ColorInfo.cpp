#include "StdAfx.h"
#include "ColorInfo.h"
#include <math.h>

CColorInfo::CColorInfo(void)
{
	m_iWidth	= 0;
	m_iHeight	= 0;
}


CColorInfo::~CColorInfo(void)
{
}


bool CColorInfo::IsValid()
{
	if( m_iWidth <= 0 || m_iHeight <= 0 )
	{
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	SetRgb
//
//	RGB data setting : 8 bit
//
//////////////////////////////////////////////////////////////////////////
bool CColorInfo::SetRgb( BYTE *pData, int iWidth, int iHeight )
{
	if( iWidth < 1 || iHeight < 1 )
	{
		return false;
	}


	m_stRgb.SetData(pData, iWidth, iHeight);


	m_iWidth = iWidth;
	m_iHeight = iHeight;



	//
	//
	//
	GenXyz_fromRgb();

	GenXyy_fromRgb();


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	SetRgb
//
//	RGB data setting : 8 bit
//
//////////////////////////////////////////////////////////////////////////
bool CColorInfo::SetRgb( BYTE *pRed, BYTE *pGreen, BYTE *pBlue, int iWidth, int iHeight )
{
	if( iWidth < 1 || iHeight < 1 )
	{
		return false;
	}


	//
	m_stRgb.SetData(pRed, pGreen, pBlue, iWidth, iHeight);



	m_iWidth = iWidth;
	m_iHeight = iHeight;


	//
	//
	//
	GenXyz_fromRgb();

	GenXyy_fromRgb();


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	SetRgb
//
//	RGB data setting : 16 bit
//
//////////////////////////////////////////////////////////////////////////
bool CColorInfo::SetRgb( unsigned short *pData, int iWidth, int iHeight )
{
	if( iWidth < 1 || iHeight < 1 )
	{
		return false;
	}


	m_stRgb.SetData(pData, iWidth, iHeight);


	m_iWidth = iWidth;
	m_iHeight = iHeight;



	//
	//
	//
	GenXyz_fromRgb();

	GenXyy_fromRgb();


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	SetRgb
//
//	RGB data setting : 16 bit
//
//////////////////////////////////////////////////////////////////////////
bool CColorInfo::SetRgb( unsigned short *pRed, unsigned short *pGreen, unsigned short *pBlue, int iWidth, int iHeight )
{
	if( iWidth < 1 || iHeight < 1 )
	{
		return false;
	}


	//
	m_stRgb.SetData(pRed, pGreen, pBlue, iWidth, iHeight);



	m_iWidth = iWidth;
	m_iHeight = iHeight;


	//
	//
	//
	GenXyz_fromRgb();

	GenXyy_fromRgb();


	return true;
}



bool CColorInfo::SetXYZ( double *pData, int iWidth, int iHeight )
{
	if( iWidth < 1 || iHeight < 1 )
	{
		return false;
	}

	m_stXyz.SetData( pData, iWidth, iHeight );

	m_iWidth	= iWidth;
	m_iHeight	= iHeight;

	return true;
}


bool CColorInfo::SetXYZ( double *pX, double *pY, double *pZ, int iWidth, int iHeight )
{
	if( iWidth < 1 || iHeight < 1 )
	{
		return false;
	}


	m_stXyz.SetData( pX, pY, pZ, iWidth, iHeight );


	m_iWidth	= iWidth;
	m_iHeight	= iHeight;




	//
	//
	//
	GenXyy_fromXyz();

	GenRgb_fromXyz();


	return true;
}


bool CColorInfo::GetSize( int *pWidth, int *pHeight )
{
	if( IsValid() == false )
	{
		return false;
	}


	*pWidth		= m_iWidth;
	*pHeight	= m_iHeight;


	return true;
}


stRgb* CColorInfo::GetRgb()
{
	if( IsValid() == false )
	{
		return NULL;
	}

	return &m_stRgb;
}


stColor* CColorInfo::GetXyz()
{
	if( IsValid() == false )
	{
		return NULL;
	}

	return &m_stXyz;
}


stColor* CColorInfo::GetXyy(  )
{
	if( IsValid() == false )
	{
		return NULL;
	}


	return &m_stXyy;
}


stColor* CColorInfo::GetLuv()
{
	if( IsValid() == false )
	{
		return NULL;
	}


	return &m_stLuv;
}


//////////////////////////////////////////////////////////////////////////
//
//	GenXyz_fromRgb
//
//	convert from rgb to xyz
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CColorInfo::GenXyz_fromRgb()
{
	bool bResult;

	if( m_stRgb.iDataType == _BHC_COLOR_RGB_8 )
	{
		bResult = GenXyz_fromRgb8();
	}

	if( m_stRgb.iDataType == _BHC_COLOR_RGB_16 )
	{
		bResult = GenXyz_fromRgb16();
	}

	return bResult;
}


//////////////////////////////////////////////////////////////////////////
//
//	GenXyz_fromRgb
//
//	convert from rgb to xyy
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CColorInfo::GenXyy_fromRgb()
{
	bool bResult;

	if( m_stRgb.iDataType == _BHC_COLOR_RGB_8 )
	{
		bResult = GenXyy_fromRgb8();
	}

	if( m_stRgb.iDataType == _BHC_COLOR_RGB_16 )
	{
		bResult = GenXyy_fromRgb16();
	}

	return bResult;
}


bool CColorInfo::GenLuv_fromRgb()
{
	bool bResult;

	if( m_stRgb.iDataType == _BHC_COLOR_RGB_8 )
	{
		bResult = GenLuv_fromRgb8();
	}

	if( m_stRgb.iDataType == _BHC_COLOR_RGB_16 )
	{
		bResult = GenLuv_fromRgb16();
	}

	return bResult;
}


//////////////////////////////////////////////////////////////////////////
//
//	GenXyz_fromRgb
//
//	convert from rgb to xyz
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CColorInfo::GenXyz_fromRgb8()
{
	//
	// get data
	//
	BYTE* pRed		= m_stRgb.pData_8[_BHC_COLOR_RGB_R];
	BYTE* pGreen	= m_stRgb.pData_8[_BHC_COLOR_RGB_G];
	BYTE* pBlue		= m_stRgb.pData_8[_BHC_COLOR_RGB_B];


	//
	// gen buffer
	//
	double *pXyz_x = new double[ m_iWidth * m_iHeight ];
	double *pXyz_y = new double[ m_iWidth * m_iHeight ];
	double *pXyz_z = new double[ m_iWidth * m_iHeight ];

	memset( pXyz_x, 0, sizeof(double) * m_iWidth * m_iHeight);
	memset( pXyz_y, 0, sizeof(double) * m_iWidth * m_iHeight);
	memset( pXyz_z, 0, sizeof(double) * m_iWidth * m_iHeight);


	// make data
	for( int i=0 ; i<m_iWidth*m_iHeight ; i++ )
	{
		pXyz_x[i] = 0.4887180 * pRed[i] + 0.3106803 * pGreen[i] + 0.2006017 * pBlue[i];
		pXyz_y[i] = 0.1762044 * pRed[i] + 0.8129847 * pGreen[i] + 0.0108109 * pBlue[i];
		pXyz_z[i] = 0.0000000 * pRed[i] + 0.0102048 * pGreen[i] + 0.9897952 * pBlue[i];
	}


	// set data
	m_stXyz.SetData( pXyz_x, pXyz_y, pXyz_z, m_iWidth, m_iHeight );


	delete[] pXyz_x;
	delete[] pXyz_y;
	delete[] pXyz_z;


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	GenXyz_fromRgb
//
//	convert from rgb to xyy
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CColorInfo::GenXyy_fromRgb8()
{
	//
	// get data
	//
	BYTE* pRed		= m_stRgb.pData_8[_BHC_COLOR_RGB_R];
	BYTE* pGreen	= m_stRgb.pData_8[_BHC_COLOR_RGB_G];
	BYTE* pBlue		= m_stRgb.pData_8[_BHC_COLOR_RGB_B];


	//
	// gen buffer
	//
	double *pXyy_x = new double[ m_iWidth * m_iHeight ];
	double *pXyy_y = new double[ m_iWidth * m_iHeight ];
	double *pXyy_yy = new double[ m_iWidth * m_iHeight ];

	memset( pXyy_x, 0, sizeof(double) * m_iWidth * m_iHeight);
	memset( pXyy_y, 0, sizeof(double) * m_iWidth * m_iHeight);
	memset( pXyy_yy, 0, sizeof(double) * m_iWidth * m_iHeight);


	// make data
	for( int i=0 ; i<m_iWidth*m_iHeight ; i++ )
	{
		double dX = 0.4887180 * pRed[i] + 0.3106803 * pGreen[i] + 0.2006017 * pBlue[i];
		double dY = 0.1762044 * pRed[i] + 0.8129847 * pGreen[i] + 0.0108109 * pBlue[i];
		double dZ = 0.0000000 * pRed[i] + 0.0102048 * pGreen[i] + 0.9897952 * pBlue[i];

		double dSum = dX + dY + dZ;

		if( dSum==0 )
		{
			pXyy_x[i]	= 0;
			pXyy_y[i]	= 0;
			pXyy_yy[i]	= 0;
		} else {
			pXyy_x[i]	= dX / ( dX + dY + dZ );
			pXyy_y[i]	= dY / ( dX + dY + dZ );
			pXyy_yy[i]	= dZ / ( dX + dY + dZ );
		}
	}


	// set data
	m_stXyy.SetData( pXyy_x, pXyy_y, pXyy_yy, m_iWidth, m_iHeight );


	delete[] pXyy_x;
	delete[] pXyy_y;
	delete[] pXyy_yy;


	return true;
}


bool CColorInfo::GenLuv_fromRgb8()
{

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	GenXyz_fromRgb
//
//	convert from rgb to xyz
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CColorInfo::GenXyz_fromRgb16()
{
	//
	// get data
	//
	unsigned short* pRed	= m_stRgb.pData_16[_BHC_COLOR_RGB_R];
	unsigned short* pGreen	= m_stRgb.pData_16[_BHC_COLOR_RGB_G];
	unsigned short* pBlue	= m_stRgb.pData_16[_BHC_COLOR_RGB_B];


	//
	// gen buffer
	//
	double *pXyz_x = new double[ m_iWidth * m_iHeight ];
	double *pXyz_y = new double[ m_iWidth * m_iHeight ];
	double *pXyz_z = new double[ m_iWidth * m_iHeight ];

	memset( pXyz_x, 0, sizeof(double) * m_iWidth * m_iHeight);
	memset( pXyz_y, 0, sizeof(double) * m_iWidth * m_iHeight);
	memset( pXyz_z, 0, sizeof(double) * m_iWidth * m_iHeight);


	// make data
	for( int i=0 ; i<m_iWidth*m_iHeight ; i++ )
	{
		pXyz_x[i] = 0.4887180 * pRed[i] + 0.3106803 * pGreen[i] + 0.2006017 * pBlue[i];
		pXyz_y[i] = 0.1762044 * pRed[i] + 0.8129847 * pGreen[i] + 0.0108109 * pBlue[i];
		pXyz_z[i] = 0.0000000 * pRed[i] + 0.0102048 * pGreen[i] + 0.9897952 * pBlue[i];
	}


	// set data
	m_stXyz.SetData( pXyz_x, pXyz_y, pXyz_z, m_iWidth, m_iHeight );


	delete[] pXyz_x;
	delete[] pXyz_y;
	delete[] pXyz_z;


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	GenXyz_fromRgb
//
//	convert from rgb to xyy
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CColorInfo::GenXyy_fromRgb16()
{
	//
	// get data
	//
	unsigned short* pRed	= m_stRgb.pData_16[_BHC_COLOR_RGB_R];
	unsigned short* pGreen	= m_stRgb.pData_16[_BHC_COLOR_RGB_G];
	unsigned short* pBlue	= m_stRgb.pData_16[_BHC_COLOR_RGB_B];


	//
	// gen buffer
	//
	double *pXyy_x = new double[ m_iWidth * m_iHeight ];
	double *pXyy_y = new double[ m_iWidth * m_iHeight ];
	double *pXyy_yy = new double[ m_iWidth * m_iHeight ];

	memset( pXyy_x, 0, sizeof(double) * m_iWidth * m_iHeight);
	memset( pXyy_y, 0, sizeof(double) * m_iWidth * m_iHeight);
	memset( pXyy_yy, 0, sizeof(double) * m_iWidth * m_iHeight);


	// make data
	for( int i=0 ; i<m_iWidth*m_iHeight ; i++ )
	{
		double dX = 0.4887180 * pRed[i] + 0.3106803 * pGreen[i] + 0.2006017 * pBlue[i];
		double dY = 0.1762044 * pRed[i] + 0.8129847 * pGreen[i] + 0.0108109 * pBlue[i];
		double dZ = 0.0000000 * pRed[i] + 0.0102048 * pGreen[i] + 0.9897952 * pBlue[i];

		double dSum = dX + dY + dZ;

		if( dSum==0 )
		{
			pXyy_x[i]	= 0;
			pXyy_y[i]	= 0;
			pXyy_yy[i]	= 0;
		} else {
			pXyy_x[i]	= dX / ( dX + dY + dZ );
			pXyy_y[i]	= dY / ( dX + dY + dZ );
			pXyy_yy[i]	= dZ / ( dX + dY + dZ );
		}
	}


	// set data
	m_stXyy.SetData( pXyy_x, pXyy_y, pXyy_yy, m_iWidth, m_iHeight );


	delete[] pXyy_x;
	delete[] pXyy_y;
	delete[] pXyy_yy;


	return true;
}


bool CColorInfo::GenLuv_fromRgb16()
{

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//
//	X, Y and Z input refer to a D65/2¡Æ standard illuminant.
//	sR, sG and sB (standard RGB) output range = 0 ¡À 255
//
//	var_X = X / 100
//	var_Y = Y / 100
//	var_Z = Z / 100
//
//	var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986
//	var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415
//	var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570
//
//	if ( var_R > 0.0031308 ) var_R = 1.055 * ( var_R ^ ( 1 / 2.4 ) ) - 0.055
//	else                     var_R = 12.92 * var_R
//	if ( var_G > 0.0031308 ) var_G = 1.055 * ( var_G ^ ( 1 / 2.4 ) ) - 0.055
//	else                     var_G = 12.92 * var_G
//	if ( var_B > 0.0031308 ) var_B = 1.055 * ( var_B ^ ( 1 / 2.4 ) ) - 0.055
//	else                     var_B = 12.92 * var_B
//
//	sR = var_R * 255
//	sG = var_G * 255
//	sB = var_B * 255
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CColorInfo::GenRgb_fromXyz()
{
	// gen buffer
	BYTE *pRed		= new BYTE[m_iWidth*m_iHeight];
	BYTE *pGreen	= new BYTE[m_iWidth*m_iHeight];
	BYTE *pBlue		= new BYTE[m_iWidth*m_iHeight];


	//
	m_stRgb.iWidth = m_iWidth;
	m_stRgb.iHeight = m_iHeight;

	for( int y=0 ; y<m_iHeight ; y++ )
	{
		int iPosY = y*m_iWidth;
		int iPosYY = y*m_iWidth*3;
		for( int x=0 ; x<m_iWidth ; x++ )
		{
			int iPos = x + y*m_iWidth;

			double var_X = m_stXyz.pData[_BHC_COLOR_XYZ_X][iPos]/ 100;
			double var_Y = m_stXyz.pData[_BHC_COLOR_XYZ_Y][iPos]/ 100;
			double var_Z = m_stXyz.pData[_BHC_COLOR_XYZ_Z][iPos]/ 100;

			double var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
			double var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415;
			double var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570;

			if ( var_R > 0.0031308 ) var_R = 1.055 * ( pow(var_R, ( 1 / 2.4 ) ) ) - 0.055;
			else                     var_R = 12.92 * var_R;
			if ( var_G > 0.0031308 ) var_G = 1.055 * ( pow(var_G, ( 1 / 2.4 ) ) ) - 0.055;
			else                     var_G = 12.92 * var_G;
			if ( var_B > 0.0031308 ) var_B = 1.055 * ( pow(var_B, ( 1 / 2.4 ) ) ) - 0.055;
			else                     var_B = 12.92 * var_B;

			double sR = var_R * 255;
			double sG = var_G * 255;
			double sB = var_B * 255;


			// set data
			pRed[x+iPosY] = sR;
			pGreen[x+iPosY] = sG;
			pBlue[x+iPosY] = sB;
		}
	}



	//
	m_stRgb.SetData(pRed, pGreen, pBlue, m_iWidth, m_iHeight);


	return true;
}


bool CColorInfo::GenXyy_fromXyz()
{
	// xyY
	double *pXyy_x = new double[m_iWidth * m_iHeight];
	double *pXyy_y = new double[m_iWidth * m_iHeight];
	double *pXyy_yy = new double[m_iWidth * m_iHeight];


	for( int y=0 ; y<m_iHeight ; y++ )
	{
		for( int x=0 ; x<m_iWidth ; x++ )
		{
			int iPos = x + y*m_iWidth;

			double xx = m_stXyz.pData[_BHC_COLOR_XYZ_X][iPos];
			double yy = m_stXyz.pData[_BHC_COLOR_XYZ_Y][iPos];
			double zz = m_stXyz.pData[_BHC_COLOR_XYZ_Z][iPos];

			double dSum = xx+yy+zz;

			if( dSum != 0 )
			{
				pXyy_x[iPos] = xx/dSum;
				pXyy_y[iPos] = yy/dSum;
				pXyy_yy[iPos] = zz/dSum;
			} else {
				pXyy_x[iPos] = 0;
				pXyy_y[iPos] = 0;
				pXyy_yy[iPos] = 0;
			}
		}
	}


	//
	m_stXyy.SetData(pXyy_x, pXyy_y, pXyy_yy, m_iWidth, m_iHeight);


	delete[] pXyy_x;
	delete[] pXyy_y;
	delete[] pXyy_yy;


	return true;
}


bool CColorInfo::GenLuv_fromXyz()
{

	return true;
}





