#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main_3()
{
	Mat test, test2;
	test.create(100, 10, CV_64FC3);
	test2.create(100, 10, CV_64FC3);
	RNG rng;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			test.at<Vec2d>(i, j)[0] = rng.uniform( (double)1, (double)1000);
			test.at<Vec2d>(i, j)[1] = test.at<Vec2d>(i, j)[0];
		}
	}

	Mat children[3];
	split(test, children);

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			test2.at<Vec2d>(i, j)[0] = children[0].at<double>(i, j);
			test2.at<Vec2d>(i, j)[1] = test2.at<Vec2d>(i, j)[0];
		}
	}

	imshow("random", test);
	if (waitKey(0) == 27)
		return 0;
	
	return 0;
}