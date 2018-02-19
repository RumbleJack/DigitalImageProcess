#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// 负责存储安检数据图像
class SecurityImage
{
public:
	SecurityImage();
	SecurityImage(SecurityImage& );
	~SecurityImage();
public:
	// 填充CImage
	void setCImageFast(unsigned char* srcData, CImage &m_pCImage);
public:
	bool enhanceALL(string dataPathFileName);
	bool readImageFromFile(const char* );
	bool showImageUsingOpenCV();
	bool saveSrcImageToFile(string = string(""), string = string(""));
	bool saveResImageToFile(string = string(""));

	bool setHighEnergyImage(unsigned char*);
	bool setLowEnergyImage(unsigned char*);
	unsigned char* getHighEnergyImage();
	unsigned char* getLowEnergyImage();

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
	unsigned char* enhancedImage;
};
