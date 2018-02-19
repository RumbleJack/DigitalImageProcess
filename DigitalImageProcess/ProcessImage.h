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

	// ���ļ�����ͼ��
	bool readIn(const char *filename);

	int get_total(char *fileName);

public:
	//��������е�����
	unsigned short *m_pixel;  
	unsigned short *m_lung;  
	//��ԭͼ��ʱ������
	unsigned short *originData;   
	//�ж��ǲ��ǵ�һ�δ�ͼƬ��ȷ��ͼƬ��ԭ����
	bool flag;  
};
