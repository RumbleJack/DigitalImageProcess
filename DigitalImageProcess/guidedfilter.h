#pragma once 
#include <opencv2/opencv.hpp>
using cv::Mat;

class GuidedFilterImpl;

// 实现《Guide image Filter》，类中符号与论文中对应。
// I 表示引导图像，p 表示滤波输入 ,q 表示滤波输出
class GuidedFilter
{
public:
	GuidedFilter(const Mat &I, int r = 40, double eps = 10);
	~GuidedFilter();
public:
	Mat filter(const Mat &p) const;
private:
	GuidedFilterImpl *impl_;
	
};

// -guidance image : I should be a gray - scale or color imgage
// -filtering input image : p should be a gray - scale image
// -local window radius : r  
// -regularization parameter : eps the smaller eps is ,the more detail preserved 
// 返回值为滤波结果，其位深与滤波输入p相同
Mat guidedFilter(const Mat &I, const Mat &p, int r = 20, double eps=10);
