#include "stdafx.h"
#include "SecurityImage.h"
#include <opencv2/highgui.hpp>
using namespace cv;

SecurityImage::SecurityImage()
{
	highEnergyImage = nullptr;
	lowEnergyImage = nullptr;
	enhancedImage = nullptr;

	imageFilePath = nullptr;
}

SecurityImage::SecurityImage(SecurityImage &)
{
	// һ���յĸ��ƹ��캯��
}

SecurityImage::~SecurityImage()
{
	delete[]highEnergyImage;
	highEnergyImage = nullptr;
	delete[]lowEnergyImage;
	lowEnergyImage = nullptr;
	delete[]enhancedImage;
	enhancedImage = nullptr;
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
		saveSrcImageToFile();

		// ����ԭʼ����

		// ���洦������
		// saveResImageToFile();
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
	Mat imageToSaveHigh(row, col, CV_16SC1, highEnergyImage);
	Mat imageToSaveLow(row, col, CV_16SC1, lowEnergyImage);

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
	if (enhancedImage == nullptr)
		return false;

	// �������ڱ���ͼ���Mat����
	Mat imageToSaveRes(row, col, CV_16SC1, enhancedImage);

	// �����������ֵ��һ��
	imageToSaveRes.convertTo(imageToSaveRes, CV_32FC1);
	imageToSaveRes = imageToSaveRes / maxValOfImage;

	// ������ֵ��Χ��[0,1] �Ŵ�[0,255];
	imageToSaveRes = imageToSaveRes * 255;
	imageToSaveRes.convertTo(imageToSaveRes, CV_8UC1);

	imwrite(saveFilePath, imageToSaveRes);

	return true;
}

bool SecurityImage::setHighEnergyImage(unsigned char * img)
{
	memcpy(highEnergyImage, reinterpret_cast<void*>(img), row*col*bytesOfPix);
	return true;
}

bool SecurityImage::setLowEnergyImage(unsigned char * img)
{
	memcpy(lowEnergyImage, reinterpret_cast<void*>(img), row*col*bytesOfPix);
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