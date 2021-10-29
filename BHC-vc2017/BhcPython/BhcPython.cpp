#pragma once


#include "BhcPython.h"

//#include "stdafx.h"
#include "Python.h"

#include "windows.h"


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


CBhcPython::CBhcPython(void)
{
	m_pModule = NULL;
	m_pClass = NULL;
	m_pInstance = NULL;

	for (int i = 0; i < _BHCPYTHON_MAX_INSTANCE_; i++)
	{
		m_pInstances[i] = NULL;
	}

	m_bInit = false;

	m_iClassifierIndex = _BHCPYTHON_CLASSIFIER_LDA_;

	m_iCnt = 0;
}


CBhcPython::~CBhcPython()
{
	if (m_pInstance != NULL )
	{
		Py_DECREF(m_pInstance);
	}

	for (int i = 0; i < _BHCPYTHON_MAX_INSTANCE_; i++)
	{
		if (m_pInstances[i] != NULL)
		{
			Py_DECREF(m_pInstances[i]);
		}
	}

	if (m_pClass != NULL)
	{
		Py_DECREF(m_pClass);
	}

	if (m_pModule != NULL)
	{
		Py_DECREF(m_pModule);
	}


	Py_Finalize();
}


void CBhcPython::initialize(const char* pModuleName)
{
	OutputDebugString("CBhcPython::initialize() > start\n");

	// python 초기화
	Py_Initialize();


	OutputDebugString("CBhcPython::initialize() > get file name\n");
	// load module
	PyObject *pName = PyUnicode_DecodeFSDefault(pModuleName); // pModuleName.py 라는 파일의 이름을 PyObject로 생성

	OutputDebugString("CBhcPython::initialize() > import\n");
	m_pModule = PyImport_Import(pName); // PyTest.py를 import 한다

	Py_DECREF(pName);


	OutputDebugString("CBhcPython::initialize() > get class name\n");
	//
	m_pClass = PyObject_GetAttrString((PyObject *)m_pModule, "MyClass");	// 

	OutputDebugString("CBhcPython::initialize() > create class\n");
	m_pInstance = PyObject_CallObject((PyObject *)m_pClass, NULL); // class의 instance 생성


	for (int i = 0; i < _BHCPYTHON_MAX_INSTANCE_; i++)
	{
		m_pInstances[i] = PyObject_CallObject((PyObject *)m_pClass, NULL); // class의 instance 생성
	}


	//
	m_bInit = true;


	OutputDebugString("CBhcPython::initialize() > end\n");
}


