#pragma once


#include "testMFCView.h"
#include "Helper.h"
// DialogHomorPre 对话框

class DialogHomorPre :  public CDialogEx
{
	DECLARE_DYNAMIC(DialogHomorPre)

public:
	DialogHomorPre(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogHomorPre();



// 对话框数据
	enum { IDD = IDD_DIALOG_HOMORPRE };

//重载
	virtual BOOL OnInitDialog();

//特性
	CtestMFCDoc* GetDoc();
	CtestMFCView* GetView();

	CSpinButtonCtrl* pSpinCtrl;
	int stepHomo, thresHomo;
	float lowerHomo, higherHomo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposSpinStepHomo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditStepHomo();
	afx_msg void OnBnOK();
	afx_msg void OnBnCancel();
};
