//  API函数库：
//  TemplateConvConv()	- 图像模板变换，通过改变模板，可以用它实现
//						  图像的平滑、锐化、边缘识别等操作。
//  MedianFilter()		- 图像中值滤波。
//  GetMedianNum()		- 获取中值。被函数MedianFilter()调用来求中值。
//  GradSharp()		    - 该函数用来对图像进行梯度锐化。
//  Robert()		    - robert边缘检测运算
//  Sobel()		        - sobel边缘检测运算
//  Prewitt()	        - prewitt边缘检测运算
//  Kirsch()		    - kirsch边缘检测运算
//  Gauss()		        - gauss边缘检测运算
//  Canny()				- Canny边缘检测运算
//  UnsharpMasking -反锐化掩模

#include "stdafx.h"
#include "ConvFilter.h"

#include <math.h>
#include <direct.h>
ConvFilter::ConvFilter(void)
{
	//调用默认的FilterKernel构造函数初始化成员变量
}	
ConvFilter::ConvFilter(FilterKernel &Temp)
{
	m_Temp = Temp;
}
ConvFilter::ConvFilter(int iTempH,int iTempW,int iTempMX,int iTempMY, float* fpArray, float fCoef )
{
	m_Temp.SetFilterKernel(iTempH,iTempW,iTempMX,iTempMY,fpArray,fCoef );
}

ConvFilter::ConvFilter(ConvFilter &TDF)
{
	m_Temp = TDF.m_Temp;
}
ConvFilter::ConvFilter(ConvFilter *pTDF)
{
	m_Temp = pTDF->m_Temp;
}

ConvFilter::~ConvFilter()
{

}


int ConvFilter::SetFK(FilterKernel &Temp)
{
	if( !IsFilterKernelIllegal( Temp) )
	{
		return -3;
	}
	m_Temp = Temp;

	return 0;
}

int ConvFilter::SetFK(int iTempH,int iTempW,int iTempMX,int iTempMY,float* fA,float fc)
{
	return m_Temp.SetFilterKernel(iTempH,iTempW,iTempMX,iTempMY,fA,fc);
}


int ConvFilter::TemplateConv( ListImage srcImg,ListImage &destImg,int mode)
{
	destImg = srcImg;
	return TemplateConv(&destImg,m_Temp,mode);
}


int ConvFilter::TemplateConv(ListImage *pListImage,FilterKernel &Temp,int mode)
{
	if ( !IsImageIllegal( pListImage) )
	{
		return -2;
	}
	switch (pListImage->GetImgDataType())
	{
	case uint_8:
		return TemplateConvUint_8 (pListImage,Temp,mode);
		break;
	case uint_12:
		return TemplateConvUint_12(pListImage,Temp,mode);
		break;
	case uint_16:
		return TemplateConvUint_16(pListImage,Temp,mode);
		break;
	case single:
		return TemplateConvSingle(pListImage,Temp,mode);
		break;
	default:
		return TemplateConvUint_8 (pListImage,Temp,mode);
		break;
	}
}

int ConvFilter::TemplateConvUint_8(ListImage *pListImage,FilterKernel &Temp,int mode)
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
	long k;
	long l;

	// 计算结果
	float fResult;

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

	switch(mode)
	{
	case 0:
		//保持边缘像素原样
		//去掉边缘几行//去掉边缘几列
		for(i = Temp.iTempMY; i <imgH - Temp.iTempH + Temp.iTempMY + 1 ; i++)
		{
			for(j = Temp.iTempMX; j <imgW - Temp.iTempW + Temp.iTempMX + 1; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos = imgW * i + j;
				fResult = 0;

				// 计算
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针

						srcPos = imgW * (i - Temp.iTempMY + k )
							+ j - Temp.iTempMX + l;

						fResult += pSrcImgData[srcPos] * Temp.fpArray[k * Temp.iTempW + l];
					}
				}
				// 乘上系数
				fResult *= Temp.fCoef;
				fResult = (float) fabs(fResult);
				if(fResult > 255)
				{
					pCpyImgData[cpyPos] = 255;
				}
				else
				{
					pCpyImgData[cpyPos]  = (unsigned char) (fResult + 0.5);
				}
			}
		}
		break;
	case 1:
		//在边缘处补零0.30..........3990处理边缘像素
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos =  imgW * i + j;
				fResult = 0;

				// 计算
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						if(    i - Temp.iTempMY + k < 0 
							|| j - Temp.iTempMX + l < 0 
							|| i - Temp.iTempMY + k > imgH - 1
							|| j - Temp.iTempMX + l > imgW - 1 )
						{
							pSrcImgData [srcPos]= 0;
						}
						else
						{
							// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
							srcPos = imgW * (i - Temp.iTempMY + k )
								+ j - Temp.iTempMX + l;
						}
						fResult += pSrcImgData [srcPos]  *  Temp.fpArray[k * Temp.iTempW + l];
					}
				}
				fResult *= Temp.fCoef;
				fResult = (float) fabs(fResult);
				if(fResult > 255)
				{
					pCpyImgData[cpyPos]  = 255;
				}
				else
				{
					pCpyImgData[cpyPos]  = (unsigned char) (fResult + 0.5);
				}
			}
		}
		break;
	case 2:
		int L;		//行数
		int C;		//列数

		//在边缘处补对边边缘像素处理	
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos = imgW * i + j;
				fResult = 0;

				// 计算
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						L = (i - Temp.iTempMY + k );
						C = j - Temp.iTempMX + l;
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针

						if( i - Temp.iTempMY + k < 0 )
						{
							L = imgH + ( i - Temp.iTempMY + k );
						}
						if( j - Temp.iTempMX + l < 0 )
						{
							C = imgW + ( j - Temp.iTempMX + l );
						}
						if( i - Temp.iTempMY + k > imgH - 1 )
						{
							L = i - Temp.iTempMY + k - imgH;
						}
						if( j - Temp.iTempMX + l > imgW - 1 )
						{
							C = j - Temp.iTempMX + l - imgW;
						}

						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						srcPos =  imgW * L
							+ C;
						fResult += pSrcImgData [srcPos] * Temp.fpArray[k * Temp.iTempW + l];
					}
				}
				fResult *= Temp.fCoef;
				fResult = (float) fabs(fResult);
				if(fResult > 255)
				{
					pCpyImgData[cpyPos]  = 255;
				}
				else
				{
					pCpyImgData[cpyPos]  = (unsigned char) (fResult + 0.5);
				}
			}
		}
		break;
	default:
		break;
	}


	// 复制变换后的图像
	pListImage->SetData(pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;
	return 0;
}
int ConvFilter::TemplateConvUint_12(ListImage *pListImage,FilterKernel &Temp,int mode)
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;
	// 指向源图像操作像素的指针
	int	srcPos;

	// 指向复制图像的指针
	unsigned short*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int	cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 循环变量
	long i;
	long j;
	long k;
	long l;

	// 计算结果
	float fResult;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned short[imgSize / 2];
	if( pCpyImgData == NULL )
	{
		return -1;
	}

	memcpy(pCpyImgData,pSrcImgData,imgSize);

	switch(mode)
	{
	case 0:
		//保持边缘像素原样
		//去掉边缘几行//去掉边缘几列
		for(i = Temp.iTempMY; i <imgH - Temp.iTempH + Temp.iTempMY + 1 ; i++)
		{
			for(j = Temp.iTempMX; j <imgW - Temp.iTempW + Temp.iTempMX + 1 ; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos =  imgW * i + j;
				fResult = 0;

				// 计算
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						srcPos = imgW * (i - Temp.iTempMY + k )
							+ j - Temp.iTempMX + l;
						fResult +=( pSrcImgData [srcPos] & (4095) )
							* Temp.fpArray[k * Temp.iTempW + l];

					}
				}
				// 乘上系数
				fResult *= Temp.fCoef;
				fResult = (float) fabs(fResult);

				if(fResult > 4095)
				{
					pCpyImgData[cpyPos]  = 4095;
				}
				else
				{
					pCpyImgData[cpyPos]  = (unsigned short)(fResult+0.5);

				}
			}
		}
		break;
	case 1:
		//在边缘处补零处理边缘像素
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos =  imgW * i + j;
				fResult = 0;

				// 计算
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						if(    i - Temp.iTempMY + k < 0 
							|| j - Temp.iTempMX + l < 0 
							|| i - Temp.iTempMY + k > imgH - 1
							|| j - Temp.iTempMX + l > imgW - 1 )
						{
							pSrcImgData [srcPos] = 0;
						}
						else
						{
							// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
							srcPos = imgW * (i - Temp.iTempMY + k )
								+ j - Temp.iTempMX + l;
						}
						fResult +=(pSrcImgData [srcPos]& (4095) )
							* Temp.fpArray[k * Temp.iTempW + l];
					}
				}
				fResult *= Temp.fCoef;
				fResult = (float) fabs(fResult);
				if(fResult > 4095)
				{
					pCpyImgData[cpyPos]  = 4095;
				}
				else
				{
					pCpyImgData[cpyPos]  = (unsigned short) (fResult + 0.5);
				}
			}
		}
		break;
	case 2:

		int L;		//行数
		int C;		//列数

		//在边缘处补对边边缘像素处理	
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos =  imgW * i + j;
				fResult = 0;

				// 计算
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						L = (i - Temp.iTempMY + k );
						C = j - Temp.iTempMX + l;
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						if( i - Temp.iTempMY + k < 0 )
						{
							L = imgH + ( i - Temp.iTempMY + k );
						}
						if( j - Temp.iTempMX + l < 0 )
						{
							C = imgW + ( j - Temp.iTempMX + l );
						}
						if( i - Temp.iTempMY + k > imgH - 1 )
						{
							L = i - Temp.iTempMY + k - imgH;
						}
						if( j - Temp.iTempMX + l > imgW - 1 )
						{
							C = j - Temp.iTempMX + l - imgW;
						}
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						srcPos =  imgW * L
							+ C;
						fResult +=( pSrcImgData [srcPos] & (4095) )
							* Temp.fpArray[k * Temp.iTempW + l];
					}
				}
				fResult *= Temp.fCoef;
				fResult = (float) fabs(fResult);
				if(fResult > 4095)
				{
					pCpyImgData[cpyPos]  = 4095;
				}
				else
				{
					pCpyImgData[cpyPos]  = (unsigned short) (fResult + 0.5);
				}
			}
		}
		break;
	default:
		break;
	}

	// 复制变换后的图像
	pListImage->SetData( (unsigned char*)pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;
	return 0;
}
int ConvFilter::TemplateConvUint_16(ListImage *pListImage,FilterKernel &Temp,int mode)
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;
	// 指向源图像操作像素的指针
	int	srcPos;

	// 指向复制图像的指针
	unsigned short*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int	cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 循环变量
	long i;
	long j;
	long k;
	long l;

	// 计算结果
	float fResult;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned short[imgSize / 2];
	if( pCpyImgData == NULL )
	{
		return -1;
	}

	memcpy(pCpyImgData,pSrcImgData,imgSize);

	switch(mode)
	{
	case 0:
		//保持边缘像素原样
		//去掉边缘几行//去掉边缘几列
		for(i = Temp.iTempMY; i <imgH - Temp.iTempH + Temp.iTempMY + 1 ; i++)
		{
			for(j = Temp.iTempMX; j <imgW - Temp.iTempW + Temp.iTempMX + 1 ; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos =  imgW * i + j;
				fResult = 0;

				// 计算
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						srcPos =  imgW * (i - Temp.iTempMY + k )
							+ j - Temp.iTempMX + l;
						fResult += pSrcImgData [srcPos] 
						* Temp.fpArray[k * Temp.iTempW + l];

					}
				}
				// 乘上系数
				fResult *= Temp.fCoef;
				fResult = (float) fabs(fResult);

				if(fResult > 65535)
				{
					pCpyImgData[cpyPos]  = 65535;
				}
				else
				{
					pCpyImgData[cpyPos]  = (unsigned short)(fResult+0.5);

				}
			}
		}
		break;
	case 1:
		//在边缘处补零处理边缘像素
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos =  imgW * i + j;
				fResult = 0;

				// 计算
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						if(    i - Temp.iTempMY + k < 0 
							|| j - Temp.iTempMX + l < 0 
							|| i - Temp.iTempMY + k > imgH - 1
							|| j - Temp.iTempMX + l > imgW - 1 )
						{
							pSrcImgData [srcPos] = 0;
						}
						else
						{
							// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
							srcPos =  imgW * (i - Temp.iTempMY + k )
								+ j - Temp.iTempMX + l;
						}
						fResult += pSrcImgData [srcPos]
						* Temp.fpArray[k * Temp.iTempW + l];
					}
				}
				fResult *= Temp.fCoef;
				fResult = (float) fabs(fResult);
				if(fResult > 65535)
				{
					pCpyImgData[cpyPos]  = 65535;
				}
				else
				{
					pCpyImgData[cpyPos]  = (unsigned short) (fResult + 0.5);
				}
			}
		}
		break;
	case 2:

		int L;		//行数
		int C;		//列数

		//在边缘处补对边边缘像素处理	
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos =  imgW * i + j;
				fResult = 0;

				// 计算
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						L = (i - Temp.iTempMY + k );
						C = j - Temp.iTempMX + l;
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						if( i - Temp.iTempMY + k < 0 )
						{
							L = imgH + ( i - Temp.iTempMY + k );
						}
						if( j - Temp.iTempMX + l < 0 )
						{
							C = imgW + ( j - Temp.iTempMX + l );
						}
						if( i - Temp.iTempMY + k > imgH - 1 )
						{
							L = i - Temp.iTempMY + k - imgH;
						}
						if( j - Temp.iTempMX + l > imgW - 1 )
						{
							C = j - Temp.iTempMX + l - imgW;
						}
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						srcPos =  imgW * L
							+ C;
						fResult +=pSrcImgData [srcPos]
						* Temp.fpArray[k * Temp.iTempW + l];
					}
				}
				fResult *= Temp.fCoef;
				fResult = (float) fabs(fResult);
				if(fResult > 65535)
				{
					pCpyImgData[cpyPos]  = 65535;
				}
				else
				{
					pCpyImgData[cpyPos]  = (unsigned short) (fResult + 0.5);
				}
			}
		}
		break;
	default:
		break;
	}

	// 复制变换后的图像
	pListImage->SetData( (unsigned char*)pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;
	return 0;
}




int ConvFilter::TemplateConvSingle(ListImage *pListImage,FilterKernel &Temp,int mode)
{
	// 指向源图像的指针
	unsigned int*	pSrcImgData;
	// 指向源图像操作像素的指针
	int	srcPos;

	// 指向复制图像的指针
	unsigned int*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int	cpyPos	;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 循环变量
	long i;
	long j;
	long k;
	long l;

	// 计算结果
	double fResult;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned int*)pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned int[imgSize / 4];
	if( pCpyImgData == NULL )
	{
		return -1;
	}

	memcpy(pCpyImgData,pSrcImgData,imgSize);		//指定字节长度

	switch(mode)
	{
	case 0:
		//保持边缘像素原样
		//去掉边缘几行//去掉边缘几列
		for(i = Temp.iTempMY; i <imgH - Temp.iTempH + Temp.iTempMY + 1 ; i++)
		{
			for(j = Temp.iTempMX; j <imgW - Temp.iTempW + Temp.iTempMX + 1 ; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos =  imgW * i + j;
				fResult = 0;

				// 计算
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						srcPos =  imgW * (i - Temp.iTempMY + k )
							+ j - Temp.iTempMX + l;
						fResult +=pSrcImgData [srcPos]
						* Temp.fpArray[k * Temp.iTempW + l];

					}
				}
				// 乘上系数
				fResult *= Temp.fCoef;
				fResult = (double) fabs(fResult);

				if(fResult > 4294967295)
				{
					pCpyImgData[cpyPos]  = 4294967295;
				}
				else
				{
					pCpyImgData[cpyPos]  = (unsigned int)(fResult+0.5);

				}
			}
		}
		break;
	case 1:
		//在边缘处补零处理边缘像素
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos =  imgW * i + j;
				fResult = 0;

				// 计算
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						if(    i - Temp.iTempMY + k < 0 
							|| j - Temp.iTempMX + l < 0 
							|| i - Temp.iTempMY + k > imgH - 1
							|| j - Temp.iTempMX + l > imgW - 1 )
						{
							pSrcImgData [srcPos] = 0;
						}
						else
						{
							// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
							srcPos =  imgW * (i - Temp.iTempMY + k )
								+ j - Temp.iTempMX + l;
						}
						fResult +=pSrcImgData [srcPos]
						* Temp.fpArray[k * Temp.iTempW + l];
					}
				}
				fResult *= Temp.fCoef;
				fResult = (double) fabs(fResult);
				if(fResult > 4294967295)
				{
					pCpyImgData[cpyPos]  = 4294967295;
				}
				else
				{
					pCpyImgData[cpyPos]  = (unsigned int) (fResult + 0.5);
				}
			}
		}
		break;
	case 2:

		int L;		//行数
		int C;		//列数

		//在边缘处补对边边缘像素处理	
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos =  imgW * i + j;
				fResult = 0;

				// 计算
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						L = (i - Temp.iTempMY + k );
						C = j - Temp.iTempMX + l;
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						if( i - Temp.iTempMY + k < 0 )
						{
							L = imgH + ( i - Temp.iTempMY + k );
						}
						if( j - Temp.iTempMX + l < 0 )
						{
							C = imgW + ( j - Temp.iTempMX + l );
						}
						if( i - Temp.iTempMY + k > imgH - 1 )
						{
							L = i - Temp.iTempMY + k - imgH;
						}
						if( j - Temp.iTempMX + l > imgW - 1 )
						{
							C = j - Temp.iTempMX + l - imgW;
						}
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						srcPos =  imgW * L
							+ C;
						fResult +=pSrcImgData [srcPos]
						* Temp.fpArray[k * Temp.iTempW + l];
					}
				}
				fResult *= Temp.fCoef;
				fResult = (double) fabs(fResult);
				if(fResult > 4294967295)
				{
					pCpyImgData[cpyPos]  = 4294967295;
				}
				else
				{
					pCpyImgData[cpyPos]  = (unsigned int) (fResult + 0.5);
				}
			}
		}
		break;
	default:
		break;
	}

	// 复制变换后的图像
	pListImage->SetData( (unsigned char*)pCpyImgData, imgSize);
	//pListImage->SetData( (unsigned char*)pSrcImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;
	return 0;
}
int ConvFilter::MedianFilter(ListImage srcImg,ListImage &destImg,int mode)
{
	destImg = srcImg;
	if ( !IsImageIllegal(&destImg ) )
	{
		return -2;
	}
	switch (destImg.GetImgDataType())
	{
	case uint_8:
		return MedianFilterUint_8 (&destImg,mode);
		break;
	case uint_12:
		return MedianFilterUint_12(&destImg,mode);
		break;
	case uint_16:
		return MedianFilterUint_16(&destImg,mode);
		break;
	case single:
		return MedianFilterSingle(&destImg,mode);
		break;
	default:
		return MedianFilterUint_8 (&destImg,mode);
		break;
	}
}


