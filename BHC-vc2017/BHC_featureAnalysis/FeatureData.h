#pragma once

#include <vector>
#include <list>


#ifndef _BHC_
#define _BHC_

#ifdef UNICODE
#define BHC_MEMO(logformat, ...)\
			{ \
			char logbuf[2048]; \
			int ret = _snprintf_s(logbuf, 2048, _TRUNCATE, logformat, ##__VA_ARGS__); \
			int len = MultiByteToWideChar( CP_ACP, 0, logbuf, -1, NULL, 0 ); \
			WCHAR wBuf[2048]; \
			MultiByteToWideChar( CP_ACP, 0, logbuf, strlen(logbuf)+1, wBuf, len ); \
			OutputDebugString(wBuf); \
			};
#else
#define BHC_MEMO(logformat, ...)\
			{ \
			char logbuf[2048]; \
			int ret = _snprintf_s(logbuf, 2048, _TRUNCATE, logformat, ##__VA_ARGS__); \
			OutputDebugString(logbuf); \
			};
#endif
#endif


class CFeatureData
{
public:
	CFeatureData(void);
	~CFeatureData(void);


	void Clear();	// deta 비움
	bool SetData( std::vector<std::string> *pData, int iWidth, int iHeight );

	bool SetTargetName( std::string strTarget );	// 불량명 설정
	bool SetIndex_target( int iIndex_target, int iIndex_result );	// 진짜/판정 불량 type 설정


	//
	//
	//
	int GetWidth();	// data 폭
	int GetHeight();	// data 높이

	// 제목줄
	std::vector<std::string>* GetColumns();
	std::string GetColumns( int i );
	bool GetColumn(std::vector<std::string> *pVecColumn);

	// 내용
	std::string GetContent( int x, int y );
	bool SetContent( int x, int y, std::string str );
	bool GetContent( int x,  std::vector<std::string> *pContent );
	bool GetContent(std::vector<std::string> *pContent, int *piWidth, int *piHeight);

	// Score
	bool GetScore( int *piNum_all, int *piNum_defect, int *piNum_miss, int *piNum_over );

	// 관심 불량
	bool SetIndex_interest( std::list<unsigned int> *pIndex );	// 관심 불량 체크
	bool CheckInterest( int iIndex );	// 관심 불량 여부 획득
	void Reset();	// 관심 불량 체크 해제

	//
	bool SetJudgment(std::list<bool> *pJudgment);

protected:
	int m_iCnt_columns;		// 열
	int m_iCnt_rows;		// 행

	bool m_bSetTarget;
	int m_iIndex_target;			// 진짜 불량 열의 index
	int m_iIndex_result;			// 판정 불량 열의 index
	std::string m_strTargetName;	// 불량의 이름

	std::vector<std::string> m_vecColumns;	// index data
	std::vector<std::string> m_vecContents;	// 내용 data
	std::vector<bool> m_vecInterest;		// 관심 데이터 체크
};

