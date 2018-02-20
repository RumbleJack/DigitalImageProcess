#include "stdafx.h"
#include "SecurityImage.h"
#include <opencv2/highgui.hpp>
using namespace cv;

#include "myHomoFilter.h"
#include "guidedfilter.h"
#include "minFilter.h"

SecurityImage::SecurityImage()
{
	highEnergyImage = nullptr;
	lowEnergyImage = nullptr;
	enHighEnergyImage = nullptr;
	enLowEnergyImage = nullptr;
	enDualEnergyImage = nullptr;

	imageFilePath = nullptr;
}

SecurityImage::SecurityImage(SecurityImage &)
{
	// 一个空的复制构造函数
}

SecurityImage::~SecurityImage()
{
	if (highEnergyImage != nullptr)
	{
		delete[]highEnergyImage;
		highEnergyImage = nullptr;
	}
	if (lowEnergyImage != nullptr)
	{
		delete[]lowEnergyImage;
		lowEnergyImage = nullptr;
	}
	/*if (enDualEnergyImage != nullptr)
	{
		delete[]enDualEnergyImage;
		enDualEnergyImage = nullptr;
	}
	if (enLowEnergyImage != nullptr)
	{
		delete[]enLowEnergyImage;
		enLowEnergyImage = nullptr;
	}
	if (enHighEnergyImage != nullptr)
	{
		delete[]enHighEnergyImage;
		enHighEnergyImage = nullptr;
	}*/

	delete imageFilePath;
	imageFilePath = nullptr;
}

bool SecurityImage::enhanceALL(string dataPathFileName)
{
	ifstream inf(dataPathFileName);
	string  tmpPathin;
	while (getline(inf, tmpPathin))
	{
		// 读入数据
		readImageFromFile(tmpPathin.c_str());

		// 显示原始数据
		//mySImage.showImageUsingOpenCV();

		// 保存原始数据
		// saveSrcImageToFile();

		// 处理原始数据

		// 保存处理数据
		saveResImageToFile();
	}
	return true;
}

bool SecurityImage::readImageFromFile(const char *imageFilePath)
{
	// 保存文件路径
	this->imageFilePath = new char[strlen(imageFilePath)+1];
	strcpy_s(this->imageFilePath, strlen(imageFilePath)+1,imageFilePath);

	// 打开图像文件
	ifstream inImage(imageFilePath, ios::in | ios::binary);
	if (!inImage)
	{
		cerr << "can't open image file \"" << imageFilePath << "\"" << endl;
		return false;
	}
	// 获取文件长度
	inImage.seekg(0, ios::end);
	int lengh = inImage.tellg();
	inImage.seekg(0, std::ios::beg);

	// 读文件数据
	char* buffer = new char[lengh];
	inImage.read(buffer, lengh);

	// 关闭文件
	inImage.close();

	// 读数据头
	channel = *reinterpret_cast<unsigned int*>(buffer);
	row = *reinterpret_cast<unsigned int*>(buffer + 4);
	col = *reinterpret_cast<unsigned int*>(buffer + 8);
	bytesOfPix = *reinterpret_cast<unsigned int*>(buffer + 12);
	maxValOfImage = *reinterpret_cast<unsigned int*>(buffer + 16);
	buffer += 20;

	// 若非双能数据，则退出
	if (channel != 2)
		return false;

	// 为高低能图像分配空间,并复制数据（此时读入数据为转置数据，row和col数据实际是被交换的）
	highEnergyImage = new unsigned char[row*col*bytesOfPix];
	lowEnergyImage = new unsigned char[row*col*bytesOfPix];
	memcpy(highEnergyImage, buffer, row*col*bytesOfPix);
	memcpy(lowEnergyImage, buffer + row*col*bytesOfPix, row*col*bytesOfPix);

	// 利用OpenCV转置矩阵
	Mat imageToTransHigh(col, row, CV_16SC1, highEnergyImage);
	Mat imageToTransLow(col, row, CV_16SC1, lowEnergyImage);
	transpose(imageToTransHigh, imageToTransHigh);
	transpose(imageToTransLow, imageToTransLow);

	//将转置后的图像数据复制到数组中，（此时row和col中的数据是真实的行列数）
	memcpy(highEnergyImage, imageToTransHigh.data, row*col * 2);
	memcpy(lowEnergyImage, imageToTransLow.data, row*col * 2);

	// 释放矩阵
	imageToTransHigh.release();
	imageToTransLow.release();
	return true;
}

