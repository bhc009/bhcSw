// CLearningDialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "BHC_featureAnalysis.h"
#include "LearningDialog.h"
#include "afxdialogex.h"
#include "../common/grid/GridCellCheck.h"

#include <algorithm>
#include <string>


void stringToChar(std::string *str, TCHAR *ch, int iMaxSize)
{
	std::vector<char> writable(str->begin(), str->end());
	writable.push_back('\0');
	char* ptr = &writable[0];


	int len = MultiByteToWideChar(CP_ACP, 0, ptr, -1, NULL, 0);
	// 	WCHAR wBuf[2048];
	MultiByteToWideChar(CP_ACP, 0, ptr, strlen(ptr) + 1, ch, len);

}



// CLearningDialog 대화 상자

IMPLEMENT_DYNAMIC(CLearningDialog, CDialog)

CLearningDialog::CLearningDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_LEARNING, pParent)
	, m_iTotal(0)
	, m_iDefectNum(0)
	, m_iOverkillNum(0)
	, m_iMissNum(0)
	, m_iNoneNum(0)
	, m_uiClassifier(0)
	, m_iProbabilityLimit(0)
{
	m_iWidth	= 0;
	m_iHeight	= 0;
}

CLearningDialog::~CLearningDialog()
{
}

void CLearningDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CLASS_COLUMN, m_cbClassColumn);
	DDX_Control(pDX, IDC_COMBO_CLASS_1, m_cbClass_1);
	DDX_Text(pDX, IDC_EDIT5, m_iTotal);
	DDX_Text(pDX, IDC_EDIT1, m_iDefectNum);
	DDX_Text(pDX, IDC_EDIT2, m_iOverkillNum);
	DDX_Text(pDX, IDC_EDIT3, m_iMissNum);
	DDX_Text(pDX, IDC_EDIT4, m_iNoneNum);
	DDX_Radio(pDX, IDC_RADIO_CLASSIFIER_LDA, (int&)m_uiClassifier);
	DDX_Text(pDX, IDC_EDIT_PROBABILITY, m_iProbabilityLimit);
	DDV_MinMaxInt(pDX, m_iProbabilityLimit, 0, 100);
}


BEGIN_MESSAGE_MAP(CLearningDialog, CDialog)
	ON_WM_NCDESTROY()
	ON_BN_CLICKED(IDC_BUTTON_TRAIN, &CLearningDialog::OnBnClickedButtonTrain)
	ON_CBN_SELCHANGE(IDC_COMBO_CLASS_COLUMN, &CLearningDialog::OnCbnSelchangeComboClassColumn)
	ON_BN_CLICKED(IDC_BUTTON_PREDICT, &CLearningDialog::OnBnClickedButtonPredict)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CLearningDialog::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CLearningDialog::OnBnClickedButtonLoad)
END_MESSAGE_MAP()


// CLearningDialog 메시지 처리기


void CLearningDialog::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete this;
}


void CLearningDialog::init()
{
	{
		CRect rtVeiwRegion;
		GetDlgItem(IDC_STATIC_DATA)->GetWindowRect(&rtVeiwRegion);

		ScreenToClient(rtVeiwRegion);

		m_Grid.Create(rtVeiwRegion, this, IDC_STATIC_DATA);

		initGrid();
	}

}



void CLearningDialog::initGrid()
{
	// index 열/행 생성
	m_Grid.SetFixedColumnCount(0);
	m_Grid.SetFixedRowCount(0);

	// data 갯수 설정
	m_Grid.SetColumnCount(3);
	m_Grid.SetRowCount(50);

	// 색설정
	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

	m_Grid.SetHeaderSort(TRUE);


	// set data
//	m_Grid.SetCellType(1, 1, RUNTIME_CLASS(CGridCell));
//	m_Grid.SetItemText(1, 1, _T("1"));
//	m_Grid.SetItemState(1, 1, m_Grid.GetItemState(1, 1) & ~GVIS_READONLY);
	m_Grid.Invalidate();
}

