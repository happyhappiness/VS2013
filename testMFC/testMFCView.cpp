
//  testMFCView.cpp : CtestMFCView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "testMFC.h"
#endif

#include "testMFCDoc.h"
#include "testMFCView.h"
#include "DialogErase.h"
#include "DialogHomorPre.h"
#include "DialogRetrCurve.h"
#include "MyPictureHolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_THREADIN WM_USER + 3

using namespace cv;
using namespace std;

// CtestMFCView

IMPLEMENT_DYNCREATE(CtestMFCView, CFormView)

BEGIN_MESSAGE_MAP(CtestMFCView, CFormView)
	ON_COMMAND(ID_FILE_OPEN, &CtestMFCView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CtestMFCView::OnFileSave)
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_ERASEBACK, &CtestMFCView::OnEditEraseback)
	ON_COMMAND(ID_EDIT_HOMORPRE, &CtestMFCView::OnEditHomorpre)
//	ON_WM_HSCROLL()
ON_COMMAND(ID_EDIT_RETRCURVE, &CtestMFCView::OnEditRetrcurve)
ON_COMMAND(ID_ConnectAuto, &CtestMFCView::OnConnectauto)
ON_COMMAND(ID_ConnectHand, &CtestMFCView::OnConnecthand)
ON_COMMAND(ID_CHANGEMODE, &CtestMFCView::OnChangemode)
ON_WM_ERASEBKGND()
ON_COMMAND(ID_Delete, &CtestMFCView::OnDelete)
ON_MESSAGE(WM_THREADIN, &CtestMFCView::OnThreadin)
END_MESSAGE_MAP()

// CtestMFCView ����/����

CtestMFCView::CtestMFCView()
	: CFormView(CtestMFCView::IDD)
{
	m_CurrPicHolder = -1;
}

CtestMFCView::~CtestMFCView()
{
	int count = m_PicHolderList.size();
	for (int i = 0; i < count; i++)
	{
		delete m_PicHolderList[i];
	}//���ָ��
}

void CtestMFCView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

void CtestMFCView::OnDraw(CDC* pDC)
{

	CFormView::OnDraw(pDC);
}//TODO: ����ͼ��


BOOL CtestMFCView::PreTranslateMessage(MSG *pMsg)
{
	//if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_LBUTTONDBLCLK)
	//{
	//	int count = m_PicHolderList.size();
	//	for (int i = 0; i < count; i++)
	//	{
	//		if (pMsg->hwnd == m_PicHolderList[i]->GetSafeHwnd())
	//		{
	//			::PostMessage(m_PicHolderList[i]->GetSafeHwnd(), pMsg->message, pMsg->wParam, pMsg->lParam);
	//		}
	//	}//�ռ�ַ�
	//}
	return CFormView::PreTranslateMessage(pMsg);
}//TODO: �¼��ַ�

BOOL CtestMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	return CFormView::PreCreateWindow(cs);

}

void CtestMFCView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}


// CtestMFCView ���

#ifdef _DEBUG
void CtestMFCView::AssertValid() const
{
	CFormView::AssertValid();
}

void CtestMFCView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}


CMainFrame* CtestMFCView::GetMainFrame() const //��ȡ�����
{
	return (CMainFrame*)(GetOwner());
}

CtestMFCDoc* CtestMFCView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestMFCDoc)));
	return (CtestMFCDoc*)m_pDocument;
}
#endif //_DEBUG

void CtestMFCView::setNowPicHolder(int currPicHolder)
{
	//TODO:���õ�ǰѡ�пؼ�
	//ȡ����ѡ�пؼ�
	if (m_CurrPicHolder != -1)
		m_PicHolderList[m_CurrPicHolder]->ShowButtons(false);
	
	//������ʾ
	m_CurrPicHolder = currPicHolder;
	if (currPicHolder == -1)
		return;
	m_PicHolderList[m_CurrPicHolder]->ShowButtons(true);
	Invalidate(false);

	//�������ݿ�
	GetDocument()->SetCurrImg(m_CurrPicHolder);

}

