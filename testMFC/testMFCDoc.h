
// testMFCDoc.h : CtestMFCDoc 类的接口
//


#pragma once
#include "Helper.h"

class CtestMFCDoc : public CDocument
{
protected: // 仅从序列化创建
	CtestMFCDoc();
	DECLARE_DYNCREATE(CtestMFCDoc)

//  特性
public:
	cv::Mat* m_Img;//显示图像
	cv::Mat* m_Src;//原始图像
	cv::Mat* m_Erase;//搽除背景后图像
	cv::Mat* m_Mask;//模板图像
	cv::Mat* m_Pre;//预处理后图像
	cv::Mat* m_Curve;//基线图像

	std::vector<cv::Mat> m_ImgList;//图像列表

// 操作
public:
	BOOL Load(cv::Mat* pImg, LPCTSTR cFileName);
	BOOL Load(std::vector<cv::Mat>& pImgList, LPCTSTR cFileName);
	BOOL Save(cv::Mat* pImg, LPCTSTR cFileName);
	BOOL OnOpenDocument(std::vector<CString>& lpszPathNameList);

	BOOL ClearLImgList();//清空列表
	BOOL RemoveOneImage(int index);//删除指定图像
	BOOL SetCurrImg(int index);//更新显示图像
	void UpdateCImage(CImage& cimage,int index = -1);
	void MatToCImage(cv::Mat& img, CImage& cimage);//mat -> cimage
	void TransformMat(CImage& cimage, float& cx, float& cy, float rotateAngle = 0, float scaleFactorX = 1, float scaleFactorY = 1, float shearMoveX = 0, float shearMoveY = 0, int index = -1);

// 重写
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CtestMFCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

};