////////////////////////////////////////////////////////////////////
//
//	SetData
//
//	데이터 설정
//
//
////////////////////////////////////////////////////////////////////
bool CLearningDialog::SetData(std::vector<std::string> *pColumn, std::vector<std::string> *pContents, int iWidth, int iHeight)
{
	// column 길이 체크
	int iSzCol = pColumn->size();
	if (iSzCol != iWidth)
	{
		return false;
	}

	// 데이터 크기 체크
	int iSzContents = pContents->size();
	if (iSzContents != iWidth * iHeight)
	{
		return false;
	}

	// Set column data
	m_vecColumn		= *pColumn;

	// Set contents
	m_vecContents	= *pContents;


	m_iWidth	= iWidth;
	m_iHeight	= iHeight;


	// Update grid
	Update_featureGrid();

	// Update class
	Update_classColumn();

	//
	Update_targetClass();

	return true;
}


////////////////////////////////////////////////////////////////////
//
//	SetTypeColumn
//
//	불량 type을 정의한 column 설정
//
//
////////////////////////////////////////////////////////////////////
bool CLearningDialog::SetTypeColumn(std::string strType)
{
	return true;
}


////////////////////////////////////////////////////////////////////
//
//	clear
//
//	데이터 초기화
//
//
////////////////////////////////////////////////////////////////////
void CLearningDialog::clear()
{
	m_vecColumn.clear();
	m_vecContents.clear();
}


////////////////////////////////////////////////////////////////////
//
//	Train
//
//	학습
//
//
////////////////////////////////////////////////////////////////////
bool CLearningDialog::Train()
{
	return true;
}


