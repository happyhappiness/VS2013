#pragma once


#include "testMFCView.h"
#include "Helper.h"
// DialogHomorPre �Ի���

class DialogHomorPre :  public CDialogEx
{
	DECLARE_DYNAMIC(DialogHomorPre)

public:
	DialogHomorPre(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogHomorPre();



// �Ի�������
	enum { IDD = IDD_DIALOG_HOMORPRE };

//����
	virtual BOOL OnInitDialog();

//����
	CtestMFCDoc* GetDoc();
	CtestMFCView* GetView();

	CSpinButtonCtrl* pSpinCtrl;
	int stepHomo, thresHomo;
	float lowerHomo, higherHomo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposSpinStepHomo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditStepHomo();
	afx_msg void OnBnOK();
	afx_msg void OnBnCancel();
};
