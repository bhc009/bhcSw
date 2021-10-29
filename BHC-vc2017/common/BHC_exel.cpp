#include "StdAfx.h"
#include "BHC_exel.h"

// ���� �б� ����
#include <iostream>
#include <fstream>
using namespace std;

// ���� �����ϱ�
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
	// �ʹ� ���� �ɸ�
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
//	���� ���� �б�
//
//	�� ���
//		1. 1��° ���� �� ������ Ȯ���ؼ� ��ü �������� ��(A)���� �����Ѵ�.
//		2. �� ������ A���� �����͸� �д´�.
//			- ù��° �����Ͱ� NULL�̸� �����Ѵ�.
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

	// index �� �б� & ���� ���� ����
	{
		is.getline(s, sizeof(s), '\n');

		char *pContext;
		char *pStr = strtok_s(s, ",", &pContext);

		// ���� ����
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

			iWidth++;	// ù��° ������ ���� �� �ݿ�
			iHeight = 1;
		}
	}


	// contents �б�
	if (iWidth > 0)
	{
		while (true)
		{
			is.getline(s, sizeof(s), ',');

			char *pStr = strtok(s, "\n");

			// ���� ����
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

		// ���� ����
		if(pStr==NULL)
		{
			break;
		}

		std::string str(pStr);
		m_vecData.push_back(str);
		iWidth++;

		// ������ ���� ��
		char *pStr_next = strtok( NULL, "\n" );

		if( pStr_next != NULL )	// ���� ���� �ܾ �����ϴ� ��� ���⿡ ���´�.
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


	// data ���� üũ
	int iVecCount = m_vecData.size();
	int iDimCount = iWidth * iHeight;

	if( iVecCount != iDimCount )
	{
		BHC_MEMO("error > CBHC_exel::Read > data size");
		return false;
	}


	// data�� ũ�� ������Ʈ
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