////////////////////////////////////////////////////////////////////
//
//	Predict
//
//	예측
//
//
////////////////////////////////////////////////////////////////////
bool CLearningDialog::Predict()
{
	BHC_MEMO("CLearningDialog::Predict() > start\n");

#ifdef _USE_PYTHON_
	// check
	if (CheckStatus() == false)
	{
		BHC_MEMO("CLearningDialog::Predict() > CheckStatus > error\n");
		return false;
	}


	//
	// set info
	//
	// 폭
	int iColNum = 0;
	if (GetCount_selectedFeature(&iColNum) == false)
	{
		return false;
	}

	// 높이
	int iRowNum = m_iHeight;



	//
	// make input matrix
	//
	double *pdA = new double[iColNum * iRowNum];
	memset(pdA, 0, sizeof(double) * iColNum * iRowNum);

	int iPos = 0;
	for (int y = 0; y < m_iHeight; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			// check : 선택한 feature인가?
			if (IsSelectedFeature(x) == false)
			{
				continue;
			}

			// get value
			double dValue = atof(m_vecContents[x + y * m_iWidth].c_str());


			// set
			pdA[iPos] = dValue;

			// update position
			iPos++;
		}
	}


	//
	// predict
	//
	int *piB = new int[iRowNum];
	memset(piB, 0, sizeof(int)*iRowNum);

	int *pProba = new int[2 * iRowNum];	// 확률 저장 버퍼
	memset(pProba, 0, sizeof(int)*2*iRowNum);

	bool bError = false;
	switch (GetClassIndex())
	{
		case _BHC_CLASSIFIER_LDA_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_LDA_);
			if (m_pPython->Predict(pdA, iColNum, iRowNum, piB) == false)
			{
				bError = true;
				BHC_MEMO("CLearningDialog::OnBnClickedButtonPredict() > Predict > error\n");
			}
			break;

		case _BHC_CLASSIFIER_QDA_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_QDA_);
			if (m_pPython->Predict(pdA, iColNum, iRowNum, piB) == false)
			{
				bError = true;
				BHC_MEMO("CLearningDialog::OnBnClickedButtonPredict() > Predict > error\n");
			}
			break;

		case _BHC_CLASSIFIER_SOFTMAX_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_SOFTMAX_);
			if (m_pPython->Predict(pdA, iColNum, iRowNum, piB) == false)
			{
				bError = true;
				BHC_MEMO("CLearningDialog::OnBnClickedButtonPredict() > Predict > error\n");
			}
			break;

		case _BHC_CLASSIFIER_SVM_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_SVM_);
			if (m_pPython->Predict(pdA, iColNum, iRowNum, piB) == false)
			{
				bError = true;
				BHC_MEMO("CLearningDialog::OnBnClickedButtonPredict() > Predict > error\n");
			}
			break;

		case _BHC_CLASSIFIER_RANDOMFOREST_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_RANDOMFOREST_);
			if (m_pPython->Predict(pdA, iColNum, iRowNum, piB) == false)
			{
				bError = true;
				BHC_MEMO("CLearningDialog::OnBnClickedButtonPredict() > Predict > error\n");
			}
			break;

		case _BHC_CLASSIFIER_SOFTMAX_PROBA_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_SOFTMAX_PROBA_);
			if (m_pPython->Predict(pdA, iColNum, iRowNum, pProba) == false)
			{
				bError = true;
				BHC_MEMO("CLearningDialog::OnBnClickedButtonPredict() > Predict > error\n");
			}

			// 확률로 분류
			for (int i = 0; i < iRowNum; i++)
			{
				// class 갯수 확인 필요
				// 일단 아래는 2개 class라 가정
				if (pProba[2 * i + 1] >= GetProbabilityLimit())
				{	// 특정 확률 이상일때 targt 불량으로 처리
					piB[i] = 1;
				}
				else {
					piB[i] = 0;
				}
			}

			break;

		case _BHC_CLASSIFIER_LOGISTIC_PROBA_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_LOGISTIC_PROBA_);
			if (m_pPython->Predict(pdA, iColNum, iRowNum, pProba) == false)
			{
				bError = true;
				BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > train > error\n");
			}

			// 확률로 분류
			for (int i = 0; i < iRowNum; i++)
			{
				if (pProba[2 * i + 1] >= GetProbabilityLimit())
				{	// 특정 확률 이상일때 target 불량으로 처리
					piB[i] = 1;
				}
				else {
					piB[i] = 0;
				}
			}

			break;

		default:
			bError = true;
			break;
	}
			
	delete[] pProba;


	if (bError == true)
	{
		delete[] pdA;
		delete[] piB;

		BHC_MEMO("CLearningDialog::Predict() > Predict > error\n");
		return false;
	}




	//
	// score
	//	1) Target 불량명을 얻는다.
	//	2) Class 정보가 있는 column의 index를 얻는다. 
	//	3) score 계산
	//
	// 1) get target name
	std::string strTargetClass;
	if (GetTargetName(&strTargetClass) == false)
	{
		delete[] pdA;
		delete[] piB;

		BHC_MEMO("CLearningDialog::Predict() > GetTargetName > error\n");
		return false;
	}

	// 2) get index of class column 
	int iIndex_classColumn = 0;
	if (GetIndex_classColumn(&iIndex_classColumn) == false)
	{
		delete[] pdA;
		delete[] piB;

		BHC_MEMO("CLearningDialog::Predict() > GetIndex_classColumn > error\n");
		return false;
	}

	// 3) 
	int iTotal = 0;
	int iDefect = 0;
	int iMiss = 0;
	int iOver = 0;
	int iNone = 0;
	for (int y = 0; y < m_iHeight; y++)
	{
		// 
		bool bTarget;
		if (strTargetClass == m_vecContents[iIndex_classColumn + y * m_iWidth])
		{
			bTarget = true;
		}
		else {
			bTarget = false;
		}


		//
		if (bTarget == true)
		{
			if (piB[y] == 1)
			{	// 불량을 불량으로
				iDefect++;
			}
			else 
			{	// 불량을 정상으로
				iMiss++;
			}
		}
		else 
		{
			if (piB[y] == 1)
			{	// 정상을 불량으로
				iOver++;
			}
			else
			{	// 정상을 정상으로
				iNone++;
			}
		}
		iTotal++;
	}

	BHC_MEMO("Prediction result : total = %d, defect = %d, miss = %d, over = %d, none = %d", iTotal, iDefect, iMiss, iOver, iNone);


	// Update UI
	m_iTotal		= iTotal;
	m_iDefectNum	= iDefect;
	m_iMissNum		= iMiss;
	m_iOverkillNum	= iOver;
	m_iNoneNum		= iNone;

	UpdateData(FALSE);



	delete[] pdA;
	delete[] piB;

#endif


	BHC_MEMO("CLearningDialog::Predict() > end\n");
	return true;
}


double *CLearningDialog::GetTrainData(int *piWidth, int *piHeight)
{
	return NULL;
}


