#include <opencv2/opencv.hpp>
using namespace cv;

int main_1()
{
	Mat img = imread("C:/Users/Sarah/Desktop/er zhuan.PNG");

	imshow("color", img);

	cvtColor(img, img, CV_RGB2GRAY);

	imshow("gray", img);

	imwrite("C:/Users/Sarah/Desktop/er zhuan.jpg", img);

	if (waitKey(0) == 27) return 0;

	return 0;
}
