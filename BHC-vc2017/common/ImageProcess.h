#pragma once

#include "ImageProcess_define.h"

#include <list>
#include <vector>
using namespace std;


class __declspec(dllexport) CImageProcess
{
public:
	CImageProcess(void);
	virtual ~CImageProcess();


	virtual void Test(void)
	{
		int a = 0;
	}

};