int ConvFilter::MedianFilterUint_8 ( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned char*	pSrcImgData;
	// 指向源图像操作像素的指针
	int	srcPos;

	// 指向复制图像的指针
	unsigned char*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int	cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

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

	//指向滤波器数组的指针
	unsigned char* aValue;

	//循环变量
	long i;
	long j;
	long k;
	long l;

	aValue = new unsigned char[ m_Temp.iTempH * m_Temp.iTempW ];
	if(aValue == NULL)
	{
		return -1;
	}

	//开始中值滤波
	//行（去掉边缘几行）
	for (i = m_Temp.iTempMY; 
		i < imgH-m_Temp.iTempH + m_Temp.iTempMY+1;
		i++)
	{
		//列（去掉边缘几列）
		for(j = m_Temp.iTempMX;
			j < imgW-m_Temp.iTempW+m_Temp.iTempMX+1;
			j++)
		{
			// 指向新第i行，第j个象素的指针
			cpyPos = imgW * i + j;

			//读取滤波器数组
			for (k = 0; k < m_Temp.iTempH; k++)
			{
				for(l = 0; l <m_Temp.iTempW; l++)
				{
					/// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
					srcPos =  imgW * (i - m_Temp.iTempMY + k )
						+ j - m_Temp.iTempMX + l;
					//保存像素值
					aValue[k*m_Temp.iTempW+l] = pSrcImgData[srcPos];
				}
			}

			//获取中值
			pCpyImgData[cpyPos] = GetMedianNum(aValue);
		}
	}

	// 复制变换后的图像
	pListImage->SetData(pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;
	delete []aValue;
	return 0;
}
int ConvFilter::MedianFilterUint_12( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;
	// 指向源图像操作像素的指针
	int	srcPos;

	// 指向复制图像的指针
	unsigned short*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int	cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

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

	//指向滤波器数组的指针
	unsigned short* aValue;

	//循环变量
	long i;
	long j;
	long k;
	long l;

	aValue = new unsigned short[ m_Temp.iTempH * m_Temp.iTempW ];
	if(aValue == NULL)
	{
		return -1;
	}

	switch(mode)
	{
	case 0:
		//开始中值滤波
		//行（去掉边缘几行）
		for (i = m_Temp.iTempMY; 
			i < imgH-m_Temp.iTempH + m_Temp.iTempMY+1;
			i++)
		{
			//列（去掉边缘几列）
			for(j = m_Temp.iTempMX;
				j < imgW-m_Temp.iTempW+m_Temp.iTempMX+1;
				j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos = imgW * i + j;

				//读取滤波器数组
				for (k = 0; k < m_Temp.iTempH; k++)
				{
					for(l = 0; l <m_Temp.iTempW; l++)
					{
						/// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						srcPos =  imgW * (i - m_Temp.iTempMY + k )
							+ j - m_Temp.iTempMX + l;
						//保存像素值
						aValue[k*m_Temp.iTempW+l] =( pSrcImgData[srcPos] & 4095 );
					}
				}

				//获取中值
				pCpyImgData[cpyPos] = GetMedianNum(aValue);
			}
		}
		break;
	case 1:
		//在边缘处补零处理边缘像素
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos = imgW * i + j;


				// 计算
				for (k = 0; k < m_Temp.iTempH; k++)
				{
					for (l = 0; l < m_Temp.iTempW; l++)
					{
						if(    i - m_Temp.iTempMY + k < 0 
							|| j - m_Temp.iTempMX + l < 0 
							|| i - m_Temp.iTempMY + k > imgH - 1
							|| j - m_Temp.iTempMX + l > imgW - 1 )
						{
							pSrcImgData[srcPos] = 0;;
						}
						else
						{
							// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
							srcPos =  imgW * (i - m_Temp.iTempMY + k )
								+ j - m_Temp.iTempMX + l;
						}
						//保存像素值
						aValue[k*m_Temp.iTempW+l] = ( pSrcImgData[srcPos] & 4095 );
					}
				}
				//获取中值
				pCpyImgData[cpyPos] = GetMedianNum(aValue);
			}
		}
		break;
	case 2:

		int L;		//行数
		int C;		//列数

		//在边缘处补对边边缘像素处理	
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos = imgW * i + j;

				// 计算
				for (k = 0; k < m_Temp.iTempH; k++)
				{
					for (l = 0; l < m_Temp.iTempW; l++)
					{
						L = (i - m_Temp.iTempMY + k );
						C = j - m_Temp.iTempMX + l;
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						if( i - m_Temp.iTempMY + k < 0 )
						{
							L = imgH + ( i - m_Temp.iTempMY + k );
						}
						if( j - m_Temp.iTempMX + l < 0 )
						{
							C = imgW + ( j - m_Temp.iTempMX + l );
						}
						if( i - m_Temp.iTempMY + k > imgH - 1 )
						{
							L = i - m_Temp.iTempMY + k - imgH;
						}
						if( j - m_Temp.iTempMX + l > imgW - 1 )
						{
							C = j - m_Temp.iTempMX + l - imgW;
						}
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						srcPos =  imgW * L
							+ C;
						//保存像素值
						aValue[k*m_Temp.iTempW+l] = ( pSrcImgData[srcPos] & 4095 );
					}
				}
				//获取中值
				pCpyImgData[cpyPos] = GetMedianNum(aValue);
			}
		}
		break;
	default:
		break;
	}

	// 复制变换后的图像
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;
	delete []aValue;
	return 0;
}
int ConvFilter::MedianFilterUint_16( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;
	// 指向源图像操作像素的指针
	int	srcPos;

	// 指向复制图像的指针
	unsigned short*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int	cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

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

	//指向滤波器数组的指针
	unsigned short* aValue;

	//循环变量
	long i;
	long j;
	long k;
	long l;

	aValue = new unsigned short[ m_Temp.iTempH * m_Temp.iTempW ];
	if(aValue == NULL)
	{
		return -1;
	}

	switch(mode)
	{
	case 0:
		//开始中值滤波
		//行（去掉边缘几行）
		for (i = m_Temp.iTempMY; 
			i < imgH-m_Temp.iTempH + m_Temp.iTempMY+1;
			i++)
		{
			//列（去掉边缘几列）
			for(j = m_Temp.iTempMX;
				j < imgW-m_Temp.iTempW+m_Temp.iTempMX+1;
				j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos = imgW * i + j;

				//读取滤波器数组
				for (k = 0; k < m_Temp.iTempH; k++)
				{
					for(l = 0; l <m_Temp.iTempW; l++)
					{
						/// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						srcPos =  imgW * (i - m_Temp.iTempMY + k )
							+ j - m_Temp.iTempMX + l;
						//保存像素值
						aValue[k*m_Temp.iTempW+l] = pSrcImgData[srcPos];
					}
				}

				//获取中值
				pCpyImgData[cpyPos] = GetMedianNum(aValue);
			}
		}
		break;
	case 1:
		//在边缘处补零处理边缘像素
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos = imgW * i + j;


				// 计算
				for (k = 0; k < m_Temp.iTempH; k++)
				{
					for (l = 0; l < m_Temp.iTempW; l++)
					{
						if(    i - m_Temp.iTempMY + k < 0 
							|| j - m_Temp.iTempMX + l < 0 
							|| i - m_Temp.iTempMY + k > imgH - 1
							|| j - m_Temp.iTempMX + l > imgW - 1 )
						{
							pSrcImgData[srcPos] = 0;;
						}
						else
						{
							// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
							srcPos =  imgW * (i - m_Temp.iTempMY + k )
								+ j - m_Temp.iTempMX + l;
						}
						//保存像素值
						aValue[k*m_Temp.iTempW+l] = pSrcImgData[srcPos];
					}
				}
				//获取中值
				pCpyImgData[cpyPos] = GetMedianNum(aValue);
			}
		}
		break;
	case 2:

		int L;		//行数
		int C;		//列数

		//在边缘处补对边边缘像素处理	
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// 指向新第i行，第j个象素的指针
				cpyPos = imgW * i + j;

				// 计算
				for (k = 0; k < m_Temp.iTempH; k++)
				{
					for (l = 0; l < m_Temp.iTempW; l++)
					{
						L = (i - m_Temp.iTempMY + k );
						C = j - m_Temp.iTempMX + l;
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						if( i - m_Temp.iTempMY + k < 0 )
						{
							L = imgH + ( i - m_Temp.iTempMY + k );
						}
						if( j - m_Temp.iTempMX + l < 0 )
						{
							C = imgW + ( j - m_Temp.iTempMX + l );
						}
						if( i - m_Temp.iTempMY + k > imgH - 1 )
						{
							L = i - m_Temp.iTempMY + k - imgH;
						}
						if( j - m_Temp.iTempMX + l > imgW - 1 )
						{
							C = j - m_Temp.iTempMX + l - imgW;
						}
						// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						srcPos =  imgW * L
							+ C;
						//保存像素值
						aValue[k*m_Temp.iTempW+l] = pSrcImgData[srcPos];
					}
				}
				//获取中值
				pCpyImgData[cpyPos] = GetMedianNum(aValue);
			}
		}
		break;
	default:
		break;
	}

	// 复制变换后的图像
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;
	delete []aValue;
	return 0;
}
int ConvFilter::MedianFilterSingle( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned int*	pSrcImgData;
	// 指向源图像操作像素的指针
	int	srcPos;

	// 指向复制图像的指针
	unsigned int*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int	cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned int*)pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned int[imgSize/4];
	if( pCpyImgData == NULL )
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	//指向滤波器数组的指针
	unsigned int* aValue;

	//循环变量
	long i;
	long j;
	long k;
	long l;

	aValue = new unsigned int[ m_Temp.iTempH * m_Temp.iTempW ];
	if(aValue == NULL)
	{
		return -1;
	}

	//开始中值滤波
	//行（去掉边缘几行）
	for (i = m_Temp.iTempMY; 
		i < imgH-m_Temp.iTempH + m_Temp.iTempMY+1;
		i++)
	{
		//列（去掉边缘几列）
		for(j = m_Temp.iTempMX;
			j < imgW-m_Temp.iTempW+m_Temp.iTempMX+1;
			j++)
		{
			// 指向新第i行，第j个象素的指针
			cpyPos = imgW * i + j;

			//读取滤波器数组
			for (k = 0; k < m_Temp.iTempH; k++)
			{
				for(l = 0; l <m_Temp.iTempW; l++)
				{
					/// 指向第i - iTempMY + k行，第j - iTempMX + l个象素的指针
					srcPos =  imgW * (i - m_Temp.iTempMY + k )
						+ j - m_Temp.iTempMX + l;
					//保存像素值
					aValue[k*m_Temp.iTempW+l] = pSrcImgData[srcPos];
				}
			}

			//获取中值
			pCpyImgData[cpyPos] = GetMedianNum(aValue);
		}
	}

	// 复制变换后的图像
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;
	delete []aValue;
	return 0;
}
unsigned char ConvFilter::GetMedianNum(unsigned char * Array)
{
	// 循环变量
	int		i;
	int		j;

	// 中间变量
	unsigned char temp;

	//数组长度
	int iFilterLen = m_Temp.iTempH * m_Temp.iTempW;

	// 用冒泡法对数组进行排序
	for (j = 0; j < iFilterLen; j++)
	{
		for (i = iFilterLen-1; i > j ; i--)
		{
			if (Array[i] < Array[i - 1])
			{
				// 互换
				temp = Array[i];
				Array[i] = Array[i - 1];
				Array[i - 1] = temp;
			}
		}
	}

	// 计算中值
	if ((iFilterLen & 1) > 0)
	{
		// 数组有奇数个元素，返回中间一个元素
		temp = Array[(iFilterLen + 1) / 2 - 1];
	}
	else
	{
		// 数组有偶数个元素，返回中间两个元素平均值
		temp = (Array[iFilterLen / 2] + Array[iFilterLen / 2 - 1]) / 2;
	}

	// 返回中值
	return temp;
}

unsigned short ConvFilter::GetMedianNum(unsigned short * Array)
{
	// 循环变量
	int		i;
	int		j;

	// 中间变量
	unsigned short temp;

	//数组长度
	int iFilterLen = m_Temp.iTempH * m_Temp.iTempW;

	// 用冒泡法对数组进行排序
	for (j = 0; j < iFilterLen; j++)
	{
		for (i = iFilterLen-1; i > j ; i--)
		{
			if (Array[i] < Array[i - 1])
			{
				// 互换
				temp = Array[i];
				Array[i] = Array[i - 1];
				Array[i - 1] = temp;
			}
		}
	}

	// 计算中值
	if ((iFilterLen & 1) > 0)
	{
		// 数组有奇数个元素，返回中间一个元素
		temp = Array[(iFilterLen + 1) / 2 - 1];
	}
	else
	{
		// 数组有偶数个元素，返回中间两个元素平均值
		temp = (Array[iFilterLen / 2] + Array[iFilterLen / 2 - 1]) / 2;
	}

	// 返回中值
	return temp;
}
unsigned int ConvFilter::GetMedianNum(unsigned int * Array)
{
	// 循环变量
	int		i;
	int		j;

	// 中间变量
	unsigned int temp;

	//数组长度
	int iFilterLen = m_Temp.iTempH * m_Temp.iTempW;

	// 用冒泡法对数组进行排序
	for (j = 0; j < iFilterLen; j++)
	{
		for (i = iFilterLen-1; i > j ; i--)
		{
			if (Array[i] < Array[i - 1])
			{
				// 互换
				temp = Array[i];
				Array[i] = Array[i - 1];
				Array[i - 1] = temp;
			}
		}
	}

	// 计算中值
	if ((iFilterLen & 1) > 0)
	{
		// 数组有奇数个元素，返回中间一个元素
		temp = Array[(iFilterLen + 1) / 2 - 1];
	}
	else
	{
		// 数组有偶数个元素，返回中间两个元素平均值
		temp = (Array[iFilterLen / 2] + Array[iFilterLen / 2 - 1]) / 2;
	}

	// 返回中值
	return temp;
}
int ConvFilter::GradSharp( ListImage srcImg,ListImage &destImg,unsigned int thre)
{
	destImg = srcImg;
	if ( !IsImageIllegal( &destImg) )
	{
		return -2;
	}
	switch (destImg.GetImgDataType())
	{
	case uint_8:
		return GradSharpUint_8 (&destImg,thre);
		break;
	case uint_12:
		return GradSharpUint_12(&destImg,thre);
		break;
	case uint_16:
		return GradSharpUint_16(&destImg,thre);
		break;
	case single:
		return GradSharpSingle(&destImg,thre);
		break;
	default:
		return GradSharpUint_8 (&destImg,thre);
		break;
	}

}



