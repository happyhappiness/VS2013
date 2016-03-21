#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

enum GaussianType{ X, Y, X_X, X_Y, Y_Y};
static Mat r, dst;

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
	case X:
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				x = i - center;
				y = j - center;
				out.at<float>(i, j) = -1 / (2 * CV_PI*powf(sigma, 4)) * (x) * expf(-(x*x + y*y) / (2 * sigma*sigma));
			}
		}
		break;
	case Y:
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				x = i - center;
				y = j - center;
				out.at<float>(i, j) = -1 / (2 * CV_PI*powf(sigma, 6)) * (y) * expf(-(x*x + y*y) / (2 * sigma*sigma));
			}
		}
		break;
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

//求解对称矩阵最大特征值对应的特征向量
void findMaxEigenVector(Mat& src, Mat& dst)
{
	Mat values, vectors;
	eigen(src, values, vectors);

	double max;
	int maxIndex = 0;
	values = abs(values);
	minMaxIdx(values, (double*)0, &max, (int*)0, &maxIndex);
	//获取对应的特征向量
	dst = vectors.row(maxIndex);
}


//根据阈值判断中心线上的点
void tresholdController(int treshold, void* = 0)
{
	double realTreshold = treshold / 200;// 0 - 2两位有效数字
	threshold(r, dst, treshold, 255, CV_THRESH_BINARY);
	imshow("Output", dst);
}

int main_1()
{
	Mat src, filterXX, filterXY, filterYY, middle;
	namedWindow("Input", WINDOW_NORMAL);
	namedWindow("Output", WINDOW_NORMAL);

	src = imread("hist_01.jpg", IMREAD_GRAYSCALE);
	if (src.empty()) {
		cout << "加载图片失败" << endl;
		return -1;
	}
	imshow("Input", src);
	
	//获取px, py, rxx, rxy, ryy
	filterXX = getGuassianFilter(X_X);
	filter2D(src, filterXX, CV_32F, filterXX);

	filterXY = getGuassianFilter(X_Y);
	filter2D(src, filterXY, CV_32F, filterXY);

	filterYY = getGuassianFilter(Y_Y);
	filter2D(src, filterYY, CV_32F, filterYY);

	//获取r初值并存储在middle中
	int rows = src.rows;
	int cols = src.cols;
	middle.create(rows, cols, CV_32F);
	float rxx, ryy, rxy, nx, ny;
	Mat hessian, n;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			//获取法线方向
			rxx = filterXX.at<float>(i, j);
			rxy = filterXY.at<float>(i, j);
			ryy = filterYY.at<float>(i, j);
			float temp[] = { rxx, rxy, rxy, ryy };
			hessian = Mat(2, 2, CV_32F, temp);
			findMaxEigenVector(hessian, n);
			nx = n.at<float>(0, 0);  
			ny = n.at<float>(0, 1);
			
			//计算r矩阵【法线方向的二阶偏导数】
			middle.at<float>(i, j) = rxx*nx*nx + 2 * rxy*nx*ny + ryy*ny*ny;
		}
	}
	imshow("r''", middle);

	//3邻域最大值判断，存储结果到r中
	float nowR;
	int nowI, nowJ;
	bool isMax;
	const int SIZE = 3/2;
	middle.copyTo(r);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			nowR = middle.at<float>(i, j);
			isMax = true;
			//邻域最大值判断
			for (int m = -SIZE; m < SIZE; m++)
			{
				if (!isMax) break;
				for (int k = -SIZE; k < SIZE; k++)
				{
					nowI = i + m;
					nowJ = j + m;
					if (nowI >= 0 && nowJ >= 0 && nowI < rows && nowJ < cols && nowR < middle.at<float>(nowI, nowJ))
					{
						isMax = false;
						break;
					}
					 
				}
			}
			if (!isMax)
				r.at<float>(i, j) = 0;
			else
				r.at<float>(i, j) = nowR; 
		}
	}

	//获取曲线点
	int treshold = 100;
	createTrackbar("r阈值", "Output", &treshold, 200, tresholdController);
	tresholdController(treshold);

	if (waitKey(0) == 27)
	{
		destroyAllWindows();
		imwrite("FindCurve_01.jpg", dst);
		return 0;
	}

	return 0;
}
