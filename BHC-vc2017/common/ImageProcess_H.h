// ImageProcess_H.h : ImageProcess_H DLL의 기본 헤더 파일입니다.
//

#pragma once

#include <sstream>
#include <iomanip>

#include "../common/ImageProcess.h"

// #include "halconcpp/HalconCpp.h"
// using namespace HalconCpp;

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


#define _BHC_IP_LEFT_	0
#define _BHC_IP_RIGHT_	1
#define _BHC_IP_TOP_	2
#define _BHC_IP_BOTTOM_	3

#define _BHC_IP_POSITIVE_	1
#define _BHC_IP_NEGATIVE_	-1

#define _BHC_IP_LOCAL_TO_WORLD_	0
#define _BHC_IP_WORLD_TO_LOCAL_	1

#define _BHC_IP_REVIEW_NUM_	1000

#define _BHC_IP_REVIEW_NORMAL_	0
#define _BHC_IP_REVIEW_SPECIAL_	1

struct stDefectInfo
{
	bool bValid;

	double dCx;
	double dCy;

	int iLeft;
	int iRight;
	int iTop;
	int iBottom;


	double dCirculatiry;
	double dLength;
	double dAngle_dgree;
	double dLength1;
	double dLength2;
	double dMinGray;
	double dMaxGray;
	double dLinear;

	char text[2048];

	stDefectInfo()
	{
		bValid = false;

		dCx = 0;
		dCy = 0;

// 		rtRegion = CRect(0, 0, 0, 0);

		iLeft	= 0;
		iRight	= 0;
		iTop	= 0;
		iBottom	= 0;

		dCirculatiry = 0.0;
		dLength = 0.0;
		dAngle_dgree = 0.0;
		dLength1 = 0.0;
		dLength2 = 0.0;
		dMinGray = 0.0;
		dMaxGray = 0.0;
		dLinear = 0.0;
	}
};


#ifndef _STPOINT2D_
#define  _STPOINT2D_

struct stPoint2D
{
	double x;
	double y;

	stPoint2D()
	{
		x = 0;
		y = 0;
	}
};
#endif


#ifdef DLLEXPORT_H
#define DLLTYPE_H __declspec(dllexport)
#else
#define DLLTYPE_H __declspec(dllimport)
#endif

class DLLTYPE_H CImageProcess_H : public CImageProcess
{
public:
	CImageProcess_H(void);
	virtual ~CImageProcess_H(void);

	void Test();
	void initialize( int iWidth, int iHeight, int iChennel );

	void SetShowImage(bool bShow);
	void SetShowRatio( double dRatio );

	//
	// scratch 특징 추출
	//
	bool ExtractFeature( unsigned char *pImage, int iWidth, int iHeight, int iSx, int iSy, int iEx, int iEy );


/*
	//
	// align
	//
	bool SetAlignPoint_world(double dX1, double dY1, double dX2, double dY2);
	bool SetAlignImage_line(BYTE *pImage, int iWidth, int iHeight, int iPos, double dCx, double dCy, int iDirX, int iDirY);
	bool SetAlignImage_line_color(BYTE *pImage, int iWidth, int iHeight, int iPos, double dCx, double dCy, int iDirX, int iDirY, int iSaveIndex);
	bool CalcCoordinateMatrix();
	bool GetCoodinate(double dX_world, double dY_wordl, double *dX_stage, double *dY_stagey, int iDir);
	double GetMatrix(int i);
	*/

