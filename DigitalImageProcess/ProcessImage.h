#pragma once

#include <fstream>
#include <cmath>
#include "ListImage.h"

using namespace std;

class ProcessImage
{
public:
	ProcessImage(void);
	~ProcessImage(void);

public:
	void setCImageSlow(CImage &img);
	void setCImageFast(CImage &img);

	// 由文件读入图像
	bool readFile(const char *filename);



public:
	//处理过程中的数据
	unsigned short *m_pixel;  
	unsigned short *m_lung;  
	//还原图像时的数据
	unsigned short *originData;   
	//判断是不是第一次打开图片，确定图片还原数据
	bool flag;  
};
