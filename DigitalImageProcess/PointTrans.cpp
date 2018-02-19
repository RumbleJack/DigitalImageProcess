#include "stdafx.h"

//#include "DIBAPI.h"

#include <math.h>
#include <direct.h>
#include "PointTrans.h"

int  PointTrans::ThresholdTrans8(ListImage *pListImage,unsigned char bThre)
{
	// 指向源图像的指针
	unsigned char*	pSrcImgData;
	// 指向源图像操作像素的指针
	int srcPos;

	// 指向复制图像的指针
	unsigned char*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 循环变量
	long i;
	long j;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned char[imgSize];
	if( pCpyImgData == NULL )
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	// 每行
	for(i = 0; i < imgH; i++)
	{
		// 每列
		for(j = 0; j < imgW; j++)
		{
			// 指向第i行，第j个象素
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;

			// 判断是否小于阈值
			if (pSrcImgData[srcPos] < bThre)
			{
				// 直接赋值为0
				pCpyImgData[cpyPos] = 0;
			}
			else
			{
				// 直接赋值为255
				pCpyImgData[cpyPos] = 255;
			}
		}
	}

	// 复制变换后的图像
	pListImage->SetData(pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
}

int  PointTrans::ThresholdTrans16(ListImage *pListImage,unsigned short bThre)
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;
	// 指向源图像操作像素的指针
	int srcPos;

	// 指向复制图像的指针
	unsigned short*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 循环变量
	long i;
	long j;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned short[imgSize/2];
	if( pCpyImgData == NULL )
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	// 每行
	for(i = 0; i < imgH; i++)
	{
		// 每列
		for(j = 0; j < imgW; j++)
		{
			// 指向第i行，第j个象素
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;

			// 判断是否小于阈值
			if (pSrcImgData[srcPos] < bThre)
			{
				// 直接赋值为0
				pCpyImgData[cpyPos] = 0;
			}
			else
			{
				// 直接赋值为65535
				pCpyImgData[cpyPos] = 65535;
			}
		}
	}

	// 复制变换后的图像
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
}

int  PointTrans::WindowTrans( ListImage *pListImage,unsigned char bLow, unsigned char bUp)
{
	// 指向源图像的指针
	unsigned char*	pSrcImgData;
	// 指向源图像操作像素的指针
	int srcPos;

	// 指向复制图像的指针
	unsigned char*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 循环变量
	long i;
	long j;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned char[imgSize];
	if( pCpyImgData == NULL )
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	// 每行
	for(i = 0; i < imgH; i++)
	{
		// 每列
		for(j = 0; j < imgW; j++)
		{
			// 指向第i行，第j个象素
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;

			// 判断是否超出范围
			if (pSrcImgData[srcPos]< bLow)
			{
				// 直接赋值为0
				pCpyImgData[cpyPos] = 0;
			}
			else if (pSrcImgData[srcPos] > bUp)
			{
				// 直接赋值为255
				pCpyImgData[cpyPos] = 255;
			}
		}
	}
	// 复制变换后的图像
	pListImage->SetData(pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
}
/*
//8位灰度拉伸
int  PointTrans::GrayStretch(ListImage *pListImage,unsigned char bX1, unsigned char bY1, unsigned char bX2, unsigned char bY2)
{
	// 指向源图像的指针
	unsigned char*	pSrcImgData;
	// 指向源图像操作像素的指针
	int srcPos;

	// 指向复制图像的指针
	unsigned char*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 循环变量
	long i;
	long j;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned char[imgSize];
	if( pCpyImgData == NULL )
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	if(bX1 > bX2)
	{
		return -2;
	}
	// 灰度映射表
	unsigned char	bMap[256];

	// 计算灰度映射表
	for (i = 0; i <= bX1; i++)
	{
		// 判断bX1是否大于0（防止分母为0）
		if (bX1 > 0)
		{
			// 线性变换
			bMap[i] = (unsigned char) bY1 * i / bX1;
		}
		else
		{
			// 直接赋值为0
			bMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)
	{
		// 判断bX1是否等于bX2（防止分母为0）
		if (bX2 != bX1)
		{
			// 线性变换
			bMap[i] = bY1 + (unsigned char) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// 直接赋值为bY1
			bMap[i] = bY1;
		}
	}
	for (; i < 256; i++)
	{
		// 判断bX2是否等于255（防止分母为0）
		if (bX2 != 255)
		{
			// 线性变换
			bMap[i] = bY2 + (unsigned char) ((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// 直接赋值为255
			bMap[i] = 255;
		}
	}

	// 每行
	for(i = 0; i < imgH; i++)
	{
		// 每列
		for(j = 0; j < imgW; j++)
		{
			// 指向第i行，第j个象素
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;

			// 计算新的灰度值
			pCpyImgData[srcPos] = bMap[pSrcImgData[srcPos]];
		}
	}
	// 复制变换后的图像
	pListImage->SetData(pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
}
*/

//16位灰度拉伸
int  PointTrans::GrayStretchUint_16(ListImage *pListImage,
							 unsigned short bX1, unsigned short bY1, unsigned short bX2, unsigned short bY2)
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;
	// 指向源图像操作像素的指针
	int srcPos;

	// 指向复制图像的指针
	unsigned short*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 循环变量
	long i;
	long j;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned short[imgSize/2];
	if( pCpyImgData == NULL )
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	if(bX1 > bX2 )
	{
		return -2;
	}

	// 灰度映射表
	unsigned short	bMap[65536];

	// 计算灰度映射表
	unsigned short grayWidth = bX2 - bX1;
	for (i = 0; i <= bX1; i++)
	{
			bMap[i] = 0;
	}
	for (i =  bX2; i < 65535; i++)
	{
			bMap[i] = 65535;
	}
	for (i = bX1; i < bX2; i++)
	{
			bMap[i] =( i - bX1 )/ (double)(bX2 - bX1) * 65536;
	}

	// 每行
	for(i = 0; i < imgH; i++)
	{
		// 每列
		for(j = 0; j < imgW; j++)
		{
			// 指向第i行，第j个象素
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;

			// 计算新的灰度值
			pCpyImgData[srcPos] = bMap[pSrcImgData[srcPos]];
		}
	}

	// 复制变换后的图像
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
}

