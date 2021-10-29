#pragma once

#include <vector>


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



// radient 휘도 계측기 데이터 저장용
struct stRadient
{
	int iWidth;
	int iHeight;
	double *pData;

	stRadient()
	{
		iWidth = 0;
		iHeight = 0;
		pData = NULL;
	}

	void free()
	{
		if( pData )
		{
			delete[] pData;
		}
		pData = NULL;
	}
};


// lumitop 휘도 계측기 저장용
struct stLumitop
{
	int iWidth;
	int iHeight;
	double *pDataX;
	double *pDataY;
	double *pDataZ;

	stLumitop()
	{
		iWidth = 0;
		iHeight = 0;
		pDataX = NULL;
		pDataY = NULL;
		pDataZ = NULL;
	}

	void free()
	{
		if( pDataX )
		{
			delete[] pDataX;
		}
		pDataX = NULL;

		if( pDataY )
		{
			delete[] pDataY;
		}
		pDataY = NULL;

		if( pDataZ )
		{
			delete[] pDataZ;
		}
		pDataZ = NULL;
	}
};



class CBHC_exel
{
public:
	CBHC_exel(void);
	~CBHC_exel(void);


	//
	// common
	//

	double *m_pProgess;

protected:
	void SetProgress(double dProgess);	// 처리현황 공유


	//
	// 휘도 계측기
	//

	// radiant camera
public:
	BOOL ReadRadient( CString strFilePath );
	stRadient *GetRadientData() {return&m_stRadientData;};

protected:
	stRadient m_stRadientData; 

	// radiant camera
public:
	BOOL ReadLumitop( CString strFilePath );
	stLumitop *GetLumitopData() {return&m_stLumitopData;};

protected:
	stLumitop m_stLumitopData; 

	//
	//
	//
public:
	bool Read( CString strFilePath );

	std::vector<std::string> *GetData();
	int GetWidth();
	int GetHeight();

protected:
	int m_iWidth_data;
	int m_iHeight_data;
	std::vector<std::string> m_vecData; 





};