void CtestMFCView::setNowPicHolder(MyPictureHolder* nowPicHolder)
{
	//Ѱ�ҿؼ���Ӧ�±�
	int count = m_PicHolderList.size();
	for (int i = 0; i < count; i++)
	{
		if (m_PicHolderList[i] == nowPicHolder)
		{
			setNowPicHolder(i);
			return;
		}
	}
}


// CtestMFCView ��Ϣ�������


void CtestMFCView::OnFileOpen()
{
	// TODO:  �򿪶��ͼ��
	vector<CString> fileNameList;
	CtestMFCDoc* pDoc = GetDocument();
	
	CFileDialog dialog(TRUE, _T(".tif"), NULL, OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("(���и�ʽͼƬ)|*.bmp;*.jpg;*.jpeg;*.tif;*.png|(*.pngͼƬ)|*.png|(.*bmp��ʽͼƬ)|*.bmp|(.*jpg��ʽͼƬ)|*.jpg|(.*jpeg��ʽͼƬ)|*.jpeg|(.*tif��ʽͼƬ)|*.tif||"));
	
	const int nMaxFiles = 20;//����ļ���
	const int nMaxPathBuffer = (nMaxFiles * (MAX_PATH + 1)) + 1;
	LPWSTR pc = (LPWSTR)malloc(nMaxPathBuffer * sizeof(WCHAR));
	if (pc)
	{
		dialog.GetOFN().lpstrFile = pc;
		dialog.GetOFN().lpstrFile[0] = NULL;
		dialog.GetOFN().nMaxFile = nMaxPathBuffer;
		if (dialog.DoModal() == IDOK)
		{
			POSITION posStart = dialog.GetStartPosition();
			while (posStart)
			{
				//��pc��ָ����ڴ��н�����ÿ���ļ�������,�����fileName��ռ���ڴ治�ܺ�pc��ռ���ڴ淢����ͻ  
				CString pathName = dialog.GetNextPathName(posStart);
				fileNameList.push_back(pathName);
			}
			pDoc->OnOpenDocument(fileNameList);//��ȡ�ļ�

			vector<Mat> imgList = pDoc ->m_ImgList;
			int count = imgList.size();

			CRect clientRect;
			this->GetClientRect(clientRect); 

			MyPictureHolder* tempPicHolder;
			for (int i = m_PicHolderList.size(); i < count; i++)
			{
				tempPicHolder = new MyPictureHolder();
				tempPicHolder->Create(clientRect, this, i, i);//��ʼ���ؼ�
				m_PicHolderList.push_back(tempPicHolder);
			}
			setNowPicHolder(m_PicHolderList.size() - 1);//���¼��ؼ�Ϊѡ��
		}
		free(pc);
	}
}


