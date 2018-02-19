//  API�����⣺
//  TemplateConvConv()	- ͼ��ģ��任��ͨ���ı�ģ�壬��������ʵ��
//						  ͼ���ƽ�����񻯡���Եʶ��Ȳ�����
//  MedianFilter()		- ͼ����ֵ�˲���
//  GetMedianNum()		- ��ȡ��ֵ��������MedianFilter()����������ֵ��
//  GradSharp()		    - �ú���������ͼ������ݶ��񻯡�
//  Robert()		    - robert��Ե�������
//  Sobel()		        - sobel��Ե�������
//  Prewitt()	        - prewitt��Ե�������
//  Kirsch()		    - kirsch��Ե�������
//  Gauss()		        - gauss��Ե�������
//  Canny()				- Canny��Ե�������
//  UnsharpMasking -������ģ

#include "stdafx.h"
#include "ConvFilter.h"

#include <math.h>
#include <direct.h>
ConvFilter::ConvFilter(void)
{
	//����Ĭ�ϵ�FilterKernel���캯����ʼ����Ա����
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
	// ָ��Դͼ���ָ��
	unsigned char*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int srcPos;

	// ָ����ͼ���ָ��
	unsigned char*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	// ѭ������
	long i;
	long j;
	long k;
	long l;

	// ������
	float fResult;

	//��ʼ��
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
		//���ֱ�Ե����ԭ��
		//ȥ����Ե����//ȥ����Ե����
		for(i = Temp.iTempMY; i <imgH - Temp.iTempH + Temp.iTempMY + 1 ; i++)
		{
			for(j = Temp.iTempMX; j <imgW - Temp.iTempW + Temp.iTempMX + 1; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos = imgW * i + j;
				fResult = 0;

				// ����
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��

						srcPos = imgW * (i - Temp.iTempMY + k )
							+ j - Temp.iTempMX + l;

						fResult += pSrcImgData[srcPos] * Temp.fpArray[k * Temp.iTempW + l];
					}
				}
				// ����ϵ��
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
		//�ڱ�Ե������0.30..........3990�����Ե����
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos =  imgW * i + j;
				fResult = 0;

				// ����
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
							// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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
		int L;		//����
		int C;		//����

		//�ڱ�Ե�����Ա߱�Ե���ش���	
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos = imgW * i + j;
				fResult = 0;

				// ����
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						L = (i - Temp.iTempMY + k );
						C = j - Temp.iTempMX + l;
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��

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

						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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


	// ���Ʊ任���ͼ��
	pListImage->SetData(pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;
	return 0;
}
int ConvFilter::TemplateConvUint_12(ListImage *pListImage,FilterKernel &Temp,int mode)
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int	srcPos;

	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	// ѭ������
	long i;
	long j;
	long k;
	long l;

	// ������
	float fResult;

	//��ʼ��
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
		//���ֱ�Ե����ԭ��
		//ȥ����Ե����//ȥ����Ե����
		for(i = Temp.iTempMY; i <imgH - Temp.iTempH + Temp.iTempMY + 1 ; i++)
		{
			for(j = Temp.iTempMX; j <imgW - Temp.iTempW + Temp.iTempMX + 1 ; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos =  imgW * i + j;
				fResult = 0;

				// ����
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
						srcPos = imgW * (i - Temp.iTempMY + k )
							+ j - Temp.iTempMX + l;
						fResult +=( pSrcImgData [srcPos] & (4095) )
							* Temp.fpArray[k * Temp.iTempW + l];

					}
				}
				// ����ϵ��
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
		//�ڱ�Ե�����㴦���Ե����
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos =  imgW * i + j;
				fResult = 0;

				// ����
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
							// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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

		int L;		//����
		int C;		//����

		//�ڱ�Ե�����Ա߱�Ե���ش���	
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos =  imgW * i + j;
				fResult = 0;

				// ����
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						L = (i - Temp.iTempMY + k );
						C = j - Temp.iTempMX + l;
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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

	// ���Ʊ任���ͼ��
	pListImage->SetData( (unsigned char*)pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;
	return 0;
}
int ConvFilter::TemplateConvUint_16(ListImage *pListImage,FilterKernel &Temp,int mode)
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int	srcPos;

	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	// ѭ������
	long i;
	long j;
	long k;
	long l;

	// ������
	float fResult;

	//��ʼ��
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
		//���ֱ�Ե����ԭ��
		//ȥ����Ե����//ȥ����Ե����
		for(i = Temp.iTempMY; i <imgH - Temp.iTempH + Temp.iTempMY + 1 ; i++)
		{
			for(j = Temp.iTempMX; j <imgW - Temp.iTempW + Temp.iTempMX + 1 ; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos =  imgW * i + j;
				fResult = 0;

				// ����
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
						srcPos =  imgW * (i - Temp.iTempMY + k )
							+ j - Temp.iTempMX + l;
						fResult += pSrcImgData [srcPos] 
						* Temp.fpArray[k * Temp.iTempW + l];

					}
				}
				// ����ϵ��
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
		//�ڱ�Ե�����㴦���Ե����
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos =  imgW * i + j;
				fResult = 0;

				// ����
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
							// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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

		int L;		//����
		int C;		//����

		//�ڱ�Ե�����Ա߱�Ե���ش���	
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos =  imgW * i + j;
				fResult = 0;

				// ����
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						L = (i - Temp.iTempMY + k );
						C = j - Temp.iTempMX + l;
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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

	// ���Ʊ任���ͼ��
	pListImage->SetData( (unsigned char*)pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;
	return 0;
}




int ConvFilter::TemplateConvSingle(ListImage *pListImage,FilterKernel &Temp,int mode)
{
	// ָ��Դͼ���ָ��
	unsigned int*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int	srcPos;

	// ָ����ͼ���ָ��
	unsigned int*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos	;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	// ѭ������
	long i;
	long j;
	long k;
	long l;

	// ������
	double fResult;

	//��ʼ��
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	imgSize = pListImage->GetImgDataSize();
	pSrcImgData = (unsigned int*)pListImage->GetImgBuffer(); 
	pCpyImgData = new unsigned int[imgSize / 4];
	if( pCpyImgData == NULL )
	{
		return -1;
	}

	memcpy(pCpyImgData,pSrcImgData,imgSize);		//ָ���ֽڳ���

	switch(mode)
	{
	case 0:
		//���ֱ�Ե����ԭ��
		//ȥ����Ե����//ȥ����Ե����
		for(i = Temp.iTempMY; i <imgH - Temp.iTempH + Temp.iTempMY + 1 ; i++)
		{
			for(j = Temp.iTempMX; j <imgW - Temp.iTempW + Temp.iTempMX + 1 ; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos =  imgW * i + j;
				fResult = 0;

				// ����
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
						srcPos =  imgW * (i - Temp.iTempMY + k )
							+ j - Temp.iTempMX + l;
						fResult +=pSrcImgData [srcPos]
						* Temp.fpArray[k * Temp.iTempW + l];

					}
				}
				// ����ϵ��
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
		//�ڱ�Ե�����㴦���Ե����
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos =  imgW * i + j;
				fResult = 0;

				// ����
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
							// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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

		int L;		//����
		int C;		//����

		//�ڱ�Ե�����Ա߱�Ե���ش���	
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos =  imgW * i + j;
				fResult = 0;

				// ����
				for (k = 0; k < Temp.iTempH; k++)
				{
					for (l = 0; l < Temp.iTempW; l++)
					{
						L = (i - Temp.iTempMY + k );
						C = j - Temp.iTempMX + l;
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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

	// ���Ʊ任���ͼ��
	pListImage->SetData( (unsigned char*)pCpyImgData, imgSize);
	//pListImage->SetData( (unsigned char*)pSrcImgData, imgSize);

	// �ͷ��ڴ�
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
	// ָ��Դͼ���ָ��
	unsigned char*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int	srcPos;

	// ָ����ͼ���ָ��
	unsigned char*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ָ���˲��������ָ��
	unsigned char* aValue;

	//ѭ������
	long i;
	long j;
	long k;
	long l;

	aValue = new unsigned char[ m_Temp.iTempH * m_Temp.iTempW ];
	if(aValue == NULL)
	{
		return -1;
	}

	//��ʼ��ֵ�˲�
	//�У�ȥ����Ե���У�
	for (i = m_Temp.iTempMY; 
		i < imgH-m_Temp.iTempH + m_Temp.iTempMY+1;
		i++)
	{
		//�У�ȥ����Ե���У�
		for(j = m_Temp.iTempMX;
			j < imgW-m_Temp.iTempW+m_Temp.iTempMX+1;
			j++)
		{
			// ָ���µ�i�У���j�����ص�ָ��
			cpyPos = imgW * i + j;

			//��ȡ�˲�������
			for (k = 0; k < m_Temp.iTempH; k++)
			{
				for(l = 0; l <m_Temp.iTempW; l++)
				{
					/// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
					srcPos =  imgW * (i - m_Temp.iTempMY + k )
						+ j - m_Temp.iTempMX + l;
					//��������ֵ
					aValue[k*m_Temp.iTempW+l] = pSrcImgData[srcPos];
				}
			}

			//��ȡ��ֵ
			pCpyImgData[cpyPos] = GetMedianNum(aValue);
		}
	}

	// ���Ʊ任���ͼ��
	pListImage->SetData(pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;
	delete []aValue;
	return 0;
}
int ConvFilter::MedianFilterUint_12( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int	srcPos;

	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ָ���˲��������ָ��
	unsigned short* aValue;

	//ѭ������
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
		//��ʼ��ֵ�˲�
		//�У�ȥ����Ե���У�
		for (i = m_Temp.iTempMY; 
			i < imgH-m_Temp.iTempH + m_Temp.iTempMY+1;
			i++)
		{
			//�У�ȥ����Ե���У�
			for(j = m_Temp.iTempMX;
				j < imgW-m_Temp.iTempW+m_Temp.iTempMX+1;
				j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos = imgW * i + j;

				//��ȡ�˲�������
				for (k = 0; k < m_Temp.iTempH; k++)
				{
					for(l = 0; l <m_Temp.iTempW; l++)
					{
						/// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
						srcPos =  imgW * (i - m_Temp.iTempMY + k )
							+ j - m_Temp.iTempMX + l;
						//��������ֵ
						aValue[k*m_Temp.iTempW+l] =( pSrcImgData[srcPos] & 4095 );
					}
				}

				//��ȡ��ֵ
				pCpyImgData[cpyPos] = GetMedianNum(aValue);
			}
		}
		break;
	case 1:
		//�ڱ�Ե�����㴦���Ե����
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos = imgW * i + j;


				// ����
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
							// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
							srcPos =  imgW * (i - m_Temp.iTempMY + k )
								+ j - m_Temp.iTempMX + l;
						}
						//��������ֵ
						aValue[k*m_Temp.iTempW+l] = ( pSrcImgData[srcPos] & 4095 );
					}
				}
				//��ȡ��ֵ
				pCpyImgData[cpyPos] = GetMedianNum(aValue);
			}
		}
		break;
	case 2:

		int L;		//����
		int C;		//����

		//�ڱ�Ե�����Ա߱�Ե���ش���	
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos = imgW * i + j;

				// ����
				for (k = 0; k < m_Temp.iTempH; k++)
				{
					for (l = 0; l < m_Temp.iTempW; l++)
					{
						L = (i - m_Temp.iTempMY + k );
						C = j - m_Temp.iTempMX + l;
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
						srcPos =  imgW * L
							+ C;
						//��������ֵ
						aValue[k*m_Temp.iTempW+l] = ( pSrcImgData[srcPos] & 4095 );
					}
				}
				//��ȡ��ֵ
				pCpyImgData[cpyPos] = GetMedianNum(aValue);
			}
		}
		break;
	default:
		break;
	}

	// ���Ʊ任���ͼ��
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;
	delete []aValue;
	return 0;
}
int ConvFilter::MedianFilterUint_16( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int	srcPos;

	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ָ���˲��������ָ��
	unsigned short* aValue;

	//ѭ������
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
		//��ʼ��ֵ�˲�
		//�У�ȥ����Ե���У�
		for (i = m_Temp.iTempMY; 
			i < imgH-m_Temp.iTempH + m_Temp.iTempMY+1;
			i++)
		{
			//�У�ȥ����Ե���У�
			for(j = m_Temp.iTempMX;
				j < imgW-m_Temp.iTempW+m_Temp.iTempMX+1;
				j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos = imgW * i + j;

				//��ȡ�˲�������
				for (k = 0; k < m_Temp.iTempH; k++)
				{
					for(l = 0; l <m_Temp.iTempW; l++)
					{
						/// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
						srcPos =  imgW * (i - m_Temp.iTempMY + k )
							+ j - m_Temp.iTempMX + l;
						//��������ֵ
						aValue[k*m_Temp.iTempW+l] = pSrcImgData[srcPos];
					}
				}

				//��ȡ��ֵ
				pCpyImgData[cpyPos] = GetMedianNum(aValue);
			}
		}
		break;
	case 1:
		//�ڱ�Ե�����㴦���Ե����
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos = imgW * i + j;


				// ����
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
							// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
							srcPos =  imgW * (i - m_Temp.iTempMY + k )
								+ j - m_Temp.iTempMX + l;
						}
						//��������ֵ
						aValue[k*m_Temp.iTempW+l] = pSrcImgData[srcPos];
					}
				}
				//��ȡ��ֵ
				pCpyImgData[cpyPos] = GetMedianNum(aValue);
			}
		}
		break;
	case 2:

		int L;		//����
		int C;		//����

		//�ڱ�Ե�����Ա߱�Ե���ش���	
		for(i = 0; i <imgH; i++)
		{
			for(j = 0; j <imgW; j++)
			{
				// ָ���µ�i�У���j�����ص�ָ��
				cpyPos = imgW * i + j;

				// ����
				for (k = 0; k < m_Temp.iTempH; k++)
				{
					for (l = 0; l < m_Temp.iTempW; l++)
					{
						L = (i - m_Temp.iTempMY + k );
						C = j - m_Temp.iTempMX + l;
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
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
						// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
						srcPos =  imgW * L
							+ C;
						//��������ֵ
						aValue[k*m_Temp.iTempW+l] = pSrcImgData[srcPos];
					}
				}
				//��ȡ��ֵ
				pCpyImgData[cpyPos] = GetMedianNum(aValue);
			}
		}
		break;
	default:
		break;
	}

	// ���Ʊ任���ͼ��
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;
	delete []aValue;
	return 0;
}
int ConvFilter::MedianFilterSingle( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned int*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int	srcPos;

	// ָ����ͼ���ָ��
	unsigned int*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ָ���˲��������ָ��
	unsigned int* aValue;

	//ѭ������
	long i;
	long j;
	long k;
	long l;

	aValue = new unsigned int[ m_Temp.iTempH * m_Temp.iTempW ];
	if(aValue == NULL)
	{
		return -1;
	}

	//��ʼ��ֵ�˲�
	//�У�ȥ����Ե���У�
	for (i = m_Temp.iTempMY; 
		i < imgH-m_Temp.iTempH + m_Temp.iTempMY+1;
		i++)
	{
		//�У�ȥ����Ե���У�
		for(j = m_Temp.iTempMX;
			j < imgW-m_Temp.iTempW+m_Temp.iTempMX+1;
			j++)
		{
			// ָ���µ�i�У���j�����ص�ָ��
			cpyPos = imgW * i + j;

			//��ȡ�˲�������
			for (k = 0; k < m_Temp.iTempH; k++)
			{
				for(l = 0; l <m_Temp.iTempW; l++)
				{
					/// ָ���i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
					srcPos =  imgW * (i - m_Temp.iTempMY + k )
						+ j - m_Temp.iTempMX + l;
					//��������ֵ
					aValue[k*m_Temp.iTempW+l] = pSrcImgData[srcPos];
				}
			}

			//��ȡ��ֵ
			pCpyImgData[cpyPos] = GetMedianNum(aValue);
		}
	}

	// ���Ʊ任���ͼ��
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;
	delete []aValue;
	return 0;
}
unsigned char ConvFilter::GetMedianNum(unsigned char * Array)
{
	// ѭ������
	int		i;
	int		j;

	// �м����
	unsigned char temp;

	//���鳤��
	int iFilterLen = m_Temp.iTempH * m_Temp.iTempW;

	// ��ð�ݷ��������������
	for (j = 0; j < iFilterLen; j++)
	{
		for (i = iFilterLen-1; i > j ; i--)
		{
			if (Array[i] < Array[i - 1])
			{
				// ����
				temp = Array[i];
				Array[i] = Array[i - 1];
				Array[i - 1] = temp;
			}
		}
	}

	// ������ֵ
	if ((iFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		temp = Array[(iFilterLen + 1) / 2 - 1];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		temp = (Array[iFilterLen / 2] + Array[iFilterLen / 2 - 1]) / 2;
	}

	// ������ֵ
	return temp;
}

unsigned short ConvFilter::GetMedianNum(unsigned short * Array)
{
	// ѭ������
	int		i;
	int		j;

	// �м����
	unsigned short temp;

	//���鳤��
	int iFilterLen = m_Temp.iTempH * m_Temp.iTempW;

	// ��ð�ݷ��������������
	for (j = 0; j < iFilterLen; j++)
	{
		for (i = iFilterLen-1; i > j ; i--)
		{
			if (Array[i] < Array[i - 1])
			{
				// ����
				temp = Array[i];
				Array[i] = Array[i - 1];
				Array[i - 1] = temp;
			}
		}
	}

	// ������ֵ
	if ((iFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		temp = Array[(iFilterLen + 1) / 2 - 1];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		temp = (Array[iFilterLen / 2] + Array[iFilterLen / 2 - 1]) / 2;
	}

	// ������ֵ
	return temp;
}
unsigned int ConvFilter::GetMedianNum(unsigned int * Array)
{
	// ѭ������
	int		i;
	int		j;

	// �м����
	unsigned int temp;

	//���鳤��
	int iFilterLen = m_Temp.iTempH * m_Temp.iTempW;

	// ��ð�ݷ��������������
	for (j = 0; j < iFilterLen; j++)
	{
		for (i = iFilterLen-1; i > j ; i--)
		{
			if (Array[i] < Array[i - 1])
			{
				// ����
				temp = Array[i];
				Array[i] = Array[i - 1];
				Array[i - 1] = temp;
			}
		}
	}

	// ������ֵ
	if ((iFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		temp = Array[(iFilterLen + 1) / 2 - 1];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		temp = (Array[iFilterLen / 2] + Array[iFilterLen / 2 - 1]) / 2;
	}

	// ������ֵ
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
	// ָ��Դͼ����������ָ��
	unsigned char*	pSrcImgData;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	// ָ��Դͼ���Ŀ��λ��
	int pos;
	int pos_1;
	int pos_2;

	//��ʼ��
	imgSize = pListImage->GetImgDataSize();
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	pSrcImgData = pListImage->GetImgBuffer(); 

	// ѭ������
	long	i;
	long	j;
	int	temp;

	// ÿ��(��ȥ��Եһ�У�
	for(i = 0; i < imgH - 1  ; i++)
	{
		// ÿ��(��ȥ��Եһ�У�
		for(j = 0; j < imgW - 1 ; j++)
		{
			pos = imgW * i
				+ j;
			pos_1 = imgW *( i+1) 
				+ j;
			pos_2 = imgW * i 
				+ j + 1;

			//�����ݶȵĽ���ֵ   abs��int�� ȡ����ֵ
			temp = abs( (int)pSrcImgData[pos] -  (int)pSrcImgData[pos_1] ) 
				+ abs( (int)pSrcImgData[pos] -  (int)pSrcImgData[pos_2]);

			// �ж��Ƿ�С����ֵ
			if (temp < 255)
			{
				// �ж��Ƿ������ֵ������С��������Ҷ�ֵ���䡣
				if (temp >= thre)
				{
					// ֱ�Ӹ�ֵΪtemp
					pSrcImgData[pos] = temp;
				}
			}
			else
			{
				// ֱ�Ӹ�ֵΪ255
				pSrcImgData[pos] = 255;
			}
		}
	}
	return 0;
}
int ConvFilter::GradSharpUint_12( ListImage *pListImage,unsigned short thre )
{
	// ָ��Դͼ����������ָ��
	unsigned short*	pSrcImgData;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	// ָ��Դͼ���Ŀ��λ��
	int pos;
	int pos_1;
	int pos_2;

	//��ʼ��
	imgSize = pListImage->GetImgDataSize();
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 

	// ѭ������
	long	i;
	long	j;
	int	temp;

	// ÿ��(��ȥ��Եһ�У�
	for(i = 0; i < imgH - 1  ; i++)
	{
		// ÿ��(��ȥ��Եһ�У�
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

				//�����ݶȵĽ���ֵ   abs��int�� ȡ����ֵ
				temp = abs( (int)pSrcImgData[pos] -  (int)pSrcImgData[pos_1] ) 
					+ abs( (int)pSrcImgData[pos] -  (int)pSrcImgData[pos_2]);

				// �ж��Ƿ�С����ֵ
				if (temp < 4095)
				{
					// �ж��Ƿ������ֵ������С��������Ҷ�ֵ���䡣
					if (temp >= thre)
					{
						// ֱ�Ӹ�ֵΪtemp
						pSrcImgData[pos] = temp;
					}
				}
				else
				{
					// ֱ�Ӹ�ֵΪ4095
					pSrcImgData[pos] = 4095;
				}
			}
		}
	}
	return 0;
}
int ConvFilter::GradSharpUint_16( ListImage *pListImage,unsigned short thre )
{
	// ָ��Դͼ����������ָ��
	unsigned short*	pSrcImgData;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	// ָ��Դͼ���Ŀ��λ��
	int pos;
	int pos_1;
	int pos_2;

	//��ʼ��
	imgSize = pListImage->GetImgDataSize();
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	pSrcImgData = (unsigned short*)pListImage->GetImgBuffer(); 

	// ѭ������
	long	i;
	long	j;
	int	temp;

	// ÿ��(��ȥ��Եһ�У�
	for(i = 0; i < imgH - 1  ; i++)
	{
		// ÿ��(��ȥ��Եһ�У�
		for(j = 0; j < imgW - 1 ; j++)
		{
			pos = imgW * i
				+ j;
			pos_1 = imgW *( i+1) 
				+ j;
			pos_2 = imgW * i 
				+ j + 1;

			//�����ݶȵĽ���ֵ   abs��int�� ȡ����ֵ
			temp = abs( (int)pSrcImgData[pos] -  (int)pSrcImgData[pos_1] ) 
				+ abs( (int)pSrcImgData[pos] -  (int)pSrcImgData[pos_2]);

			// �ж��Ƿ�С����ֵ
			if (temp < 65535)
			{
				// �ж��Ƿ������ֵ������С��������Ҷ�ֵ���䡣
				if (temp >= thre)
				{
					// ֱ�Ӹ�ֵΪtemp
					pSrcImgData[pos] = temp;
				}
			}
			else
			{
				// ֱ�Ӹ�ֵΪ255
				pSrcImgData[pos] = 65535;
			}
		}
	}
	return 0;
}
int ConvFilter::GradSharpSingle( ListImage *pListImage,unsigned int thre )
{
	// ָ��Դͼ����������ָ��
	unsigned int*	pSrcImgData;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	// ָ��Դͼ���Ŀ��λ��
	int pos;
	int pos_1;
	int pos_2;

	//��ʼ��
	imgSize = pListImage->GetImgDataSize();
	imgH = pListImage->GetImgHeight();
	imgW = pListImage->GetImgWidth();
	pSrcImgData = (unsigned int*)pListImage->GetImgBuffer(); 

	// ѭ������
	long	i;
	long	j;
	double 	temp;

	// ÿ��(��ȥ��Եһ�У�
	for(i = 0; i < imgH - 1  ; i++)
	{
		// ÿ��(��ȥ��Եһ�У�
		for(j = 0; j < imgW - 1 ; j++)
		{
			pos = imgW * i
				+ j;
			pos_1 = imgW *( i+1) 
				+ j;
			pos_2 = imgW * i 
				+ j + 1;

			//�����ݶȵĽ���ֵ   abs��int�� ȡ����ֵ
			temp = abs( (double)pSrcImgData[pos] -  (double)pSrcImgData[pos_1] ) 
				+ abs( (double)pSrcImgData[pos] -  (double)pSrcImgData[pos_2]);

			// �ж��Ƿ�С����ֵ
			if (temp < 4294967295)
			{
				// �ж��Ƿ������ֵ������С��������Ҷ�ֵ���䡣
				if (temp >= thre)
				{
					// ֱ�Ӹ�ֵΪtemp
					pSrcImgData[pos] = temp;
				}
			}
			else
			{
				// ֱ�Ӹ�ֵΪ255
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

	//��ʱ�������洢��Ƶ����
	int * copyImg ;
	//��ʱ�������洢ԭͼ
	ListImage t_srcImg ;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
	imgH = (&destImg)->GetImgHeight();
	imgW = (&destImg)->GetImgWidth();
	imgSize = (&destImg)->GetImgDataSize();
	copyImg = new int[imgH*imgW];
	t_srcImg = srcImg;

	// ��һ������ԭͼ�����Gauss�˲�
	// 3��3��˹ģ��
	m_Temp.iTempH = 3;
	m_Temp.iTempW  = 3;
	m_Temp.iTempMX = 1;
	m_Temp.iTempMY = 1;
	m_Temp.fCoef = (float) (1.0 / 16.0);
	// ����ģ��Ԫ��
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

	//�ڶ�������ģ�����ͼ��ɫ����������copyImg��
	//�ڶ�������ԭͼ��ȥƽ�����ͼ�񣬵õ���Ƶ���֣��洢��copyImg��
	SubImgUint_16(&t_srcImg, &destImg, copyImg,1,1);

	////������, ����Ƶ������ԭͼ��ӣ��õ���Ƶ��ǿ��ͼ��
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

	// ָ��x,y��������ָ��
	long * pGradX; 
	long * pGradY;

	// �ݶȵķ���
	long * pGradRange;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	// ��һ������ԭͼ�����Gauss�˲�
	// 3��3��˹ģ��
	m_Temp.iTempH = 3;
	m_Temp.iTempW  = 3;
	m_Temp.iTempMX = 1;
	m_Temp.iTempMY = 1;
	m_Temp.fCoef = (float) (1.0 / 16.0);
	// ����ģ��Ԫ��
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

	// �ڶ�������Gauss�˲����ͼ����㷽����


	if( DirGrad((&destImg),pGradX,pGradY) != 0)
	{
		return DirGrad((&destImg),pGradX,pGradY);
	}


	// �������������ݶȵķ���

	if( GradRange((&destImg),pGradX,pGradY,pGradRange) != 0)
	{
		return GradRange((&destImg),pGradX,pGradY,pGradRange);
	}


	// ���Ĳ���Ӧ��non-maximum ����
	if( NonmaxSuppress((&destImg),pGradX, pGradY,pGradRange) != 0 )
	{
		return NonmaxSuppress((&destImg),pGradX, pGradY,pGradRange) ;
	}

	// ���岽��Ӧ��Hysteresis���ҵ����еı߽�
	if( Hysteresis((&destImg),pGradRange,dRatioLow,dRatioHigh)!= 0 )
	{
		return Hysteresis((&destImg),pGradRange,dRatioLow,dRatioHigh);
	}

	//pListImage->SetData(pImgEdge,imgSize);

	// �ͷ��ڴ�
	delete pGradX      ;
	delete pGradY      ;
	delete pGradRange  ;

	return 0;
}

int ConvFilter::RobertsUint_8 ( ListImage *pListImage )
{
	// ָ��Դͼ���ָ��
	unsigned char*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int	srcPos;

	// ָ����ͼ���ָ��
	unsigned char*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	//����ֵ
	double result;
	unsigned char pixel[4];

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	memset(pCpyImgData, (unsigned char)255, imgSize );


	//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
	for(j = imgH -1; j > 0; j--)
	{
		for(i = 0;i <imgW-1; i++)
		{
			//����ʹ��2��2��ģ�壬Ϊ��ֹԽ�磬���Բ��������±ߺ����ұߵ���������
			// ָ��Դͼ���j�У���i�����ص�ָ��			
			srcPos =  imgW * j + i;

			// ָ��Ŀ��ͼ���j�У���i�����ص�ָ��			
			cpyPos = imgW * j + i;

			//ȡ�õ�ǰָ�봦2*2���������ֵ��ע��Ҫת��Ϊunsigned char��
			pixel[0] = (unsigned char)pSrcImgData[srcPos] ;
			pixel[1] = (unsigned char)pSrcImgData[srcPos+1]  ;
			pixel[2] =(unsigned char)pSrcImgData[srcPos - imgW] ;
			pixel[3] =(unsigned char)pSrcImgData[(srcPos - imgW + 1)] ;

			//����Ŀ��ͼ���еĵ�ǰ��
			result = sqrt((double)( pixel[0] - pixel[3] )*( pixel[0] - pixel[3] ) 
				+ ( pixel[1] - pixel[2] )*( pixel[1] - pixel[2] ));
			pCpyImgData[cpyPos] = (unsigned char)result;

		}
	}

	// ���Ƹ�ʴ���ͼ��
	pListImage->SetData( pCpyImgData,imgSize );

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
}
int ConvFilter::SobelUint_8 ( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned char*	pSrcImgData;

	// ָ����ͼ���ָ��
	unsigned char*	pCpyImgData1;
	// ָ����ͼ���ָ��
	unsigned char*	pCpyImgData2;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos1;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos2;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	// ����Sobelģ�����
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

	//����ListImage
	ListImage listTemp(imgW, imgH);
	listTemp.SetData(pCpyImgData1,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// ����Sobelģ�����
	sobelTemp.fpArray[0] = -1.0;
	sobelTemp.fpArray[1] = 0.0;
	sobelTemp.fpArray[2] = 1.0;
	sobelTemp.fpArray[3] = -2.0;
	sobelTemp.fpArray[4] = 0.0;
	sobelTemp.fpArray[5] = 2.0;
	sobelTemp.fpArray[6] = -1.0;
	sobelTemp.fpArray[7] = 0.0;
	sobelTemp.fpArray[8] = 1.0;

	//����ListImage
	listTemp.SetData(pCpyImgData2,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��
	pListImage->SetData(pCpyImgData1,imgSize);


	// �ͷ��ڴ�
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// ����
	return 0;
}
int ConvFilter::PrewittUint_8 ( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned char*	pSrcImgData;

	// ָ����ͼ���ָ��
	unsigned char*	pCpyImgData1;
	// ָ����ͼ���ָ��
	unsigned char*	pCpyImgData2;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos1;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos2;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	// ����Prewittģ�����
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

	//����ListImage
	ListImage listTemp(imgW, imgH);
	listTemp.SetData(pCpyImgData1,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,PrewittTemp) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// ����Prewittģ�����
	PrewittTemp.fpArray[0] = 1.0;
	PrewittTemp.fpArray[1] = 0.0;
	PrewittTemp.fpArray[2] = -1.0;
	PrewittTemp.fpArray[3] = 1.0;
	PrewittTemp.fpArray[4] = 0.0;
	PrewittTemp.fpArray[5] = -1.0;
	PrewittTemp.fpArray[6] = 1.0;
	PrewittTemp.fpArray[7] = 0.0;
	PrewittTemp.fpArray[8] = -1.0;

	//����ListImage
	listTemp.SetData(pCpyImgData2,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,PrewittTemp) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��

	pListImage->SetData(pCpyImgData1,imgSize);
	// �ͷ��ڴ�
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// ����
	return 0;
}
int ConvFilter::KirschUint_8 ( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned char*	pSrcImgData;
	// ָ����ͼ���ָ��
	unsigned char*	pCpyImgData1;
	// ָ����ͼ���ָ��
	unsigned char*	pCpyImgData2;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos1;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos2;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	// ����Kirschģ��1����
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

	// ����TemplateConv()����
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH);
	pListNew->SetData(pCpyImgData1,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0  )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData1,pListNew->GetImgBuffer(),imgSize);

	// ����Kirschģ��2����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;


	// ����TemplateConvConv()����
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��3����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = 5.0;


	// ����TemplateConvConv()����
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];	

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2,pSrcImgData, imgSize);

	// ����Kirschģ��4����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// ����TemplateConvConv()����
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��5����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// ����TemplateConvConv()����
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);
	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��6����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = -3.0;

	// ����TemplateConv()����
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��7����
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// ����TemplateConv()����
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��8����
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// ����TemplateConv()����
	pListNew->SetData(pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��
	pListImage->SetData(pCpyImgData1, imgSize);


	// �ͷ��ڴ�
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// ����
	return 0;
}
int ConvFilter::GaussUint_8 ( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned char*	pSrcImgData;
	// ָ����ͼ���ָ��
	unsigned char*	pCpyImgData;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	// ����Gaussģ�����
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

	// ����TemplateConv()����
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH);
	pListNew->SetData(pCpyImgData,pListImage->GetImgDataSize());

	if ( TemplateConv(pListNew, m_Temp) != 0 )
	{
		return TemplateConv(pListNew, m_Temp);
	}
	pCpyImgData = pListNew->GetImgBuffer();

	// ���ƾ���ģ��������ͼ��Դͼ��
	pListImage->SetData(pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;
	return 0;
}
int ConvFilter::RobertsUint_12( ListImage *pListImage )
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int	srcPos;

	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	//����ֵ
	double result;
	unsigned short pixel[4];

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	memset(pCpyImgData, (unsigned short)4095, imgSize );


	//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
	for(j = imgH -1; j > 0; j--)
	{
		for(i = 0;i <imgW-1; i++)
		{
			//����ʹ��2��2��ģ�壬Ϊ��ֹԽ�磬���Բ��������±ߺ����ұߵ���������

			// ָ��Դͼ���j�У���i�����ص�ָ��			
			srcPos =  imgW * j + i;

			// ָ��Ŀ��ͼ���j�У���i�����ص�ָ��			
			cpyPos = imgW * j + i;

			//ȡ�õ�ǰָ�봦2*2���������ֵ��ע��Ҫת��Ϊunsigned short��
			pixel[0] = (unsigned short)pSrcImgData[srcPos]  & 4095;
			pixel[1] = (unsigned short)pSrcImgData[srcPos+1]  & 4095;
			pixel[2] = (unsigned short)pSrcImgData[srcPos - imgW]  & 4095;
			pixel[3] = (unsigned short)pSrcImgData[(srcPos - imgW + 1)] & 4095;

			//����Ŀ��ͼ���еĵ�ǰ��
			result = sqrt((double)( pixel[0] - pixel[3] )*( pixel[0] - pixel[3] ) 
				+ ( pixel[1] - pixel[2] )*( pixel[1] - pixel[2] ));
			pCpyImgData[cpyPos] = (unsigned short)result;

		}
	}

	// ���Ƹ�ʴ���ͼ��
	pListImage->SetData( (unsigned char*)pCpyImgData,imgSize );

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
}
int ConvFilter::SobelUint_12( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;

	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData1;
	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData2;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos1;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos2;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	// ����Sobelģ�����
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

	//����ListImage
	ListImage listTemp(imgW, imgH,1,uint_16);
	listTemp.SetData((unsigned char*)pCpyImgData1,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// ����Sobelģ�����
	sobelTemp.fpArray[0] = -1.0;
	sobelTemp.fpArray[1] = 0.0;
	sobelTemp.fpArray[2] = 1.0;
	sobelTemp.fpArray[3] = -2.0;
	sobelTemp.fpArray[4] = 0.0;
	sobelTemp.fpArray[5] = 2.0;
	sobelTemp.fpArray[6] = -1.0;
	sobelTemp.fpArray[7] = 0.0;
	sobelTemp.fpArray[8] = 1.0;

	//����ListImage
	listTemp.SetData((unsigned char*)pCpyImgData2,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
			{
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
			}

		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��
	pListImage->SetData((unsigned char*)pCpyImgData1,imgSize);


	// �ͷ��ڴ�
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// ����
	return 0;
}
int ConvFilter::PrewittUint_12( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;

	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData1;
	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData2;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos1;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos2;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	// ����Prewittģ�����
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

	//����ListImage
	ListImage listTemp(imgW, imgH,1,uint_12);
	listTemp.SetData((unsigned char*)pCpyImgData1,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,PrewittTemp,mode) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// ����Prewittģ�����
	PrewittTemp.fpArray[0] = 1.0;
	PrewittTemp.fpArray[1] = 0.0;
	PrewittTemp.fpArray[2] = -1.0;
	PrewittTemp.fpArray[3] = 1.0;
	PrewittTemp.fpArray[4] = 0.0;
	PrewittTemp.fpArray[5] = -1.0;
	PrewittTemp.fpArray[6] = 1.0;
	PrewittTemp.fpArray[7] = 0.0;
	PrewittTemp.fpArray[8] = -1.0;

	//����ListImage
	listTemp.SetData((unsigned char*)pCpyImgData2,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,PrewittTemp,mode) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��

	pListImage->SetData((unsigned char*)pCpyImgData1,imgSize);
	// �ͷ��ڴ�
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// ����
	return 0;
}
int ConvFilter::KirschUint_12( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;
	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData1;
	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData2;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos1;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos2;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	// ����Kirschģ��1����
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

	// ����TemplateConv()����
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH,1,uint_12);
	pListNew->SetData((unsigned char*)pCpyImgData1,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0  )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData1,pListNew->GetImgBuffer(),imgSize);

	// ����Kirschģ��2����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;


	// ����TemplateConvConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��3����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = 5.0;


	// ����TemplateConvConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];	

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2,pSrcImgData, imgSize);

	// ����Kirschģ��4����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// ����TemplateConvConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��5����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// ����TemplateConvConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);
	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��6����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = -3.0;

	// ����TemplateConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��7����
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// ����TemplateConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��8����
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// ����TemplateConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			pCpyImgData1[cpyPos1] = pCpyImgData1[cpyPos1] & 4095;
			pCpyImgData2[cpyPos2] = pCpyImgData2[cpyPos2] & 4095;
			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��
	pListImage->SetData((unsigned char*)pCpyImgData1, imgSize);


	// �ͷ��ڴ�
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// ����
	return 0;
}
int ConvFilter::GaussUint_12( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;
	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	// ����Gaussģ�����
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

	// ����TemplateConv()����
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH,1,uint_12);
	pListNew->SetData((unsigned char*)pCpyImgData,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData,pListNew->GetImgBuffer(),imgSize);

	// ���ƾ���ģ��������ͼ��Դͼ��
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;
	delete pListNew;
	return 0;
}
int ConvFilter::RobertsUint_16( ListImage *pListImage)
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int	srcPos;

	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	//����ֵ
	double result;
	unsigned short pixel[4];

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	memset(pCpyImgData, (unsigned short)4095, imgSize );


	//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
	for(j = imgH -1; j > 0; j--)
	{
		for(i = 0;i <imgW-1; i++)
		{
			//����ʹ��2��2��ģ�壬Ϊ��ֹԽ�磬���Բ��������±ߺ����ұߵ���������

			// ָ��Դͼ���j�У���i�����ص�ָ��			
			srcPos =  imgW * j + i;

			// ָ��Ŀ��ͼ���j�У���i�����ص�ָ��			
			cpyPos = imgW * j + i;

			//ȡ�õ�ǰָ�봦2*2���������ֵ��ע��Ҫת��Ϊunsigned short��
			pixel[0] =(unsigned short)pSrcImgData[srcPos] ;
			pixel[1] = (unsigned short)pSrcImgData[srcPos+1]  ;
			pixel[2] =(unsigned short)pSrcImgData[srcPos - imgW] ;
			pixel[3] =(unsigned short)pSrcImgData[(srcPos - imgW + 1)] ;

			//����Ŀ��ͼ���еĵ�ǰ��
			result = sqrt((double)( pixel[0] - pixel[3] )*( pixel[0] - pixel[3] ) 
				+ ( pixel[1] - pixel[2] )*( pixel[1] - pixel[2] ));
			pCpyImgData[cpyPos] = (unsigned short)result;

		}
	}

	// ���Ƹ�ʴ���ͼ��
	pListImage->SetData( (unsigned char*)pCpyImgData,imgSize );

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
}
int ConvFilter::SobelUint_16( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;

	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData1;
	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData2;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos1;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos2;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	// ����Sobelģ�����
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

	//����ListImage
	ListImage listTemp(imgW, imgH,1,uint_16);
	listTemp.SetData((unsigned char*)pCpyImgData1,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// ����Sobelģ�����
	sobelTemp.fpArray[0] = -1.0;
	sobelTemp.fpArray[1] = 0.0;
	sobelTemp.fpArray[2] = 1.0;
	sobelTemp.fpArray[3] = -2.0;
	sobelTemp.fpArray[4] = 0.0;
	sobelTemp.fpArray[5] = 2.0;
	sobelTemp.fpArray[6] = -1.0;
	sobelTemp.fpArray[7] = 0.0;
	sobelTemp.fpArray[8] = 1.0;

	//����ListImage
	listTemp.SetData((unsigned char*)pCpyImgData2,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
			{
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
			}

		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��
	pListImage->SetData((unsigned char*)pCpyImgData1,imgSize);


	// �ͷ��ڴ�
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// ����
	return 0;
}
int ConvFilter::PrewittUint_16( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;

	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData1;
	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData2;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos1;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos2;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	// ����Prewittģ�����
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

	//����ListImage
	ListImage listTemp(imgW, imgH,1,uint_16);
	listTemp.SetData((unsigned char*)pCpyImgData1,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,PrewittTemp,mode) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// ����Prewittģ�����
	PrewittTemp.fpArray[0] = 1.0;
	PrewittTemp.fpArray[1] = 0.0;
	PrewittTemp.fpArray[2] = -1.0;
	PrewittTemp.fpArray[3] = 1.0;
	PrewittTemp.fpArray[4] = 0.0;
	PrewittTemp.fpArray[5] = -1.0;
	PrewittTemp.fpArray[6] = 1.0;
	PrewittTemp.fpArray[7] = 0.0;
	PrewittTemp.fpArray[8] = -1.0;

	//����ListImage
	listTemp.SetData((unsigned char*)pCpyImgData2,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,PrewittTemp,mode) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��

	pListImage->SetData((unsigned char*)pCpyImgData1,imgSize);
	// �ͷ��ڴ�
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// ����
	return 0;
}
int ConvFilter::KirschUint_16( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;
	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData1;
	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData2;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos1;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos2;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	// ����Kirschģ��1����
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

	// ����TemplateConv()����
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH,1,uint_16);
	pListNew->SetData((unsigned char*)pCpyImgData1,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0  )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData1,pListNew->GetImgBuffer(),imgSize);

	// ����Kirschģ��2����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;


	// ����TemplateConvConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��3����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = 5.0;


	// ����TemplateConvConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];	

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2,pSrcImgData, imgSize);

	// ����Kirschģ��4����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// ����TemplateConvConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��5����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// ����TemplateConvConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);
	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��6����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = -3.0;

	// ����TemplateConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��7����
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// ����TemplateConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��8����
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// ����TemplateConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��
	pListImage->SetData((unsigned char*)pCpyImgData1, imgSize);


	// �ͷ��ڴ�
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// ����
	return 0;
}
int ConvFilter::GaussUint_16( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;
	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	// ����Gaussģ�����
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

	// ����TemplateConv()����
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH,1,uint_16);
	pListNew->SetData((unsigned char*)pCpyImgData,pListImage->GetImgDataSize());

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData,pListNew->GetImgBuffer(),imgSize);

	// ���ƾ���ģ��������ͼ��Դͼ��
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;
	return 0;
}