////////////////////////////////////////////////////////////////////
//
//	UpdateGrid
//
//	예측
//
//
////////////////////////////////////////////////////////////////////
bool CLearningDialog::Update_featureGrid()
{
	// check
	if (CheckStatus() == false)
	{
		return false;
	}
//	if (m_iWidth < 1 || m_iHeight < 1 || m_vecColumn.size() != m_iWidth || m_vecContents.size() != m_iWidth * m_iHeight)
//	{
//		return false;
//	}


	// make 
	int iWidth_grid		= 2;
	int iHeight_grid	= m_iWidth;
	m_Grid.SetColumnCount(iWidth_grid);
	m_Grid.SetRowCount(iHeight_grid);



	// set column 
	int iIndexX = 1;
	for (int y = 0 ; y < m_iWidth; y++)
	{
		// set 1st column : check 
		m_Grid.SetCellType(y, 0, RUNTIME_CLASS(CGridCellCheck));

		// set 2nd column : feature name
		std::string str = m_vecColumn[y];

		WCHAR wBuf[2048];
		stringToChar(&str, wBuf, 2048);

		m_Grid.SetItemText(y, iIndexX, wBuf);
	}

	// set cell width
	m_Grid.AutoSizeColumns(GVS_BOTH);


	// 갱신 : grid
	m_Grid.Invalidate();
}


////////////////////////////////////////////////////////////////////
//
//	Update_typeColumn
//
//	ㅇ 학습
//
//
////////////////////////////////////////////////////////////////////
bool CLearningDialog::Update_classColumn()
{
	// check condition
	if (CheckStatus() == false)
	{
		return false;
	}


	m_cbClassColumn.ResetContent();

	int iPos = 0;
	for (std::vector<std::string>::iterator ite = m_vecColumn.begin(); ite != m_vecColumn.end(); ite++)
	{
		std::string str = *ite;

		WCHAR wBuf[2048];
		stringToChar(&str, wBuf, 2048);

		m_cbClassColumn.InsertString(iPos, wBuf);

		iPos++;
	}


	//
	m_cbClassColumn.SetCurSel(0);
}


////////////////////////////////////////////////////////////////////
//
//	Update_classes
//
//	ㅇ 분류에 사용한 클래스를 UI에 입력한다.
//
//
////////////////////////////////////////////////////////////////////
bool CLearningDialog::Update_targetClass()
{
	// check condition
	if (CheckStatus() == false)
	{
		return false;
	}


	// type이 정의된 column 얻기
	int iIndexTypeColumn = 0;
	if (GetIndex_classColumn(&iIndexTypeColumn) == false)
	{
		return false;
	}

	// 해당 column data 얻기
	std::vector<std::string> vecUniqueType;
	for (int y = 0; y < m_iHeight; y++)
	{
		int iPos = iIndexTypeColumn + y*m_iWidth;
		vecUniqueType.push_back(m_vecContents[iPos]);
	}


	// 중복 type 제거
	std::sort(vecUniqueType.begin(), vecUniqueType.end());
//	std::vector<int> aaa;
//	auto last = std::unique(vecUniqueType.begin(), vecUniqueType.end());
	vecUniqueType.erase(std::unique(vecUniqueType.begin(), vecUniqueType.end()), vecUniqueType.end());


	// push data
	m_cbClass_1.ResetContent();
	m_strClasses.clear();

	int iPos = 0;
	for (std::vector<std::string>::iterator ite = vecUniqueType.begin(); ite != vecUniqueType.end(); ite++)
	{
		std::string str = *ite;

		WCHAR wBuf[2048];
		stringToChar(&str, wBuf, 2048);

		m_cbClass_1.InsertString(iPos, wBuf);
		m_strClasses.push_back(*ite);

		iPos++;
	}

	m_cbClass_1.SetCurSel(0);
	
}


////////////////////////////////////////////////////////////////////
//
//	CheckStatus
//
//	ㅇ 본 클래스의 기능이 동작이 가능한 상태인지 점검한다.
//
//
////////////////////////////////////////////////////////////////////
bool CLearningDialog::CheckStatus()
{
	if (m_iWidth < 1 || m_iHeight < 1 || m_vecColumn.size() != m_iWidth || m_vecContents.size() != m_iWidth * m_iHeight)
	{
		return false;
	}

	return true;
}


////////////////////////////////////////////////////////////////////
//
//	GetTypeIndex
//
//	ㅇ type이 있는 column의 index를 가져온다.
//
//
////////////////////////////////////////////////////////////////////
bool CLearningDialog::GetIndex_classColumn(int *piIndex)
{
//	if (m_cbClassColumn.GetStackSize() == 0)
//	{
//		*piIndex = -1;
//		return false;
//	}


	*piIndex = m_cbClassColumn.GetCurSel();

	return true;
}



