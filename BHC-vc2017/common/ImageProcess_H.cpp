// ImageProcess_H.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//
#define DLLEXPORT_H

#include "ImageProcess_H.h"

#include "halconcpp/HalconCpp.h"
using namespace HalconCpp;

#include <atlconv.h>

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #endif

HalconCpp::HShapeModel m_hsTemplate;
HalconCpp::HImage m_hiTemplateImage;

// HalconCpp::HHomMat2D m_hhmTemplateToModel;	// template image to template model


CImageProcess_H::CImageProcess_H(void)
{/*
	m_dResolution_x = 0.001725;	//	mm/pixel
	m_dResolution_y = 0.001725;	//	mm/pixel

	//
	// align
	//
	for (int i = 0; i < 6; i++)
	{
		m_dAffineMatirx[i] = 0.0;
	}


	m_bDisplay = false;;
	m_dDisplayRatio = 0.5;



	for( int i=0 ; i<_BHC_IP_REVIEW_NUM_ ; i++ )
	{
		m_pReview1[i] = NULL;
		m_pReview2[i] = NULL;
	}*/
}


CImageProcess_H::~CImageProcess_H()
{/*
	for( int i=0 ; i<_BHC_IP_REVIEW_NUM_ ; i++ )
	{
		if( m_pReview1[i] )
		{
			delete[] m_pReview1[i];
		}
		m_pReview1[i] = NULL;

		if( m_pReview2[i] )
		{
			delete[] m_pReview2[i];
		}
		m_pReview2[i] = NULL;


		m_resullt[i] = false;
	}*/
}



void CImageProcess_H::initialize( int iWidth, int iHeight, int iChennel )
{
	ResetObjDb(iWidth, iHeight, iChennel);
}



void CImageProcess_H::Test()
{
	HWindow w(300, 300, 300, 300);

	HString str("Hello halcon~\nclick");

	Hlong row = 10;
	Hlong col = 10;
	HString CoordSystem = "window";
	HString Color = "red";

	w.SetColor("green");
	w.SetTposition(10, 10);
	w.WriteString("zzz");

	w.Click();

	w.CloseWindow();
}


void CImageProcess_H::SetShowImage(bool bShow)
{
// 	m_bDisplay = bShow;
}


void CImageProcess_H::SetShowRatio( double dRatio )
{
// 	m_dDisplayRatio = dRatio;
}



