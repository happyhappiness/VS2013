#include "Helper.h"
using namespace cv;
using namespace std;


static Mat src, gray, dst, mask;
void onStepController(int step, void* = 0)
{
	eraseBackgroundWithMask(gray, dst, mask, step);
	imshow("Output", dst);

}

int main() {

	namedWindow("Input", 0);
	namedWindow("Output", 0);

	//º”‘ÿÕº∆¨
	src = imread("01_withoutBack.tif", IMREAD_COLOR);
	if (src.empty()) {
		cout << "º”‘ÿÕº∆¨ ß∞‹" << endl;
		return -1;
	}
	imshow("Input", src);

	cvtColor(src, gray, CV_RGB2GRAY);
	gray.convertTo(gray, CV_32F);
	
	//Õ¨Ã¨¬À≤®
	float lower, upper, thresholds;
	lower = 1.0;
	upper = 2.5;
	thresholds = 100;

	dct(gray, gray);
	hef(gray, gray, lower, upper, thresholds);
	idct(gray, gray);
	gray.convertTo(gray, CV_8U);

	mask = imread("01_mask.tif", IMREAD_GRAYSCALE);
	if (mask.empty()) {
		cout << "º”‘ÿƒ£∞Â ß∞‹" << endl;
		return -1;
	}

	int step = 14;
	createTrackbar("ÀıΩ¯≤Ω ˝", "Output", &step, 100, onStepController);
	onStepController(step);

	if (waitKey(0) == 27) {
		destroyAllWindows();
		imwrite("pre_01.tif", dst);
		return 0;
	}

	return 0;
}

