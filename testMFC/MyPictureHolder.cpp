#include "stdafx.h"
#include "MyPictureHolder.h"
#include "testMFCView.h"
using namespace cv;
using namespace std;

MyPictureHolder::MyPictureHolder()
{
	// do nothing
}

BOOL MyPictureHolder::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return this->CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

BOOL MyPictureHolder::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, int index)
{
	if (Create(NULL, WS_CHILD | WS_VISIBLE | SS_BLACKRECT |SS_NOTIFY, rect, pParentWnd, nID))
	{
		UpdateCImage(index);//������ʾ
		m_Width = 10;//Ĭ�ϲ������δ�С

		//��λ���γ�ʼ��
		MyButton* button;
		for (int i = 0; i < 9; i++)
		{
			m_RectButtons.push_back(CRect());
			button = new MyButton();
			button->Create(m_RectButtons[i], this, i, i);
			m_Buttons.push_back(button);
		}
		ResizeRects(rect);
		ShowButtons(false);//����ʾ
		
		//�任������ʼ��
		m_RotationAngle = 0;//�仯����
		m_ScaleFactorX = 1;
		m_ScaleFactorY = 1;
		m_ShearMoveX = 0;
		m_ShearMoveY = 0;
		m_cx = 0.5;
		m_cy = 0.5;
		m_OriginClientRect = rect;//ԭʼ��С
		TRACE(L"index:%d\t cimageW:%d\n", index, m_CImage.GetWidth());
		return true;
	}

	return false;
}

void MyPictureHolder::ResizeRects(const RECT& parentRect)
{
	if (m_RectButtons.size() != 9)
		return;

	int delta = 0;
	int left = delta;
	int top = delta;
	int right = parentRect.right - m_Width;
	int bottom = parentRect.bottom - m_Width;
	int centerX = (left + right) / 2;
	int centerY = (top + bottom) / 2;
	CSize size(m_Width, m_Width);
	m_RectButtons[TopLeft] = CRect(CPoint(left, top), size);
	m_RectButtons[BottomRight] = CRect(CPoint(right, bottom), size);

	m_RectButtons[TopRight] = CRect(CPoint(right, top), size);
	m_RectButtons[BottomLeft] = CRect(CPoint(left, bottom), size);	
	
	m_RectButtons[TopCenter] = CRect(CPoint(centerX, top), size);
	m_RectButtons[BottomCenter] = CRect(CPoint(centerX, bottom), size);


	m_RectButtons[CenterLeft] = CRect(CPoint(left, centerY), size);
	m_RectButtons[CenterRight] = CRect(CPoint(right, centerY), size);


	m_RectButtons[Center] = CRect(CPoint(m_cx*(parentRect.right - parentRect.left), m_cy*((parentRect.bottom - parentRect.top))), size);


	//λͼ��λ���γ�ʼ��
	m_RectCImage = CRect(m_Width, m_Width, right - m_Width, bottom - m_Width);
}

void MyPictureHolder::ShowButtons(bool isToShow)
{
	if (isToShow)
	{
		CRect clientRect;
		this->GetClientRect(clientRect);
		ResizeRects(CRect(clientRect));
	}
	else
	{
		int count = m_RectButtons.size();
		for (int i = 0; i < count; i++)
		{
			m_RectButtons[i].SetRectEmpty();
		}
	}
	
	//����m_RectButtons�޸�buttons
	AdjustButtons();
}

void MyPictureHolder::AdjustButtons()
{
	//���ݾ����޸�Buttons�ɼ���
	int count = m_Buttons.size();
	for (int i = 0; i < count; i++)
	{
		m_Buttons[i]->adjustSize(m_RectButtons[i]);
	}
}

void MyPictureHolder::ChangeMode()
{
	int count = m_Buttons.size();
	for (int i = 0; i < count; i++)
	{
		m_Buttons[i]->changeMode();
	}
}

CPoint MyPictureHolder::GetCenter()
{
	return m_RectButtons[Center].TopLeft();
}//�����м��

void MyPictureHolder::SetCenter(CPoint center)
{
	m_RectButtons[Center] = CRect(center, CPoint(center.x + m_Width, center.y + m_Width));
	m_Buttons[Center]->adjustSize(m_RectButtons[Center]);

	//�������ĵ�������
	CRect clientRect;
	GetClientRect(clientRect);
	CPoint centerTemp = m_RectButtons[Center].TopLeft();
	m_cx = centerTemp.x / (float)clientRect.Width();
	m_cy = centerTemp.y / (float)clientRect.Height();
}//�����м��

void MyPictureHolder::UpdateCImage(int index)
{
	CMainFrame* pWindow = (CMainFrame*)(AfxGetMainWnd());
	CtestMFCDoc* pDoc = (CtestMFCDoc*)(pWindow->GetActiveDocument());

	pDoc->UpdateCImage(m_CImage, index);
}

