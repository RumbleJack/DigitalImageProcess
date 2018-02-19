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
	//���ݴ��� 
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
			//�����8λ�Ҷ�ͼ��ֱ�Ӷ�ȡһ��BYTEλΪ�Ҷ�ֵ  
			//�����24λRGBͼ�������ζ�ȡpixAddr,pixAddr+1,pixAddr+2ΪB��G��R����ֵ  
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
		// ��ʧ��
		return false;
	}
	// ��ȡ�ļ�����
	in.seekg(0, ios::end);
	unsigned int fileLen = in.tellg();
	// ����ռ�
	unsigned char* contents = new unsigned char[fileLen];
	// �����ļ�
	in.seekg(0, ios::beg);
	in.read((char*)contents, fileLen);
	// �ر��ļ�
	in.close();
	
	// ��ֵ
	m_pixel = (unsigned short*)contents;
	return true;
}