bool SecurityImage::showImageUsingOpenCV()
{
	// 定义用于显示图像的Mat矩阵
	Mat imageToShowHigh(row, col, CV_16SC1, highEnergyImage);
	Mat imageToShowLow(row, col, CV_16SC1, lowEnergyImage);

	// 利用像素最大值归一化
	imageToShowHigh.convertTo(imageToShowHigh, CV_32FC1);
	imageToShowLow.convertTo(imageToShowLow, CV_32FC1);
	imageToShowHigh = imageToShowHigh / maxValOfImage;
	imageToShowLow = imageToShowLow / maxValOfImage;
	
	// 显示图像
	imshow("HighEnergy", imageToShowHigh);
	imshow("lowEnergy", imageToShowLow);

	waitKey(0);
	destroyAllWindows();

	// 释放矩阵
	imageToShowHigh.release();
	imageToShowLow.release();

	return true;
}

bool SecurityImage::saveSrcImageToFile(string saveFilePathHigh, string saveFilePathLow )
{
	// 构造保存路径
	if (saveFilePathHigh == string("") || saveFilePathLow == string(""))
	{
		string tPath(imageFilePath);

		// 去掉原本图像数据的后缀名
		size_t pos0 = tPath.find_last_of('\\');
		size_t pos1 = tPath.find_last_of('.');

		// 保存文件名
		imageFileName = tPath.substr(pos0+1, pos1 - (pos0+1));

		// 截取路径前缀
		tPath = tPath.substr(0, pos0 +1) + "save\\";

		// 生成文件保存路径
		saveFilePathHigh = tPath + imageFileName + "_High.bmp";
		saveFilePathLow = tPath + imageFileName + "_Low.bmp";
	}

	// 定义用于保存图像的Mat矩阵
	Mat imageToSaveHigh(row, col, CV_16UC1, highEnergyImage);
	Mat imageToSaveLow(row, col, CV_16UC1, lowEnergyImage);

	// 利用像素最大值归一化
	imageToSaveHigh.convertTo(imageToSaveHigh, CV_32FC1);
	imageToSaveLow.convertTo(imageToSaveLow, CV_32FC1);
	imageToSaveHigh = imageToSaveHigh / maxValOfImage;
	imageToSaveLow = imageToSaveLow / maxValOfImage;

	// 将像素值范围从[0,1] 放大到[0,255];
	imageToSaveHigh = imageToSaveHigh * 255;
	imageToSaveLow = imageToSaveLow * 255;
	imageToSaveHigh.convertTo(imageToSaveHigh, CV_8UC1);
	imageToSaveLow.convertTo(imageToSaveLow, CV_8UC1);

	imwrite(saveFilePathHigh, imageToSaveHigh);
	imwrite(saveFilePathLow, imageToSaveLow);

	return true;
}

bool SecurityImage::saveResImageToFile(string saveFilePath)
{
	// 构造保存路径
	if (saveFilePath == string("") )
	{
		string tPath(imageFilePath);

		// 去掉原本图像数据的后缀名
		size_t pos0 = tPath.find_last_of('\\');
		size_t pos1 = tPath.find_last_of('.');

		// 保存文件名
		imageFileName = tPath.substr(pos0 + 1, pos1 - (pos0 + 1));

		// 截取路径前缀
		tPath = tPath.substr(0, pos0 + 1) + "save\\";

		// 生成文件保存路径
		saveFilePath = tPath + imageFileName + "_Res.bmp";
	}

	// 判断增强结果是否为空
	if (enDualEnergyImage == nullptr)
		return false;

	// 定义用于保存图像的Mat矩阵
	Mat imageToSaveRes(row, col, CV_16UC1, enDualEnergyImage);

	// 利用像素最大值归一化
	imageToSaveRes.convertTo(imageToSaveRes, CV_32FC1);
	imageToSaveRes = imageToSaveRes / maxValOfImage;

	// 将像素值范围从[0,1] 放大到[0,255];
	imageToSaveRes = imageToSaveRes * 255;
	imageToSaveRes.convertTo(imageToSaveRes, CV_8UC1);

	imwrite(saveFilePath, imageToSaveRes);

	return true;
}

bool SecurityImage::setHighEnergyImage(unsigned char * data)
{
	memcpy(highEnergyImage, reinterpret_cast<void*>(data), row*col*bytesOfPix);
	return true;
}

bool SecurityImage::setLowEnergyImage(unsigned char * data)
{
	memcpy(lowEnergyImage, reinterpret_cast<void*>(data), row*col*bytesOfPix);
	return true;
}