int ConvFilter::GradSharpUint_8 ( ListImage *pListImage,unsigned char thre )
{
	// 指向源图像数据区的指针
	unsigned char*	pSrcImgData;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 指向源图像的目标位置
	int pos;
	int pos_1;
	int pos_2;

	//初始化
	imgSize = pListImage->GetImgDataSize();
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	pSrcImgData = pListImage->GetImgBuffer(); 

	// 循环变量
	long	i;
	long	j;
	int	temp;

	// 每行(除去边缘一行）
	for(i = 0; i < imgH - 1  ; i++)
	{
		// 每列(除去边缘一列）
		for(j = 0; j < imgW - 1 ; j++)
		{
			pos = imgW * i
				+ j;
			pos_1 = imgW *( i+1) 
				+ j;
			pos_2 = imgW * i 
				+ j + 1;

			//计算梯度的近似值   abs（int） 取绝对值
			temp = abs( (int)pSrcImgData[pos] -  (int)pSrcImgData[pos_1] ) 
				+ abs( (int)pSrcImgData[pos] -  (int)pSrcImgData[pos_2]);

			// 判断是否小于阈值
			if (temp < 255)
			{
				// 判断是否大于阈值，对于小于情况，灰度值不变。
				if (temp >= thre)
				{
					// 直接赋值为temp
					pSrcImgData[pos] = temp;
				}
			}
			else
			{
				// 直接赋值为255
				pSrcImgData[pos] = 255;
			}
		}
	}
	return 0;
}
int ConvFilter::GradSharpUint_12( ListImage *pListImage,unsigned short thre )
{
	// 指向源图像数据区的指针
	unsigned short*	pSrcImgData;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 指向源图像的目标位置
	int pos;
	int pos_1;
	int pos_2;

	//初始化
	imgSize = pListImage->GetImgDataSize();
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 

	// 循环变量
	long	i;
	long	j;
	int	temp;

	// 每行(除去边缘一行）
	for(i = 0; i < imgH - 1  ; i++)
	{
		// 每列(除去边缘一列）
		for(j = 0; j < imgW - 1 ; j++)
		{
			for(j = 0; j < imgW - 1 ; j++)
			{
				pos = imgW * i
					+ j;
				pos_1 = imgW *( i+1) 
					+ j;
				pos_2 = imgW * i 
					+ j + 1;

				//计算梯度的近似值   abs（int） 取绝对值
				temp = abs( (int)pSrcImgData[pos] -  (int)pSrcImgData[pos_1] ) 
					+ abs( (int)pSrcImgData[pos] -  (int)pSrcImgData[pos_2]);

				// 判断是否小于阈值
				if (temp < 4095)
				{
					// 判断是否大于阈值，对于小于情况，灰度值不变。
					if (temp >= thre)
					{
						// 直接赋值为temp
						pSrcImgData[pos] = temp;
					}
				}
				else
				{
					// 直接赋值为4095
					pSrcImgData[pos] = 4095;
				}
			}
		}
	}
	return 0;
}
int ConvFilter::GradSharpUint_16( ListImage *pListImage,unsigned short thre )
{
	// 指向源图像数据区的指针
	unsigned short*	pSrcImgData;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 指向源图像的目标位置
	int pos;
	int pos_1;
	int pos_2;

	//初始化
	imgSize = pListImage->GetImgDataSize();
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 

	// 循环变量
	long	i;
	long	j;
	int	temp;

	// 每行(除去边缘一行）
	for(i = 0; i < imgH - 1  ; i++)
	{
		// 每列(除去边缘一列）
		for(j = 0; j < imgW - 1 ; j++)
		{
			pos = imgW * i
				+ j;
			pos_1 = imgW *( i+1) 
				+ j;
			pos_2 = imgW * i 
				+ j + 1;

			//计算梯度的近似值   abs（int） 取绝对值
			temp = abs( (int)pSrcImgData[pos] -  (int)pSrcImgData[pos_1] ) 
				+ abs( (int)pSrcImgData[pos] -  (int)pSrcImgData[pos_2]);

			// 判断是否小于阈值
			if (temp < 65535)
			{
				// 判断是否大于阈值，对于小于情况，灰度值不变。
				if (temp >= thre)
				{
					// 直接赋值为temp
					pSrcImgData[pos] = temp;
				}
			}
			else
			{
				// 直接赋值为255
				pSrcImgData[pos] = 65535;
			}
		}
	}
	return 0;
}
int ConvFilter::GradSharpSingle( ListImage *pListImage,unsigned int thre )
{
	// 指向源图像数据区的指针
	unsigned int*	pSrcImgData;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	// 指向源图像的目标位置
	int pos;
	int pos_1;
	int pos_2;

	//初始化
	imgSize = pListImage->GetImgDataSize();
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	pSrcImgData = (unsigned int*)pListImage->GetImgBuffer(); 

	// 循环变量
	long	i;
	long	j;
	double 	temp;

	// 每行(除去边缘一行）
	for(i = 0; i < imgH - 1  ; i++)
	{
		// 每列(除去边缘一列）
		for(j = 0; j < imgW - 1 ; j++)
		{
			pos = imgW * i
				+ j;
			pos_1 = imgW *( i+1) 
				+ j;
			pos_2 = imgW * i 
				+ j + 1;

			//计算梯度的近似值   abs（int） 取绝对值
			temp = abs( (double)pSrcImgData[pos] -  (double)pSrcImgData[pos_1] ) 
				+ abs( (double)pSrcImgData[pos] -  (double)pSrcImgData[pos_2]);

			// 判断是否小于阈值
			if (temp < 4294967295)
			{
				// 判断是否大于阈值，对于小于情况，灰度值不变。
				if (temp >= thre)
				{
					// 直接赋值为temp
					pSrcImgData[pos] = temp;
				}
			}
			else
			{
				// 直接赋值为255
				pSrcImgData[pos] = 4294967295;
			}
		}
	}
	return 0;
}
int  ConvFilter::UnsharpMasking( ListImage srcImg,ListImage &destImg,float ratio ,int mode)
{
	destImg = srcImg;
	if ( !IsImageIllegal( &destImg) )
	{
		return -2;
	}

	//临时变量，存储高频分量
	int * copyImg ;
	//临时变量，存储原图
	ListImage t_srcImg ;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = (&destImg)->GetImgHeight();
	imgW = (&destImg)->GetImgWidth();
	imgSize = (&destImg)->GetImgDataSize();
	copyImg = new int[imgH*imgW];
	t_srcImg = srcImg;

	// 第一步：对原图象进行Gauss滤波
	// 3×3高斯模板
	m_Temp.iTempH = 3;
	m_Temp.iTempW  = 3;
	m_Temp.iTempMX = 1;
	m_Temp.iTempMY = 1;
	m_Temp.fCoef = (float) (1.0 / 16.0);
	// 设置模板元素
	m_Temp.fpArray = new float[9];
	m_Temp.fpArray[0] = 1.0;
	m_Temp.fpArray[1] = 2.0;
	m_Temp.fpArray[2] = 1.0;
	m_Temp.fpArray[3] = 2.0;
	m_Temp.fpArray[4] = 4.0;
	m_Temp.fpArray[5] = 2.0;
	m_Temp.fpArray[6] = 1.0;
	m_Temp.fpArray[7] = 2.0;
	m_Temp.fpArray[8] = 1.0;
	TemplateConv((&destImg),m_Temp,mode);

	//第二步，将模糊后的图像反色，并保存在copyImg；
	//第二步，用原图减去平滑后的图像，得到高频部分，存储在copyImg；
	SubImgUint_16(&t_srcImg, &destImg, copyImg,1,1);

	////第三步, 将高频部分与原图相加，得到高频增强的图像
	AddImgUint_16(&t_srcImg, copyImg, &destImg,1,ratio);

	return 0;
}
int ConvFilter::Roberts( ListImage srcImg,ListImage &destImg )
{
	destImg = srcImg;
	if ( !IsImageIllegal( &destImg) )
	{
		return -2;
	}
	switch ((&destImg)->GetImgDataType())
	{
	case uint_8:
		return RobertsUint_8 (&destImg);
		break;
	case uint_12:
		return RobertsUint_12(&destImg);
		break;
	case uint_16:
		return RobertsUint_16(&destImg);
		break;
	case single:
		return RobertsSingle(&destImg);
		break;
	default:
		return RobertsUint_8 (&destImg);
		break;
	}
}


int ConvFilter::Sobel( ListImage srcImg,ListImage &destImg, int mode )
{
	destImg = srcImg;
	if ( !IsImageIllegal( &destImg) )
	{
		return -2;
	}
	switch ((&destImg)->GetImgDataType())
	{
	case uint_8:
		return SobelUint_8 (&destImg,mode);
		break;
	case uint_12:
		return SobelUint_12(&destImg,mode);
		break;
	case uint_16:
		return SobelUint_16(&destImg,mode);
		break;
	case single:
		return SobelSingle(&destImg,mode);
		break;
	default:
		return SobelUint_8 (&destImg,mode);
		break;
	}
}




int ConvFilter::Prewitt( ListImage srcImg,ListImage &destImg, int mode )
{
	destImg = srcImg;
	if ( !IsImageIllegal( &destImg) )
	{
		return -2;
	}
	switch ((&destImg)->GetImgDataType())
	{
	case uint_8:
		return PrewittUint_8 (&destImg,mode);
		break;
	case uint_12:
		return PrewittUint_12(&destImg,mode);
		break;
	case uint_16:
		return PrewittUint_16(&destImg,mode);
		break;
	case single:
		return PrewittSingle(&destImg,mode);
		break;
	default:
		return PrewittUint_8 (&destImg,mode);
		break;
	}
}




int ConvFilter::Kirsch( ListImage srcImg,ListImage &destImg, int mode )
{
	destImg = srcImg;
	if ( !IsImageIllegal( &destImg) )
	{
		return -2;
	}
	switch ((&destImg)->GetImgDataType())
	{
	case uint_8:
		return KirschUint_8 (&destImg,mode);
		break;
	case uint_12:
		return KirschUint_12(&destImg,mode);
		break;
	case uint_16:
		return KirschUint_16(&destImg,mode);
		break;
	case single:
		return KirschSingle(&destImg,mode);
		break;
	default:
		return KirschUint_8 (&destImg,mode);
		break;
	}

}

int ConvFilter::Gauss( ListImage srcImg,ListImage &destImg, int mode )
{
	destImg = srcImg;
	if ( !IsImageIllegal( &destImg) )
	{
		return -2;
	}
	switch ((&destImg)->GetImgDataType())
	{
	case uint_8:
		return GaussUint_8 (&destImg,mode);
		break;
	case uint_12:
		return GaussUint_12(&destImg,mode);
		break;
	case uint_16:
		return GaussUint_16(&destImg,mode);
		break;
	case single:
		return GaussSingle(&destImg,mode);
		break;
	default:
		return GaussUint_8 (&destImg,mode);
		break;
	}

}

int ConvFilter::Canny(  ListImage srcImg,ListImage &destImg,int mode,double dRatioLow,double dRatioHigh)
{
	destImg = srcImg;
	if ( !IsImageIllegal( &destImg) )
	{
		return -2;
	}

	// 指向x,y方向导数的指针
	long * pGradX; 
	long * pGradY;

	// 梯度的幅度
	long * pGradRange;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = (&destImg)->GetImgHeight();
	imgW = (&destImg)->GetImgWidth();
	imgSize = (&destImg)->GetImgDataSize();

	pGradX      = new long [imgH*imgW];
	pGradY      = new long [imgH*imgW];
	pGradRange    = new long [imgH*imgW];

	if ( pGradX == NULL || pGradY == NULL || pGradRange == NULL )
	{
		return -1;
	}

	// 第一步：对原图象进行Gauss滤波
	// 3×3高斯模板
	m_Temp.iTempH = 3;
	m_Temp.iTempW  = 3;
	m_Temp.iTempMX = 1;
	m_Temp.iTempMY = 1;
	m_Temp.fCoef = (float) (1.0 / 16.0);
	// 设置模板元素
	m_Temp.fpArray = new float[9];
	m_Temp.fpArray[0] = 1.0;
	m_Temp.fpArray[1] = 2.0;
	m_Temp.fpArray[2] = 1.0;
	m_Temp.fpArray[3] = 2.0;
	m_Temp.fpArray[4] = 4.0;
	m_Temp.fpArray[5] = 2.0;
	m_Temp.fpArray[6] = 1.0;
	m_Temp.fpArray[7] = 2.0;
	m_Temp.fpArray[8] = 1.0;

	TemplateConv((&destImg),m_Temp,mode);

	// 第二步：对Gauss滤波后的图象计算方向导数


	if( DirGrad((&destImg),pGradX,pGradY) != 0)
	{
		return DirGrad((&destImg),pGradX,pGradY);
	}


	// 第三步：计算梯度的幅度

	if( GradRange((&destImg),pGradX,pGradY,pGradRange) != 0)
	{
		return GradRange((&destImg),pGradX,pGradY,pGradRange);
	}


	// 第四步：应用non-maximum 抑制
	if( NonmaxSuppress((&destImg),pGradX, pGradY,pGradRange) != 0 )
	{
		return NonmaxSuppress((&destImg),pGradX, pGradY,pGradRange) ;
	}

	// 第五步：应用Hysteresis，找到所有的边界
	if( Hysteresis((&destImg),pGradRange,dRatioLow,dRatioHigh)!= 0 )
	{
		return Hysteresis((&destImg),pGradRange,dRatioLow,dRatioHigh);
	}

	//pListImage->SetData(pImgEdge,imgSize);

	// 释放内存
	delete pGradX      ;
	delete pGradY      ;
	delete pGradRange  ;

	return 0;
}

