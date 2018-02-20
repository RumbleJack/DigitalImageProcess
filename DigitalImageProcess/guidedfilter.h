#pragma once 
#include <opencv2/opencv.hpp>
using cv::Mat;

class GuidedFilterImpl;

// ʵ�֡�Guide image Filter�������з����������ж�Ӧ��
// I ��ʾ����ͼ��p ��ʾ�˲����� ,q ��ʾ�˲����
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
// ����ֵΪ�˲��������λ�����˲�����p��ͬ
Mat guidedFilter(const Mat &I, const Mat &p, int r = 20, double eps=10);
