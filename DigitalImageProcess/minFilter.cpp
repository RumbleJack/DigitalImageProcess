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
	//�����˲��˶�ͼ�����
	int group = 2 * r + 1;
	int rPlus = group - (src.rows + 2 * r) % (group);
	int cPlus = group - (src.cols + 2 * r) % (group);

	Mat temp;
	//����kernel��group��src��Ե��չ
	copyMakeBorder(src, temp, r, r + rPlus, r, r + cPlus, IPL_BORDER_CONSTANT, 255);
	//��������СֵG,H
	Mat G, H;
	getGHRow(temp, G, H, group);
	//��G����rλ��H����rλ��ȡ��Сֵ
	G.colRange(r, src.cols + 2 * r).copyTo(G.colRange(0, src.cols + r));
	//���ƺ�����ʱ�Ḳ�ǣ������ҪԤ�ȿ���
	Mat K = H.colRange(0, src.cols + r).clone();
	K.copyTo(H.colRange(r, src.cols + 2 * r));
	temp = min(G, H);

	Mat temp1 = temp(Range(r, src.rows + r), Range(r, src.cols + r)).clone();
	//���½�ͼ���Ե���255
	copyMakeBorder(temp1, temp, r, r + rPlus, r, r + cPlus, IPL_BORDER_CONSTANT, 255);
	//��������СֵG,H
	getGHCol(temp, G, H, group);
	//��G����rλ��H����rλ��ȡ��Сֵ
	G.rowRange(r, src.rows + 2 * r).copyTo(G.rowRange(0, src.rows + r));

	K = H.rowRange(0, src.rows+r).clone();
	K.copyTo(H.rowRange(r, src.rows + 2*r));
	temp = min(G, H);

	//�����ֵ��dst
	dst = temp(Range(r, r + src.rows), Range(r, r + src.cols));
}
void MinFilterImpl::getGHRow(Mat temp, Mat &G, Mat &H, int group)
{
	G = temp.clone();
	H = temp.clone();

	//ͳһ��������
	int tempType = temp.type();
	//G.convertTo(G, CV_32FC1);
	//H.convertTo(H, CV_32FC1);

	//��֤����
	int rows = temp.rows, cols = temp.cols;
	if (rows%group != 0 || cols% group != 0)
	{
		cerr << "temp��������" << endl;
		return;
	}

	//����GH
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

	//�ָ���������
	//G.convertTo(G, tempType);
	//H.convertTo(H, tempType);
}
void MinFilterImpl::getGHCol(Mat temp, Mat &G, Mat &H, int group)
{
	G = temp.clone();
	H = temp.clone();

	//ͳһ��������
	int tempType = temp.type();
	//G.convertTo(G, CV_32FC1);
	//H.convertTo(H, CV_32FC1);

	//��֤����
	int rows = temp.rows, cols = temp.cols;
	if (rows%group != 0 || cols% group != 0)
	{
		cerr << "temp��������" << endl;
		return;
	}

	//����GH
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

	//�ָ���������
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

//ʵ�ֿ�����Сֵ�˲�,
//src��Ϊ��ͨ������ͨ��
//dstλ��䣬ͨ������Ϊ1
//ksizeӦΪ������
void minFilter(Mat src, Mat &dst,
	Size ksize, Point anchor,
	int borderType )
{
	MinFilter(ksize.width / 2).filter(src, dst);
}