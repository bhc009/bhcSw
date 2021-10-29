#pragma once

// RGB 색구분
#define _BHC_COLOR_RGB_R	0
#define _BHC_COLOR_RGB_G	1
#define _BHC_COLOR_RGB_B	2

// XYZ 색구분
#define _BHC_COLOR_XYZ_X	0
#define _BHC_COLOR_XYZ_Y	1
#define _BHC_COLOR_XYZ_Z	2

// XYY 색구분
#define _BHC_COLOR_XYY_X	0
#define _BHC_COLOR_XYY_Y	1
#define _BHC_COLOR_XYY_YY	2

// LUV 색구분
#define _BHC_COLOR_LUV_L	0
#define _BHC_COLOR_LUV_U	1
#define _BHC_COLOR_LUV_V	2

// RGB depth
#define _BHC_COLOR_RGB_8	8
#define _BHC_COLOR_RGB_16	16


struct stRgb
{
	int iDataType;

	// dimension
	int iWidth;
	int iHeight;

	BYTE *pData;	// 8bit show

	// data
	BYTE *pData_8[3];	// 8 bit
	unsigned short *pData_16[3];	// 16 bit


	stRgb()
	{
		iDataType = _BHC_COLOR_RGB_8;
		
		iWidth	= 0;
		iHeight	= 0;

		// 
		pData	= NULL;

		// 8bit
		pData_8[0] = NULL;
		pData_8[1] = NULL;
		pData_8[2] = NULL;

		// 16 bit
		pData_16[0] = NULL;
		pData_16[1] = NULL;
		pData_16[2] = NULL;
	};


	~stRgb()
	{
		// 8bit
		if( pData )
		{
			delete[] pData;
		}
		pData = NULL;

		for( int i=0 ; i<3 ; i++ )
		{
			// 8bit
			if( pData_8[i] )
			{
				delete[] pData_8[i];
			}
			pData_8[i] = NULL;

			// 16 bit
			if( pData_16[i] )
			{
				delete[] pData_16[i];
			}
			pData_16[i] = NULL;
		}
	};


	bool SetData( BYTE *pData_In, int iWidth_In, int iHeight_In )
	{
		if( iWidth_In < 1 || iHeight_In < 1 )
		{
			return false;
		}


		//
		// 
		//
		// regenerate buffer
		for( int i=0 ; i<3 ; i++ )
		{
			if( pData_8[i] )
			{
				delete[] pData_8[i];
			}
			pData_8[i] = new BYTE[iWidth_In*iHeight_In];
		}

		// set data
		for( int y=0 ; y<iHeight_In ; y++ )
		{
			int iPosY_src = y*iWidth_In*3;
			int iPosY_dst = y*iWidth_In;
			for( int x=0 ; x<iWidth_In ; x++ )
			{
				pData_8[_BHC_COLOR_RGB_R][x + iPosY_dst] = pData_In[ 3*x + iPosY_src + 2 ];
				pData_8[_BHC_COLOR_RGB_G][x + iPosY_dst] = pData_In[ 3*x + iPosY_src + 1 ];
				pData_8[_BHC_COLOR_RGB_B][x + iPosY_dst] = pData_In[ 3*x + iPosY_src ];
			}
		}


		//
		//
		//
		// regenerate buffer
		if( pData )
		{
			delete[] pData;
		}
		pData = new BYTE[3*iWidth_In*iHeight_In];

		// set data
		for( int y=0 ; y<iHeight_In ; y++ )
		{
			int iPosY = y*iWidth_In*3;
			for( int x=0 ; x<iWidth_In ; x++ )
			{
				pData[x + iPosY] = pData_In[ 3*x + iPosY + 2 ];
				pData[x + iPosY] = pData_In[ 3*x + iPosY + 1 ];
				pData[x + iPosY] = pData_In[ 3*x + iPosY ];
			}
		}

		// data type
		iDataType = _BHC_COLOR_RGB_8;

		// data dimension
		iWidth = iWidth_In;
		iHeight = iHeight_In;	


		return true;
	};


