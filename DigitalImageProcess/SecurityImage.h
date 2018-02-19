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
	unsigned int channel;
	unsigned int row;
	unsigned int col;
	unsigned int bytesOfPix;
	unsigned int maxValOfImage;

	char* imageFilePath;
	string imageFileName;

private:
	unsigned char* highEnergyImage;
	unsigned char* lowEnergyImage;
	unsigned char* enhancedImage;
};