int ConvFilter::RobertsUint_8 ( ListImage *pListImage )
{
	// 指向源图像的指针
	unsigned char*	pSrcImgData;
	// 指向源图像操作像素的指针
	int	srcPos;

	// 指向复制图像的指针
	unsigned char*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int	cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned char[imgSize];
	if (pCpyImgData == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	//像素值
	double result;
	unsigned char pixel[4];

	// 初始化新分配的内存，设定初始值为255
	memset(pCpyImgData, (unsigned char)255, imgSize );


	//使用水平方向的结构元素进行腐蚀
	for(j = imgH -1; j > 0; j--)
	{
		for(i = 0;i <imgW-1; i++)
		{
			//由于使用2×2的模板，为防止越界，所以不处理最下边和最右边的两列像素
			// 指向源图像第j行，第i个象素的指针			
			srcPos =  imgW * j + i;

			// 指向目标图像第j行，第i个象素的指针			
			cpyPos = imgW * j + i;

			//取得当前指针处2*2区域的像素值，注意要转换为unsigned char型
			pixel[0] = (unsigned char)pSrcImgData[srcPos] ;
			pixel[1] = (unsigned char)pSrcImgData[srcPos+1]  ;
			pixel[2] =(unsigned char)pSrcImgData[srcPos - imgW] ;
			pixel[3] =(unsigned char)pSrcImgData[(srcPos - imgW + 1)] ;

			//计算目标图像中的当前点
			result = sqrt((double)( pixel[0] - pixel[3] )*( pixel[0] - pixel[3] ) 
				+ ( pixel[1] - pixel[2] )*( pixel[1] - pixel[2] ));
			pCpyImgData[cpyPos] = (unsigned char)result;

		}
	}

	// 复制腐蚀后的图像
	pListImage->SetData( pCpyImgData,imgSize );

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
}
int ConvFilter::SobelUint_8 ( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned char*	pSrcImgData;

	// 指向复制图像的指针
	unsigned char*	pCpyImgData1;
	// 指向复制图像的指针
	unsigned char*	pCpyImgData2;
	// 指向复制图像操作像素的指针
	int	cpyPos1;
	// 指向复制图像操作像素的指针
	int	cpyPos2;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = pListImage->GetImgBuffer(); 
	pCpyImgData1 = new unsigned char[imgSize];
	if (pCpyImgData1 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData1,pSrcImgData,imgSize);
	pCpyImgData2 = new unsigned char[imgSize];
	if (pCpyImgData2 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData2,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	// 设置Sobel模板参数
	FilterKernel sobelTemp;

	sobelTemp.iTempW = 3;
	sobelTemp.iTempH = 3;
	sobelTemp.fCoef = 1.0;
	sobelTemp.iTempMX = 1;
	sobelTemp.iTempMY = 1;
	sobelTemp.fpArray = new float[9];
	sobelTemp.fpArray[0] = -1.0;
	sobelTemp.fpArray[1] = -2.0;
	sobelTemp.fpArray[2] = -1.0;
	sobelTemp.fpArray[3] = 0.0;
	sobelTemp.fpArray[4] = 0.0;
	sobelTemp.fpArray[5] = 0.0;
	sobelTemp.fpArray[6] = 1.0;
	sobelTemp.fpArray[7] = 2.0;
	sobelTemp.fpArray[8] = 1.0;

	//构造ListImage
	ListImage listTemp(imgW, imgH);
	listTemp.SetData(pCpyImgData1,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// 设置Sobel模板参数
	sobelTemp.fpArray[0] = -1.0;
	sobelTemp.fpArray[1] = 0.0;
	sobelTemp.fpArray[2] = 1.0;
	sobelTemp.fpArray[3] = -2.0;
	sobelTemp.fpArray[4] = 0.0;
	sobelTemp.fpArray[5] = 2.0;
	sobelTemp.fpArray[6] = -1.0;
	sobelTemp.fpArray[7] = 0.0;
	sobelTemp.fpArray[8] = 1.0;

	//构造ListImage
	listTemp.SetData(pCpyImgData2,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// 复制经过模板运算后的图像到源图像
	pListImage->SetData(pCpyImgData1,imgSize);


	// 释放内存
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// 返回
	return 0;
}
int ConvFilter::PrewittUint_8 ( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned char*	pSrcImgData;

	// 指向复制图像的指针
	unsigned char*	pCpyImgData1;
	// 指向复制图像的指针
	unsigned char*	pCpyImgData2;
	// 指向复制图像操作像素的指针
	int	cpyPos1;
	// 指向复制图像操作像素的指针
	int	cpyPos2;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = pListImage->GetImgBuffer(); 
	pCpyImgData1 = new unsigned char[imgSize];
	if (pCpyImgData1 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData1,pSrcImgData,imgSize);
	pCpyImgData2 = new unsigned char[imgSize];
	if (pCpyImgData2 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData2,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	// 设置Prewitt模板参数
	FilterKernel PrewittTemp;
	PrewittTemp.iTempW = 3;
	PrewittTemp.iTempH = 3;
	PrewittTemp.fCoef = 1.0;
	PrewittTemp.iTempMX = 1;
	PrewittTemp.iTempMY = 1;
	PrewittTemp.fpArray = new float[9];
	PrewittTemp.fpArray[0] = -1.0;
	PrewittTemp.fpArray[1] = -1.0;
	PrewittTemp.fpArray[2] = -1.0;
	PrewittTemp.fpArray[3] = 0.0;
	PrewittTemp.fpArray[4] = 0.0;
	PrewittTemp.fpArray[5] = 0.0;
	PrewittTemp.fpArray[6] = 1.0;
	PrewittTemp.fpArray[7] = 1.0;
	PrewittTemp.fpArray[8] = 1.0;

	//构造ListImage
	ListImage listTemp(imgW, imgH);
	listTemp.SetData(pCpyImgData1,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,PrewittTemp) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// 设置Prewitt模板参数
	PrewittTemp.fpArray[0] = 1.0;
	PrewittTemp.fpArray[1] = 0.0;
	PrewittTemp.fpArray[2] = -1.0;
	PrewittTemp.fpArray[3] = 1.0;
	PrewittTemp.fpArray[4] = 0.0;
	PrewittTemp.fpArray[5] = -1.0;
	PrewittTemp.fpArray[6] = 1.0;
	PrewittTemp.fpArray[7] = 0.0;
	PrewittTemp.fpArray[8] = -1.0;

	//构造ListImage
	listTemp.SetData(pCpyImgData2,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,PrewittTemp) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// 复制经过模板运算后的图像到源图像

	pListImage->SetData(pCpyImgData1,imgSize);
	// 释放内存
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// 返回
	return 0;
}
int ConvFilter::KirschUint_8 ( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned char*	pSrcImgData;
	// 指向复制图像的指针
	unsigned char*	pCpyImgData1;
	// 指向复制图像的指针
	unsigned char*	pCpyImgData2;
	// 指向复制图像操作像素的指针
	int	cpyPos1;
	// 指向复制图像操作像素的指针
	int	cpyPos2;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = pListImage->GetImgBuffer(); 
	pCpyImgData1 = new unsigned char[imgSize];
	if (pCpyImgData1 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData1,pSrcImgData,imgSize);
	pCpyImgData2 = new unsigned char[imgSize];
	if (pCpyImgData2 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData2,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	// 设置Kirsch模板1参数
	m_Temp.fpArray = new float[9];
	m_Temp.iTempW = 3;
	m_Temp.iTempH = 3;
	m_Temp.fCoef = 1.0;
	m_Temp.iTempMX = 1;
	m_Temp.iTempMY = 1;
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH);
	pListNew->SetData(pCpyImgData1,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0  )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData1,pListNew->GetImgBuffer(),imgSize);

	// 设置Kirsch模板2参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;


	// 调用TemplateConvConv()函数
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板3参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = 5.0;


	// 调用TemplateConvConv()函数
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];	

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2,pSrcImgData, imgSize);

	// 设置Kirsch模板4参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// 调用TemplateConvConv()函数
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板5参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// 调用TemplateConvConv()函数
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);
	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板6参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板7参数
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板8参数
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 复制经过模板运算后的图像到源图像
	pListImage->SetData(pCpyImgData1, imgSize);


	// 释放内存
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// 返回
	return 0;
}
int ConvFilter::GaussUint_8 ( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned char*	pSrcImgData;
	// 指向复制图像的指针
	unsigned char*	pCpyImgData;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

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

	// 设置Gauss模板参数
	m_Temp.iTempW = 5;
	m_Temp.iTempH = 5;
	m_Temp.fCoef = 1.0;
	m_Temp.iTempMX = 3;
	m_Temp.iTempMY = 3;
	m_Temp.fpArray = new float[25];
	m_Temp.fpArray[0] = -2.0;
	m_Temp.fpArray[1] = -4.0;
	m_Temp.fpArray[2] = -4.0;
	m_Temp.fpArray[3] = -4.0;
	m_Temp.fpArray[4] = -2.0;
	m_Temp.fpArray[5] = -4.0;
	m_Temp.fpArray[6] = 0.0;
	m_Temp.fpArray[7] = 8.0;
	m_Temp.fpArray[8] = 0.0;
	m_Temp.fpArray[9] = -4.0;
	m_Temp.fpArray[10] = -4.0;
	m_Temp.fpArray[11] = 8.0;
	m_Temp.fpArray[12] = 24.0;
	m_Temp.fpArray[13] = 8.0;
	m_Temp.fpArray[14] = -4.0;
	m_Temp.fpArray[15] = -4.0;
	m_Temp.fpArray[16] = 0.0;
	m_Temp.fpArray[17] = 8.0;
	m_Temp.fpArray[18] = 0.0;
	m_Temp.fpArray[19] = -4.0;
	m_Temp.fpArray[20] = -2.0;
	m_Temp.fpArray[21] = -4.0;
	m_Temp.fpArray[22] = -4.0;
	m_Temp.fpArray[23] = -4.0;
	m_Temp.fpArray[24] = -2.0;

	// 调用TemplateConv()函数
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH);
	pListNew->SetData(pCpyImgData,pListImage->GetImgDataSize());

	if ( TemplateConv(pListNew, m_Temp) != 0 )
	{
		return TemplateConv(pListNew, m_Temp);
	}
	pCpyImgData = pListNew->GetImgBuffer();

	// 复制经过模板运算后的图像到源图像
	pListImage->SetData(pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;
	return 0;
}
int ConvFilter::RobertsUint_12( ListImage *pListImage )
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;
	// 指向源图像操作像素的指针
	int	srcPos;

	// 指向复制图像的指针
	unsigned short*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int	cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned short[imgSize/2];
	if (pCpyImgData == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	//像素值
	double result;
	unsigned short pixel[4];

	// 初始化新分配的内存，设定初始值为255
	memset(pCpyImgData, (unsigned short)4095, imgSize );


	//使用水平方向的结构元素进行腐蚀
	for(j = imgH -1; j > 0; j--)
	{
		for(i = 0;i <imgW-1; i++)
		{
			//由于使用2×2的模板，为防止越界，所以不处理最下边和最右边的两列像素

			// 指向源图像第j行，第i个象素的指针			
			srcPos =  imgW * j + i;

			// 指向目标图像第j行，第i个象素的指针			
			cpyPos = imgW * j + i;

			//取得当前指针处2*2区域的像素值，注意要转换为unsigned short型
			pixel[0] = (unsigned short)pSrcImgData[srcPos]  & 4095;
			pixel[1] = (unsigned short)pSrcImgData[srcPos+1]  & 4095;
			pixel[2] = (unsigned short)pSrcImgData[srcPos - imgW]  & 4095;
			pixel[3] = (unsigned short)pSrcImgData[(srcPos - imgW + 1)] & 4095;

			//计算目标图像中的当前点
			result = sqrt((double)( pixel[0] - pixel[3] )*( pixel[0] - pixel[3] ) 
				+ ( pixel[1] - pixel[2] )*( pixel[1] - pixel[2] ));
			pCpyImgData[cpyPos] = (unsigned short)result;

		}
	}

	// 复制腐蚀后的图像
	pListImage->SetData( (unsigned char*)pCpyImgData,imgSize );

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
}
int ConvFilter::SobelUint_12( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;

	// 指向复制图像的指针
	unsigned short*	pCpyImgData1;
	// 指向复制图像的指针
	unsigned short*	pCpyImgData2;
	// 指向复制图像操作像素的指针
	int	cpyPos1;
	// 指向复制图像操作像素的指针
	int	cpyPos2;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 
	pCpyImgData1 = new unsigned short[imgSize/2];
	if (pCpyImgData1 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData1,pSrcImgData,imgSize);
	pCpyImgData2 = new unsigned short[imgSize/2];
	if (pCpyImgData2 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData2,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	// 设置Sobel模板参数
	FilterKernel sobelTemp;

	sobelTemp.iTempW = 3;
	sobelTemp.iTempH = 3;
	sobelTemp.fCoef = 1.0;
	sobelTemp.iTempMX = 1;
	sobelTemp.iTempMY = 1;
	sobelTemp.fpArray = new float[9];
	sobelTemp.fpArray[0] = -1.0;
	sobelTemp.fpArray[1] = -2.0;
	sobelTemp.fpArray[2] = -1.0;
	sobelTemp.fpArray[3] = 0.0;
	sobelTemp.fpArray[4] = 0.0;
	sobelTemp.fpArray[5] = 0.0;
	sobelTemp.fpArray[6] = 1.0;
	sobelTemp.fpArray[7] = 2.0;
	sobelTemp.fpArray[8] = 1.0;

	//构造ListImage
	ListImage listTemp(imgW, imgH,1,uint_16);
	listTemp.SetData((unsigned char*)pCpyImgData1,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// 设置Sobel模板参数
	sobelTemp.fpArray[0] = -1.0;
	sobelTemp.fpArray[1] = 0.0;
	sobelTemp.fpArray[2] = 1.0;
	sobelTemp.fpArray[3] = -2.0;
	sobelTemp.fpArray[4] = 0.0;
	sobelTemp.fpArray[5] = 2.0;
	sobelTemp.fpArray[6] = -1.0;
	sobelTemp.fpArray[7] = 0.0;
	sobelTemp.fpArray[8] = 1.0;

	//构造ListImage
	listTemp.SetData((unsigned char*)pCpyImgData2,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
			{
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
			}

		}
	}

	// 复制经过模板运算后的图像到源图像
	pListImage->SetData((unsigned char*)pCpyImgData1,imgSize);


	// 释放内存
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// 返回
	return 0;
}
int ConvFilter::PrewittUint_12( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;

	// 指向复制图像的指针
	unsigned short*	pCpyImgData1;
	// 指向复制图像的指针
	unsigned short*	pCpyImgData2;
	// 指向复制图像操作像素的指针
	int	cpyPos1;
	// 指向复制图像操作像素的指针
	int	cpyPos2;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 
	pCpyImgData1 = new unsigned short[imgSize/2];
	if (pCpyImgData1 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData1,pSrcImgData,imgSize);
	pCpyImgData2 = new unsigned short[imgSize/2];
	if (pCpyImgData2 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData2,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	// 设置Prewitt模板参数
	FilterKernel PrewittTemp;
	PrewittTemp.iTempW = 3;
	PrewittTemp.iTempH = 3;
	PrewittTemp.fCoef = 1.0;
	PrewittTemp.iTempMX = 1;
	PrewittTemp.iTempMY = 1;
	PrewittTemp.fpArray = new float[9];
	PrewittTemp.fpArray[0] = -1.0;
	PrewittTemp.fpArray[1] = -1.0;
	PrewittTemp.fpArray[2] = -1.0;
	PrewittTemp.fpArray[3] = 0.0;
	PrewittTemp.fpArray[4] = 0.0;
	PrewittTemp.fpArray[5] = 0.0;
	PrewittTemp.fpArray[6] = 1.0;
	PrewittTemp.fpArray[7] = 1.0;
	PrewittTemp.fpArray[8] = 1.0;

	//构造ListImage
	ListImage listTemp(imgW, imgH,1,uint_12);
	listTemp.SetData((unsigned char*)pCpyImgData1,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,PrewittTemp,mode) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// 设置Prewitt模板参数
	PrewittTemp.fpArray[0] = 1.0;
	PrewittTemp.fpArray[1] = 0.0;
	PrewittTemp.fpArray[2] = -1.0;
	PrewittTemp.fpArray[3] = 1.0;
	PrewittTemp.fpArray[4] = 0.0;
	PrewittTemp.fpArray[5] = -1.0;
	PrewittTemp.fpArray[6] = 1.0;
	PrewittTemp.fpArray[7] = 0.0;
	PrewittTemp.fpArray[8] = -1.0;

	//构造ListImage
	listTemp.SetData((unsigned char*)pCpyImgData2,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,PrewittTemp,mode) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// 复制经过模板运算后的图像到源图像

	pListImage->SetData((unsigned char*)pCpyImgData1,imgSize);
	// 释放内存
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// 返回
	return 0;
}
int ConvFilter::KirschUint_12( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;
	// 指向复制图像的指针
	unsigned short*	pCpyImgData1;
	// 指向复制图像的指针
	unsigned short*	pCpyImgData2;
	// 指向复制图像操作像素的指针
	int	cpyPos1;
	// 指向复制图像操作像素的指针
	int	cpyPos2;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 
	pCpyImgData1 = new unsigned short[imgSize/sizeof(unsigned short)];
	if (pCpyImgData1 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData1,pSrcImgData,imgSize);
	pCpyImgData2 = new unsigned short[imgSize/2];
	if (pCpyImgData2 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData2,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	// 设置Kirsch模板1参数
	m_Temp.fpArray = new float[9];
	m_Temp.iTempW = 3;
	m_Temp.iTempH = 3;
	m_Temp.fCoef = 1.0;
	m_Temp.iTempMX = 1;
	m_Temp.iTempMY = 1;
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH,1,uint_12);
	pListNew->SetData((unsigned char*)pCpyImgData1,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0  )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData1,pListNew->GetImgBuffer(),imgSize);

	// 设置Kirsch模板2参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;


	// 调用TemplateConvConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板3参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = 5.0;


	// 调用TemplateConvConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];	

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2,pSrcImgData, imgSize);

	// 设置Kirsch模板4参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// 调用TemplateConvConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板5参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// 调用TemplateConvConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);
	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板6参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板7参数
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板8参数
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 复制经过模板运算后的图像到源图像
	pListImage->SetData((unsigned char*)pCpyImgData1, imgSize);


	// 释放内存
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// 返回
	return 0;
}
int ConvFilter::GaussUint_12( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;
	// 指向复制图像的指针
	unsigned short*	pCpyImgData;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

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

	// 设置Gauss模板参数
	m_Temp.iTempW = 5;
	m_Temp.iTempH = 5;
	m_Temp.fCoef = 1.0;
	m_Temp.iTempMX = 3;
	m_Temp.iTempMY = 3;
	m_Temp.fpArray = new float[25];
	m_Temp.fpArray[0] = -2.0;
	m_Temp.fpArray[1] = -4.0;
	m_Temp.fpArray[2] = -4.0;
	m_Temp.fpArray[3] = -4.0;
	m_Temp.fpArray[4] = -2.0;
	m_Temp.fpArray[5] = -4.0;
	m_Temp.fpArray[6] = 0.0;
	m_Temp.fpArray[7] = 8.0;
	m_Temp.fpArray[8] = 0.0;
	m_Temp.fpArray[9] = -4.0;
	m_Temp.fpArray[10] = -4.0;
	m_Temp.fpArray[11] = 8.0;
	m_Temp.fpArray[12] = 24.0;
	m_Temp.fpArray[13] = 8.0;
	m_Temp.fpArray[14] = -4.0;
	m_Temp.fpArray[15] = -4.0;
	m_Temp.fpArray[16] = 0.0;
	m_Temp.fpArray[17] = 8.0;
	m_Temp.fpArray[18] = 0.0;
	m_Temp.fpArray[19] = -4.0;
	m_Temp.fpArray[20] = -2.0;
	m_Temp.fpArray[21] = -4.0;
	m_Temp.fpArray[22] = -4.0;
	m_Temp.fpArray[23] = -4.0;
	m_Temp.fpArray[24] = -2.0;

	// 调用TemplateConv()函数
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH,1,uint_12);
	pListNew->SetData((unsigned char*)pCpyImgData,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData,pListNew->GetImgBuffer(),imgSize);

	// 复制经过模板运算后的图像到源图像
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;
	delete pListNew;
	return 0;
}
int ConvFilter::RobertsUint_16( ListImage *pListImage)
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;
	// 指向源图像操作像素的指针
	int	srcPos;

	// 指向复制图像的指针
	unsigned short*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int	cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned short[imgSize/2];
	if (pCpyImgData == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	//像素值
	double result;
	unsigned short pixel[4];

	// 初始化新分配的内存，设定初始值为255
	memset(pCpyImgData, (unsigned short)4095, imgSize );


	//使用水平方向的结构元素进行腐蚀
	for(j = imgH -1; j > 0; j--)
	{
		for(i = 0;i <imgW-1; i++)
		{
			//由于使用2×2的模板，为防止越界，所以不处理最下边和最右边的两列像素

			// 指向源图像第j行，第i个象素的指针			
			srcPos =  imgW * j + i;

			// 指向目标图像第j行，第i个象素的指针			
			cpyPos = imgW * j + i;

			//取得当前指针处2*2区域的像素值，注意要转换为unsigned short型
			pixel[0] =(unsigned short)pSrcImgData[srcPos] ;
			pixel[1] = (unsigned short)pSrcImgData[srcPos+1]  ;
			pixel[2] =(unsigned short)pSrcImgData[srcPos - imgW] ;
			pixel[3] =(unsigned short)pSrcImgData[(srcPos - imgW + 1)] ;

			//计算目标图像中的当前点
			result = sqrt((double)( pixel[0] - pixel[3] )*( pixel[0] - pixel[3] ) 
				+ ( pixel[1] - pixel[2] )*( pixel[1] - pixel[2] ));
			pCpyImgData[cpyPos] = (unsigned short)result;

		}
	}

	// 复制腐蚀后的图像
	pListImage->SetData( (unsigned char*)pCpyImgData,imgSize );

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
}
int ConvFilter::SobelUint_16( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;

	// 指向复制图像的指针
	unsigned short*	pCpyImgData1;
	// 指向复制图像的指针
	unsigned short*	pCpyImgData2;
	// 指向复制图像操作像素的指针
	int	cpyPos1;
	// 指向复制图像操作像素的指针
	int	cpyPos2;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 
	pCpyImgData1 = new unsigned short[imgSize/2];
	if (pCpyImgData1 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData1,pSrcImgData,imgSize);
	pCpyImgData2 = new unsigned short[imgSize/2];
	if (pCpyImgData2 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData2,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	// 设置Sobel模板参数
	FilterKernel sobelTemp;

	sobelTemp.iTempW = 3;
	sobelTemp.iTempH = 3;
	sobelTemp.fCoef = 1.0;
	sobelTemp.iTempMX = 1;
	sobelTemp.iTempMY = 1;
	sobelTemp.fpArray = new float[9];
	sobelTemp.fpArray[0] = -1.0;
	sobelTemp.fpArray[1] = -2.0;
	sobelTemp.fpArray[2] = -1.0;
	sobelTemp.fpArray[3] = 0.0;
	sobelTemp.fpArray[4] = 0.0;
	sobelTemp.fpArray[5] = 0.0;
	sobelTemp.fpArray[6] = 1.0;
	sobelTemp.fpArray[7] = 2.0;
	sobelTemp.fpArray[8] = 1.0;

	//构造ListImage
	ListImage listTemp(imgW, imgH,1,uint_16);
	listTemp.SetData((unsigned char*)pCpyImgData1,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// 设置Sobel模板参数
	sobelTemp.fpArray[0] = -1.0;
	sobelTemp.fpArray[1] = 0.0;
	sobelTemp.fpArray[2] = 1.0;
	sobelTemp.fpArray[3] = -2.0;
	sobelTemp.fpArray[4] = 0.0;
	sobelTemp.fpArray[5] = 2.0;
	sobelTemp.fpArray[6] = -1.0;
	sobelTemp.fpArray[7] = 0.0;
	sobelTemp.fpArray[8] = 1.0;

	//构造ListImage
	listTemp.SetData((unsigned char*)pCpyImgData2,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
			{
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
			}

		}
	}

	// 复制经过模板运算后的图像到源图像
	pListImage->SetData((unsigned char*)pCpyImgData1,imgSize);


	// 释放内存
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// 返回
	return 0;
}
int ConvFilter::PrewittUint_16( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;

	// 指向复制图像的指针
	unsigned short*	pCpyImgData1;
	// 指向复制图像的指针
	unsigned short*	pCpyImgData2;
	// 指向复制图像操作像素的指针
	int	cpyPos1;
	// 指向复制图像操作像素的指针
	int	cpyPos2;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 
	pCpyImgData1 = new unsigned short[imgSize/2];
	if (pCpyImgData1 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData1,pSrcImgData,imgSize);
	pCpyImgData2 = new unsigned short[imgSize/2];
	if (pCpyImgData2 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData2,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	// 设置Prewitt模板参数
	FilterKernel PrewittTemp;
	PrewittTemp.iTempW = 3;
	PrewittTemp.iTempH = 3;
	PrewittTemp.fCoef = 1.0;
	PrewittTemp.iTempMX = 1;
	PrewittTemp.iTempMY = 1;
	PrewittTemp.fpArray = new float[9];
	PrewittTemp.fpArray[0] = -1.0;
	PrewittTemp.fpArray[1] = -1.0;
	PrewittTemp.fpArray[2] = -1.0;
	PrewittTemp.fpArray[3] = 0.0;
	PrewittTemp.fpArray[4] = 0.0;
	PrewittTemp.fpArray[5] = 0.0;
	PrewittTemp.fpArray[6] = 1.0;
	PrewittTemp.fpArray[7] = 1.0;
	PrewittTemp.fpArray[8] = 1.0;

	//构造ListImage
	ListImage listTemp(imgW, imgH,1,uint_16);
	listTemp.SetData((unsigned char*)pCpyImgData1,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,PrewittTemp,mode) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// 设置Prewitt模板参数
	PrewittTemp.fpArray[0] = 1.0;
	PrewittTemp.fpArray[1] = 0.0;
	PrewittTemp.fpArray[2] = -1.0;
	PrewittTemp.fpArray[3] = 1.0;
	PrewittTemp.fpArray[4] = 0.0;
	PrewittTemp.fpArray[5] = -1.0;
	PrewittTemp.fpArray[6] = 1.0;
	PrewittTemp.fpArray[7] = 0.0;
	PrewittTemp.fpArray[8] = -1.0;

	//构造ListImage
	listTemp.SetData((unsigned char*)pCpyImgData2,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,PrewittTemp,mode) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// 复制经过模板运算后的图像到源图像

	pListImage->SetData((unsigned char*)pCpyImgData1,imgSize);
	// 释放内存
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// 返回
	return 0;
}
int ConvFilter::KirschUint_16( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;
	// 指向复制图像的指针
	unsigned short*	pCpyImgData1;
	// 指向复制图像的指针
	unsigned short*	pCpyImgData2;
	// 指向复制图像操作像素的指针
	int	cpyPos1;
	// 指向复制图像操作像素的指针
	int	cpyPos2;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 
	pCpyImgData1 = new unsigned short[imgSize/2];
	if (pCpyImgData1 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData1,pSrcImgData,imgSize);
	pCpyImgData2 = new unsigned short[imgSize/2];
	if (pCpyImgData2 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData2,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	// 设置Kirsch模板1参数
	m_Temp.fpArray = new float[9];
	m_Temp.iTempW = 3;
	m_Temp.iTempH = 3;
	m_Temp.fCoef = 1.0;
	m_Temp.iTempMX = 1;
	m_Temp.iTempMY = 1;
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH,1,uint_16);
	pListNew->SetData((unsigned char*)pCpyImgData1,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0  )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData1,pListNew->GetImgBuffer(),imgSize);

	// 设置Kirsch模板2参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;


	// 调用TemplateConvConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板3参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = 5.0;


	// 调用TemplateConvConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];	

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2,pSrcImgData, imgSize);

	// 设置Kirsch模板4参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// 调用TemplateConvConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板5参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// 调用TemplateConvConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);
	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板6参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板7参数
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板8参数
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 复制经过模板运算后的图像到源图像
	pListImage->SetData((unsigned char*)pCpyImgData1, imgSize);


	// 释放内存
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// 返回
	return 0;
}
int ConvFilter::GaussUint_16( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned short*	pSrcImgData;
	// 指向复制图像的指针
	unsigned short*	pCpyImgData;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

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

	// 设置Gauss模板参数
	m_Temp.iTempW = 5;
	m_Temp.iTempH = 5;
	m_Temp.fCoef = 1.0;
	m_Temp.iTempMX = 3;
	m_Temp.iTempMY = 3;
	m_Temp.fpArray = new float[25];
	m_Temp.fpArray[0] = -2.0;
	m_Temp.fpArray[1] = -4.0;
	m_Temp.fpArray[2] = -4.0;
	m_Temp.fpArray[3] = -4.0;
	m_Temp.fpArray[4] = -2.0;
	m_Temp.fpArray[5] = -4.0;
	m_Temp.fpArray[6] = 0.0;
	m_Temp.fpArray[7] = 8.0;
	m_Temp.fpArray[8] = 0.0;
	m_Temp.fpArray[9] = -4.0;
	m_Temp.fpArray[10] = -4.0;
	m_Temp.fpArray[11] = 8.0;
	m_Temp.fpArray[12] = 24.0;
	m_Temp.fpArray[13] = 8.0;
	m_Temp.fpArray[14] = -4.0;
	m_Temp.fpArray[15] = -4.0;
	m_Temp.fpArray[16] = 0.0;
	m_Temp.fpArray[17] = 8.0;
	m_Temp.fpArray[18] = 0.0;
	m_Temp.fpArray[19] = -4.0;
	m_Temp.fpArray[20] = -2.0;
	m_Temp.fpArray[21] = -4.0;
	m_Temp.fpArray[22] = -4.0;
	m_Temp.fpArray[23] = -4.0;
	m_Temp.fpArray[24] = -2.0;

	// 调用TemplateConv()函数
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH,1,uint_16);
	pListNew->SetData((unsigned char*)pCpyImgData,pListImage->GetImgDataSize());

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData,pListNew->GetImgBuffer(),imgSize);

	// 复制经过模板运算后的图像到源图像
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;
	return 0;
}


