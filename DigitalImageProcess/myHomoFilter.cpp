#include "stdafx.h"
#include"myHomoFilter.h"

#include<iostream>
using namespace std;

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

//对彩色或灰度图像进行同态滤波
bool myHomoFilter(Mat inImage, Mat& outImage)
{
	//处理彩色RGB图像
	if (inImage.channels() >= 3)	
	{
		Mat YCrCbImage;
		vector<Mat> channels;
		cvtColor(inImage, YCrCbImage, COLOR_BGR2YCrCb);
		split(YCrCbImage, channels);
		myHomoFilterForSingleChannel(channels[0], channels[0]);
		merge(channels, YCrCbImage);
		cvtColor(YCrCbImage, outImage, COLOR_YCrCb2BGR);
	} else			
	{
		//处理单通道图像		
		myHomoFilterForSingleChannel(inImage, outImage);
	}
	return true;
}

//对单通道图像同态滤波
bool myHomoFilterForSingleChannel(Mat inImage, Mat& outImage)
{
	Mat tmpImg = inImage.clone();

	// 取自然对数
	inImage.convertTo(tmpImg, CV_32FC1);
	cv::log(tmpImg + 0.00001, tmpImg);		//防止像素为0，故加一个很小的浮点数

	// DFT将图像转化到频率域
	Mat complexI;
	Mat planes[] = { Mat_<float>(tmpImg), Mat::zeros(tmpImg.size(), CV_32FC1) }; //Mat_<float>(tmpImg)是什么用法。
	merge(planes, 2, complexI);
	dft(complexI, complexI);	//计算结果应该是复数，双通道？
	split(complexI, planes); // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))

	// 生成butterWorth滤波器
	Mat butterWorth = getButterWorth(inImage.rows,inImage.cols,2000,1,1.5,0.5);
	
	// 频域相乘
	//imshow("butterWorth", butterWorth/2);
	multiply(planes[0], butterWorth, planes[0]);
	multiply(planes[1], butterWorth, planes[1]);
	merge(planes, 2, complexI);

	// IDFT反变换到时域
	idft(complexI, tmpImg, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);
	
	//取指数
	exp(tmpImg, tmpImg);
	tmpImg.convertTo(outImage, inImage.type());

	return true;
}

// 按照图像的二维频谱生成对应尺寸的巴特沃斯滤波器
Mat getButterWorth(int rows, int cols, float a, float n, float d, float e)
{
	Mat butterWorth = Mat(cv::Size(cols, rows), CV_32FC1, cv::Scalar(0));

	//生成butterworth滤波器的频域表示
	for (int i = 0; i <= rows / 2; i++)
	{
		for (int j = 0; j <= cols / 2; j++)
		{
			double q = i*i + j*j;
			double fLow = 1.0 / (1 + pow(q / a, n));
			double fHigh = 1 - fLow;
			double fBoost = d*fHigh + e;

			//直流分量为1
			if (i == 0 && j == 0)
				fBoost = 1;
			butterWorth.at<float>(i, j) = fBoost;
			//第三、一、四象限
			butterWorth.at<float>(rows - 1 - i, j) = butterWorth.at<float>(i, j);
			butterWorth.at<float>(i, cols - 1 - j) = butterWorth.at<float>(i, j);
			butterWorth.at<float>(rows - 1 - i, cols - 1 - j) = butterWorth.at<float>(i, j);
		}
	}
	return butterWorth;
}
