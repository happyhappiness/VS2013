// DialogRetrCurve.cpp : 实现文件
//

#include "stdafx.h"
#include "testMFC.h"
#include "DialogRetrCurve.h"
#include "afxdialogex.h"

using namespace cv;


//  DialogRetrCurve 对话框
IMPLEMENT_DYNAMIC(DialogRetrCurve, CDialogEx)

DialogRetrCurve::DialogRetrCurve(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogRetrCurve::IDD, pParent)
{

}

DialogRetrCurve::~DialogRetrCurve()
{
}


CtestMFCDoc* DialogRetrCurve::GetDoc()
{
	CMainFrame* pFrame = (CMainFrame*)(GetParent());
	CtestMFCDoc* pDoc = (CtestMFCDoc*)(pFrame->GetActiveDocument());
	return pDoc;
}


CtestMFCView* DialogRetrCurve::GetView()
{
	CMainFrame* pFrame = (CMainFrame*)(GetParent());
	CtestMFCView* pView = (CtestMFCView*)(pFrame->GetActiveView());
	return pView;
}


BOOL DialogRetrCurve::OnInitDialog()
{
	//TODO: 参数初始化
	pListBoxConnectType = (CListBox*)(GetDlgItem(IDC_LIST_CONNECTYPE_RETRCURVE));
	pListBoxConnectType->AddString(L"4");
	pListBoxConnectType->AddString(L"8"); 
	pListBoxConnectType->SetCurSel(0);
	pListBoxLimitPoints = (CListBox*)(GetDlgItem(IDC_LIST_LIMITPOINTS_RETRCURVE));
	CString str;
	for (int i = 1; i < 50; i++)
	{
		str.Format(L"%d", i);
		pListBoxLimitPoints->AddString(str);
	}//1- 30
	pListBoxLimitPoints->SetCurSel(19);

	pSliderThreshold = (CSliderCtrl*)(GetDlgItem(IDC_SLIDER_THRESHOLD_RETRCURVE));
	pSliderThreshold->SetRange(1, 255);
	pSliderThreshold->SetPos(120);
	pSliderThreshold->SetTicFreq(15);
	pSliderStep = (CSliderCtrl*)(GetDlgItem(IDC_SLIDER_RETRCURVE));
	pSliderStep->SetRange(1, 50);
	pSliderStep->SetPos(20);
	pSliderStep->SetTicFreq(2);

	step = 20;
	thresholdValue = 120;
	connectType = 4;
	limitPoints = 20;
	UpdateData(false);

	return CDialogEx::OnInitDialog();
}

void DialogRetrCurve::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//editCtrl
	DDX_Text(pDX, IDC_EDIT_THRESHOLD_RETRCURVE, thresholdValue);
	DDV_MinMaxFloat(pDX, thresholdValue, 1, 255);
	DDX_Text(pDX, IDC_EDIT_STEP_RETRCURVE, step);
	DDV_MinMaxInt(pDX, step, 1, 50);
}


BEGIN_MESSAGE_MAP(DialogRetrCurve, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST_LIMITPOINTS_RETRCURVE, &DialogRetrCurve::OnLbnSelchangeListLimitpointsRetrcurve)
	ON_LBN_SELCHANGE(IDC_LIST_CONNECTYPE_RETRCURVE, &DialogRetrCurve::OnLbnSelchangeListConnectypeRetrcurve)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDOK_RETRCURVE, &DialogRetrCurve::OnOKRetrcurve)
	ON_BN_CLICKED(IDCANCEL_RETRCURVE, &DialogRetrCurve::OnCancelRetrcurve)
	ON_EN_CHANGE(IDC_EDIT_STEP_RETRCURVE, &DialogRetrCurve::OnEnChangeEditStepRetrCurve)
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD_RETRCURVE, &DialogRetrCurve::OnEnChangeEditThresholdRetrcurve)
END_MESSAGE_MAP()


// DialogRetrCurve 消息处理程序


void DialogRetrCurve::OnEnChangeEditStepRetrCurve()
{
	UpdateData(true);
	pSliderStep->SetPos(step);
}


void DialogRetrCurve::OnEnChangeEditThresholdRetrcurve()
{

	UpdateData(true);
	pSliderThreshold->SetPos(thresholdValue);
}


void DialogRetrCurve::OnLbnSelchangeListConnectypeRetrcurve()
{
	step = 4*(pListBoxConnectType->GetCurSel() + 1);
}


void DialogRetrCurve::OnLbnSelchangeListLimitpointsRetrcurve()
{
	limitPoints = pListBoxLimitPoints->GetCurSel() + 1;
}


void DialogRetrCurve::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  滑动条处理
	if (pScrollBar != NULL && (CSliderCtrl*)(pScrollBar) == pSliderThreshold)
	{
		thresholdValue = (float)(pSliderThreshold->GetPos());// 1 - 255
	}
	else if ((CSliderCtrl*)(pScrollBar) == pSliderStep)
	{
		step = pSliderStep->GetPos();
	}//1 - 50

	UpdateData(false);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void DialogRetrCurve::OnOKRetrcurve()
{

	// TODO: 调用工作者线程
	//CWinThread* pTreadCounter = AfxBeginThread(ExecutingCounting, AfxGetMainWnd());

	ThreadParam threadParam;
	threadParam.dlgRetrCurve = this;
	threadParam.pDoc = this->GetDoc();
	threadParam.pView = this->GetView();
	//threadParam.pThreadCounter = pTreadCounter;

	AfxBeginThread(ExecutingFunction, &threadParam);

	CDialogEx::OnOK();
}


void DialogRetrCurve::OnCancelRetrcurve()
{
	CDialogEx::OnCancel();
}

UINT ExecutingFunction(LPVOID pParam)
{
	//TODO: 提取曲线的线程函数
	ThreadParam* pThreadParam = (ThreadParam*)(pParam);
	DialogRetrCurve* dlgRetrCurve = pThreadParam->dlgRetrCurve;
	CtestMFCDoc* pDoc = pThreadParam->pDoc;
	CtestMFCView* pView = pThreadParam->pView;
//	CWinThread* pThreadCounter = pThreadParam->pThreadCounter;

	Mat src;
	src = *(pDoc->m_Pre);
	if (pView->m_CurrPicHolder != -1 && !src.empty())
	{
		Mat dst;
		retrieveCurves(src, dst, *(pDoc->m_Mask), dlgRetrCurve->thresholdValue, dlgRetrCurve->step, dlgRetrCurve->limitPoints, dlgRetrCurve->connectType);
		//更新数据库
		dst.copyTo(*(pDoc->m_Curve));
		dst.copyTo(*(pDoc->m_Img));
		//dst.copyTo(pDoc->m_ImgList[pView->m_CurrPicHolder]);
		//更新显示
		pView->m_PicHolderList[pView->m_CurrPicHolder]->UpdateCImage(-1);
		pView->Invalidate(false);
		//pThreadCounter->ExitInstance();
	}
	return 0;
}


//UINT ExecutingCounting(LPVOID pParam)
//{
//	//显示进度
//	CString str;
//
//	CMainFrame* pMainFrame = (CMainFrame*)pParam;
//	CStatusBar* statusBar = pMainFrame->GetStatusBar();
//
//	int i = 0;
//	while (true)
//	{
//		str.Format(L"正在处理中 ...%d", i % 100 + 1);
//		statusBar->SetPaneText(0, str);
//		i++;
//
//		//Sleep(1000);//休眠
//	}
//
//	return 0;
//}
