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
	// һ���յĸ��ƹ��캯��
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
		// ��������
		readImageFromFile(tmpPathin.c_str());

		// ��ʾԭʼ����
		//mySImage.showImageUsingOpenCV();

		// ����ԭʼ����
		// saveSrcImageToFile();

		// ����ԭʼ����

		// ���洦������
		saveResImageToFile();
	}
	return true;
}

bool SecurityImage::readImageFromFile(const char *imageFilePath)
{
	// �����ļ�·��
	this->imageFilePath = new char[strlen(imageFilePath)+1];
	strcpy_s(this->imageFilePath, strlen(imageFilePath)+1,imageFilePath);

	// ��ͼ���ļ�
	ifstream inImage(imageFilePath, ios::in | ios::binary);
	if (!inImage)
	{
		cerr << "can't open image file \"" << imageFilePath << "\"" << endl;
		return false;
	}
	// ��ȡ�ļ�����
	inImage.seekg(0, ios::end);
	int lengh = inImage.tellg();
	inImage.seekg(0, std::ios::beg);

	// ���ļ�����
	char* buffer = new char[lengh];
	inImage.read(buffer, lengh);

	// �ر��ļ�
	inImage.close();

	// ������ͷ
	channel = *reinterpret_cast<unsigned int*>(buffer);
	row = *reinterpret_cast<unsigned int*>(buffer + 4);
	col = *reinterpret_cast<unsigned int*>(buffer + 8);
	bytesOfPix = *reinterpret_cast<unsigned int*>(buffer + 12);
	maxValOfImage = *reinterpret_cast<unsigned int*>(buffer + 16);
	buffer += 20;

	// ����˫�����ݣ����˳�
	if (channel != 2)
		return false;

	// Ϊ�ߵ���ͼ�����ռ�,���������ݣ���ʱ��������Ϊת�����ݣ�row��col����ʵ���Ǳ������ģ�
	highEnergyImage = new unsigned char[row*col*bytesOfPix];
	lowEnergyImage = new unsigned char[row*col*bytesOfPix];
	memcpy(highEnergyImage, buffer, row*col*bytesOfPix);
	memcpy(lowEnergyImage, buffer + row*col*bytesOfPix, row*col*bytesOfPix);

	// ����OpenCVת�þ���
	Mat imageToTransHigh(col, row, CV_16SC1, highEnergyImage);
	Mat imageToTransLow(col, row, CV_16SC1, lowEnergyImage);
	transpose(imageToTransHigh, imageToTransHigh);
	transpose(imageToTransLow, imageToTransLow);

	//��ת�ú��ͼ�����ݸ��Ƶ������У�����ʱrow��col�е���������ʵ����������
	memcpy(highEnergyImage, imageToTransHigh.data, row*col * 2);
	memcpy(lowEnergyImage, imageToTransLow.data, row*col * 2);

	// �ͷž���
	imageToTransHigh.release();
	imageToTransLow.release();
	return true;
}

bool SecurityImage::showImageUsingOpenCV()
{
	// ����������ʾͼ���Mat����
	Mat imageToShowHigh(row, col, CV_16SC1, highEnergyImage);
	Mat imageToShowLow(row, col, CV_16SC1, lowEnergyImage);

	// �����������ֵ��һ��
	imageToShowHigh.convertTo(imageToShowHigh, CV_32FC1);
	imageToShowLow.convertTo(imageToShowLow, CV_32FC1);
	imageToShowHigh = imageToShowHigh / maxValOfImage;
	imageToShowLow = imageToShowLow / maxValOfImage;
	
	// ��ʾͼ��
	imshow("HighEnergy", imageToShowHigh);
	imshow("lowEnergy", imageToShowLow);

	waitKey(0);
	destroyAllWindows();

	// �ͷž���
	imageToShowHigh.release();
	imageToShowLow.release();

	return true;
}

