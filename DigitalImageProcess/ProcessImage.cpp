#include "StdAfx.h"
#include "ProcessImage.h"

ProcessImage::ProcessImage(void)
{
	m_pixel = NULL;
	originData = NULL;
	flag = true;
}

ProcessImage::~ProcessImage(void)
{
	if (m_pixel != NULL)
		delete m_pixel;
	if (originData != NULL)
		delete originData;
}

void ProcessImage::setCImageSlow(CImage &img)
{
	//数据处理 
	int maxY = img.GetHeight(), maxX = img.GetWidth();  

	int index = 0; 
	for (int y=0; y<maxY; y++) {  
		for (int x=0; x<maxX; x++) {     
			unsigned short temp = m_pixel[index] / 256;
			img.SetPixelRGB(x,y,temp,temp,temp);
			index++;
		}}
}

void ProcessImage::setCImageFast(CImage &img)
{
	unsigned short* srcData = (unsigned short*)m_pixel;

	int maxY = img.GetHeight(), maxX = img.GetWidth();  

	byte* pRealData;  
	pRealData=(unsigned char *)img.GetBits();  
	int pit=img.GetPitch();  
	int bitCount=img.GetBPP()/8; 

	int index = 0;
	for (int y=0; y<maxY; y++) {  
		for (int x=0; x<maxX; x++) {  
			unsigned short temp = srcData[index]/256;
			*(pRealData + pit*y + x*bitCount) = (unsigned char)temp;  
			*(pRealData + pit*y + x*bitCount +1) = (unsigned char)temp;  
			*(pRealData + pit*y + x*bitCount +2) = (unsigned char)temp;  
			index++;
			//如果是8位灰度图像，直接读取一个BYTE位为灰度值  
			//如果是24位RGB图像，则依次读取pixAddr,pixAddr+1,pixAddr+2为B、G、R分量值  
		}
	}
	
	if (flag)
	{
		int len =  maxY * maxX;
		originData = new unsigned short[len];
		for ( int i=0; i<len; i++)
			originData[i] = m_pixel[i];
		flag = false;
	}
}

bool ProcessImage::readFile(const char * file)
{
	ifstream in(file, ios::in | ios::binary);
	if (!in)
	{
		// 打开失败
		return false;
	}
	// 读取文件长度
	in.seekg(0, ios::end);
	unsigned int fileLen = in.tellg();
	// 分配空间
	unsigned char* contents = new unsigned char[fileLen];
	// 读入文件
	in.seekg(0, ios::beg);
	in.read((char*)contents, fileLen);
	// 关闭文件
	in.close();
	
	// 赋值
	m_pixel = (unsigned short*)contents;
	return true;
}


