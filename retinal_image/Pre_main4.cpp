#include "Helper.h"
using namespace cv;
using namespace std;

int main_4() {

	namedWindow("Input", 0);
	namedWindow("Output", 0);

	//º”‘ÿÕº∆¨
	Mat src, rot, middle,dst, mask;
	src = imread("01_rot.tif", IMREAD_COLOR);
	if (src.empty()) {
		cout << "º”‘ÿÕº∆¨ ß∞‹" << endl;
		return -1;
	}
	imshow("Input", src);

	eraseBackground(src, src, mask);
	imwrite("01_mask_rot.tif", mask);
	homomorphicPre(src, dst, mask);
	imwrite("01_pre_rot.tif", dst);

	vector<vector<Point>> contours;
	retrieveCurves(dst, dst, mask, contours);
	imshow("Output", dst);

	if (waitKey(0) == 27) {
	
		destroyAllWindows();
		return 0;
	}

	return 0;
}

