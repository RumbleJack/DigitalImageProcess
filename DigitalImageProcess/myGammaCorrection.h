#pragma once
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

void MyGammaCorrection(Mat& src, Mat& dst, float fGamma)
{
	CV_Assert(src.data);

	// 只接受16位，实际处理12位
	CV_Assert(src.depth() == sizeof(ushort));

	// 构造查找表(0~4095)
	unsigned short lut[4096];
	for (int i = 0; i < 4096; i++)
	{
		float tempVal = pow(static_cast<float>(i / 4095.0), fGamma)  * 4095.0f;
		lut[i] = saturate_cast<ushort>(tempVal);
	}

	dst = src.clone();
	MatIterator_<ushort> it, end;
	for (it = dst.begin<ushort>(), end = dst.end<ushort>(); it != end; it++)
	{
		if ((*it) < 4096)
			*it = lut[(*it)];
		else
			*it = 4095;
	}
}