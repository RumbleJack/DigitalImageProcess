#pragma once

#include "ListImage.h"
class PointTrans
{
public:
	// ����ԭ��  0�ɹ���-1�ڴ����-2��������
	int  ThresholdTrans16( ListImage *pListImage,unsigned short bThre);
	int  ThresholdTrans8( ListImage *pListImage,unsigned char bThre);
	int  WindowTrans( ListImage *pListImage,unsigned char bLow, unsigned char bUp);
	int  GrayStretchUint_16( ListImage *pListImage,unsigned short bX1, unsigned short bY1, unsigned short bX2, unsigned short bY2);
	int  InteEqualize( ListImage *pListImage);
	int TR_gamma(ListImage *pListImage, double Gamma,double c);
	int In_gamma(ListImage *pListImage, double Gamma,double c);
	int  GrayReverse(ListImage *pListImage);
};
