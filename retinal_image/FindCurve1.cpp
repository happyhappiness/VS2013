#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

enum GaussianType{X_X, X_Y, Y_Y};

//返回二维二阶高斯函数sigma = 2.6; size = 5;
Mat getGuassianFilter(GaussianType type)
{
	int n = 5;
	float sigma = 2.25;

	int center = n / 2;
	float x, y;
	Mat out(n, n, CV_32FC1);
	switch (type)
	{
	case X_X:
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				x = i - center;
				y = j - center;
				out.at<float>(i, j) = -1 / (2 * CV_PI*powf(sigma, 6)) * (sigma*sigma - x) * expf(-(x*x + y*y) / (2 * sigma*sigma));
			}
		}
		break;
	case X_Y:
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				x = i - center;
				y = j - center;
				out.at<float>(i, j) = (x*y) / (2 * CV_PI*powf(sigma, 6)) * expf(-(x*x + y*y) / (2 * sigma*sigma));
			}
		}
		break;
	case Y_Y:
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				x = i - center;
				y = j - center;
				out.at<float>(i, j) = -1 / (2 * CV_PI*powf(sigma, 6)) * (sigma*sigma - y) * expf(-(x*x + y*y) / (2 * sigma*sigma));
			}
		}
		break;
	}
	normalize(out, out, -1, 1, NORM_MINMAX);
    return out;
}

int main_1()
{
	Mat src, filter, dst1, dst2, dst3;
	namedWindow("Input", WINDOW_NORMAL);
	namedWindow("Output1", WINDOW_NORMAL);
	namedWindow("Output2", WINDOW_NORMAL);
	namedWindow("Output3", WINDOW_NORMAL);

	src = imread("pre_hist_02_h.jpg", IMREAD_GRAYSCALE);
	if (src.empty()) {
		cout << "加载图片失败" << endl;
		return -1;
	}
	imshow("Input", src);

	FileStorage fs;
	fs.open("gaussianFIlter.xml", FileStorage::WRITE);

	filter = getGuassianFilter(X_X);
	fs << "XX" << filter;
	filter2D(src, dst1, CV_32F, filter);
	imshow("Output1", dst1);
	filter = getGuassianFilter(X_Y);
	fs << "XY" << filter;
	filter2D(src, dst2, CV_32F, filter);
	imshow("Output2", dst2);
	filter = getGuassianFilter(Y_Y);
	fs << "YY" << filter;
	filter2D(src, dst3, CV_32F, filter);
	imshow("Output3", dst3);
	if (waitKey(0) == 27)
	{
		//imwrite("FindCurve_02_h.jpg", dst);
		destroyAllWindows();
		fs.release();
		return 0;
	}

	return 0;
}
