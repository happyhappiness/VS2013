#pragma once

#include "Helper.h"
#include "testMFCView.h"

// DialogRetrCurve �Ի���


class DialogRetrCurve :  public CDialogEx
{
	DECLARE_DYNAMIC(DialogRetrCurve)

public:
	DialogRetrCurve(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogRetrCurve();

// �Ի�������
	enum { IDD = IDD_DIALOG_RETRCURVE };
//����

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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


//�����̺߳���
UINT ExecutingFunction(LPVOID pParam);
//UINT ExecutingCounting(LPVOID pParam);
//�����ṹ��
struct ThreadParam{
	DialogRetrCurve* dlgRetrCurve;
	CtestMFCView* pView;
	CtestMFCDoc* pDoc;
	//CWinThread* pThreadCounter;
};