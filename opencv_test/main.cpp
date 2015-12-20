#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
	Mat img = imread("C:/Users/Sarah/Desktop/2.jpg");

	imshow("color", img);

	cvtColor(img, img, CV_RGB2GRAY);

	imshow("gray", img);
	if (waitKey(0) == 27) return 0;

	return 0;
}
