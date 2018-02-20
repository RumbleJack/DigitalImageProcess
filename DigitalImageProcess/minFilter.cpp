#include "stdafx.h"
#include "minFilter.h"
#include <vector>
#include <iostream>
using namespace std;

class MinFilterImpl
{
public:
	MinFilterImpl(int radius) :r(radius){}
	virtual ~MinFilterImpl() {}
	void minFilter(Mat src, Mat &dst);
private:
	void filterSingleChannel(Mat src, Mat &dst);
	void  getGHRow(Mat temp, Mat &G, Mat &H, int group);
	void  getGHCol(Mat temp, Mat &G, Mat &H, int group);
private:
	int r;
};
void MinFilterImpl::minFilter(Mat src, Mat &dst)
{
	if (src.channels() == 1)
	{
		dst.create(src.size(), src.type());
		filterSingleChannel(src, dst);
	}
	else
	{
		vector<Mat> srcSplit;
		vector<Mat> dstSplit(3);
		split(src, srcSplit);
		for (int i = 0; i < srcSplit.size(); i++)
			filterSingleChannel(srcSplit[i], dstSplit[i]);
		dstSplit[0] = min(dstSplit[0], min(dstSplit[1], dstSplit[2]));

		dst.create(dstSplit[0].size(), dstSplit[0].type());
		dst = dstSplit[0];
	}
}
void MinFilterImpl::filterSingleChannel(Mat src, Mat &dst)
{
	//根据滤波核对图像分组
	int group = 2 * r + 1;
	int rPlus = group - (src.rows + 2 * r) % (group);
	int cPlus = group - (src.cols + 2 * r) % (group);

	Mat temp;
	//根据kernel和group将src边缘拓展
	copyMakeBorder(src, temp, r, r + rPlus, r, r + cPlus, IPL_BORDER_CONSTANT, 255);
	//计算行最小值G,H
	Mat G, H;
	getGHRow(temp, G, H, group);
	//将G左移r位，H右移r位，取最小值
	G.colRange(r, src.cols + 2 * r).copyTo(G.colRange(0, src.cols + r));
	//右移和下移时会覆盖，因此需要预先拷贝
	Mat K = H.colRange(0, src.cols + r).clone();
	K.copyTo(H.colRange(r, src.cols + 2 * r));
	temp = min(G, H);

	Mat temp1 = temp(Range(r, src.rows + r), Range(r, src.cols + r)).clone();
	//重新将图像边缘填充255
	copyMakeBorder(temp1, temp, r, r + rPlus, r, r + cPlus, IPL_BORDER_CONSTANT, 255);
	//计算列最小值G,H
	getGHCol(temp, G, H, group);
	//将G上移r位，H下移r位，取最小值
	G.rowRange(r, src.rows + 2 * r).copyTo(G.rowRange(0, src.rows + r));

	K = H.rowRange(0, src.rows+r).clone();
	K.copyTo(H.rowRange(r, src.rows + 2*r));
	temp = min(G, H);

	//结果赋值给dst
	dst = temp(Range(r, r + src.rows), Range(r, r + src.cols));
}
void MinFilterImpl::getGHRow(Mat temp, Mat &G, Mat &H, int group)
{
	G = temp.clone();
	H = temp.clone();

	//统一数据类型
	int tempType = temp.type();
	//G.convertTo(G, CV_32FC1);
	//H.convertTo(H, CV_32FC1);

	//验证参数
	int rows = temp.rows, cols = temp.cols;
	if (rows%group != 0 || cols% group != 0)
	{
		cerr << "temp参数错误" << endl;
		return;
	}

	//计算GH
	for (int c = 0; c < rows; c++)
	{
		uchar* dataG = G.ptr<uchar>(c);
		uchar* dataH = H.ptr<uchar>(c);
		for (int i = 0, j = 0; i < cols; i++, j = (j+1)%group)
		{
			if (j == 0)
				continue;
			*(dataG + i) = min(*(dataG + i - 1), *(dataG + i));
			*(dataH + i - j + (group - 1 - j)) = 
				min(*(dataH + i - j + (group - 1 - j) + 1), *(dataH + i - j + (group - 1 - j)));
		}
	}

	//恢复数据类型
	//G.convertTo(G, tempType);
	//H.convertTo(H, tempType);
}
void MinFilterImpl::getGHCol(Mat temp, Mat &G, Mat &H, int group)
{
	G = temp.clone();
	H = temp.clone();

	//统一数据类型
	int tempType = temp.type();
	//G.convertTo(G, CV_32FC1);
	//H.convertTo(H, CV_32FC1);

	//验证参数
	int rows = temp.rows, cols = temp.cols;
	if (rows%group != 0 || cols% group != 0)
	{
		cerr << "temp参数错误" << endl;
		return;
	}

	//计算GH
	int step = G.step;
	for (int c = 0; c < cols; c++)
	{
		uchar* dataG = G.ptr<uchar>(0,c);
		uchar* dataH = H.ptr<uchar>(0,c);
		for (int i = 0, j = 0; i < rows; i++, j = (j + 1) % group)
		{
			if (j == 0)
				continue;
			*(dataG + i*step) = min(*(dataG + (i - 1)*step), *(dataG + i*step));
			*(dataH + (i - j + (group - 1 - j))*step) = min(*(dataH + (i - j + (group - 1 - j) + 1)*step),
				*(dataH + (i - j + (group - 1 - j))*step));
		}
	}

	//恢复数据类型
	//G.convertTo(G, tempType);
	//H.convertTo(H, tempType);
}

MinFilter::MinFilter(int r)
{
	impl_ = new MinFilterImpl(r);
}
void MinFilter::filter(Mat src, Mat &dst)
{
	CV_Assert(src.channels() == 1 || src.channels() == 3);
	impl_->minFilter(src, dst);
}

//实现快速最小值滤波,
//src可为单通道或三通道
//dst位深不变，通道数变为1
//ksize应为正方形
void minFilter(Mat src, Mat &dst,
	Size ksize, Point anchor,
	int borderType )
{
	MinFilter(ksize.width / 2).filter(src, dst);
}