//////////////////////////////////////////////////////////////////////////
//
//	ExtractFeature
//
//	Feature 추출하기
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::ExtractFeature( unsigned char *pImage, int iWidth, int iHeight, int iSx, int iSy, int iEx, int iEy )
{
	if( pImage == NULL || iWidth<1 || iHeight<1)
	{
		return false;
	}
/*

	//
	// params
	//
	int iThreshold_light	= 10;
	int iThreshold_dark		= 20;
	int iMinSize			= 4;


	//
	//
	//

	// gen halcon image
	HImage hiIn;
	MakeHImage( pImage, iWidth, iHeight, &hiIn );

	// gen ROI
	HRegion hrInterst;
	hrInterst.GenRectangle1(iSy, iSx, iEy, iEx);

	// crop image
	HImage hiIn_crop = hiIn.CropRectangle1(iSy-10, iSx-10, iEy+10, iEx+10);
	Hlong hlWidth_crop, hlHeight_crop;
	hiIn_crop.GetImageSize(&hlWidth_crop, &hlHeight_crop);


	// *
	m_hiOrg = hiIn_crop;

	// precessing
	HRegion hrLignts = hiIn_crop.FastThreshold(128 + iThreshold_light, 255, iMinSize).Connection();	// light defects
	HRegion hrDarks = hiIn_crop.FastThreshold(0, 128 - iThreshold_dark, iMinSize).Connection();		// dark defects

	// select center defect
	double dCx = hlWidth_crop/2;
	double dCy = hlHeight_crop/2;

	int iLightIndex;
	FindNearestRegionIndex(hrLignts, dCx, dCy, &iLightIndex);

	int iDarkIndex;
	if( FindNearestRegionIndex(hrDarks, dCx, dCy, &iDarkIndex) == false )
	{
		return false;
	}
	HRegion hrDark = hrDarks.SelectObj(iDarkIndex);

	DISPLAY(hiIn_crop, &hrDark, NULL, NULL, "ExtractFeature > dark region", 5);


	//
	//
	//
	HXLDCont hxl;
	if( CalcStringFeature(	&hrDark, 
							&hrDarks, 
							&hxl, 
							&m_string_angMean, 
							&m_string_angDev, 
							&m_string_devAngMean, 
							&m_string_devAngDev,
							&m_string_lentgh,
							&m_string_dist) == false )
	{
		return false;
	}

// 	DISPLAY(hiIn_crop, NULL, &hxl, NULL, "ExtractFeature > dark region > string feature", 5);




	//
	//
	//
	char text[2048];
	int ret = _snprintf_s(text, 2048, _TRUNCATE, "L=%.1f A(%.1f, %.1f), DA(%.1f,%.1f), D=%.1f", m_string_lentgh, m_string_angMean*180/3.14, m_string_angDev*180/3.14, m_string_devAngMean*180/3.14 , m_string_devAngDev*180/3.14, m_string_dist );
	DISPLAY(hiIn_crop, NULL, &hxl, NULL, text, 5);

	DWORD dwTime = GetTickCount();
	char filePath[2048];
	_snprintf_s(filePath, 2048, _TRUNCATE, "d:\\test\\%d_sc", dwTime );

	list<HXLDCont> listLine;
	listLine.push_back(hxl);

	list<stDefectInfo> listInfo;

	stDefectInfo stInfo;
	stInfo.dCx = 1;
	stInfo.iBottom = 1;
	_snprintf_s(stInfo.text, 2048, _TRUNCATE, "L=%.1f", m_string_lentgh );
	listInfo.push_back(stInfo);


	stInfo.dCx = 1;
	stInfo.iBottom = 6;
	_snprintf_s(stInfo.text, 2048, _TRUNCATE, "A(%.1f, %.1f)", m_string_angMean*180/3.14, m_string_angDev*180/3.14);
	listInfo.push_back(stInfo);


	stInfo.dCx = 1;
	stInfo.iBottom = 11;
	_snprintf_s(stInfo.text, 2048, _TRUNCATE, "DA(%.1f,%.1f)", m_string_devAngMean*180/3.14 , m_string_devAngDev*180/3.14 );
	listInfo.push_back(stInfo);


	stInfo.dCx = 1;
	stInfo.iBottom = 16;
	_snprintf_s(stInfo.text, 2048, _TRUNCATE, "D=%.1f", m_string_dist );
	listInfo.push_back(stInfo);


	SAVE_IMAGE(hiIn_crop, NULL, &listLine, &listInfo, filePath);
	*/
	return true;
}
/*


// void CImageProcess_H::DISPLAY( const HImage & image, HXLDCont & xld, CString strMsg )
// void CImageProcess_H::DISPLAY( const HImage & image, HRegion * region, HXLDCont * xld, CString strMsg, double dZoomRatio )
// {
// 	if( m_bDisplay == false )
// 	{
// 		return;
// 	}
// 
// 
// 	HTuple imageWidth, imageHeight;
// 	image.GetImageSize( &imageWidth, & imageHeight );
// 
// 	HWindow w(0,0,imageWidth,imageHeight);
// 
// 	Hlong	winWidth = imageWidth;
// 	Hlong	winHeight = imageHeight;
// 	w.SetPart(0,0,winHeight, winWidth);
// 
// 	w.SetWindowExtents(	0,
// 		0,
// 		winWidth * dZoomRatio,
// 		winHeight * dZoomRatio);
// 
// 	w.SetColored(6);
// // 	w.SetDraw("margin");
// 
// 
// 	w.DispImage(image);
// 
// 	if( xld )
// 	{
// 		w.DispXld(*xld);
// 	}
// 
// 	if( region )
// 	{
// 		w.DispRegion(*region);
// 	}
// 
// 
// 	w.SetColor("green");
// 	w.SetTposition(10,10);
// 	w.WriteString((CStringA)strMsg);
// 
// 	w.Click();
// }

void CImageProcess_H::DISPLAY(const HImage & image, HRegion * region, HXLDCont * xld, std::list<stDisplsyInfo> *pTextList, std::string strMsg )
{
	DISPLAY( image, region, xld, pTextList, strMsg, m_dDisplayRatio);
}


void CImageProcess_H::DISPLAY(const HImage & image, HRegion * region, HXLDCont * xld, std::list<stDisplsyInfo> *pTextList, std::string strMsg, double dZoomRatio)
{
	if (m_bDisplay == false)
	{
		return;
	}


	HTuple imageWidth, imageHeight;
	image.GetImageSize(&imageWidth, &imageHeight);

	HWindow w(0, 0, imageWidth, imageHeight);

	Hlong	winWidth = imageWidth;
	Hlong	winHeight = imageHeight;
	w.SetPart(0, 0, winHeight, winWidth);

	w.SetWindowExtents(	0,
						0,
						Hlong(winWidth * dZoomRatio),
						Hlong(winHeight * dZoomRatio));

	w.SetColored(6);
	// 	w.SetDraw("margin");


	w.DispImage(image);

	if (xld)
	{
		w.DispXld(*xld);
	}

	if (region)
	{
		w.DispRegion(*region);
	}


	if (pTextList)
	{
		for (std::list<stDisplsyInfo>::iterator ite = pTextList->begin(); ite != pTextList->end(); ite++)
		{
			if (ite->bOk)
			{
				w.SetColor("green");
			}
			else {
				w.SetColor("red");
			}
			w.SetTposition(ite->y, ite->x);
			w.WriteString(ite->str.c_str());
		}
	}


	w.SetColor("green");
	w.SetTposition(10, 10);
	w.WriteString(strMsg.c_str());


	w.Click();
}


// bool CImageProcess_H::Inspection(BYTE ** ppImageData, int iWidth, int iHeight, list<stDefectInfo> *pDefectList)
// {
// 	int nImages = 8;
// 
// 
// 	//
// 	// Make gray images
// 	//
// 	HImage hImage[8];
// 
// 	for (int i = 0; i < nImages; i++)
// 	{
// 		// make halcon image
// 		HImage hImg_color;
// 		MakeHImage_color(ppImageData[i], iWidth, iHeight, &hImg_color);
// 
// 
// 		// convert to gray
// 		hImage[i] = hImg_color.Rgb1ToGray();
// 
// 		// save image
// // 		CString strFileName;
// // 		strFileName.Format(_T("d:\\test\\insp_01_gray_%d"), i);
// // 		hImage[i].WriteImage("bmp", 0, (CStringA)strFileName );
// 	}
// 
// 
// 
// 	//
// 	// threshold
// 	//
// 	HRegion hrThreshold[8];
// 	for (int i = 0; i < nImages; i++)
// 	{
// 		// 		hrThreshold[i] = hImage[i].Threshold(60, 255).ClosingCircle(3.5);
// 		hrThreshold[i] = hImage[i].Threshold(60, 255).DilationCircle(3.5);
// 	}
// 
// 
// 
// 	//
// 	// connection
// 	//
// 	HRegion hrThreshold_connect[8];
// 	for (int i = 0; i < nImages; i++)
// 	{
// 		hrThreshold_connect[i] = hrThreshold[i].Connection();
// 
// 
// 		// save image
// // 		CString strFileName;
// // 		strFileName.Format(_T("d:\\test\\insp_02_candidate_%d"), i);
// // 		SAVE_IMAGE( hImage[i], hrThreshold_connect[i], strFileName );
// 	}
// 
// 
// 	//
// 	// filtering
// 	//
// 	HRegion hrThreshold_connect_filter[8];
// 
// 	// shape 설정
// 	HTuple paramShape, paramMin, paramMax;
// 
// 	paramShape[0] = "area";
// 	paramMin[0] = 3;
// 	paramMax[0] = 2000;
// 
// 	for (int i = 0; i < nImages; i++)
// 	{
// 		hrThreshold_connect_filter[i] = hrThreshold_connect[i].SelectShape(paramShape, "and", paramMin, paramMax);
// 
// 		// save image
// 		CString strFileName;
// 		strFileName.Format(_T("d:\\test\\insp_03_candidate_%d"), i);
// 		SAVE_IMAGE(hImage[i], hrThreshold_connect[i], strFileName);
// 	}
// 
// 
// 
// 	//
// 	// merge & connection
// 	//
// 	HRegion hrThreshold_connect_filter_merge = hrThreshold_connect_filter[0];
// 
// 	for (int i = 1; i < nImages; i++)
// 	{
// 		hrThreshold_connect_filter_merge = hrThreshold_connect_filter_merge.Union2(hrThreshold_connect_filter[i]);
// 	}
// 
// 	HRegion hrCandidates;
// 	hrCandidates = hrThreshold_connect_filter_merge.Union1().Connection();
// 
// 
// 	SAVE_IMAGE(hImage[0], hrCandidates, _T("d:\\test\\insp_05_candidate"));
// 
// 
// 	//
// 	// get information
// 	//
// 	HRegion hrValidDefect;
// 	list<HXLDCont> hxList;
// 	hrValidDefect.GenEmptyObj();
// 
// 	for (int i = 0; i < hrCandidates.CountObj(); i++)
// 	{
// 		HRegion hrDefect = hrCandidates.SelectObj(i + 1);
// 
// 		// center
// 		HTuple htCx, htCy;
// 		hrDefect.AreaCenter(&htCy, &htCx);
// 
// 		// mbr 1
// 		HTuple htLeft_mbr1, htTop_mbr1, htRight_mbr1, htBottom_mbr1;
// 		hrDefect.SmallestRectangle1(&htTop_mbr1, &htLeft_mbr1, &htBottom_mbr1, &htRight_mbr1);
// 
// 
// 		// mbr 2
// 		HTuple htX_mbr2, htY_mbr2, htLen1_mbr2, htLen2_mbr2, htPhi_mbr2;
// 		hrDefect.SmallestRectangle2(&htY_mbr2, &htX_mbr2, &htPhi_mbr2, &htLen1_mbr2, &htLen2_mbr2);
// 
// 
// 		//
// 		HTuple htCircularity = hrDefect.Circularity();
// 
// 		// length
// 
// 
// 		//
// 		HXLDCont hxCurve;
// 		double dMaxDistance = 0.0;
// 		if (CalcNonLinearity(&hrDefect, &hxCurve, &dMaxDistance))
// 		{
// 			hxList.push_back(hxCurve);
// 		}
// 
// 
// 		//
// 		stDefectInfo defectInfo;
// 
// 		defectInfo.bValid = false;
// 
// 		defectInfo.dCx = htCx;
// 		defectInfo.dCy = htCy;
// 
// 		defectInfo.rtRegion = CRect(htLeft_mbr1, htTop_mbr1, htRight_mbr1, htBottom_mbr1);
// 
// 		defectInfo.dAngle_dgree = htPhi_mbr2 * 180.0 / PI;
// 		defectInfo.dLength1 = htLen1_mbr2;
// 		defectInfo.dLength2 = htLen2_mbr2;
// 
// 		defectInfo.dCirculatiry = htCircularity;
// 
// 		if (defectInfo.dCirculatiry <= 0.3)
// 		{
// 			defectInfo.bValid = true;
// 		}
// 
// 
// 		defectInfo.dLinear = dMaxDistance;	// 선형성
// 
// 
// 		if (defectInfo.bValid)
// 		{
// 			pDefectList->push_back(defectInfo);
// 			hrValidDefect = hrValidDefect.Union2(hrDefect);
// 		}
// 	}
// 
// 
// 	// 	SAVE_IMAGE( hImage[0], hrCandidates, *pDefectList, _T("d:\\test\\insp_06_candidate_info") );
// 	SAVE_IMAGE(hImage[0], &hrValidDefect, &hxList, pDefectList, _T("d:\\test\\insp_06_candidate_info"));
// 
// 
// 	return true;
// }



//////////////////////////////////////////////////////////////////////////
//
// CalcFeature_sc
//
//	- scratch 정도 검사
//
//////////////////////////////////////////////////////////////////////////
// bool CImageProcess_H::CalcFeature_sc(BYTE *pImageData, int iWidth, int iHeight, CRect rtRoi, double *pDValue)
// {
// 	CString strFileName;
// 	ostringstream strStream;
// 	// 	strStream<<"aaa"<<setw(3)<<setfill(0)<<1;
// 
// 		//
// 		// Convert image type : byte -> himage
// 		//
// 	HImage hiImage;
// 
// 	hiImage.GenImage1("byte",			// data type
// 		iWidth,			// image width
// 		iHeight,		// image height
// 		pImageData);	// data pointer
// 
// // save image
// 	strFileName.Format(_T("d:\\test\\CalcFeature_sc_01_org"));
// 	hiImage.WriteImage("bmp", 0, (CStringA)strFileName);
// 
// 
// 	//
// 	// crop image
// 	//
// 	HImage hiImage_crop = hiImage.CropPart(rtRoi.top, rtRoi.left, rtRoi.Width(), rtRoi.Height());
// 
// 	strFileName.Format(_T("d:\\test\\CalcFeature_sc_02_crop"));
// 
// 
// 	//
// 	// threshold
// 	//
// 	HRegion hrThreshold = hiImage_crop.Threshold(127, 255);
// 
// 	SAVE_IMAGE(hiImage_crop, &hrThreshold, NULL, NULL, _T("d:\\test\\CalcFeature_sc_03_threshold"));
// 
// 
// 	//
// 	// select one 
// 	//
// 	HRegion hrCandidates = hrThreshold.Connection();
// 
// 	int iNumCandidates = hrCandidates.CountObj();
// 
// 	if (iNumCandidates == 0)
// 	{
// 		*pDValue = -1.0;
// 
// 		return FALSE;
// 	}
// 
// 	// roi의 중심
// 	HTuple htCol_center = (double)rtRoi.Width() / 2.0;
// 	HTuple htRow_center = (double)rtRoi.Height() / 2.0;
// 
// 
// 	// roi의 중심에 가장 가까운 것 선택
// 	HRegion hrDefect;
// 	double dMinDistance = DBL_MAX;
// 
// 	for (int i = 0; i < hrCandidates.CountObj(); i++)
// 	{
// 		HRegion hrSelect = hrCandidates.SelectObj(i + 1);
// 
// 		HTuple htRow_select, htCol_select;
// 		hrSelect.AreaCenter(&htRow_select, &htCol_select);
// 
// 		HTuple htDistance_select;
// 		DistancePp(htRow_select, htCol_select, htRow_center, htCol_center, &htDistance_select);
// 
// 		double dDistance_select = htDistance_select;
// 
// 		if (dDistance_select < dMinDistance)
// 		{
// 			dMinDistance = dDistance_select;
// 			hrDefect = hrSelect;
// 		}
// 	}
// 
// 	if (hrDefect.Area() == 0)
// 	{
// 		*pDValue = -1.0;
// 
// 		return false;
// 	}
// 
// 
// 	//
// 	//
// 	//
// 	HXLDCont hxCurve;
// 	double dValue = 0.0;
// 
// 	if (!CalcNonLinearity(&hrDefect, &hxCurve, &dValue))
// 	{
// 		*pDValue = -1.0;
// 
// 		return false;
// 	}
// 
// 
// 	*pDValue = dValue;
// 
// 
// 	return true;
// }



//////////////////////////////////////////////////////////////////////////
//
//	SetAlignPoint_world
//
//	Align 기준점 좌표 입력(도면 좌표)
//
//		double dX1 : 좌상단 x
//		double dY1 : 좌상단 y
//		double dX2 : 우하단 x
//		double dY2 : 우하단 y
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::SetAlignPoint_world(double dX1, double dY1, double dX2, double dY2)
{
	m_dX_AlignKey_world[0] = dX1;
	m_dY_AlignKey_world[0] = dY1;

	m_dX_AlignKey_world[1] = dX2;
	m_dY_AlignKey_world[1] = dY2;


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	SetAlignImage_line
//
//	Align을 하기 위한 line 영상 획득 및 검사
//
//
//		BYTE *pImage,	: 영상 버퍼
//		int iWidth,		; 입력 영상 크기
//		int iHeight,	: 입력 영상 크기
//		int iPos,		: 제품 위치( _BHC_IP_LEFT ~ )
//		double dCx,		: stage 좌표
//		double dCy,		: stage 좌표
//		int iDirX,		: 영상과 stage간 좌표 방향
//		int iDirY		: 영상과 stage간 좌표 방향
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::SetAlignImage_line(BYTE *pImage, int iWidth, int iHeight, int iPos, double dCx, double dCy, int iDirX, int iDirY)
{

	double dCx_image = iWidth / 2.0;
	double dCy_image = iHeight / 2.0;


	//
	// convert image type : c++ > halcon
	//
	HImage hiGray;
	if (MakeHImage(pImage, iWidth, iHeight, &hiGray) == false)
	{
		return false;
	}

	//
	// Edge point 검사
	//
	stPoint2D pt1_image, pt2_image;

	// derivate_gauss
	HImage hiGray_deriv;
	if (iPos == _BHC_IP_LEFT_ || iPos == _BHC_IP_RIGHT_)
	{
		hiGray_deriv = hiGray.DerivateGauss(3, "x");
	}
	else {
		hiGray_deriv = hiGray.DerivateGauss(3, "y");
	}

	// threshold
	HRegion hrThreshold;
	if (iPos == _BHC_IP_BOTTOM_ || iPos == _BHC_IP_RIGHT_)
	{
		hrThreshold = hiGray_deriv.Threshold(1, 999);
	}
	else if (iPos == _BHC_IP_TOP_ || iPos == _BHC_IP_LEFT_)
	{
		hrThreshold = hiGray_deriv.InvertImage().Threshold(1, 999);
	}
	DISPLAY(hiGray, &hrThreshold, NULL, NULL, "threshold", m_dDisplayRatio);


	// connection
	HRegion hrThreshold_candidates = hrThreshold.Connection();
	DISPLAY(hiGray, &hrThreshold_candidates, NULL, NULL, "conection", m_dDisplayRatio);


	if (hrThreshold_candidates.CountObj() == 0)
	{
		return false;
	}

	// select_shape : lendth
	HTuple paramShape, paramMin, paramMax;
	paramShape[0] = "rect2_len1";
	paramMin[0] = (iWidth / 2.0) / 2.0;	// iWidth/2.0 가 최대 길이
	paramMax[0] = (iWidth / 2.0)*2.0;

	HRegion hrThreshold_candidates_lengthFilter = hrThreshold_candidates.SelectShape(paramShape, "and", paramMin, paramMax);
	DISPLAY(hiGray, &hrThreshold_candidates_lengthFilter, NULL, NULL, "select long", m_dDisplayRatio);

	if (hrThreshold_candidates_lengthFilter.CountObj() == 0)
	{
		return false;
	}

	// select_shape : position
	HRegion hrTarget;

	double dDistanceMin = DBL_MAX;	// init
	for (int i = 0; i < hrThreshold_candidates_lengthFilter.CountObj(); i++)
	{
		HRegion hrRoi = hrThreshold_candidates_lengthFilter.SelectObj(i + 1);

		// center
		HTuple htRow, htCol;
		hrRoi.AreaCenter(&htRow, &htCol);

		// distance
		HTuple htDistance;
		DistancePp(htRow, htCol, HTuple(iHeight / 2.0), HTuple(iWidth / 2.0), &htDistance);

		// update
		double dDistance_local = htDistance;
		if (dDistance_local < dDistanceMin)
		{
			dDistanceMin = dDistance_local;

			hrTarget = hrRoi;
		}
	}

	DISPLAY(hiGray, &hrTarget, NULL, NULL, "select center", m_dDisplayRatio);


	// skeleton
	HRegion hrTarget_skeleton = hrTarget.DilationCircle(3.5).Skeleton();
	DISPLAY(hiGray, &hrTarget_skeleton, NULL, NULL, "skeleton", m_dDisplayRatio);

	// gen xld
	HXLDCont hxEdge = hrTarget_skeleton.GenContoursSkeletonXld(1, "filter");
	HXLDCont hxEdge_union = hxEdge.UnionCollinearContoursXld(10, 1, 2, 0.1, "attr_keep");
	DISPLAY(hiGray, NULL, &hxEdge_union, NULL, "gen xld", m_dDisplayRatio);

	if (hxEdge_union.CountObj() == 0)
	{
		return false;
	}

	// 	HTuple paramShape2, paramMin2, paramMax2;
	paramShape[0] = "rect2_len1";
	paramMin[0] = (iWidth / 2.0) / 2.0;	// iWidth/2.0 가 최대 길이
	paramMax[0] = (iWidth / 2.0)*2.0;
	HXLDCont hxEdge_union_lengthFilter = hxEdge_union.SelectShapeXld(paramShape, "and", paramMin, paramMax);
	DISPLAY(hiGray, NULL, &hxEdge_union_lengthFilter, NULL, "select long", m_dDisplayRatio);

	if (hxEdge_union_lengthFilter.CountObj() == 0)
	{
		return false;
	}


	HXLDCont hxTarget = hxEdge_union_lengthFilter.SelectObj(1);	// init

	dDistanceMin = DBL_MAX;	// init
	for (int i = 0; i < hxEdge_union_lengthFilter.CountObj(); i++)
	{
		HXLDCont hrRoi = hxEdge_union_lengthFilter.SelectObj(i + 1);

		// center
		HTuple htRow, htCol, htArea;
		hxEdge_union_lengthFilter.AreaCenterPointsXld(&htRow, &htCol);

		// distance
		HTuple htDistance;
		DistancePp(htRow, htCol, HTuple(iHeight / 2.0), HTuple(iWidth / 2.0), &htDistance);

		// update
		double dDistance_local = htDistance;
		if (dDistance_local < dDistanceMin)
		{
			dDistanceMin = dDistance_local;

			hxTarget = hrRoi;
		}
	}
	DISPLAY(hiGray, NULL, &hxTarget, NULL, "selected line", m_dDisplayRatio);






	// line fitting
	HTuple htRowBegin, htColBegin, htRowEnd, htColEnd, htNr, htNc, htDist;
	hxTarget.FitLineContourXld("tukey",
		-1,
		0,
		5,
		2,
		&htRowBegin,
		&htColBegin,
		&htRowEnd,
		&htColEnd,
		&htNr,
		&htNc,
		&htDist);


	pt1_image.x = htColBegin;
	pt1_image.y = htRowBegin;
	pt2_image.x = htColEnd;
	pt2_image.y = htRowEnd;


	//
	// convert coordinate : image > stage
	//
	stPoint2D pt1_stage, pt2_stage;

	pt1_stage.x = dCx + iDirX * (pt1_image.x - dCx_image) * m_dResolution_x;
	pt1_stage.y = dCy + iDirY * (pt1_image.y - dCy_image) * m_dResolution_y;

	pt2_stage.x = dCx + iDirX * (pt2_image.x - dCx_image) * m_dResolution_x;
	pt2_stage.y = dCy + iDirY * (pt2_image.y - dCy_image) * m_dResolution_y;


	//
	// 결과값 저장
	//
	switch (iPos)
	{
	case _BHC_IP_LEFT_:
		m_dX_AlignLine_left_local[0] = pt1_stage.x;
		m_dY_AlignLine_left_local[0] = pt1_stage.y;
		m_dX_AlignLine_left_local[1] = pt2_stage.x;
		m_dY_AlignLine_left_local[1] = pt2_stage.y;
		break;

	case _BHC_IP_RIGHT_:
		m_dX_AlignLine_right_local[0] = pt1_stage.x;
		m_dY_AlignLine_right_local[0] = pt1_stage.y;
		m_dX_AlignLine_right_local[1] = pt2_stage.x;
		m_dY_AlignLine_right_local[1] = pt2_stage.y;
		break;

	case _BHC_IP_TOP_:
		m_dX_AlignLine_top_local[0] = pt1_stage.x;
		m_dY_AlignLine_top_local[0] = pt1_stage.y;
		m_dX_AlignLine_top_local[1] = pt2_stage.x;
		m_dY_AlignLine_top_local[1] = pt2_stage.y;
		break;

	case _BHC_IP_BOTTOM_:
		m_dX_AlignLine_bottom_local[0] = pt1_stage.x;
		m_dY_AlignLine_bottom_local[0] = pt1_stage.y;
		m_dX_AlignLine_bottom_local[1] = pt2_stage.x;
		m_dY_AlignLine_bottom_local[1] = pt2_stage.y;
		break;

	default:
// 		OutputDebugString(_T("SetAlignImage_line > error > iPos"));
		return false;
	}


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	SetAlignImage_line
//
//	Align을 하기 위한 line 영상 획득 및 검사
//
//
//		BYTE *pImage,	: 영상 버퍼
//		int iWidth,		; 입력 영상 크기
//		int iHeight,	: 입력 영상 크기
//		int iPos,		: 제품 위치( _BHC_IP_LEFT ~ )
//		double dCx,		: stage 좌표
//		double dCy,		: stage 좌표
//		int iDirX,		: 영상과 stage간 좌표 방향
//		int iDirY		: 영상과 stage간 좌표 방향
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::SetAlignImage_line_color(BYTE *pImage, int iWidth, int iHeight, int iPos, double dCx, double dCy, int iDirX, int iDirY, int iSaveIndex)
{

	double dCx_image = iWidth / 2.0;
	double dCy_image = iHeight / 2.0;


	//
	// convert image type : c++ > halcon
	//
	HImage hiColor;
	MakeHImage_color( pImage, iWidth, iHeight, &hiColor );

	HImage hiGray = hiColor.Rgb1ToGray();


	ostringstream ostr;
	ostr<<"d:\\test\\input"<<iSaveIndex;
	SAVE_IMAGE(hiColor, NULL, NULL,NULL,std::string(ostr.str()));


	//
	// Edge point 검사
	//
	stPoint2D pt1_image, pt2_image;

	// derivate_gauss
	HImage hiGray_deriv;
	if (iPos == _BHC_IP_LEFT_ || iPos == _BHC_IP_RIGHT_)
	{
		hiGray_deriv = hiGray.DerivateGauss(3, "x");
	}
	else {
		hiGray_deriv = hiGray.DerivateGauss(3, "y");
	}

	// threshold
	HRegion hrThreshold;
	if (iPos == _BHC_IP_BOTTOM_ || iPos == _BHC_IP_RIGHT_)
	{
		hrThreshold = hiGray_deriv.Threshold(1, 999);
	}
	else if (iPos == _BHC_IP_TOP_ || iPos == _BHC_IP_LEFT_)
	{
		hrThreshold = hiGray_deriv.InvertImage().Threshold(1, 999);
	}
	DISPLAY(hiGray, &hrThreshold, NULL, NULL, "threshold", m_dDisplayRatio);


	// connection
	HRegion hrThreshold_candidates = hrThreshold.Connection();
// 	DISPLAY(hiGray, &hrThreshold_candidates, NULL, NULL, "conection", m_dDisplayRatio);


	if (hrThreshold_candidates.CountObj() == 0)
	{
		return false;
	}

	// select_shape : lendth
	HTuple paramShape, paramMin, paramMax;
	paramShape[0] = "rect2_len1";
	paramMin[0] = (iWidth / 2.0) / 2.0;	// iWidth/2.0 가 최대 길이
	paramMax[0] = (iWidth / 2.0)*2.0;

	HRegion hrThreshold_candidates_lengthFilter = hrThreshold_candidates.SelectShape(paramShape, "and", paramMin, paramMax);
// 	DISPLAY(hiGray, &hrThreshold_candidates_lengthFilter, NULL, NULL, "select long", m_dDisplayRatio);

	if (hrThreshold_candidates_lengthFilter.CountObj() == 0)
	{
		return false;
	}

	// select_shape : position
	HRegion hrTarget;

	double dDistanceMin = DBL_MAX;	// init
	for (int i = 0; i < hrThreshold_candidates_lengthFilter.CountObj(); i++)
	{
		HRegion hrRoi = hrThreshold_candidates_lengthFilter.SelectObj(i + 1);

		// center
		HTuple htRow, htCol;
		hrRoi.AreaCenter(&htRow, &htCol);

		// distance
		HTuple htDistance;
		DistancePp(htRow, htCol, HTuple(iHeight / 2.0), HTuple(iWidth / 2.0), &htDistance);

		// update
		double dDistance_local = htDistance;
		if (dDistance_local < dDistanceMin)
		{
			dDistanceMin = dDistance_local;

			hrTarget = hrRoi;
		}
	}

// 	DISPLAY(hiGray, &hrTarget, NULL, NULL, "select center", m_dDisplayRatio);


	// skeleton
	HRegion hrTarget_skeleton = hrTarget.DilationCircle(3.5).Skeleton();
// 	DISPLAY(hiGray, &hrTarget_skeleton, NULL, NULL, "skeleton", m_dDisplayRatio);

	// gen xld
	HXLDCont hxEdge = hrTarget_skeleton.GenContoursSkeletonXld(1, "filter");
	HXLDCont hxEdge_union = hxEdge.UnionCollinearContoursXld(10, 1, 2, 0.1, "attr_keep");
// 	DISPLAY(hiGray, NULL, &hxEdge_union, NULL, "gen xld", m_dDisplayRatio);

	if (hxEdge_union.CountObj() == 0)
	{
		return false;
	}

	// 	HTuple paramShape2, paramMin2, paramMax2;
	paramShape[0] = "rect2_len1";
	paramMin[0] = (iWidth / 2.0) / 2.0;	// iWidth/2.0 가 최대 길이
	paramMax[0] = (iWidth / 2.0)*2.0;
	HXLDCont hxEdge_union_lengthFilter = hxEdge_union.SelectShapeXld(paramShape, "and", paramMin, paramMax);
// 	DISPLAY(hiGray, NULL, &hxEdge_union_lengthFilter, NULL, "select long", m_dDisplayRatio);

	if (hxEdge_union_lengthFilter.CountObj() == 0)
	{
		return false;
	}


	HXLDCont hxTarget = hxEdge_union_lengthFilter.SelectObj(1);	// init

	dDistanceMin = DBL_MAX;	// init
	for (int i = 0; i < hxEdge_union_lengthFilter.CountObj(); i++)
	{
		HXLDCont hrRoi = hxEdge_union_lengthFilter.SelectObj(i + 1);

		// center
		HTuple htRow, htCol, htArea;
		hxEdge_union_lengthFilter.AreaCenterPointsXld(&htRow, &htCol);

		// distance
		HTuple htDistance;
		DistancePp(htRow, htCol, HTuple(iHeight / 2.0), HTuple(iWidth / 2.0), &htDistance);

		// update
		double dDistance_local = htDistance;
		if (dDistance_local < dDistanceMin)
		{
			dDistanceMin = dDistance_local;

			hxTarget = hrRoi;
		}
	}
// 	DISPLAY(hiGray, NULL, &hxTarget, NULL, "selected line", m_dDisplayRatio);


	// line fitting
	HTuple htRowBegin, htColBegin, htRowEnd, htColEnd, htNr, htNc, htDist;
	hxTarget.FitLineContourXld("tukey",
		-1,
		0,
		5,
		2,
		&htRowBegin,
		&htColBegin,
		&htRowEnd,
		&htColEnd,
		&htNr,
		&htNc,
		&htDist);


	HTuple htRows_line, htCols_line;
	htRows_line[0] = htRowBegin;
	htRows_line[1] = htRowEnd;
	htCols_line[0] = htColBegin;
	htCols_line[1] = htColEnd;

	HXLDCont hxLine;
	GenContourPolygonXld( &hxLine, htRows_line, htCols_line );
	DISPLAY(hiGray, NULL, &hxLine, NULL, "final line", m_dDisplayRatio);



	//
	pt1_image.x = htColBegin;
	pt1_image.y = htRowBegin;
	pt2_image.x = htColEnd;
	pt2_image.y = htRowEnd;


	//
	// convert coordinate : image > stage
	//
	stPoint2D pt1_stage, pt2_stage;

	pt1_stage.x = dCx + iDirX * (pt1_image.x - dCx_image) * m_dResolution_x;
	pt1_stage.y = dCy + iDirY * (pt1_image.y - dCy_image) * m_dResolution_y;

	pt2_stage.x = dCx + iDirX * (pt2_image.x - dCx_image) * m_dResolution_x;
	pt2_stage.y = dCy + iDirY * (pt2_image.y - dCy_image) * m_dResolution_y;


	//
	// 결과값 저장
	//
	switch (iPos)
	{
	case _BHC_IP_LEFT_:
		m_dX_AlignLine_left_local[0] = pt1_stage.x;
		m_dY_AlignLine_left_local[0] = pt1_stage.y;
		m_dX_AlignLine_left_local[1] = pt2_stage.x;
		m_dY_AlignLine_left_local[1] = pt2_stage.y;
		break;

	case _BHC_IP_RIGHT_:
		m_dX_AlignLine_right_local[0] = pt1_stage.x;
		m_dY_AlignLine_right_local[0] = pt1_stage.y;
		m_dX_AlignLine_right_local[1] = pt2_stage.x;
		m_dY_AlignLine_right_local[1] = pt2_stage.y;
		break;

	case _BHC_IP_TOP_:
		m_dX_AlignLine_top_local[0] = pt1_stage.x;
		m_dY_AlignLine_top_local[0] = pt1_stage.y;
		m_dX_AlignLine_top_local[1] = pt2_stage.x;
		m_dY_AlignLine_top_local[1] = pt2_stage.y;
		break;

	case _BHC_IP_BOTTOM_:
		m_dX_AlignLine_bottom_local[0] = pt1_stage.x;
		m_dY_AlignLine_bottom_local[0] = pt1_stage.y;
		m_dX_AlignLine_bottom_local[1] = pt2_stage.x;
		m_dY_AlignLine_bottom_local[1] = pt2_stage.y;
		break;

	default:
		// 		OutputDebugString(_T("SetAlignImage_line > error > iPos"));
		return false;
	}


	// save result image
	std::list<HXLDCont> hxList;
	hxList.push_back(hxLine);

	ostr.str("");
	ostr.clear();
	ostr<<"d:\\test\\result"<<iSaveIndex;
	SAVE_IMAGE(hiColor, NULL, &hxList, NULL,std::string(ostr.str()));


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	CalcCoordinateMatrix
//
//	모서리 부분의 선분을 이용하여 꼭지점을 구하고, 이를 이용하고 좌표 변환 matrix 계산
//	
//	Affine matrix 획득
//		world to local : 도면 좌표에서 stage 좌표를 얻을 수 있도록...
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::CalcCoordinateMatrix()
{
	bool bOk;
	//
	// Align point 결정
	//
	// left-top

	bOk = CalcConer(m_dX_AlignLine_left_local[0],	m_dY_AlignLine_left_local[0],
					m_dX_AlignLine_left_local[1],	m_dY_AlignLine_left_local[1],
					m_dX_AlignLine_top_local[0],	m_dY_AlignLine_top_local[0],
					m_dX_AlignLine_top_local[1],	m_dY_AlignLine_top_local[1],
					&(m_dX_AlignKey_local[0]),
					&(m_dY_AlignKey_local[0]));

	if (bOk == false)
	{
		return false;
	}

	// right-bottom
	bOk = CalcConer(m_dX_AlignLine_right_local[0],	m_dY_AlignLine_right_local[0],
					m_dX_AlignLine_right_local[1],	m_dY_AlignLine_right_local[1],
					m_dX_AlignLine_bottom_local[0],	m_dY_AlignLine_bottom_local[0],
					m_dX_AlignLine_bottom_local[1],	m_dY_AlignLine_bottom_local[1],
					&(m_dX_AlignKey_local[1]),
					&(m_dY_AlignKey_local[1]));

	if (bOk == false)
	{
		return false;
	}


	//
	// calc matrix
	//
	HTuple htRows_local, htCols_local, htRows_world, htCols_world;

	htRows_local[0] = m_dY_AlignKey_local[0];
	htRows_local[1] = m_dY_AlignKey_local[1];

	htCols_local[0] = m_dX_AlignKey_local[0];
	htCols_local[1] = m_dX_AlignKey_local[1];

	htRows_world[0] = m_dY_AlignKey_world[0];
	htRows_world[1] = m_dY_AlignKey_world[1];

	htCols_world[0] = m_dX_AlignKey_world[0];
	htCols_world[1] = m_dX_AlignKey_world[1];


	VectorToSimilarity(htRows_world, htCols_world, htRows_local, htCols_local, &m_htCoordinateMatrix_worldToLocal);

	VectorToSimilarity(htRows_local, htCols_local, htRows_world, htCols_world, &m_htCoordinateMatrix_localToWorld);


	int nData = m_htCoordinateMatrix_worldToLocal.TupleLength();
	// 	std::string strMemo;
	// 	sprintf_s( (char*)strMemo.c_str(), "similarity matrix : #element = %d", nData );

	double a = m_htCoordinateMatrix_localToWorld[0];
	double b = m_htCoordinateMatrix_localToWorld[1];
	double c = m_htCoordinateMatrix_localToWorld[2];
	double d = m_htCoordinateMatrix_localToWorld[3];
	double e = m_htCoordinateMatrix_localToWorld[4];
	double f = m_htCoordinateMatrix_localToWorld[5];

	for( int i=0 ; i<6 ; i++ )
	{
		m_dAffineMatirx[i] = m_htCoordinateMatrix_worldToLocal[i];
	}

	return true;
}



bool CImageProcess_H::GetCoodinate(double dX_world, double dY_wordl, double *dX_stage, double *dY_stage, int iDir)
{
	HTuple htRow, htCol;

	if (iDir == _BHC_IP_LOCAL_TO_WORLD_)
	{
		AffineTransPoint2d(m_htCoordinateMatrix_localToWorld, HTuple(dY_wordl), HTuple(dX_world), &htRow, &htCol);
	}



	if (iDir == _BHC_IP_WORLD_TO_LOCAL_)
	{
		AffineTransPoint2d(m_htCoordinateMatrix_worldToLocal, HTuple(dY_wordl), HTuple(dX_world), &htRow, &htCol);
	}



	*dX_stage = htCol;
	*dY_stage = htRow;

	return false;
}


//////////////////////////////////////////////////////////////////////////
//
//	FindRoi_batWeld
//
//	ROI 영역 추출
//
//		- 용접부위 8개소 검출
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::FindRoi_batWeld(BYTE *pData, int iWidth, int iHeight, int iThreshold, int iMinSize, int iMaxSize)
{
	if (pData == NULL || iWidth < 1 || iHeight < 1)
	{
		return false;
	}

	std::ostringstream ostr;

	// halcon image 생성
	HImage hiGray;
	if (!MakeHImage(pData, iWidth, iHeight, &hiGray))
	{
		return false;
	}


	//
	// find welding
	//
	// threshold
	HRegion hrThreshold = hiGray.Threshold(iThreshold, 255);
// 	HRegion hrThreshold = hiGray.Threshold(0, iThreshold);

	DISPLAY(hiGray, &hrThreshold, NULL, NULL, "threshold", m_dDisplayRatio);

	// dilation
	HRegion hrThreshold_dilation = hrThreshold.ErosionCircle(1.5).DilationCircle(10.5);
// 	HRegion hrThreshold_dilation = hrThreshold.ErosionCircle(1.5).DilationCircle(5.5);


	// connection
	HRegion hrCandidates = hrThreshold_dilation.Connection();


	// select shape
	HTuple paramShape, paramMin, paramMax;
	paramShape[0] = "area";
	paramMin[0] = iMinSize;	// iWidth/2.0 가 최대 길이
	paramMax[0] = iMaxSize;

	paramShape[1] = "circularity";
	paramMin[1] = 0.4;	// iWidth/2.0 가 최대 길이
	paramMax[1] = 1.0;

	paramShape[2] = "outer_radius";
	paramMin[2] = 30;	// 20;	// iWidth/2.0 가 최대 길이
	paramMax[2] = 100;	// 80;

	HRegion hrLast = hrCandidates.SelectShape(paramShape, "and", paramMin, paramMax);

	hrLast = hrLast.Intersection(hrThreshold);

	// 유효성 검사
	int nBlob = (int)hrLast.CountObj();

	ostr << "last blob number is " << nBlob << std::endl;
	DISPLAY(hiGray, &hrLast, NULL, NULL, ostr.str(), m_dDisplayRatio);


	if (nBlob != 8)
	{
		return false;
	}


	//
	// sort
	//
	int iSortedIndex[8];
	HTuple htRows, htCols;
	hrLast.AreaCenter(&htRows, &htCols);

	// 좌우 2개 군으로 정렬 - x 좌표 기준 
	HTuple htIndices = htCols.TupleSortIndex();

	// 왼쪽 4개 정렬
	{
		HTuple htRows_left = htRows.Clone();
		for (int i = 4; i < 8; i++)
		{
			// 오른쪽 것들의 y좌표를 무효화 한다.(최대치 설정)
			int index = htIndices[i];
			htRows_left[index] = htRows_left.TupleMax() + 1.0;
		}

		HTuple htIndices_left = htRows_left.TupleSortIndex();	// y 좌표로 정렬

		// 업데이트
		for (int i = 0; i < 4; i++)
		{
			iSortedIndex[i] = htIndices_left[i];
		}
	}

	// 오른쪽 4개 정렬
	{
		HTuple htRows_right = htRows.Clone();
		for (int i = 0; i < 4; i++)
		{
			// 왼쪽 것들의 y좌표를 무효화 한다.(최대치 설정)
			int index = htIndices[i];
			htRows_right[index] = htRows_right.TupleMax() + 1.0;
		}

		HTuple htIndices_right = htRows_right.TupleSortIndex();	// y 좌표로 정렬

		// 업데이트
		for (int i = 0; i < 4; i++)
		{
			iSortedIndex[4 + i] = htIndices_right[i];
		}
	}


	//
	// 결과 저장
	//
	std::list<stDisplsyInfo> displayInfo;
	for (int i = 0; i < 8; i++)
	{
		m_hrRoi[i] = hrLast.SelectObj(iSortedIndex[i] + 1);

		// 중심점 검출
		double cx, cy;
		m_hrRoi[i].AreaCenter(&cy, &cx);


		// 정보 기록
		stDisplsyInfo info;

		info.x = (int)cx;
		info.y = (int)cy;
		info.bOk = true;

		ostr.str("");
		ostr.clear();
		ostr << "order " << i << std::endl;
		info.str = ostr.str();


		// 불량 정보 추가
		displayInfo.push_back(info);
	}


	DISPLAY(hiGray, NULL, NULL, &displayInfo, "", m_dDisplayRatio);


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	Inspect_batWeld
//
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::Inspect_batWeld(BYTE *pData, int iWidth, int iHeight)
{

	// halcon image 생성
	HImage hiRed, hiGreen, hiBlue;
	if (MakeHImage_color(pData, iWidth, iHeight, &hiRed, &hiGreen, &hiBlue) == false)
	{
		return false;
	}


	HImage hiGray;
	Rgb3ToGray( hiRed, hiGreen, hiBlue, &hiGray );
// 	if( m_bDisplay)
// 	{
// 		hiGray.WriteImage("bmp", 0, "d:\\test\\gray.bmp");
// 	}



	// XYZ 정보 추출
	HImage hiXyz_x, hiXyz_y, hiXyz_z;
	TransFromRgb(hiRed, hiGreen, hiBlue, &hiXyz_x, &hiXyz_y, &hiXyz_z, HTuple("ciexyz"));


	// HSV 정보 추출
	HImage hiHsv_h, hiHsv_s, hiHsv_v;
	TransFromRgb(hiRed, hiGreen, hiBlue, &hiHsv_h, &hiHsv_s, &hiHsv_v, HTuple("hsv"));


	// HSV 정보 추출
	HImage hiLuv_l, hiLuv_u, hiLuv_v;
	TransFromRgb(hiRed, hiGreen, hiBlue, &hiLuv_l, &hiLuv_u, &hiLuv_v, HTuple("cieluv"));


	HRegion hrOutters;


	hrOutters.GenEmptyRegion();

	BHC_MEMO("1");

	for (int i = 0; i < 8; i++)
	{
		HRegion hrInterest = m_hrRoi[i];

		// 좌표 획득
		HTuple htRows, htCols;
		hrInterest.GetRegionPoints(&htRows, &htCols);

		BHC_MEMO("2");

		//
		// 
		//
		// area
		int iArea = 0;
		double dRadius = 0.0;
		double dRow_outter = 0;
		double dCol_outter = 0;
		if( !InspWeldingArea( hiRed, hrInterest, &iArea, &dRadius, &dRow_outter, &dCol_outter ) )
		{
			continue;
		}
		m_stBatWeldInfo[i].iArea = iArea;
// 		m_stBatWeldInfo[i].iArea = hrInterest.Area();

		// radius
		m_stBatWeldInfo[i].dRadius = dRadius;

		HRegion hrOutter;
		hrOutter.GenCircle(dRow_outter, dCol_outter, dRadius);
		ConcatObj(hrOutters, hrOutter, &hrOutters);


		// edge

		BHC_MEMO("3");

		//
		// xyz
		//
		HTuple htXyz_x = hiXyz_x.GetGrayval(htRows, htCols);
		HTuple htXyz_y = hiXyz_y.GetGrayval(htRows, htCols);
		HTuple htXyz_z = hiXyz_z.GetGrayval(htRows, htCols);

		// x
		HTuple htMean = htXyz_x.TupleMean();
		HTuple htMin = htXyz_x.TupleMin();
		HTuple htMax = htXyz_x.TupleMax();
		HTuple htDev = htXyz_x.TupleDeviation();
		m_stBatWeldInfo[i].dXyz_x_mean = htMean / 255.0;
		m_stBatWeldInfo[i].dXyz_x_deviation = htDev / 255.0;

		m_stBatWeldInfo[i].listXyz_x.clear();
		for( int k=0 ; k<htXyz_x.TupleLength() ; k++ )
		{
			double a = htXyz_x[k];
			m_stBatWeldInfo[i].listXyz_x.push_back(htXyz_x[k]/255.0);
		}


		// y
		htMean = htXyz_y.TupleMean();
		htMin = htXyz_y.TupleMin();
		htMax = htXyz_y.TupleMax();
		htDev = htXyz_y.TupleDeviation();
		m_stBatWeldInfo[i].dXyz_y_mean = htMean / 255.0;
		m_stBatWeldInfo[i].dXyz_y_deviation = htDev / 255.0;

		m_stBatWeldInfo[i].listXyz_y.clear();
		for( int k=0 ; k<htXyz_y.TupleLength() ; k++ )
		{
			m_stBatWeldInfo[i].listXyz_y.push_back(htXyz_y[k]/255.0);
		}

		// z
		htMean = htXyz_z.TupleMean();
		htMin = htXyz_z.TupleMin();
		htMax = htXyz_z.TupleMax();
		htDev = htXyz_z.TupleDeviation();
		m_stBatWeldInfo[i].dXyz_z_mean = htMean / 255.0;
		m_stBatWeldInfo[i].dXyz_z_deviation = htDev / 255.0;

		m_stBatWeldInfo[i].listXyz_z.clear();
		for( int k=0 ; k<htXyz_z.TupleLength() ; k++ )
		{
			m_stBatWeldInfo[i].listXyz_z.push_back(htXyz_z[k]/255.0);
		}

		BHC_MEMO("4");

		//
		// hsv
		//
		HTuple htHsv_h = hiHsv_h.GetGrayval(htRows, htCols);
		HTuple htHsv_s = hiHsv_s.GetGrayval(htRows, htCols);
		HTuple htHsv_v = hiHsv_v.GetGrayval(htRows, htCols);

		// h
		htMean = htHsv_h.TupleMean();
		htMin = htHsv_h.TupleMin();
		htMax = htHsv_h.TupleMax();
		htDev = htHsv_h.TupleDeviation();
		m_stBatWeldInfo[i].dHsv_h_mean = htMean / 255.0;
		m_stBatWeldInfo[i].dHsv_h_deviation = htDev / 255.0;

		double dSum = 0;
		unsigned int uiCnt = 0;
		m_stBatWeldInfo[i].listHsv_h.clear();
		for( int k=0 ; k<htHsv_h.TupleLength() ; k++ )
		{
			// m_stBatWeldInfo[i].listHsv_h.push_back(htHsv_h[k]/255.0);


			// 임의로 각도 변경
			double dH = htHsv_h[k]/255.0;

			if( dH>0.5 )
			{
				dH = dH - 0.5;
			} else {
				dH = dH + 0.5;
			}

			m_stBatWeldInfo[i].listHsv_h.push_back(dH);

			dSum += dH;
			uiCnt++;
		}
		m_stBatWeldInfo[i].dHsv_h_mean = dSum / uiCnt;

		// s
		htMean = htHsv_s.TupleMean();
		htMin = htHsv_s.TupleMin();
		htMax = htHsv_s.TupleMax();
		htDev = htHsv_s.TupleDeviation();
		m_stBatWeldInfo[i].dHsv_s_mean = htMean / 255.0;
		m_stBatWeldInfo[i].dHsv_s_deviation = htDev / 255.0;

		m_stBatWeldInfo[i].listHsv_s.clear();
		for( int k=0 ; k<htHsv_s.TupleLength() ; k++ )
		{
			m_stBatWeldInfo[i].listHsv_s.push_back(htHsv_s[k]/255.0);
		}

		// v
		htMean = htHsv_v.TupleMean();
		htMin = htHsv_v.TupleMin();
		htMax = htHsv_v.TupleMax();
		htDev = htHsv_v.TupleDeviation();
		m_stBatWeldInfo[i].dHsv_v_mean = htMean / 255.0;
		m_stBatWeldInfo[i].dHsv_v_deviation = htDev / 255.0;

		m_stBatWeldInfo[i].listHsv_v.clear();
		for( int k=0 ; k<htHsv_v.TupleLength() ; k++ )
		{
			m_stBatWeldInfo[i].listHsv_v.push_back(htHsv_v[k]/255.0);
		}

		BHC_MEMO("5");

		//
		// LUV
		//
		HTuple htLuv_l = hiLuv_l.GetGrayval(htRows, htCols);
		HTuple htLuv_u = hiLuv_u.GetGrayval(htRows, htCols);
		HTuple htLuv_v = hiLuv_v.GetGrayval(htRows, htCols);

		// h
		htMean	= htLuv_l.TupleMean();
		htMin	= htLuv_l.TupleMin();
		htMax	= htLuv_l.TupleMax();
		htDev	= htLuv_l.TupleDeviation();
		m_stBatWeldInfo[i].dLuv_l_mean		= htMean / 255.0;
		m_stBatWeldInfo[i].dLuv_l_deviation = htDev / 255.0;

		m_stBatWeldInfo[i].listLuv_l.clear();
		for( int k=0 ; k<htLuv_l.TupleLength() ; k++ )
		{
			m_stBatWeldInfo[i].listLuv_l.push_back(htLuv_l[k]/255.0);
		}

		// s
		htMean	= htLuv_u.TupleMean();
		htMin	= htLuv_u.TupleMin();
		htMax	= htLuv_u.TupleMax();
		htDev	= htLuv_u.TupleDeviation();
		m_stBatWeldInfo[i].dLuv_u_mean		= htMean / 255.0;
		m_stBatWeldInfo[i].dLuv_u_deviation = htDev / 255.0;

		m_stBatWeldInfo[i].listLuv_u.clear();
		for( int k=0 ; k<htLuv_u.TupleLength() ; k++ )
		{
			m_stBatWeldInfo[i].listLuv_u.push_back(htLuv_u[k]/255.0);
		}

		// v
		htMean	= htLuv_v.TupleMean();
		htMin	= htLuv_v.TupleMin();
		htMax	= htLuv_v.TupleMax();
		htDev	= htLuv_v.TupleDeviation();
		m_stBatWeldInfo[i].dLuv_v_mean		= htMean / 255.0;
		m_stBatWeldInfo[i].dLuv_v_deviation = htDev / 255.0;

		m_stBatWeldInfo[i].listLuv_v.clear();
		for( int k=0 ; k<htLuv_v.TupleLength() ; k++ )
		{
			m_stBatWeldInfo[i].listLuv_v.push_back(htLuv_v[k]/255.0);
		}
		BHC_MEMO("6");
	}

	BHC_MEMO("7");

	DISPLAY(hiGray, &hrOutters, NULL, NULL, "outters" );


	return true;
}


bool CImageProcess_H::InspWeldingArea( const HalconCpp::HImage & image, HalconCpp::HRegion & hrRoi, int *piArea, double *pdRadius_outter, double *pdRow_outter, double *pdCol_outter )
{
	int iThreshold = 60;

	// reduce domain
	HImage hiRoi = image.ReduceDomain(hrRoi);


	// threshold & fill
	HRegion hrThreshold = hiRoi.Threshold(iThreshold, 255).FillUp();


	// opening ( remove noise )
	HRegion hrThreshold_open = hrThreshold.OpeningCircle(3.5);


	// outter radius
	HTuple htRow_outter, htCol_outter, htRadius_outter;
	hrThreshold_open.SmallestCircle( &htRow_outter, &htCol_outter, &htRadius_outter );

	HRegion hrOutter;
	hrOutter.GenCircle( htRow_outter, htCol_outter, htRadius_outter );

	// result
	// area
	// radius
	*piArea = hrThreshold.Area();
	*pdRadius_outter = htRadius_outter;
	*pdRow_outter = htRow_outter;
	*pdCol_outter = htCol_outter;


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	GetInfo_batWeld
//
//	용접부위 정보 추출
//
//
//		- 용접부 8개소에 대한 정보 획득
//
//////////////////////////////////////////////////////////////////////////
stInfo_batWeld *CImageProcess_H::GetInfo_batWeld(int *pDataNum)
{
	*pDataNum = 8;


	return m_stBatWeldInfo;
}



double CImageProcess_H::GetMatrix(int i)
{
	return m_dAffineMatirx[i];
}
*/

//////////////////////////////////////////////////////////////////////////
//
//	LoadTemplate
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::LoadTemplate()
{
	BHC_MEMO("CImageProcess_H::LoadTemplate > start");

	std::string strFile_template("d:\\AA_template.htp");
	std::string strFile_image("d:\\AA_image.bmp");


	BHC_MEMO("CImageProcess_H::LoadTemplate > template loading");
	// Load template model from file
	m_hsTemplate.ReadShapeModel(strFile_template.c_str());


	BHC_MEMO("CImageProcess_H::LoadTemplate > template image loading");
	// Load template image from image
	m_hiTemplateImage.ReadImage(strFile_image.c_str());


	BHC_MEMO("CImageProcess_H::LoadTemplate > find shape");
	// find shape
	HTuple htRow, htCol, htAngle, htScore;
	m_hsTemplate.FindShapeModel(m_hiTemplateImage,
		-0.39,
		0.79,
		0.5,	// min score
		1,	// Num match
		0.5,	// max overlap
		"none",	// subpixel
		0,	// num level
		0.9,	// greediness
		&htRow,
		&htCol,
		&htAngle,
		&htScore );


	// check error
	int iNumResult = htRow.Length();
	if( iNumResult < 1 )
	{
		BHC_MEMO("CImageProcess_H::LoadTemplate > fail > no result");
		return false;
	}


	BHC_MEMO("CImageProcess_H::LoadTemplate > calc matrix from model to template");
	// Calc matrix : model to template
	HHomMat2D hmForward;
	hmForward.VectorAngleToRigid( 0, 0,0, htRow[0], htCol[0], htAngle[0] );


	BHC_MEMO("CImageProcess_H::LoadTemplate > calc matrix from template to model");
	// Cac matrix : template to model
	HHomMat2D hhmTemplateToModel = hmForward.HomMat2dInvert();

	// save to file
	HSerializedItem hsData = hhmTemplateToModel.SerializeHomMat2d();
	HFile hfSave;
	hfSave.OpenFile(HTuple("d:\\hmm.hmm"), HTuple("output_binary") );
	hsData.FwriteSerializedItem(hfSave);


	BHC_MEMO("CImageProcess_H::LoadTemplate > show result");
	// * show result
	HXLDCont hxTemplate = m_hsTemplate.GetShapeModelContours(1);
	HXLDCont hxTemplate_matching = hxTemplate.AffineTransContourXld(hmForward);

	list<HalconCpp::HXLDCont> hxSave;
	hxSave.push_back(hxTemplate_matching);

//  	SAVE_IMAGE(m_hiTemplateImage, NULL, &hxSave, NULL, "d:\\test\\LoadTemplate");


	BHC_MEMO("CImageProcess_H::LoadTemplate > end");

	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	FindTemplate
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::FindTemplate( unsigned char *pData, int iWidth, int iHeight, unsigned char *pResult )
{
	BHC_MEMO("CImageProcess_H::FindTemplate > start");


	//
	//
	//

	// 이미지 type 변경 : c++ -> halcon
	HImage hiIn("byte", iWidth, iHeight, pData);


	BHC_MEMO("CImageProcess_H::FindTemplate > find template");
	// find template
	HTuple htRow, htCol, htAngle, htScore;
	m_hsTemplate.FindShapeModel(hiIn,
								-0.39,
								0.79,
								0.5,		// min score
								1,			// Num match
								0.5,		// max overlap
								"none",		// subpixel
								0,			// num level
								0.9,		// greediness
								&htRow,
								&htCol,
								&htAngle,
								&htScore );


	// check error
	int iNumResult = htRow.Length();
	if( iNumResult < 1 )
	{
		BHC_MEMO("CImageProcess_H::FindTemplate > fail > no result");
		return false;
	}


	BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image");
	//
	// Result
	//
	HHomMat2D hhmModelToInput;
	hhmModelToInput.VectorAngleToRigid( 0, 0,0, htRow[0], htCol[0], htAngle[0] );

	// ROI image
	// read file : Template to model
	HFile hfSave;
	hfSave.OpenFile(HTuple("d:\\hmm.hmm"), HTuple("input_binary") );	// file open
	
	HSerializedItem hsData;
	hsData.FreadSerializedItem(hfSave);	// read data

	HHomMat2D hhmTemplateToModel;
	hhmTemplateToModel.DeserializeHomMat2d(hsData);	// set matrix

	// Calc matrix from template to input
	HHomMat2D hhmTemplateToInput = hhmModelToInput.HomMat2dCompose(hhmTemplateToModel);

	// Set result image
	HImage hiROI = m_hiTemplateImage.AffineTransImage(hhmTemplateToInput, "constant", "false");

	HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;
	GetImagePointer1(hiROI, &hv_Pointer, &hv_Type, &hv_Width1, &hv_Height1);

	BYTE *pRoi = (LPBYTE)hv_Pointer.L();

	memcpy( pResult, pRoi, iWidth*iHeight );



	BHC_MEMO("CImageProcess_H::LoadTemplate > show result 1");
	//
	// show
	//
	// show result 1 : matching
	HXLDCont hxTemplate = m_hsTemplate.GetShapeModelContours(1);
	HXLDCont hxTemplate_matching = hxTemplate.AffineTransContourXld(hhmModelToInput);

	list<HalconCpp::HXLDCont> hxSave;
	hxSave.push_back(hxTemplate_matching);

// 	SAVE_IMAGE(hiIn, NULL, &hxSave, NULL, "d:\\test\\Template_found");


	BHC_MEMO("CImageProcess_H::LoadTemplate > show result 2");
	// show result 2 : ROI
// 	SAVE_IMAGE(hiROI, NULL, &hxSave, NULL, "d:\\test\\Template_image");


	BHC_MEMO("CImageProcess_H::LoadTemplate > end");


	return true;
}


/*
//////////////////////////////////////////////////////////////////////////
//
// Make himage(1ch)
//
//	- BGR 순서
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::MakeHImage(BYTE *pInputData, int iWidth, int iHeight, HImage* outImage)
{
	if (iWidth < 1 || iHeight < 1)
	{
		return false;
	}


	// 이미지 type 변경 : c++ -> halcon
	outImage->GenImage1("byte", iWidth, iHeight, pInputData);


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
// Make himage(color)
//
//	- BGR 순서
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::MakeHImage_color(BYTE *pInputData, int iWidth, int iHeight, HImage* outImage)
{
	if (iWidth < 1 || iHeight < 1)
	{
		return false;
	}


	//
	BYTE *pRed = new BYTE[iWidth*iHeight];
	BYTE *pGreen = new BYTE[iWidth*iHeight];
	BYTE *pBlue = new BYTE[iWidth*iHeight];

	for (int y = 0; y < iHeight; y++)
	{
		for (int x = 0; x < iWidth; x++)
		{
			pBlue[x + y * iWidth] = pInputData[3 * x + y * iWidth * 3];
			pGreen[x + y * iWidth] = pInputData[3 * x + 1 + y * iWidth * 3];
			pRed[x + y * iWidth] = pInputData[3 * x + 2 + y * iWidth * 3];
		}
	}


	// 이미지 type 변경 : c++ -> halcon
	outImage->GenImage3("byte", iWidth, iHeight, pRed, pGreen, pBlue);


	//
	delete[] pRed;
	delete[] pGreen;
	delete[] pBlue;


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::MakeHImage_color(BYTE *pInputData, int iWidth, int iHeight, HImage* hiRed, HImage* hiGreen, HImage* hiBlue)
{
	if (iWidth < 1 || iHeight < 1)
	{
		return false;
	}


	//
	BYTE *pRed = new BYTE[iWidth*iHeight];
	BYTE *pGreen = new BYTE[iWidth*iHeight];
	BYTE *pBlue = new BYTE[iWidth*iHeight];

	for (int y = 0; y < iHeight; y++)
	{
		for (int x = 0; x < iWidth; x++)
		{
			pBlue[x + y * iWidth] = pInputData[3 * x + y * iWidth * 3];
			pGreen[x + y * iWidth] = pInputData[3 * x + y * iWidth * 3 + 1];
			pRed[x + y * iWidth] = pInputData[3 * x + y * iWidth * 3 + 2];
		}
	}


	// 이미지 type 변경 : c++ -> halcon
	hiRed->GenImage1("byte", iWidth, iHeight, pRed);
	hiGreen->GenImage1("byte", iWidth, iHeight, pGreen);
	hiBlue->GenImage1("byte", iWidth, iHeight, pBlue);


	//
	delete[] pRed;
	delete[] pGreen;
	delete[] pBlue;


	return true;
}


// void CImageProcess_H::SAVE_IMAGE(const HImage & image, HRegion & region, CString strFileName)
// {
// 	HTuple imageWidth, imageHeight;
// 	image.GetImageSize(&imageWidth, &imageHeight);
// 
// 	HWindow w(0, 0, imageWidth, imageHeight, 0, "invisible", "");
// 
// 
// 	// 표시 영역 설정
// 	Hlong	winWidth = imageWidth;
// 	Hlong	winHeight = imageHeight;
// 	w.SetPart(0, 0, winHeight, winWidth);
// 
// 	w.SetColored(6);
// 	w.DispImage(image);
// 
// 	// 영역 그리기 설정
// 	w.SetDraw("margin");
// 	w.DispRegion(region);
// 
// 	HImage img;
// 	img.DumpWindowImage(w);
// 	img.WriteImage("bmp", 0, (CStringA)strFileName);
// 
// 	w.CloseWindow();
// }


//////////////////////////////////////////////////////////////////////////
//
// 영상 저장
//
//////////////////////////////////////////////////////////////////////////
void CImageProcess_H::SAVE_IMAGE(const HImage & image, HRegion * pRegion, list<HXLDCont> *pListHx, list<stDefectInfo> *pListInfo, std::string strFileName)
{
	HTuple imageWidth, imageHeight;
	image.GetImageSize(&imageWidth, &imageHeight);

	int wid = imageWidth;
	int hgt = imageHeight;

// 	HWindow w(0, 0, imageWidth*5, imageHeight*5, 0, "invisible", "");
	HWindow w(0, 0, imageWidth, imageHeight, 0, "invisible", "");


	// 표시 영역 설정
	Hlong	winWidth = imageWidth;
	Hlong	winHeight = imageHeight;
	w.SetPart(0, 0, winHeight, winWidth);

	// 	// 윈도우 크기 설정
// 	w.SetWindowExtents(0,0,winWidth,winHeight);

	w.SetColored(3);
	w.DispImage(image);

	//
	// 제목 표시
	//
// 	w.SetFont(HString("Courier-Normal-8"));
	w.SetColor("green");


	//
	// Draw region
	//
	w.SetDraw("margin");

	if (pRegion)
	{
		w.DispRegion(*pRegion);
	}


	//
	// Draw xld
	//
// 	w.SetLineWidth(5);
	if (pListHx)
	{
		for (list<HXLDCont>::iterator ite = pListHx->begin(); ite != pListHx->end(); ite++)
		{
			w.DispXld(*ite);
		}
	}



	//
	// Draw text
	//
	if (pListInfo)
	{
		for (list<stDefectInfo>::iterator ite = pListInfo->begin(); ite != pListInfo->end(); ite++)
		{
			if (ite->bValid)
			{
				w.SetColor("red");
			}
			else {
				w.SetColor("green");
			}



// 			CString strMemo;
// 			strMemo.Format(_T("C_%.1f,L_%.1f"), ite->dCirculatiry, ite->dLinear);

// 			char text[2048];
// 			int ret = _snprintf_s(text, 2048, _TRUNCATE, "C_%.1f,L_%.1f", ite->dCirculatiry, ite->dLinear);

// 			w.SetTposition((Hlong)(ite->iBottom), (Hlong)(ite->dCx));
// 			w.WriteString(text);
			w.SetTposition((Hlong)(ite->iBottom), (Hlong)(ite->dCx));
			w.WriteString(ite->text);
		}
	}



	//
	// mask image & save
	//
	HImage img;
	img.DumpWindowImage(w);
	img.WriteImage("bmp", 0, strFileName.c_str());

	w.CloseWindow();
}



//////////////////////////////////////////////////////////////////////////
//
//	CalcNonLinearity
//
//	스크래치 여부 분석용
//
//	1. 불량영역을 NUBS 곡선으로 fitting 
//	2. 원 곡선과 fitting 곡선과 차이를 구한다.
//	3. 차이가 클수록 스크래치가 아닌 것으로 생각한다.
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::CalcNonLinearity(HRegion *pRegion, HXLDCont *pHxLine, double *pMaxDistance)
{
	//
	// skeleton
	// 
	HRegion hrSkeleton = pRegion->Skeleton();


	//
	// covert region to xld
	//
	HXLDCont hxBone = hrSkeleton.GenContoursSkeletonXld(1, "filter");

	// 	*pHxLine = hxBone;	// 1

	HXLDCont hxBone_union = hxBone.UnionCotangentialContoursXld(0, 30, 0.785398, 25, 10, 2, "attr_forget");

	// 	*pHxLine = hxBone_union;	//2


	// 여러 라인이 나올수 있음 > 가장 긴 것 선택
	HXLDCont hxBone_long;
	double dMaxLength = -1;
	for (int i = 0; i < hxBone_union.CountObj(); i++)
	{
		double dLocalLength = hxBone_union.SelectObj(i + 1).LengthXld();

		if (dLocalLength > dMaxLength)
		{
			hxBone_long = hxBone_union.SelectObj(i + 1);

			dMaxLength = dLocalLength;
		}
	}


	//
	// smooth xld
	//
	HXLDCont hxBone_long_smooth = hxBone_long.SmoothContoursXld(5);


	//
	// gen nubs curve
	//
	// get points
	HTuple htRows, htCols;
	hxBone_long_smooth.GetContourXld(&htRows, &htCols);
	int iLength = (int)(htRows.Length());

	// select points
	int iStep = max(10, iLength / 4);
	HTuple htRows_key, htCols_key;
	int iCount_key = 0;
	for (int i = 0; i < iLength; i += iStep)
	{
		htRows_key[iCount_key] = htRows[i];
		htCols_key[iCount_key] = htCols[i];

		iCount_key++;
	}

	htRows_key[iCount_key - 1] = htRows[iLength - 1];
	htCols_key[iCount_key - 1] = htCols[iLength - 1];
	// 	iCount_key++;


	if (iCount_key < 4)
	{
		*pMaxDistance = -1.0;

		return false;
	}

	// gen nurbs
	HXLDCont hxBone_long_smooth_nurbs;
	hxBone_long_smooth_nurbs.GenContourNurbsXld(htRows_key,
		htCols_key,
		"auto",
		"auto",
		3,		// degree
		10,		// max error
		10);	// max distance


	//
	// calc distance
	//
	HTuple htRows_nurbs, htCols_nurbs;
	hxBone_long_smooth_nurbs.GetContourXld(&htRows_nurbs, &htCols_nurbs);

	int iLength_nurbs = (int)(htRows_nurbs.Length());

	double dMaxDistance = 0;
	for (int i = 0; i < iLength_nurbs; i++)
	{
		double dLocalMin = DBL_MAX;
		for (int k = 0; k < iLength; k++)
		{
			// 거리값
			HTuple htDistance;
			DistancePp(htRows_nurbs[i], htCols_nurbs[i], htRows[k], htCols[k], &htDistance);

			// 최대값 갱신
			double dDistance = htDistance;
			if (dDistance < dLocalMin)
			{
				dLocalMin = dDistance;
			}
		}


		if (dLocalMin > dMaxDistance)
		{
			dMaxDistance = dLocalMin;
		}
	}


	*pMaxDistance = dMaxDistance;
	*pHxLine = hxBone_long_smooth_nurbs;


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	CalcStringFeature_nonLinear
//
//	곡선으로 fitting 후, 이에 대해 벗어나는 정도를 검사한다.
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::CalcStringFeature_nonLinear(HalconCpp::HXLDCont *pHxLine, HalconCpp::HXLDCont *pHxFit, double *pMaxDistance)
{
	//
	// smooth xld
	//
	HXLDCont hxBone_long_smooth = pHxLine->SmoothContoursXld(5);


	//
	// gen nubs curve
	//
	// get points
	HTuple htRows, htCols;
	hxBone_long_smooth.GetContourXld(&htRows, &htCols);
	int iLength = (int)(htRows.Length());

	// select points : fitting을 위해서 N등분하여 key point를 업는다.
	int iStep = max(5, iLength / 4);
	HTuple htRows_key, htCols_key;
	int iCount_key = 0;
	for (int i = 0; i < iLength; i += iStep)
	{
		htRows_key[iCount_key] = htRows[i];
		htCols_key[iCount_key] = htCols[i];

		iCount_key++;
	}

	htRows_key[iCount_key - 1] = htRows[iLength - 1];
	htCols_key[iCount_key - 1] = htCols[iLength - 1];
	// 	iCount_key++;


	// point 갯수가 부족하면 pass
	if (iCount_key < 4)
	{
		*pMaxDistance = -1.0;

		return false;
	}

	// gen nurbs
	HXLDCont hxBone_long_smooth_nurbs;
	hxBone_long_smooth_nurbs.GenContourNurbsXld(htRows_key,
		htCols_key,
		"auto",
		"auto",
		3,		// degree
		10,		// max error
		10);	// max distance

	*pHxFit = hxBone_long_smooth_nurbs;


	//
	// calc distance
	//
	HTuple htRows_nurbs, htCols_nurbs;
	hxBone_long_smooth_nurbs.GetContourXld(&htRows_nurbs, &htCols_nurbs);

	int iLength_nurbs = (int)(htRows_nurbs.Length());

	double dMaxDistance = 0;
	for (int i = 0; i < iLength_nurbs; i++)
	{
		double dLocalMin = DBL_MAX;
		for (int k = 0; k < iLength; k++)
		{
			// 거리값
			HTuple htDistance;
			DistancePp(htRows_nurbs[i], htCols_nurbs[i], htRows[k], htCols[k], &htDistance);

			// 최대값 갱신
			double dDistance = htDistance;
			if (dDistance < dLocalMin)
			{
				dLocalMin = dDistance;
			}
		}


		if (dLocalMin > dMaxDistance)
		{
			dMaxDistance = dLocalMin;
		}
	}


	*pMaxDistance = dMaxDistance;
	*pHxLine = hxBone_long_smooth_nurbs;


	return true;
}


bool CImageProcess_H::CalcStringFeature(HalconCpp::HRegion *pHrTarget, HalconCpp::HRegion *pHrCandiates, HalconCpp::HXLDCont *pHxLine, double *pAngle_mean, double *pAngle_std, double *pAngleDer_mean, double *pAngleDer_std, double *pLength, double *pDist )
{
	double dRagio = 0.5;


	double dAngle_mean = -1;
	double dAngle_dev = -1;
	double dAngleDerivative_mean = -1;
	double dAngleDerivative_dev = -1;
	double dLength = -1;
	double dMaxDistance = -1;

	//
	// gen xld
	//

	// skeleton
	HRegion hrSkeleton = pHrCandiates->Skeleton();	// skeleton

	// covert region to xld
	HXLDCont hxBone = hrSkeleton.GenContoursSkeletonXld(1, "filter");	// convert : region > xld

	HXLDCont hxBone_union = hxBone.UnionCotangentialContoursXld(1, "auto", 0.785398, 25, 20, -5, "attr_forget");	// 인접선 연결

	HXLDCont hxBone_union_union = hxBone_union.UnionAdjacentContoursXld( 1, 1, "attr_keep").SmoothContoursXld(7);	// 인접선 연결 & smooth

// 	*pHxLine = hxBone_union_union;




	//
	// 관심 line 선택
	//
	//	1) 길이 기준을 정렬
	//	2) 길이가 긴것 부터해서 기존 region에 속한 선을 검출
	//
	HTuple htLength = hxBone_union_union.LengthXld();	// 선들의 길이
	HTuple htIndex_sort = htLength.TupleSortIndex();	// 길이 기준으로 선들을 정렬
	int nLength = htLength.Length();	// 선들의 갯 수 


	// 유효한 선인지 점검
	for( int i=nLength ; i>0 ; i--)
	{
		// 선 선택
		int iHtIndex_main = htIndex_sort[i-1];
		HXLDCont hxSelect = hxBone_union_union.SelectObj(iHtIndex_main+1);	// 선 선택

		// 유효 영역 설정
		HRegion hrSelect = hxSelect.GenRegionContourXld("filled").DilationCircle(1.5);	// convert region to xld & dilation

		// check intersection : 유효 영역에 속하는 선인지 점검
		HRegion hrIntersect = pHrTarget->Intersection(hrSelect);
		int iArea = hrIntersect.Area();


		// 유효영역에 속하는 선이면, 아래 실행
		if( hrIntersect.Area() > 1 )	// !!! : 겹치는 영역이 없으면 영역값이 1이다.
		{
			HTuple htLength_select = hxSelect.LengthXld();	// 길이 계산


			// 추가로 선 하나 더에 대해 목표 위치에 있는지 본다.
			bool bAdd = false;
			HXLDCont hxSelect_add;	// 
			for( int k=i-1 ; k>0 ; k-- )
			{
				int iHtIndex_add = htIndex_sort[k-1];	// index

				hxSelect_add = hxBone_union_union.SelectObj(iHtIndex_add+1);	// 선 선택

				HRegion hrSelect_add = hxSelect_add.GenRegionContourXld("filled").DilationCircle(1.5);	// convert region to xld & dilation
				HRegion hrIntersect_add = pHrTarget->Intersection(hrSelect_add);

				double dLength_add = hxSelect_add.LengthXld();

				if( hrSelect_add.Area() > 1 && dLength_add > dRagio * htLength_select[0] )	// !!! : 겹치는 영역이 없으면 영역값이 1이다.
				{
					bAdd = true;
					k = 0;
				}
			}

			// 선 선택
			if( bAdd == false )
			{
				*pHxLine = hxSelect;
			}else {
				*pHxLine = hxSelect.ConcatObj(hxSelect_add);
			}

			if( bAdd == false )
			{
				dLength = hxSelect.LengthXld();
			}else {
				dLength = hxSelect.LengthXld() + hxSelect_add.LengthXld();
			}

			// feature 계산 : angle
			if( CalcStringFeature( pHxLine, &dAngle_mean, &dAngle_dev, &dAngleDerivative_mean, &dAngleDerivative_dev) == false )
			{
				return false;
			}


			//	
			if( bAdd == false )
			{
				double dTempDist;
				HXLDCont hxFitting;	// 
				if( CalcStringFeature_nonLinear(&hxSelect, &hxFitting, &dTempDist) )
				{
					dMaxDistance = dTempDist;

					DISPLAY( m_hiOrg, NULL, &hxFitting, NULL, "CalcStringFeature_nonLinear > fitting", 5 );
				} 
			}else {
				double dTempDist;
				HXLDCont hxFitting;	// 
				if( CalcStringFeature_nonLinear(&hxSelect, &hxFitting, &dTempDist))
				{
					dMaxDistance = max( dMaxDistance, dTempDist );

					DISPLAY( m_hiOrg, NULL, &hxFitting, NULL, "CalcStringFeature_nonLinear > fitting", 5 );
				}

				if( CalcStringFeature_nonLinear(&hxSelect_add, &hxFitting, &dTempDist) )
				{
					dMaxDistance = max( dMaxDistance, dTempDist );

					DISPLAY( m_hiOrg, NULL, &hxFitting, NULL, "CalcStringFeature_nonLinear > fitting", 5 );
				}
			}


			//
			*pAngle_mean	= dAngle_mean;
			*pAngle_std		= dAngle_dev;
			*pAngleDer_mean	= dAngleDerivative_mean;
			*pAngleDer_std	= dAngleDerivative_dev;
			*pLength		= dLength;
			*pDist			= dMaxDistance;



			return true;
		}
	}


	return false;
}


bool CImageProcess_H::CalcStringFeature( HalconCpp::HXLDCont *pHxld, double *pdAngle_mean, double *pdAngleStd, double *pdAngleDer_mean, double *pdAngleDer_std)
{
	int nData = pHxld->CountObj();

	if( nData < 1 )
	{
		return false;
	}

	HTuple htAng_mean;
	HTuple htAng_std;
	HTuple htAngleNum;

	HTuple htAngDer_mean;
	HTuple htAngDer_std;
	HTuple htAngleDerNum;

	int iResult = 0;

	for( int i=0 ; i<nData ; i++ )
	{
		HXLDCont hxSelect = pHxld->SelectObj(i+1);

		HTuple htTemp = hxSelect.LengthXld();
		double dLength = htTemp[0];
		int a = hxSelect.CountObj();
		if( dLength < 10)
		{
			continue;;
		}

		// get angles : 크기가 작으면 에러남
		HTuple htAngles = hxSelect.GetContourAngleXld("abs", "regress", 3 );	// 각도의 분산을 구하려면, 각도에 따라 첫번째 인자에 대한 고려가 필요함

		htAngleNum[i] = htAngles.Length();
		double dAngle_avg = htAngles.TupleMean();	// main axis

		for( int k=0 ; k<htAngleNum[i] ; k++ )
		{
			htAngles[k] = CalcAngleRalation(dAngle_avg, htAngles[k]);
// 			double dAngle_rot = htAngles[k] - dAngle_avg;
// 
// 			while( dAngle_rot < 0 )
// 			{
// 				dAngle_rot += 2*3.14159265359;
// 			}
// 
// 			if( dAngle_rot > 3.14159265359 * 3 / 2)
// 			{
// 				htAngles[k] = dAngle_rot - 2*3.14159265359;
// 			} else if( dAngle_rot > 3.14159265359 ) 
// 			{
// 				htAngles[k] = dAngle_rot - 3.14159265359;
// 			} else if( dAngle_rot > 3.14159265359 / 2 )
// 			{
// 				htAngles[k] = dAngle_rot - 3.14159265359;
// 			} else {
// 				htAngles[k] = dAngle_rot;
// 			}
		}

		htAng_mean[i] = dAngle_avg;		// 평균 각도
		htAng_std[i] = htAngles.TupleDeviation();	// 각도의 분산


		// 
		HTuple htAnglesDer;
		int n = htAngles.Length();
		for( int k=0 ; k < n - 1 ; k++ )
		{
			htAnglesDer[k] = CalcAngleRalation(htAngles[k], htAngles[k+1]);
// 			htAnglesDer[k] = htAngles[k+1] - htAngles[k];
		}

		htAngleDerNum[i] = htAnglesDer.Length();

		htAngDer_mean[i] = htAnglesDer.TupleMean();		// 평균 각도
		htAngDer_std[i] = htAnglesDer.TupleDeviation();	// 각도의 분산

		iResult++;
	}

	if( iResult==0 )
	{
		return false;
	}

	double dSum1 = htAng_mean.TupleMult(htAngleNum).TupleSum();
	double dSum2 = htAngleNum.TupleSum();
	*pdAngle_mean = dSum1/dSum2;

	dSum1 = htAng_std.TupleMult(htAngleNum).TupleSum();
	dSum2 = htAngleNum.TupleSum();
	*pdAngleStd = dSum1/dSum2;

	dSum1 = htAngDer_mean.TupleMult(htAngleDerNum).TupleSum();
	dSum2 = htAngleDerNum.TupleSum();
	*pdAngleDer_mean = dSum1/dSum2;

	dSum1 = htAngDer_std.TupleMult(htAngleDerNum).TupleSum();
	dSum2 = htAngleDerNum.TupleSum();
	*pdAngleDer_std = dSum1/dSum2;


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	두 각도간의 관계를 구한다.
//
//	main 각도를 기준으로 sub 각도의 계산 결과는 -90  ~ 90도 
//
//
//////////////////////////////////////////////////////////////////////////
double CImageProcess_H::CalcAngleRalation( double dAngMain, double dAngSub )
{
	double dResultAngle = 0;

	double dAngle_rot = dAngSub - dAngMain;

	while( dAngle_rot < 0 )
	{
		dAngle_rot += 2*3.14159265359;
	}

	if( dAngle_rot > 3.14159265359 * 3 / 2)
	{
		dResultAngle = dAngle_rot - 2*3.14159265359;
	} else if( dAngle_rot > 3.14159265359 ) 
	{
		dResultAngle = dAngle_rot - 3.14159265359;
	} else if( dAngle_rot > 3.14159265359 / 2 )
	{
		dResultAngle = dAngle_rot - 3.14159265359;
	} else {
		dResultAngle = dAngle_rot;
	}


	return dResultAngle;
}


//////////////////////////////////////////////////////////////////////////
//
// CalcConer
//
//	ㅇ 두 선간의 교점 검사
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::CalcConer(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3, double dX4, double dY4, double *pDx, double *pDy)
{
	// 교점 검출


	// 직선에 대한 parameter
	// ax + by = c
	double dA1 = dY2 - dY1;
	double dB1 = dX1 - dX2;
	double dC1 = (dA1 * dX1) + (dB1 * dY1);


	// 직선에 대한 parameter
	// ax + by = c
	double dA2 = dY4 - dY3;
	double dB2 = dX3 - dX4;
	double dC2 = (dA2 * dX3) + (dB2 * dY3);

	double dDe = (dA1 * dB2) - (dB1 * dA2);

	// 예외
	if (dDe == 0)
	{
		return false;
	}


	*pDx = (LONG)(((dC1*dB2) - (dB1*dC2)) / dDe);
	*pDy = (LONG)(((dA1*dC2) - (dC1*dA2)) / dDe);


	return true;
}


bool CImageProcess_H::SetReviewImage( BYTE *pImage, int iWidth, int iHeight, int iDepth, int iCam, int iSeq )
{
// 	BHC_MEMO("SetReviewImage cam=%d, seq=%d", iCam, iSeq);

	if( iSeq >= _BHC_IP_REVIEW_NUM_ )
	{
// 		BHC_MEMO("SetReviewImage cam=%d, seq=%d...count over", iCam, iSeq);
		return false;
	}


	// review
	if(iCam==0)
	{
		if( m_pReview1[iSeq] )
		{
			delete[] m_pReview1[iSeq];
		}

		m_pReview1[iSeq] = new BYTE[ iWidth * iHeight * iDepth ];

		memcpy( m_pReview1[iSeq], pImage, iWidth * iHeight * iDepth );

		m_iWidth_review1 = iWidth;
		m_iHeight_review1 = iHeight;
	}

	// fire
	if( iCam==1 )
	{
		if( m_pReview2[iSeq] )
		{
			delete[] m_pReview2[iSeq];
		}

		m_pReview2[iSeq] = new BYTE[ iWidth * iHeight * iDepth ];

		memcpy( m_pReview2[iSeq], pImage, iWidth * iHeight * iDepth );
		
		m_iWidth_review2 = iWidth;
		m_iHeight_review2 = iHeight;
	}


	return true;
}



bool CImageProcess_H::InspectReview(int iSeq )
{
// 	BHC_MEMO("InspectReview seq=%d", iSeq);

	USES_CONVERSION;
	char logbuf[2048];
	int ret = _snprintf_s(logbuf, 2048, _TRUNCATE, "InspectReview seq=%d", iSeq);
	OutputDebugString(A2W(logbuf));

	BYTE *pReview = m_pReview1[iSeq];
	BYTE *pFire = m_pReview2[iSeq];

	if( pReview==NULL || pFire ==NULL )
	{
// 		BHC_MEMO("InspectReview seq=%d...no buffer", iSeq);
		return false;
	}


	//
	// make image
	//
	// Review 영상
	HImage hiReview_color;
	MakeHImage_color(pReview, m_iWidth_review1, m_iHeight_review1,&hiReview_color);

	HImage hiReview = hiReview_color.Rgb1ToGray();


	// 열화상 영상
	HImage hiFire;
	MakeHImage(pFire, m_iWidth_review2, m_iHeight_review2, &hiFire );
	int iWidth_fire = m_iWidth_review2;
	int iHeight_fire = m_iHeight_review2;




	//
	//
	//
	HTuple rows_normal, cols_normal, rows_special, cols_special;
	rows_normal[0] = 1660;
	rows_normal[1] = 601;
	cols_normal[0] = 730;
	cols_normal[1] = 1755;
	rows_special[0] = 135;
	rows_special[1] = 272;
	cols_special[0] = 372;
	cols_special[1] = 223;

	rows_normal[0] = 1593;
	rows_normal[1] = 220;
	cols_normal[0] = 1960;
	cols_normal[1] = 405;
	rows_special[0] = 139;
	rows_special[1] = 335;
	cols_special[0] = 240;
	cols_special[1] = 446;






// 	HTuple htAffine;
// 	VectorToSimilarity( rows_special, cols_special, rows_normal, cols_normal, &htAffine );
	HHomMat2D hmAffine;
	hmAffine.VectorToSimilarity(rows_special, cols_special, rows_normal, cols_normal);




	//
	// review #2 - special camera
	//
	HRegion hrCandidates_special;
	{
		int wid = m_iWidth_review2;
		int hgt = m_iHeight_review2;
		HImage hiFire_median = hiFire.MedianImage("circle", 1, "mirrored");

		HImage hiGauss1;
		hiGauss1.GenGaussFilter(0,
								3.0,
								0.0,
								"none",
								"rft",
								iWidth_fire,
								iHeight_fire );

		HImage hiGauss2;
		hiGauss2.GenGaussFilter(0,
								10.0,
								0.0,
								"none",
								"rft",
								iWidth_fire,
								iHeight_fire );


		HImage hiFilter = hiGauss1.SubImage(hiGauss2, 1,0 );


		//
		HImage hiFFT = hiFire_median.RftGeneric("to_freq", "none", "complex", iWidth_fire);
		HImage hiFFT_convol = hiFFT.ConvolFft(hiFilter);
		HImage hiFFT_convol_invFft = hiFFT_convol.RftGeneric( "from_freq", "n", "real", iWidth_fire );

		HImage hiFFT_convol_invFft_affine = hiFFT_convol_invFft.AffineTransImageSize( hmAffine, "bilinear", m_iWidth_review1, m_iHeight_review1 );

// 		WriteImage(hiFFT_convol_invFft, "hobj", 0, "d:\\test\\1");
// 		WriteImage(hiFFT_convol_invFft_affine, "hobj", 0, "d:\\test\\2");

		HRegion hrThreshold = hiFFT_convol_invFft_affine.InvertImage().HysteresisThreshold(2, 4, 10 * 7).ClosingCircle(3.5);	// dark
// 		HRegion hrThreshold = hiFFT_convol_invFft.InvertImage().HysteresisThreshold(2, 4, 10).ClosingCircle(3.5);	// dark


		HRegion hrCandidates = hrThreshold.Connection();


		HTuple paramShape, paramMin, paramMax;
		paramShape[0] = "width";
		paramMin[0] = 20;
		paramMax[0] = m_iWidth_review1/2;

		paramShape[1] = "height";
		paramMin[1] = 20;
		paramMax[1] = m_iHeight_review1/2;

		hrCandidates_special = hrCandidates.SelectShape(paramShape, "and", paramMin, paramMax);
		hrCandidates_special = hrCandidates;


		//
// 		HImage hiFire_affine = hiFire.AffineTransImageSize( hmAffine, "constant", m_iWidth_review1, m_iHeight_review1 );


// 		ostringstream ost;
// 		ost<<"d:\\test\\"<<iSeq<<"special";
// 		SAVE_IMAGE(hiFire, &hrCandidates_filter, NULL, NULL, ost.str());
	}


	//
	// review #1 - normal camera
	//
	HRegion hrCandidates_normal;
	{
		int wid = m_iWidth_review1;
		int hgt = m_iHeight_review1;
		HImage himedian = hiReview.MedianImage("circle", 1, "mirrored");

		HImage hiGauss1;
		hiGauss1.GenGaussFilter(0,
								3.0,
								3.0,
								"none",
								"rft",
								wid,
								hgt );

		HImage hiGauss2;
		hiGauss2.GenGaussFilter(0,
								10.0,
								10.0,
								"none",
								"rft",
								wid,
								hgt );


		HImage hiFilter = hiGauss1.SubImage(hiGauss2, 1,0 );


		//
		HImage hiFFT = himedian.RftGeneric("to_freq", "none", "complex", wid);
		HImage hiFFT_convol = hiFFT.ConvolFft(hiFilter);
		HImage hiFFT_convol_invFft = hiFFT_convol.RftGeneric( "from_freq", "n", "real", wid );


		HRegion hrThreshold = hiFFT_convol_invFft.InvertImage().HysteresisThreshold(2, 4, 10).ClosingCircle(3.5);	// dark


		HRegion hrCandidates = hrThreshold.Connection();


		HTuple paramShape, paramMin, paramMax;
		paramShape[0] = "width";
		paramMin[0] = 100/1.75 * 0.8;
		paramMax[0] = 10000;	//500/1.75;

		paramShape[1] = "height";
		paramMin[1] = 100/1.75 * 0.8;
		paramMax[1] = 10000;	//500/1.75;

		hrCandidates_normal = hrCandidates.SelectShape(paramShape, "and", paramMin, paramMax);


// 		ostringstream ost;
// 		ost<<"d:\\test\\"<<iSeq<<"normal";
// 		SAVE_IMAGE(hiReview, &hrCandidates_filter, NULL, NULL, ost.str());
	}


	//
	//
	//
	bool bDefect = false;
	for( int i=0 ; i<hrCandidates_normal.CountObj() ; i++ )
	{
		HRegion hrInterest = hrCandidates_normal.SelectObj(i+1);

		HRegion hrIntersection = hrInterest.Intersection(hrCandidates_special);

// 		HRegion hrTemp;
// 		hrTemp.GenRectangle1(0,0,m_iHeight_review1, m_iWidth_review1);
// 		HRegion hrIntersection = hrInterest.Intersection(hrTemp);

		HTuple row,col;
		hrIntersection.GetRegionPoints(&row, &col);

		if( row.Length() == 0 )
		{
			bDefect = true;
		}
	}


	if( bDefect )
	{
		m_resullt[iSeq] = true;

		ostringstream ostr;
		ostr<<"d:\\test\\"<<iSeq<<"special-NG";
		HImage hiFire_affine = hiFire.AffineTransImageSize( hmAffine, "constant", m_iWidth_review1, m_iHeight_review1 );
		SAVE_IMAGE(hiFire_affine, &hrCandidates_special, NULL, NULL, ostr.str());


		ostr.str("");
		ostr.clear();
		ostr<<"d:\\test\\"<<iSeq<<"normal-NG";
		SAVE_IMAGE(hiReview, &hrCandidates_normal, NULL, NULL, ostr.str());
	}
	else 
	{
		m_resullt[iSeq] = false;

		ostringstream ostr;
		ostr<<"d:\\test\\"<<iSeq<<"special-OK";
		HImage hiFire_affine = hiFire.AffineTransImageSize( hmAffine, "constant", m_iWidth_review1, m_iHeight_review1 );
		SAVE_IMAGE(hiFire_affine, &hrCandidates_special, NULL, NULL, ostr.str());


		ostr.str("");
		ostr.clear();
		ostr<<"d:\\test\\"<<iSeq<<"normal-OK";
		SAVE_IMAGE(hiReview, &hrCandidates_normal, NULL, NULL, ostr.str());
	}


	if( m_pReview1[iSeq] )
	{
		delete[] m_pReview1[iSeq];
	}
	m_pReview1[iSeq] = NULL;

	if( m_pReview2[iSeq] )
	{
		delete[] m_pReview2[iSeq];
	}
	m_pReview2[iSeq] = NULL;


// 	BHC_MEMO("InspectReview seq=%d...OK", iSeq);


	return true;
}


bool CImageProcess_H::FindNearestRegionIndex( const HalconCpp::HRegion hrIn, double dX_target, double dY_target, int *pIndex )
{
	// Region들의 중심좌표 획득
	HTuple htRows_in, htCols_in;
	hrIn.AreaCenter( &htRows_in, &htCols_in );	

	int iCnt	= htRows_in.TupleLength();

	if( iCnt==0 )
	{
		return false;
	}


	// targe points
	HTuple htRows_target, htCols_target;
	TupleGenConst(iCnt, HTuple(dX_target), &htCols_target );
	TupleGenConst(iCnt, HTuple(dY_target), &htRows_target );


	// 거리계산
	HTuple htDistance;
	DistancePp( htRows_in, htCols_in, htRows_target, htCols_target, &htDistance);


	// sort distance
	HTuple htSort = htDistance.TupleSortIndex();


	//
	*pIndex = htSort[0] + 1;


	return true;
}
*/