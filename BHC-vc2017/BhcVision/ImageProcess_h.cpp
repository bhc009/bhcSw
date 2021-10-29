// ImageProcess_H.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//
#define DLLEXPORT_H

#include "ImageProcess_h.h"

#include "halconcpp/HalconCpp.h"
using namespace HalconCpp;

// #include <atlconv.h>

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #endif

HalconCpp::HShapeModel m_hsTemplate;		// template
HalconCpp::HShapeModel g_hsTemplates[4];	// templates

HalconCpp::HImage m_hiROI_active;
HalconCpp::HImage m_hiROI_bm;
HalconCpp::HImage m_hiROI_cameraHole;
HalconCpp::HImage m_hiROI_speaker;
HalconCpp::HImage m_hiTemplateImage;	// old

HalconCpp::HTuple g_htRows_boundarys[200];
HalconCpp::HTuple g_htCols_boundarys[200];

HalconCpp::HImage g_hiROI[2][14];
HalconCpp::HImage g_hiInput[2][14];

int g_test;
// HalconCpp::HHomMat2D m_hhmTemplateToModel;	// template image to template model


CImageProcess_H::CImageProcess_H(void)
{
	g_test = 0;
	m_dResolution_x = 0.001725;	//	mm/pixel
	m_dResolution_y = 0.001725;	//	mm/pixel

	//
	// align
	//
	for (int i = 0; i < 6; i++)
	{
		m_dAffineMatirx_worldToLocal[i] = 0.0;
		m_dAffineMatirx_localToWorld[i] = 0.0;
	}


	m_bDisplay = false;;
	m_dDisplayRatio = 0.5;


	m_bSave = false;


	for( int i=0 ; i<_BHC_IP_REVIEW_NUM_ ; i++ )
	{
		m_pReview1[i] = NULL;
		m_pReview2[i] = NULL;
	}


	for (int i = 0; i < 4; i++)
	{
		m_dTemplatePosition_rows[i] = 0.0;
		m_dTemplatePosition_cols[i] = 0.0;
	}


	m_iBoundaryNum = 0;
	m_pBoundaryList_x = NULL;
	m_pBoundaryList_y = NULL;
}


CImageProcess_H::~CImageProcess_H()
{
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
	}

	// delete x array
	if (m_pBoundaryList_x)
	{
		delete[] m_pBoundaryList_x;
	}
	m_pBoundaryList_x = NULL;

	// delete y array
	if (m_pBoundaryList_y)
	{
		delete[] m_pBoundaryList_y;
	}
	m_pBoundaryList_y = NULL;
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

//	w.CloseWindow();
}


void CImageProcess_H::SetShowImage(bool bShow)
{
	m_bDisplay = bShow;
}


void CImageProcess_H::SetShowRatio( double dRatio )
{
	m_dDisplayRatio = dRatio;
}


int CImageProcess_H::setGlobal(int a)
{
	g_test = a;

	return 0;
}
int CImageProcess_H::getGlobal()
{
	return g_test;
}


//////////////////////////////////////////////////////////////////////////
//
// Make himage(color)
//
//	- BGR 순서
//
//////////////////////////////////////////////////////////////////////////
bool MakeHImage_color(BYTE *pInputData, int iWidth, int iHeight, HImage* outImage)
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
bool MakeHImage_color(BYTE *pInputData, int iWidth, int iHeight, HImage* hiRed, HImage* hiGreen, HImage* hiBlue)
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


//////////////////////////////////////////////////////////////////////////
//
// 영상 저장
//
//////////////////////////////////////////////////////////////////////////
void SAVE_IMAGE(const HImage & image, HRegion * pRegion, std::list<HXLDCont> *pListHx, std::list<stDefectInfo> *pListInfo, std::string strFileName, int iType )
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
//	w.SetFont(HString("default-Normal-48"));
//	w.SetColor("green");


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
		for (std::list<HXLDCont>::iterator ite = pListHx->begin(); ite != pListHx->end(); ite++)
		{
			w.DispXld(*ite);
		}
	}



	//
	// Draw text
	//
	if (pListInfo)
	{
		for (std::list<stDefectInfo>::iterator ite = pListInfo->begin(); ite != pListInfo->end(); ite++)
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

	if(iType==-1 )
	{
		img.WriteImage("hobj", 0, strFileName.c_str());
	} else {
		img.WriteImage("bmp", 0, strFileName.c_str());
	}

//	w.CloseWindow();
}