	bool SetData( BYTE *pRed_In, BYTE *pGreen_In, BYTE *pBlue_In, int iWidth_In, int iHeight_In )
	{
		if( iWidth_In < 1 || iHeight_In < 1 )
		{
			return false;
		}


		//
		// 
		//
		// regenerate buffer
		for( int i=0 ; i<3 ; i++ )
		{
			if( pData_8[i] )
			{
				delete[] pData_8[i];
			}
			pData_8[i] = new BYTE[iWidth_In*iHeight_In];
		}

		// set data
		memcpy( pData_8[_BHC_COLOR_RGB_R], pRed_In, iWidth_In * iHeight_In );
		memcpy( pData_8[_BHC_COLOR_RGB_G], pGreen_In, iWidth_In * iHeight_In );
		memcpy( pData_8[_BHC_COLOR_RGB_B], pBlue_In, iWidth_In * iHeight_In );
		
		
		//
		// BGR 순서로 저장
		//
		// regenerate buffer
		if( pData )
		{
			delete[] pData;
		}
		pData = new BYTE[3*iWidth_In*iHeight_In];

		// set data
		for( int y=0 ; y<iHeight_In ; y++ )
		{
			int iPosY_src = y*iWidth_In;
			int iPosY_dst = y*iWidth_In*3;
			for( int x=0 ; x<iWidth_In ; x++ )
			{
				pData[3*x + iPosY_dst		] = pBlue_In[	x + iPosY_src ];	// B
				pData[3*x + iPosY_dst + 1	] = pGreen_In[	x + iPosY_src ];	// G	
				pData[3*x + iPosY_dst + 2	] = pRed_In[	x + iPosY_src ];	// R
			}
		}


		// data type
		iDataType = _BHC_COLOR_RGB_8;

		// data dimension
		iWidth = iWidth_In;
		iHeight = iHeight_In;	


		return true;
	};


	bool SetData( unsigned short *pData_In, int iWidth_In, int iHeight_In )
	{
		if( iWidth_In < 1 || iHeight_In < 1 )
		{
			return false;
		}


		//
		// 
		//
		// regenerate buffer
		for( int i=0 ; i<3 ; i++ )
		{
			if( pData_16[i] )
			{
				delete[] pData_16[i];
			}
			pData_16[i] = new unsigned short[iWidth_In*iHeight_In];
		}

		// set data
		for( int y=0 ; y<iHeight_In ; y++ )
		{
			int iPosY_src = y*iWidth_In*3;
			int iPosY_dst = y*iWidth_In;
			for( int x=0 ; x<iWidth_In ; x++ )
			{
				pData_16[_BHC_COLOR_RGB_R][x + iPosY_dst] = pData_In[ 3*x + iPosY_src + 2 ];
				pData_16[_BHC_COLOR_RGB_G][x + iPosY_dst] = pData_In[ 3*x + iPosY_src + 1 ];
				pData_16[_BHC_COLOR_RGB_B][x + iPosY_dst] = pData_In[ 3*x + iPosY_src ];
			}
		}


		//
		//
		//
		// regenerate buffer
		if( pData )
		{
			delete[] pData;
		}
		pData = new BYTE[3*iWidth_In*iHeight_In];

		// set data
		for( int y=0 ; y<iHeight_In ; y++ )
		{
			int iPosY = y*iWidth_In*3;
			for( int x=0 ; x<iWidth_In ; x++ )
			{
// 				if( x==3390 && y==2172 )
// 				{
// 					int a = 0;
// 				}
				pData[3*x + iPosY		] = min( 255, max( 0, pData_In[ 3*x + iPosY		]>>4 ) );
				pData[3*x + iPosY + 1	] = min( 255, max( 0, pData_In[ 3*x + iPosY + 1	]>>4 ) );
				pData[3*x + iPosY + 2	] = min( 255, max( 0, pData_In[ 3*x + iPosY + 2	]>>4 ) );
			}
		}



		// data type
		iDataType = _BHC_COLOR_RGB_16;

		// data dimension
		iWidth = iWidth_In;
		iHeight = iHeight_In;	


		return true;
	};


	bool SetData( unsigned short *pRed_In, unsigned short *pGreen_In, unsigned short *pBlue_In, int iWidth_In, int iHeight_In )
	{
		if( iWidth_In < 1 || iHeight_In < 1 )
		{
			return false;
		}


		//
		// 
		//
		// regenerate buffer
		for( int i=0 ; i<3 ; i++ )
		{
			if( pData_16[i] )
			{
				delete[] pData_16[i];
			}
			pData_16[i] = new unsigned short[iWidth_In*iHeight_In];
		}

		// set data
		memcpy( pData_16[_BHC_COLOR_RGB_R], pRed_In,	iWidth_In * iHeight_In );
		memcpy( pData_16[_BHC_COLOR_RGB_G], pGreen_In,	iWidth_In * iHeight_In );
		memcpy( pData_16[_BHC_COLOR_RGB_B], pBlue_In,	iWidth_In * iHeight_In );


		//
		// BGR 순서로 저장
		//
		// regenerate buffer
		if( pData )
		{
			delete[] pData;
		}
		pData = new BYTE[3*iWidth_In*iHeight_In];

		// set data
		for( int y=0 ; y<iHeight_In ; y++ )
		{
			int iPosY_src = y*iWidth_In;
			int iPosY_dst = y*iWidth_In*3;
			for( int x=0 ; x<iWidth_In ; x++ )
			{
				pData[3*x + iPosY_dst		] = pBlue_In[	x + iPosY_src ]>>4;	// B
				pData[3*x + iPosY_dst + 1	] = pGreen_In[	x + iPosY_src ]>>4;	// G	
				pData[3*x + iPosY_dst + 2	] = pRed_In[	x + iPosY_src ]>>4;	// R
			}
		}


		// data type
		iDataType = _BHC_COLOR_RGB_16;

		// data dimension
		iWidth = iWidth_In;
		iHeight = iHeight_In;	


		return true;
	};
};


