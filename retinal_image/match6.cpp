#include <iostream>
#include "Helper.h"
using namespace cv;
using namespace std;

int main_6()
{
	namedWindow("dst", 0);

	Mat src, src_rot, mask_rot, mask, dst, dst_rot;
	src = imread("01_pre.tif", IMREAD_GRAYSCALE);
	src_rot = imread("01_pre_rot.tif", IMREAD_GRAYSCALE);

	//vector<float> phys;
	//caclInvariantMoment(src, phys);
	//for (int i = 0; i < phys.size(); i++)
	//	cout << phys[i] << "\t";
	//cout << endl;
	//caclInvariantMoment(src_rot, phys);
	//for (int i = 0; i < phys.size(); i++)
	//	cout << phys[i] << "\t";
	//cout << endl;

	mask = imread("01_mask.tif", IMREAD_GRAYSCALE);
	mask_rot = imread("01_mask_rot.tif", IMREAD_GRAYSCALE);
	if (src.empty())
	{
		cout << "Í¼Æ¬¼ÓÔØ´íÎó" << endl;
		return -1;
	}

	showTime();
	vector<vector<Point>> contours, contours_test;
	retrieveCurves(src, dst, mask, contours);
	retrieveCurves(src_rot, dst_rot, mask_rot, contours_test);
	imshow("middle", dst);
	imshow("middle_rot", dst_rot);
	imwrite("01_region.tif", dst);
	imwrite("01_region_rot.tif", dst_rot);

	registerTwoContours(contours, contours_test, dst);
	imshow("dst", dst);
	showTime();

	//imshow("dst_rot", dst);

	if (waitKey(0) == 27)
	{
		destroyAllWindows();
		return 0;
	}


	return 0;
}