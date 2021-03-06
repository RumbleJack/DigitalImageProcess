#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include <opencv2/highgui.hpp>
using  cv::Mat;

// 负责存储安检数据图像
class SecurityImage
{
public:
	SecurityImage();
	SecurityImage(SecurityImage& );
	~SecurityImage();

// Set、Get函数
public:
	bool setHighEnergyImage(unsigned char*);
	bool setLowEnergyImage(unsigned char*);
	unsigned char* getHighEnergyImage();
	unsigned char* getLowEnergyImage();

	unsigned char* getEnHighEnergyImage();
	unsigned char* getEnLowEnergyImage();
	unsigned char* getEnDualEnergyImage();

// 输入输出函数
public:
	bool readImageFromFile(const char*);
	// 填充CImage
	void setCImageFast(unsigned char* srcData, CImage &m_pCImage);
	bool showImageUsingOpenCV();
	bool saveSrcImageToFile(string = string(""), string = string(""));
	bool saveResImageToFile(string = string(""));

public:
	bool enhanceALL(string dataPathFileName);
	bool enhanceHigh();
	bool enhanceLow();
	bool enhanceDual();

public:
	// 将图像灰度拉伸到区间[0,maxValOfImage],锐化后有一部分像素超出了原本的最大值
	bool strtchGrayToFullInterval(Mat &srcImg,Mat &dstImg);
	
public:
	unsigned int row;
	unsigned int col;
	unsigned int channel;
	unsigned int bytesOfPix;
	unsigned int maxValOfImage;

	char* imageFilePath;
	string imageFileName;

private:
	unsigned char* highEnergyImage;
	unsigned char* lowEnergyImage;
	unsigned char* enHighEnergyImage;
	unsigned char* enLowEnergyImage;
	unsigned char* enDualEnergyImage;
};
