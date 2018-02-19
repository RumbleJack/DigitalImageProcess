#pragma once

enum ImgDataType
{
	uint_8,
	uint_12,
	uint_16,
	single,
	dfloat,
};

class  ListImage 
{
// 构造析构函数
public:
	ListImage(void);
	ListImage(ListImage &img);
	ListImage(ListImage *pImg);	
	ListImage(unsigned int width, unsigned int height, unsigned int channel = 1,
		ImgDataType type = uint_8, unsigned char *buf = NULL);
	~ListImage();

public:
	ListImage & operator = (ListImage &img);
	//ListImage CloneRect(iRect r);
	//下面几个函数的返回值：-1：输入参数范围有误；0：没有做任何改变；1：成功设置并进行数据改动
	int SetSize(unsigned int width, unsigned int height);
	int SetChannel(unsigned int channel);
	int SetType(ImgDataType type);
	int SetData(unsigned char * buf, unsigned int dataSize);
	int SetZero();

	unsigned char *GetImgBuffer()const{return m_ImgData;};
	unsigned int GetImgWidth()const{return m_ImgWidth;};
	unsigned int GetImgHeight()const{return m_ImgHeight;};
	unsigned int GetImgChannel()const{return m_Channel;};
	unsigned int GetImgBitDepth()const{return m_BitDepth;};
	ImgDataType GetImgDataType()const{return m_PixType;};
	unsigned int GetImgDataSize()const{return m_DataSize;};

private:
	unsigned char * m_ImgData;
	unsigned int m_ImgWidth;
	unsigned int m_ImgHeight;
	unsigned int m_Channel;
	unsigned int m_BitDepth;
	ImgDataType  m_PixType;
	unsigned int m_DataSize;
};
