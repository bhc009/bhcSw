#pragma once


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


	double *m_pProgess;

	//
	// radiant camera
	//
	BOOL ReadRadient( CString strFilePath );
	stRadient *GetRadientData() {return&m_stRadientData;};

	BOOL ReadLumitop( CString strFilePath );
	stLumitop *GetLumitopData() {return&m_stLumitopData;};


protected:
	void SetProgress(double dProgess);	// 처리현황 공유


	stRadient m_stRadientData; 
	stLumitop m_stLumitopData; 


};

