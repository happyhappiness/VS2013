
// testMFCView.h : CtestMFCView 类的接口
//

#pragma once

#include "resource.h"
#include "MainFrm.h"
#include "Helper.h"
#include "testMFCDoc.h"
#include "MyPictureHolder.h"

class CtestMFCView : public CFormView
{
protected: //  仅从序列化创建
	CtestMFCView();
	DECLARE_DYNCREATE(CtestMFCView)

public:
	enum{ IDD = IDD_TESTMFC_FORM };

// 特性
public:

	int m_CurrPicHolder;
	std::vector<MyPictureHolder*> m_PicHolderList;//图片控件列表
	
//	void ChangeSingleAndMulti();

	CMainFrame* GetMainFrame() const;
	CtestMFCDoc* GetDocument() const;

// 操作
public:
	
	void setNowPicHolder(int currPicHolder);
	void setNowPicHolder(MyPictureHolder* nowPicHolder);

// 重写
public:
	virtual void OnDraw(CDC* pDC = 0);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG *pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CtestMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditEraseback();
	afx_msg void OnEditHomorpre();
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEditRetrcurve();
	afx_msg void OnConnectauto();
	afx_msg void OnConnecthand();
	afx_msg void OnChangemode();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDelete();
protected:
	afx_msg LRESULT OnThreadin(WPARAM wParam, LPARAM lParam);
};

void onMouseForOpencvWindow(int event, int x, int y, int flags, void*param);
UINT ConnectionThread(LPVOID pParam);

#ifndef _DEBUG  // testMFCView.cpp 中的调试版本
inline CtestMFCDoc* CtestMFCView::GetDocument() const
   { return reinterpret_cast<CtestMFCDoc*>(m_pDocument); }
#endif

