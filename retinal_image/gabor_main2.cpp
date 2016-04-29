#include <opencv2/core/core.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <cmath>  
#include <iostream>  

using namespace std;
using namespace cv;

const double PI = 3.14159265;

// ref: http://blog.csdn.net/watkinsong/article/details/7876361  
Mat getMyGabor(int width, int height, int U, int V, const string kernel_name)
{


	// variables for gabor filter  
	double Kmax = PI / 2;
	double f = sqrt(2.0);//频率间隔
	double sigma = 2 * PI;

	int half_width = width / 2;
	int half_height = height / 2;//gabor 滤波的大小
	double Qu = PI*U / 8;//U 决定 Qu (theta角度0-7)
	double sqsigma = sigma*sigma;
	double Kv = Kmax / pow(f, V);// V 决定 Kv (频率【尺度】0-4)
	double postmean = exp(-sqsigma / 2);

	Mat kernel_re(width, height, CV_64F);
	Mat kernel_im(width, height, CV_64F);
	Mat kernel_mag(width, height, CV_64F);

	double tmp1, tmp2, tmp3;
	for (int j = -half_height; j <= half_height; j++){
		for (int i = -half_width; i <= half_width; i++){
			tmp1 = exp(-(Kv*Kv*(j*j + i*i)) / (2 * sqsigma));
			tmp2 = cos(Kv*cos(Qu)*i + Kv*sin(Qu)*j) - postmean;//
			tmp3 = sin(Kv*cos(Qu)*i + Kv*sin(Qu)*j);

			
			kernel_re.at<double>(j + half_height, i + half_width) =
			(double)(Kv*Kv*tmp1*tmp2 / sqsigma);

			kernel_im.at<double>(j + half_height, i + half_width) =
			(double)(Kv*Kv*tmp1*tmp3 / sqsigma);
		}
	}

	magnitude(kernel_re, kernel_im, kernel_mag);

	if (kernel_name.compare("real") == 0)
		return kernel_re;
	else if (kernel_name.compare("imag") == 0)
		return kernel_im;
	else{
		printf("Invalid kernel name!\n");
		return kernel_mag;
	}
}

/**
	原图像与gabor滤波进行卷积
*/
Mat gabor_filter(Mat& img, int type)
{
	// Ref: Mian Zhou. Thesis. Gabor-Boosting Face Recognition.  
	// https://code.google.com/p/gaborboosting/  
	const int kernel_size = 69; // should be odd  
	// variables for gabor filter  
	double Kmax = PI / 2;
	double f = sqrt(2.0);
	double sigma = 2 * PI;
	int U = 7;
	int V = 4;
	int GaborH = kernel_size;
	int GaborW = kernel_size;//the size of gabor window each time
	int UStart = 0, UEnd = 8;
	int VStart = 0, VEnd = 5;

	Mat kernel_re, kernel_im;
	Mat dst_re, dst_im, dst_mag;

	// variables for filter2D  
	Point archor(-1, -1);
	int ddepth = CV_64F;//CV_64F  
	double delta = 0;

	// filter image with gabor bank  
	Mat totalMat, totalMat_re, totalMat_im;
	//多尺度
	for (V = VStart; V < VEnd; V++){
		Mat colMat, colMat_re, colMat_im;
		//多角度
		for (U = UStart; U < UEnd; U++){
			kernel_re = getMyGabor(GaborW, GaborH, U, V,"real");
			kernel_im = getMyGabor(GaborW, GaborH, U, V,"imag");
			
			//执行卷积
			filter2D(img, dst_re, ddepth, kernel_re);
			filter2D(img, dst_im, ddepth, kernel_im);

			dst_mag.create(img.rows, img.cols, CV_64FC1);
			magnitude(Mat_<float>(dst_re), Mat_<float>(dst_im),
				dst_mag);
			normalize(dst_mag, dst_mag, 0, 1, CV_MINMAX);
			normalize(dst_re, dst_re, 0, 1, CV_MINMAX);
			normalize(dst_im, dst_im, 0, 1, CV_MINMAX);

			//初始化
			if (U == UStart){
				colMat = dst_mag;
				colMat_re = dst_re;
				colMat_im = dst_im;
			}//叠加角度
			else{
				addWeighted(colMat, 0.5, dst_mag, 0.5, 0.0, colMat);
				addWeighted(colMat_re, 0.5, dst_re, 0.5, 0.0, colMat_re);
				addWeighted(colMat_im, 0.5, dst_im, 0.5, 0.0, colMat_im);/*
				vconcat(colMat, dst_mag, colMat);
				vconcat(colMat_re, dst_re, colMat_re);
				vconcat(colMat_im, dst_im, colMat_im);*/
			}
		}

		if (V == VStart){
			totalMat = colMat;
			totalMat_re = colMat_re;
			totalMat_im = colMat_im;
		}//叠加尺度
		else{
			addWeighted(totalMat, 0.5, colMat, 0.5, 0.0, totalMat);
			addWeighted(totalMat_re, 0.5, colMat_re, 0.5, 0.0, totalMat_re);
			addWeighted(totalMat_im, 0.5, colMat_im, 0.5, 0.0, totalMat_im);/*
			hconcat(totalMat, colMat, totalMat);
			hconcat(totalMat_re, colMat_re, totalMat_re);
			hconcat(totalMat_im, colMat_im, totalMat_im);*/
		}
	}

	// return   
	switch (type){
	case 0:
		return totalMat;
	case 1:
		return totalMat_re;
	case 2:
		return totalMat_im;
	default:
		return totalMat;
	}
}

