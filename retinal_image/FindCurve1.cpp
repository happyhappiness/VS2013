#include "Helper.h"
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;



int main_1()
{
	namedWindow("dst", 0);
	Mat src, mask, back, pre, dst;

	src = imread("01.tif", IMREAD_COLOR);

	eraseBackground(src, back, mask);
	homomorphicPre(back, pre, mask);

	vector<vector<Point>> contours;
	retrieveCurves(pre, dst, mask, contours);

	imshow("dst", dst);
	
	Mat region0 = Mat(dst.size(), CV_8UC1, Scalar::all(0));
	//判断点pt是否在某点集确定的区域内
	for (int i = 0; i < dst.rows; i++)
	{
		for (int j = 0; j < dst.cols; j++)
		{
			Rect rect = minAreaRect(contours[0]).boundingRect();
			if (rect.contains(Point(j,i)))
				region0.at<uchar>(i, j) = 255;//轮廓内
			//if (pointPolygonTest(vertex, Point(j, i), false) >= 0)
		}
	}
	imshow("region0", region0);

	if (waitKey(0) == 27)
	{
		destroyAllWindows();
		return 0;
	}

	return 0;
}
