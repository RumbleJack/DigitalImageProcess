#pragma once 
#include <opencv2/opencv.hpp>
using namespace cv;

class MinFilterImpl;
class MinFilter
{
public:
	MinFilter(int r);
	~MinFilter(){};
	void filter(Mat src, Mat &dst);
private:
	MinFilterImpl *impl_;
};

//实现快速最小值滤波
void minFilter(Mat src, Mat &dst,
	Size ksize, Point anchor = Point(-1, -1),
	int borderType = BORDER_DEFAULT);