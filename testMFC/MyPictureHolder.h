#pragma once
#include "Helper.h"
#include "MyButton.h"
#include "afxwin.h"

class  MyPictureHolder :
	public CStatic
{
public:
	MyPictureHolder();
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID, int index = -1);

	void ResizeRects(const RECT& parentRect);//初始定位矩形

	void ShowButtons(bool isToShow);//调整定位矩形
	void AdjustButtons();//调整按钮

	void ChangeMode();
	void UpdateCImage(int index = -1);
	void TransformCImage(int index = -1);

	void IncreaseFactor(float rotateAngle = 0, float scaleFactorX = 0, float scaleFactorY = 0, float shearMoveX = 0, float shearMoveY = 0);

	CPoint GetCenter();
	void SetCenter(CPoint center);
	
	~MyPictureHolder();

	CImage m_CImage;
private:
	CRect m_RectCImage;
	std::vector<CRect> m_RectButtons;
	std::vector<MyButton*> m_Buttons;
	int m_Width;

	float m_RotationAngle;//变化因子
	float m_ScaleFactorX;
	float m_ScaleFactorY;
	float m_ShearMoveX;
	float m_ShearMoveY;
	float m_cx;
	float m_cy;
	CRect m_OriginClientRect;

protected:
	//TODO:这个位置会影响偶
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point); 
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

void onMouseForOpencvWindow(int event, int x, int y, int flags, void*param);
