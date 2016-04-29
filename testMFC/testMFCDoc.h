
// testMFCDoc.h : CtestMFCDoc ��Ľӿ�
//


#pragma once
#include "Helper.h"

class CtestMFCDoc : public CDocument
{
protected: // �������л�����
	CtestMFCDoc();
	DECLARE_DYNCREATE(CtestMFCDoc)

//  ����
public:
	cv::Mat* m_Img;//��ʾͼ��
	cv::Mat* m_Src;//ԭʼͼ��
	cv::Mat* m_Erase;//���������ͼ��
	cv::Mat* m_Mask;//ģ��ͼ��
	cv::Mat* m_Pre;//Ԥ�����ͼ��
	cv::Mat* m_Curve;//����ͼ��

	std::vector<cv::Mat> m_ImgList;//ͼ���б�

// ����
public:
	BOOL Load(cv::Mat* pImg, LPCTSTR cFileName);
	BOOL Load(std::vector<cv::Mat>& pImgList, LPCTSTR cFileName);
	BOOL Save(cv::Mat* pImg, LPCTSTR cFileName);
	BOOL OnOpenDocument(std::vector<CString>& lpszPathNameList);

	BOOL ClearLImgList();//����б�
	BOOL RemoveOneImage(int index);//ɾ��ָ��ͼ��
	BOOL SetCurrImg(int index);//������ʾͼ��
	void UpdateCImage(CImage& cimage,int index = -1);
	void MatToCImage(cv::Mat& img, CImage& cimage);//mat -> cimage
	void TransformMat(CImage& cimage, float& cx, float& cy, float rotateAngle = 0, float scaleFactorX = 1, float scaleFactorY = 1, float shearMoveX = 0, float shearMoveY = 0, int index = -1);

// ��д
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CtestMFCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

};
