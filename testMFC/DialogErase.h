#pragma once

#include "testMFCView.h"
#include "Helper.h"
// DialogErase �Ի���

class DialogErase  : public CDialogEx
{
	DECLARE_DYNAMIC(DialogErase)


public:
	DialogErase(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogErase();

// �Ի�������
	enum { IDD = IDD_DIALOG_ERASE };

//����
	virtual BOOL OnInitDialog();


//����

	CSliderCtrl* pSliderCtrl;
	CSpinButtonCtrl* pSpinCtrl;
	int thresholdValue, stepErase;
	CtestMFCDoc* GetDoc();
	CtestMFCView* GetView();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtOK();
	afx_msg void OnDeltaposSpinStepErase(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditThresErase();
	afx_msg void OnEnChangeEditStepErase();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtCancel();
};
