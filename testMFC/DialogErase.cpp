// DialogErase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testMFC.h"
#include "DialogErase.h"
#include "afxdialogex.h"

using namespace std;
using namespace cv;
//  DialogErase �Ի���

IMPLEMENT_DYNAMIC(DialogErase, CDialogEx)

DialogErase::DialogErase(CWnd* pParent /*=NULL*/) :

	CDialogEx(DialogErase::IDD, pParent)
{
}


BOOL DialogErase::OnInitDialog()
{
	//TODO: ������ʼ��

	pSliderCtrl = (CSliderCtrl*)(GetDlgItem(IDC_SLIDER_THRES_ERASE));
	pSliderCtrl->SetTicFreq(10);
	pSliderCtrl->SetRange(1, 200);
	pSliderCtrl->SetPos(70);

	pSpinCtrl = (CSpinButtonCtrl*)(GetDlgItem(IDC_SPIN_STEP_ERASE));
	pSpinCtrl->SetPos(7);

	//���±༭��
	thresholdValue = 70;
	stepErase = 7;
	UpdateData(false);

	return CDialogEx::OnInitDialog();
}

CtestMFCDoc* DialogErase::GetDoc()
{
	CMainFrame* pFrame = (CMainFrame*)(GetParent());
	CtestMFCDoc* pDoc = (CtestMFCDoc*)(pFrame->GetActiveDocument());
	return pDoc;
}


CtestMFCView* DialogErase::GetView()
{
	CMainFrame* pFrame = (CMainFrame*)(GetParent());
	CtestMFCView* pView = (CtestMFCView*)(pFrame->GetActiveView());
	return pView;
}

DialogErase::~DialogErase()
{
}

void DialogErase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//TODO: DDX���ݰ�
	DDX_Text(pDX, IDC_EDIT_THRES_ERASE, thresholdValue);
	DDV_MinMaxInt(pDX, thresholdValue, 1, 200);

	DDX_Text(pDX, IDC_EDIT_STEP_ERASE, stepErase);
	DDV_MinMaxInt(pDX, stepErase, 1, 50);

}


BEGIN_MESSAGE_MAP(DialogErase, CDialogEx)
	ON_BN_CLICKED(IDOK_ERASE, &DialogErase::OnBtOK)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_STEP_ERASE, &DialogErase::OnDeltaposSpinStepErase)
	ON_EN_CHANGE(IDC_EDIT_THRES_ERASE, &DialogErase::OnEnChangeEditThresErase)
	ON_EN_CHANGE(IDC_EDIT_STEP_ERASE, &DialogErase::OnEnChangeEditStepErase)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDCANCEL_ERASE, &DialogErase::OnBtCancel)
END_MESSAGE_MAP()


// DialogErase ��Ϣ�������


void DialogErase::OnBtOK()
{
	//TODO: �������
	CString str;
	UpdateData(true);
	
	CtestMFCDoc* pDoc = GetDoc();
	CtestMFCView* pView = GetView();
	//��ѡ��ͼƬ
	if (pView->m_CurrPicHolder != -1)
	{
		Mat src = Mat(pDoc->m_ImgList[pView->m_CurrPicHolder]);//��ȡԭͼ
		Mat dst, mask;
		eraseBackground(src, dst, mask, thresholdValue, stepErase);

		//�������ݿ�
		dst.copyTo(*(pDoc->m_Erase));
		mask.copyTo(*(pDoc->m_Mask));
		dst.copyTo(*(pDoc->m_Img));
		//dst.copyTo(pDoc->m_ImgList[pView->m_CurrPicHolder]);
		//������ʾ
		pView->m_PicHolderList[pView->m_CurrPicHolder]->UpdateCImage(-1);
		pView->Invalidate(false);
	}
	CDialogEx::OnOK();
}

void DialogErase::OnDeltaposSpinStepErase(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);// -1 ��

	stepErase -= pNMUpDown->iDelta;
	if (stepErase > 50 || stepErase < 1)
		MessageBox(L"������1-50����", L"testMFC", MB_ICONWARNING);

	pSpinCtrl->SetPos(stepErase);
	UpdateData(false);

	*pResult = 0;
}

void DialogErase::OnEnChangeEditThresErase()
{
	UpdateData(true);
	pSliderCtrl->SetPos(thresholdValue);
}

void DialogErase::OnEnChangeEditStepErase()
{
	
	UpdateData(true);
	pSpinCtrl->SetPos(stepErase);
}

void DialogErase::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//TODO:  �������¼�����
	if (pScrollBar!= NULL && (CSliderCtrl*)(pScrollBar) == pSliderCtrl)
	{
		thresholdValue = pSliderCtrl->GetPos();
		UpdateData(false);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void DialogErase::OnBtCancel()
{
	CDialogEx::OnCancel();
}