int ConvFilter::RobertsSingle( ListImage *pListImage )
{
	// ָ��Դͼ���ָ��
	unsigned int*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int	srcPos;

	// ָ����ͼ���ָ��
	unsigned int*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	//����ֵ
	double result;
	double pixel[4];

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	memset(pCpyImgData, (unsigned int)4095, imgSize );


	//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
	for(j = imgH -1; j > 0; j--)
	{
		for(i = 0;i <imgW-1; i++)
		{
			//����ʹ��2��2��ģ�壬Ϊ��ֹԽ�磬���Բ��������±ߺ����ұߵ���������

			// ָ��Դͼ���j�У���i�����ص�ָ��			
			srcPos =  imgW * j + i;

			// ָ��Ŀ��ͼ���j�У���i�����ص�ָ��			
			cpyPos = imgW * j + i;

			//ȡ�õ�ǰָ�봦2*2���������ֵ��ע��Ҫת��Ϊunsigned int��
			pixel[0] = (double)pSrcImgData[srcPos] ;
			pixel[1] = (double)pSrcImgData[srcPos+1]  ;
			pixel[2] = (double)pSrcImgData[srcPos - imgW] ;
			pixel[3] = (double)pSrcImgData[(srcPos - imgW + 1)] ;

			//����Ŀ��ͼ���еĵ�ǰ��
			result = sqrt(( pixel[0] - pixel[3] )*( pixel[0] - pixel[3] ) 
				+ ( pixel[1] - pixel[2] )*( pixel[1] - pixel[2] ));
			pCpyImgData[cpyPos] = (unsigned int)result;

		}
	}

	// ���Ƹ�ʴ���ͼ��
	pListImage->SetData( (unsigned char*)pCpyImgData,imgSize );

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
}
int ConvFilter::SobelSingle( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned int*	pSrcImgData;

	// ָ����ͼ���ָ��
	unsigned int*	pCpyImgData1;
	// ָ����ͼ���ָ��
	unsigned int*	pCpyImgData2;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos1;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos2;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	// ����Sobelģ�����
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

	//����ListImage
	ListImage listTemp(imgW, imgH,1,single);
	listTemp.SetData((unsigned char*)pCpyImgData1,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// ����Sobelģ�����
	sobelTemp.fpArray[0] = -1.0;
	sobelTemp.fpArray[1] = 0.0;
	sobelTemp.fpArray[2] = 1.0;
	sobelTemp.fpArray[3] = -2.0;
	sobelTemp.fpArray[4] = 0.0;
	sobelTemp.fpArray[5] = 2.0;
	sobelTemp.fpArray[6] = -1.0;
	sobelTemp.fpArray[7] = 0.0;
	sobelTemp.fpArray[8] = 1.0;

	//����ListImage
	listTemp.SetData((unsigned char*)pCpyImgData2,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,sobelTemp,mode) != 0 )
	{
		return TemplateConv(&listTemp,sobelTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
			{
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
			}
		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��
	pListImage->SetData((unsigned char*)pCpyImgData1,imgSize);


	// �ͷ��ڴ�
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// ����
	return 0;
}
int ConvFilter::PrewittSingle( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned int*	pSrcImgData;

	// ָ����ͼ���ָ��
	unsigned int*	pCpyImgData1;
	// ָ����ͼ���ָ��
	unsigned int*	pCpyImgData2;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos1;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos2;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	// ����Prewittģ�����
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

	//����ListImage
	ListImage listTemp(imgW, imgH,1,single);
	listTemp.SetData((unsigned char*)pCpyImgData1,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,PrewittTemp,mode) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp,mode);
	}
	memcpy(pCpyImgData1,listTemp.GetImgBuffer(),imgSize);

	// ����Prewittģ�����
	PrewittTemp.fpArray[0] = 1.0;
	PrewittTemp.fpArray[1] = 0.0;
	PrewittTemp.fpArray[2] = -1.0;
	PrewittTemp.fpArray[3] = 1.0;
	PrewittTemp.fpArray[4] = 0.0;
	PrewittTemp.fpArray[5] = -1.0;
	PrewittTemp.fpArray[6] = 1.0;
	PrewittTemp.fpArray[7] = 0.0;
	PrewittTemp.fpArray[8] = -1.0;

	//����ListImage
	listTemp.SetData((unsigned char*)pCpyImgData2,imgSize);

	// ����Template()����
	if ( TemplateConv(&listTemp,PrewittTemp,mode) != 0)
	{
		return TemplateConv(&listTemp,PrewittTemp,mode);
	}
	memcpy(pCpyImgData2,listTemp.GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��

	pListImage->SetData((unsigned char*)pCpyImgData1,imgSize);
	// �ͷ��ڴ�
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// ����
	return 0;
}
int ConvFilter::KirschSingle( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned int*	pSrcImgData;
	// ָ����ͼ���ָ��
	unsigned int*	pCpyImgData1;
	// ָ����ͼ���ָ��
	unsigned int*	pCpyImgData2;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos1;
	// ָ����ͼ��������ص�ָ��
	int	cpyPos2;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	//ѭ������
	long i;
	long j;

	// ����Kirschģ��1����
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

	// ����TemplateConv()����
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH,1,single);
	pListNew->SetData((unsigned char*)pCpyImgData1,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0  )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData1,pListNew->GetImgBuffer(),imgSize);

	// ����Kirschģ��2����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;


	// ����TemplateConvConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��3����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = 5.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = 5.0;


	// ����TemplateConvConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];	

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2,pSrcImgData, imgSize);

	// ����Kirschģ��4����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = 5.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// ����TemplateConvConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��5����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = -3.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = 5.0;

	// ����TemplateConvConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);
	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��6����
	m_Temp.fpArray[0] = -3.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = 5.0;
	m_Temp.fpArray[8] = -3.0;

	// ����TemplateConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��7����
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = -3.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = 5.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// ����TemplateConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(pCpyImgData2, pSrcImgData, imgSize);

	// ����Kirschģ��8����
	m_Temp.fpArray[0] = 5.0;
	m_Temp.fpArray[1] = 5.0;
	m_Temp.fpArray[2] = -3.0;
	m_Temp.fpArray[3] = 5.0;
	m_Temp.fpArray[4] = 0.0;
	m_Temp.fpArray[5] = -3.0;
	m_Temp.fpArray[6] = -3.0;
	m_Temp.fpArray[7] = -3.0;
	m_Temp.fpArray[8] = -3.0;

	// ����TemplateConv()����
	pListNew->SetData((unsigned char*)pCpyImgData2,imgSize);

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData2,pListNew->GetImgBuffer(),imgSize);

	//����������ͼ������ֵ
	for (j = 0; j <imgH; j++)
	{
		for (i = 0; i <imgW - 1; i++)
		{
			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			cpyPos1 = imgW * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			cpyPos2 = imgW * j + i;

			if (pCpyImgData2[cpyPos2] > pCpyImgData1[cpyPos1])
				pCpyImgData1[cpyPos1] = pCpyImgData2[cpyPos2];
		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��
	pListImage->SetData((unsigned char*)pCpyImgData1, imgSize);


	// �ͷ��ڴ�
	delete []pCpyImgData1;
	delete []pCpyImgData2;
	// ����
	return 0;
}
int ConvFilter::GaussSingle( ListImage *pListImage, int mode )
{
	// ָ��Դͼ���ָ��
	unsigned int*	pSrcImgData;
	// ָ����ͼ���ָ��
	unsigned int*	pCpyImgData;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
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

	// ����Gaussģ�����
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

	// ����TemplateConv()����
	ListImage *pListNew;
	pListNew = new ListImage(imgW,imgH,1,single);
	pListNew->SetData((unsigned char*)pCpyImgData,pListImage->GetImgDataSize());

	if ( TemplateConv(pListNew, m_Temp,mode) != 0 )
	{
		return TemplateConv(pListNew, m_Temp,mode);
	}
	memcpy(pCpyImgData,pListNew->GetImgBuffer(),imgSize);

	// ���ƾ���ģ��������ͼ��Դͼ��
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// �ͷ��ڴ�
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
	//��ʱ����
	int i;
	int j;
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	unsigned char* pSrcImgData = (unsigned char*)pListImage->GetImgBuffer();
	// ����x����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{

			pGradX[i*imgW+j] = (long) ( pSrcImgData[i*imgW+min(imgW-1,j+1)]  
			- pSrcImgData[i*imgW+max(0,j-1)]     );
		}
	}
	// ����y����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
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
	//��ʱ����
	int i;
	int j;
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	unsigned short* pSrcImgData = (unsigned short*)pListImage->GetImgBuffer();
	// ����x����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{

			pGradX[i*imgW+j] = (long) ( pSrcImgData[i*imgW+min(imgW-1,j+1)]  
			- pSrcImgData[i*imgW+max(0,j-1)]     );
		}
	}
	// ����y����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
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
	//��ʱ����
	int i;
	int j;
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	unsigned short* pSrcImgData = (unsigned short*)pListImage->GetImgBuffer();
	// ����x����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{

			pGradX[i*imgW+j] = (long) ( pSrcImgData[i*imgW+min(imgW-1,j+1)]  
			- pSrcImgData[i*imgW+max(0,j-1)]     );
		}
	}
	// ����y����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
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
	//��ʱ����
	int i;
	int j;
	int imgH = pListImage->GetImgHeight();
	int imgW = pListImage->GetImgWidth();
	unsigned int* pSrcImgData = (unsigned int*)pListImage->GetImgBuffer();
	// ����x����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{

			pGradX[i*imgW+j] = (long) ( pSrcImgData[i*imgW+min(imgW-1,j+1)]  
			- pSrcImgData[i*imgW+max(0,j-1)]     );
			int x = pGradX[i*imgW+j];
		}
	}
	// ����y����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
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
	// �м����
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

	// ѭ�����Ʊ���
	int pos;

	// x�����ݶȷ���
	long gx  ;
	long gy  ;

	// ��ʱ����
	long g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;

	int i;
	int j;
	// ����ͼ���Ե����Ϊ�����ܵı߽��
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

			// �����ǰ���ص��ݶȷ���Ϊ0�����Ǳ߽��
			if(pGradRange[pos] == 0 )
			{
				pImgEdge[pos] = 0 ;
			}
			else
			{
				// ��ǰ���ص��ݶȷ���
				dTmp = pGradRange[pos] ;

				// x��y������
				gx = pGradX[pos]  ;
				gy = pGradY[pos]  ;

				// ���������y������x������˵�������ķ�����ӡ�������y������
				if (abs(gy) > abs(gx)) 
				{
					// �����ֵ�ı���
					weight = fabs((double)gx)/fabs((double)gy); 

					g2 = pGradRange[pos-imgW] ; 
					g4 = pGradRange[pos+imgW] ;

					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pGradRange[pos-imgW-1] ;
						g3 = pGradRange[pos+imgW+1] ;
					} 

					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	   g2 g1
					//		C         
					//	g3 g4  
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					} 
				}

				// ���������x������y������˵�������ķ�����ӡ�������x����
				// ����ж���������x������y������ȵ����
				else
				{
					// �����ֵ�ı���
					weight = fabs((double)gy)/fabs((double)gx);  

					g2 = pGradRange[pos+1] ; 
					g4 = pGradRange[pos-1] ;

					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pGradRange[pos+imgW+1] ;
						g3 = pGradRange[pos-imgW-1] ;
					} 
					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					}
				}

				// ��������g1-g4���ݶȽ��в�ֵ
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;

					// ��ǰ���ص��ݶ��Ǿֲ������ֵ
					// �õ�����Ǹ��߽��
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pImgEdge[pos] = 128 ;
					}
					else
					{
						// �������Ǳ߽��
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

	// ѭ�����Ʊ���
	int pos;

	// x�����ݶȷ���
	long gx  ;
	long gy  ;

	// ��ʱ����
	long g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;

	int i;
	int j;
	// ����ͼ���Ե����Ϊ�����ܵı߽��
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

			// �����ǰ���ص��ݶȷ���Ϊ0�����Ǳ߽��
			if(pGradRange[pos] == 0 )
			{
				pImgEdge[pos] = 0 ;
			}
			else
			{
				// ��ǰ���ص��ݶȷ���
				dTmp = pGradRange[pos] ;

				// x��y������
				gx = pGradX[pos]  ;
				gy = pGradY[pos]  ;

				// ���������y������x������˵�������ķ�����ӡ�������y������
				if (abs(gy) > abs(gx)) 
				{
					// �����ֵ�ı���
					weight = fabs((double)gx)/fabs((double)gy); 

					g2 = pGradRange[pos-imgW] ; 
					g4 = pGradRange[pos+imgW] ;

					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pGradRange[pos-imgW-1] ;
						g3 = pGradRange[pos+imgW+1] ;
					} 

					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	   g2 g1
					//		C         
					//	g3 g4  
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					} 
				}

				// ���������x������y������˵�������ķ�����ӡ�������x����
				// ����ж���������x������y������ȵ����
				else
				{
					// �����ֵ�ı���
					weight = fabs((double)gy)/fabs((double)gx);  

					g2 = pGradRange[pos+1] ; 
					g4 = pGradRange[pos-1] ;

					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pGradRange[pos+imgW+1] ;
						g3 = pGradRange[pos-imgW-1] ;
					} 
					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					}
				}

				// ��������g1-g4���ݶȽ��в�ֵ
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;

					// ��ǰ���ص��ݶ��Ǿֲ������ֵ
					// �õ�����Ǹ��߽��
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pImgEdge[pos] = 128 ;
					}
					else
					{
						// �������Ǳ߽��
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

	// ѭ�����Ʊ���
	int pos;

	// x�����ݶȷ���
	long gx  ;
	long gy  ;

	// ��ʱ����
	long g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;

	int i;
	int j;
	// ����ͼ���Ե����Ϊ�����ܵı߽��
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

			// �����ǰ���ص��ݶȷ���Ϊ0�����Ǳ߽��
			if(pGradRange[pos] == 0 )
			{
				pImgEdge[pos] = 0 ;
			}
			else
			{
				// ��ǰ���ص��ݶȷ���
				dTmp = pGradRange[pos] ;

				// x��y������
				gx = pGradX[pos]  ;
				gy = pGradY[pos]  ;

				// ���������y������x������˵�������ķ�����ӡ�������y������
				if (abs(gy) > abs(gx)) 
				{
					// �����ֵ�ı���
					weight = fabs((double)gx)/fabs((double)gy); 

					g2 = pGradRange[pos-imgW] ; 
					g4 = pGradRange[pos+imgW] ;

					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pGradRange[pos-imgW-1] ;
						g3 = pGradRange[pos+imgW+1] ;
					} 

					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	   g2 g1
					//		C         
					//	g3 g4  
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					} 
				}

				// ���������x������y������˵�������ķ�����ӡ�������x����
				// ����ж���������x������y������ȵ����
				else
				{
					// �����ֵ�ı���
					weight = fabs((double)gy)/fabs((double)gx);  

					g2 = pGradRange[pos+1] ; 
					g4 = pGradRange[pos-1] ;

					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pGradRange[pos+imgW+1] ;
						g3 = pGradRange[pos-imgW-1] ;
					} 
					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					}
				}

				// ��������g1-g4���ݶȽ��в�ֵ
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;

					// ��ǰ���ص��ݶ��Ǿֲ������ֵ
					// �õ�����Ǹ��߽��
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pImgEdge[pos] = 32768 ;
					}
					else
					{
						// �������Ǳ߽��
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

	// ѭ�����Ʊ���
	int pos;

	// x�����ݶȷ���
	long gx  ;
	long gy  ;

	// ��ʱ����
	long g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;

	int i;
	int j;
	// ����ͼ���Ե����Ϊ�����ܵı߽��
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

			// �����ǰ���ص��ݶȷ���Ϊ0�����Ǳ߽��
			if(pGradRange[pos] == 0 )
			{
				pImgEdge[pos] = 0 ;
			}
			else
			{
				// ��ǰ���ص��ݶȷ���
				dTmp = pGradRange[pos] ;

				// x��y������
				gx = pGradX[pos]  ;
				gy = pGradY[pos]  ;

				// ���������y������x������˵�������ķ�����ӡ�������y������
				if (abs(gy) > abs(gx)) 
				{
					// �����ֵ�ı���
					weight = fabs((double)gx)/fabs((double)gy); 

					g2 = pGradRange[pos-imgW] ; 
					g4 = pGradRange[pos+imgW] ;

					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pGradRange[pos-imgW-1] ;
						g3 = pGradRange[pos+imgW+1] ;
					} 

					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	   g2 g1
					//		C         
					//	g3 g4  
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					} 
				}

				// ���������x������y������˵�������ķ�����ӡ�������x����
				// ����ж���������x������y������ȵ����
				else
				{
					// �����ֵ�ı���
					weight = fabs((double)gy)/fabs((double)gx);  

					g2 = pGradRange[pos+1] ; 
					g4 = pGradRange[pos-1] ;

					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pGradRange[pos+imgW+1] ;
						g3 = pGradRange[pos-imgW-1] ;
					} 
					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pGradRange[pos-imgW+1] ;
						g3 = pGradRange[pos+imgW-1] ;
					}
				}

				// ��������g1-g4���ݶȽ��в�ֵ
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;

					// ��ǰ���ص��ݶ��Ǿֲ������ֵ
					// �õ�����Ǹ��߽��
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pImgEdge[pos] = 2147483648 ;
					}
					else
					{
						// �������Ǳ߽��
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

	// ����TraceEdge��Ҫ�ĵ���ֵ���Լ�Hysteresis����ʹ�õĸ���ֵ
	EstimateThreshold(pListImage,&nThdHigh, &nThdLow,dRatioHigh, dRatioLow,pGradRange);

	// ���ѭ������Ѱ�Ҵ���nThdHigh�ĵ㣬��Щ�㱻���������߽�㣬Ȼ����
	// TraceEdge���������ٸõ��Ӧ�ı߽�
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 

			// ����������ǿ��ܵı߽�㣬�����ݶȴ��ڸ���ֵ����������Ϊ
			// һ���߽�����
			if((pImgEdge[pos] == 128) && (pGradRange[pos] >= nThdHigh))
			{
				// ���øõ�Ϊ�߽��
				pImgEdge[pos] = 255;
				TraceEdge(pListImage,i, j, nThdLow, pGradRange);
			}
		}
	}

	// ��Щ��û�б�����Ϊ�߽��������Ѿ������ܳ�Ϊ�߽��
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 
			if(pImgEdge[pos] != 255)
			{
				// ����Ϊ�Ǳ߽��
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

	// ����TraceEdge��Ҫ�ĵ���ֵ���Լ�Hysteresis����ʹ�õĸ���ֵ
	EstimateThreshold(pListImage,&nThdHigh, &nThdLow,dRatioHigh, dRatioLow,pGradRange);


	// ���ѭ������Ѱ�Ҵ���nThdHigh�ĵ㣬��Щ�㱻���������߽�㣬Ȼ����
	// TraceEdge���������ٸõ��Ӧ�ı߽�
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 

			// ����������ǿ��ܵı߽�㣬�����ݶȴ��ڸ���ֵ����������Ϊ
			// һ���߽�����
			if((pImgEdge[pos] == 128) && (pGradRange[pos] >= nThdHigh))
			{
				// ���øõ�Ϊ�߽��
				pImgEdge[pos] = 4095;
				TraceEdge(pListImage,i, j, nThdLow, pGradRange);
			}
		}
	}

	// ��Щ��û�б�����Ϊ�߽��������Ѿ������ܳ�Ϊ�߽��
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 
			if(pImgEdge[pos] != 4095)
			{
				// ����Ϊ�Ǳ߽��
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

	// ����TraceEdge��Ҫ�ĵ���ֵ���Լ�Hysteresis����ʹ�õĸ���ֵ
	EstimateThreshold(pListImage,&nThdHigh, &nThdLow,dRatioHigh, dRatioLow,pGradRange);


	// ���ѭ������Ѱ�Ҵ���nThdHigh�ĵ㣬��Щ�㱻���������߽�㣬Ȼ����
	// TraceEdge���������ٸõ��Ӧ�ı߽�
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 

			// ����������ǿ��ܵı߽�㣬�����ݶȴ��ڸ���ֵ����������Ϊ
			// һ���߽�����
			if((pImgEdge[pos] == 32768) && (pGradRange[pos] >= nThdHigh))
			{
				// ���øõ�Ϊ�߽��
				pImgEdge[pos] = 65535;
				TraceEdge(pListImage,i, j, nThdLow, pGradRange);
			}
		}
	}

	// ��Щ��û�б�����Ϊ�߽��������Ѿ������ܳ�Ϊ�߽��
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 
			if(pImgEdge[pos] != 65535)
			{
				// ����Ϊ�Ǳ߽��
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

	// ����TraceEdge��Ҫ�ĵ���ֵ���Լ�Hysteresis����ʹ�õĸ���ֵ
	EstimateThreshold(pListImage,&nThdHigh, &nThdLow,dRatioHigh, dRatioLow,pGradRange);


	// ���ѭ������Ѱ�Ҵ���nThdHigh�ĵ㣬��Щ�㱻���������߽�㣬Ȼ����
	// TraceEdge���������ٸõ��Ӧ�ı߽�
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 

			// ����������ǿ��ܵı߽�㣬�����ݶȴ��ڸ���ֵ����������Ϊ
			// һ���߽�����
			if((pImgEdge[pos] == 2147483648) && (pGradRange[pos] >= nThdHigh))
			{
				// ���øõ�Ϊ�߽��
				pImgEdge[pos] = 4294967295;
				//TraceEdge(pListImage,i, j, nThdLow, pGradRange);
			}
		}
	}

	// ��Щ��û�б�����Ϊ�߽��������Ѿ������ܳ�Ϊ�߽��
	for(i=0; i<imgH; i++)
	{
		for(j=0; j<imgW; j++)
		{
			pos = i*imgW + j ; 
			if(pImgEdge[pos] != 4294967295)
			{
				// ����Ϊ�Ǳ߽��
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
	// ѭ�����Ʊ���
	int y;
	int x;
	int k;


	int nHist[1024] ;

	// ���ܵı߽���Ŀ
	int nEdgeNb     ;

	// ����ݶ�ֵ
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 

	// ��ʼ��
	for(k=0; k<1024; k++) 
	{
		nHist[k] = 0; 
	}

	// ͳ��ֱ��ͼ��Ȼ������ֱ��ͼ������ֵ
	for(y=0; y<imgH; y++)
	{
		for(x=0; x<imgW; x++)
		{
			// ֻ��ͳ����Щ�����Ǳ߽�㣬���һ�û�д����������
			if(pImgEdge[y*imgW+x]==128)
			{
				nHist[ pGradRange[y*imgW+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// ͳ�ƾ����������ֵ��ֹ(non-maximum suppression)�����ж�������
	for(k=1; k<1024; k++)
	{
		if(nHist[k] != 0)
		{
			// ����ݶ�ֵ
			nMaxMag = k;
		}

		// �ݶ�Ϊ0�ĵ��ǲ�����Ϊ�߽���
		// ����non-maximum suppression���ж�������
		nEdgeNb += nHist[k];
	}

	// �ݶȱȸ���ֵ*pThdHighС�����ص�����Ŀ
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);

	k = 1;
	nEdgeNb = nHist[1];

	// �������ֵ
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// ���ø���ֵ
	*pThdHigh = k ;

	// ���õ���ֵ
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
	// ѭ�����Ʊ���
	int y;
	int x;
	int k;


	int nHist[100000] ;

	// ���ܵı߽���Ŀ
	int nEdgeNb     ;

	// ����ݶ�ֵ
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 

	// ��ʼ��
	for(k=0; k<100000; k++) 
	{
		nHist[k] = 0; 
	}

	// ͳ��ֱ��ͼ��Ȼ������ֱ��ͼ������ֵ
	for(y=0; y<imgH; y++)
	{
		for(x=0; x<imgW; x++)
		{
			// ֻ��ͳ����Щ�����Ǳ߽�㣬���һ�û�д����������
			if(pImgEdge[y*imgW+x]==128)
			{
				nHist[ pGradRange[y*imgW+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// ͳ�ƾ����������ֵ��ֹ(non-maximum suppression)�����ж�������
	for(k=1; k<100000; k++)
	{
		if(nHist[k] != 0)
		{
			// ����ݶ�ֵ
			nMaxMag = k;
		}

		// �ݶ�Ϊ0�ĵ��ǲ�����Ϊ�߽���
		// ����non-maximum suppression���ж�������
		nEdgeNb += nHist[k];
	}

	// �ݶȱȸ���ֵ*pThdHighС�����ص�����Ŀ
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);

	k = 1;
	nEdgeNb = nHist[1];

	// �������ֵ
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// ���ø���ֵ
	*pThdHigh = k ;

	// ���õ���ֵ
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
	// ѭ�����Ʊ���
	int y;
	int x;
	int k;


	int nHist[100000] ;

	// ���ܵı߽���Ŀ
	int nEdgeNb     ;

	// ����ݶ�ֵ
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 

	// ��ʼ��
	for(k=0; k<100000; k++) 
	{
		nHist[k] = 0; 
	}

	// ͳ��ֱ��ͼ��Ȼ������ֱ��ͼ������ֵ
	for(y=0; y<imgH; y++)
	{
		for(x=0; x<imgW; x++)
		{
			// ֻ��ͳ����Щ�����Ǳ߽�㣬���һ�û�д����������
			if(pImgEdge[y*imgW+x]==32768)
			{
				nHist[ pGradRange[y*imgW+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// ͳ�ƾ����������ֵ��ֹ(non-maximum suppression)�����ж�������
	for(k=1; k<100000; k++)
	{
		if(nHist[k] != 0)
		{
			// ����ݶ�ֵ
			nMaxMag = k;
		}

		// �ݶ�Ϊ0�ĵ��ǲ�����Ϊ�߽���
		// ����non-maximum suppression���ж�������
		nEdgeNb += nHist[k];
	}

	// �ݶȱȸ���ֵ*pThdHighС�����ص�����Ŀ
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);

	k = 1;
	nEdgeNb = nHist[1];

	// �������ֵ
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// ���ø���ֵ
	*pThdHigh = k ;

	// ���õ���ֵ
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
	// ѭ�����Ʊ���
	int y;
	int x;
	int k;

	// ���ܵı߽���Ŀ
	int nEdgeNb     ;

	// ����ݶ�ֵ
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0; 
	nHighCount = 0;

	//ͳ��ֱ��ͼ
	int nHist[70000] ;

	// ��ʼ��
	for(k=0; k<70000; k++) 
	{
		nHist[k] = 0; 
	}

	// ͳ��ֱ��ͼ��Ȼ������ֱ��ͼ������ֵ
	for(y=0; y<imgH; y++)
	{
		for(x=0; x<imgW; x++)
		{
			// ֻ��ͳ����Щ�����Ǳ߽�㣬���һ�û�д����������
			if(pImgEdge[y*imgW+x]==2147483648)
			{
				nHist[ pGradRange[y*imgW+x]/100000 ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// ͳ�ƾ����������ֵ��ֹ(non-maximum suppression)�����ж�������
	for(k=1; k<70000; k++)
	{
		if(nHist[k] != 0)
		{
			// ����ݶ�ֵ
			nMaxMag = k * 100000;
		}

		// �ݶ�Ϊ0�ĵ��ǲ�����Ϊ�߽���
		// ����non-maximum suppression���ж�������
		nEdgeNb += nHist[k];
	}

	// �ݶȱȸ���ֵ*pThdHighС�����ص�����Ŀ
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);

	k = 1;
	nEdgeNb = nHist[1];

	// �������ֵ
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}


	// ���ø���ֵ
	*pThdHigh = k * 100000;

	// ���õ���ֵ
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

	// ��8�������ؽ��в�ѯ

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
		// ���������Ϊ���ܵı߽�㣬��û�д����
		// �����ݶȴ�����ֵ
		if(pImgEdge[yy*imgW+xx] == 128  && pGradRange[yy*imgW+xx]>=nLowThd)
		{
			// �Ѹõ����ó�Ϊ�߽��
			pImgEdge[yy*imgW+xx] = 255 ;

			// �Ըõ�Ϊ���Ľ��и���
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
	// ��8�������ؽ��в�ѯ
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;

	int k  ;

	for(k=0; k<8; k++)
	{
		yy = y + yNb[k] ;
		xx = x + xNb[k] ;
		// ���������Ϊ���ܵı߽�㣬��û�д����
		// �����ݶȴ�����ֵ
		if(pImgEdge[yy*imgW+xx] == 128  && pGradRange[yy*imgW+xx]>=nLowThd)
		{
			// �Ѹõ����ó�Ϊ�߽��
			pImgEdge[yy*imgW+xx] = 4095 ;

			// �Ըõ�Ϊ���Ľ��и���
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
	// ��8�������ؽ��в�ѯ
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;

	int k  ;

	for(k=0; k<8; k++)
	{
		yy = y + yNb[k] ;
		xx = x + xNb[k] ;
		// ���������Ϊ���ܵı߽�㣬��û�д����
		// �����ݶȴ�����ֵ
		if(pImgEdge[yy*imgW+xx] == 32768  && pGradRange[yy*imgW+xx]>=nLowThd)
		{
			// �Ѹõ����ó�Ϊ�߽��
			pImgEdge[yy*imgW+xx] = 65535 ;

			// �Ըõ�Ϊ���Ľ��и���
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
	// ��8�������ؽ��в�ѯ
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;

	int k  ;

	for(k=0; k<8; k++)
	{
		yy = y + yNb[k] ;
		xx = x + xNb[k] ;
		// ���������Ϊ���ܵı߽�㣬��û�д����
		// �����ݶȴ�����ֵ
		if(pImgEdge[yy*imgW+xx] == 2147483648  && pGradRange[yy*imgW+xx]>=nLowThd)
		{
			// �Ѹõ����ó�Ϊ�߽��
			pImgEdge[yy*imgW+xx] = 4294967295;

			// �Ըõ�Ϊ���Ľ��и���
			TraceEdge(pListImage,yy, xx, nLowThd, pGradRange);
		}
	}
	return 0;
}


//int ConvFilter::AddOrSubImgUint_8(ListImage *pSrc1,ListImage *pSrc2,ListImage *pDest,int mode ,
//								  float ratio1 ,float ratio2 )
//{
//	// ָ��Դ��Ŀ��ͼ����������ָ��
//	unsigned char*	pSrcImgData1;
//	unsigned char*	pSrcImgData2;
//	unsigned char*	pDestImgData;
//
//	// ͼ��ߡ���
//	int imgH;
//	int imgW;
//	int imgSize;
//
//	//��ʼ��
//	imgSize = pSrc1->GetImgDataSize();
//	imgH = pSrc1->GetImgHeight();
//	imgW = pSrc1->GetImgWidth();
//	pSrcImgData1 = pSrc1->GetImgBuffer(); 
//	pSrcImgData2 = pSrc2->GetImgBuffer(); 
//	pDestImgData = pDest->GetImgBuffer(); 
//
//	// ѭ������
//	long	i;
//	long	j;
//	long   k;
//	int	temp;
//
//	if(mode == 0)
//	{
//		// ÿ��(��ȥ��Եһ�У�
//		for(i = 0; i < imgH  ; i++)
//		{
//			// ÿ��(��ȥ��Եһ�У�
//			for(j = 0; j < imgW  ; j++)
//			{
//				// ָ��ͼ���i�У���j�����ص�ָ��
//				k = imgW * i + j;
//
//				//���㲢��ֵ
//				temp = pSrcImgData1[k]*ratio1 + pSrcImgData2[k]*ratio2;
//
//				// �ж��Ƿ�С����ֵ
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
//		// ÿ��(��ȥ��Եһ�У�
//		for(i = 0; i < imgH  ; i++)
//		{
//			// ÿ��(��ȥ��Եһ�У�
//			for(j = 0; j < imgW  ; j++)
//			{
//				// ָ��ͼ���i�У���j�����ص�ָ��
//				k = imgW * i + j;
//
//				//���㲢��ֵ
//				temp = pSrcImgData1[k]*ratio1 - pSrcImgData2[k]*ratio2;
//
//				// �ж��Ƿ�С����ֵ
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
//	// ָ��Դ��Ŀ��ͼ����������ָ��
//	unsigned short*	pSrcImgData1;
//	unsigned short*	pSrcImgData2;
//	unsigned short*	pDestImgData;
//
//	// ͼ��ߡ���
//	int imgH;
//	int imgW;
//	int imgSize;
//
//	//��ʼ��
//	imgSize = pSrc1->GetImgDataSize();
//	imgH = pSrc1->GetImgHeight();
//	imgW = pSrc1->GetImgWidth();
//	pSrcImgData1 = (unsigned short*)pSrc1->GetImgBuffer(); 
//	pSrcImgData2 = (unsigned short*)pSrc2->GetImgBuffer(); 
//	pDestImgData = (unsigned short*)pDest->GetImgBuffer(); 
//
//	// ѭ������
//	long	i;
//	long	j;
//	long   k;
//	int	temp;
//
//	if(mode == 0)
//	{
//		// ÿ��(��ȥ��Եһ�У�
//		for(i = 0; i < imgH  ; i++)
//		{
//			// ÿ��(��ȥ��Եһ�У�
//			for(j = 0; j < imgW  ; j++)
//			{
//				// ָ��ͼ���i�У���j�����ص�ָ��
//				k = imgW * i + j;
//
//				//���㲢��ֵ
//				temp = pSrcImgData1[k]*ratio1 + pSrcImgData2[k]*ratio2;
//
//				// �ж��Ƿ�С����ֵ
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
//		// ÿ��(��ȥ��Եһ�У�
//		for(i = 0; i < imgH  ; i++)
//		{
//			// ÿ��(��ȥ��Եһ�У�
//			for(j = 0; j < imgW  ; j++)
//			{
//				// ָ��ͼ���i�У���j�����ص�ָ��
//				k = imgW * i + j;
//
//				//���㲢��ֵ
//				temp = pSrcImgData1[k]*ratio1 - pSrcImgData2[k]*ratio2;
//
//				// �ж��Ƿ�С����ֵ
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
//	// ָ��Դ��Ŀ��ͼ����������ָ��
//	unsigned short*	pSrcImgData1;
//	unsigned short*	pSrcImgData2;
//	unsigned short*	pDestImgData;
//
//	// ͼ��ߡ���
//	int imgH;
//	int imgW;
//	int imgSize;
//
//	//��ʼ��
//	imgSize = pSrc1->GetImgDataSize();
//	imgH = pSrc1->GetImgHeight();
//	imgW = pSrc1->GetImgWidth();
//	pSrcImgData1 = (unsigned short*)pSrc1->GetImgBuffer(); 
//	pSrcImgData2 = (unsigned short*) pSrc2->GetImgBuffer(); 
//	pDestImgData = (unsigned short*)pDest->GetImgBuffer(); 
//
//	// ѭ������
//	long	i;
//	long	j;
//	long   k;
//	int	temp;
//
//	if(mode == 0)
//	{
//		// ÿ��(��ȥ��Եһ�У�
//		for(i = 0; i < imgH  ; i++)
//		{
//			// ÿ��(��ȥ��Եһ�У�
//			for(j = 0; j < imgW  ; j++)
//			{
//				// ָ��ͼ���i�У���j�����ص�ָ��
//				k = imgW * i + j;
//
//				//���㲢��ֵ
//				temp = pSrcImgData1[k]*ratio1 + pSrcImgData2[k]*ratio2;
//
//				// �ж��Ƿ�С����ֵ
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
//		// ÿ��(��ȥ��Եһ�У�
//		for(i = 0; i < imgH  ; i++)
//		{
//			// ÿ��(��ȥ��Եһ�У�
//			for(j = 0; j < imgW  ; j++)
//			{
//				// ָ��ͼ���i�У���j�����ص�ָ��
//				k = imgW * i + j;
//
//				//���㲢��ֵ
//				temp = pSrcImgData1[k]*ratio1 - pSrcImgData2[k]*ratio2;
//
//				// �ж��Ƿ�С����ֵ
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
//	// ָ��Դ��Ŀ��ͼ����������ָ��
//	unsigned int*	pSrcImgData1;
//	unsigned int*	pSrcImgData2;
//	unsigned int*	pDestImgData;
//
//	// ͼ��ߡ���
//	int imgH;
//	int imgW;
//
//	//��ʼ��
//	imgH = pSrc1->GetImgHeight();
//	imgW = pSrc1->GetImgWidth();
//	pSrcImgData1 = (unsigned int*)pSrc1->GetImgBuffer(); 
//	pSrcImgData2 = (unsigned int*)pSrc2->GetImgBuffer(); 
//	pDestImgData = (unsigned int*)pDest->GetImgBuffer(); 
//
//	// ѭ������
//	long	i;
//	long	j;
//	long   k;
//	long long	temp;
//
//	if(mode == 0)
//	{
//		// ÿ��(��ȥ��Եһ�У�
//		for(i = 0; i < imgH  ; i++)
//		{
//			// ÿ��(��ȥ��Եһ�У�
//			for(j = 0; j < imgW  ; j++)
//			{
//				// ָ��ͼ���i�У���j�����ص�ָ��
//				k = imgW * i + j;
//
//				//���㲢��ֵ
//				temp = pSrcImgData1[k]*ratio1 + pSrcImgData2[k]*ratio2;
//
//				// �ж��Ƿ�С����ֵ
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
//		// ÿ��(��ȥ��Եһ�У�
//		for(i = 0; i < imgH  ; i++)
//		{
//			// ÿ��(��ȥ��Եһ�У�
//			for(j = 0; j < imgW  ; j++)
//			{
//				// ָ��ͼ���i�У���j�����ص�ָ��
//				k = imgW * i + j;
//
//				//���㲢��ֵ
//				temp = pSrcImgData1[k]*ratio1 - pSrcImgData2[k]*ratio2;
//
//				// �ж��Ƿ�С����ֵ
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
	// ָ��Դ��Ŀ��ͼ����������ָ��
	unsigned short*	pSrcImgData1;
	unsigned short*	pSrcImgData2;
	int*	pDestImgData;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
	imgSize = pSrc1->GetImgDataSize();
	imgH = pSrc1->GetImgHeight();
	imgW = pSrc1->GetImgWidth();
	pSrcImgData1 = (unsigned short*)pSrc1->GetImgBuffer(); 
	pSrcImgData2 = (unsigned short*) pSrc2->GetImgBuffer(); 
	pDestImgData = pDest; 

	// ѭ������
	long	i;
	long	j;
	long   k;
	int	temp;


	// ÿ��(��ȥ��Եһ�У�
	for(i = 0; i < imgH  ; i++)
	{
		// ÿ��(��ȥ��Եһ�У�
		for(j = 0; j < imgW  ; j++)
		{
			// ָ��ͼ���i�У���j�����ص�ָ��
			k = imgW * i + j;

			//���㲢��ֵ
			temp = pSrcImgData1[k]*ratio1 - pSrcImgData2[k]*ratio2;

			// �ж��Ƿ�С����ֵ
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
	// ָ��Դ��Ŀ��ͼ����������ָ��
	unsigned short*	pSrcImgData1;
	int*	pSrcImgData2;
	unsigned short*	pDestImgData;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	//��ʼ��
	imgSize = pSrc1->GetImgDataSize();
	imgH = pSrc1->GetImgHeight();
	imgW = pSrc1->GetImgWidth();
	pSrcImgData1 = (unsigned short*)pSrc1->GetImgBuffer(); 
	pSrcImgData2 = pSrc2; 
	pDestImgData = (unsigned short*) pDest->GetImgBuffer(); 

	// ѭ������
	long	i;
	long	j;
	long   k;
	int	temp;


	// ÿ��(��ȥ��Եһ�У�
	for(i = 0; i < imgH  ; i++)
	{
		// ÿ��(��ȥ��Եһ�У�
		for(j = 0; j < imgW  ; j++)
		{
			// ָ��ͼ���i�У���j�����ص�ָ��
			k = imgW * i + j;

			//���㲢��ֵ
			temp = pSrcImgData1[k]*ratio1 - pSrcImgData2[k]*ratio2;

			// �ж��Ƿ�С����ֵ
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