#pragma once

#define _BHC_PYTHON_UNIT_	(1000)	// 실수를 정수화하는 단위(= 3자리까지 사용한다.)


#ifdef BHC_VISION_EXPORT
#define DLLTYPE_H __declspec(dllexport)
#else
#define DLLTYPE_H __declspec(dllimport)
#endif

#define _BHCPYTHON_CLASSIFIER_LDA_				0
#define _BHCPYTHON_CLASSIFIER_QDA_				1
#define _BHCPYTHON_CLASSIFIER_SOFTMAX_			2
#define _BHCPYTHON_CLASSIFIER_SVM_				3
#define _BHCPYTHON_CLASSIFIER_RANDOMFOREST_		4
#define _BHCPYTHON_CLASSIFIER_SOFTMAX_PROBA_	5
#define _BHCPYTHON_CLASSIFIER_LOGISTIC_PROBA_	6	// 2진 분류기 확률

#define _BHCPYTHON_MAX_INSTANCE_				10

class DLLTYPE_H CBhcPython
{
public:
	CBhcPython(void);
	virtual ~CBhcPython(void);

	void initialize(const char* pModuleName);	// 

	void Test();
	void Test_global();
	void Test_class();
	void Test_float();

	void Set_global(int a);
	int Get_global();

	void Set_member(int a);
	int Get_member();

	void Set_member(int iIndex, int a);
	int Get_member(int iIndex);


	// learn & fit
	bool Fit(double *pA, int *pB, int iCol, int iRow);
	bool Predict(double *pA, int iCol, int iRow, int *pB);

	bool Fit(int iIndex, double *pA, int *pB, int iCol, int iRow);
	bool Predict(int iIndex, double *pA, int iCol, int iRow, int *pB);

	// save model
	bool Load(const char* pName);	// 
	bool Save(const char* pName);	// 

	bool Load(const char* pName, int iIndex);	// 
	bool Save(const char* pName, int iIndex);	// 

	// set classifier
	bool SetClassifier(int iIndex);

protected:
	bool m_bInit;

	void *m_pModule;	// file
	void *m_pClass;		// class
	void *m_pInstance;	// class instance
	void *m_pInstances[_BHCPYTHON_MAX_INSTANCE_];	// class instance

	int m_iClassifierIndex;	// 사용 classifier에 대한 정의

	int m_iCnt;
};
