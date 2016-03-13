#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using  namespace std;
using namespace cv;

const int RGB2HSI = 0;
const int HSI2RGB = 1;

//RGB和HSI的转换
void converter(Mat& in, Mat& out, int type)
{
	int rows = in.rows;
	int cols = in.cols;
	float r, g, b, mins, theta;
	float zeros = 0.0001;
	float ii, s, h, hh;
	if (type == RGB2HSI)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				b = in.at<Vec3f>(i, j)[0];
				g = in.at<Vec3f>(i, j)[1]; 
				r = in.at<Vec3f>(i, j)[2];
				out.at<Vec3f>(i, j)[0] = (r + g + b) / 3;//I

				mins = min<float>(r, g);
				mins = min<float>(mins, b);
				out.at<Vec3f>(i, j)[1] = 1 - (3*mins/(r + g + b));//S
				if (out.at<Vec3f>(i, j)[1] < 0)
					out.at<Vec3f>(i, j)[1] = zeros;
				 
				theta = acosf((2 * r - g - b) / (2 * sqrt((r-g)*(r-g) + (r-b)*(g-b)))) *180/CV_PI;
				if (theta < 0) theta = zeros;
				if (b > g)
					out.at<Vec3f>(i, j)[2] = 360 - theta;
				else
					out.at<Vec3f>(i, j)[2] = theta;//H
			}
		}
	} else if (type == HSI2RGB)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				ii = in.at<Vec3f>(i, j)[0];
				s = in.at<Vec3f>(i, j)[1];
				h = in.at<Vec3f>(i, j)[2];
				hh = h*CV_PI / 180;
				if (h <= 120)
				{
					out.at<Vec3f>(i, j)[0] = ii*(1 - s);//b
					out.at<Vec3f>(i, j)[2] = ii*(1 + s*cos(hh) / cos(1/3*CV_PI - hh));//r
					if (out.at<Vec3f>(i, j)[2] < 0)
						out.at<Vec3f>(i, j)[2] = zeros;
					out.at<Vec3f>(i, j)[1] = ii*3 - out.at<Vec3f>(i, j)[0] - out.at<Vec3f>(i, j)[2];//g
					if (out.at<Vec3f>(i, j)[1] < 0)
						out.at<Vec3f>(i, j)[1] = zeros;
					continue;
				}
				else if (h < 240)
				{
					out.at<Vec3f>(i, j)[2] = ii*(1 - s);//r
					out.at<Vec3f>(i, j)[1] = ii*(1 + s*cos(hh - 2 / 3 * CV_PI) / cos(CV_PI - hh));//g
					if (out.at<Vec3f>(i, j)[1] < 0)
						out.at<Vec3f>(i, j)[1] = zeros;
					out.at<Vec3f>(i, j)[0] = ii*3 - out.at<Vec3f>(i, j)[1] - out.at<Vec3f>(i, j)[2];//b
					if (out.at<Vec3f>(i, j)[0] < 0)
						out.at<Vec3f>(i, j)[0] = zeros;
					continue;
				}
				else
				{
					out.at<Vec3f>(i, j)[1] = ii*(1 - s);//g
					out.at<Vec3f>(i, j)[0] = ii*(1 + s*cos(hh - 4 / 3 * CV_PI) / cos(5 / 3 * CV_PI - hh));//b
					if (out.at<Vec3f>(i, j)[0] < 0)
						out.at<Vec3f>(i, j)[0] = zeros;
					out.at<Vec3f>(i, j)[2] = ii*3 - out.at<Vec3f>(i, j)[1] - out.at<Vec3f>(i, j)[0];//r
					if (out.at<Vec3f>(i, j)[2] < 0)
						out.at<Vec3f>(i, j)[2] = zeros;
				}
			}
		}
	}

	return;
}

//RGB和HSI的转换简洁版
void converter2(Mat& in, Mat& out, int type)
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
				out.at<float>(i, j)= (r + g + b) / 3;//I
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


//高斯滤波器
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

float lower, upper, thresholds;
int main() {
	// Load input image
	Mat img = imread("20051216_44092_0200_PP.tif", CV_LOAD_IMAGE_COLOR);
	if (img.empty()) {
		cout << "Failed to load file"<< endl;
		return -1;
	}
	namedWindow("Input", WINDOW_NORMAL);
	imshow("Input", img);
	int width = img.cols;
	int height = img.rows;
	int channel = img.channels();
	img.convertTo(img, CV_32FC3, 1.0 / 255.0);
	Mat I(height, width, CV_32FC1);
	converter2(img, I, RGB2HSI);
	//converter(img, img, RGB2HSI);

	// Obtain parameters from command line arguments
	
	/*cout << "  scale for  low frequency (default = 0.5): ";
	cin >> lower;
	cout << "  scale for high frequency (default = 2.0): ";
	cin >> upper;
	cout << "  threshold value for frequency domain (default = 7.5):";
	cin >> thresholds;*/
	lower = 0.5;
	upper = 1.2;
	thresholds = 70;

	//// Perform DFT, high emphasis filter and IDFT
	//vector<Mat> chs(channel, Mat(height, width, CV_32FC1)), spc(channel, Mat(height, width, CV_32FC1));
	//split(img, chs);
	//namedWindow("chs10", WINDOW_NORMAL);
	//imshow("chs10", chs[0]);

	//for (int c = 0; c<channel; c++) {
	//	dct(chs[c], spc[c]);
	//	hef(spc[c], spc[c], lower, upper, thresholds);
	//	idct(spc[c], chs[c]);
	//}
	//Mat out;
	//merge(chs, out);
	////converter(out, out, HSI2RGB);

	dct(I,I);
	hef(I, I, lower, upper, thresholds);
	idct(I, I);
	converter2(I, img, HSI2RGB);
	Mat out(img);

	// Display result
	namedWindow("Output", WINDOW_NORMAL);	
	imshow("Output", out);
	waitKey(0);
	destroyAllWindows();

	// Save result
	out.convertTo(out, CV_8UC3, 255.0);
	imwrite("pre_homomorphic_20051216_44092_0200_PP.tif", out);
}