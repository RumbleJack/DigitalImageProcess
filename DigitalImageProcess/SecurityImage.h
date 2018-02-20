#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ����洢��������ͼ��
class SecurityImage
{
public:
	SecurityImage();
	SecurityImage(SecurityImage& );
	~SecurityImage();

// Set��Get����
public:
	bool setHighEnergyImage(unsigned char*);
	bool setLowEnergyImage(unsigned char*);
	unsigned char* getHighEnergyImage();
	unsigned char* getLowEnergyImage();

	unsigned char* getEnHighEnergyImage();
	unsigned char* getEnLowEnergyImage();
	unsigned char* getEnDualEnergyImage();

// �����������
public:
	bool readImageFromFile(const char*);
	// ���CImage
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