Mat image, filtered_image, dst_image;
int treshold1, treshold2;

void trackBarForThreshold1(int , void* = 0){


	double real_treshold1 = ((double)treshold1); // 在 0 - 0.5之间调整
	double real_treshold2 = ((double)treshold2); // 在 0 - 0.5之间调整
	Canny(filtered_image, dst_image, real_treshold1, real_treshold2);
	imshow("dst_image", dst_image);
}
void trackBarForThreshold2(int , void* = 0){

	double real_treshold1 = ((double)treshold1); // 在 0 - 0.5之间调整
	double real_treshold2 = ((double)treshold2); // 在 0 - 0.5之间调整
	Canny(filtered_image, dst_image, real_treshold1, real_treshold2);
	imshow("dst_image", dst_image);
}

int main()
{
	namedWindow("src_image", 0);
	namedWindow("filtered_image", 0);
	namedWindow("dst_image", 0);
	
	image = imread("01_pre.tif", IMREAD_GRAYSCALE); //读取灰度图像
	if (!image.data) // Check for invalid input  
	{
		cout << "读取错误" << std::endl;
		return -1;
	}
	filtered_image.create(image.rows, image.cols, CV_64FC1);
	dst_image.create(image.rows, image.cols, CV_64FC1);
	imshow("src_image", image);

	//gabor滤波处理
	filtered_image = gabor_filter(image, 0);
	imshow("filtered_image", filtered_image);
	filtered_image.convertTo(filtered_image, CV_8UC1, 600.0);
		
	//阈值分割边缘创建腐蚀控制条 
	treshold1 = 10;
	treshold2= 10;
	createTrackbar("阈值一", "dst_image", &treshold1, 200, trackBarForThreshold1);
	createTrackbar("阈值二", "dst_image", &treshold2, 200, trackBarForThreshold2);
	trackBarForThreshold1(treshold1);

	if (waitKey(0) == 27)
	{
		imwrite("gaborFilter_01.jpg", filtered_image);//滤波处理图像保存
		imwrite("gaborCanny_01.jpg", dst_image);//滤波处理图像保存
		destroyAllWindows();
		return 0;
	}

	return 0;
}