int ConvFilter::RobertsSingle( ListImage *pListImage )
{
	// 指向源图像的指针
	unsigned int*	pSrcImgData;
	// 指向源图像操作像素的指针
	int	srcPos;

	// 指向复制图像的指针
	unsigned int*	pCpyImgData;
	// 指向复制图像操作像素的指针
	int	cpyPos;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned int*)pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned int[imgSize/4];
	if (pCpyImgData == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	//像素值
	double result;
	double pixel[4];

	// 初始化新分配的内存，设定初始值为255
	memset(pCpyImgData, (unsigned int)4095, imgSize );


	//使用水平方向的结构元素进行腐蚀
	for(j = imgH -1; j > 0; j--)
	{
		for(i = 0;i <imgW-1; i++)
		{
			//由于使用2×2的模板，为防止越界，所以不处理最下边和最右边的两列像素

			// 指向源图像第j行，第i个象素的指针			
			srcPos =  imgW * j + i;

			// 指向目标图像第j行，第i个象素的指针			
			cpyPos = imgW * j + i;

			//取得当前指针处2*2区域的像素值，注意要转换为unsigned int型
			pixel[0] = (double)pSrcImgData[srcPos] ;
			pixel[1] = (double)pSrcImgData[srcPos+1]  ;
			pixel[2] = (double)pSrcImgData[srcPos - imgW] ;
			pixel[3] = (double)pSrcImgData[(srcPos - imgW + 1)] ;

			//计算目标图像中的当前点
			result = sqrt(( pixel[0] - pixel[3] )*( pixel[0] - pixel[3] ) 
				+ ( pixel[1] - pixel[2] )*( pixel[1] - pixel[2] ));
			pCpyImgData[cpyPos] = (unsigned int)result;

		}
	}

	// 复制腐蚀后的图像
	pListImage->SetData( (unsigned char*)pCpyImgData,imgSize );

	// 释放内存
	delete []pCpyImgData;

	// 返回
	return 0;
}
int ConvFilter::SobelSingle( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned int*	pSrcImgData;

	// 指向复制图像的指针
	unsigned int*	pCpyImgData1;
	// 指向复制图像的指针
	unsigned int*	pCpyImgData2;
	// 指向复制图像操作像素的指针
	int	cpyPos1;
	// 指向复制图像操作像素的指针
	int	cpyPos2;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned int*)pListImage->GetImgBuffer(); 
	pCpyImgData1 = new unsigned int[imgSize/4];
	if (pCpyImgData1 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData1,pSrcImgData,imgSize);
	pCpyImgData2 = new unsigned int[imgSize/4];
	if (pCpyImgData2 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData2,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	// 设置Sobel模板参数
	FilterKernel sobelTemp;

	sobelTemp.iTempW = 3;
	sobelTemp.iTempH = 3;
	sobelTemp.fCoef = 1.0;
	sobelTemp.iTempMX = 1;
	sobelTemp.iTempMY = 1;
	sobelTemp.fpArray = new float[9];
	sobelTemp.fpArray[0] = -1.0;
	sobelTemp.fpArray[1] = -2.0;
	sobelTemp.fpArray[2] = -1.0;
	sobelTemp.fpArray[3] = 0.0;
	sobelTemp.fpArray[4] = 0.0;
	sobelTemp.fpArray[5] = 0.0;
	sobelTemp.fpArray[6] = 1.0;
	sobelTemp.fpArray[7] = 2.0;
	sobelTemp.fpArray[8] = 1.0;

	//构造ListImage
	ListImage listTemp(imgW, imgH,1,single);
	listTemp.SetData((unsigned char*)pCpyImgData1,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// 设置Sobel模板参数
	sobelTemp.fpArray[0] = -1.0;
	sobelTemp.fpArray[1] = 0.0;
	sobelTemp.fpArray[2] = 1.0;
	sobelTemp.fpArray[3] = -2.0;
	sobelTemp.fpArray[4] = 0.0;
	sobelTemp.fpArray[5] = 2.0;
	sobelTemp.fpArray[6] = -1.0;
	sobelTemp.fpArray[7] = 0.0;
	sobelTemp.fpArray[8] = 1.0;

	//构造ListImage
	listTemp.SetData((unsigned char*)pCpyImgData2,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
			{
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
			}
		}
	}

	// 复制经过模板运算后的图像到源图像
	pListImage->SetData((unsigned char*)pCpyImgData1,imgSize);


	// 释放内存
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// 返回
	return 0;
}
int ConvFilter::PrewittSingle( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned int*	pSrcImgData;

	// 指向复制图像的指针
	unsigned int*	pCpyImgData1;
	// 指向复制图像的指针
	unsigned int*	pCpyImgData2;
	// 指向复制图像操作像素的指针
	int	cpyPos1;
	// 指向复制图像操作像素的指针
	int	cpyPos2;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned int*)pListImage->GetImgBuffer(); 
	pCpyImgData1 = new unsigned int[imgSize/4];
	if (pCpyImgData1 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData1,pSrcImgData,imgSize);
	pCpyImgData2 = new unsigned int[imgSize/4];
	if (pCpyImgData2 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData2,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	// 设置Prewitt模板参数
	FilterKernel PrewittTemp;
	PrewittTemp.iTempW = 3;
	PrewittTemp.iTempH = 3;
	PrewittTemp.fCoef = 1.0;
	PrewittTemp.iTempMX = 1;
	PrewittTemp.iTempMY = 1;
	PrewittTemp.fpArray = new float[9];
	PrewittTemp.fpArray[0] = -1.0;
	PrewittTemp.fpArray[1] = -1.0;
	PrewittTemp.fpArray[2] = -1.0;
	PrewittTemp.fpArray[3] = 0.0;
	PrewittTemp.fpArray[4] = 0.0;
	PrewittTemp.fpArray[5] = 0.0;
	PrewittTemp.fpArray[6] = 1.0;
	PrewittTemp.fpArray[7] = 1.0;
	PrewittTemp.fpArray[8] = 1.0;

	//构造ListImage
	ListImage listTemp(imgW, imgH,1,single);
	listTemp.SetData((unsigned char*)pCpyImgData1,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,PrewittTemp,mode) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// 设置Prewitt模板参数
	PrewittTemp.fpArray[0] = 1.0;
	PrewittTemp.fpArray[1] = 0.0;
	PrewittTemp.fpArray[2] = -1.0;
	PrewittTemp.fpArray[3] = 1.0;
	PrewittTemp.fpArray[4] = 0.0;
	PrewittTemp.fpArray[5] = -1.0;
	PrewittTemp.fpArray[6] = 1.0;
	PrewittTemp.fpArray[7] = 0.0;
	PrewittTemp.fpArray[8] = -1.0;

	//构造ListImage
	listTemp.SetData((unsigned char*)pCpyImgData2,imgSize);

	// 调用Template()函数
	if ( TemplateConv(&listTemp,PrewittTemp,mode) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// 复制经过模板运算后的图像到源图像

	pListImage->SetData((unsigned char*)pCpyImgData1,imgSize);
	// 释放内存
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// 返回
	return 0;
}
int ConvFilter::KirschSingle( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned int*	pSrcImgData;
	// 指向复制图像的指针
	unsigned int*	pCpyImgData1;
	// 指向复制图像的指针
	unsigned int*	pCpyImgData2;
	// 指向复制图像操作像素的指针
	int	cpyPos1;
	// 指向复制图像操作像素的指针
	int	cpyPos2;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned int*)pListImage->GetImgBuffer(); 
	pCpyImgData1 = new unsigned int[imgSize/4];
	if (pCpyImgData1 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData1,pSrcImgData,imgSize);
	pCpyImgData2 = new unsigned int[imgSize/4];
	if (pCpyImgData2 == NULL)
	{
		return -1;
	}
	memcpy(pCpyImgData2,pSrcImgData,imgSize);

	//循环变量
	long i;
	long j;

	// 设置Kirsch模板1参数
	m_Temp.fpArray = new float[9];
	m_Temp.iTempW = 3;
	m_Temp.iTempH = 3;
	m_Temp.fCoef = 1.0;
	m_Temp.iTempMX = 1;
	m_Temp.iTempMY = 1;
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH,1,single);
	pListNew->SetData((unsigned char*)pCpyImgData1,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0  )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData1,pListNew->GetImgBuffer(),imgSize);

	// 设置Kirsch模板2参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;


	// 调用TemplateConvConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板3参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = 5.0;


	// 调用TemplateConvConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];	

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2,pSrcImgData, imgSize);

	// 设置Kirsch模板4参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// 调用TemplateConvConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板5参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// 调用TemplateConvConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);
	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板6参数
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板7参数
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// 设置Kirsch模板8参数
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// 调用TemplateConv()函数
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//求两幅缓存图像的最大值
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// 指向缓存图像1倒数第j行，第i个象素的指针			
			cpyPos1 = imgW * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// 复制经过模板运算后的图像到源图像
	pListImage->SetData((unsigned char*)pCpyImgData1, imgSize);


	// 释放内存
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// 返回
	return 0;
}
int ConvFilter::GaussSingle( ListImage *pListImage, int mode )
{
	// 指向源图像的指针
	unsigned int*	pSrcImgData;
	// 指向复制图像的指针
	unsigned int*	pCpyImgData;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned int*)pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned int[imgSize/4];
	if( pCpyImgData == NULL )
	{
		return -1;
	}
	memcpy(pCpyImgData,pSrcImgData,imgSize);

	// 设置Gauss模板参数
	m_Temp.iTempW = 5;
	m_Temp.iTempH = 5;
	m_Temp.fCoef = 1.0;
	m_Temp.iTempMX = 3;
	m_Temp.iTempMY = 3;
	m_Temp.fpArray = new float[25];
	m_Temp.fpArray[0] = -2.0;
	m_Temp.fpArray[1] = -4.0;
	m_Temp.fpArray[2] = -4.0;
	m_Temp.fpArray[3] = -4.0;
	m_Temp.fpArray[4] = -2.0;
	m_Temp.fpArray[5] = -4.0;
	m_Temp.fpArray[6] = 0.0;
	m_Temp.fpArray[7] = 8.0;
	m_Temp.fpArray[8] = 0.0;
	m_Temp.fpArray[9] = -4.0;
	m_Temp.fpArray[10] = -4.0;
	m_Temp.fpArray[11] = 8.0;
	m_Temp.fpArray[12] = 24.0;
	m_Temp.fpArray[13] = 8.0;
	m_Temp.fpArray[14] = -4.0;
	m_Temp.fpArray[15] = -4.0;
	m_Temp.fpArray[16] = 0.0;
	m_Temp.fpArray[17] = 8.0;
	m_Temp.fpArray[18] = 0.0;
	m_Temp.fpArray[19] = -4.0;
	m_Temp.fpArray[20] = -2.0;
	m_Temp.fpArray[21] = -4.0;
	m_Temp.fpArray[22] = -4.0;
	m_Temp.fpArray[23] = -4.0;
	m_Temp.fpArray[24] = -2.0;

	// 调用TemplateConv()函数
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH,1,single);
	pListNew->SetData((unsigned char*)pCpyImgData,pListImage->GetImgDataSize());

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData,pListNew->GetImgBuffer(),imgSize);

	// 复制经过模板运算后的图像到源图像
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// 释放内存
	delete []pCpyImgData;
	return 0;
}

