#pragma once

#include "testMFCView.h"
#include "Helper.h"
// DialogErase 对话框

class DialogErase  : public CDialogEx
{
	DECLARE_DYNAMIC(DialogErase)


public:
	DialogErase(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogErase();

// 对话框数据
	enum { IDD = IDD_DIALOG_ERASE };

//重载
	virtual BOOL OnInitDialog();


//特性

	CSliderCtrl* pSliderCtrl;
	CSpinButtonCtrl* pSpinCtrl;
	int thresholdValue, stepErase;
	CtestMFCDoc* GetDoc();
	CtestMFCView* GetView();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtOK();
	afx_msg void OnDeltaposSpinStepErase(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditThresErase();
	afx_msg void OnEnChangeEditStepErase();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtCancel();
};
