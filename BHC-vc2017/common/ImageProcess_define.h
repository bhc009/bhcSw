#pragma once

#include <list>
using namespace std;

#ifndef _FLOAT_POINT_
struct FPoint
{
	float x;
	float y;
};
#define _FLOAT_POINT_
#endif



// edge direction
#define DIR_0	0
#define DIR_30	1
#define DIR_60	2
#define DIR_90	3
#define DIR_120	4
#define DIR_150	5
#define DIR_180	6
#define DIR_210	7
#define DIR_240	8
#define DIR_270	9
#define DIR_300	10
#define DIR_330	11


#define _IP_PI_ 3.14159265359

//
#ifndef MIN
#define MIN(X,Y) (((X)>(Y))?(Y):(X))
#endif

#ifndef MAX
#define MAX(X,Y) (((X)>(Y))?(X):(Y))
#endif

#ifndef POINT_XY
#define POINT_XY(img,iWidth,x,y)	img[ (x) + (y)*(iWidth) ]
#endif

#ifndef RANGE_LIMIT
#define RANGE_LIMIT(v_min, v, v_max)	MAX( MIN( v, v_max), v_min )
#endif




//////////////////////////////////////////////////////////////////////////
//
//	용접검사
//
//


struct stInfo_batWeld
{
	//
	int iArea;
	double dRadius;

	// xyy
	double dXyz_x_mean;
	double dXyz_y_mean;
	double dXyz_z_mean;
	double dXyz_x_deviation;
	double dXyz_y_deviation;
	double dXyz_z_deviation;
	std::list<double> listXyz_x;
	std::list<double> listXyz_y;
	std::list<double> listXyz_z;

	// hsv
	double dHsv_h_mean;
	double dHsv_s_mean;
	double dHsv_v_mean;
	double dHsv_h_deviation;
	double dHsv_s_deviation;
	double dHsv_v_deviation;
	std::list<double> listHsv_h;
	std::list<double> listHsv_s;
	std::list<double> listHsv_v;

	// luv
	double dLuv_l_mean;
	double dLuv_u_mean;
	double dLuv_v_mean;
	double dLuv_l_deviation;
	double dLuv_u_deviation;
	double dLuv_v_deviation;
	std::list<double> listLuv_l;
	std::list<double> listLuv_u;
	std::list<double> listLuv_v;


	stInfo_batWeld()
	{
		iArea = 0;
		dRadius = 0.0;

		// xyy
		dXyz_x_mean = 0;
		dXyz_y_mean = 0;
		dXyz_z_mean = 0;
		dXyz_x_deviation = 0;
		dXyz_y_deviation = 0;
		dXyz_z_deviation = 0;

		// hsv
		dHsv_h_mean = 0;
		dHsv_s_mean = 0;
		dHsv_v_mean = 0;
		dHsv_h_deviation = 0;
		dHsv_s_deviation = 0;
		dHsv_v_deviation = 0;

		// luv
		dLuv_l_mean = 0;
		dLuv_u_mean = 0;
		dLuv_v_mean = 0;
		dLuv_l_deviation = 0;
		dLuv_u_deviation = 0;
		dLuv_v_deviation = 0;
	};
};


struct stDisplsyInfo
{
	int x;				// 영상좌표
	int y;				// 영상좌표
	std::string str;	// 문자열
	bool bOk;

	// 초기화
	stDisplsyInfo()
	{
		x = 0;		// 영상좌표
		y = 0;		// 영상좌표
		bOk = false;
	}
};


//
//
//////////////////////////////////////////////////////////////////////////