struct stColor
{
	double *pData[3];

	int iWidth;
	int iHeight;

	stColor()
	{
		pData[0] = NULL;
		pData[1] = NULL;
		pData[2] = NULL;
	};

	~stColor()
	{
		for( int i=0 ; i<3 ; i++ )
		{
			if( pData[i] )
			{
				delete[] pData[i];
			}
			pData[i] = NULL;
		}
	};

	bool stColor::SetData( double *p1, double *p2, double *p3, int _iWidth, int _iHeight)
	{
		if( _iWidth<1 || _iHeight<1 )
		{
			return false;
		}

		iWidth = _iWidth;
		iHeight = _iHeight;

		// 1
		if( pData[0] )
		{
			delete pData[0];
		}
		pData[0] = new double[iWidth*iHeight];
		memcpy( pData[0], p1, sizeof(double)*iWidth*iHeight );

		// 2
		if( pData[1] )
		{
			delete pData[1];
		}
		pData[1] = new double[iWidth*iHeight];
		memcpy( pData[1], p2, sizeof(double)*iWidth*iHeight );

		// 3
		if( pData[2] )
		{
			delete pData[2];
		}
		pData[2] = new double[iWidth*iHeight];
		memcpy( pData[2], p3, sizeof(double)*iWidth*iHeight );

		return true;
	};

	bool stColor::SetData( double *_pData, int _iWidth, int _iHeight)
	{
		if( _iWidth<1 || _iHeight<1 )
		{
			return false;
		}

		iWidth = _iWidth;
		iHeight = _iHeight;

		// 
		for( int i=0 ; i<3 ; i++ )
		{
			if( pData[i] )
			{
				delete pData[i];
			}
			pData[i] = new double[iWidth*iHeight];
		}
		

		for( int y=0 ; y<iHeight ; y++ )
		{
			int iPosY = y*iWidth;
			int _iPosY = y*iWidth*3;
			for( int x=0 ; x<iWidth ; x++ )
			{
				pData[0][x + iPosY] = _pData[ 3*x + _iPosY ];
				pData[1][x + iPosY] = _pData[ 3*x + _iPosY + 1];
				pData[2][x + iPosY] = _pData[ 3*x + _iPosY + 2];
			}
		}

		return true;
	};
};


class CColorInfo
{
public:
	CColorInfo(void);
	~CColorInfo(void);

	bool IsValid();

	bool GetSize( int *pWidth, int *pHeight );

	bool SetRgb( BYTE *pData, int iWidth, int iHeight );
	bool SetRgb( BYTE *pRed, BYTE *pGreen, BYTE *pBlue, int iWidth, int iHeight );

	bool SetRgb( unsigned short *pData, int iWidth, int iHeight );
	bool SetRgb( unsigned short *pRed, unsigned short *pGreen, unsigned short *pBlue, int iWidth, int iHeight );

	bool SetXYZ( double *pData, int iWidth, int iHeight );
	bool SetXYZ( double *pX, double *pY, double *pZ, int iWidth, int iHeight );

	stRgb* GetRgb();
	stColor* GetXyz();
	stColor* GetXyy();
	stColor* GetLuv();


protected:
	stRgb m_stRgb;

	stColor m_stXyz;	// XYZ
	stColor m_stXyy;	// XYY
	stColor m_stLuv;	// LUV

	int m_iWidth;
	int m_iHeight;


	bool GenXyz_fromRgb();
	bool GenXyy_fromRgb();
	bool GenLuv_fromRgb();
	bool GenXyz_fromRgb8();
	bool GenXyy_fromRgb8();
	bool GenLuv_fromRgb8();
	bool GenXyz_fromRgb16();
	bool GenXyy_fromRgb16();
	bool GenLuv_fromRgb16();

	bool GenRgb_fromXyz();
	bool GenXyy_fromXyz();
	bool GenLuv_fromXyz();
};

