#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	Mat a(3, 3, CV_32FC1);
	a.at<float>(0, 0) = -2;
	a.at<float>(0, 1) = 2;
	a.at<float>(0, 2) = 2;

	a.at<float>(1, 0) = 2;
	a.at<float>(1, 1) = 1;
	a.at<float>(1, 2) = 4;

	a.at<float>(2, 0) = 2;
	a.at<float>(2, 1) = 4;
	a.at<float>(2, 2) = 1;

	Mat values, vectors;
	eigen(a, values, vectors);
	cout << values << endl;
	cout << vectors << endl;

	return 0;
}