int ConvFilter::DirGrad(ListImage *pListImage, long *pGradX , long *pGradY)
{
	switch(pListImage->GetImgDataType())
	{
	case uint_8:
		return DirGradUint_8(pListImage, pGradX , pGradY);
		break;
	case uint_12:
		return DirGradUint_12(pListImage, pGradX , pGradY);
		break;

	case uint_16:
		return DirGradUint_16(pListImage, pGradX , pGradY);
		break;
	case single:
		return DirGradSingle(pListImage, pGradX , pGradY);
		break;
	default:
		return DirGradUint_8(pListImage, pGradX , pGradY);
	}
}
int ConvFilter::DirGradUint_8(ListImage *pListImage, long *pGradX , long *pGradY)
{
	//临时变量
	int i;
	int j;
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	unsigned char* pSrcImgData = (unsigned char*)pListImage->GetImgBuffer();
	// 计算x方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{

			pGradX[i*imgW+j] = (long) ( pSrcImgData[i*imgW+min(imgW-1,j+1)]  
			- pSrcImgData[i*imgW+max(0,j-1)]     );
		}
	}
	// 计算y方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(j=0; j<imgW; j++)
	{
		for(i=0; i<imgH; i++)
		{
			pGradY[i*imgW+j] = (long) ( pSrcImgData[min(imgH-1,i+1)*imgW + j]  
			- pSrcImgData[max(0,i-1)*imgW+ j ]   );
		}
	}
	return 0;
}
int ConvFilter::DirGradUint_12(ListImage *pListImage, long *pGradX , long *pGradY)
{
	//临时变量
	int i;
	int j;
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	unsigned short* pSrcImgData = (unsigned short*)pListImage->GetImgBuffer();
	// 计算x方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{

			pGradX[i*imgW+j] = (long) ( pSrcImgData[i*imgW+min(imgW-1,j+1)]  
			- pSrcImgData[i*imgW+max(0,j-1)]     );
		}
	}
	// 计算y方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(j=0; j<imgW; j++)
	{
		for(i=0; i<imgH; i++)
		{
			pGradY[i*imgW+j] = (long) ( pSrcImgData[min(imgH-1,i+1)*imgW + j]  
			- pSrcImgData[max(0,i-1)*imgW+ j ]   );
		}
	}
	return 0;
}


int ConvFilter::DirGradUint_16(ListImage *pListImage, long *pGradX , long *pGradY)
{
	//临时变量
	int i;
	int j;
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	unsigned short* pSrcImgData = (unsigned short*)pListImage->GetImgBuffer();
	// 计算x方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{

			pGradX[i*imgW+j] = (long) ( pSrcImgData[i*imgW+min(imgW-1,j+1)]  
			- pSrcImgData[i*imgW+max(0,j-1)]     );
		}
	}
	// 计算y方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(j=0; j<imgW; j++)
	{
		for(i=0; i<imgH; i++)
		{
			pGradY[i*imgW+j] = (long) ( pSrcImgData[min(imgH-1,i+1)*imgW + j]  
			- pSrcImgData[max(0,i-1)*imgW+ j ]   );
		}
	}
	return 0;
}
int ConvFilter::DirGradSingle(ListImage *pListImage, long *pGradX , long *pGradY)
{
	//临时变量
	int i;
	int j;
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	unsigned int* pSrcImgData = (unsigned int*)pListImage->GetImgBuffer();
	// 计算x方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{

			pGradX[i*imgW+j] = (long) ( pSrcImgData[i*imgW+min(imgW-1,j+1)]  
			- pSrcImgData[i*imgW+max(0,j-1)]     );
			int x = pGradX[i*imgW+j];
		}
	}
	// 计算y方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(j=0; j<imgW; j++)
	{
		for(i=0; i<imgH; i++)
		{
			pGradY[i*imgW+j] = (long) ( pSrcImgData[min(imgH-1,i+1)*imgW + j]  
			- pSrcImgData[max(0,i-1)*imgW+ j ]   );
		}
	}
	return 0;
}
int ConvFilter::GradRange(ListImage *pListImage,long *pGradX, long *pGradY, long *pGradRange)
{
	// 中间变量
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	long double dSquareX;
	long double dSquareY;
	int i;
	int j;
	if(pListImage->GetImgDataType() == single)
	{
		for(i=0; i<imgH; i++)
		{
			for(j=0; j<imgW; j++)
			{
				dSquareX = (pGradX[i*imgW + j]/65536) * (pGradX[i*imgW + j]/65536);
				dSquareY = (pGradY[i*imgW + j]/65536) * (pGradY[i*imgW + j]/65536);
				pGradRange[i*imgW + j] = (long)(sqrt(dSquareX + dSquareY) + 0.5) * 65536;
			}
		}
	}
	else
	{
		for(i=0; i<imgH; i++)
		{
			for(j=0; j<imgW; j++)
			{
				dSquareX = pGradX[i*imgW + j] * pGradX[i*imgW + j];
				dSquareY = pGradY[i*imgW + j] * pGradY[i*imgW + j];
				pGradRange[i*imgW + j] = (long)(sqrt(dSquareX + dSquareY) + 0.5);
			}
		}
	}

	return 0;

}
int ConvFilter::NonmaxSuppress(ListImage *pListImage,long *pGradX, long *pGradY,long *pGradRange)
{
	switch(pListImage->GetImgDataType())
	{
	case uint_8:
		return NonmaxSuppressUint_8(pListImage,pGradX, pGradY,pGradRange);
		break;
	case uint_12:
		return NonmaxSuppressUint_12(pListImage,pGradX, pGradY,pGradRange);
		break;
	case uint_16:
		return NonmaxSuppressUint_16(pListImage,pGradX, pGradY,pGradRange);
		break;
	case single:
		return NonmaxSuppressSingle(pListImage,pGradX, pGradY,pGradRange);
		break;
	default:
		return NonmaxSuppressUint_8(pListImage,pGradX, pGradY,pGradRange);
	}


}
int ConvFilter::NonmaxSuppressUint_8(ListImage *pListImage,long *pGradX, long *pGradY,long *pGradRange)
{
	unsigned char*	pImgEdge = (unsigned char*)pListImage->GetImgBuffer();
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();

	// 循环控制变量
	int pos;

	// x方向梯度分量
	long gx  ;
	long gy  ;

	// 临时变量
	long g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;

	int i;
	int j;
	// 设置图象边缘部分为不可能的边界点
	for(j=0; j<imgW; j++)		
	{
		pImgEdge[j] = 0 ;
		pImgEdge[imgH-1+j] = 0;
	}
	for(i=0; i<imgH; i++)		
	{
		pImgEdge[i*imgW] = 0 ;
		pImgEdge[i*imgW + imgW-1] = 0;
	}

	for(i=1; i<imgH-1; i++)
	{
		for(j=1; j<imgW-1; j++)
		{
			pos = i*imgW + j ;

			// 如果当前象素的梯度幅度为0，则不是边界点
			if(pGradRange[pos] == 0 )
			{
				pImgEdge[pos] = 0 ;
			}
			else
			{
				// 当前象素的梯度幅度
				dTmp = pGradRange[pos] ;

				// x，y方向导数
				gx = pGradX[pos]  ;
				gy = pGradY[pos]  ;

				// 如果方向导数y分量比x分量大，说明导数的方向更加“趋向”于y分量。
				if (abs(gy) > abs(gx)) 
				{
					// 计算插值的比例
					weight = fabs((double)gx)/fabs((double)gy); 

					g2 = pGradRange[pos-imgW] ; 
					g4 = pGradRange[pos+imgW] ;

					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pGradRange[pos-imgW-1] ;
						g3 = pGradRange[pos+imgW+1] ;
					} 

					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	   g2 g1
					//		C         
					//	g3 g4  
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					} 
				}

				// 如果方向导数x分量比y分量大，说明导数的方向更加“趋向”于x分量
				// 这个判断语句包含了x分量和y分量相等的情况
				else
				{
					// 计算插值的比例
					weight = fabs((double)gy)/fabs((double)gx);  

					g2 = pGradRange[pos+1] ; 
					g4 = pGradRange[pos-1] ;

					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pGradRange[pos+imgW+1] ;
						g3 = pGradRange[pos-imgW-1] ;
					} 
					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					}
				}

				// 下面利用g1-g4对梯度进行插值
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;

					// 当前象素的梯度是局部的最大值
					// 该点可能是个边界点
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pImgEdge[pos] = 128 ;
					}
					else
					{
						// 不可能是边界点
						pImgEdge[pos] = 0 ;
					}
				}
			} //else
		} // for
	}
	pListImage->SetData(pImgEdge,imgSize);
	return 0;
}
int ConvFilter::NonmaxSuppressUint_12(ListImage *pListImage,long *pGradX, long *pGradY,long *pGradRange)
{
	unsigned short*	pImgEdge = (unsigned short*)pListImage->GetImgBuffer();
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();

	// 循环控制变量
	int pos;

	// x方向梯度分量
	long gx  ;
	long gy  ;

	// 临时变量
	long g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;

	int i;
	int j;
	// 设置图象边缘部分为不可能的边界点
	for(j=0; j<imgW; j++)		
	{
		pImgEdge[j] = 0 ;
		pImgEdge[imgH-1+j] = 0;
	}
	for(i=0; i<imgH; i++)		
	{
		pImgEdge[i*imgW] = 0 ;
		pImgEdge[i*imgW + imgW-1] = 0;
	}

	for(i=1; i<imgH-1; i++)
	{
		for(j=1; j<imgW-1; j++)
		{
			pos = i*imgW + j ;

			// 如果当前象素的梯度幅度为0，则不是边界点
			if(pGradRange[pos] == 0 )
			{
				pImgEdge[pos] = 0 ;
			}
			else
			{
				// 当前象素的梯度幅度
				dTmp = pGradRange[pos] ;

				// x，y方向导数
				gx = pGradX[pos]  ;
				gy = pGradY[pos]  ;

				// 如果方向导数y分量比x分量大，说明导数的方向更加“趋向”于y分量。
				if (abs(gy) > abs(gx)) 
				{
					// 计算插值的比例
					weight = fabs((double)gx)/fabs((double)gy); 

					g2 = pGradRange[pos-imgW] ; 
					g4 = pGradRange[pos+imgW] ;

					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pGradRange[pos-imgW-1] ;
						g3 = pGradRange[pos+imgW+1] ;
					} 

					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	   g2 g1
					//		C         
					//	g3 g4  
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					} 
				}

				// 如果方向导数x分量比y分量大，说明导数的方向更加“趋向”于x分量
				// 这个判断语句包含了x分量和y分量相等的情况
				else
				{
					// 计算插值的比例
					weight = fabs((double)gy)/fabs((double)gx);  

					g2 = pGradRange[pos+1] ; 
					g4 = pGradRange[pos-1] ;

					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pGradRange[pos+imgW+1] ;
						g3 = pGradRange[pos-imgW-1] ;
					} 
					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					}
				}

				// 下面利用g1-g4对梯度进行插值
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;

					// 当前象素的梯度是局部的最大值
					// 该点可能是个边界点
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pImgEdge[pos] = 128 ;
					}
					else
					{
						// 不可能是边界点
						pImgEdge[pos] = 0 ;
					}
				}
			} //else
		} // for
	}
	pListImage->SetData((unsigned char*)pImgEdge,imgSize);
	return 0;
}
int ConvFilter::NonmaxSuppressUint_16(ListImage *pListImage,long *pGradX, long *pGradY,long *pGradRange)
{
	unsigned short*	pImgEdge = (unsigned short*)pListImage->GetImgBuffer();
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();

	// 循环控制变量
	int pos;

	// x方向梯度分量
	long gx  ;
	long gy  ;

	// 临时变量
	long g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;

	int i;
	int j;
	// 设置图象边缘部分为不可能的边界点
	for(j=0; j<imgW; j++)		
	{
		pImgEdge[j] = 0 ;
		pImgEdge[imgH-1+j] = 0;
	}
	for(i=0; i<imgH; i++)		
	{
		pImgEdge[i*imgW] = 0 ;
		pImgEdge[i*imgW + imgW-1] = 0;
	}

	for(i=1; i<imgH-1; i++)
	{
		for(j=1; j<imgW-1; j++)
		{
			pos = i*imgW + j ;

			// 如果当前象素的梯度幅度为0，则不是边界点
			if(pGradRange[pos] == 0 )
			{
				pImgEdge[pos] = 0 ;
			}
			else
			{
				// 当前象素的梯度幅度
				dTmp = pGradRange[pos] ;

				// x，y方向导数
				gx = pGradX[pos]  ;
				gy = pGradY[pos]  ;

				// 如果方向导数y分量比x分量大，说明导数的方向更加“趋向”于y分量。
				if (abs(gy) > abs(gx)) 
				{
					// 计算插值的比例
					weight = fabs((double)gx)/fabs((double)gy); 

					g2 = pGradRange[pos-imgW] ; 
					g4 = pGradRange[pos+imgW] ;

					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pGradRange[pos-imgW-1] ;
						g3 = pGradRange[pos+imgW+1] ;
					} 

					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	   g2 g1
					//		C         
					//	g3 g4  
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					} 
				}

				// 如果方向导数x分量比y分量大，说明导数的方向更加“趋向”于x分量
				// 这个判断语句包含了x分量和y分量相等的情况
				else
				{
					// 计算插值的比例
					weight = fabs((double)gy)/fabs((double)gx);  

					g2 = pGradRange[pos+1] ; 
					g4 = pGradRange[pos-1] ;

					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pGradRange[pos+imgW+1] ;
						g3 = pGradRange[pos-imgW-1] ;
					} 
					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					}
				}

				// 下面利用g1-g4对梯度进行插值
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;

					// 当前象素的梯度是局部的最大值
					// 该点可能是个边界点
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pImgEdge[pos] = 32768 ;
					}
					else
					{
						// 不可能是边界点
						pImgEdge[pos] = 0 ;
					}
				}
			} //else
		} // for
	}
	pListImage->SetData((unsigned char*)pImgEdge,imgSize);
	return 0;
}
int ConvFilter::NonmaxSuppressSingle(ListImage *pListImage,long *pGradX, long *pGradY,long *pGradRange)
{
	unsigned int*	pImgEdge = (unsigned int*)pListImage->GetImgBuffer();
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();

	// 循环控制变量
	int pos;

	// x方向梯度分量
	long gx  ;
	long gy  ;

	// 临时变量
	long g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;

	int i;
	int j;
	// 设置图象边缘部分为不可能的边界点
	for(j=0; j<imgW; j++)		
	{
		pImgEdge[j] = 0 ;
		pImgEdge[imgH-1+j] = 0;
	}
	for(i=0; i<imgH; i++)		
	{
		pImgEdge[i*imgW] = 0 ;
		pImgEdge[i*imgW + imgW-1] = 0;
	}

	for(i=1; i<imgH-1; i++)
	{
		for(j=1; j<imgW-1; j++)
		{
			pos = i*imgW + j ;

			// 如果当前象素的梯度幅度为0，则不是边界点
			if(pGradRange[pos] == 0 )
			{
				pImgEdge[pos] = 0 ;
			}
			else
			{
				// 当前象素的梯度幅度
				dTmp = pGradRange[pos] ;

				// x，y方向导数
				gx = pGradX[pos]  ;
				gy = pGradY[pos]  ;

				// 如果方向导数y分量比x分量大，说明导数的方向更加“趋向”于y分量。
				if (abs(gy) > abs(gx)) 
				{
					// 计算插值的比例
					weight = fabs((double)gx)/fabs((double)gy); 

					g2 = pGradRange[pos-imgW] ; 
					g4 = pGradRange[pos+imgW] ;

					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pGradRange[pos-imgW-1] ;
						g3 = pGradRange[pos+imgW+1] ;
					} 

					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	   g2 g1
					//		C         
					//	g3 g4  
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					} 
				}

				// 如果方向导数x分量比y分量大，说明导数的方向更加“趋向”于x分量
				// 这个判断语句包含了x分量和y分量相等的情况
				else
				{
					// 计算插值的比例
					weight = fabs((double)gy)/fabs((double)gx);  

					g2 = pGradRange[pos+1] ; 
					g4 = pGradRange[pos-1] ;

					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pGradRange[pos+imgW+1] ;
						g3 = pGradRange[pos-imgW-1] ;
					} 
					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					}
				}

				// 下面利用g1-g4对梯度进行插值
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;

					// 当前象素的梯度是局部的最大值
					// 该点可能是个边界点
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pImgEdge[pos] = 2147483648 ;
					}
					else
					{
						// 不可能是边界点
						pImgEdge[pos] = 0 ;
					}
				}
			} //else
		} // for
	}
	pListImage->SetData((unsigned char*)pImgEdge,imgSize);
	return 0;
}

