#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;


//绿色通道直方图均衡化增强
Mat histPre(Mat &in)//W:局部变量的引用
{
	Mat r, g, b;
	r.create(in.size(), CV_8UC1);
	g.create(in.size(), CV_8UC1);
	b.create(in.size(), CV_8UC1);
	vector<Mat> children;

	//根据BGR分割
	children.push_back(b);
	children.push_back(g);
	children.push_back(r);
	split(in, children);

	equalizeHist(g, g);

	return g;
}



//HSI空间进行同态滤波处理，频域增强
Mat homomorphicPre(Mat & in)
{
	int rows = in.rows;
	int cols = in.cols;
	Mat rgbs[3];
	split(in, rgbs);
	Mat iI0(rows, cols, CV_64FC1), iI;//HSI模型的I分量，进行同态滤波处理
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			iI0.at<double>(i, j) = (double)(rgbs[0].at<uchar>(i, j) + rgbs[1].at<uchar>(i, j) + rgbs[2].at<uchar>(i, j)) / 3;
		}
	}
	vector<Mat> temp;
	temp.push_back(Mat_<double>(iI0));
	temp.push_back(Mat::zeros(rows, cols, CV_64FC1));
	merge(temp, iI);

	//同态滤波器常量
	Mat hFunc(rows, cols, CV_64FC2);
	int hh = 1.2;
	int hl = 0.5;
	int d0 = 50;
	int cParam = 1;
	int x0 = rows / 2;
	int y0 = cols / 2;
	int dFunc;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			dFunc = pow((i - rows), 2)+ pow((j - y0),2);
			if (dFunc == 0)
				hFunc.at<Vec2d>(i, j)[0] = hl;
			else
				hFunc.at<Vec2d>(i, j)[0] = (hh - hl)*(1 - exp(-cParam * pow(dFunc, 2) / pow(d0, 2))) + hl;
			hFunc.at<Vec2d>(i, j)[1] = hFunc.at<Vec2d>(i, j)[0];
		}
	}
	
	//对数以及傅里叶变换
	log(iI, iI);
	dft(iI, iI);
	Mat result[2] = { Mat::zeros(rows, cols, CV_64FC1), Mat::zeros(rows, cols, CV_64FC1) };
	split(iI, result);

	magnitude(result[0], result[1], result[1]);
	result[1] += Scalar::all(1);
	log(result[1], result[1]);
	namedWindow("dft", WINDOW_NORMAL);

	multiply(iI, hFunc, iI);
	//取实部分并逆傅里叶变换
	
	split(iI, result);

	magnitude(result[0], result[1], result[1]);
	result[1] += Scalar::all(1);
	log(result[1], result[1]);
	namedWindow("dft", WINDOW_NORMAL);
	imshow("dft", result[1]);

	dft(result[0], result[0], DFT_INVERSE);
	exp(result[0], result[0]);

	//根据HSI和RGB转换的规律计算增强后的图像
	Mat dst(rows, cols, CV_64FC3);
	Mat rate(rows, cols, CV_64FC1);
	divide(result[0], iI0, rate);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			dst.at<Vec3d>(i, j)[0] = rate.at<double>(i, j) * in.at<Vec3b>(i, j)[0];
			dst.at<Vec3d>(i, j)[1] = rate.at<double>(i, j) * in.at<Vec3b>(i, j)[1];
			dst.at<Vec3d>(i, j)[2] = rate.at<double>(i, j) * in.at<Vec3b>(i, j)[2];
		}
	}

	if (waitKey(0) == 27)
		return dst;

	return dst;
}


int main_4()
{
	
	Mat src, dst;
	src = imread("02_h.jpg", IMREAD_COLOR);
	if (!src.data)
	{
		cout << "ERROR INPUT" << endl;
		return -1;
	}

	
	namedWindow("src", WINDOW_FREERATIO);
	imshow("src", src);

	//绿色通道增强：对高质量图片处理效果较好
	/*dst = histPre(src);
	namedWindow("dst", WINDOW_NORMAL);
	imshow("dst", dst);
	imwrite("pre_hist_2.jpg", dst);*/

	dst = homomorphicPre(src);
	namedWindow("dst", WINDOW_NORMAL);
	imshow("dst", dst);
	imwrite("pre_homomorphic.jpg", dst);

	if (waitKey(0) == 27)
		return 0;
	return 0;

}