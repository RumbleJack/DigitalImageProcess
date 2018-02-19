#include "stdafx.h"

//#include "DIBAPI.h"

#include <math.h>
#include <direct.h>
#include "PointTrans.h"

int  PointTrans::ThresholdTrans8(ListImage *pListImage,unsigned char bThre)
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

	// ÿ��
	for(i = 0; i < imgH; i++)
	{
		// ÿ��
		for(j = 0; j < imgW; j++)
		{
			// ָ���i�У���j������
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;

			// �ж��Ƿ�С����ֵ
			if (pSrcImgData[srcPos] < bThre)
			{
				// ֱ�Ӹ�ֵΪ0
				pCpyImgData[cpyPos] = 0;
			}
			else
			{
				// ֱ�Ӹ�ֵΪ255
				pCpyImgData[cpyPos] = 255;
			}
		}
	}

	// ���Ʊ任���ͼ��
	pListImage->SetData(pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
}

int  PointTrans::ThresholdTrans16(ListImage *pListImage,unsigned short bThre)
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int srcPos;

	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	// ѭ������
	long i;
	long j;

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

	// ÿ��
	for(i = 0; i < imgH; i++)
	{
		// ÿ��
		for(j = 0; j < imgW; j++)
		{
			// ָ���i�У���j������
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;

			// �ж��Ƿ�С����ֵ
			if (pSrcImgData[srcPos] < bThre)
			{
				// ֱ�Ӹ�ֵΪ0
				pCpyImgData[cpyPos] = 0;
			}
			else
			{
				// ֱ�Ӹ�ֵΪ65535
				pCpyImgData[cpyPos] = 65535;
			}
		}
	}

	// ���Ʊ任���ͼ��
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
}

int  PointTrans::WindowTrans( ListImage *pListImage,unsigned char bLow, unsigned char bUp)
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

	// ÿ��
	for(i = 0; i < imgH; i++)
	{
		// ÿ��
		for(j = 0; j < imgW; j++)
		{
			// ָ���i�У���j������
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;

			// �ж��Ƿ񳬳���Χ
			if (pSrcImgData[srcPos]< bLow)
			{
				// ֱ�Ӹ�ֵΪ0
				pCpyImgData[cpyPos] = 0;
			}
			else if (pSrcImgData[srcPos] > bUp)
			{
				// ֱ�Ӹ�ֵΪ255
				pCpyImgData[cpyPos] = 255;
			}
		}
	}
	// ���Ʊ任���ͼ��
	pListImage->SetData(pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
}
/*
//8λ�Ҷ�����
int  PointTrans::GrayStretch(ListImage *pListImage,unsigned char bX1, unsigned char bY1, unsigned char bX2, unsigned char bY2)
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

	if(bX1 > bX2)
	{
		return -2;
	}
	// �Ҷ�ӳ���
	unsigned char	bMap[256];

	// ����Ҷ�ӳ���
	for (i = 0; i <= bX1; i++)
	{
		// �ж�bX1�Ƿ����0����ֹ��ĸΪ0��
		if (bX1 > 0)
		{
			// ���Ա任
			bMap[i] = (unsigned char) bY1 * i / bX1;
		}
		else
		{
			// ֱ�Ӹ�ֵΪ0
			bMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)
	{
		// �ж�bX1�Ƿ����bX2����ֹ��ĸΪ0��
		if (bX2 != bX1)
		{
			// ���Ա任
			bMap[i] = bY1 + (unsigned char) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// ֱ�Ӹ�ֵΪbY1
			bMap[i] = bY1;
		}
	}
	for (; i < 256; i++)
	{
		// �ж�bX2�Ƿ����255����ֹ��ĸΪ0��
		if (bX2 != 255)
		{
			// ���Ա任
			bMap[i] = bY2 + (unsigned char) ((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// ֱ�Ӹ�ֵΪ255
			bMap[i] = 255;
		}
	}

	// ÿ��
	for(i = 0; i < imgH; i++)
	{
		// ÿ��
		for(j = 0; j < imgW; j++)
		{
			// ָ���i�У���j������
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;

			// �����µĻҶ�ֵ
			pCpyImgData[srcPos] = bMap[pSrcImgData[srcPos]];
		}
	}
	// ���Ʊ任���ͼ��
	pListImage->SetData(pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
}
*/

