#include <iostream>
#include "Helper.h"
using namespace cv;
using namespace std;

int main_9()
{

	vector<Mat> src;
	Mat img = imread("test2.jpg", IMREAD_COLOR);
	//cvtColor(img, img, CV_RGB2GRAY);
	//medianBlur(img, img, 5);
	src.push_back(img);
	Mat img1 = imread("test1.jpg", IMREAD_COLOR);
	//medianBlur(img1, img1, 5);
	//cvtColor(img1, img1, CV_RGB2GRAY);
	src.push_back(img1);

	Mat dst;
	imageFusionWithAutoWeight(src, dst, 0);
	//imageFusionWithWaveletMax(src, dst, 1);
	GaussianBlur(dst, dst, Size(3, 3), 0, 1);
	imshow("dst", dst);

	if (waitKey(0) == 27)
	{
		destroyAllWindows();
		return 0;
	}
}