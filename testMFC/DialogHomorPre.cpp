// DialogHomorPre.cpp : 实现文件
//

#include "stdafx.h"
#include "testMFC.h"
#include "DialogHomorPre.h"
#include "afxdialogex.h"

using namespace cv;
//  DialogHomorPre 对话框

IMPLEMENT_DYNAMIC(DialogHomorPre, CDialogEx)

DialogHomorPre::DialogHomorPre(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogHomorPre::IDD, pParent)
{

}

DialogHomorPre::~DialogHomorPre()
{
}


CtestMFCDoc* DialogHomorPre::GetDoc()
{
	CMainFrame* pFrame = (CMainFrame*)(GetParent());
	CtestMFCDoc* pDoc = (CtestMFCDoc*)(pFrame->GetActiveDocument());
	return pDoc;
}


CtestMFCView* DialogHomorPre::GetView()
{
	CMainFrame* pFrame = (CMainFrame*)(GetParent());
	CtestMFCView* pView = (CtestMFCView*)(pFrame->GetActiveView());
	return pView;
}

BOOL DialogHomorPre::OnInitDialog()
{
	//TODO: 初始化参数
	pSpinCtrl = (CSpinButtonCtrl*)(GetDlgItem(IDC_EDIT_STEP_HOMO));
	pSpinCtrl->SetPos(14);

	lowerHomo = 1.0;
	higherHomo = 2.5;
	thresHomo = 70;
	stepHomo = 14;
	UpdateData(false);	

	CEdit* pEdit;
	pEdit = (CEdit*)(GetDlgItem(IDC_EDIT_LOWER_HOMO));
	pEdit->SetReadOnly();

	pEdit = (CEdit*)(GetDlgItem(IDC_EDIT_HIGHER_HOMO));
	pEdit->SetReadOnly();

	pEdit = (CEdit*)(GetDlgItem(IDC_EDIT_THRES__HOMO));
	pEdit->SetReadOnly();

	return CDialogEx::OnInitDialog();
}

void DialogHomorPre::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//TODO:数据绑定
	DDX_Text(pDX, IDC_EDIT_STEP_HOMO, stepHomo);
	DDV_MinMaxInt(pDX,stepHomo, 1, 50);


	DDX_Text(pDX, IDC_EDIT_LOWER_HOMO, lowerHomo);
	DDV_MinMaxInt(pDX, lowerHomo, 0.5, 1.5);


	DDX_Text(pDX, IDC_EDIT_HIGHER_HOMO, higherHomo);
	DDV_MinMaxInt(pDX, higherHomo, 2.0, 5.0);


	DDX_Text(pDX, IDC_EDIT_THRES__HOMO, thresHomo);
	DDV_MinMaxInt(pDX, thresHomo, 1, 100);
}


BEGIN_MESSAGE_MAP(DialogHomorPre, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_STEP_HOMO, &DialogHomorPre::OnDeltaposSpinStepHomo)
	ON_EN_CHANGE(IDC_EDIT_STEP_HOMO, &DialogHomorPre::OnEnChangeEditStepHomo)
	ON_BN_CLICKED(IDOK, &DialogHomorPre::OnBnOK)
	ON_BN_CLICKED(IDCANCEL, &DialogHomorPre::OnBnCancel)
END_MESSAGE_MAP()


// DialogHomorPre 消息处理程序


void DialogHomorPre::OnDeltaposSpinStepHomo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);// -1 上

	stepHomo -= pNMUpDown->iDelta;
	if (stepHomo > 50 || stepHomo < 1)
		MessageBox(L"请输入1-50数字", L"testMFC", MB_ICONWARNING);

	pSpinCtrl->SetPos(stepHomo);
	UpdateData(false);

	*pResult = 0;
}


void DialogHomorPre::OnEnChangeEditStepHomo()
{
	UpdateData(true);
	pSpinCtrl->SetPos(stepHomo);
}


void DialogHomorPre::OnBnOK()
{
	// TODO:  频域域增强
	CtestMFCDoc* pDoc = GetDoc();
	CtestMFCView* pView = GetView();
	Mat src = *(pDoc->m_Erase);
	if (pView->m_CurrPicHolder != -1 && !src.empty())
	{
		Mat mask = *(pDoc->m_Mask);
		Mat dst;
		homomorphicPre(src, dst, mask, stepHomo, lowerHomo, higherHomo, thresHomo);

		//更新数据库
		dst.copyTo(*(pDoc->m_Pre));
		dst.copyTo(*(pDoc->m_Img));
		//dst.copyTo(pDoc->m_ImgList[pView->m_CurrPicHolder]);
		//更新显示
		pView->m_PicHolderList[pView->m_CurrPicHolder]->UpdateCImage(-1);
		pView->Invalidate(false);
	}
	CDialogEx::OnOK();
}


void DialogHomorPre::OnBnCancel()
{
	CDialogEx::OnCancel();
}