int  PointTrans::InteEqualize( ListImage *pListImage)
{
	// 指向源图像的指针
	unsigned char*	pSrcImgData;
	// 指向源图像操作像素的指针
	int srcPos;

	// 指向复制图像的指针
	unsigned char*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 循环变量
	long i;
	long j;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned char[imgSize];
	if( pCpyImgData == NULL )
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	// 临时变量
	long	lTemp;

	// 灰度映射表
	unsigned char	bMap[256];

	// 灰度映射表
	long	lCount[256];

	// 重置计数为0
	for (i = 0; i < 256; i ++)
	{
		// 清零
		lCount[i] = 0;
	}

	// 计算各个灰度值的计数
	for (i = 0; i < imgH; i ++)
	{
		for (j = 0; j < imgW; j ++)
		{
			srcPos = imgW * i + j;

			// 计数加1
			lCount[pSrcImgData[srcPos]]++;
		}
	}

	// 计算灰度映射表
	for (i = 0; i < 256; i++)
	{
		// 初始为0
		lTemp = 0;

		for (j = 0; j <= i ; j++)
		{
			lTemp += lCount[j];
		}

		// 计算对应的新灰度值
		bMap[i] = (unsigned char) (lTemp * 255 / imgH / imgW);
	}

	// 每行
	for(i = 0; i < imgH; i++)
	{
		// 每列
		for(j = 0; j < imgW; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			srcPos = imgW * (imgH - 1 - i) + j;

			// 计算新的灰度值
			pCpyImgData[srcPos] = bMap[pSrcImgData[srcPos]];
		}
	}
	// 复制变换后的图像
	pListImage->SetData(pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
}

int PointTrans::TR_gamma(ListImage *pListImage, double Gamma,double c)
{
	// 指向源图像的指针
	unsigned char*	pSrcImgData;
	// 指向源图像操作像素的指针
	int srcPos;

	// 指向复制图像的指针
	unsigned char*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 循环变量
	long i;
	long j;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned char[imgSize];
	if( pCpyImgData == NULL )
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	// 每行
	for(i = 0; i < imgH; i++)
	{
		// 每列
		for(j = 0; j < imgW; j++)
		{
			// 指向第i行，第j个象素
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;
			pCpyImgData[cpyPos] =c*pow(pSrcImgData[srcPos],Gamma);	 

		}
	}
	// 复制变换后的图像
	pListImage->SetData(pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
}
int PointTrans::In_gamma(ListImage *pListImage, double Gamma,double c)
{
	// 指向源图像的指针
	unsigned char*	pSrcImgData;
	// 指向源图像操作像素的指针
	int srcPos;

	// 指向复制图像的指针
	unsigned char*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 循环变量
	long i;
	long j;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned char[imgSize];
	if( pCpyImgData == NULL )
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	// 每行
	for(i = 0; i < imgH; i++)
	{
		// 每列
		for(j = 0; j < imgW; j++)
		{
			// 指向第i行，第j个象素
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;
			pCpyImgData[cpyPos] = pow((pSrcImgData[srcPos]/c),(1.0/Gamma));    
		}
	}
	// 复制变换后的图像
	pListImage->SetData(pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
}
int  PointTrans::GrayReverse(ListImage *pListImage)
	{
			// 指向源图像的指针
	unsigned char*	pSrcImgData;
	// 指向源图像操作像素的指针
	int srcPos;

	// 指向复制图像的指针
	unsigned char*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 循环变量
	long i;
	long j;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned char[imgSize];
	if( pCpyImgData == NULL )
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	// 每行
	for(i = 0; i < imgH; i++)
	{
		// 每列
		for(j = 0; j < imgW; j++)
		{
			// 指向第i行，第j个象素
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;
			pCpyImgData[cpyPos] =255 - pSrcImgData[srcPos];	 

		}
	}
	// 复制变换后的图像
	pListImage->SetData(pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
	}