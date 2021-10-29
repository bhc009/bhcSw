#pragma once



// CBHC_graph2D 뷰입니다.
#include <list>

#define _BHC_DIR_WORLD_TO_LOCAL_	0
#define _BHC_DIR_LOCAL_TO_WORLD_	1

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

class CBHC_graph2D : public CScrollView
{
	DECLARE_DYNCREATE(CBHC_graph2D)

protected:
	CBHC_graph2D();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CBHC_graph2D();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	DECLARE_MESSAGE_MAP()


public:
	void SetRange( double dX_min, double dX_max, double dY_min, double dY_max );
	void SetGrid( double dStepX, double dStepY );
	void SetSize( int x, int y );

	void ClearData();
	void AddData(double x, double y);
	void AddData2(double x, double y);

protected:

	CSize m_szTotal;
	int m_iEdgeSize;

	double m_dX_min;
	double m_dX_max;
	double m_dY_min;
	double m_dY_max;

	double m_dStepX_sub;
	double m_dStepY_sub;

	std::list<stPoint2D> m_listPoint;
	std::list<stPoint2D> m_listPoint2;

	void Draw_mainAxis(CDC* pDC);
	void Draw_subAxis(CDC* pDC);
	void ConvertCoordinate(int iDirection, double dX_in, double dY_in, double *pdX, double *pdY);



};


