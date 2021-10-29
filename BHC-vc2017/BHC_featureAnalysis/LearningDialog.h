#pragma once

#include "../common/grid/GridCtrl.h"
#include "../BhcPython/BhcPython.h"

#include <vector>

#define _USE_PYTHON_


#define _BHC_CLASSIFIER_NONE_			-1
#define _BHC_CLASSIFIER_LDA_			0
#define _BHC_CLASSIFIER_QDA_			1
#define _BHC_CLASSIFIER_SOFTMAX_		2
#define _BHC_CLASSIFIER_SVM_			3
#define _BHC_CLASSIFIER_RANDOMFOREST_	4
#define _BHC_CLASSIFIER_SOFTMAX_PROBA_	5
#define _BHC_CLASSIFIER_LOGISTIC_PROBA_	6

// CLearningDialog 대화 상자

class CLearningDialog : public CDialog
{
	DECLARE_DYNAMIC(CLearningDialog)

public:
	CLearningDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLearningDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LEARNING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcDestroy();
	afx_msg void OnBnClickedButtonTrain();
	afx_msg void OnCbnSelchangeComboClassColumn();
	afx_msg void OnBnClickedButtonPredict();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();


public:
	void init();
	bool SetData(std::vector<std::string> *pColumn, std::vector<std::string> *pContents, int iWidth, int iHeight);
	bool SetTypeColumn(std::string strType);

#ifdef _USE_PYTHON_
	CBhcPython *m_pPython;	// 파이썬 모듈
#endif

protected:
	int m_iWidth;	// data의 폭
	int m_iHeight;	// data의 높이
	std::vector<std::string> m_vecColumn;	// column data
	std::vector<std::string> m_vecContents;	// contents
	std::vector<std::string> m_strClasses;	// 사용 가능한 defect type

	CComboBox m_cbClassColumn;	// class를 정의한 column 선택
	CComboBox m_cbClass_1;		// 분류할 class 선택 

	CGridCtrl m_Grid;	// data view
	void initGrid();

	void clear();

	bool Train();	// 학습
	bool Predict();	// 예측

	double *GetTrainData(int *piWidth, int *piHeight);

	bool Update_featureGrid();
	bool Update_classColumn();
	bool Update_targetClass();


	bool CheckStatus();
	bool GetIndex_classColumn(int *piIndex);
	bool GetCount_selectedFeature(int *piCount);
	bool IsSelectedFeature(int i);
	bool GetTargetName(std::string *pStr);
	int GetClassIndex();
	int GetProbabilityLimit();

public:
	unsigned int m_iTotal;
	unsigned int m_iDefectNum;
	unsigned int m_iOverkillNum;
	unsigned int m_iMissNum;
	unsigned int m_iNoneNum;
	UINT m_uiClassifier;
	int m_iProbabilityLimit;
};
