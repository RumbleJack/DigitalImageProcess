#pragma once
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

// 同态滤波
bool myHomoFilter(cv::Mat inImage, cv::Mat& outImage);
//对单通道图像同态滤波
bool myHomoFilterForSingleChannel(Mat inImage, Mat& outImage);
//生成butterWorth滤波器
Mat getButterWorth(int width, int height, float a = 1000, float n = 3, float d = 1.5, float e = 0.5);
