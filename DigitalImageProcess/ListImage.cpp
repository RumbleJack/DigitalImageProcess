#include "stdafx.h"
#include "ListImage.h"

ListImage::ListImage()
{
	m_ImgWidth=0;
	m_ImgHeight=0;
	m_Channel=1;
	m_PixType=uint_8;
	m_BitDepth=8;
	m_ImgData=NULL;
	m_DataSize=0;
}

ListImage::ListImage(ListImage &img)
{
	m_ImgWidth=img.GetImgWidth();
	m_ImgHeight=img.GetImgHeight();
	m_Channel=img.GetImgChannel();
	m_BitDepth=img.GetImgBitDepth();
	m_PixType=img.GetImgDataType();
	m_DataSize=img.GetImgDataSize();
	m_ImgData=new unsigned char [m_DataSize];
	memcpy(m_ImgData,img.GetImgBuffer(),m_DataSize);

}

ListImage::ListImage(ListImage *pImg)
{
	m_ImgWidth=pImg->GetImgWidth();
	m_ImgHeight=pImg->GetImgHeight();
	m_Channel=pImg->GetImgChannel();
	m_BitDepth=pImg->GetImgBitDepth();
	m_PixType=pImg->GetImgDataType();
	m_DataSize=pImg->GetImgDataSize();
	m_ImgData= new unsigned char [m_DataSize];
	memcpy(m_ImgData,pImg->GetImgBuffer(),m_DataSize);
}

ListImage::ListImage(unsigned int width, unsigned int height, unsigned int channel, ImgDataType type, unsigned char * buf)
{
	m_ImgWidth = width;
	m_ImgHeight = height;
	m_Channel = channel>4 ? 1 : channel;
	m_PixType = type;

	switch (type)
	{
	case uint_8:
		m_BitDepth = 8;
		break;
	case uint_12:
		m_BitDepth = 12;
		break;
	case uint_16:
		m_BitDepth = 16;
		break;
	case single:
		m_BitDepth = 32;
		break;
	case dfloat:
		m_BitDepth = 64;
		break;
	}

	m_DataSize = m_ImgWidth * m_ImgHeight * m_Channel * ((m_BitDepth + 4) / 8);
	m_ImgData = new unsigned char[m_DataSize];

	if (buf != NULL)
		memcpy(m_ImgData, buf, m_DataSize);

}

ListImage::~ListImage()
{
	if(m_ImgData!=NULL)
	{
		delete []m_ImgData;
		m_ImgData=NULL;
	}
}

ListImage & ListImage::operator = (   ListImage &img) 
{
	if(this!=&img)
	{
		if(m_ImgData != NULL)
		{
			delete []m_ImgData;
		}

		m_ImgWidth=img.GetImgWidth();
		m_ImgHeight=img.GetImgHeight();
		m_Channel=img.GetImgChannel();
		m_BitDepth=img.GetImgBitDepth();
		m_PixType=img.GetImgDataType();
		m_DataSize=img.GetImgDataSize();
		m_ImgData=new unsigned char [m_DataSize];
		memcpy(m_ImgData,img.GetImgBuffer(),m_DataSize);
	}
	return *this;
}

//ListImage ListImage::CloneRect(iRect r)
//{
//	ListImage tmImg(r.width,r.height,m_Channel,m_PixType);
//	unsigned char *pDst;
//	unsigned char *pSrc;
//	int i;
//	int pixSize = m_Channel*((m_BitDepth+4)/8);
//	for(i=r.y;i<r.y+r.height;i++)
//	{
//		pSrc=m_ImgData+pixSize*i*m_ImgWidth;
//		pDst=tmImg.GetImgBuffer()+pixSize*(i-r.y)*r.width;
//		memcpy(pDst,pSrc,pixSize*r.width);
//	}
//	return tmImg;
//}

int ListImage::SetSize(unsigned int width, unsigned int height)
{
	if(width==0||height==0)
		return -1;
	if(m_ImgWidth==width&&m_ImgHeight==height)
		return 0;

	m_ImgWidth=width;
	m_ImgHeight=height;
	m_DataSize=m_ImgWidth*m_ImgHeight*m_Channel*((m_BitDepth+4)/8);
	if(m_ImgData!=NULL)
	{
		delete []m_ImgData;
		m_ImgData=new unsigned char [m_DataSize];
	}
	else
	{
		m_ImgData=new unsigned char [m_DataSize];
	}
	return 1;
}

int ListImage::SetChannel(unsigned int channel)
{
	if(channel==0||channel>4)
		return -1;
	if(m_Channel==channel)
		return 0;


	m_Channel=channel;
	m_DataSize=m_ImgWidth*m_ImgHeight*m_Channel*((m_BitDepth+4)/8);
	if(m_ImgData!=NULL)
	{
		delete []m_ImgData;
		m_ImgData=new unsigned char [m_DataSize];
	}

	return 1;
}

int ListImage::SetType(ImgDataType type)
{
	if(m_PixType==type)
		return 0;

	m_PixType = type;
	switch(type)
	{
	case uint_8:
		m_BitDepth=8;
		break;
	case uint_12:
		m_BitDepth=12;
		break;
	case uint_16:
		m_BitDepth=16;
		break;
	case single:
		m_BitDepth=32;
		break;
	case dfloat:
		m_BitDepth=64;
		break;
	}
	m_DataSize=m_ImgWidth*m_ImgHeight*m_Channel*((m_BitDepth+4)/8);
	if(m_ImgData!=NULL)
	{
		delete []m_ImgData;
		m_ImgData=new unsigned char [m_DataSize];
	}

	return 1;
}

int ListImage::SetData(unsigned char * buf, unsigned int dataSize)
{
	if(dataSize!=m_DataSize)
		return -1;
	memcpy(m_ImgData,buf,m_DataSize);
	return 1;
}

int ListImage::SetZero()
{
	if(m_ImgData==NULL)
		return 0;

	memset(m_ImgData,0,m_DataSize);
	return 1;
}