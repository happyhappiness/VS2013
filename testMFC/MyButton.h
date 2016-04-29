#pragma once
#include "afxwin.h"

enum RECTS { TopLeft, BottomRight, TopRight, BottomLeft, TopCenter, BottomCenter, CenterLeft, CenterRight, Center };

class MyButton :
	public CButton
{
public:
	MyButton();
	virtual BOOL Create( LPCTSTR lpszCaption,DWORD dwStyle,const RECT& rect,CWnd* pParentWnd,UINT nID);
	BOOL Create(const RECT& rect, CWnd* pParent, UINT nID, int type);

	void changeMode();
	void adjustSize(const CRect& rect);//调整位置大小
	void updateCursor();

	~MyButton();

private:
	int m_Type;
	bool m_IsRotate;
	bool m_IsManulating;

protected:	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

