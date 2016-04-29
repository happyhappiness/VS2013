#include "stdafx.h"
#include "MyButton.h"
#include "MyPictureHolder.h"
#include "testMFCView.h"

using namespace cv;

MyButton::MyButton()
{
}


MyButton::~MyButton()
{
}


BOOL MyButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);
}

BOOL MyButton::Create(const RECT& rect, CWnd* pParent, UINT nID, int type)
{
	if (Create(NULL, WS_CHILD | WS_VISIBLE, rect, pParent, nID))
	{
		m_Type = type;//定义类型
		m_IsRotate = false;
		m_IsManulating = false;//默认不操作
		return true;
	}

	return false;
}

void MyButton::changeMode()
{
	m_IsRotate = !m_IsRotate;
}

void MyButton::adjustSize(const CRect& rect)
{
	this->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
}

void MyButton::updateCursor()
{

	HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
	switch (m_Type)
	{
	case TopLeft:
	case BottomRight:
			if (m_IsRotate)
				cursor = LoadCursor(NULL, IDC_SIZENWSE);
			else
				cursor = LoadCursor(NULL, IDC_SIZENWSE);
			break;
	case TopRight:
	case BottomLeft:
		if (m_IsRotate)
			cursor = LoadCursor(NULL, IDC_SIZENESW);
		else
			cursor = LoadCursor(NULL, IDC_SIZENESW);
		break;
	case TopCenter:
	case BottomCenter:
		if (m_IsRotate)
			cursor = LoadCursor(NULL, IDC_SIZEWE);
		else
			cursor = LoadCursor(NULL, IDC_SIZENS);
		break;
	case CenterLeft:
	case CenterRight:
		if (m_IsRotate)
			cursor = LoadCursor(NULL, IDC_SIZENS);
		else
			cursor = LoadCursor(NULL, IDC_SIZEWE);
		break;
	case Center:
		cursor = LoadCursor(NULL, IDC_HAND);
		break;
	}

	::SetCursor(cursor);
}

BEGIN_MESSAGE_MAP(MyButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void MyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 事件处理函数
	updateCursor();

	if (m_IsManulating)
	{
		MyPictureHolder* picHolder = (MyPictureHolder*)(GetParent());
		CMainFrame* pWindow = (CMainFrame*)(AfxGetMainWnd());
		CtestMFCView* pView = (CtestMFCView*)(pWindow->GetActiveView());
		CtestMFCDoc* pDoc = (CtestMFCDoc*)(pWindow->GetActiveDocument());
		
		//获取变换起始点
		CRect windowRect;
		GetWindowRect(windowRect);
		Point2f origin = Point2f(windowRect.left, windowRect.top);
		TRACE("origin(%4.2f, %4.2f)\n", origin.x, origin.y);
		//获取变换当前位置
		TRACE("nowClient(%4.2f, %4.2f)\n", point.x, point.y);
		ClientToScreen(&point);
		Point2f now = Point2f(point.x, point.y);
		TRACE("nowWindow(%4.2f, %4.2f)\n", now.x, now.y);
		//获取变换差
		float delta_X = (now.x - origin.x) / max((float)origin.x, (float)now.x);
		float delta_Y = (now.y - origin.y) / max((float)origin.y,(float)now.y);
		TRACE("deltaXY(%4.2f, %4.2f)\n", delta_X, delta_Y);

		float rotateAngle = 0, scaleFactorX = 0, scaleFactorY = 0, shearMoveX = 0, shearMoveY = 0;
		switch (m_Type)
		{
			case TopLeft:
			case BottomRight:
				if (m_IsRotate)
				{
					rotateAngle = 5;
				}
				else
				{
					if (m_Type == TopLeft)
					{
						scaleFactorX = -delta_X;
						scaleFactorY = -delta_X;
					}
					else
					{
						scaleFactorX = delta_X;
						scaleFactorY = delta_X;
					}
				}
				break;
			case TopRight:
			case BottomLeft:
				if (m_IsRotate)
				{
					rotateAngle = -5;
				}
				else
				{
					if (m_Type == BottomLeft)
					{
						scaleFactorX = -delta_X;
						scaleFactorY = -delta_X;
					}
					else
					{
						scaleFactorX = delta_X;
						scaleFactorY = delta_X;
					}
				}
				break;
			case TopCenter:
			case BottomCenter:
				if (m_IsRotate)
				{
					shearMoveX = delta_X * 10;
				}
				else
				{
					if (m_Type == TopCenter)
						scaleFactorY = -delta_Y;
					else
						scaleFactorY = delta_Y;
				}
				break;
			case CenterLeft:
			case CenterRight:
				if (m_IsRotate)
				{
					shearMoveY = delta_Y * 10;
				}
				else
				{
					if (m_Type == CenterLeft)
						scaleFactorX = -delta_X;
					else
						scaleFactorX = delta_X;
				}
				break;
			case Center:
				picHolder->ScreenToClient(&point);
				picHolder->SetCenter(point);
				TRACE("nowCenter(%4.2f, %4.2f)\n", point.x, point.y);
				break;
		}

		//更新变换参数并更新显示
		picHolder->IncreaseFactor(rotateAngle, scaleFactorX, scaleFactorY, shearMoveX, shearMoveY);
		picHolder->TransformCImage(-1);
		pView->Invalidate(false);
	}

	CButton::OnMouseMove(nFlags, point);
}

void MyButton::OnPaint()
{
	// 绘制代码
	CPaintDC dc(this); 
	CRect clientRect;
	GetClientRect(clientRect);
	dc.Rectangle(clientRect);
}


void MyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 鼠标按下
	m_IsManulating = true;

	CButton::OnLButtonDown(nFlags, point);
}


void MyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 鼠标升起
	m_IsManulating = false;

	CButton::OnLButtonUp(nFlags, point);
}
