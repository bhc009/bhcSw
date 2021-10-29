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


	void Clear();	// deta ���
	bool SetData( std::vector<std::string> *pData, int iWidth, int iHeight );

	bool SetTargetName( std::string strTarget );	// �ҷ��� ����
	bool SetIndex_target( int iIndex_target, int iIndex_result );	// ��¥/���� �ҷ� type ����


	//
	//
	//
	int GetWidth();	// data ��
	int GetHeight();	// data ����

	// ������
	std::vector<std::string>* GetColumns();
	std::string GetColumns( int i );
	bool GetColumn(std::vector<std::string> *pVecColumn);

	// ����
	std::string GetContent( int x, int y );
	bool SetContent( int x, int y, std::string str );
	bool GetContent( int x,  std::vector<std::string> *pContent );
	bool GetContent(std::vector<std::string> *pContent, int *piWidth, int *piHeight);

	// Score
	bool GetScore( int *piNum_all, int *piNum_defect, int *piNum_miss, int *piNum_over );

	// ���� �ҷ�
	bool SetIndex_interest( std::list<unsigned int> *pIndex );	// ���� �ҷ� üũ
	bool CheckInterest( int iIndex );	// ���� �ҷ� ���� ȹ��
	void Reset();	// ���� �ҷ� üũ ����

	//
	bool SetJudgment(std::list<bool> *pJudgment);

protected:
	int m_iCnt_columns;		// ��
	int m_iCnt_rows;		// ��

	bool m_bSetTarget;
	int m_iIndex_target;			// ��¥ �ҷ� ���� index
	int m_iIndex_result;			// ���� �ҷ� ���� index
	std::string m_strTargetName;	// �ҷ��� �̸�

	std::vector<std::string> m_vecColumns;	// index data
	std::vector<std::string> m_vecContents;	// ���� data
	std::vector<bool> m_vecInterest;		// ���� ������ üũ
};

