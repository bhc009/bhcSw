#pragma once



// CBHC_graph2D 뷰입니다.
#include <list>

#define _BHC_DIR_WORLD_TO_LOCAL_	0
#define _BHC_DIR_LOCAL_TO_WORLD_	1

#define WM_INVALIDATE_VIWE		WM_USER+1


struct stPtDisplay
{
	double x;
	double y;

	int iRed;
	int iGreen;
	int iBlue;

	int iIndex;

	bool bSelect;

	stPtDisplay()
	{
		x = 0;
		y = 0;

		iRed	= 0;
		iGreen	= 255;
		iBlue	= 0;

		iIndex = 0;

		bSelect = false;
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
	void SetPointRadius( int iRadius );
	void SetFilter( bool bUse, int iIndex );

	void SetMessage( CWnd *pWnd, UINT uiMessage );

	void Reset();

	std::list<unsigned int> GetIndex_selection();

	void ClearData();
	void AddData(double x, double y);
	void AddData2(double x, double y);
	void AddData(double x, double y, int iRed, int iGreen, int iBlue, int iIndex);

protected:

	CSize m_szTotal;
	int m_iEdgeSize;

	// 원본 기준 축값
	double m_dX_min_world;
	double m_dX_max_world;
	double m_dY_min_world;
	double m_dY_max_world;

	// 화면 기준 축값
	double m_dX_min_local;
	double m_dX_max_local;
	double m_dY_min_local;
	double m_dY_max_local;

	double m_dStepX_sub;
	double m_dStepY_sub;

	std::list<stPtDisplay> m_listPoint;
	std::list<stPtDisplay> m_listPoint2;

	int m_iRadius_point;

	bool m_bUseFilter;
	int m_iFilterIndex;

	void Draw_mainAxis(CDC* pDC);
	void Draw_subAxis(CDC* pDC);
	void ConvertCoordinate(int iDirection, double dX_in, double dY_in, double *pdX, double *pdY);


	// 
	void ZoomIn();
	void ZoomOut();
	double GetZoomRatio();

	int m_iZoom;


	// mouse
	CPoint m_ptLButtonDown;
	CPoint m_ptCurrent;
	bool m_bLButtonDown;


	//
	CWnd *m_pParent;
	UINT m_uiMessage;

public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnReceive( WPARAM, LPARAM );
};