void CtestMFCView::OnFileSave()
{
	// TODO:  ���浱ǰͼƬ
	CFileDialog dialog(FALSE, _T(".tif"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("(���и�ʽͼƬ)|*.bmp;*.jpg;*.jpeg;*.tif|(.*bmp��ʽͼƬ)|*.bmp|(.*jpg��ʽͼƬ)|*.jpg|(.*jpeg��ʽͼƬ)|*.jpeg|(.*tif��ʽͼƬ)|*.tif||"));
	if (dialog.DoModal() == IDOK)
	{
		CString fileName = dialog.GetPathName();
		GetDocument()->OnSaveDocument(fileName);//����m_Img
	}//����ͼƬ
}


void CtestMFCView::OnEditEraseback()
{
	// TODO:  ������������Ĳ������ÿ�
	DialogErase dlgErase;
	dlgErase.DoModal();
}


void CtestMFCView::OnEditHomorpre()
{
	// TODO:  ̬ͬ�˲���Ƶ������ǿ
	DialogHomorPre dlgHomorPre;
	dlgHomorPre.DoModal();
}



void CtestMFCView::OnEditRetrcurve()
{
	// TODO: ��ȡ����
	DialogRetrCurve dlgRetrCurve;
	dlgRetrCurve.DoModal();
}


void CtestMFCView::OnConnectauto()
{
	//�����̣߳��Զ�ƴ��
	AfxBeginThread(ConnectionThread, this); 
}


void CtestMFCView::OnConnecthand()
{
	// TODO:  �ֶ�ƴ�ӽ��

	TRACE("connect hand");
}


void CtestMFCView::OnChangemode()
{
	if (m_CurrPicHolder != -1)
		m_PicHolderList[m_CurrPicHolder]->ChangeMode();
}

void CtestMFCView::OnDelete()
{
	// TODO:  ɾ����ǰ�ؼ�
	if (m_CurrPicHolder != -1)
	{
		//ɾ���ؼ�
		TRACE(L"index:%d\t cimageW:%d\n", m_CurrPicHolder, m_PicHolderList[m_CurrPicHolder]->m_CImage.GetWidth());
		delete m_PicHolderList[m_CurrPicHolder];
		m_PicHolderList.erase(m_PicHolderList.begin() + m_CurrPicHolder);
		//�޸����ݿ�
		GetDocument()->RemoveOneImage(m_CurrPicHolder);

		//���ѡ��
		m_CurrPicHolder = -1;
	}
}

void CtestMFCView::OnMouseMove(UINT nFlags, CPoint point)
{

	CFormView::OnMouseMove(nFlags, point);

	// TODO:  ����ƶ�
	CString str;
	str.Format(_T("��ǰ����(%d, %d)"), point.x, point.y); 
	CStatusBar* statusBar = GetMainFrame()->GetStatusBar();

	statusBar->SetPaneText(0, str);
}

void CtestMFCView::OnSize(UINT nType, int cx, int cy)
{

	CFormView::OnSize(nType, cx, cy);

	// TODO: ����������
	//int count = m_PicHolderList.size();
	//for (int i = 0; i < count; i++)
	//{
	//	m_PicHolderList[i]->OnSize(nType, cx, cy);
	//}

	Invalidate(false);//����
}

BOOL CtestMFCView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  ���ñ���ɫ
	CBrush backBrush(RGB(255,255,255));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}

UINT ConnectionThread(LPVOID pParam)
{

	CtestMFCView* pView = (CtestMFCView*)(pParam);
	CtestMFCDoc* pDoc = pView->GetDocument();
	HWND pHwnd = pView->GetSafeHwnd();
	SendMessage(pHwnd, WM_THREADIN, 0, 0);

	//TODO: �Զ�ƴ���̺߳��ĺ���
	vector<Mat> imgs = pDoc->m_ImgList;
	Mat img = imgs[imgs.size() - 1];//���һ��Ϊ�ο�
	Mat dst;
	imgs.pop_back();
	seriesForImages(imgs, img, dst);

	//�������ݿ�
	dst.copyTo(*(pDoc->m_Img));

	SendMessage(pHwnd, WM_THREADIN, 1, 0);

	return 0;
}

afx_msg LRESULT CtestMFCView::OnThreadin(WPARAM wParam, LPARAM lParam)
{

	CStatusBar* statusBar = GetMainFrame()->GetStatusBar();
	if (wParam == 0)
	{
		statusBar->SetPaneText(0, _T("ƴ���С���"));
	}
	else if (wParam == 1)
	{
		statusBar->SetPaneText(0, _T("ƴ�����"));
		CtestMFCDoc* pDoc = GetDocument();

		//ɾ���ؼ�
		setNowPicHolder(-1);//���״̬
		int count = m_PicHolderList.size();
		for (int i = 0; i < count; i++)
		{
			delete m_PicHolderList[i];
		}
		m_PicHolderList.clear();

		//ɾ�����ݿ�
		pDoc->ClearLImgList();

		//�������ݿ�
		pDoc->m_ImgList.push_back(*(pDoc->m_Img));

		//��ʾƴ�ӽ��
		MyPictureHolder* nowPicHolder = new MyPictureHolder();
		CRect clientRect;
		GetClientRect(clientRect);

		m_PicHolderList.push_back(nowPicHolder);
		nowPicHolder->Create(clientRect, this, 0, 0);

		setNowPicHolder(0);
	}

	return 0;
}