//16λ�Ҷ�����
int  PointTrans::GrayStretchUint_16(ListImage *pListImage,
							 unsigned short bX1, unsigned short bY1, unsigned short bX2, unsigned short bY2)
{
	// ָ��Դͼ���ָ��
	unsigned short*	pSrcImgData;
	// ָ��Դͼ��������ص�ָ��
	int srcPos;

	// ָ����ͼ���ָ��
	unsigned short*	pCpyImgData;
	// ָ����ͼ��������ص�ָ��
	int cpyPos;

	// ͼ��ߡ���
	int imgH;
	int imgW;
	int imgSize;

	// ѭ������
	long i;
	long j;

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

	if(bX1 > bX2 )
	{
		return -2;
	}

	// �Ҷ�ӳ���
	unsigned short	bMap[65536];

	// ����Ҷ�ӳ���
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

	// ÿ��
	for(i = 0; i < imgH; i++)
	{
		// ÿ��
		for(j = 0; j < imgW; j++)
		{
			// ָ���i�У���j������
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;

			// �����µĻҶ�ֵ
			pCpyImgData[srcPos] = bMap[pSrcImgData[srcPos]];
		}
	}

	// ���Ʊ任���ͼ��
	pListImage->SetData((unsigned char*)pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
}

int  PointTrans::InteEqualize( ListImage *pListImage)
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

	// ��ʱ����
	long	lTemp;

	// �Ҷ�ӳ���
	unsigned char	bMap[256];

	// �Ҷ�ӳ���
	long	lCount[256];

	// ���ü���Ϊ0
	for (i = 0; i < 256; i ++)
	{
		// ����
		lCount[i] = 0;
	}

	// ��������Ҷ�ֵ�ļ���
	for (i = 0; i < imgH; i ++)
	{
		for (j = 0; j < imgW; j ++)
		{
			srcPos = imgW * i + j;

			// ������1
			lCount[pSrcImgData[srcPos]]++;
		}
	}

	// ����Ҷ�ӳ���
	for (i = 0; i < 256; i++)
	{
		// ��ʼΪ0
		lTemp = 0;

		for (j = 0; j <= i ; j++)
		{
			lTemp += lCount[j];
		}

		// �����Ӧ���»Ҷ�ֵ
		bMap[i] = (unsigned char) (lTemp * 255 / imgH / imgW);
	}

	// ÿ��
	for(i = 0; i < imgH; i++)
	{
		// ÿ��
		for(j = 0; j < imgW; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			srcPos = imgW * (imgH - 1 - i) + j;

			// �����µĻҶ�ֵ
			pCpyImgData[srcPos] = bMap[pSrcImgData[srcPos]];
		}
	}
	// ���Ʊ任���ͼ��
	pListImage->SetData(pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
}

int PointTrans::TR_gamma(ListImage *pListImage, double Gamma,double c)
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

	// ÿ��
	for(i = 0; i < imgH; i++)
	{
		// ÿ��
		for(j = 0; j < imgW; j++)
		{
			// ָ���i�У���j������
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;
			pCpyImgData[cpyPos] =c*pow(pSrcImgData[srcPos],Gamma);	 

		}
	}
	// ���Ʊ任���ͼ��
	pListImage->SetData(pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
}
int PointTrans::In_gamma(ListImage *pListImage, double Gamma,double c)
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

	// ÿ��
	for(i = 0; i < imgH; i++)
	{
		// ÿ��
		for(j = 0; j < imgW; j++)
		{
			// ָ���i�У���j������
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;
			pCpyImgData[cpyPos] = pow((pSrcImgData[srcPos]/c),(1.0/Gamma));    
		}
	}
	// ���Ʊ任���ͼ��
	pListImage->SetData(pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
}
int  PointTrans::GrayReverse(ListImage *pListImage)
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

	// ÿ��
	for(i = 0; i < imgH; i++)
	{
		// ÿ��
		for(j = 0; j < imgW; j++)
		{
			// ָ���i�У���j������
			srcPos = imgW * i + j;
			cpyPos = imgW * i + j;
			pCpyImgData[cpyPos] =255 - pSrcImgData[srcPos];	 

		}
	}
	// ���Ʊ任���ͼ��
	pListImage->SetData(pCpyImgData, imgSize);

	// �ͷ��ڴ�
	delete []pCpyImgData;

	// ����
	return 0;
	}