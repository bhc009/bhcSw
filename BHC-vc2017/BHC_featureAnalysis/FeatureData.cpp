#include "FeatureData.h"
#include "FeatureData.h"
#include "StdAfx.h"


CFeatureData::CFeatureData(void)
{
	m_iCnt_columns	= 0;
	m_iCnt_rows		= 0;

	m_bSetTarget = true;
	m_iIndex_target	= 0;
	m_iIndex_result	= 0;

	m_strTargetName = "Dent";
}


CFeatureData::~CFeatureData(void)
{
}



//////////////////////////////////////////////////////////////////////////
//
//	SetData
//
//	데이터 셋업
//
//	std::vector<std::string> *pData, 
//	int iWidth, 
//	int iHeight : column 열 포함 길이
//
//////////////////////////////////////////////////////////////////////////
bool CFeatureData::SetData( std::vector<std::string> *pData, int iWidth, int iHeight )
{
	if( pData->empty() || iWidth<1 || iHeight<1 )
	{
		return false;
	}


	//
	Clear();


	// set index
	for( int i=0 ; i<iWidth ; i++ )
	{
		m_vecColumns.push_back( (*pData)[i] );
	}


	// set contents
	for( int y=1 ; y<iHeight ; y++ )
	{
		for( int x=0 ; x<iWidth ; x++ )
		{
			std::string str = (*pData)[x + y*iWidth];
			m_vecContents.push_back( str );
		}
	}


	// 
	for( int y=1 ; y<iHeight ; y++ )
	{
		m_vecInterest.push_back(true);
	}


	// set size
	m_iCnt_columns	= iWidth;
	m_iCnt_rows		= iHeight - 1;



	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	SetTarget
//
//	대상 불량명 설정
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CFeatureData::SetTargetName( std::string strTarget )
{
	m_strTargetName = strTarget;

	return true;
}


bool CFeatureData::SetIndex_target( int iIndex_target, int iIndex_result )
{
	m_iIndex_target	= iIndex_target;
	m_iIndex_result	= iIndex_result;

	m_bSetTarget = true;


	return true;
}


bool CFeatureData::SetIndex_interest( std::list<unsigned int> *pIndex )
{
	// clear 
	for( std::vector<bool>::iterator ite = m_vecInterest.begin() ; ite != m_vecInterest.end() ; ite++ )
	{
		*ite = false;
	}


	// set
	for( std::list<unsigned int>::iterator ite = pIndex->begin() ; ite != pIndex->end() ; ite++ )
	{
		int iIndex = *ite;

		if( iIndex < m_iCnt_rows)
		{
			m_vecInterest[iIndex] = true;
		}
	}


	return true;
}


void CFeatureData::Clear()
{
	m_iCnt_columns	= 0;
	m_iCnt_rows		= 0;

	m_vecColumns.clear();
	m_vecContents.clear();
	m_vecInterest.clear();

	m_bSetTarget = false;
}


void CFeatureData::Reset()
{
	for( std::vector<bool>::iterator ite = m_vecInterest.begin() ; ite != m_vecInterest.end() ; ite++ )
	{
		*ite = true;
	}
}

int CFeatureData::GetWidth()
{
	return m_iCnt_columns;
}


int CFeatureData::GetHeight()
{
	return m_iCnt_rows;
}



//////////////////////////////////////////////////////////////////////////
//
//	SetJudgment
//
//	불량 판정 결과 업데이트
//
//
//////////////////////////////////////////////////////////////////////////
bool CFeatureData::SetJudgment(std::list<bool> *pJudgment)
{
	if( pJudgment->size() != GetHeight() )
	{
		return false;
	}


	int iRow = 0;	// 행 정보
	int iIndexJugde = m_iIndex_result;	// 열 정보


	for( std::list<bool>::iterator ite = pJudgment->begin() ; ite != pJudgment->end() ; ite++, iRow++ )
	{
		if( *ite == true )
		{
			std::string str = m_strTargetName;
			SetContent( iIndexJugde, iRow, str );
		} else {
			std::string strPre("Not ");
			strPre += m_strTargetName;
			SetContent( iIndexJugde, iRow, strPre );
		}
	}


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	GetContent
//	
//	ㅇ 내용
//		content에서 (x, y)의 데이터를 로딩한다.
//
//////////////////////////////////////////////////////////////////////////
std::string CFeatureData::GetContent( int x, int y )
{
	int iPosY = y * m_iCnt_columns;

	return m_vecContents[ x + iPosY ];
}


//////////////////////////////////////////////////////////////////////////
//
//	SetContent
//	
//	ㅇ 내용
//		content의 x,y에 데이터를 입력한다.
//
//////////////////////////////////////////////////////////////////////////
bool CFeatureData::SetContent( int x, int y, std::string str )
{
	if( x >= GetWidth() || y >=GetHeight() )
	{
// 		BHC_MEMO("CFeatureData::SetContent > error");

		return false;
	}


	int iPosY = y * m_iCnt_columns;

	m_vecContents[ x + iPosY ] = str;


	return true;

}


/////////////////////////////////////////////////////////////////////
//
//	GetContent
//
//	ㅇ 내용
//		"x"번째 column에 해당하는 data를 얻는다.
//
//
/////////////////////////////////////////////////////////////////////
bool CFeatureData::GetContent( int x,  std::vector<std::string> *pContent )
{
	if ( x >= GetWidth() || pContent == NULL )
	{
		// BHC_MEMO("CFeatureData::SetContent > error");

		return false;
	}

	pContent->clear();

	for( int y=0 ; y<m_iCnt_rows ; y++ )
	{
		int iPosY = y * m_iCnt_columns;

		pContent->push_back( m_vecContents[ x + iPosY ] );
	}


	return true;
}


/////////////////////////////////////////////////////////////////////
//
//	GetContent
//
//	ㅇ 내용
//		내용에 해당하는 data를 얻는다.
//
//
/////////////////////////////////////////////////////////////////////
bool CFeatureData::GetContent(std::vector<std::string> *pContent, int *piWidth, int *piHeight)
{
	if (m_vecContents.size() != GetWidth() * GetHeight() )
	{
		return false;
	}

	*piWidth	= GetWidth();	// 데이터 폭
	*piHeight	= GetHeight();	// 데이터 높이
	*pContent	= m_vecContents;// 데이터
}


/////////////////////////////////////////////////////////////////////
//
//	GetColumns
//
//	ㅇ 내용
//		Column data를 얻는다.
//
//
/////////////////////////////////////////////////////////////////////
std::vector<std::string>* CFeatureData::GetColumns()
{
	return &m_vecColumns;
}


/////////////////////////////////////////////////////////////////////
//
//	GetColumn
//
//	ㅇ 내용
//		Column data를 얻는다.
//
//
/////////////////////////////////////////////////////////////////////
bool CFeatureData::GetColumn(std::vector<std::string> *pVecColumn)
{
	if (m_vecColumns.size() != GetWidth() )
	{
		BHC_MEMO("CFeatureData::GetColumn > eror : %d, %d\n", m_vecColumns.size(), GetWidth());

		return false;
	}


	*pVecColumn = m_vecColumns;

	return true;
}


/////////////////////////////////////////////////////////////////////
//
//	GetColumns
//
//	ㅇ 내용
//		i번째 Column data를 얻는다.
//
//
/////////////////////////////////////////////////////////////////////
std::string CFeatureData::GetColumns( int i )
{
	return m_vecColumns[i];
}


/////////////////////////////////////////////////////////////////////
//
//	GetScore
//
//	ㅇ 내용
//		검출력 데이터를 구한다.
//
//
/////////////////////////////////////////////////////////////////////
bool CFeatureData::GetScore( int *piNum_all, int *piNum_defect, int *piNum_miss, int *piNum_over )
{
	if( m_bSetTarget == false )
	{
		return false;
	}

// 	std::string str("Dent");
	std::string str = m_strTargetName;

	int iAll	= 0;
	int iDefect = 0;
	int iMiss	= 0;
	int iOver	= 0;

	int iTarget = m_iIndex_target;
	int iResult = m_iIndex_result;

	for( int y=0 ; y<m_iCnt_rows ; y++ )
	{
		int iPosY = y * m_iCnt_columns;

		std::string strTarget = m_vecContents[iTarget + iPosY];
		std::string strResult = m_vecContents[iResult + iPosY];

		bool bMatch_target = !str.compare(strTarget);
		bool bMatch_result = !str.compare(strResult);

		// 불량
		if( bMatch_target && bMatch_result )
		{
			iDefect++;
		}

		// 미검
		if( bMatch_target && !bMatch_result )
		{
			iMiss++;
		}

		// 과검
		if( !bMatch_target && bMatch_result )
		{
			iOver++;
		}

		if( !bMatch_target && !bMatch_result )
		{
			// nothing
		}

		iAll++;
	}


	*piNum_all		= iAll;
	*piNum_defect	= iDefect;
	*piNum_miss		= iMiss;
	*piNum_over		= iOver;

	return true;
}


bool CFeatureData::CheckInterest( int iIndex )
{
	if( iIndex >= m_vecInterest.size() )
	{
		return false;
	}

	return m_vecInterest[iIndex];
}