void MyPictureHolder::TransformCImage(int index)
{
	CMainFrame* pWindow = (CMainFrame*)(AfxGetMainWnd());
	CtestMFCDoc* pDoc = (CtestMFCDoc*)(pWindow->GetActiveDocument());
	CtestMFCView* pView = (CtestMFCView*)(pWindow->GetActiveView());

	//�任������
	float cx = m_cx;
	float cy = m_cy;
	pDoc->TransformMat(m_CImage,cx, cy, m_RotationAngle, m_ScaleFactorX, m_ScaleFactorY, m_ShearMoveX, m_ShearMoveY, index);

	//���ݱ������㳤���
	TRACE("windowRate(%4.2f,%4.2f)\n", cx, cy);
	cx = cx* m_OriginClientRect.Width();
	cy = cy* m_OriginClientRect.Height();
	TRACE("windowSize(%4.2f,%4.2f)\n", cx, cy);
	//���ݴ�С�仯��������ʼ��,��֤���ĵ��ڸ�����λ�ò���
	//��ȡ���ĵ�
	CPoint center = m_RectButtons[Center].TopLeft();
	ClientToScreen(&center);
	pView->ScreenToClient(&center);
	TRACE("centerInView(%d,%d)\n", center.x, center.y);
	//�������ϵ�
	CPoint topLeft = CPoint(center.x - cx / 2, center.y - cy / 2);
	TRACE("TopLeftInView(%d,%d)\n", topLeft.x, topLeft.y);

	SetWindowPos(NULL, topLeft.x, topLeft.y, cx, cy, 0);
}

void MyPictureHolder::IncreaseFactor(float rotateAngle, float scaleFactorX, float scaleFactorY, float shearMoveX, float shearMoveY)
{
	m_ScaleFactorX = max((float)0,scaleFactorX + m_ScaleFactorX);
	m_ScaleFactorY = max((float)0, scaleFactorY + m_ScaleFactorY);
	TRACE("m_scaleFactorXY(%4.2f,%4.2f)\n", m_ScaleFactorX, m_ScaleFactorY);
	m_RotationAngle += rotateAngle;
	m_ShearMoveX += shearMoveX;
	m_ShearMoveY += shearMoveY;
}

MyPictureHolder::~MyPictureHolder()
{
	//�ͷ�ָ��
	int count = m_Buttons.size();
	for (int i = 0; i < count; i++)
	{
		delete	m_Buttons[i];
	}
}

BEGIN_MESSAGE_MAP(MyPictureHolder, CStatic)
	ON_WM_ENABLE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDBLCLK()
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void MyPictureHolder::OnPaint()
{
	//���Ʊ���
	CPaintDC dc(this);
	CRect clientRect;
	GetClientRect(clientRect);
	CBrush blackBrush = CBrush(RGB(255, 255, 255));
	CBrush* oldBursh = dc.SelectObject(&blackBrush);
	dc.Rectangle(clientRect);
	dc.SelectObject(oldBursh);

	//// TODO: !!!!͸����ʾCImage
	if (m_CImage.IsNull())
	{
		m_CImage.Load(L"1.jpg");
	}
	if (!m_CImage.IsNull())
	{
		if (m_CImage.GetBPP() / 8 == 3)
			m_CImage.TransparentBlt(dc.m_hDC, m_RectCImage, RGB(1, 1, 1));//!!!����������Ϊ0��0��0
		else
			m_CImage.TransparentBlt(dc.m_hDC, m_RectCImage, 0);
	}
}

void MyPictureHolder::OnMouseMove(UINT nFlags, CPoint point)
{
	CMainFrame* pWindow = (CMainFrame*)(AfxGetMainWnd());
	CtestMFCView* pView = (CtestMFCView*)(pWindow->GetActiveView());
	if ((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		//TODO:!!!�ƶ�����

		//��ȡ���ĵ�
		CPoint center = m_RectButtons[Center].TopLeft();
		ClientToScreen(&center);
		ClientToScreen(&point);
		CPoint topLeft = CPoint(0, 0);
		ClientToScreen(&topLeft);
		topLeft = CPoint(point.x - center.x + topLeft.x, point.y - center.y + topLeft.y);
		pView->ScreenToClient(&topLeft);
		TRACE("TopLeftInView(%4.2f,%4.2f)\n", topLeft.x, topLeft.y);

		SetWindowPos(NULL, topLeft.x, topLeft.y, 0, 0, SWP_NOSIZE);
		pView->Invalidate(false);
	}
	//pView->OnMouseMove(nFlags, point);
	CStatic::OnMouseMove(nFlags, point);
}

void MyPictureHolder::OnMove(int x, int y)
{
	CStatic::OnMove(x, y);

}

void MyPictureHolder::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	////���ݴ�С�仯��������ʼ��,��֤���ĵ��ڸ�����λ�ò���
	//CtestMFCView* pView = (CtestMFCView*)(GetParent());
	////��ȡ���ĵ�
	//CPoint center = m_RectButtons[Center].TopLeft();
	//ClientToScreen(&center);
	//pView->ScreenToClient(&center);
	//TRACE("centerInView(%4.2f,%4.2f)\n", center.x, center.y);
	//CPoint topLeft = CPoint(center.x - cx / 2, center.y - cy / 2);
	//pView->ClientToScreen(&topLeft);
	//TRACE("TopLeftInView(%4.2f,%4.2f)\n", topLeft.x, topLeft.y);
	//SetWindowPos(NULL, topLeft.x, topLeft.y, 0, 0, SWP_NOSIZE);

	ResizeRects(CRect(0,0,cx,cy));//������λ����
	AdjustButtons();//������ť
}

void MyPictureHolder::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CMainFrame* pWindow = (CMainFrame*)(AfxGetMainWnd());
	CtestMFCView* pView = (CtestMFCView*)(pWindow->GetActiveView());
	pView->setNowPicHolder(this);

	CStatic::OnLButtonDblClk(nFlags, point);
}


