
//  testMFCDoc.cpp : CtestMFCDoc ���ʵ��
//

#include  "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "testMFC.h"
#endif

#include "testMFCDoc.h"
#include "Helper.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
// CtestMFCDoc

IMPLEMENT_DYNCREATE(CtestMFCDoc, CDocument)

BEGIN_MESSAGE_MAP(CtestMFCDoc, CDocument)
END_MESSAGE_MAP()


// CtestMFCDoc ����/����

CtestMFCDoc::CtestMFCDoc()
{
	m_Img = new Mat(100,100, CV_8UC3,Scalar::all(0));
	m_Src = new Mat;
	m_Mask = new Mat;
	m_Erase = new Mat;
	m_Pre = new Mat;
	m_Curve = new Mat;
}

CtestMFCDoc::~CtestMFCDoc()
{
	delete m_Img;
	delete m_Src;
	delete m_Mask;
	delete m_Erase;
	delete m_Pre;
	delete m_Curve;
}


BOOL CtestMFCDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	//����ͼ��
	return Load(m_Img, lpszPathName);

}

BOOL CtestMFCDoc::OnOpenDocument(vector<CString>& lpszPathNameList)
{
	int count = lpszPathNameList.size();
	std::string fileName;
	Mat img;
	for (int i = 0; i < count; i++)
	{
		fileName = CT2A(lpszPathNameList[i]);
		img = imread(fileName, IMREAD_COLOR);//��ɫģʽ����
		if (!img.empty())
		{
			m_ImgList.push_back(img);
		}//׷��
	}

	return TRUE;
}

BOOL CtestMFCDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnSaveDocument(lpszPathName))
		return FALSE;

	return Save(m_Img, lpszPathName);
}

BOOL CtestMFCDoc::ClearLImgList()
{
	m_ImgList.clear();
	return true;
}

BOOL CtestMFCDoc::RemoveOneImage(int index)
{
	m_ImgList.erase(m_ImgList.begin() + index);//�Ƴ�ĳ��ͼƬ
	return true;
}

BOOL CtestMFCDoc::SetCurrImg(int index)
{
	m_ImgList[index].copyTo(*m_Img);

	return true;
}

void CtestMFCDoc::UpdateCImage(CImage& cimage, int index)
{
	Mat img;
	if (index == -1)
		m_Img->copyTo(img);
	else
		m_ImgList[index].copyTo(img);

	MatToCImage(img, cimage);
	imwrite("dst0.bmp", img);
	cimage.Save(L"dst1.bmp");
	TRACE(L"index:%d\t cimageW:%d\n", index, cimage.GetWidth());
}

void CtestMFCDoc::TransformMat(CImage& cimage, float& cx, float& cy, float rotateAngle, float scaleFactorX, float scaleFactorY, float shearMoveX, float shearMoveY, int index)
{
	Mat img;
	if (index == -1)
		m_Img->copyTo(img);
	else
		m_ImgList[index].copyTo(img);

	Mat resized, roted, shearedX, shearedY;

	//���ݱ����������ĵ�
	Point2f center = Point2f(cx*img.cols, cy*img.rows);
	

	//����
	cv::resize(img, resized, Size(0, 0), scaleFactorX, scaleFactorY, INTER_AREA);
	center = Point2f(cx*resized.cols, cy*resized.rows);

	//��ת
	Mat rotate = getRotationMatrix2D(center, rotateAngle, 1.0);
	warpAffine(resized, roted, rotate, Size(0, 0), INTER_AREA, BORDER_CONSTANT, Scalar::all(0));


	//ˮƽб��
	shearMoveX = abs(shearMoveX);
	copyMakeBorder(roted, shearedX, 0, 0, shearMoveX, shearMoveX, BORDER_CONSTANT, Scalar::all(0));
	center = Point2f(cx*shearedX.cols, cy*shearedX.rows);
	Point2f start[3] = { Point2f(0, 0), Point2f(10, 0), center };
	Point2f end[3] = { Point2f(shearMoveX, 0), Point2f(10 + shearMoveX, 0), center};
	Mat shear = getAffineTransform(start, end);
	warpAffine(shearedX, shearedX, shear, Size(0, 0), INTER_AREA, BORDER_CONSTANT, Scalar::all(0));

	//��ֱб��
	//shearMoveY = max(shearMoveY, (float)0);
	shearMoveY = abs(shearMoveY);
	copyMakeBorder(shearedX, shearedY, shearMoveY, shearMoveY, 0, 0, BORDER_CONSTANT, Scalar::all(0));
	center = Point2f(cx*shearedY.cols, cy*shearedY.rows);
	Point2f startV[3] = { Point2f(0, 0), Point2f(0, 10), center };
	Point2f endV[3] = { Point2f(0, shearMoveY), Point2f(0, 10 + shearMoveX), center };
	shear = getAffineTransform(startV, endV);
	warpAffine(shearedY, shearedY, shear, Size(0, 0), INTER_AREA, BORDER_CONSTANT, Scalar::all(0));

	//������ʾ
	MatToCImage(shearedY, cimage);

	//����ǰ��ͼ�����仯����
	cx = (float)shearedY.cols / (float)img.cols;
	cy = (float)shearedY.rows / (float)img.rows;
}