//////////////////////////////////////////////////////////////////////////
//
// Make himage(1ch)
//
//	- BGR 순서
//
//////////////////////////////////////////////////////////////////////////
bool MakeHImage(unsigned char *pInputData, int iWidth, int iHeight, HImage* outImage)
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
// GetIndex_biggest
//
//	가장 큰것의 index 획득
//
//
//////////////////////////////////////////////////////////////////////////
bool GetIndex_biggest( HRegion *pHrIn, int *iIndex )
{
	if( pHrIn->CountObj() < 1 )
	{
		return false;
	}


	HTuple htArea = pHrIn->Area();

	HTuple htSortedIndex = htArea.TupleSortIndex();


	*iIndex = htSortedIndex[htSortedIndex.Length() - 1];


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	PitchCompare
//
//	pResult : 함수 외부에서 버퍼 생성 필요
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::PitchCompare(unsigned char *pImage, int iWidth, int iHeight, int iDx, int iDy, unsigned char *pResult)
{
	if (iWidth < 1 || iHeight < 1 || iDx < 0 || iDy < 0)
	{
		return false;
	}

	BYTE *pImg = pImage;
	int iWid = iWidth;
	int iHgt = iHeight;

	HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;

	// 자체 영상 로딩
// 	HImage hiIn;
// 	hiIn.ReadImage("d:\\test\\img.bmp");
// 
// 	HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;
// 	GetImagePointer1(hiIn, &hv_Pointer, &hv_Type, &hv_Width1, &hv_Height1);
// 
// 	BYTE *pImg = (LPBYTE)hv_Pointer.L();
// 	int iWid = hv_Width1;
// 	int iHgt = hv_Height1;

	//
	// info
	//
	int iWidth_roi = iWid;
	int iHeight_roi = iHgt - 2*(iDy + 1);

	int iFirstStep = iWid * (iDy + 1);

	BHC_MEMO("start");
	//
	// make pitch images
	//

	// center
	HImage hiROI_c;
	hiROI_c.GenImage1("byte", iWidth_roi, iHeight_roi, pImg + iFirstStep);

	if( m_bSave )
	{
		hiROI_c.WriteImage("bmp", 0, "d:\\test\\pitch_c");
	}

	// left
	HImage hiROI_l;
	hiROI_l.GenImage1("byte", iWidth_roi, iHeight_roi, pImg + iFirstStep - iDx );

	if (m_bSave)
	{
		hiROI_l.WriteImage("bmp", 0, "d:\\test\\pitch_l");
	}

	// right
	HImage hiROI_r;
	hiROI_r.GenImage1("byte", iWidth_roi, iHeight_roi, pImg + iFirstStep + iDx);

	if (m_bSave)
	{
		hiROI_r.WriteImage("bmp", 0, "d:\\test\\pitch_r");
	}

	// top
	HImage hiROI_t;
	hiROI_t.GenImage1("byte", iWidth_roi, iHeight_roi, pImg + iFirstStep - iDy* iWid);

	if (m_bSave)
	{
		hiROI_t.WriteImage("bmp", 0, "d:\\test\\pitch_t");
	}

	// bottom
	HImage hiROI_b;
	hiROI_b.GenImage1("byte", iWidth_roi, iHeight_roi, pImg + iFirstStep + iDy * iWid);

	if (m_bSave)
	{
		hiROI_b.WriteImage("bmp", 0, "d:\\test\\pitch_b");
	}



	BHC_MEMO("start2");
	//
	// make diff
	//
	// 차이값의 최소
// 	HImage hiDiff_lr = hiROI_c.SubImage( hiROI_l.AddImage(hiROI_r, 0.5, 0), 1, 127 );
// 	HImage hiDiff_tb = hiROI_c.SubImage(hiROI_t.AddImage(hiROI_b, 0.5, 0), 1, 127);

	// 절대차이값의 최소
	HImage hiDiff_lr = hiROI_c.AbsDiffImage(hiROI_l.AddImage(hiROI_r, 0.5, 0), 1);
	HImage hiDiff_tb = hiROI_c.AbsDiffImage(hiROI_t.AddImage(hiROI_b, 0.5, 0), 1);

	if (m_bSave)
	{
		hiDiff_lr.WriteImage("bmp", 0, "d:\\test\\diff_lr");
		hiDiff_tb.WriteImage("bmp", 0, "d:\\test\\diff_tb");
	}


	//
	// min
	//
	HImage hiMinDiff = hiDiff_lr.MinImage(hiDiff_tb).ScaleImage(1, 127);

	if (m_bSave)
	{
		hiMinDiff.WriteImage("bmp", 0, "d:\\test\\min diff");
	}



	//
	// copy to result buffer
	//
	GetImagePointer1(hiMinDiff, &hv_Pointer, &hv_Type, &hv_Width1, &hv_Height1);
	BYTE *pRst = (LPBYTE)hv_Pointer.L();

	memcpy(pResult + 2 * (iDy + 1), pRst, iWidth_roi*iHeight_roi);


	BHC_MEMO("end");


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	Preprocessing_median
//
//	Median filter를 이용한 전처리
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::Preprocessing_median(unsigned char *pImage, int iWidth, int iHeight, int iMaskWidth, unsigned char *pResult)
{
	if (iWidth < 1 || iHeight < 1 || iMaskWidth < 3 )
	{
		return false;
	}

	HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;


	BHC_MEMO("start");


	// input image
	HImage hiIn;
	hiIn.GenImage1("byte", iWidth, iHeight, pImage);


	HImage hiMedian = hiIn.MedianRect(iMaskWidth, 1);

	HImage hiResult = hiIn.SubImage(hiMedian, 2, 128);


	//
	// copy to result buffer
	//
	GetImagePointer1(hiResult, &hv_Pointer, &hv_Type, &hv_Width1, &hv_Height1);
	BYTE *pRst = (LPBYTE)hv_Pointer.L();

	memcpy(pResult, pRst, iWidth*iHeight);


	BHC_MEMO("end");

	return true;
}

bool CImageProcess_H::Preprocessing_median(const char* cPath, const char* cName)
{
	std::string strPath(cPath);
	std::string strName(cName);


	std::string strFullName = strPath + "//" + strName;


	HImage hiIn;
	hiIn.ReadImage(strFullName.c_str());


	//
	// processing
	//
	HImage hiMedian = hiIn.MedianRect(15, 15);

	HImage hiResult = hiIn.SubImage(hiMedian, 2, 128);



	//
	// save
	//
	int indexDot = strName.find(".", 0);
	std::string strNameToSave = strName.substr(0, indexDot);

	std::string strFullNameToSave = "d://test//" + strNameToSave;

	hiResult.WriteImage("jpg", 0, strFullNameToSave.c_str());


	return true;
}


bool CImageProcess_H::Preprocessing_normalize(const char* cPath, const char* cName)
{
	std::string strPath(cPath);
	std::string strName(cName);


	std::string strFullName = strPath + "//" + strName;


	HImage hiIn;
	hiIn.ReadImage(strFullName.c_str());


	//
	// processing
	//
	HImage hiMedian = hiIn.MedianRect(15, 15);

	HImage hiResult = hiIn.DivImage(hiMedian, 128, 0);



	//
	// save
	//
	int indexDot = strName.find(".", 0);
	std::string strNameToSave = strName.substr(0, indexDot);

	std::string strFullNameToSave = "d://test//" + strNameToSave;

	hiResult.WriteImage("jpg", 0, strFullNameToSave.c_str());


	return true;
}


bool CImageProcess_H::MakeBmpImage(const char* cPath, const char* cName)
{
	std::string strPath(cPath);
	std::string strName(cName);


	std::string strFullName = strPath + "//" + strName;


	HImage hiIn;
 	hiIn.ReadImage(strFullName.c_str());


	int indexDot =strName.find(".", 0);
	std::string strNameToSave = strName.substr(0, indexDot);

	std::string strFullNameToSave = "d://test//" + strNameToSave;

	hiIn.WriteImage("bmp", 0, strFullNameToSave.c_str());


	return true;
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
*/


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
bool CImageProcess_H::SetAlignImage_line(unsigned char *pImage, int iWidth, int iHeight, int iPos, double dCx, double dCy, int iDirX, int iDirY)
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
// 	DISPLAY(hiGray, &hrThreshold, NULL, NULL, "threshold", m_dDisplayRatio);


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
bool CImageProcess_H::SetAlignImage_line_color(unsigned char *pImage, int iWidth, int iHeight, int iPos, double dCx, double dCy, int iDirX, int iDirY, int iSaveIndex)
{

	double dCx_image = iWidth / 2.0;
	double dCy_image = iHeight / 2.0;


	//
	// convert image type : c++ > halcon
	//
	HImage hiColor;
	MakeHImage_color( pImage, iWidth, iHeight, &hiColor );

	HImage hiGray = hiColor.Rgb1ToGray();


// 	ostringstream ostr;
// 	ostr<<"d:\\test\\input"<<iSaveIndex;
// 	SAVE_IMAGE(hiColor, NULL, NULL,NULL,std::string(ostr.str()));


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
// 	DISPLAY(hiGray, &hrThreshold, NULL, NULL, "threshold", m_dDisplayRatio);


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
// 	DISPLAY(hiGray, NULL, &hxLine, NULL, "final line", m_dDisplayRatio);



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

// 	ostr.str("");
// 	ostr.clear();
// 	ostr<<"d:\\test\\result"<<iSaveIndex;
// 	SAVE_IMAGE(hiColor, NULL, &hxList, NULL,std::string(ostr.str()));


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

	bOk = CalcCorner(m_dX_AlignLine_left_local[0],	m_dY_AlignLine_left_local[0],
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
	bOk = CalcCorner(m_dX_AlignLine_right_local[0],	m_dY_AlignLine_right_local[0],
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

	HalconCpp::HTuple htCoordinateMatrix_worldToLocal;
	VectorToSimilarity(htRows_world, htCols_world, htRows_local, htCols_local, &htCoordinateMatrix_worldToLocal);

	HalconCpp::HTuple htCoordinateMatrix_localToWorld;
	VectorToSimilarity(htRows_local, htCols_local, htRows_world, htCols_world, &htCoordinateMatrix_localToWorld);


	int nData = htCoordinateMatrix_worldToLocal.TupleLength();
	// 	std::string strMemo;
	// 	sprintf_s( (char*)strMemo.c_str(), "similarity matrix : #element = %d", nData );

// 	double a = htCoordinateMatrix_localToWorld[0];
// 	double b = htCoordinateMatrix_localToWorld[1];
// 	double c = htCoordinateMatrix_localToWorld[2];
// 	double d = htCoordinateMatrix_localToWorld[3];
// 	double e = htCoordinateMatrix_localToWorld[4];
// 	double f = htCoordinateMatrix_localToWorld[5];

	for( int i=0 ; i<6 ; i++ )
	{
		m_dAffineMatirx_worldToLocal[i] = htCoordinateMatrix_worldToLocal[i];
		m_dAffineMatirx_localToWorld[i] = htCoordinateMatrix_localToWorld[i];
	}

	return true;
}



bool CImageProcess_H::GetCoodinate(double dX_world, double dY_wordl, double *dX_stage, double *dY_stage, int iDir)
{
	HTuple htRow, htCol;

	if (iDir == _BHC_IP_LOCAL_TO_WORLD_)
	{
		HTuple htCoordinateMatrix_localToWorld;
		for( int i=0 ; i<6 ; i++ )
		{
			htCoordinateMatrix_localToWorld[i] = m_dAffineMatirx_localToWorld[i];
		}
		AffineTransPoint2d(htCoordinateMatrix_localToWorld, HTuple(dY_wordl), HTuple(dX_world), &htRow, &htCol);
	}



	if (iDir == _BHC_IP_WORLD_TO_LOCAL_)
	{
		HTuple htCoordinateMatrix_worldToLocal;
		for( int i=0 ; i<6 ; i++ )
		{
			htCoordinateMatrix_worldToLocal[i] = m_dAffineMatirx_worldToLocal[i];
		}
		AffineTransPoint2d(htCoordinateMatrix_worldToLocal, HTuple(dY_wordl), HTuple(dX_world), &htRow, &htCol);
	}



	*dX_stage = htCol;
	*dY_stage = htRow;

	return false;
}

/*
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
*/


double CImageProcess_H::GetMatrix(int i)
{
	return m_dAffineMatirx_worldToLocal[i];
}



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


	BHC_MEMO("CImageProcess_H::LoadTemplate > end");

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	LoadTemplate
//
//	ㅇ 다음 항목 로딩
//		- template
//		- active area
//		- bm area
//		- ear hole
//		- speaker hole
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::LoadTemplate(const char* cName_Template, const char* cName_AA, const char* cName_bm, const char* cName_ear, const char* cName_speaker)
{
	BHC_MEMO("CImageProcess_H::LoadTemplate > start");

	std::string strFile_template(cName_Template);
	std::string strFile_activeROI(cName_AA);
	std::string strFile_bmROI(cName_bm);
	std::string strFile_earROI(cName_ear);
	std::string strFile_speakerROI(cName_speaker);


	BHC_MEMO("CImageProcess_H::LoadTemplate > template loading");
	// Load template model from file
	m_hsTemplate.ReadShapeModel(strFile_template.c_str());


	BHC_MEMO("CImageProcess_H::LoadTemplate > active ROI loading");
	// Load template image from image
	m_hiROI_active.ReadImage(strFile_activeROI.c_str());


	BHC_MEMO("CImageProcess_H::LoadTemplate > bm ROI loading");
	// Load template image from image
	m_hiROI_bm.ReadImage(strFile_bmROI.c_str());


	BHC_MEMO("CImageProcess_H::LoadTemplate > ear hole ROI loading");
	// Load template image from image
	m_hiROI_cameraHole.ReadImage(strFile_earROI.c_str());


	BHC_MEMO("CImageProcess_H::LoadTemplate > speaker ROI loading");
	// Load template image from image
	m_hiROI_speaker.ReadImage(strFile_speakerROI.c_str());


	BHC_MEMO("CImageProcess_H::LoadTemplate > end");


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
//	LoadTemplate
//
//	ㅇ 다음 항목 로딩
//		- template #1
//		- template #2
//		- template #3
//		- template #4
//		- ROI 영상에서 template의 좌표(row) 
//		- ROI 영상에서 template의 좌표(col)
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::LoadTemplate(const char* pFilePath_ltTemplate, const char* pFilePath_rtTemplate, const char* pFilePath_lbTemplate, const char* pFilePath_rbTemplate, const char* pFilePath_rows, const char* pFilePath_cols)
{
	BHC_MEMO("CImageProcess_H::LoadTemplate > start");

	std::string strFile_template_lt(pFilePath_ltTemplate);
	std::string strFile_template_rt(pFilePath_rtTemplate);
	std::string strFile_template_lb(pFilePath_lbTemplate);
	std::string strFile_template_rb(pFilePath_rbTemplate);
	std::string strFile_rows(pFilePath_rows);
	std::string strFile_cols(pFilePath_cols);


	BHC_MEMO("CImageProcess_H::LoadTemplate > template loading #1");
	// Load template model from file
	// !로딩 실패 시 에러처리 추가 필요
	g_hsTemplates[0].ReadShapeModel(strFile_template_lt.c_str());

	BHC_MEMO("CImageProcess_H::LoadTemplate > template loading #2");
	// Load template model from file
	// !로딩 실패 시 에러처리 추가 필요
	g_hsTemplates[1].ReadShapeModel(strFile_template_rt.c_str());


	BHC_MEMO("CImageProcess_H::LoadTemplate > template loading #3");
	// Load template model from file
	// !로딩 실패 시 에러처리 추가 필요
	g_hsTemplates[2].ReadShapeModel(strFile_template_lb.c_str());


	BHC_MEMO("CImageProcess_H::LoadTemplate > template loading #4");
	// Load template model from file
	// !로딩 실패 시 에러처리 추가 필요
	g_hsTemplates[3].ReadShapeModel(strFile_template_rb.c_str());


	BHC_MEMO("CImageProcess_H::LoadTemplate > position loading");
	// Load position of template
	HTuple htRows;
	ReadTuple(pFilePath_rows, &htRows);

	HTuple htCols;
	ReadTuple(pFilePath_cols, &htCols);

	if (htRows.Length() != 4 || htCols.Length() != 4)
	{
		return false;
	}

	for (int i = 0; i < 4; i++)
	{
		m_dTemplatePosition_rows[i] = htRows[i];
		m_dTemplatePosition_cols[i] = htCols[i];
	}


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
	if( m_bSave )
	{
		hiIn.WriteImage("bmp", 0, "d:\\test\\FindTemplate - input");
	}


	BHC_MEMO("CImageProcess_H::FindTemplate > find template");
	// find template
	HTuple htRow, htCol, htAngle, htScore;
	m_hsTemplate.FindShapeModel(hiIn,
								-0.39,
								0.79,
								0.3,		// min score
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
	double dScore = htScore[0];
	BHC_MEMO("CImageProcess_H::FindTemplate > score > %.2f", dScore);


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

	std::list<HalconCpp::HXLDCont> hxSave;
	hxSave.push_back(hxTemplate_matching);

// 	SAVE_IMAGE(hiIn, NULL, &hxSave, NULL, "d:\\test\\Template_found");
	BHC_MEMO("CImageProcess_H::LoadTemplate > show result 2");
	// show result 2 : ROI
	if( m_bSave )
	{
		SAVE_IMAGE(hiROI, NULL, &hxSave, NULL, "d:\\test\\FindTemplate - output", 0);
	}



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
bool CImageProcess_H::FindTemplate(unsigned char *pData, int iWidth, int iHeight, unsigned char *pActive, unsigned char *pBm, unsigned char *pCamera, unsigned char *pSpeaker)
{
	BHC_MEMO("CImageProcess_H::FindTemplate > start");


	//
	// 1. template matching
	//

	// 이미지 type 변경 : c++ -> halcon
	HImage hiIn("byte", iWidth, iHeight, pData);
	if (m_bSave)
	{
		hiIn.WriteImage("bmp", 0, "d:\\test\\FindTemplate - input");
	}


	BHC_MEMO("CImageProcess_H::FindTemplate > find template");
	// find template
	HTuple htRow, htCol, htAngle, htScore;
	m_hsTemplate.FindShapeModel(hiIn,
								-0.39,
								0.79,
								0.3,		// min score
								1,			// Num match
								0.5,		// max overlap
								"none",		// subpixel
								0,			// num level
								0.9,		// greediness
								&htRow,
								&htCol,
								&htAngle,
								&htScore);


	// check error
	int iNumResult = htRow.Length();
	if (iNumResult < 1)
	{
		BHC_MEMO("CImageProcess_H::FindTemplate > fail > no result");
		return false;
	}
	double dScore = htScore[0];
	BHC_MEMO("CImageProcess_H::FindTemplate > score > %.2f", dScore);



	BHC_MEMO("CImageProcess_H::LoadTemplate > calc relation");
	//
	// 2. calculate relation
	//

	// 1) model to input
	HHomMat2D hhmModelToInput;
	hhmModelToInput.VectorAngleToRigid(0, 0, 0, htRow[0], htCol[0], htAngle[0]);

	// 2) ROI template to model
	// read file 
	HFile hfSave;
	hfSave.OpenFile(HTuple("d:\\ROI_relation.mat"), HTuple("input_binary"));	// file open

	HSerializedItem hsData;
	hsData.FreadSerializedItem(hfSave);	// read data

	HHomMat2D hhmTemplateToModel;
	hhmTemplateToModel.DeserializeHomMat2d(hsData);	// set matrix

	// 3) ROI template to input
	HHomMat2D hhmTemplateToInput = hhmModelToInput.HomMat2dCompose(hhmTemplateToModel);



	BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image");
	//
	// 3. make ROI inages
	//

	// AA ROI
	BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image AA");
	if (pActive)
	{
		// 버전 1 : ROI 영상을 이용한 영상 생성
/*		HImage hiROI = m_hiROI_active.AffineTransImage(hhmTemplateToInput, "constant", "false");
		BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image 1");

		HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;
		GetImagePointer1(hiROI, &hv_Pointer, &hv_Type, &hv_Width1, &hv_Height1);

		BYTE *pRoi = (LPBYTE)hv_Pointer.L();
		BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image 2");

		memcpy(pActive, pRoi, iWidth*iHeight);	// AA

		// save
		if (m_bSave)
		{
			//SAVE_IMAGE(hiROI, NULL, NULL, NULL, "d:\\test\\FindTemplate - ROI_AA", 0);
			hiROI.WriteImage("bmp", 0, "d:\\test\\FindTemplate - ROI_AA");
		}
		BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image 3");
*/

		// 버전 2 : dwg를 이용한 영상 생성
		// 
		HImage hiROI;
		hiROI.GenImageConst("byte", iWidth, iHeight);

		// load ROI template
		HXLDCont hxldROI_AA;
		HTuple htDxfStatus = hxldROI_AA.ReadContourXldDxf(HTuple("d:\\ROI_AA.dxf"), HTuple(), HTuple() );

		// transform
		HXLDCont hxldROI_AA_trans = hxldROI_AA.AffineTransContourXld(hhmTemplateToInput);

		// convert to region
		HRegion hrAA = hxldROI_AA_trans.GenRegionContourXld("filled");

		// make image
		HImage hiROI_AA = hrAA.PaintRegion(hiROI, 255, "fill");

		// copy to result
		HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;
		GetImagePointer1(hiROI_AA, &hv_Pointer, &hv_Type, &hv_Width1, &hv_Height1);

		BYTE *pRoi = (LPBYTE)hv_Pointer.L();

		memcpy(pActive, pRoi, iWidth*iHeight);	// BM

		// save
		if (m_bSave)
		{
			//SAVE_IMAGE(hiROI_AA, NULL, NULL, NULL, "d:\\test\\FindTemplate - ROI_AA", 0);
			hiROI_AA.WriteImage("bmp", 0, "d:\\test\\FindTemplate - ROI_AA");
		}
	}

	// BM ROI
	BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image BM");
	if (pBm)
	{
		//
		HImage hiROI = m_hiROI_bm.AffineTransImage(hhmTemplateToInput, "constant", "false");

		HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;
		GetImagePointer1(hiROI, &hv_Pointer, &hv_Type, &hv_Width1, &hv_Height1);

		BYTE *pRoi = (LPBYTE)hv_Pointer.L();
		BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image 1");

		memcpy(pBm, pRoi, iWidth*iHeight);	// BM
		BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image 2");

		// save
		if (m_bSave)
		{
			//SAVE_IMAGE(hiROI, NULL, NULL, NULL, "d:\\test\\FindTemplate - ROI_BM", 0);
			hiROI.WriteImage("bmp", 0, "d:\\test\\FindTemplate - ROI_BM");
		}
		BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image 3");

		/*
				HImage hiROI_AA;
				hiROI_AA.GenImageConst("byte", iWidth, iHeight);

				// load ROI template
				HXLDCont hxldROI_AA;
				HTuple htDxfStatus = hxldROI_AA.ReadContourXldDxf(HTuple("d:\\ROI_AA.dxf"), HTuple(), HTuple() );

				// transform
				HXLDCont hxldROI_AA_trans = hxldROI_AA.AffineTransContourXld(hhmTemplateToInput);

				// convert to region
				HRegion hrAA = hxldROI_AA_trans.GenRegionContourXld("filled");

				// make image
				hrAA.PaintRegion(hiROI_AA, 255, "fill");

				// save
				if (m_bSave)
				{
					SAVE_IMAGE(hiROI_AA, NULL, NULL, NULL, "d:\\test\\FindTemplate - ROI_AA", 0);
				}*/
	}

	// camera hole ROI
	BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image camera");
	if (pCamera)
	{
		//
/*		HImage hiROI = m_hiROI_cameraHole.AffineTransImage(hhmTemplateToInput, "constant", "false");

		HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;
		GetImagePointer1(hiROI, &hv_Pointer, &hv_Type, &hv_Width1, &hv_Height1);

		BYTE *pRoi = (LPBYTE)hv_Pointer.L();
		BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image 1");

		memcpy(pCamera, pRoi, iWidth*iHeight);	// BM
		BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image 2");

		// save
		if (m_bSave)
		{
			//SAVE_IMAGE(hiROI, NULL, NULL, NULL, "d:\\test\\FindTemplate - ROI_camera", 0);
			hiROI.WriteImage("bmp", 0, "d:\\test\\FindTemplate - ROI_camera");
		}
		BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image 3");
		*/

		HImage hiROI;
		hiROI.GenImageConst("byte", iWidth, iHeight);

		// load ROI template
		HXLDCont hxldROI_AA;
		HTuple htDxfStatus = hxldROI_AA.ReadContourXldDxf(HTuple("d:\\ROI_cHole.dxf"), HTuple(), HTuple());

		// transform
		HXLDCont hxldROI_AA_trans = hxldROI_AA.AffineTransContourXld(hhmTemplateToInput);

		// convert to region
		HRegion hrAA = hxldROI_AA_trans.GenRegionContourXld("filled");

		// make image
		HImage hiROI_camera = hrAA.PaintRegion(hiROI, 255, "fill");

		// copy to result
		HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;
		GetImagePointer1(hiROI_camera, &hv_Pointer, &hv_Type, &hv_Width1, &hv_Height1);

		BYTE *pRoi = (LPBYTE)hv_Pointer.L();

		memcpy(pCamera, pRoi, iWidth*iHeight);	// BM

		// save
		if (m_bSave)
		{
			//SAVE_IMAGE(hiROI_camera, NULL, NULL, NULL, "d:\\test\\FindTemplate - ROI_camera", 0);
			hiROI_camera.WriteImage("bmp", 0, "d:\\test\\FindTemplate - ROI_camera");
		}
	}

	// speaker hole ROI
	BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image speaker");
	if(pSpeaker )
	{
/*		//
		HImage hiROI = m_hiROI_speaker.AffineTransImage(hhmTemplateToInput, "constant", "false");

		HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;
		GetImagePointer1(hiROI, &hv_Pointer, &hv_Type, &hv_Width1, &hv_Height1);

		BYTE *pRoi = (LPBYTE)hv_Pointer.L();
		BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image 1");

		memcpy(pSpeaker, pRoi, iWidth*iHeight);	// BM
		BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image 2");

		// save
		if (m_bSave)
		{
			//SAVE_IMAGE(hiROI, NULL, NULL, NULL, "d:\\test\\FindTemplate - ROI_spearker", 0);
			hiROI.WriteImage("bmp", 0, "d:\\test\\FindTemplate - ROI_spearker");

		}
		BHC_MEMO("CImageProcess_H::LoadTemplate > make ROI image 3");
*/

		HImage hiROI;
		hiROI.GenImageConst("byte", iWidth, iHeight);

		// load ROI template
		HXLDCont hxldROI_AA;
		HTuple htDxfStatus = hxldROI_AA.ReadContourXldDxf(HTuple("d:\\ROI_sHole.dxf"), HTuple(), HTuple());

		// transform
		HXLDCont hxldROI_AA_trans = hxldROI_AA.AffineTransContourXld(hhmTemplateToInput);

		// convert to region
		HRegion hrAA = hxldROI_AA_trans.GenRegionContourXld("filled");

		// make image
		HImage hiROI_speaker = hrAA.PaintRegion(hiROI, 255, "fill");

		// copy to result
		HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;
		GetImagePointer1(hiROI_speaker, &hv_Pointer, &hv_Type, &hv_Width1, &hv_Height1);

		BYTE *pRoi = (LPBYTE)hv_Pointer.L();

		memcpy(pSpeaker, pRoi, iWidth*iHeight);	// BM

		// save
		if (m_bSave)
		{
//			SAVE_IMAGE(hiROI_speaker, NULL, NULL, NULL, "d:\\test\\FindTemplate - ROI_spearker", 0);
			hiROI_speaker.WriteImage("bmp", 0, "d:\\test\\FindTemplate - ROI_spearker");
		}
	}


/*
	BHC_MEMO("CImageProcess_H::LoadTemplate > show result");
	//
	// show
	//
	// show result 1 : matching
	HXLDCont hxTemplate = m_hsTemplate.GetShapeModelContours(1);
	HXLDCont hxTemplate_matching = hxTemplate.AffineTransContourXld(hhmModelToInput);

	std::list<HalconCpp::HXLDCont> hxSave;
	hxSave.push_back(hxTemplate_matching);

	if (m_bSave)
	{
		// 	SAVE_IMAGE(hiIn, NULL, &hxSave, NULL, "d:\\test\\Template_found");
	}
*/

	BHC_MEMO("CImageProcess_H::LoadTemplate > end");


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	FindTemplate
//
//	4개의 template를 이용하여 4개의 좌표쌍을 찾아서 affine 변환
//
//	[in] unsigned char *pData, : 검사 영상
//	[in] int iWidth, 영상 폭
//	[in] int iHeight, 영상 길이
//	[in/out] unsigned char *pROI, ROI 영상
//	[in] double dMinScore	template matching 시 score
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::FindTemplate(unsigned char *pData, int iWidth, int iHeight, unsigned char *pROI, double dMinScore)
{
	BHC_MEMO("CImageProcess_H::FindTemplate > start");

	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;
	std::list<HalconCpp::HXLDCont> hxSave;

	// 
	// test 용
	//
/*
	HImage hiInput1;
	hiInput1.ReadImage("d:\\test\\a.bmp");
	GetImagePointer1(hiInput1, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	pData = (LPBYTE)hv_Pointer.L();

	HImage hiInput2;
	hiInput2.ReadImage("d:\\ROI_AA.bmp");
	GetImagePointer1(hiInput2, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	pROI = (LPBYTE)hv_Pointer.L();

	iWidth = hv_Width;
	iHeight = hv_Height;
*/


	// 이미지 type 변경 : c++ -> halcon
	HImage hiIn("byte", iWidth, iHeight, pData);
	if (m_bSave)
	{
		hiIn.WriteImage("bmp", 0, "d:\\test\\FindTemplate-1_input");
	}

	//
	// 1. template matching
	//
	BHC_MEMO("CImageProcess_H::FindTemplate > find template");

	// matching
	HTuple htRows_target, htCols_target;
	for (int i = 0; i < 4; i++)
	{
		// ROI 설정
		HRegion hrROI;
		if (i == 0)
		{	// left top
			hrROI.GenRectangle1(0, 0, iHeight / 2, iWidth / 2);
		}
		else if (i == 1) { // right top
			hrROI.GenRectangle1(iWidth / 2, iWidth/2, iHeight / 2, iWidth -1);
		}
		else if (i == 2) { // left bottom
			hrROI.GenRectangle1(iHeight / 2, 0, iHeight -1, iWidth / 2);
		}
		else if (i == 3) { // right bottom
			hrROI.GenRectangle1(iHeight / 2, iWidth / 2, iHeight -1, iWidth - 1);
		}

		HImage hiReduce = hiIn.ReduceDomain(hrROI);	// ROI 설정

		// find template
		HTuple htRow, htCol, htAngle, htScore;
		g_hsTemplates[i].FindShapeModel(hiReduce,	//hiIn,	
										-0.39,
										0.79,
										dMinScore,		// min score 0.3
										1,			// Num match
										0.5,		// max overlap
										"none",		// subpixel
										0,			// num level
										0.9,		// greediness
										&htRow,
										&htCol,
										&htAngle,
										&htScore);


		// check error
		int iNumResult = htRow.Length();
		if (iNumResult < 1)
		{
			BHC_MEMO("CImageProcess_H::FindTemplate > fail > no result");
			return false;
		}

		// show score
		double dScore = htScore[0];
		BHC_MEMO("CImageProcess_H::FindTemplate > score > %.2f", dScore);

		// store result
		htRows_target[i] = htRow[0];
		htCols_target[i] = htCol[0];


		// * show info : mask tranform matrix
		HHomMat2D hmTrans;
		hmTrans.VectorAngleToRigid(0, 0, 0, htRow[0], htCol[0], htAngle[0]);

		// * show info : get contour
		HXLDCont hxTemplate = g_hsTemplates[i].GetShapeModelContours(1);
		HXLDCont hxTemplate_matching = hxTemplate.AffineTransContourXld(hmTrans);

		// * show info : add contour info
		hxSave.push_back(hxTemplate_matching);
	}


	// make affine transform
	HTuple htRows_src, htCols_src;
	for (int i = 0; i < 4; i++)
	{
		htRows_src[i] = m_dTemplatePosition_rows[i];
		htCols_src[i] = m_dTemplatePosition_cols[i];
	}

	HHomMat2D hmAffine;
	hmAffine.VectorToHomMat2d(htRows_src, htCols_src, htRows_target, htCols_target);


	// affine transform
	HImage hiROI("byte", iWidth, iHeight, pROI);
	HImage hiROI_tansform = hiROI.AffineTransImage(hmAffine, "nearest_neighbor", "false");

	if (m_bSave)
	{
		hiROI_tansform.WriteImage("bmp", 0, "d:\\test\\FindTemplate-2_ROI");
	}

	// copy to result
//	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;
	GetImagePointer1(hiROI_tansform, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	BYTE *pRoi_tranform = (LPBYTE)hv_Pointer.L();

//	memcpy(pROI, pRoi_tranform, iWidth * iHeight);


	BHC_MEMO("CImageProcess_H::LoadTemplate > show result");
	//
	// show
	//
	if (m_bSave)
	{
		SAVE_IMAGE(hiIn, NULL, &hxSave, NULL, "d:\\test\\FindTemplate-3_result", 0);
	}


	BHC_MEMO("CImageProcess_H::FindTemplate > end");


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	MakeTemplate
//
//	
//
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::MakeTemplate(unsigned char *pImage, int iWidth, int iHeight, std::list<int> *pListX, std::list<int> *pListY, int iSx, int iEx, int iPeriod, int iSeedY, int iIter)
{
	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;

	BHC_MEMO("CImageProcess_H::MakeTemplate > %d, %d, %d, %d, %d\n", iSx, iEx, iPeriod, iSeedY, iIter);

	// 
	// test 용
	//
	HImage hiInput1;
	hiInput1.ReadImage("d:\\test\\a.bmp");

	GetImagePointer1(hiInput1, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	pImage = (LPBYTE)hv_Pointer.L();

	iWidth = hv_Width;
	iHeight = hv_Height;


	//
	// flat image
	//
	unsigned char *pFlat = new unsigned char[iWidth*iHeight];

	if (!FlatImage(pImage, iWidth, iHeight,
		pFlat,
		iPeriod,	// 40,
		iSx, iEx,	// 2000, 6000,
		iSeedY,		// 5000,
		iIter))		// 10))
	{
		return false;
	}

	HImage hiFlat;
	hiFlat.GenImage1("byte", iWidth, iHeight, pFlat);

	if (m_bSave)
	{
		hiFlat.WriteImage("bmp", 0, "d:\\test\\MakeTemplate-flat.bmp");
	}


	//
	// make template
	//
	HTuple htRows, htCols;

	for( int i=0 ; i<4 ; i++ )
	{
		double dX, dY;

		char fileName[1024];
		_snprintf_s(fileName, 1024, "d:\\test\\Template_%d.hc", i);

		if (!MakeTemplateAndSave(pFlat, iWidth, iHeight,
			pListX[i],
			pListY[i],
			&dX,
			&dY,
			fileName))
// 			"d:\\test\\Template_lt.hc"))
		{
			delete[] pFlat;
			return false;
		}

		htRows[i] = dY;
		htCols[i] = dX;
	}

	htRows.WriteTuple("d:\\test\\templat-pos_x.hc");
	htCols.WriteTuple("d:\\test\\templat-pos_y.hc");



	delete[] pFlat;


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	FindRoi_BM
//
//	BM 영상에서 ROI 찾기
//
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::FindRoi_BM( unsigned char *pImage, int iWidth, int iHeight, unsigned char *pResult )
{
	int iStep = 48;
	int iThreshold = 50;



	//
	// Make himage
	//
	HImage hiIn;
	MakeHImage(pImage, iWidth, iHeight, &hiIn );



	BHC_MEMO("CImageProcess_H::FindRoi_BM > make background");
	//
	// make background image
	//
	HImage hiBack;
	hiBack.GenImageConst("byte", iWidth, iHeight );

	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;
	GetImagePointer1(hiBack, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	BYTE *pBack = (LPBYTE)hv_Pointer.L();

	for( int i=0 ; i<iStep ; i++ )
	{
		for( int y=i ; y<iHeight ; y+=iStep )
		{
			memcpy( pBack + y*iWidth, pImage + i*iWidth, iWidth );	// 한줄 복사
		}
	}

	SAVE_IMAGE( hiBack, NULL, NULL, NULL, "d:\\test\\FindRoi_BM-back", 0);



	BHC_MEMO("CImageProcess_H::FindRoi_BM > find BM candidates");
	//
	// find BM area
	//
	HRegion hrBMCandidate = hiIn.SubImage(hiBack, 1, 128 ).Threshold(0, 128 - iThreshold).Connection();	// sub & threshold & connection

	// 높이로 필터링
	HTuple paramShape, paramMin, paramMax;
	paramShape[0] = "height";
	paramMin[0] = iHeight/2;
	paramMax[0] = 2*iHeight;

	HRegion hrBMCandidate_filter = hrBMCandidate.SelectShape(paramShape, "and", paramMin, paramMax);

	// 영역이 가장 큰 것 선택
	int iIndex = 0;
	if( GetIndex_biggest( &hrBMCandidate_filter, &iIndex ) == false )
	{
		BHC_MEMO("CImageProcess_H::FindRoi_BM > error > no area");
	}

	HRegion hrBM = hrBMCandidate_filter.SelectObj(iIndex+1);

	SAVE_IMAGE( hiBack, &hrBM, NULL, NULL, "d:\\test\\FindRoi_BM-BM region", 0);



	BHC_MEMO("CImageProcess_H::FindRoi_BM > reshape BM area"); 
	//
	// 
	//
	HRegion hrBM_closing = hrBM.ClosingRectangle1(1, 48).FillUp();
	HRegion hrBM_closing_erosion = hrBM_closing.ErosionRectangle1(1, 600);

	// find MBR
	HTuple htRow, htCol, htRad, htLen1,htLen2;
	hrBM_closing_erosion.SmallestRectangle2(&htRow, &htCol, &htRad, &htLen1,&htLen2);

	// gen MBR
	HRegion hrResult;
	hrResult.GenRectangle2(htRow, htCol, htRad, htLen1,htLen2);

	SAVE_IMAGE( hiBack, &hrResult, NULL, NULL, "d:\\test\\FindRoi_BM-result region", 0);



	//
	// make result image
	//
	HTuple htRows, htCols;
	hrResult.GetRegionPoints(&htRows, &htCols);

	memset(pResult, 0, iWidth*iHeight);
	for( int i=0 ; i<htRows.Length() ; i++ )
	{
		int iPos = htCols[i] + htRows[i];
		pResult[iPos] = 255;
	}




	return true;
}



bool CheckRipple( HImage hiIn, HRegion hrROI, double dThreshold, double *pdAngle, int *pnArea, double *pdVar)
{
	BHC_MEMO("CheckRipple > crop");
	// crop
	HTuple htRow1, htCol1, htRow2, htCol2;
	hrROI.SmallestRectangle1( &htRow1, &htCol1, &htRow2, &htCol2 );
	HImage hiCrop = hiIn.CropRectangle1( htRow1, htCol1, htRow2, htCol2 );

	HTuple hlWidth, hlHeight;
	hiCrop.GetImageSize(&hlWidth, &hlHeight);



	BHC_MEMO("CheckRipple > make mask");
	// masking
	HRegion hrMasking_1;
	hrMasking_1.GenRectangle2(HTuple(hlHeight/2), HTuple(hlWidth/2), HTuple(0), HTuple(5), HTuple(hlWidth/2) );

	HRegion hrMasking_2;
	hrMasking_2.GenRectangle2(HTuple(hlHeight/2), HTuple(hlWidth/2), HTuple(0), HTuple(hlHeight/2), HTuple(5) );

	HRegion hrMasking = hrMasking_1.Union2(hrMasking_2);



	BHC_MEMO("CheckRipple > FFT");
	// FFT
	HImage hiFFT = hiCrop.FftGeneric("to_freq", -1, "sqrt", "dc_center", "complex");
	HImage hiFFT_power = hiFFT.PowerReal();
	HImage hiFFT_power_smooth = hiFFT_power.BinomialFilter(3,3);


// 	SAVE_IMAGE(hiFFT_power_smooth, NULL, NULL, NULL, "d:\\test\\CheckRipple-FFT", -1 );


	BHC_MEMO("CheckRipple > threshold");
	// threshold
	HRegion hrThrehold = hiFFT_power_smooth.Threshold(dThreshold, 99999999);

	HRegion hrThrehold_masking = hrThrehold.Difference(hrMasking);

// 	SAVE_IMAGE(hiFFT_power_smooth, &hrThrehold_masking, NULL, NULL, "d:\\test\\CheckRipple-threshold", -1 );



	BHC_MEMO("CheckRipple > get feature");
	// get feature
	*pdAngle = hrThrehold_masking.RegionFeatures("orientation")*180/3.14;
//	*pnArea = hrThrehold_masking.Area();

	double dXY = hrThrehold_masking.RegionFeatures("moments_m11");
	double dXX = hrThrehold_masking.RegionFeatures("moments_m20");
	double dYY = hrThrehold_masking.RegionFeatures("moments_m02");

	*pdVar = dXY / ( sqrt(dXX) * sqrt(dYY) );


	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// FindRipple
//
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::FindRipple( unsigned char *pImage, int iWidth, int iHeight, int iSx, int iSy, int iEx, int iEy, double dThr_angle, int iThr_area, double dThr_var, int *piCount )
{
	int iStep = 1000;


	HImage hiIn;
	MakeHImage(pImage, iWidth, iHeight, &hiIn);


	std::list<stDefectInfo> listDefect;

	int nCnt = 0;
	for( int y=iSy ; y<iEy-iStep ; y+=iStep)
	{
		for( int x=iSx ; x<iEx-iStep ; x+=iStep)
		{
			// make ROI
			HRegion hrROI;
			hrROI.GenRectangle1(y, x, y+iStep-1, x+iStep-1 );


			// check ripple
			double dAngle;
			double dVar;
			int iArea;

			CheckRipple( hiIn, hrROI, 20.0, &dAngle, &iArea, &dVar);

			stDefectInfo info;
			info.iLeft = x;
			info.iRight = x;
			info.iTop = y;
			info.iBottom = y;
			info.dCx = x;
			info.dCy = y;
			info.bValid = true;
			_snprintf( info.text, 1024, "Angle %.1f, area %d, var %.1f", dAngle, iArea, dVar );

			listDefect.push_back(info);



			if( iArea >= iThr_area && fabs(dVar) >= dThr_var )
			{
				if( dAngle < 0 + dThr_angle && dAngle > 0 - dThr_angle )
				{
					continue;
				}
				if( dAngle < 90 + dThr_angle && dAngle > 90 - dThr_angle )
				{
					continue;
				}
				if( dAngle < 180 + dThr_angle && dAngle > 180 - dThr_angle )
				{
					continue;
				}
				if( dAngle < -90 + dThr_angle && dAngle > -90 - dThr_angle )
				{
					continue;
				}
				if( dAngle < -180 + dThr_angle && dAngle > -180 - dThr_angle )
				{
					continue;
				}


				nCnt++;
			}



			BHC_MEMO(info.text);
		}
	}

	*piCount = nCnt;


	SAVE_IMAGE(hiIn, NULL, NULL, &listDefect, "d:\\test\\FindRipple", 0);


	return true;
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
bool CImageProcess_H::CalcCorner(double dX1, double dY1, double dX2, double dY2, double dX3, double dY3, double dX4, double dY4, double *pDx, double *pDy)
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



/*

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
*/




bool CImageProcess_H::SetReviewImage( unsigned char *pImage, int iWidth, int iHeight, int iDepth, int iCam, int iSeq )
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

// 		ostringstream ostr;
// 		ostr<<"d:\\test\\"<<iSeq<<"special-NG";
// 		HImage hiFire_affine = hiFire.AffineTransImageSize( hmAffine, "constant", m_iWidth_review1, m_iHeight_review1 );
// 		SAVE_IMAGE(hiFire_affine, &hrCandidates_special, NULL, NULL, ostr.str());


// 		ostr.str("");
// 		ostr.clear();
// 		ostr<<"d:\\test\\"<<iSeq<<"normal-NG";
// 		SAVE_IMAGE(hiReview, &hrCandidates_normal, NULL, NULL, ostr.str());
	}
	else 
	{
		m_resullt[iSeq] = false;

// 		ostringstream ostr;
// 		ostr<<"d:\\test\\"<<iSeq<<"special-OK";
// 		HImage hiFire_affine = hiFire.AffineTransImageSize( hmAffine, "constant", m_iWidth_review1, m_iHeight_review1 );
// 		SAVE_IMAGE(hiFire_affine, &hrCandidates_special, NULL, NULL, ostr.str());


// 		ostr.str("");
// 		ostr.clear();
// 		ostr<<"d:\\test\\"<<iSeq<<"normal-OK";
// 		SAVE_IMAGE(hiReview, &hrCandidates_normal, NULL, NULL, ostr.str());
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


//////////////////////////////////////////////////////////////////////////////
//
//	Erosion
//
//	2번에 걸쳐서 erosion을 실행하는 함수
//
//////////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::Erosion(unsigned char *pImage, int iWidth, int iHeight, int iFilterSize_1st, int iFilterSize_2nd )
{
	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;

	// 
	// test 용
	//
	HImage hiInput1;
	hiInput1.ReadImage("d:\\test\\a.jpg");

	GetImagePointer1(hiInput1, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	pImage = (LPBYTE)hv_Pointer.L();


	iWidth = hv_Width;
	iHeight = hv_Height;

	BHC_MEMO("Erosion_gray > start");
	// gen image
	HImage hiIn;
	hiIn.GenImage1("byte", iWidth, iHeight, pImage);


	// threshold
	HRegion hrThreshold = hiIn.Threshold(1, 255);


	// erosion > erosion > invert
	double dRadius = 0.5 + iFilterSize_1st;
	HRegion hrThreshold_erosion = hrThreshold.ErosionCircle(dRadius).ErosionRectangle1(1, 2 * iFilterSize_2nd + 1).Complement();


	//
	hiIn.OverpaintRegion(hrThreshold_erosion, 0, "fill");


	// 
//	HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;
	GetImagePointer1(hiIn, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	BYTE *pResult = (LPBYTE)hv_Pointer.L();

	memcpy(pImage, pResult, iWidth*iHeight);


//	BHC_MEMO("Erosion_gray > end");

//	hiInput1.WriteImage("bmp", 0, "d:\\test\\erosion.bmp");


	return true;
}


//////////////////////////////////////////////////////////////////////////////
//
//	Erosion
//
//	gray level 영상에 대해서 2번에 걸쳐서 erosion을 실행하는 함수
//
//	unsigned char *pImage, 
//	int iWidth, 
//	int iHeight, 
//	int iFilterSize_1st, : 2 x iFilterSize_1st + 1 이 필터 크기
//	int iFilterSize_2nd	: 2 x iFilterSize_2nd + 1 이 필터 크기
//
//////////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::Erosion_gray(unsigned char *pImage, int iWidth, int iHeight, int iFilterSize_1st, int iFilterSize_2nd)
{
	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;

	// 
	// test 용
	//
	HImage hiInput1;
	hiInput1.ReadImage("d:\\test\\a.jpg");

	GetImagePointer1(hiInput1, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	pImage = (LPBYTE)hv_Pointer.L();


	iWidth = hv_Width;
	iHeight = hv_Height;

	BHC_MEMO("Erosion_gray > start");
	// gen image
	HImage hiIn;
	hiIn.GenImage1("byte", iWidth, iHeight, pImage);


	// 1. 1st erogion
	HImage hiIn_erosion = hiIn.GrayErosionShape( 2*iFilterSize_1st + 1, 2 * iFilterSize_1st + 1, "octagon");	// d원형
//	HImage hiIn_erosion = hiIn.GrayErosionShape(2*iFilterSize_1st + 1, 2*iFilterSize_1st + 1, "rectangle");	// 사각형


	// 2. 2nd erosion
	HImage hiIn_erosion_erosion = hiIn_erosion.GrayErosionRect(2 * iFilterSize_2nd + 1, 1);


	// 
//	HTuple hv_Pointer, hv_Type, hv_Width1, hv_Height1;
	GetImagePointer1(hiIn_erosion_erosion, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	BYTE *pResult = (LPBYTE)hv_Pointer.L();

	memcpy(pImage, pResult, iWidth*iHeight);

	BHC_MEMO("Erosion_gray > end");

	hiInput1.WriteImage("bmp", 0, "d:\\test\\erosion.bmp");

	return true;
}



//////////////////////////////////////////////////////////////////////////////
//
//	InspectMesh
//
//
//////////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::InspectMesh(unsigned char *pImage, int iWidth, int iHeight, int iDiv, int *pResultArea)
{
	int iMargin = 10;
	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;


	BHC_MEMO("InspectMesh > start");


	// gen image
	HImage hiIn;
	hiIn.GenImage1("byte", iWidth, iHeight, pImage);


	// gen ROI image
	HRegion hrROI;
	hrROI.GenRectangle1(iMargin, iMargin, iHeight - iMargin - 1, iWidth - iMargin - 1);
	HImage hiROI = hiIn.ReduceDomain(hrROI);


	// gen part image
	HRegion hrROI_div[5];
	HImage hiROI_div[5];
	HRegion hrROI_div_thr[5];
	double dDivWidth = (iWidth - 2 * iMargin) / 5;
	for (int i = 0; i < 5; i++)
	{
		hrROI_div[i].GenRectangle1(iMargin, iMargin + i * dDivWidth, iHeight - iMargin - 1, iMargin + (i + 1) * dDivWidth);
		hiROI_div[i] = hiIn.ReduceDomain(hrROI_div[i]);
		hrROI_div_thr[i] = hiROI_div[i].Threshold(200, 255);
	}


	// find threshold
	bool bFound = false;
	double dRadius = 0;
	for (int i = 5; i < 10; i++)
	{
		for (int k = 0; k < 5; k++)
		{
			dRadius = 0.5 + (double)i;
			HRegion hrPart = hrROI_div_thr[k].DilationCircle(dRadius);

			int iArea_all = hrROI_div[k].Area();
			int iArea_thr = hrPart.Area();
			if (iArea_thr > iArea_all*0.95)
			{
				k = 5;
				i = 10;
				bFound = true;
			}
		}
	}

	if (bFound == false)
	{
		*pResultArea = -1;

		return false;
	}



	//
	HRegion hrThr = hiROI.Threshold(0, 200).ErosionCircle(dRadius).Connection();

	int iMaxSize = 0;
	for (int i = 0; i < hrThr.CountObj(); i++)
	{
		HRegion hrDefect = hrThr.SelectObj(i + 1);

		int iDefectArea = hrDefect.Area();
		if (iDefectArea > iMaxSize)
		{
			iMaxSize = iDefectArea;
		}
	}

	*pResultArea = iMaxSize;


	if (m_bSave)
	{
		SAVE_IMAGE(hiIn, &hrThr, NULL, NULL, "d:\\test\\mesh deformation", 0);
	}


	BHC_MEMO("InspectMesh > end");


	return true;
}


//////////////////////////////////////////////////////////////////////////////
//
//	GetBoundaryPoint
//
//	ROI 영상 대상 외곽선 추출 함수
//
//
//////////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::CalcBoundaryPoint(unsigned char *pImage, int iWidth, int iHeight, int iRoiGray, int iInnerRange, int iOutterRange)
{
	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;

	// 
	// test 용
	//
	HImage hiInput1;
	hiInput1.ReadImage("d:\\test\\a.bmp");

	GetImagePointer1(hiInput1, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	pImage = (LPBYTE)hv_Pointer.L();

	iWidth = hv_Width;
	iHeight = hv_Height;


	BHC_MEMO("GetBoundaryPoint > start");

	std::list<HXLDCont> hxList;

	// 준비
	// gen image
	HImage hiIn;
	hiIn.GenImage1("byte", iWidth, iHeight, pImage);


	//
	// get boundary
	//
	// get boundary region : threshold > fill holes > blob
	HRegion hrRoi = hiIn.Threshold(iRoiGray, iRoiGray).FillUp().Connection();


	// * 2개이상 region이 있으면 종료
	double dArea = hrRoi.RegionFeatures("area");
	if (dArea < 1)
	{
		return false;
	}

	// gen boundary line : gen region > smooth
	HXLDCont hxBoundary = hrRoi.GenContourRegionXld("border").SmoothContoursXld(3);


	//
	//
	//
	int nBoundary = 1 + iInnerRange + iOutterRange;
	m_iBoundaryNum = nBoundary;
	//CreateBoundaryList(nBoundary);

	int iCurLine = 0;

	// 
	for (int i = -iOutterRange; i <= iInnerRange; i++)
	{
		// 평행선 구하기
		HXLDCont hxInterest = hxBoundary.GenParallelContourXld("regression_normal", i);

		// points 얻기
		HTuple htRows_boundary, htCols_boundary;
		hxInterest.GetContourXld(&(g_htRows_boundarys[iCurLine]), &(g_htCols_boundarys[iCurLine]));

		/*
		// 저장하기
		std::list<int> *pX = &(m_pBoundaryList_x[iCurLine]);
		std::list<int> *pY = &(m_pBoundaryList_y[iCurLine]);


		for (int k = 0; k < htRows_boundary.TupleLength(); k++)
		{
			double x = htCols_boundary[k];
			double y = htCols_boundary[k];

			pX->push_back( (int)x );
			pX->push_back( (int)y) ;
		}
		BHC_MEMO("GetBoundaryPoint > 3.3");
*/
		iCurLine++;

		// *
		//hxList.push_back(hxInterest);
	}
	

	BHC_MEMO("GetBoundaryPoint > end");
//	if (m_bSave)
//	{
//		SAVE_IMAGE(hiIn, NULL, &hxList, NULL, "d:\\test\\GetBoundaryPoint", 1);
//	}


	return true;
}


//////////////////////////////////////////////////////////////////////////////
//
//	InspectBoundary
//
//	ROI 끝부분에 대한 검사(전처리)
//
//
//////////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::InspectBoundary(unsigned char *pImage, int iWidth, int iHeight, unsigned char *pRoi, unsigned char *pResult, int iRoiGray, int iInnerRange, int iOutterRange)
{
	int iMinSize = 8;
	double dAngleMargin = 10.0 * 3.14 / 180.0;
	int iThreshold = 10;

	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;

	// 
	// test 용
	//
	// set org 영상
	HImage hiInput1;
	hiInput1.ReadImage("d:\\test\\InspectBoundary_org_mark.bmp");

	GetImagePointer1(hiInput1, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	pImage = (LPBYTE)hv_Pointer.L();

	iWidth = hv_Width;
	iHeight = hv_Height;

	// set ROI 영상
	HImage hiRoi;
	hiRoi.ReadImage("d:\\test\\InspectBoundary_roi.bmp");

	GetImagePointer1(hiRoi, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	pRoi = (LPBYTE)hv_Pointer.L();


	// output image
	HImage hiResult;
	hiResult.GenImageConst("byte", iWidth, iHeight);

	GetImagePointer1(hiResult, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	pResult = (LPBYTE)hv_Pointer.L();


	BHC_MEMO("InspectBoundary > start");

	std::list<HXLDCont> hxList;


	// 준비
	HImage hiIn;
	hiIn.GenImage1("byte", iWidth, iHeight, pImage);


	// gen image
	HImage hiROI;
	hiROI.GenImage1("byte", iWidth, iHeight, pRoi);



	BHC_MEMO("InspectBoundary > get boundary line");
	//
	// get boundary points
	//
	// get boundary region : threshold > fill holes > blob
	HRegion hrRoi = hiROI.Threshold(iRoiGray, iRoiGray).FillUp().Connection();


	// * 2개이상 region이 있으면 종료
	double dArea = hrRoi.RegionFeatures("area");
	if (dArea < 1)
	{
		return false;
	}

	// gen boundary line : gen region > smooth
	HXLDCont hxBoundary = hrRoi.GenContourRegionXld("border").SmoothContoursXld(3);


	BHC_MEMO("InspectBoundary > get boundary area");
	//
	// get boundary area
	//
	int nBoundary = 1 + iInnerRange + iOutterRange;
	m_iBoundaryNum = nBoundary;


	HTuple htRow_Boundary[200];
	HTuple htCol_Boundary[200];
	HTuple htGray_Boundary[200];


	// 
	int iCurLine = 0;
	for (int i = -iOutterRange; i <= iInnerRange; i++)
	{
		// 평행선 구하기
		HXLDCont hxInterest = hxBoundary.GenParallelContourXld("regression_normal", i);

		// points 얻기
		hxInterest.GetContourXld(&(htRow_Boundary[iCurLine]), &(htCol_Boundary[iCurLine]));

		// get gray value
		htGray_Boundary[iCurLine] = hiIn.GetGrayval(htRow_Boundary[iCurLine], htCol_Boundary[iCurLine]);

		iCurLine++;
	}

	int iLen = htRow_Boundary[0].TupleLength();
	int iResidue = iLen - 30000;

	{
		BHC_MEMO("InspectBoundary > 1 > make image");
		//
		// make image
		//
		HImage hiBoundary;
		hiBoundary.GenImageConst("byte", 30000, nBoundary);

		GetImagePointer1(hiBoundary, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
		unsigned char *pBoundary = (LPBYTE)hv_Pointer.L();

		for (int i = 0; i < nBoundary; i++)
		{
			HTuple htCol_image = HTuple::TupleGenSequence(0, 29999, 1);
			HTuple htRow_image = HTuple::TupleGenConst(30000, i);
			HTuple htGray_image = htGray_Boundary[i].TupleSelectRange(0, 29999);

			hiBoundary.SetGrayval(htRow_image, htCol_image, htGray_image);
		}

		if (m_bSave)
		{
			hiBoundary.WriteImage("bmp", 0, "d:\\test\\InspectBoundary-linear image");
		}


		BHC_MEMO("InspectBoundary > 1 > find defect & marking");
		//
		// find defect
		//
		HRegion hrThreshold = hiBoundary.Threshold(iThreshold, 255).Connection();

		for (int i = 0; i < hrThreshold.CountObj(); i++)
		{
			HRegion hrDefect = hrThreshold.SelectObj(i + 1);

			// 크기 제한
			if (hrDefect.Area() < iMinSize)
			{
				continue;
			}

			// 각도 제한
			double dAnlge = hrDefect.OrientationRegion();
			if (dAnlge < dAngleMargin && dAnlge > -dAngleMargin)
			{
				continue;
			}
			if (dAnlge > 3.14 - dAngleMargin || dAnlge < -3.14 + dAngleMargin)
			{
				continue;
			}

			// 위치 제한
			HTuple htRow1, htRow2, htCol1, htCol2;
			hrDefect.SmallestRectangle1(&htRow1, &htCol1, &htRow2, &htCol2);

			double dY = htRow1;
			if (dY < iOutterRange / 2)
			{
				continue;
			}


			// marking
			HTuple htRows_defect, htCols_defect;
			hrDefect.GetRegionPoints(&htRows_defect, &htCols_defect);

			for (int k = 0; k < htRows_defect.TupleLength(); k++)
			{
				// position at linear image
				int iRow = htRows_defect[k];
				int iCol = htCols_defect[k];

				// position at org image
				double dRow_org = htRow_Boundary[iRow][iCol];
				double dCol_org = htCol_Boundary[iRow][iCol];
				double dGray = htGray_Boundary[iRow][iCol];

				pResult[(int)dCol_org + (int)dRow_org * iWidth] = 255;
				//pResult[(int)dCol_org + (int)dRow_org * iWidth] = min(128 + 2 * dGray, 255);
			}

		}
	}


	if (iResidue > 10)
	{
		BHC_MEMO("InspectBoundary > 2 > make image");
		//
		// make image
		//
		HImage hiBoundary;
		hiBoundary.GenImageConst("byte", iResidue, nBoundary);

		GetImagePointer1(hiBoundary, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
		unsigned char *pBoundary = (LPBYTE)hv_Pointer.L();

		for (int i = 0; i < nBoundary; i++)
		{
			HTuple htCol_image = HTuple::TupleGenSequence(0, iResidue-1, 1);
			HTuple htRow_image = HTuple::TupleGenConst(iResidue, i);
			HTuple htGray_image = htGray_Boundary[i].TupleSelectRange(30000, 30000+iResidue-1);

			hiBoundary.SetGrayval(htRow_image, htCol_image, htGray_image);
		}

		if (m_bSave)
		{
			hiBoundary.WriteImage("bmp", 0, "d:\\test\\InspectBoundary-linear image_2");
		}


		BHC_MEMO("InspectBoundary > 2 > find defect & marking");
		//
		// find defect
		//
		HRegion hrThreshold = hiBoundary.Threshold(iThreshold, 255).Connection();

		for (int i = 0; i < hrThreshold.CountObj(); i++)
		{
			HRegion hrDefect = hrThreshold.SelectObj(i + 1);

			// 크기 제한
			if (hrDefect.Area() < iMinSize)
			{
				continue;
			}

			// 각도 제한
			double dAnlge = hrDefect.OrientationRegion();
			if (dAnlge < dAngleMargin && dAnlge > -dAngleMargin)
			{
				continue;
			}
			if (dAnlge > 3.14 - dAngleMargin || dAnlge < -3.14 + dAngleMargin)
			{
				continue;
			}

			// 위치 제한
			HTuple htRow1, htRow2, htCol1, htCol2;
			hrDefect.SmallestRectangle1(&htRow1, &htCol1, &htRow2, &htCol2);

			double dY = htRow1;
			if (dY < iOutterRange / 2)
			{
				continue;
			}


			// marking
			HTuple htRows_defect, htCols_defect;
			hrDefect.GetRegionPoints(&htRows_defect, &htCols_defect);

			for (int k = 0; k < htRows_defect.TupleLength(); k++)
			{
				// position at linear image
				int iRow = htRows_defect[k];
				int iCol = 30000 + htCols_defect[k];

				// position at org image
				double dRow_org = htRow_Boundary[iRow][iCol];
				double dCol_org = htCol_Boundary[iRow][iCol];
				double dGray = htGray_Boundary[iRow][iCol];

				pResult[(int)dCol_org + (int)dRow_org * iWidth] = 255;
				//pResult[(int)dCol_org + (int)dRow_org * iWidth] = min(128 + 2 * dGray, 255);
			}

		}
	}

	if (m_bSave)
	{
		hiResult.WriteImage("bmp", 0, "d:\\test\\InspectBoundary-result image");
	}


	BHC_MEMO("InspectBoundary > end");


/*
	iCurLine = 0;
	for (int i = 0; i < nBoundary; i++)
	{
		unsigned char *pTemp = pBoundary + i * 30000;

		for (int x = 0; x < 30000; x++)
		{
			double dGray = htGray_Boundary[iCurLine][x];
			*pTemp++ = dGray;
		}

		iCurLine++;
	}*/


	return true;
}


bool CImageProcess_H::CreateBoundaryList(int iCount)
{
	if (iCount < 1)
	{
		return false;
	}


	// set count
	m_iBoundaryNum = iCount;

	// gen x array
	if (m_pBoundaryList_x)
	{
		delete[] m_pBoundaryList_x;
	}
	m_pBoundaryList_x = new std::list<int>[iCount];

	// gen y array
	if (m_pBoundaryList_y)
	{
		delete[] m_pBoundaryList_y;
	}
	m_pBoundaryList_y = new std::list<int>[iCount];


	return true;
}



//////////////////////////////////////////////////////////////////////////////
//
//	FindROICoordinate
//
//	ROI 상하좌우 좌표 검출
//
//
//	[in] unsigned char *pSrcImage	
//	[in] int iWidth					
//	[in] int iHeight				
//	[out] int *pSx  
//	[out] int *pSy  
//	[out] int *pEx  
//	[out] int *pEy  
//
//
//////////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::FindROICoordinate(unsigned char *pSrcImage, int iWidth, int iHeight, int *pSx, int *pSy, int *pEx, int *pEy)
{
	int iCx = iWidth / 2;
	int iCy = iHeight / 2;


	// up
	*pSy = 0;
	for (int y = 0; y < iHeight; y++)
	{
		int iPos = iCx + y * iWidth;

		if (pSrcImage[iPos] != 0)
		{
			*pSy = y;
			y = iHeight;
		}
	}

	// bottom
	*pEy = 0;
	for (int y = iHeight - 1; y >= 0; y--)
	{
		int iPos = iCx + y * iWidth;

		if (pSrcImage[iPos] != 0)
		{
			*pEy = y;
			y = -1;
		}
	}

	// left
	*pSx = 0;
	for (int x = 0; x < iWidth; x++)
	{
		int iPos = x + iCy * iWidth;

		if (pSrcImage[iPos] != 0)
		{
			*pSx = x;
			x = iWidth;
		}
	}

	// right
	*pEx = 0;
	for (int x = iWidth - 1; x >= 0; x--)
	{
		int iPos = x + iCy * iWidth;

		if (pSrcImage[iPos] != 0)
		{
			*pEx = x;
			x = -1;
		}
	}



	return true;
}



//////////////////////////////////////////////////////////////////////////////
//
//	Preprocess_median_overpaint
//
//	Median 함수를 이용한 전처리
//
//
//
//	[in] unsigned char *pSrcImage	
//	[in] int iWidth					
//	[in] int iHeight				
//	[out] unsigned char *pDstImage	
//	[in] int iSx  
//	[in] int iSy  
//	[in] int iEx  
//	[in] int iEy  
//
//
//////////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::Preprocess_median_overpaint(unsigned char *pSrcImage, int iWidth, int iHeight, unsigned char *pDstImage, int iSx, int iSy, int iEx, int iEy)
{
	BHC_MEMO("Preprocess_median_overpaint > start > sx_%d sy_%d ex_%d ey_%d", iSx, iSy, iEx, iEy);

	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;


	// ROI의 크기
	int iWidth_crop = iEx - iSx + 1;
	int iHeight_crop = iEy - iSy + 1;

	if (iWidth_crop < 1 || iHeight_crop < 1)
	{
		return false;
	}


	//
	// 준비
	//
	// 처리 대상 영상 생성
	HImage hiIn;
	hiIn.GenImageConst("byte", iWidth, iHeight);

	GetImagePointer1(hiIn, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	BYTE *pSrc = (LPBYTE)hv_Pointer.L();

	// ROI 영역 부위에 대한 복사
	int iCntY = 0;
	for (int y = iSy; y <= iEy; y++, iCntY++)
	{
		memcpy(pSrc + iCntY * iWidth_crop, pSrcImage + y * iWidth + iSx, iWidth_crop);
	}


	// median 영상 생성
	HImage hiMedian = hiIn.MedianRect(31, 1);


	// 차영상 생성
	HImage hiResult = hiIn.SubImage(hiMedian, 2, 128);


	// 결과 저장
	GetImagePointer1(hiResult, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	BYTE *pRst = (LPBYTE)hv_Pointer.L();

	// ROI 영역 부위에 대한 복사
	iCntY = 0;
	for (int y = iSy; y <= iEy; y++, iCntY++)
	{
		memcpy(pDstImage + y * iWidth + iSx, pRst + iCntY * iWidth_crop, iWidth_crop);
	}


	BHC_MEMO("Preprocess_median_overpaint > end");

	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	ResizeImage
//
//	입력영상의 scale을 변환하여 다른 버퍼로 옮긴다.
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::ResizeImage(unsigned char *pSrcImage, int iWidth, int iHeight, int iTargetWidth, int iTargetHeight, double dSx, double dSy, int iTx, int iTy, unsigned char *pTargetBuffer)
{
	BHC_MEMO("ResizeImage > start > sx_%f sy_%f tx_%d ty_%d", dSx, dSy, iTx, iTy);

	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;

	// 
	// test 용
	//
	HImage hiInput1;
	hiInput1.ReadImage("d:\\test\\a.bmp");
	GetImagePointer1(hiInput1, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	pSrcImage = (LPBYTE)hv_Pointer.L();

	iWidth = hv_Width;
	iHeight = hv_Height;



	// check
	if (iTargetWidth < 1 || iTargetHeight < 1)
	{
		return false;
	}


	// 
	//
	// 준비
	//
	// 처리 대상 영상 생성
	HImage hiIn;
	hiIn.GenImage1("byte", iWidth, iHeight, pSrcImage);

	
	//
	// resize
	//
	HImage hiResize = hiIn.ZoomImageSize(iTargetWidth, iTargetHeight, "constant");


	//
	// gen matrix
	//
	// identity matrix
	HHomMat2D hmIdentity;
	hmIdentity.HomMat2dIdentity();

	// scale
	HHomMat2D hmIdentity_scale = hmIdentity.HomMat2dScale(dSy, dSx, 0, 0);

	// translation
	HHomMat2D hmIdentity_scale_trans = hmIdentity_scale.HomMat2dTranslate(iTy, iTx);


	//
	// make result image
	//
	// result image
	HImage hiResize_affine = hiResize.AffineTransImage(hmIdentity_scale_trans, "constant", "false");

	GetImagePointer1(hiResize_affine, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	BYTE *pRst = (LPBYTE)hv_Pointer.L();

//	memcpy(pTargetBuffer, pRst, iTargetWidth*iTargetHeight);

	if (m_bSave)
	{
		hiResize_affine.WriteImage("bmp", 0, "d:\\test\\ResizeImage");
	}

	BHC_MEMO("ResizeImage > end");

	return false;
}


//////////////////////////////////////////////////////////////////////////
//
//	FlatImage
//
//	CG 영상 대상 ROI 밝기 분포 보정
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::FlatImage(unsigned char *pImage, int iWidth, int iHeight, unsigned char *pResult, int iPeriod, int iSx, int iEx, int iSeedY, int iRangeY)
{
	//
	// ROI 라인별 평균 gray 계산 
	//
	int iStartY = iSeedY - (iSeedY%iPeriod) + iPeriod;
	BHC_MEMO("CImageProcess_H::FlatImage > iStartY=%d", iStartY);

	double *pAverage = new double[iPeriod];
	int iIndex = 0;
	for (int y = iStartY; y < iStartY + iPeriod; y++, iIndex++)
	{
		unsigned long ulSum = 0;
		int iCount = 0;

		// gray sum
		for (int i = 0; i < iRangeY; i++)
		{
			int iPosY = (y + i * iPeriod)*iWidth;	// y좌표
			for (int x = iSx; x < iEx; x++)
			{
				int iPos = x + iPosY;
				ulSum += pImage[iPos];

				iCount++;
			}
		}

		// calc average
		double dAvg = (double)ulSum / iCount;
		pAverage[iIndex] = dAvg;

		BHC_MEMO("CImageProcess_H::FlatImage > [%d]%f", iIndex, dAvg);
	}

	// get min gray
	double dMinAverage = DBL_MAX;
	for(int i = 0; i < iPeriod; i++)
	{
		if (pAverage[i] < dMinAverage)
		{
			dMinAverage = pAverage[i];
		}
	}

	// * 
	if (dMinAverage <= 0)
	{
		return false;
	}

	BHC_MEMO("CImageProcess_H::FlatImage > min=%f", dMinAverage);

	//
	// flat image
	//
	iIndex = 0;
	for (int y = 0; y < iHeight; y++)
	{
		int iPosY = y*iWidth;
		for (int x = 0; x < iWidth; x++)
		{
			int iPos = x + iPosY;

			pResult[iPos] = (double)pImage[iPos] * dMinAverage / pAverage[iIndex];
		}

		iIndex = (iIndex + 1) % iPeriod;
	}
	

	delete[] pAverage;


	return true;
}



//////////////////////////////////////////////////////////////////////////
//
//	MakeTemplateAndSave
//
//	template 생성, 저장
//
//	unsigned char *pImage, 
//	int iWidth, 
//	int iHeight, 
//	std::list<int> xList, , 
//	std::list<int> yList, 
//	double *pX, 
//	double *pY, 
//	const char* pFileName
//
//////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::MakeTemplateAndSave(unsigned char *pImage, int iWidth, int iHeight, std::list<int> xList, std::list<int> yList, double *pX, double *pY, const char* pFileName)
{
	if ((xList.size() < 4) ||
		(yList.size() < 4) ||
		(xList.size() != yList.size()))
	{
		return false;
	}


	// 
	// gen image
	//
	HImage hiIn;
	hiIn.GenImage1("byte", iWidth, iHeight, pImage);


	//
	// gen ROI
	//
	HTuple htRows, htCols;

	// x 좌표 생성
	int iIndex = 0;
	for (std::list<int>::iterator ite = xList.begin(); ite != xList.end(); ite++, iIndex++)
	{
		htCols[iIndex] = *ite;
	}
	htCols[iIndex] = *(xList.begin());	// 첫번째 좌표를 마지막에 넣어서 폐곡선 생성

	// y 좌표 생성
	iIndex = 0;
	for (std::list<int>::iterator ite = yList.begin(); ite != yList.end(); ite++, iIndex++)
	{
		htRows[iIndex] = *ite;
	}
	htRows[iIndex] = *(yList.begin());	// 첫번째 좌표를 마지막에 넣어서 폐곡선 생성

	// gen hregion
	HRegion hrRoi;
	hrRoi.GenRegionPolygon(htRows, htCols);
	HRegion hrRoi_fill = hrRoi.FillUp();

	if (hrRoi_fill.Area() == 0)
	{
		return false;
	}


	// gen ROI image
	HImage hiROI = hiIn.ReduceDomain(hrRoi_fill);


	//
	// gen template
	//
	HShapeModel htModel;
	try
	{
		htModel.CreateShapeModel(hiROI, "auto",
			-0.39,
			0.79,
			"auto",
			"auto",
			"use_polarity",
			"auto",
			"auto");
	}
	catch (HException &except)
	{
		if (except.ErrorCode() == H_MSG_TRUE)
		{
			int a = 0;
		}
		else {
			return false;
		}
	}

	// save file
	htModel.WriteShapeModel(pFileName);



	//
	//
	//
	HTuple htRow, htCol, htAngle, htScore;
	try
	{
		htModel.FindShapeModel(hiIn,
		-0.39,
		0.79,
		0.3,
		1,
		0.5,
		"least_squares",
		0,
		0.9,
		&htRow, 
		&htCol, 
		&htAngle, 
		&htScore);
	}
	catch (HException &except)
	{
		if (except.ErrorCode() == H_MSG_TRUE)
		{
			int a = 0;
		}
		else {
			return false;
		}
	}

	if (htRow.TupleLength() != 1)
	{
		return false;
	}

	*pX = htCol[0];
	*pY = htRow[0];


	return true;
}





bool CImageProcess_H::train(double *pData, int iRow, int iCol, int *pClass)
{
	int iNumInput = iCol;
	int iNumHidden = iCol;
	int iNumOutPut = 2;


	// create
	HTuple htMLPHandle;
	CreateClassMlp(iNumInput,
		iNumHidden,
		iNumOutPut,
		"softmax",
		"normalization",
		10,
		42,
		&htMLPHandle);


	// set data
	for (int i = 0; i < iRow; i++)
	{
		// feature
		HTuple htFeature;
		for (int k = 0; k < iCol; k++)
		{
			htFeature[k] = pData[i*iCol + k];
		}

		// class
		HTuple htTarget;
		htTarget = pClass[i];

		// add
		AddSampleClassMlp(htMLPHandle, htFeature, htTarget);
	}


	// train save model
	HTuple htError, hrErrorLog;
	try
	{
		TrainClassMlp(htMLPHandle, 200, 1, 0.01, &htError, &hrErrorLog);
	}
	catch (HException &HDevExpDefaultException)
	{
		return false;
	}


	// save
	WriteClassMlp(htMLPHandle, "d:\\model.hmd");

	return true;
}



bool CImageProcess_H::predict(double *pData, int iCol, int *pClass, double *pProba)
{
	// read model
	HTuple htMLPHandle;
	try
	{
		ReadClassMlp("d:\\model.hmd", &htMLPHandle);
	}
	catch (HException &HDevExpDefaultException)
	{
		return false;
	}


	// set data
	HTuple htFeatures;
	for (int i = 0; i < iCol ; i++)
	{
		htFeatures[i] = pData[i];
	}


	// classify
	HTuple htClass, htConfidence;
	ClassifyClassMlp(htMLPHandle,
		htFeatures,
		1,
		&htClass,
		&htConfidence);


	// result
	*pClass = htClass[0];
	*pProba = htConfidence[0];


	return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//	init_globalBuffer
//
//	global buffer 생성
//
//////////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::Init_globalBuffer(int iWidth, int iHeight)
{
	BHC_MEMO("init_globalBuffer > start > %d x %d", iWidth, iHeight);


	for (int iSeq = 0; iSeq < 2; iSeq++)
	{
		for (int iScan = 0; iScan < 14; iScan++)
		{
			g_hiInput[iSeq][iScan].GenImageConst("byte", iWidth, iHeight);
			g_hiROI[iSeq][iScan].GenImageConst("byte", iWidth, iHeight);
		}
	}


	BHC_MEMO("init_globalBuffer > start");

	return true;
}


//////////////////////////////////////////////////////////////////////////////
//
//	Set_globalBuffer_input
//
//	원본 영상 복사
//
//////////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::Set_globalBuffer_input(int iSeq, int iScan, unsigned char *pData, int iWidth, int iHeight)
{
	BHC_MEMO("Set_globalBuffer_input > start > [%d][%d] %d x %d", iSeq, iScan, iWidth, iHeight);


	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;
	GetImagePointer1(g_hiInput[iSeq][iScan], &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	BYTE *pTarget = (LPBYTE)hv_Pointer.L();

	memcpy(pTarget, pData, iWidth*iHeight);


	BHC_MEMO("Set_globalBuffer_input > start");

	return true;

}



//////////////////////////////////////////////////////////////////////////////
//
//	Set_globalBuffer_roi
//
//	ROI 영상 복사
//
//////////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::Set_globalBuffer_roi(int iSeq, int iScan, unsigned char *pData, int iWidth, int iHeight)
{
	if (iSeq < 0 || iSeq > 1 || iScan < 0 || iScan > 13)
	{
		BHC_MEMO("Set_globalBuffer_roi > error > %d, %d", iSeq, iScan);
		return false;
	}

	BHC_MEMO("Set_globalBuffer_roi > start > [%d][%d] %d x %d", iSeq, iScan, iWidth, iHeight);


	HTuple hv_Pointer, hv_Type, hv_Width, hv_Height;
	GetImagePointer1(g_hiROI[iSeq][iScan], &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	BYTE *pTarget = (LPBYTE)hv_Pointer.L();

	memcpy(pTarget, pData, iWidth*iHeight);


	BHC_MEMO("Set_globalBuffer_roi > end");

	return true;
}



//////////////////////////////////////////////////////////////////////////////
//
//	Set_globalBuffer_roi
//
//	ROI 영상 복사
//
//////////////////////////////////////////////////////////////////////////////
bool CImageProcess_H::Get_globalBuffer_size(int &iWidth, int &iHeight)
{
	BHC_MEMO("Get_globalBuffer_size > start ", );

	Hlong hlWidth, hlHeight;
	g_hiInput[0][0].GetImageSize(&hlWidth, &hlHeight);

	iWidth = hlWidth;
	iHeight = hlHeight;

	BHC_MEMO("Get_globalBuffer_size > end");

	return true;
}