////////////////////////////////////////////////////////////////////
//
//	GetValidColumnCount
//
//	ㅇ 유효한 feature의 갯수를 가져온다.
//		- UI에서 체크버튼으로 선택한 feature
//
//
////////////////////////////////////////////////////////////////////
bool CLearningDialog::GetCount_selectedFeature(int *piCount)
{
	// check
	if (CheckStatus() == false)
	{
		return false;
	}


	//
	int iCount = 0;
	for (int y = 0; y < m_iWidth; y++)
	{
		CGridCellCheck* pCell = (CGridCellCheck*)m_Grid.GetCell(y, 0);	// 첫번째 열 고정
		BOOL bCheck = pCell->GetCheck();

		//if (bCheck)
		if (IsSelectedFeature(y))
		{
			iCount++;

			BHC_MEMO("\n CLearningDialog::GetValidColumnCount > %dth feature checked", y);
		}
	}


	*piCount = iCount;	// save


	// check
	if(iCount == 0)
	{
		return false;
	}


	return true;
}


////////////////////////////////////////////////////////////////////
//
//	IsValidColumn
//
//	ㅇ 선택한 feature인지를 리턴한다.
//
////////////////////////////////////////////////////////////////////
bool CLearningDialog::IsSelectedFeature(int i)
{
	CGridCellCheck* pCell = (CGridCellCheck*)m_Grid.GetCell(i, 0);	// 첫번째 열 고정
	BOOL bCheck = pCell->GetCheck();


	if (bCheck == FALSE)
	{
		return false;
	}


	return true;
}



////////////////////////////////////////////////////////////////////
//
//	GetProbabilityLimit
//
//	ㅇ 예측시 사용할 확률에 대한 기준치를 얻는다.
//
//	ㅇ 범위 : 0 ~ 100
//
////////////////////////////////////////////////////////////////////
int CLearningDialog::GetProbabilityLimit()
{
	m_iProbabilityLimit = GetDlgItemInt(IDC_EDIT_PROBABILITY);

	return m_iProbabilityLimit;
}


////////////////////////////////////////////////////////////////////
//
//	GetTargetName
//
//	ㅇ Target 불량명을 얻는다.
//
////////////////////////////////////////////////////////////////////
bool CLearningDialog::GetTargetName(std::string *pStr )
{
	int iTargetIndex = m_cbClass_1.GetCurSel();


	if (iTargetIndex >= m_strClasses.size())
	{
		return false;
	}


	*pStr = m_strClasses[iTargetIndex];


	return true;
}


int CLearningDialog::GetClassIndex()
{
	if (IsDlgButtonChecked(IDC_RADIO_CLASSIFIER_LDA))
	{
		m_uiClassifier = _BHC_CLASSIFIER_LDA_;
		return _BHC_CLASSIFIER_LDA_;
	}

	if (IsDlgButtonChecked(IDC_RADIO_CLASSIFIER_QDA))
	{
		m_uiClassifier = _BHC_CLASSIFIER_QDA_;
		return _BHC_CLASSIFIER_QDA_;
	}

	if (IsDlgButtonChecked(IDC_RADIO_CLASSIFIER_SOFTMAX))
	{
		m_uiClassifier = _BHC_CLASSIFIER_SOFTMAX_;
		return _BHC_CLASSIFIER_SOFTMAX_;
	}

	if (IsDlgButtonChecked(IDC_RADIO_CLASSIFIER_SVM))
	{
		m_uiClassifier = _BHC_CLASSIFIER_SVM_;
		return _BHC_CLASSIFIER_SVM_;
	}

	if (IsDlgButtonChecked(IDC_RADIO_CLASSIFIER_RANDOM_FOREST))
	{
		m_uiClassifier = _BHC_CLASSIFIER_RANDOMFOREST_;
		return _BHC_CLASSIFIER_RANDOMFOREST_;
	}

	if (IsDlgButtonChecked(IDC_RADIO_CLASSIFIER_SOFTMAX_PROBABILITY))
	{
		m_uiClassifier = _BHC_CLASSIFIER_SOFTMAX_PROBA_;
		return _BHC_CLASSIFIER_SOFTMAX_PROBA_;
	}

	if (IsDlgButtonChecked(IDC_RADIO_CLASSIFIER_LOGISTIC))
	{
		m_uiClassifier = _BHC_CLASSIFIER_LOGISTIC_PROBA_;
		return _BHC_CLASSIFIER_LOGISTIC_PROBA_;
	}

	return _BHC_CLASSIFIER_NONE_;
}