void CBhcPython::Test()
{
	OutputDebugString("CBhcPython::Test() > s\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Test() > error > no init\n");

		return;
	}


	if (m_pModule != NULL)
	{
		printf("not null!!\n");

		OutputDebugString("CBhcPython::Test() > get fuc\n");
		// get function
//		PyObject *pFunc = PyObject_GetAttrString( (PyObject *)m_pModule, "test"); // test()라는 함수
		PyObject *pFunc = PyObject_GetAttrString((PyObject *)m_pModule, "get"); // test()라는 함수

		if (pFunc && PyCallable_Check(pFunc)) // callable한지 확인
		{
			OutputDebugString("CBhcPython::Test() > excute\n");
			// excute function
			PyObject *pValue = PyObject_CallObject(pFunc, NULL); // pFunc에 매개변수 전달해서 실행한다

			// show result
			if (pValue != NULL) //리턴값
			{
				OutputDebugString("CBhcPython::Test() > show\n");
				
				long a = PyLong_AsLong(pValue);

				BHC_MEMO("CBhcPython::Test() > %d\n", a);

				Py_DECREF(pValue);
			}

			// void 형 함수는 여기로 바로 빠짐

			Py_DECREF(pFunc);
		}
	}
	else { //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Test() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Test() > e\n");
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Test_global
//
//	인스턴스내 함수를 실행한다.
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void CBhcPython::Test_global()
{
	OutputDebugString("CBhcPython::Test_global() > s\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Test_global() > error > no init\n");

		return;
	}


	if (m_pInstance != NULL)
	{
		OutputDebugString("CBhcPython::Test_global() > fuc\n");

		PyObject *pResult = PyObject_CallMethod((PyObject *)m_pInstance, "add", "i", m_iCnt++); // 인스턴스에서 함수를 실행한다.

		// show result
		if (pResult != NULL) //리턴값
		{
			OutputDebugString("CBhcPython::Test_global() > show result\n");

			long lValue = PyLong_AsLong(pResult);

			BHC_MEMO("CBhcPython::Test_global() > %d\n", lValue);

			Py_DECREF(pResult);
		}
	}
	else { //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Test_global() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Test_global() > e\n");
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Test_global
//
//	numpy 모듈 동작 테스트
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void CBhcPython::Test_class()
{
	OutputDebugString("CBhcPython::Test_class() > s\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Test_class() > error > no init\n");

		return;
	}


	if (m_pInstance != NULL)
	{
		OutputDebugString("CBhcPython::Test_class() > fuc\n");

		// make params
		PyObject *nParam = PyTuple_New(3);
		for (long i = 0; i < 3; i++) {
			PyObject *item = PyLong_FromLong( i*m_iCnt);
			PyTuple_SetItem(nParam, i, item);
			Py_XDECREF(item);
		}

		// run
		PyObject *pResult = PyObject_CallMethod((PyObject *)m_pInstance, "pushData", "(iO)", m_iCnt%2, nParam); // 인스턴스에서 함수를 실행한다.
		Py_XDECREF(nParam);

		m_iCnt++;


		// show result
		if (pResult != NULL) //리턴값
		{
			OutputDebugString("CBhcPython::Test_class() > show result\n");

			int iDataNum = PyObject_Length(pResult);
			BHC_MEMO("CBhcPython::Test_class() > #result %d\n", iDataNum);

			for (int i = 0; i < iDataNum; i++)
			{
				PyObject *piItem = PySequence_GetItem(pResult, i);

				double dValue = PyFloat_AsDouble(piItem);

				BHC_MEMO("CBhcPython::Test_class() > result[%d]%f\n", i, dValue );
	
				Py_XDECREF(piItem);
			}

			Py_XDECREF(pResult);
		}
	}
	else { //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Test_class() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Test_class() > e\n");
}



void CBhcPython::Test_float()
{
	OutputDebugString("CBhcPython::Test_float() > s\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Test_float() > error > no init\n");

		return;
	}


	if (m_pInstance != NULL)
	{
		OutputDebugString("CBhcPython::Test_float() > fuc\n");

		// make params
		PyObject *nParam = PyTuple_New(3);
		for (long i = 0; i < 3; i++) {
			PyObject *item = PyFloat_FromDouble(0.1);
//			PyObject *item = PyLong_FromLong(i);
			PyTuple_SetItem(nParam, i, item);
			Py_XDECREF(item);
		}

		// run - one
		PyObject *pResult_1 = PyObject_CallMethod((PyObject *)m_pInstance, "add", "d", 1.234); // 인스턴스에서 함수를 실행한다.
		if (pResult_1 != NULL) //리턴값
		{
			double dValue_1 = PyFloat_AsDouble(pResult_1);
			BHC_MEMO("add double = %f", dValue_1);

			Py_XDECREF(pResult_1);
		}

		// run - array
		PyObject *pResult_2 = PyObject_CallMethod((PyObject *)m_pInstance, "set", "iO&", 3, nParam); // 인스턴스에서 함수를 실행한다.

		if (pResult_2 != NULL) //리턴값
		{
			double dValue_2 = PyFloat_AsDouble(pResult_2);
			BHC_MEMO("sum double = %f", dValue_2);
			
			Py_XDECREF(pResult_2);
		}

		Py_XDECREF(nParam);
	}
	else 
	{ //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Test_float() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Test_float() > e\n");
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CBhcPython::Fit(double *pA, int *pB, int iCol, int iRow)
{
	OutputDebugString("CBhcPython::Fit() > s\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Fit() > error > no init\n");

		return false;
	}


	bool bResult = false;


	if (m_pInstance != NULL)
	{
		OutputDebugString("CBhcPython::Fit() > fuc\n");

		// make params
		PyObject *poResult_1 = PyObject_CallMethod((PyObject *)m_pInstance, "ML_setDim", "ii", iRow, iCol ); // 인스턴스에서 함수를 실행한다.
		Py_XDECREF(poResult_1);

		// A matrix
		for (long y = 0; y < iRow; y++)
		{
			for (long x = 0; x < iCol; x++)
			{
				int iPos = x + y * iCol;
				double dValue = pA[iPos];

				PyObject *poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_setA", "iid", y, x, dValue); // 인스턴스에서 함수를 실행한다.

				Py_XDECREF(poResult);

				BHC_MEMO("Fit A(%d,%d) = %f\n", x, y, dValue);
			}
		}

		// B matrix
		for (long y = 0; y < iRow; y++)
		{
			long lValue = pB[y];

			PyObject *poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_setB", "iii", y, 0, lValue); // 인스턴스에서 함수를 실행한다.
			
			Py_XDECREF(poResult);
				
			BHC_MEMO("Fit B(%d) = %d", y, lValue);
		}


		// run
		//PyObject *poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_learn", NULL ); // 인스턴스에서 함수를 실행한다.
		PyObject *poResult;
		switch (m_iClassifierIndex)
		{
		case _BHCPYTHON_CLASSIFIER_LDA_:
			OutputDebugString("CBhcPython::Fit() > lda\n");
			poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_learn_lda", NULL); // 인스턴스에서 함수를 실행한다.
			break;
		case _BHCPYTHON_CLASSIFIER_QDA_:
			OutputDebugString("CBhcPython::Fit() > qda\n");
			poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_learn_qda", NULL); // 인스턴스에서 함수를 실행한다.
			break;
		case _BHCPYTHON_CLASSIFIER_SOFTMAX_:
		case _BHCPYTHON_CLASSIFIER_SOFTMAX_PROBA_:
			OutputDebugString("CBhcPython::Fit() > softmax\n");
			poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_learn_soft", NULL); // 인스턴스에서 함수를 실행한다.
			break;
		case _BHCPYTHON_CLASSIFIER_SVM_:
			OutputDebugString("CBhcPython::Fit() > svm\n");
			poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_learn_svc", NULL); // 인스턴스에서 함수를 실행한다.
			break;
		case _BHCPYTHON_CLASSIFIER_RANDOMFOREST_:
			OutputDebugString("CBhcPython::Fit() > random forest\n");
			poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_learn", NULL); // 인스턴스에서 함수를 실행한다.
			break;
		case _BHCPYTHON_CLASSIFIER_LOGISTIC_PROBA_:
			OutputDebugString("CBhcPython::Fit() > logistic(proba)\n");
			poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_learn_logit", NULL); // 인스턴스에서 함수를 실행한다.
			break;
		default:
			poResult = NULL;
			break;
		}


		// show result
		if (poResult != NULL) //리턴값
		{
			OutputDebugString("CBhcPython::Fit() > OK\n");

			bResult = true;

			Py_XDECREF(poResult);
		}
	}
	else 
	{ //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Fit() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Fit() > e\n");


	return bResult;
}
/*
bool CBhcPython::Fit(double *pA, int *pB, int iCol, int iRow)
{
	OutputDebugString("CBhcPython::Fit() > s\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Fit() > error > no init\n");

		return false;
	}

	bool bResult = false;

	if (m_pInstance != NULL)
	{
		OutputDebugString("CBhcPython::Fit() > fuc\n");

		// make params
		PyObject *poA = PyTuple_New(iCol*iRow);	//
		PyObject *poB = PyTuple_New(iRow);
		for (long y = 0 ; y < iRow ; y++)
		{
			for (long x = 0; x < iCol; x++)
			{
				int iPos = x + y * iCol;
//				double dValue = pA[iPos];
				long lA = pA[iPos] * 1000;//; *_BHC_PYTHON_UNIT_;

				PyObject *poTempA = PyLong_FromLong(lA);
//				PyObject *poTempA = PyFloat_FromDouble(iPos);
				PyTuple_SetItem(poA, iPos, poTempA);
				Py_XDECREF(poTempA);

				BHC_MEMO("Fit A(%d,%d) = %d\n", x, y, lA);
			}

			long lValue = pB[y];
			PyObject *poTempB = PyLong_FromLong(lValue);
			PyTuple_SetItem(poB, y, poTempB);
			Py_XDECREF(poTempB);

			BHC_MEMO("Fit B(%d) = %d", y, lValue);
		}


		// run
		PyObject *pResult = PyObject_CallMethod((PyObject *)m_pInstance, "fit", "iiOO", iCol, iRow, poA, poB); // 인스턴스에서 함수를 실행한다.

		Py_XDECREF(poA);
		Py_XDECREF(poB);


		// show result
		if (pResult != NULL) //리턴값
		{
			OutputDebugString("CBhcPython::Fit() > show result\n");

			bResult = true;

			Py_XDECREF(pResult);
		}
	}
	else
	{ //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Fit() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Fit() > e\n");


	return bResult;
}
*/


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Fit
//
//	learn
//
//	A x = B
//	A : given
//	B : giver
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CBhcPython::Fit(int iIndex, double *pA, int *pB, int iCol, int iRow)
{
	OutputDebugString("CBhcPython::Fit() > start\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Fit() > error > no init\n");

		return false;
	}


	bool bResult = false;


	if (m_pInstances[iIndex] != NULL)
	{
		// make params
		PyObject *poResult_1 = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_setDim", "ii", iRow, iCol ); // 인스턴스에서 함수를 실행한다.
		Py_XDECREF(poResult_1);


		// make A matrix
		for (long y = 0; y < iRow; y++)
		{
			for (long x = 0; x < iCol; x++)
			{
				int iPos = x + y * iCol;
				double dValue = pA[iPos];

				PyObject *poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_setA", "iid", y, x, dValue); // 인스턴스에서 함수를 실행한다.

				Py_XDECREF(poResult);

				BHC_MEMO("Fit A(%d,%d) = %f\n", x, y, dValue);
			}
		}

		// make B matrix
		for (long y = 0; y < iRow; y++)
		{
			long lValue = pB[y];

			PyObject *poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_setB", "iii", y, 0, lValue); // 인스턴스에서 함수를 실행한다.
			
			Py_XDECREF(poResult);
				
			BHC_MEMO("Fit B(%d) = %d", y, lValue);
		}


		// learn
		//PyObject *poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_learn", NULL ); // 인스턴스에서 함수를 실행한다.
		PyObject *poResult;
		switch (m_iClassifierIndex)
		{
		case _BHCPYTHON_CLASSIFIER_LDA_:
			OutputDebugString("CBhcPython::Fit() > lda\n");
			poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_learn_lda", NULL); // 인스턴스에서 함수를 실행한다.
			break;
		case _BHCPYTHON_CLASSIFIER_QDA_:
			OutputDebugString("CBhcPython::Fit() > qda\n");
			poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_learn_qda", NULL); // 인스턴스에서 함수를 실행한다.
			break;
		case _BHCPYTHON_CLASSIFIER_SOFTMAX_:
		case _BHCPYTHON_CLASSIFIER_SOFTMAX_PROBA_:
			OutputDebugString("CBhcPython::Fit() > softmax\n");
			poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_learn_soft", NULL); // 인스턴스에서 함수를 실행한다.
			break;
		case _BHCPYTHON_CLASSIFIER_SVM_:
			OutputDebugString("CBhcPython::Fit() > svm\n");
			poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_learn_svc", NULL); // 인스턴스에서 함수를 실행한다.
			break;
		case _BHCPYTHON_CLASSIFIER_RANDOMFOREST_:
			OutputDebugString("CBhcPython::Fit() > random forest\n");
			poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_learn", NULL); // 인스턴스에서 함수를 실행한다.
			break;
		case _BHCPYTHON_CLASSIFIER_LOGISTIC_PROBA_:
			OutputDebugString("CBhcPython::Fit() > logistic(proba)\n");
			poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_learn_logit", NULL); // 인스턴스에서 함수를 실행한다.
			break;
		default:
			poResult = NULL;
			break;
		}


		// show result
		if (poResult != NULL) //리턴값
		{
			bResult = true;

			Py_XDECREF(poResult);
		}
		else {
			OutputDebugString("CBhcPython::Fit() > erro > no result\n");

		}
	}
	else 
	{ //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Fit() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Fit() > end\n");


	return bResult;
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CBhcPython::Predict(double *pA, int iCol, int iRow, int *pB)
{
	OutputDebugString("CBhcPython::Predict() > s\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Predict() > error > no init\n");

		return false;
	}

	bool bResult = false;

	if (m_pInstance != NULL)
	{
		OutputDebugString("CBhcPython::Predict() > fuc\n");

		// make params
		PyObject *poResult_1 = PyObject_CallMethod((PyObject *)m_pInstance, "ML_setDim", "ii", iRow, iCol); // 인스턴스에서 함수를 실행한다.
		Py_XDECREF(poResult_1);


		int iResultCount = 0;
		for (long y = 0; y < iRow; y++)
		{
			// make params
			for (long x = 0; x < iCol; x++)
			{
				double dValue = pA[x + y*iCol];

				PyObject *poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_setX", "id", x, dValue); // 인스턴스에서 함수를 실행한다.

				Py_XDECREF(poResult);

				BHC_MEMO("predict X(%d) = %f\n", x, dValue);
			}


			// run
			PyObject *poResult;
			bool bIsProbability = false;	// 출력값이 확률값인지?
			switch (m_iClassifierIndex)
			{
			case _BHCPYTHON_CLASSIFIER_LDA_:
				OutputDebugString("CBhcPython::Predict() > lda\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_predict_lda", NULL); // 인스턴스에서 함수를 실행한다.
				break;
			case _BHCPYTHON_CLASSIFIER_QDA_:
				OutputDebugString("CBhcPython::Predict() > qda\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_predict_qda", NULL); // 인스턴스에서 함수를 실행한다.
				break;
			case _BHCPYTHON_CLASSIFIER_SOFTMAX_:
				OutputDebugString("CBhcPython::Predict() > softmax\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_predict_soft", NULL); // 인스턴스에서 함수를 실행한다.
				break;
			case _BHCPYTHON_CLASSIFIER_SVM_:
				OutputDebugString("CBhcPython::Predict() > svm\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_predict_svc", NULL); // 인스턴스에서 함수를 실행한다.
				break;
			case _BHCPYTHON_CLASSIFIER_RANDOMFOREST_:
				OutputDebugString("CBhcPython::Predict() > random forest\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_predict", NULL); // 인스턴스에서 함수를 실행한다.
				break;
			case _BHCPYTHON_CLASSIFIER_SOFTMAX_PROBA_:
				OutputDebugString("CBhcPython::Predict() > random forest\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_predict_soft_proba", NULL); // 인스턴스에서 함수를 실행한다.
				break;
			case _BHCPYTHON_CLASSIFIER_LOGISTIC_PROBA_:
				OutputDebugString("CBhcPython::Predict() > logistic(proba)\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstance, "ML_predict_logistic_proba", NULL); // 인스턴스에서 함수를 실행한다.
				bIsProbability = true;
				break;
			default:
				poResult = NULL;
				break;
			}

			// show result
			if (poResult != NULL) //리턴값
			{
				OutputDebugString("CBhcPython::Predict() > show result\n");

				int iDataNum = PyObject_Length(poResult);
				BHC_MEMO("CBhcPython::Test_class() > #result %d\n", iDataNum);

				for (int i = 0; i < iDataNum; i++)
				{
					PyObject *poTempResult = PySequence_GetItem(poResult, i);

					// store result
					if (bIsProbability == true)
					{	// 확률출력값은 double로 받는다.
						double dValue = PyFloat_AsDouble(poTempResult);
						pB[iResultCount] = (int)(dValue * 100);

						BHC_MEMO("CBhcPython::Test_class() > result[%d][%d]%f\n", y, i, dValue);
					}
					else 
					{	// class 출력값은 int로 받는다.
						long lValue = PyLong_AsLong(poTempResult);
						pB[y] = lValue;

						BHC_MEMO("CBhcPython::Test_class() > result[%d][i]%d\n", y, i, lValue);
					}

					Py_XDECREF(poTempResult);

					iResultCount++;
				}

				bResult = true;

				Py_XDECREF(poResult);
			}
		}
	}
	else
	{	//pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Predict() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Predict() > e\n");


	return bResult;
}
/*
bool CBhcPython::Predict(double *pA, int iCol, int iRow, int *pB)
{
	OutputDebugString("CBhcPython::Predict() > s\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Predict() > error > no init\n");

		return false;
	}

	bool bResult = false;

	if (m_pInstance != NULL)
	{
		OutputDebugString("CBhcPython::Predict() > fuc\n");

		// make params
		PyObject *poA = PyTuple_New(iCol*iRow);	//
		for (long y = 0; y < iRow; y++)
		{
			for (long x = 0; x < iCol; x++)
			{
				int iPos = x + y * iCol;
//				double dValue = pA[iPos];
				long lA = pA[iPos] * _BHC_PYTHON_UNIT_;

//				PyObject *poTempA = PyFloat_FromDouble(dValue);
				PyObject *poTempA = PyLong_FromLong(lA);
				PyTuple_SetItem(poA, iPos, poTempA);
				Py_XDECREF(poTempA);
			}
		}


		// run
		PyObject *pResult = PyObject_CallMethod((PyObject *)m_pInstance, "predict", "(iiO)", iCol, iRow, poA); // 인스턴스에서 함수를 실행한다.

		Py_XDECREF(poA);


		// show result
		if (pResult != NULL) //리턴값
		{
			OutputDebugString("CBhcPython::Predict() > show result\n");

			int iDataNum = PyObject_Length(pResult);
			BHC_MEMO("CBhcPython::Test_class() > #result %d\n", iDataNum);

			for (int i = 0; i < iDataNum; i++)
			{
				PyObject *poTempResult = PySequence_GetItem(pResult, i);

				long lValue = PyLong_AsLong(poTempResult);

				pB[i] = lValue;

				BHC_MEMO("CBhcPython::Test_class() > result[%d]%d\n", i, lValue);

				Py_XDECREF(poTempResult);
			}

			bResult = true;

			Py_XDECREF(pResult);
		}
	}
	else
	{ //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Predict() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Predict() > e\n");


	return bResult;
}
*/



/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CBhcPython::Predict(int iIndex, double *pA, int iCol, int iRow, int *pB)
{
	OutputDebugString("CBhcPython::Predict() > start\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Predict() > error > no init\n");

		return false;
	}

	bool bResult = false;

	if (m_pInstances[iIndex] != NULL)
	{
		// make params

		// demension 설정
		PyObject *poResult_1 = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_setDim", "ii", iRow, iCol); // 인스턴스에서 함수를 실행한다.
		Py_XDECREF(poResult_1);


		int iResultCount = 0;
		for (long y = 0; y < iRow; y++)
		{
			// make params
			// 데이터 입력
			for (long x = 0; x < iCol; x++)
			{
				double dValue = pA[x + y*iCol];

				PyObject *poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_setX", "id", x, dValue); // 인스턴스에서 함수를 실행한다.

				Py_XDECREF(poResult);

				BHC_MEMO("predict X(%d) = %f\n", x, dValue);
			}


			// run
			PyObject *poResult;
			bool bIsProbability = false;	// 출력값이 확률값인지?
			switch (m_iClassifierIndex)
			{
			case _BHCPYTHON_CLASSIFIER_LDA_:
				OutputDebugString("CBhcPython::Predict() > lda\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_predict_lda", NULL); // 인스턴스에서 함수를 실행한다.
				break;
			case _BHCPYTHON_CLASSIFIER_QDA_:
				OutputDebugString("CBhcPython::Predict() > qda\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_predict_qda", NULL); // 인스턴스에서 함수를 실행한다.
				break;
			case _BHCPYTHON_CLASSIFIER_SOFTMAX_:
				OutputDebugString("CBhcPython::Predict() > softmax\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_predict_soft", NULL); // 인스턴스에서 함수를 실행한다.
				break;
			case _BHCPYTHON_CLASSIFIER_SVM_:
				OutputDebugString("CBhcPython::Predict() > svm\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_predict_svc", NULL); // 인스턴스에서 함수를 실행한다.
				break;
			case _BHCPYTHON_CLASSIFIER_RANDOMFOREST_:
				OutputDebugString("CBhcPython::Predict() > random forest\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_predict", NULL); // 인스턴스에서 함수를 실행한다.
				break;
			case _BHCPYTHON_CLASSIFIER_SOFTMAX_PROBA_:
				OutputDebugString("CBhcPython::Predict() > random forest\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_predict_soft_proba", NULL); // 인스턴스에서 함수를 실행한다.
				break;
			case _BHCPYTHON_CLASSIFIER_LOGISTIC_PROBA_:
				OutputDebugString("CBhcPython::Predict() > logistic(proba)\n");
				poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "ML_predict_logistic_proba", NULL); // 인스턴스에서 함수를 실행한다.
				bIsProbability = true;
				break;
			default:
				poResult = NULL;
				break;
			}

			// get result
			if (poResult != NULL) //리턴값
			{
				int iDataNum = PyObject_Length(poResult);
				BHC_MEMO("CBhcPython::Test_class() > #result %d\n", iDataNum);

				for (int i = 0; i < iDataNum; i++)
				{
					PyObject *poTempResult = PySequence_GetItem(poResult, i);

					// store result
					if (bIsProbability == true)
					{	// 확률출력값은 double로 받는다.
						double dValue = PyFloat_AsDouble(poTempResult);
						pB[iResultCount] = (int)(dValue * 100);

						BHC_MEMO("CBhcPython::Test_class() > result[%d][%d]%f\n", y, i, dValue);
					}
					else 
					{	// class 출력값은 int로 받는다.
						long lValue = PyLong_AsLong(poTempResult);
						pB[y] = lValue;

						BHC_MEMO("CBhcPython::Test_class() > result[%d][i]%d\n", y, i, lValue);
					}

					Py_XDECREF(poTempResult);

					iResultCount++;
				}

				bResult = true;

				Py_XDECREF(poResult);
			}
			else {
				OutputDebugString("CBhcPython::Predict() > error > no result\n");
			}
		}
	}
	else
	{	//pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Predict() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Predict() > end\n");


	return bResult;
}


bool CBhcPython::Load(const char* pName)
{
	OutputDebugString("CBhcPython::Load() > s\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Load() > m_bInit > error\n");

		return false;
	}

	bool bResult = false;

	if (m_pInstance != NULL)
	{
		OutputDebugString("CBhcPython::Load() > fuc\n");

		PyObject *poResult = PyObject_CallMethod((PyObject *)m_pInstance, "load", "(s)", pName); // 인스턴스에서 함수를 실행한다.

		if (poResult != NULL) //리턴값
		{
			Py_XDECREF(poResult);

			OutputDebugString("CBhcPython::Load() > OK\n");
		}

		bResult = true;
	}
	else
	{	//pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Load() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Load() > e\n");


	return bResult;
}


//
// load 함수 실행
//
bool CBhcPython::Load(const char* pName, int iIndex)
{
	OutputDebugString("CBhcPython::Load() > start\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Load() > m_bInit > error\n");

		return false;
	}

	bool bResult = false;

	if (m_pInstances[iIndex] != NULL)
	{
		// 함수 실행
		PyObject *poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "load", "(s)", pName); // 인스턴스에서 함수를 실행한다.

		if (poResult != NULL) //리턴값
		{
			Py_XDECREF(poResult);
		}
		else {
			OutputDebugString("CBhcPython::Load() > error > no resut\n");
		}

		bResult = true;
	}
	else
	{	//pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Load() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Load() > end\n");


	return bResult;
}


bool CBhcPython::Save(const char* pName)
{
	OutputDebugString("CBhcPython::Save() > s\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Save() > m_bInit > error\n");

		return false;
	}

	bool bResult = false;

	if (m_pInstance != NULL)
	{
		OutputDebugString("CBhcPython::Save() > fuc\n");

		PyObject *poResult = PyObject_CallMethod((PyObject *)m_pInstance, "save", "(s)", pName); // 인스턴스에서 함수를 실행한다.

		if (poResult != NULL) //리턴값
		{
			Py_XDECREF(poResult);

			OutputDebugString("CBhcPython::Save() > OK\n");
		}

		bResult = true;
	}
	else
	{	//pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Save() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Load() > e\n");


	return bResult;
}



bool CBhcPython::Save(const char* pName, int iIndex)
{
	OutputDebugString("CBhcPython::Save() > start\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Save() > m_bInit > error\n");

		return false;
	}

	bool bResult = false;

	if (m_pInstances[iIndex] != NULL)
	{
		PyObject *poResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "save", "(s)", pName); // 인스턴스에서 함수를 실행한다.

		if (poResult != NULL) //리턴값
		{
			Py_XDECREF(poResult);
		}
		else {
			OutputDebugString("CBhcPython::Save() > error > no result\n");
		}

		bResult = true;
	}
	else
	{	//pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Save() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Load() > end\n");


	return bResult;
}



bool CBhcPython::SetClassifier(int iIndex)
{
	switch (iIndex)
	{
		case _BHCPYTHON_CLASSIFIER_LDA_ :
		case _BHCPYTHON_CLASSIFIER_QDA_:
		case _BHCPYTHON_CLASSIFIER_SOFTMAX_:
		case _BHCPYTHON_CLASSIFIER_SVM_:
		case _BHCPYTHON_CLASSIFIER_RANDOMFOREST_:
		case _BHCPYTHON_CLASSIFIER_SOFTMAX_PROBA_:
		case _BHCPYTHON_CLASSIFIER_LOGISTIC_PROBA_:
			m_iClassifierIndex = iIndex;
			return true;

		default:
			return false;
	}


	return false;
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Test_global
//
//	인스턴스내 함수를 실행한다.
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void CBhcPython::Set_global(int a)
{
	OutputDebugString("CBhcPython::Set_global() > start\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Set_global() > error > no init\n");

		return;
	}


	if (m_pInstance != NULL)
	{
		PyObject *pResult = PyObject_CallMethod((PyObject *)m_pModule, "set_global", "i", a); // 인스턴스에서 함수를 실행한다.

		// show result
		if (pResult != NULL) //리턴값
		{
			long lValue = PyLong_AsLong(pResult);

			Py_DECREF(pResult);

			BHC_MEMO("CBhcPython::Set_global() > OK > %d\n", lValue);
		}
		else {
			BHC_MEMO("CBhcPython::Set_global() > error > no result\n");
		}
	}
	else { //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Set_global() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Set_global() > end\n");
}



int CBhcPython::Get_global()
{
	long lVlaue = -1;

	OutputDebugString("CBhcPython::Get_global() > start\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Get_global() > error > no initialize\n");

		return lVlaue;
	}


	if (m_pModule != NULL)
	{
		// get function
		PyObject *pFunc = PyObject_GetAttrString((PyObject *)m_pModule, "get_global"); // test()라는 함수

		if (pFunc && PyCallable_Check(pFunc)) // callable한지 확인
		{
			// excute function
			PyObject *pValue = PyObject_CallObject(pFunc, NULL); // pFunc에 매개변수 전달해서 실행한다

			// show result
			if (pValue != NULL) //리턴값
			{
				lVlaue = PyLong_AsLong(pValue);

				Py_DECREF(pValue);

				BHC_MEMO("CBhcPython::Get_global() > OK > value is %d\n", lVlaue);
			}
			else {
				BHC_MEMO("CBhcPython::Get_global() > error > no return\n");
			}

			// void 형 함수는 여기로 바로 빠짐

			Py_DECREF(pFunc);
		}
		else {
			OutputDebugString("CBhcPython::Get_global() > error > Not callable\n");
		}
	}
	else { //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Get_global() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Get_global() > end\n");

	return lVlaue;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Test_global
//
//	numpy 모듈 동작 테스트
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void CBhcPython::Set_member(int a)
{
	OutputDebugString("CBhcPython::Set_member() > start\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Set_member() > error > no init\n");

		return;
	}


	if (m_pInstance != NULL)
	{
		// run
		PyObject *pResult = PyObject_CallMethod((PyObject *)m_pInstance, "set_member", "i", a); // 인스턴스에서 함수를 실행한다.


		// show result
		if (pResult != NULL) //리턴값
		{
			Py_XDECREF(pResult);

			OutputDebugString("CBhcPython::Set_member() > OK\n");
		} else {
			OutputDebugString("CBhcPython::Set_member() > error > no result\n");
		}
	}
	else { //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Set_member() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Set_member() > end\n");
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Test_global
//
//	numpy 모듈 동작 테스트
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void CBhcPython::Set_member(int iIndex, int a)
{
	OutputDebugString("CBhcPython::Set_member() > start\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Set_member() > error > no init\n");

		return;
	}


	if (m_pInstances[iIndex] != NULL)
	{
		// run
		PyObject *pResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "set_member", "i", a); // 인스턴스에서 함수를 실행한다.

		// show result
		if (pResult != NULL) //리턴값
		{
			Py_XDECREF(pResult);

			OutputDebugString("CBhcPython::Set_member() > OK\n");
		}
		else {
			OutputDebugString("CBhcPython::Set_member() > error > no result\n");
		}
	}
	else { //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Set_member() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Set_member() > end\n");
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Test_global
//
//	numpy 모듈 동작 테스트
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
int CBhcPython::Get_member()
{
	int iResult = -1;
	OutputDebugString("CBhcPython::Get_member() > start\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Get_member() > error > no init\n");

		return iResult;
	}


	if (m_pInstance != NULL)
	{
		// make params
		// run
		PyObject *pResult = PyObject_CallMethod((PyObject *)m_pInstance, "get_member", NULL); // 인스턴스에서 함수를 실행한다.


		// show result
		if (pResult != NULL) //리턴값
		{
			//iResult = PyObject_Length(pResult);

			iResult = PyLong_AsLong(pResult);

			Py_XDECREF(pResult);

			BHC_MEMO("CBhcPython::Get_member() > OK > return is %d\n", iResult);
		}
		else {
			OutputDebugString("CBhcPython::Get_member() > error > no result\n");
		}
	}
	else { //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Get_member() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Get_member() > end\n");

	return iResult;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Test_global
//
//	numpy 모듈 동작 테스트
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
int CBhcPython::Get_member(int iIndex)
{
	int iResult = -1;
	OutputDebugString("CBhcPython::Get_member() > start\n");

	if (m_bInit == false)
	{
		OutputDebugString("CBhcPython::Get_member() > error > no init\n");

		return iResult;
	}


	if (m_pInstances[iIndex] != NULL)
	{
		// make params
		// run
		PyObject *pResult = PyObject_CallMethod((PyObject *)m_pInstances[iIndex], "get_member", NULL); // 인스턴스에서 함수를 실행한다.


		// show result
		if (pResult != NULL) //리턴값
		{
			//iResult = PyObject_Length(pResult);

			iResult = PyLong_AsLong(pResult);

			Py_XDECREF(pResult);

			BHC_MEMO("CBhcPython::Get_member() > OK > return is %d\n", iResult);
		}
		else {
			OutputDebugString("CBhcPython::Get_member() > error > no result\n");
		}
	}
	else { //pModule 임포트 안됐을때 ㅠㅠ
		OutputDebugString("CBhcPython::Get_member() > error > pModule is null T.T\n");
	}


	OutputDebugString("CBhcPython::Get_member() > end\n");

	return iResult;
}




