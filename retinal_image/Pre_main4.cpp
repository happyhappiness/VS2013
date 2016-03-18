#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

const int RGB2HSI = 0;
const int HSI2RGB = 1;

//绿色通道直方图均衡化增强 param1 CV_8UC3
void histPre(Mat &in, Mat& out)//W:局部变量的引用
{
	vector<Mat> children;

	//根据BGR分割
	split(in, children);
	out = children[1];//green
	out.convertTo(out, CV_8UC1, 255.0);

	equalizeHist(out, out);
}

//RGB和HSI的转换简洁版  typeRGB2HSI param1 CV_32FC3 param2 CV_32FC1 
//						typeHSI2RGB param1 CV_32FC1 param2 CV_32FC3 
void converter(Mat& in, Mat& out, int type)
{
	int rows = in.rows;
	int cols = in.cols;
	float r, g, b;
	float ii, rate;
	if (type == RGB2HSI)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				b = in.at<Vec3f>(i, j)[0];
				g = in.at<Vec3f>(i, j)[1];
				r = in.at<Vec3f>(i, j)[2];
				out.at<float>(i, j) = (r + g + b) / 3;//I
			}
		}
	}
	else if (type == HSI2RGB)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				ii = in.at<float>(i, j);
				b = out.at<Vec3f>(i, j)[0];
				g = out.at<Vec3f>(i, j)[1];
				r = out.at<Vec3f>(i, j)[2];
				rate = ii / (r + g + b) / 3;
				if (isnan(rate)) continue;
				out.at<Vec3f>(i, j)[0] *= rate;
				out.at<Vec3f>(i, j)[0] *= rate;
				out.at<Vec3f>(i, j)[0] *= rate;
			}
		}
	}

	return;
}

//高斯滤波器 param1 CV_32FC1 param2 CV_32FC1
void hef(Mat& input, Mat& output, float lower, float upper, float threshold)
{
	Mat* i_ptr = &input;
	int width = i_ptr->cols;
	int height = i_ptr->rows;
	int channel = i_ptr->channels();

	Mat* o_ptr = &output;
	if (i_ptr != o_ptr) {
		*o_ptr = Mat(height, width, CV_MAKETYPE(CV_32F, channel));
	}

	for (int y = 0; y<height; y++) {
		for (int x = 0; x<width; x++) {
			float r = sqrt((float)(x*x + y*y));
			double coeff = (1.0 - 1.0 / (1.0 + exp(r - threshold))) * (upper - lower) + lower;
			for (int c = 0; c<channel; c++) {
				o_ptr->at<float>(y, x*channel + c) = coeff * i_ptr->at<float>(y, x*channel + c);
			}
		}
	}
}

int main_4() {

	//加载图片
	Mat src = imread("02_h.jpg", CV_LOAD_IMAGE_COLOR);
	if (src.empty()) {
		cout << "加载图片失败" << endl;
		return -1;
	}
	namedWindow("Input", WINDOW_NORMAL);
	imshow("Input", src);
	int width = src.cols;
	int height = src.rows;
	int channel = src.channels();

	//转化到HSI
	src.convertTo(src, CV_32FC3, 1.0 / 255.0);
	Mat I(height, width, CV_32FC1);
	converter(src, I, RGB2HSI);
	
	float lower, upper, thresholds;
	//同态滤波
	lower = 0.5;
	upper = 1.2;
	thresholds = 70;
	dct(I, I);
	hef(I, I, lower, upper, thresholds);
	idct(I, I);

	//转化到RGB
	converter(I, src, HSI2RGB);
	namedWindow("MIDDLE", WINDOW_NORMAL);
	imshow("MIDDLE", src);

	//空间域增强
	Mat out;
	histPre(src, out);
	//imwrite("pre_4.jpg", out);
	namedWindow("Output", WINDOW_NORMAL);
	imshow("Output", out);
	if (waitKey(0) == 27) {
		destroyAllWindows();
		return 0;
	}

	return 0;
}