////////////////////////////////////////////////////////////////////
//
//	OnBnClickedButtonTrain
//
//	ㅇ 학습
//		Ax = B
//		A : feature
//		B : class
//
////////////////////////////////////////////////////////////////////
void CLearningDialog::OnBnClickedButtonTrain()
{
#ifdef _USE_PYTHON_
	BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > start\n");
	if (CheckStatus() == false)
	{
		BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > CheckStatus > error\n");
		return;
	}


	//
	// make matrix A
	//
	// get valid column(feature) count
	int nValidColumn = 0;
	if (GetCount_selectedFeature(&nValidColumn) == false)
	{
		BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > GetValidColumnCount > error\n");
		return;
	}

	// set matrix dimension
	int iColNum = nValidColumn;
	int iRowNum = m_iHeight;
	int iDim = iColNum * iRowNum;

	// make matrix A
	double *pdA = new double[iColNum * iRowNum];

	int iPos = 0;
	for (int y = 0; y < m_iHeight; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			// check
			if (IsSelectedFeature(x) == false)
			{
				continue;
			}

			// get
			double dValue = atof(m_vecContents[x + y * m_iWidth].c_str());


			// *
			if (iPos >= iDim)
			{
				BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > make matrix A > error\n");
				continue;
			}

			// set
			pdA[iPos] = dValue;

			// update position
			iPos++;
		}
	}


	//
	// make matrix B
	//
	// get target name
	int iTargetIndex = m_cbClass_1.GetCurSel();
	std::string strTarget = m_strClasses[iTargetIndex];

	// get index of type column 
	int iTypeIndex = 0;
	if (GetIndex_classColumn(&iTypeIndex) == false)
	{
		BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > GetTypeIndex > error\n");
		return;
	}

	// make matrix
	int *piB = new int[iRowNum];
	for (int y = 0; y < m_iHeight; y++)
	{
		if (strTarget == m_vecContents[iTypeIndex + y * m_iWidth])
		{
			piB[y] = 1;
		}
		else {
			piB[y] = 0;
		}
	}


	//
	// train
	//
	switch (GetClassIndex())
	{
		case _BHC_CLASSIFIER_LDA_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_LDA_);
			if (m_pPython->Fit(pdA, piB, iColNum, iRowNum) == false)
			{
				BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > train > error\n");
			}
			break;

		case _BHC_CLASSIFIER_QDA_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_QDA_);
			if (m_pPython->Fit(pdA, piB, iColNum, iRowNum) == false)
			{
				BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > train > error\n");
			}
			break;

		case _BHC_CLASSIFIER_SOFTMAX_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_SOFTMAX_);
			if (m_pPython->Fit(pdA, piB, iColNum, iRowNum) == false)
			{
				BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > train > no classifier\n");
			}
			break;

		case _BHC_CLASSIFIER_SVM_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_SVM_);
			if (m_pPython->Fit(pdA, piB, iColNum, iRowNum) == false)
			{
				BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > train > error\n");
			}
			break;

		case _BHC_CLASSIFIER_RANDOMFOREST_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_RANDOMFOREST_);
			if (m_pPython->Fit(pdA, piB, iColNum, iRowNum) == false)
			{
				BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > train > error\n");
			}
			break;

		case _BHC_CLASSIFIER_SOFTMAX_PROBA_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_SOFTMAX_PROBA_);
			if (m_pPython->Fit(pdA, piB, iColNum, iRowNum) == false)
			{
				BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > train > error\n");
			}
			break;

		case _BHC_CLASSIFIER_LOGISTIC_PROBA_:
			m_pPython->SetClassifier(_BHCPYTHON_CLASSIFIER_LOGISTIC_PROBA_);
			if (m_pPython->Fit(pdA, piB, iColNum, iRowNum) == false)
			{
				BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > train > error\n");
			}
			break;

		default:
			break;
	}


	delete[] pdA;
	delete[] piB;


	BHC_MEMO("CLearningDialog::OnBnClickedButtonTrain() > end\n");

#endif
}


void CLearningDialog::OnCbnSelchangeComboClassColumn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Update_targetClass();
}


void CLearningDialog::OnBnClickedButtonPredict()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Predict();
}


void CLearningDialog::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
#ifdef _USE_PYTHON_
	if (m_pPython->Save("d:/model.pkl") == false)
	{
		BHC_MEMO("CLearningDialog::OnBnClickedButtonSave() > error\n");
	}
#endif
}


void CLearningDialog::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
#ifdef _USE_PYTHON_
	m_pPython->Load("d:/model.pkl");
#endif
}