bool SecurityImage::saveSrcImageToFile(string saveFilePathHigh, string saveFilePathLow )
{
	// ���챣��·��
	if (saveFilePathHigh == string("") || saveFilePathLow == string(""))
	{
		string tPath(imageFilePath);

		// ȥ��ԭ��ͼ�����ݵĺ�׺��
		size_t pos0 = tPath.find_last_of('\\');
		size_t pos1 = tPath.find_last_of('.');

		// �����ļ���
		imageFileName = tPath.substr(pos0+1, pos1 - (pos0+1));

		// ��ȡ·��ǰ׺
		tPath = tPath.substr(0, pos0 +1) + "save\\";

		// �����ļ�����·��
		saveFilePathHigh = tPath + imageFileName + "_High.bmp";
		saveFilePathLow = tPath + imageFileName + "_Low.bmp";
	}

	// �������ڱ���ͼ���Mat����
	Mat imageToSaveHigh(row, col, CV_16UC1, highEnergyImage);
	Mat imageToSaveLow(row, col, CV_16UC1, lowEnergyImage);

	// �����������ֵ��һ��
	imageToSaveHigh.convertTo(imageToSaveHigh, CV_32FC1);
	imageToSaveLow.convertTo(imageToSaveLow, CV_32FC1);
	imageToSaveHigh = imageToSaveHigh / maxValOfImage;
	imageToSaveLow = imageToSaveLow / maxValOfImage;

	// ������ֵ��Χ��[0,1] �Ŵ�[0,255];
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
	// ���챣��·��
	if (saveFilePath == string("") )
	{
		string tPath(imageFilePath);

		// ȥ��ԭ��ͼ�����ݵĺ�׺��
		size_t pos0 = tPath.find_last_of('\\');
		size_t pos1 = tPath.find_last_of('.');

		// �����ļ���
		imageFileName = tPath.substr(pos0 + 1, pos1 - (pos0 + 1));

		// ��ȡ·��ǰ׺
		tPath = tPath.substr(0, pos0 + 1) + "save\\";

		// �����ļ�����·��
		saveFilePath = tPath + imageFileName + "_Res.bmp";
	}

	// �ж���ǿ����Ƿ�Ϊ��
	if (enDualEnergyImage == nullptr)
		return false;

	// �������ڱ���ͼ���Mat����
	Mat imageToSaveRes(row, col, CV_16UC1, enDualEnergyImage);

	// �����������ֵ��һ��
	imageToSaveRes.convertTo(imageToSaveRes, CV_32FC1);
	imageToSaveRes = imageToSaveRes / maxValOfImage;

	// ������ֵ��Χ��[0,1] �Ŵ�[0,255];
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
	// ��ȡͼ������
	int maxY = AtlImg.GetHeight(), maxX = AtlImg.GetWidth();
	byte* pRealData = (unsigned char *)AtlImg.GetBits();
	int pit = AtlImg.GetPitch();
	int bitCount = AtlImg.GetBPP() / 8;

	// ����CImage����
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
	// �������ڱ���ͼ���Mat����
	Mat srcImg(row, col, CV_16UC1, highEnergyImage);
	Mat dstImg = srcImg.clone();

	//double clipLimit = 0.1;
	//Ptr<cv::CLAHE> clahe = createCLAHE();
	//clahe->setClipLimit(clipLimit);
	//clahe->apply(srcImg, dstImg);

	// �����˲���(ȥ������)
	dstImg = guidedFilter(srcImg, dstImg, 20, 10);

	// ̬ͬ�˲���
	myHomoFilter(dstImg, dstImg);

	// �����˲���(ȥ������)
	dstImg = guidedFilter(srcImg, dstImg, 20, 10);

	//Mat temp;
	//dstImg.convertTo(temp, CV_32FC1);
	//imshow("temp", temp/4096);
	//waitKey(0);


	
	// ����ռ�
	enHighEnergyImage = new unsigned char[row*col*bytesOfPix];
	if (dstImg.isContinuous())
	{
		memcpy(enHighEnergyImage, dstImg.data, row*col*bytesOfPix);
	}
	else
	{
		// ���и���
		for (int i = 0; i < dstImg.rows; i++)
		{
			memcpy(enHighEnergyImage + i*col*bytesOfPix, dstImg.ptr<uchar>(i), col*bytesOfPix);
		}
	}

	return true;
}
bool SecurityImage::enhanceLow()
{
	// �������ڱ���ͼ���Mat����
	Mat srcImg(row, col, CV_16UC1, lowEnergyImage);
	Mat dstImg = srcImg.clone();

	//double clipLimit = 0.1;
	//Ptr<cv::CLAHE> clahe = createCLAHE();
	//clahe->setClipLimit(clipLimit);
	//clahe->apply(srcImg, dstImg);

	

	// �����˲���(ȥ������)
	dstImg = guidedFilter(srcImg, dstImg, 20, 10);

	// ̬ͬ�˲���
	myHomoFilter(dstImg, dstImg);

	// �����˲���(ȥ������)
	dstImg = guidedFilter(srcImg, dstImg, 20, 10);

	
	//Mat temp;
	//dstImg.convertTo(temp, CV_32FC1);
	//imshow("temp", temp/4096);
	//waitKey(0);

	// ����ռ�
	enLowEnergyImage = new unsigned char[row*col*bytesOfPix];
	if (dstImg.isContinuous())
	{
		memcpy(enLowEnergyImage, dstImg.data, row*col*bytesOfPix);
	}
	else
	{
		// ���и���
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