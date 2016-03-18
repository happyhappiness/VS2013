#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

Mat src, dst;

//滑动条响应函数
void onTrackBar1(int treshold1, void* userData)
{
	Canny(src, dst, treshold1, *((int *)userData));
	imshow("Output", dst);
}

//滑动条响应函数
void onTrackBar2(int treshold2, void* userData)
{
	Canny(src, dst, *((int *)userData), treshold2);
	imshow("Output", dst);
}

int main_3()
{
	namedWindow("Input", WINDOW_NORMAL);
	namedWindow("Output", WINDOW_NORMAL);

	src = imread("pre_02_h.jpg", CV_LOAD_IMAGE_COLOR);
	if (src.empty()) {
		cout << "加载图片失败" << endl;
		return -1;
	}
	imshow("Input", src);

	int treshold1 = 10;
	int treshold2 = 100;
	createTrackbar("阈值一", "Output", &treshold1, 100, onTrackBar1, &treshold2);
	createTrackbar("阈值二", "Output", &treshold2, 250, onTrackBar2, &treshold1);
	onTrackBar1(treshold1, &treshold2);

	if (waitKey(0) == 27) {
		imwrite("cannayDetect_02_h.jpg", dst);
		destroyAllWindows();
		return 0;
	}

	return 0;
}