unsigned char * SecurityImage::getHighEnergyImage()
{
	return highEnergyImage;
}

unsigned char * SecurityImage::getLowEnergyImage()
{
	return lowEnergyImage;
}

unsigned char* SecurityImage::getEnHighEnergyImage()
{
	return enHighEnergyImage;
}
unsigned char* SecurityImage::getEnLowEnergyImage()
{
	return enLowEnergyImage;
}
unsigned char* SecurityImage::getEnDualEnergyImage()
{
	return enDualEnergyImage;
}
void SecurityImage::setCImageFast(unsigned char* srcData,CImage &AtlImg)
{
	if (srcData == nullptr)
		return;
	// 获取图像属性
	int maxY = AtlImg.GetHeight(), maxX = AtlImg.GetWidth();
	byte* pRealData = (unsigned char *)AtlImg.GetBits();
	int pit = AtlImg.GetPitch();
	int bitCount = AtlImg.GetBPP() / 8;

	// 设置CImage像素
	int index = 0;
	for (int y = 0; y < maxY; y++) {
		for (int x = 0; x<maxX; x++) {
			unsigned short temp = ((unsigned short*)srcData)[index] / (maxValOfImage/255.0);
			*(pRealData + pit*y + x*bitCount) = temp < 256 ? temp : 255;
			*(pRealData + pit*y + x*bitCount + 1) = temp < 256 ? temp : 255;
			*(pRealData + pit*y + x*bitCount + 2) = temp < 256 ? temp : 255;
			index++;
		}
	}
}

bool SecurityImage::enhanceHigh()
{
	// 定义用于保存图像的Mat矩阵
	Mat srcImg(row, col, CV_16UC1, highEnergyImage);
	Mat dstImg = srcImg.clone();

	//double clipLimit = 0.1;
	//Ptr<cv::CLAHE> clahe = createCLAHE();
	//clahe->setClipLimit(clipLimit);
	//clahe->apply(srcImg, dstImg);

	// 引导滤波器(去除光晕)
	dstImg = guidedFilter(srcImg, dstImg, 20, 10);

	// 同态滤波器
	myHomoFilter(dstImg, dstImg);

	// 引导滤波器(去除光晕)
	dstImg = guidedFilter(srcImg, dstImg, 20, 10);

	//Mat temp;
	//dstImg.convertTo(temp, CV_32FC1);
	//imshow("temp", temp/4096);
	//waitKey(0);


	
	// 分配空间
	enHighEnergyImage = new unsigned char[row*col*bytesOfPix];
	if (dstImg.isContinuous())
	{
		memcpy(enHighEnergyImage, dstImg.data, row*col*bytesOfPix);
	}
	else
	{
		// 按行复制
		for (int i = 0; i < dstImg.rows; i++)
		{
			memcpy(enHighEnergyImage + i*col*bytesOfPix, dstImg.ptr<uchar>(i), col*bytesOfPix);
		}
	}

	return true;
}
bool SecurityImage::enhanceLow()
{
	// 定义用于保存图像的Mat矩阵
	Mat srcImg(row, col, CV_16UC1, lowEnergyImage);
	Mat dstImg = srcImg.clone();

	//double clipLimit = 0.1;
	//Ptr<cv::CLAHE> clahe = createCLAHE();
	//clahe->setClipLimit(clipLimit);
	//clahe->apply(srcImg, dstImg);

	

	// 引导滤波器(去除光晕)
	dstImg = guidedFilter(srcImg, dstImg, 20, 10);

	// 同态滤波器
	myHomoFilter(dstImg, dstImg);

	// 引导滤波器(去除光晕)
	dstImg = guidedFilter(srcImg, dstImg, 20, 10);

	
	//Mat temp;
	//dstImg.convertTo(temp, CV_32FC1);
	//imshow("temp", temp/4096);
	//waitKey(0);

	// 分配空间
	enLowEnergyImage = new unsigned char[row*col*bytesOfPix];
	if (dstImg.isContinuous())
	{
		memcpy(enLowEnergyImage, dstImg.data, row*col*bytesOfPix);
	}
	else
	{
		// 按行复制
		for (int i = 0; i < dstImg.rows; i++)
		{
			memcpy(enLowEnergyImage + i*col*bytesOfPix, dstImg.ptr<uchar>(i), col*bytesOfPix);
		}
	}

	return true;
}
bool SecurityImage::enhanceDual()
{
	enDualEnergyImage = highEnergyImage;
	return true;
}