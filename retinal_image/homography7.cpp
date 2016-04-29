#include <iostream>
#include "Helper.h"
using namespace cv;
using namespace std;

int main_7()
{
	namedWindow("dst", 0);
	Mat dst = imread("dst.tif", IMREAD_COLOR);
	Mat roi = dst(Rect(10, 10, 100, 100));
	Mat black = Mat(roi.size(), CV_8UC3, Scalar::all(0));
	black.copyTo(roi);
	imshow("roi", roi);
	imshow("dst", dst);

	if (waitKey(0) == 27)
	{
		destroyAllWindows();
		return 0;
	}

	return 0;
}