int ConvFilter::Hysteresis(ListImage *pListImage,
						   long *pGradRange, double dRatioLow ,double dRatioHigh )
{
	switch(pListImage->GetImgDataType())
	{
	case uint_8:
		return HysteresisUint_8(pListImage,pGradRange,dRatioLow,dRatioHigh );
		break;
	case uint_12:
		return HysteresisUint_12(pListImage,pGradRange,dRatioLow,dRatioHigh );
		break;
	case uint_16:
		return HysteresisUint_16(pListImage,pGradRange,dRatioLow,dRatioHigh );
		break;
	case single:
		return HysteresisSingle(pListImage,pGradRange,dRatioLow,dRatioHigh );
		break;
	default:
		return HysteresisUint_8(pListImage,pGradRange,dRatioLow,dRatioHigh );
	}
}
int ConvFilter::HysteresisUint_8(ListImage *pListImage,
								 long *pGradRange, double dRatioLow ,double dRatioHigh )
{
	unsigned char*	pImgEdge = (unsigned char*)pListImage->GetImgBuffer();

	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();

	int nThdHigh ;
	int nThdLow  ;

	int pos;
	int i ;
	int j ;

	// 估计TraceEdge需要的低阈值，以及Hysteresis函数使用的高阈值
	EstimateThreshold(pListImage,&nThdHigh, &nThdLow,dRatioHigh, dRatioLow,pGradRange);

	// 这个循环用来寻找大于nThdHigh的点，这些点被用来当作边界点，然后用
	// TraceEdge函数来跟踪该点对应的边界
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 

			// 如果该象素是可能的边界点，并且梯度大于高阈值，该象素作为
			// 一个边界的起点
			if((pImgEdge[pos] == 128) && (pGradRange[pos] >= nThdHigh))
			{
				// 设置该点为边界点
				pImgEdge[pos] = 255;
				TraceEdge(pListImage,i, j, nThdLow, pGradRange);
			}
		}
	}

	// 那些还没有被设置为边界点的象素已经不可能成为边界点
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 
			if(pImgEdge[pos] != 255)
			{
				// 设置为非边界点
				pImgEdge[pos] = 0;
			}
		}
	}
	pListImage->SetData(pImgEdge,imgSize);
	return 0;
}
int ConvFilter::HysteresisUint_12(ListImage *pListImage,
								  long *pGradRange, double dRatioLow ,double dRatioHigh )
{
	unsigned short*	pImgEdge = (unsigned short*)pListImage->GetImgBuffer();

	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();

	int nThdHigh ;
	int nThdLow  ;

	int pos;
	int i ;
	int j ;

	// 估计TraceEdge需要的低阈值，以及Hysteresis函数使用的高阈值
	EstimateThreshold(pListImage,&nThdHigh, &nThdLow,dRatioHigh, dRatioLow,pGradRange);


	// 这个循环用来寻找大于nThdHigh的点，这些点被用来当作边界点，然后用
	// TraceEdge函数来跟踪该点对应的边界
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 

			// 如果该象素是可能的边界点，并且梯度大于高阈值，该象素作为
			// 一个边界的起点
			if((pImgEdge[pos] == 128) && (pGradRange[pos] >= nThdHigh))
			{
				// 设置该点为边界点
				pImgEdge[pos] = 4095;
				TraceEdge(pListImage,i, j, nThdLow, pGradRange);
			}
		}
	}

	// 那些还没有被设置为边界点的象素已经不可能成为边界点
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 
			if(pImgEdge[pos] != 4095)
			{
				// 设置为非边界点
				pImgEdge[pos] = 0;
			}
		}
	}
	pListImage->SetData((unsigned char*)pImgEdge,imgSize);
	return 0;
}
int ConvFilter::HysteresisUint_16(ListImage *pListImage,
								  long *pGradRange, double dRatioLow ,double dRatioHigh )
{
	unsigned short*	pImgEdge = (unsigned short*)pListImage->GetImgBuffer();

	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();

	int nThdHigh ;
	int nThdLow  ;

	int pos;
	int i ;
	int j ;

	// 估计TraceEdge需要的低阈值，以及Hysteresis函数使用的高阈值
	EstimateThreshold(pListImage,&nThdHigh, &nThdLow,dRatioHigh, dRatioLow,pGradRange);


	// 这个循环用来寻找大于nThdHigh的点，这些点被用来当作边界点，然后用
	// TraceEdge函数来跟踪该点对应的边界
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 

			// 如果该象素是可能的边界点，并且梯度大于高阈值，该象素作为
			// 一个边界的起点
			if((pImgEdge[pos] == 32768) && (pGradRange[pos] >= nThdHigh))
			{
				// 设置该点为边界点
				pImgEdge[pos] = 65535;
				TraceEdge(pListImage,i, j, nThdLow, pGradRange);
			}
		}
	}

	// 那些还没有被设置为边界点的象素已经不可能成为边界点
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 
			if(pImgEdge[pos] != 65535)
			{
				// 设置为非边界点
				pImgEdge[pos] = 0;
			}
		}
	}
	pListImage->SetData((unsigned char*)pImgEdge,imgSize);
	return 0;
}
int ConvFilter::HysteresisSingle(ListImage *pListImage,
								 long *pGradRange, double dRatioLow ,double dRatioHigh )
{
	unsigned int*	pImgEdge = (unsigned int*)pListImage->GetImgBuffer();

	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();

	int nThdHigh ;
	int nThdLow  ;

	int pos;
	int i ;
	int j ;

	// 估计TraceEdge需要的低阈值，以及Hysteresis函数使用的高阈值
	EstimateThreshold(pListImage,&nThdHigh, &nThdLow,dRatioHigh, dRatioLow,pGradRange);


	// 这个循环用来寻找大于nThdHigh的点，这些点被用来当作边界点，然后用
	// TraceEdge函数来跟踪该点对应的边界
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 

			// 如果该象素是可能的边界点，并且梯度大于高阈值，该象素作为
			// 一个边界的起点
			if((pImgEdge[pos] == 2147483648) && (pGradRange[pos] >= nThdHigh))
			{
				// 设置该点为边界点
				pImgEdge[pos] = 4294967295;
				//TraceEdge(pListImage,i, j, nThdLow, pGradRange);
			}
		}
	}

	// 那些还没有被设置为边界点的象素已经不可能成为边界点
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 
			if(pImgEdge[pos] != 4294967295)
			{
				// 设置为非边界点
				pImgEdge[pos] = 0;
			}
		}
	}
	pListImage->SetData((unsigned char*)pImgEdge,imgSize);
	return 0;
}
int ConvFilter::EstimateThreshold(ListImage *pListImage, int *pThdHigh,int *pThdLow, 
								  double dRatioHigh, double dRatioLow,long* pGradRange) 
{ 

	switch(pListImage->GetImgDataType())
	{
	case uint_8:
		return EstimateThresholdUint_8(pListImage, pThdHigh,pThdLow,
			dRatioHigh, dRatioLow, pGradRange);
		break;
	case uint_12:
		return EstimateThresholdUint_12(pListImage, pThdHigh,pThdLow,
			dRatioHigh, dRatioLow, pGradRange);
		break;
	case uint_16:
		return EstimateThresholdUint_16(pListImage, pThdHigh,pThdLow,
			dRatioHigh, dRatioLow, pGradRange);
		break;
	case single:
		return EstimateThresholdSingle(pListImage, pThdHigh,pThdLow,
			dRatioHigh, dRatioLow, pGradRange);
		break;
	default:
		return EstimateThresholdUint_8(pListImage, pThdHigh,pThdLow,
			dRatioHigh, dRatioLow, pGradRange);
	}


}
int ConvFilter::EstimateThresholdUint_8(ListImage *pListImage, int *pThdHigh,int *pThdLow, 
										double dRatioHigh, double dRatioLow,long* pGradRange)
{
	unsigned char*	pImgEdge = (unsigned char*)pListImage->GetImgBuffer();

	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();
	// 循环控制变量
	int y;
	int x;
	int k;


	int nHist[1024] ;

	// 可能的边界数目
	int nEdgeNb     ;

	// 最大梯度值
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 

	// 初始化
	for(k=0; k<1024; k++) 
	{
		nHist[k] = 0; 
	}

	// 统计直方图，然后利用直方图计算阈值
	for(y=0; y<imgH; y++)
	{
		for(x=0; x<imgW; x++)
		{
			// 只是统计那些可能是边界点，并且还没有处理过的象素
			if(pImgEdge[y*imgW+x]==128)
			{
				nHist[ pGradRange[y*imgW+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// 统计经过“非最大值抑止(non-maximum suppression)”后有多少象素
	for(k=1; k<1024; k++)
	{
		if(nHist[k] != 0)
		{
			// 最大梯度值
			nMaxMag = k;
		}

		// 梯度为0的点是不可能为边界点的
		// 经过non-maximum suppression后有多少象素
		nEdgeNb += nHist[k];
	}

	// 梯度比高阈值*pThdHigh小的象素点总数目
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);

	k = 1;
	nEdgeNb = nHist[1];

	// 计算高阈值
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// 设置高阈值
	*pThdHigh = k ;

	// 设置低阈值
	*pThdLow  = (int)((*pThdHigh) * dRatioLow+ 0.5);
	return 0;
}
int ConvFilter::EstimateThresholdUint_12(ListImage *pListImage, int *pThdHigh,int *pThdLow, 
										 double dRatioHigh, double dRatioLow,long* pGradRange)
{
	unsigned short*	pImgEdge = (unsigned short*)pListImage->GetImgBuffer();

	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();
	// 循环控制变量
	int y;
	int x;
	int k;


	int nHist[100000] ;

	// 可能的边界数目
	int nEdgeNb     ;

	// 最大梯度值
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 

	// 初始化
	for(k=0; k<100000; k++) 
	{
		nHist[k] = 0; 
	}

	// 统计直方图，然后利用直方图计算阈值
	for(y=0; y<imgH; y++)
	{
		for(x=0; x<imgW; x++)
		{
			// 只是统计那些可能是边界点，并且还没有处理过的象素
			if(pImgEdge[y*imgW+x]==128)
			{
				nHist[ pGradRange[y*imgW+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// 统计经过“非最大值抑止(non-maximum suppression)”后有多少象素
	for(k=1; k<100000; k++)
	{
		if(nHist[k] != 0)
		{
			// 最大梯度值
			nMaxMag = k;
		}

		// 梯度为0的点是不可能为边界点的
		// 经过non-maximum suppression后有多少象素
		nEdgeNb += nHist[k];
	}

	// 梯度比高阈值*pThdHigh小的象素点总数目
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);

	k = 1;
	nEdgeNb = nHist[1];

	// 计算高阈值
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// 设置高阈值
	*pThdHigh = k ;

	// 设置低阈值
	*pThdLow  = (int)((*pThdHigh) * dRatioLow+ 0.5);
	return 0;
}
int ConvFilter::EstimateThresholdUint_16(ListImage *pListImage, int *pThdHigh,int *pThdLow, 
										 double dRatioHigh, double dRatioLow,long* pGradRange)
{
	unsigned short*	pImgEdge = (unsigned short*)pListImage->GetImgBuffer();

	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();
	// 循环控制变量
	int y;
	int x;
	int k;


	int nHist[100000] ;

	// 可能的边界数目
	int nEdgeNb     ;

	// 最大梯度值
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 

	// 初始化
	for(k=0; k<100000; k++) 
	{
		nHist[k] = 0; 
	}

	// 统计直方图，然后利用直方图计算阈值
	for(y=0; y<imgH; y++)
	{
		for(x=0; x<imgW; x++)
		{
			// 只是统计那些可能是边界点，并且还没有处理过的象素
			if(pImgEdge[y*imgW+x]==32768)
			{
				nHist[ pGradRange[y*imgW+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// 统计经过“非最大值抑止(non-maximum suppression)”后有多少象素
	for(k=1; k<100000; k++)
	{
		if(nHist[k] != 0)
		{
			// 最大梯度值
			nMaxMag = k;
		}

		// 梯度为0的点是不可能为边界点的
		// 经过non-maximum suppression后有多少象素
		nEdgeNb += nHist[k];
	}

	// 梯度比高阈值*pThdHigh小的象素点总数目
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);

	k = 1;
	nEdgeNb = nHist[1];

	// 计算高阈值
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// 设置高阈值
	*pThdHigh = k ;

	// 设置低阈值
	*pThdLow  = (int)((*pThdHigh) * dRatioLow+ 0.5);
	return 0;
}
int ConvFilter::EstimateThresholdSingle(ListImage *pListImage, int *pThdHigh,int *pThdLow, 
										double dRatioHigh, double dRatioLow,long* pGradRange)
{
	unsigned int *pImgEdge = (unsigned int*)pListImage->GetImgBuffer();

	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();
	// 循环控制变量
	int y;
	int x;
	int k;

	// 可能的边界数目
	int nEdgeNb     ;

	// 最大梯度值
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0; 
	nHighCount = 0;

	//统计直方图
	int nHist[70000] ;

	// 初始化
	for(k=0; k<70000; k++) 
	{
		nHist[k] = 0; 
	}

	// 统计直方图，然后利用直方图计算阈值
	for(y=0; y<imgH; y++)
	{
		for(x=0; x<imgW; x++)
		{
			// 只是统计那些可能是边界点，并且还没有处理过的象素
			if(pImgEdge[y*imgW+x]==2147483648)
			{
				nHist[ pGradRange[y*imgW+x]/100000 ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// 统计经过“非最大值抑止(non-maximum suppression)”后有多少象素
	for(k=1; k<70000; k++)
	{
		if(nHist[k] != 0)
		{
			// 最大梯度值
			nMaxMag = k * 100000;
		}

		// 梯度为0的点是不可能为边界点的
		// 经过non-maximum suppression后有多少象素
		nEdgeNb += nHist[k];
	}

	// 梯度比高阈值*pThdHigh小的象素点总数目
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);

	k = 1;
	nEdgeNb = nHist[1];

	// 计算高阈值
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}


	// 设置高阈值
	*pThdHigh = k * 100000;

	// 设置低阈值
	*pThdLow  = (int)((*pThdHigh) * dRatioLow+ 0.5);
	return 0;
}
int ConvFilter::TraceEdge (ListImage *pListImage,int y, int x, int nLowThd, long *pGradRange)
{ 
	switch(pListImage->GetImgDataType())
	{
	case uint_8:
		return TraceEdgeUint_8 (pListImage,y,x,nLowThd,pGradRange);
		break;
	case uint_12:
		return TraceEdgeUint_12(pListImage,y,x,nLowThd,pGradRange);
		break;
	case uint_16:
		return TraceEdgeUint_16 (pListImage,y,x,nLowThd,pGradRange);
		break;
	case single:
		return TraceEdgeSingle (pListImage,y,x,nLowThd,pGradRange);
		break;
	default:
		return TraceEdgeUint_8 (pListImage,y,x,nLowThd,pGradRange);
	}


} 

int ConvFilter::TraceEdgeUint_8 (ListImage *pListImage,int y, int x, int nLowThd, long *pGradRange)
{
	unsigned char*	pImgEdge = (unsigned char*)pListImage->GetImgBuffer();
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();

	// 对8邻域象素进行查询

	static int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	static int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;
	int k  ;

	for(k=0; k<8; k++)
	{
		yy = y + yNb[k] ;
		xx = x + xNb[k] ;
		if( yy >= imgH || xx >= imgW || yy< 0 || xx < 0)
		{
			continue;
		}
		// 如果该象素为可能的边界点，又没有处理过
		// 并且梯度大于阈值
		if(pImgEdge[yy*imgW+xx] == 128  && pGradRange[yy*imgW+xx]>=nLowThd)
		{
			// 把该点设置成为边界点
			pImgEdge[yy*imgW+xx] = 255 ;

			// 以该点为中心进行跟踪
			TraceEdgeUint_8(pListImage,yy, xx, nLowThd, pGradRange);
		}
	}
	return 0;
}
int ConvFilter::TraceEdgeUint_12 (ListImage *pListImage,int y, int x, int nLowThd, long *pGradRange)
{
	unsigned short*	pImgEdge = (unsigned short*)pListImage->GetImgBuffer();
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();
	// 对8邻域象素进行查询
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;

	int k  ;

	for(k=0; k<8; k++)
	{
		yy = y + yNb[k] ;
		xx = x + xNb[k] ;
		// 如果该象素为可能的边界点，又没有处理过
		// 并且梯度大于阈值
		if(pImgEdge[yy*imgW+xx] == 128  && pGradRange[yy*imgW+xx]>=nLowThd)
		{
			// 把该点设置成为边界点
			pImgEdge[yy*imgW+xx] = 4095 ;

			// 以该点为中心进行跟踪
			TraceEdge(pListImage,yy, xx, nLowThd, pGradRange);
		}
	}
	return 0;
}
int ConvFilter::TraceEdgeUint_16 (ListImage *pListImage,int y, int x, int nLowThd, long *pGradRange)
{
	unsigned short*	pImgEdge = (unsigned short*)pListImage->GetImgBuffer();
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();
	// 对8邻域象素进行查询
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;

	int k  ;

	for(k=0; k<8; k++)
	{
		yy = y + yNb[k] ;
		xx = x + xNb[k] ;
		// 如果该象素为可能的边界点，又没有处理过
		// 并且梯度大于阈值
		if(pImgEdge[yy*imgW+xx] == 32768  && pGradRange[yy*imgW+xx]>=nLowThd)
		{
			// 把该点设置成为边界点
			pImgEdge[yy*imgW+xx] = 65535 ;

			// 以该点为中心进行跟踪
			TraceEdge(pListImage,yy, xx, nLowThd, pGradRange);
		}
	}
	return 0;
}

int ConvFilter::TraceEdgeSingle (ListImage *pListImage,int y, int x, int nLowThd, long *pGradRange)
{
	unsigned int*	pImgEdge = (unsigned int*)pListImage->GetImgBuffer();
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	int imgSize = pListImage->GetImgDataSize();
	// 对8邻域象素进行查询
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;

	int k  ;

	for(k=0; k<8; k++)
	{
		yy = y + yNb[k] ;
		xx = x + xNb[k] ;
		// 如果该象素为可能的边界点，又没有处理过
		// 并且梯度大于阈值
		if(pImgEdge[yy*imgW+xx] == 2147483648  && pGradRange[yy*imgW+xx]>=nLowThd)
		{
			// 把该点设置成为边界点
			pImgEdge[yy*imgW+xx] = 4294967295;

			// 以该点为中心进行跟踪
			TraceEdge(pListImage,yy, xx, nLowThd, pGradRange);
		}
	}
	return 0;
}


//int ConvFilter::AddOrSubImgUint_8(ListImage *pSrc1,ListImage *pSrc2,ListImage *pDest,int mode ,
//								  float ratio1 ,float ratio2 )
//{
//	// 指向源、目标图像数据区的指针
//	unsigned char*	pSrcImgData1;
//	unsigned char*	pSrcImgData2;
//	unsigned char*	pDestImgData;
//
//	// 图像高、宽
//	int imgH;
//	int imgW;
//	int imgSize;
//
//	//初始化
//	imgSize = pSrc1->GetImgDataSize();
//	imgH = pSrc1->GetImgHeight();
//	imgW = pSrc1->GetImgWidth();
//	pSrcImgData1 = pSrc1->GetImgBuffer(); 
//	pSrcImgData2 = pSrc2->GetImgBuffer(); 
//	pDestImgData = pDest->GetImgBuffer(); 
//
//	// 循环变量
//	long	i;
//	long	j;
//	long   k;
//	int	temp;
//
//	if(mode == 0)
//	{
//		// 每行(除去边缘一行）
//		for(i = 0; i < imgH  ; i++)
//		{
//			// 每列(除去边缘一列）
//			for(j = 0; j < imgW  ; j++)
//			{
//				// 指向图像第i行，第j个象素的指针
//				k = imgW * i + j;
//
//				//计算并赋值
//				temp = pSrcImgData1[k]*ratio1 + pSrcImgData2[k]*ratio2;
//
//				// 判断是否小于阈值
//				if (temp < 255)
//				{
//					pDestImgData[k] = temp;
//				}
//				else
//				{
//					pDestImgData[k] = 255;
//				}
//			}
//		}
//	}
//	else
//	{
//		// 每行(除去边缘一行）
//		for(i = 0; i < imgH  ; i++)
//		{
//			// 每列(除去边缘一列）
//			for(j = 0; j < imgW  ; j++)
//			{
//				// 指向图像第i行，第j个象素的指针
//				k = imgW * i + j;
//
//				//计算并赋值
//				temp = pSrcImgData1[k]*ratio1 - pSrcImgData2[k]*ratio2;
//
//				// 判断是否小于阈值
//				if (temp > 0)
//				{
//					pDestImgData[k] = temp;
//				}
//				else
//				{
//					pDestImgData[k] = 0;
//				}
//			}
//		}
//	}
//	return 0;
//}
//
//int ConvFilter::AddOrSubImgUint_12(ListImage *pSrc1,ListImage *pSrc2,ListImage *pDest,int mode ,
//								   float ratio1 ,float ratio2 )
//{
//	// 指向源、目标图像数据区的指针
//	unsigned short*	pSrcImgData1;
//	unsigned short*	pSrcImgData2;
//	unsigned short*	pDestImgData;
//
//	// 图像高、宽
//	int imgH;
//	int imgW;
//	int imgSize;
//
//	//初始化
//	imgSize = pSrc1->GetImgDataSize();
//	imgH = pSrc1->GetImgHeight();
//	imgW = pSrc1->GetImgWidth();
//	pSrcImgData1 = (unsigned short*)pSrc1->GetImgBuffer(); 
//	pSrcImgData2 = (unsigned short*)pSrc2->GetImgBuffer(); 
//	pDestImgData = (unsigned short*)pDest->GetImgBuffer(); 
//
//	// 循环变量
//	long	i;
//	long	j;
//	long   k;
//	int	temp;
//
//	if(mode == 0)
//	{
//		// 每行(除去边缘一行）
//		for(i = 0; i < imgH  ; i++)
//		{
//			// 每列(除去边缘一列）
//			for(j = 0; j < imgW  ; j++)
//			{
//				// 指向图像第i行，第j个象素的指针
//				k = imgW * i + j;
//
//				//计算并赋值
//				temp = pSrcImgData1[k]*ratio1 + pSrcImgData2[k]*ratio2;
//
//				// 判断是否小于阈值
//				if (temp < 4095)
//				{
//					pDestImgData[k] = temp;
//				}
//				else
//				{
//					pDestImgData[k] = 4095;
//				}
//			}
//		}
//	}
//	else
//	{
//		// 每行(除去边缘一行）
//		for(i = 0; i < imgH  ; i++)
//		{
//			// 每列(除去边缘一列）
//			for(j = 0; j < imgW  ; j++)
//			{
//				// 指向图像第i行，第j个象素的指针
//				k = imgW * i + j;
//
//				//计算并赋值
//				temp = pSrcImgData1[k]*ratio1 - pSrcImgData2[k]*ratio2;
//
//				// 判断是否小于阈值
//				if (temp > 0)
//				{
//					pDestImgData[k] = temp;
//				}
//				else
//				{
//					pDestImgData[k] = 0;
//				}
//			}
//		}
//	}
//	return 0;
//}
//int ConvFilter::AddOrSubImgUint_16(ListImage *pSrc1,ListImage *pSrc2,ListImage *pDest,int mode ,
//								   float ratio1 ,float ratio2 )
//{	
//	// 指向源、目标图像数据区的指针
//	unsigned short*	pSrcImgData1;
//	unsigned short*	pSrcImgData2;
//	unsigned short*	pDestImgData;
//
//	// 图像高、宽
//	int imgH;
//	int imgW;
//	int imgSize;
//
//	//初始化
//	imgSize = pSrc1->GetImgDataSize();
//	imgH = pSrc1->GetImgHeight();
//	imgW = pSrc1->GetImgWidth();
//	pSrcImgData1 = (unsigned short*)pSrc1->GetImgBuffer(); 
//	pSrcImgData2 = (unsigned short*) pSrc2->GetImgBuffer(); 
//	pDestImgData = (unsigned short*)pDest->GetImgBuffer(); 
//
//	// 循环变量
//	long	i;
//	long	j;
//	long   k;
//	int	temp;
//
//	if(mode == 0)
//	{
//		// 每行(除去边缘一行）
//		for(i = 0; i < imgH  ; i++)
//		{
//			// 每列(除去边缘一列）
//			for(j = 0; j < imgW  ; j++)
//			{
//				// 指向图像第i行，第j个象素的指针
//				k = imgW * i + j;
//
//				//计算并赋值
//				temp = pSrcImgData1[k]*ratio1 + pSrcImgData2[k]*ratio2;
//
//				// 判断是否小于阈值
//				if (temp < 65535)
//				{
//					pDestImgData[k] = temp;
//				}
//				else
//				{
//					pDestImgData[k] = 65535;
//				}
//			}
//		}
//	}
//	else
//	{
//		// 每行(除去边缘一行）
//		for(i = 0; i < imgH  ; i++)
//		{
//			// 每列(除去边缘一列）
//			for(j = 0; j < imgW  ; j++)
//			{
//				// 指向图像第i行，第j个象素的指针
//				k = imgW * i + j;
//
//				//计算并赋值
//				temp = pSrcImgData1[k]*ratio1 - pSrcImgData2[k]*ratio2;
//
//				// 判断是否小于阈值
//				if (temp > 0)
//				{
//					pDestImgData[k] = temp;
//				}
//				else
//				{
//					pDestImgData[k] = 0;
//				}
//			}
//		}
//	}
//	return 0;
//}
//int ConvFilter::AddOrSubImgSingle(ListImage *pSrc1,ListImage *pSrc2,ListImage *pDest,int mode ,
//								  float ratio1 ,float ratio2 )
//{
//	// 指向源、目标图像数据区的指针
//	unsigned int*	pSrcImgData1;
//	unsigned int*	pSrcImgData2;
//	unsigned int*	pDestImgData;
//
//	// 图像高、宽
//	int imgH;
//	int imgW;
//
//	//初始化
//	imgH = pSrc1->GetImgHeight();
//	imgW = pSrc1->GetImgWidth();
//	pSrcImgData1 = (unsigned int*)pSrc1->GetImgBuffer(); 
//	pSrcImgData2 = (unsigned int*)pSrc2->GetImgBuffer(); 
//	pDestImgData = (unsigned int*)pDest->GetImgBuffer(); 
//
//	// 循环变量
//	long	i;
//	long	j;
//	long   k;
//	long long	temp;
//
//	if(mode == 0)
//	{
//		// 每行(除去边缘一行）
//		for(i = 0; i < imgH  ; i++)
//		{
//			// 每列(除去边缘一列）
//			for(j = 0; j < imgW  ; j++)
//			{
//				// 指向图像第i行，第j个象素的指针
//				k = imgW * i + j;
//
//				//计算并赋值
//				temp = pSrcImgData1[k]*ratio1 + pSrcImgData2[k]*ratio2;
//
//				// 判断是否小于阈值
//				if (temp < (long long)65536 *(long long)65536 - 1)
//				{
//					pDestImgData[k] = temp;
//				}
//				else
//				{
//					pDestImgData[k] = (long long)65536 *(long long)65536 - 1	;
//				}
//			}
//		}
//	}
//	else
//	{
//		// 每行(除去边缘一行）
//		for(i = 0; i < imgH  ; i++)
//		{
//			// 每列(除去边缘一列）
//			for(j = 0; j < imgW  ; j++)
//			{
//				// 指向图像第i行，第j个象素的指针
//				k = imgW * i + j;
//
//				//计算并赋值
//				temp = pSrcImgData1[k]*ratio1 - pSrcImgData2[k]*ratio2;
//
//				// 判断是否小于阈值
//				if (temp > 0)
//				{
//					pDestImgData[k] = temp;
//				}
//				else
//				{
//					pDestImgData[k] = 0;
//				}
//			}
//		}
//	}
//	return 0;
//}

//int SubImgUint_8(ListImage *pSrc1,ListImage *pSrc2,int * pDest,
//	float ratio1 = 1,float ratio2 = 1);
//int SubImgUint_12(ListImage *pSrc1,ListImage *pSrc2,int * pDest,
//	float ratio1 = 1,float ratio2 = 1);
int ConvFilter::SubImgUint_16(ListImage *pSrc1,ListImage *pSrc2,int * pDest,
				  float ratio1,float ratio2)
{	
	// 指向源、目标图像数据区的指针
	unsigned short*	pSrcImgData1;
	unsigned short*	pSrcImgData2;
	int*	pDestImgData;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgSize = pSrc1->GetImgDataSize();
	imgH = pSrc1->GetImgHeight();
	imgW = pSrc1->GetImgWidth();
	pSrcImgData1 = (unsigned short*)pSrc1->GetImgBuffer(); 
	pSrcImgData2 = (unsigned short*) pSrc2->GetImgBuffer(); 
	pDestImgData = pDest; 

	// 循环变量
	long	i;
	long	j;
	long   k;
	int	temp;


	// 每行(除去边缘一行）
	for(i = 0; i < imgH  ; i++)
	{
		// 每列(除去边缘一列）
		for(j = 0; j < imgW  ; j++)
		{
			// 指向图像第i行，第j个象素的指针
			k = imgW * i + j;

			//计算并赋值
			temp = pSrcImgData1[k]*ratio1 - pSrcImgData2[k]*ratio2;

			// 判断是否小于阈值
			if (temp > 0)
			{
				pDestImgData[k] = temp;
			}
			else
			{
				pDestImgData[k] = 0;
			}
		}
	}
	return 0;
}
//int SubImgSingle(ListImage *pSrc1,ListImage *pSrc2,int * pDest,
//	float ratio1 = 1,float ratio2 = 1);

//int AddImgUint_8(ListImage *pSrc1,int * pSrc2,ListImage *pDest
//	float ratio1 = 1,float ratio2 = 1);
//int AddImgUint_12(ListImage *pSrc1,int* pSrc2,ListImage *pDest
//	float ratio1 = 1,float ratio2 = 1);
int ConvFilter::AddImgUint_16(ListImage *pSrc1,int *pSrc2,ListImage *pDest,
	float ratio1,float ratio2 )
{	
	// 指向源、目标图像数据区的指针
	unsigned short*	pSrcImgData1;
	int*	pSrcImgData2;
	unsigned short*	pDestImgData;

	// 图像高、宽
	int imgH;
	int imgW;
	int imgSize;

	//初始化
	imgSize = pSrc1->GetImgDataSize();
	imgH = pSrc1->GetImgHeight();
	imgW = pSrc1->GetImgWidth();
	pSrcImgData1 = (unsigned short*)pSrc1->GetImgBuffer(); 
	pSrcImgData2 = pSrc2; 
	pDestImgData = (unsigned short*) pDest->GetImgBuffer(); 

	// 循环变量
	long	i;
	long	j;
	long   k;
	int	temp;


	// 每行(除去边缘一行）
	for(i = 0; i < imgH  ; i++)
	{
		// 每列(除去边缘一列）
		for(j = 0; j < imgW  ; j++)
		{
			// 指向图像第i行，第j个象素的指针
			k = imgW * i + j;

			//计算并赋值
			temp = pSrcImgData1[k]*ratio1 - pSrcImgData2[k]*ratio2;

			// 判断是否小于阈值
			if (temp > 0)
			{
				pDestImgData[k] = temp;
			}
			else
			{
				pDestImgData[k] = 0;
			}
		}
	}
	return 0;
}
//int AddImgSingle(ListImage *pSrc1,int *pSrc2,ListImage *pDest
//	float ratio1 = 1,float ratio2 = 1);
bool ConvFilter::IsImageIllegal( ListImage *pListImage)
{
	if( pListImage->GetImgChannel() != 1 )
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool ConvFilter::IsFilterKernelIllegal( FilterKernel &Temp)
{
	if(Temp.fCoef < 0 || Temp.iTempH < 1 || Temp.iTempW < 1 
		|| Temp.iTempMX < 0 || Temp.iTempMY < 0 
		|| Temp.fpArray == NULL )
	{
		return false;
	}
	else
	{
		return true;
	}
}

//int ConvFilter::AddOrSubImg(ListImage *pSrc1,ListImage *pSrc2,ListImage *pDest,int mode ,
//							float ratio1 ,float ratio2 )
//{
//	switch(pSrc1->GetImgDataType())
//	{
//	case uint_8:
//		return AddOrSubImgUint_8(pSrc1,pSrc2, pDest,mode,ratio1,ratio2);
//		break;
//	case uint_12:
//		return AddOrSubImgUint_12(pSrc1,pSrc2, pDest,mode,ratio1,ratio2);
//		break;
//	case uint_16:
//		return AddOrSubImgUint_16(pSrc1,pSrc2, pDest,mode,ratio1,ratio2);
//		break;
//	case single:
//		return AddOrSubImgSingle(pSrc1,pSrc2, pDest,mode,ratio1,ratio2);
//		break;
//	default:
//		return AddOrSubImgUint_8(pSrc1,pSrc2, pDest,mode,ratio1,ratio2);
//	}
//}