void CtestMFCDoc::MatToCImage(cv::Mat& img, CImage& cimage)
{
	int width = img.cols;
	int height = img.rows;
	int channels = img.channels();
	if (!cimage.IsNull())
		cimage.Destroy();//��һ���Ƿ�ֹ�ظ���������ڴ�����  
	cimage.Create(width, height, 8 * channels);

	uchar* ps;
	uchar* pimg = (uchar*)cimage.GetBits(); //��ȡCImage�����ش�������ָ��  
	int step = cimage.GetPitch();//ÿ�е��ֽ���,ע���������ֵ�����и�  

	// �����1��ͨ����ͼ��(�Ҷ�ͼ��) DIB��ʽ����Ҫ�Ե�ɫ������    
	// CImage�������˵�ɫ�壬����Ҫ�������и�ֵ��  
	if (1 == channels)
	{
		RGBQUAD* ColorTable;
		int MaxColors = 256;
		//�������ͨ��CI.GetMaxColorTableEntries()�õ���С(�������CI.Load����ͼ��Ļ�)    
		ColorTable = new RGBQUAD[MaxColors];
		cimage.GetColorTable(0, MaxColors, ColorTable);//������ȡ��ָ��    
		for (int i = 0; i<MaxColors; i++)
		{
			ColorTable[i].rgbBlue = (BYTE)i;
			//BYTE��ucharһ���£���MFC�ж�����    
			ColorTable[i].rgbGreen = (BYTE)i;
			ColorTable[i].rgbRed = (BYTE)i;
		}
		cimage.SetColorTable(0, MaxColors, ColorTable);
		delete[]ColorTable;
	}


	for (int i = 0; i < height; i++)
	{
		ps = img.ptr<uchar>(i);
		for (int j = 0; j < width; j++)
		{
			if (1 == channels)
			{
				*(pimg + i*step + j) = ps[j];
				//*(pimg + i*step + j) = 105;  
			}
			else if (3 == channels)
			{
				*(pimg + i*step + j * 3) = ps[j * 3];
				*(pimg + i*step + j * 3 + 1) = ps[j * 3 + 1];
				*(pimg + i*step + j * 3 + 2) = ps[j * 3 + 2];
			}
		}
	}
}

BOOL CtestMFCDoc::Load(Mat* pImg, LPCTSTR cFileName)
{
	std::string fileName = CT2A(cFileName);
	Mat img = imread(fileName, IMREAD_COLOR);
	if (!img.empty())
	{
		img.copyTo(*pImg);
		//TODO: ԭͼ�����
		img.copyTo(*m_Src);
		return TRUE;
	}
	else
		return FALSE;

}

BOOL CtestMFCDoc::Save( Mat* pImg, LPCTSTR cFileName)
{
	(*pImg).convertTo((*pImg), CV_8U);//ת��Ϊ����
	std::string fileName = CT2A(cFileName);
	return imwrite(fileName, *pImg);
}

BOOL CtestMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

// CtestMFCDoc ���л�

void CtestMFCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  �ڴ���Ӵ洢����
	}
	else
	{
		// TODO:  �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CtestMFCDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CtestMFCDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CtestMFCDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CtestMFCDoc ���

#ifdef _DEBUG
void CtestMFCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CtestMFCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CtestMFCDoc ����
