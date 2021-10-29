#include "StdAfx.h"
#include "BHC_exel.h"

// 파일 읽기 쓰기
#include <iostream>
#include <fstream>
using namespace std;

// 엑셀 접근하기
#include "../common/XLEzAutomation.h"


CBHC_exel::CBHC_exel(void)
{
	m_pProgess = NULL;

	m_iWidth_data	= 0;
	m_iHeight_data	= 0;
}


CBHC_exel::~CBHC_exel(void)
{
	m_stRadientData.free();
}


BOOL CBHC_exel::ReadRadient( CString strFilePath )
{
	m_stRadientData.iWidth = 4896;
	m_stRadientData.iHeight = 3264/5;

	if( m_stRadientData.pData )
	{
		delete[] m_stRadientData.pData;
	}

	m_stRadientData.pData = new double[m_stRadientData.iWidth * m_stRadientData.iHeight];




	//
	// 너무 오래 걸림
	//
// 	CXLEzAutomation xl(FALSE);
// 
// 	xl.OpenExcelFile(strFilePath);	// open file
// 
// 	int iStartY = 9;
// 	int iStartX = 1;
// 
// 	for( int y=0 ; y<m_stRadientData.iHeight ; y++ )
// 	{
// 		for( int x=0 ; x<m_stRadientData.iWidth - 1 ; x++ )
// 		{
// 			CString strValue = xl.GetCellValue( iStartX + x, iStartY + y );
// 
// 			m_stRadientData.pData[ x + y*m_stRadientData.iWidth ] = _tstof(strValue.GetBuffer());
// 		}
// 	}
// 
// 
// 	xl.ReleaseExcel();



	//
	// 
	//
	SetProgress(0.0);

	ifstream is(strFilePath);
 	char s[100];
// 	wchar_t  s[100];

	is.getline(s, sizeof(s));	// 
	is.getline(s, sizeof(s));	// data
	is.getline(s, sizeof(s));	// data width
	is.getline(s, sizeof(s));	// data height
	is.getline(s, sizeof(s));	// start x
	is.getline(s, sizeof(s));	// end y
	is.getline(s, sizeof(s));	// start y
	is.getline(s, sizeof(s));	// end y

	double dValue;
	for( int y=0 ; y<m_stRadientData.iHeight ; y++ )
	{
		for( int x=0 ; x<m_stRadientData.iWidth - 1 ; x++ )
		{
			is.getline(s, sizeof(s), '\t');

			dValue = atof(s);

			m_stRadientData.pData[ x + y*m_stRadientData.iWidth ] = dValue;
		}

		is.getline(s, sizeof(s));
		dValue = atof(s);

		m_stRadientData.pData[ (m_stRadientData.iWidth - 1) + y*m_stRadientData.iWidth ] = dValue;

		// *
		SetProgress( (double)(y+1)/m_stRadientData.iHeight );
	}


	return TRUE;
}


BOOL CBHC_exel::ReadLumitop( CString strFilePath )
{
	m_stLumitopData.iWidth = 2752;
	m_stLumitopData.iHeight = 2204;

	if( m_stLumitopData.pDataX )
	{
		delete[] m_stLumitopData.pDataX;
	}

	m_stLumitopData.pDataX = new double[m_stLumitopData.iWidth * m_stLumitopData.iHeight];

	if( m_stLumitopData.pDataY )
	{
		delete[] m_stLumitopData.pDataY;
	}

	m_stLumitopData.pDataY = new double[m_stLumitopData.iWidth * m_stLumitopData.iHeight];

	if( m_stLumitopData.pDataZ )
	{
		delete[] m_stLumitopData.pDataZ;
	}

	m_stLumitopData.pDataZ = new double[m_stLumitopData.iWidth * m_stLumitopData.iHeight];


	//
	// 
	//
	SetProgress(0.0);

	ifstream is(strFilePath);
	char s[100];
	// 	wchar_t  s[100];

	is.getline(s, sizeof(s));	// 

	double dValue;

	for( int i=0 ; i<3 ; i++ )
	{
		// set target buffer
		double *pData;
		if( i==0 )
		{
			pData = m_stLumitopData.pDataX;
		}
		else if( i==1 )
		{
			pData = m_stLumitopData.pDataY;
		}
		else if( i==2 )
		{
			pData = m_stLumitopData.pDataZ;
		}


		// read
		is.getline(s, sizeof(s));	// data
		for( int y=0 ; y<m_stLumitopData.iHeight ; y++ )
		{
			for( int x=0 ; x<m_stLumitopData.iWidth - 1 ; x++ )
			{
				is.getline(s, sizeof(s), '\t');

				dValue = atof(s);

				pData[ x + y*m_stLumitopData.iWidth ] = dValue;
			}

			is.getline(s, sizeof(s));
			dValue = atof(s);

			pData[ (m_stLumitopData.iWidth - 1) + y*m_stLumitopData.iWidth ] = dValue;

			// *
			SetProgress( (double)(y+1 + i*m_stLumitopData.iHeight)/(m_stLumitopData.iHeight*3) );
		}
	}


	return TRUE;
}