	//
	// template matching
	//
	bool LoadTemplate();
	bool FindTemplate( unsigned char *pData, int iWidth, int iHeight, unsigned char *pResult );

/*
	HalconCpp::HShapeModel m_hsTemplate;
	HalconCpp::HImage m_hiTemplateImage;

	HalconCpp::HHomMat2D m_hhmTemplateToModel;	// template image to template model


	//
	// review 검사
	//
	bool SetReviewImage(BYTE *pImage, int iWidth, int iHeight, int iDepth, int iCam, int iSeq );
	bool InspectReview(int iSeq );
	BYTE m_resullt[_BHC_IP_REVIEW_NUM_];


	//
	// LGC 용접검사
	//
	bool FindRoi_batWeld(BYTE *pData, int iWidth, int iHeight, int iThreshold, int iMinSize, int iMaxSize);
	bool Inspect_batWeld(BYTE *pData, int iWidth, int iHeight);
	stInfo_batWeld *GetInfo_batWeld(int *pDataNum);


protected:
	// public:
	//
	// common
	//
	int m_bDisplay;
	double m_dDisplayRatio;

	bool MakeHImage(BYTE *pInputData, int iWidth, int iHeight, HalconCpp::HImage* outImage);
	bool MakeHImage_color(BYTE *pInputData, int iWidth, int iHeight, HalconCpp::HImage* outImage);
	bool MakeHImage_color(BYTE *pInputData, int iWidth, int iHeight, HalconCpp::HImage* hiRed, HalconCpp::HImage* hiGreen, HalconCpp::HImage* hiBlue);

// 	void SAVE_IMAGE(const HImage & image, HRegion & region, CString strFileName);
 	void SAVE_IMAGE(const HalconCpp::HImage & image, HalconCpp::HRegion * pRegion, list<HalconCpp::HXLDCont> *pListHx, list<stDefectInfo> *pListInfo, std::string strFileName);

	// 	void DISPLAY( const HImage & image, HRegion * region, HXLDCont * xld, CString strMsg, double dZoomRatio = 1.0 );
	void DISPLAY(const HalconCpp::HImage & image, HalconCpp::HRegion * region, HalconCpp::HXLDCont * xld, std::list<stDisplsyInfo> *pTextList, std::string strMsg);
	void DISPLAY(const HalconCpp::HImage & image, HalconCpp::HRegion * region, HalconCpp::HXLDCont * xld, std::list<stDisplsyInfo> *pTextList, std::string strMsg, double dZoomRatio );


	bool FindNearestRegionIndex( const HalconCpp::HRegion hrIn, double dX_target, double dY_target, int *pIndex );


	//
	// scratch 특징 추출
	//
	HalconCpp::HImage m_hiOrg;

	bool CalcNonLinearity(HalconCpp::HRegion *pRegion, HalconCpp::HXLDCont *pHxLine, double *pMaxDistance);

	bool CalcStringFeature(HalconCpp::HRegion *pHrTarget, HalconCpp::HRegion *pHrCandiates, HalconCpp::HXLDCont *pHxLine, double *pAngle_mean, double *pAngle_std, double *pAngleDer_mean, double *pAngleDer_std, double *pLength, double *pDist );
	bool CalcStringFeature( HalconCpp::HXLDCont *pHxld, double *pdAngle_mean, double *pdAngleStd, double *pdAngleDer_mean, double *pdAngleDer_std);
	bool CalcStringFeature_nonLinear(HalconCpp::HXLDCont *pHxLine, HalconCpp::HXLDCont *pHxFit, double *pMaxDistance);
	double CalcAngleRalation( double dAngMain, double dAngSub );

	// feature
	double m_string_angMean;
	double m_string_angDev;

	double m_string_devAngMean;
	double m_string_devAngDev;

	double m_string_lentgh;
	double m_string_dist;


	//
	// align
	//
	// 광학 resolution
	double m_dResolution_x;	// mm/pixel
	double m_dResolution_y;	// mm/pixel

	// align 용 기준점의 위치
	double m_dX_AlignKey_local[2];
	double m_dY_AlignKey_local[2];
	double m_dX_AlignKey_world[2];
	double m_dY_AlignKey_world[2];

	// align 용 기준선의 위치
	double m_dX_AlignLine_left_local[2];
	double m_dY_AlignLine_left_local[2];
	double m_dX_AlignLine_top_local[2];
	double m_dY_AlignLine_top_local[2];
	double m_dX_AlignLine_right_local[2];
	double m_dY_AlignLine_right_local[2];
	double m_dX_AlignLine_bottom_local[2];
	double m_dY_AlignLine_bottom_local[2];

	double m_dX_AlignLine_left_world[2];
	double m_dY_AlignLine_left_world[2];
	double m_dX_AlignLine_top_world[2];
	double m_dY_AlignLine_top_world[2];
	double m_dX_AlignLine_right_world[2];
	double m_dY_AlignLine_right_world[2];
	double m_dX_AlignLine_bottom_world[2];
	double m_dY_AlignLine_bottom_world[2];

	// 좌표 변환 matrix
	double m_dAffineMatirx[6];

	HalconCpp::HTuple m_htCoordinateMatrix_worldToLocal;
	HalconCpp::HTuple m_htCoordinateMatrix_localToWorld;

	//
	bool CalcConer(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3, double dX4, double dY4, double *pDx, double *pDy);


	//
	//
	//
	BYTE *m_pReview1[_BHC_IP_REVIEW_NUM_];
	int m_iWidth_review1;
	int m_iHeight_review1;

	BYTE *m_pReview2[_BHC_IP_REVIEW_NUM_];
	int m_iWidth_review2;
	int m_iHeight_review2;



	//
	// LGC 용접검사
	//
	HalconCpp::HRegion m_hrRoi[8];
	stInfo_batWeld m_stBatWeldInfo[8];
	
	bool InspWeldingArea( const HalconCpp::HImage & image, HalconCpp::HRegion & hrRoi, int *piArea, double *pdRadius_outter, double *pdRow_outter, double *pdCol_outter );
	*/
};
