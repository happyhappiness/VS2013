
//  testMFCView.cpp : CtestMFCView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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

// CtestMFCView 构造/析构

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
	}//清空指针
}

void CtestMFCView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

void CtestMFCView::OnDraw(CDC* pDC)
{

	CFormView::OnDraw(pDC);
}//TODO: 绘制图像


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
	//	}//空间分发
	//}
	return CFormView::PreTranslateMessage(pMsg);
}//TODO: 事件分发

BOOL CtestMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	return CFormView::PreCreateWindow(cs);

}

void CtestMFCView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}


// CtestMFCView 诊断

#ifdef _DEBUG
void CtestMFCView::AssertValid() const
{
	CFormView::AssertValid();
}

void CtestMFCView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}


CMainFrame* CtestMFCView::GetMainFrame() const //获取主框架
{
	return (CMainFrame*)(GetOwner());
}

CtestMFCDoc* CtestMFCView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestMFCDoc)));
	return (CtestMFCDoc*)m_pDocument;
}
#endif //_DEBUG

void CtestMFCView::setNowPicHolder(int currPicHolder)
{
	//TODO:设置当前选中控件
	//取消旧选中控件
	if (m_CurrPicHolder != -1)
		m_PicHolderList[m_CurrPicHolder]->ShowButtons(false);
	
	//更新显示
	m_CurrPicHolder = currPicHolder;
	if (currPicHolder == -1)
		return;
	m_PicHolderList[m_CurrPicHolder]->ShowButtons(true);
	Invalidate(false);

	//更新数据库
	GetDocument()->SetCurrImg(m_CurrPicHolder);

}

void CtestMFCView::setNowPicHolder(MyPictureHolder* nowPicHolder)
{
	//寻找控件对应下标
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


// CtestMFCView 消息处理程序


void CtestMFCView::OnFileOpen()
{
	// TODO:  打开多幅图像
	vector<CString> fileNameList;
	CtestMFCDoc* pDoc = GetDocument();
	
	CFileDialog dialog(TRUE, _T(".tif"), NULL, OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("(所有格式图片)|*.bmp;*.jpg;*.jpeg;*.tif;*.png|(*.png图片)|*.png|(.*bmp格式图片)|*.bmp|(.*jpg格式图片)|*.jpg|(.*jpeg格式图片)|*.jpeg|(.*tif格式图片)|*.tif||"));
	
	const int nMaxFiles = 20;//最大文件数
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
				//从pc所指向的内存中解析出每个文件的名字,这里的fileName所占的内存不能和pc所占的内存发生冲突  
				CString pathName = dialog.GetNextPathName(posStart);
				fileNameList.push_back(pathName);
			}
			pDoc->OnOpenDocument(fileNameList);//读取文件

			vector<Mat> imgList = pDoc ->m_ImgList;
			int count = imgList.size();

			CRect clientRect;
			this->GetClientRect(clientRect); 

			MyPictureHolder* tempPicHolder;
			for (int i = m_PicHolderList.size(); i < count; i++)
			{
				tempPicHolder = new MyPictureHolder();
				tempPicHolder->Create(clientRect, this, i, i);//初始化控件
				m_PicHolderList.push_back(tempPicHolder);
			}
			setNowPicHolder(m_PicHolderList.size() - 1);//最新加载即为选中
		}
		free(pc);
	}
}


void CtestMFCView::OnFileSave()
{
	// TODO:  保存当前图片
	CFileDialog dialog(FALSE, _T(".tif"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("(所有格式图片)|*.bmp;*.jpg;*.jpeg;*.tif|(.*bmp格式图片)|*.bmp|(.*jpg格式图片)|*.jpg|(.*jpeg格式图片)|*.jpeg|(.*tif格式图片)|*.tif||"));
	if (dialog.DoModal() == IDOK)
	{
		CString fileName = dialog.GetPathName();
		GetDocument()->OnSaveDocument(fileName);//保存m_Img
	}//加载图片
}


void CtestMFCView::OnEditEraseback()
{
	// TODO:  弹出搽除背景的参数配置框
	DialogErase dlgErase;
	dlgErase.DoModal();
}


void CtestMFCView::OnEditHomorpre()
{
	// TODO:  同态滤波的频率域增强
	DialogHomorPre dlgHomorPre;
	dlgHomorPre.DoModal();
}



void CtestMFCView::OnEditRetrcurve()
{
	// TODO: 提取曲线
	DialogRetrCurve dlgRetrCurve;
	dlgRetrCurve.DoModal();
}


void CtestMFCView::OnConnectauto()
{
	//调用线程，自动拼接
	AfxBeginThread(ConnectionThread, this); 
}


void CtestMFCView::OnConnecthand()
{
	// TODO:  手动拼接结果

	TRACE("connect hand");
}


void CtestMFCView::OnChangemode()
{
	if (m_CurrPicHolder != -1)
		m_PicHolderList[m_CurrPicHolder]->ChangeMode();
}

void CtestMFCView::OnDelete()
{
	// TODO:  删除当前控件
	if (m_CurrPicHolder != -1)
	{
		//删除控件
		TRACE(L"index:%d\t cimageW:%d\n", m_CurrPicHolder, m_PicHolderList[m_CurrPicHolder]->m_CImage.GetWidth());
		delete m_PicHolderList[m_CurrPicHolder];
		m_PicHolderList.erase(m_PicHolderList.begin() + m_CurrPicHolder);
		//修改数据库
		GetDocument()->RemoveOneImage(m_CurrPicHolder);

		//清空选择
		m_CurrPicHolder = -1;
	}
}

void CtestMFCView::OnMouseMove(UINT nFlags, CPoint point)
{

	CFormView::OnMouseMove(nFlags, point);

	// TODO:  鼠标移动
	CString str;
	str.Format(_T("当前坐标(%d, %d)"), point.x, point.y); 
	CStatusBar* statusBar = GetMainFrame()->GetStatusBar();

	statusBar->SetPaneText(0, str);
}

void CtestMFCView::OnSize(UINT nType, int cx, int cy)
{

	CFormView::OnSize(nType, cx, cy);

	// TODO: 放缩处理函数
	//int count = m_PicHolderList.size();
	//for (int i = 0; i < count; i++)
	//{
	//	m_PicHolderList[i]->OnSize(nType, cx, cy);
	//}

	Invalidate(false);//更新
}

BOOL CtestMFCView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  设置背景色
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

	//TODO: 自动拼接线程核心函数
	vector<Mat> imgs = pDoc->m_ImgList;
	Mat img = imgs[imgs.size() - 1];//最后一个为参考
	Mat dst;
	imgs.pop_back();
	seriesForImages(imgs, img, dst);

	//更新数据库
	dst.copyTo(*(pDoc->m_Img));

	SendMessage(pHwnd, WM_THREADIN, 1, 0);

	return 0;
}

afx_msg LRESULT CtestMFCView::OnThreadin(WPARAM wParam, LPARAM lParam)
{

	CStatusBar* statusBar = GetMainFrame()->GetStatusBar();
	if (wParam == 0)
	{
		statusBar->SetPaneText(0, _T("拼接中……"));
	}
	else if (wParam == 1)
	{
		statusBar->SetPaneText(0, _T("拼接完成"));
		CtestMFCDoc* pDoc = GetDocument();

		//删除控件
		setNowPicHolder(-1);//清空状态
		int count = m_PicHolderList.size();
		for (int i = 0; i < count; i++)
		{
			delete m_PicHolderList[i];
		}
		m_PicHolderList.clear();

		//删除数据库
		pDoc->ClearLImgList();

		//更新数据库
		pDoc->m_ImgList.push_back(*(pDoc->m_Img));

		//显示拼接结果
		MyPictureHolder* nowPicHolder = new MyPictureHolder();
		CRect clientRect;
		GetClientRect(clientRect);

		m_PicHolderList.push_back(nowPicHolder);
		nowPicHolder->Create(clientRect, this, 0, 0);

		setNowPicHolder(0);
	}

	return 0;
}