void CBHC_exel::SetProgress(double dProgess)
{
	if( m_pProgess )
	{
		*m_pProgess = dProgess;
	}
}



//////////////////////////////////////////////////////////////////////////
//
//	Read
//
//	엑셀 파일 읽기
//
//	ㅇ 방법
//		1. 1번째 열의 행 갯수를 확인해서 전체 데이터의 폭(A)으로 결정한다.
//		2. 매 열마다 A개의 데이터를 읽는다.
//			- 첫번째 데이터가 NULL이면 종료한다.
//
//////////////////////////////////////////////////////////////////////////
bool CBHC_exel::Read( CString strFilePath )
{
	int iWidth	= 0;
	int iHeight	= 0;

	m_vecData.clear();	// 

	ifstream is;//(strFilePath);
	char s[4096];

	// read file
	is.open(strFilePath);

	if( is.is_open() == false )
	{
		BHC_MEMO("error > CBHC_exel::Read > file open");
		return false;
	}

	// index 열 읽기 & 가로 갯수 설정
	{
		is.getline(s, sizeof(s), '\n');

		char *pContext;
		char *pStr = strtok_s(s, ",", &pContext);

		// 종료 조건
		if (pStr != NULL)
		{
			std::string str(pStr);
			m_vecData.push_back(str);

			while (true)
			{
				pStr = strtok_s(NULL, ",", &pContext);

				if (pStr == NULL)
				{
					break;
				}

				std::string str(pStr);
				m_vecData.push_back(str);

				iWidth++;
			}

			iWidth++;	// 첫번째 데이터 읽은 것 반영
			iHeight = 1;
		}
	}


	// contents 읽기
	if (iWidth > 0)
	{
		while (true)
		{
			is.getline(s, sizeof(s), ',');

			char *pStr = strtok(s, "\n");

			// 종료 조건
			if (pStr == NULL)
			{
				break;
			}

			//
			std::string str(pStr);
			m_vecData.push_back(str);

			//
			for (int i = 1; i < iWidth-1; i++)
			{
				is.getline(s, sizeof(s), ',');

				std::string str(pStr);
				m_vecData.push_back(str);
			}

			is.getline(s, sizeof(s), '\n');
			std::string str_last(pStr);
			m_vecData.push_back(str_last);

			iHeight++;
		}
	}

/*
	while( true )
	{
		is.getline(s, sizeof(s), ',');
		//is.getline(s, sizeof(s), '\n');

		char *pStr = strtok( s, "\n" );

		// 종료 조건
		if(pStr==NULL)
		{
			break;
		}

		std::string str(pStr);
		m_vecData.push_back(str);
		iWidth++;

		// 다음줄 포함 시
		char *pStr_next = strtok( NULL, "\n" );

		if( pStr_next != NULL )	// 다음 줄의 단어를 포함하는 경우 여기에 들어온다.
		{
			std::string str_next(pStr_next);
			m_vecData.push_back(str_next);

			iWidth = 1;
			iHeight++;
		}
	}

	if( iHeight > 0 ) 
	{
		iHeight++;
	}
*/

	// close file
	is.close();


	// data 갯수 체크
	int iVecCount = m_vecData.size();
	int iDimCount = iWidth * iHeight;

	if( iVecCount != iDimCount )
	{
		BHC_MEMO("error > CBHC_exel::Read > data size");
		return false;
	}


	// data의 크기 업데이트
	m_iWidth_data = iWidth;
	m_iHeight_data = iHeight;


	return true;
}



std::vector<std::string> * CBHC_exel::GetData()
{
	return &m_vecData;
}


int CBHC_exel::GetWidth()
{
	return m_iWidth_data;
}


int CBHC_exel::GetHeight()
{
	return m_iHeight_data;
}
