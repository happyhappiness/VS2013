#pragma once

#include "Helper.h"
#include "testMFCView.h"

// DialogRetrCurve 对话框


class DialogRetrCurve :  public CDialogEx
{
	DECLARE_DYNAMIC(DialogRetrCurve)

public:
	DialogRetrCurve(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogRetrCurve();

// 对话框数据
	enum { IDD = IDD_DIALOG_RETRCURVE };
//特性

	CSliderCtrl* pSliderThreshold;
	CSliderCtrl* pSliderStep;
	CListBox* pListBoxLimitPoints;
	CListBox* pListBoxConnectType;
	float thresholdValue;
	int connectType,limitPoints;
	int step;

	virtual BOOL OnInitDialog();
	CtestMFCDoc* GetDoc();
	CtestMFCView* GetView();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeListLimitpointsRetrcurve();
	afx_msg void OnEnChangeEditStepRetrCurve();
	afx_msg void OnEnChangeEditThresholdRetrcurve();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnOKRetrcurve();
	afx_msg void OnCancelRetrcurve();
	afx_msg void OnLbnSelchangeListConnectypeRetrcurve();
protected:
//	afx_msg LRESULT OnThreadover(WPARAM wParam, LPARAM lParam);
};


//声明线程函数
UINT ExecutingFunction(LPVOID pParam);
//UINT ExecutingCounting(LPVOID pParam);
//参数结构体
struct ThreadParam{
	DialogRetrCurve* dlgRetrCurve;
	CtestMFCView* pView;
	CtestMFCDoc* pDoc;
	//CWinThread* pThreadCounter;
};