#pragma once
#include "ListImage.h"
#include  "PointDef.h"
/*************************************************************************
 *
 *   类名称：
 *   ConvFilter
 *
 *  类成员：
 *
 *  FilterKernel m_Temp     -用于掩模运算的模板		
 *	
 *	 函数名称：
 *
 *  Template()			- 图像模板变换，通过改变模板，可以用它实现
 *									  图像的平滑、锐化、边缘识别等操作。
 *  MedianFilter()		- 图像中值滤波。
 *  GetMedianNum()		- 获取中值。被函数MedianFilter()调用来求中值。
 *  GradSharp()		    - 该函数用来对图像进行梯度锐化。
 *  UnsharpMasking -反锐化掩模
 *
 *  Robert()		    - robert边缘检测运算
 *  Sobel()		        - sobel边缘检测运算
 *  Prewitt()	        - prewitt边缘检测运算
 *  Kirsch()		        - kirsch边缘检测运算
 *  Gauss()		        - gauss边缘检测运算
 *  Canny()				- Canny边缘检测运算参数:
 *
 *  说明：该类仅支持单通道，8位，12位，16位，32位图像的运算
 *	返回值:   int              - 0:正常； -1：内存分配失败；-2：图片格式不支持 -3：模板参数出错
 *   
 *
 ************************************************************************/
class  ConvFilter
{
public:
	ConvFilter(void);

	ConvFilter(FilterKernel &Temp);
	ConvFilter(int iTempH,int iTempW,int iTempMX,int iTempMY,float* fpArray = NULL,float fCoef = 0.11);

	ConvFilter(ConvFilter &TDF);
	ConvFilter(ConvFilter *pTDF);	

	~ConvFilter();

public:
	int SetFK(FilterKernel &Temp);
	int SetFK(int iTempH,int iTempW,int iTempMX,int iTempMY,float* fA = NULL,float fc = 0.11);

public:
/*************************************************************************
 *
 * 函数名称：
 *   TemplateConv()				- 图像模板变换，通过改变模板，可以用它实现
 *												图像的平滑、锐化、边缘识别等操作。
 *
 * 参数:
 *   ListImage srcImg		     - 源图像
 *   ListImage &destImg       - 处理后图像的引用
 *   int mode      - 指定卷积方式
 *								mode = 0 : 边缘像素保留原值；
 *								mode = 1 : 边缘像素补零计算；
 *								mode = 2 : 边缘像素取对边像素计算；
 *
 * 返回值:
 *   int              - 0:正常； -1：内存分配失败；-2：图片格式不支持 -3：模板参数出错
 *
 * 说明:
 *   该函数用指定的模板 m_Temp （任意大小）来对图像进行卷积操作。（通过SetFK（）设置m_Temp）
 *
 ************************************************************************/
	int  TemplateConv( ListImage srcImg,ListImage &destImg,int mode = 0);

/*************************************************************************
 *
 * 函数名称：
 *   MedianFilter()
 *
  * 参数:
 *   ListImage srcImg		     - 源图像
 *   ListImage &destImg       - 处理后图像的引用
 *   int mode      - 指定卷积方式
 *								mode = 0 : 边缘像素保留原值；
 *								mode = 1 : 边缘像素补零计算；
 *								mode = 2 : 边缘像素取对边像素计算；
 *
 * 返回值:
 *   int              - 0:正常； -1：内存分配失败；-2：图片格式不支持 -3：模板参数出错
 *
 * 说明:
 *   该函数对图像进行中值滤波。
 *   中值滤波的模板宽度和高度可以由m_Temp设定；
 *
 ************************************************************************/
	int MedianFilter( ListImage srcImg,ListImage &destImg, int mode = 0);

/*************************************************************************
 *
 * 函数名称：
 *   GradSharp()
 *
  * 参数:
 *   ListImage srcImg		     - 源图像
 *   ListImage &destImg       - 处理后图像的引用
 *   unsigned int Thre           - 指定锐化阈值：
 *												当梯度小于指定阈值时，源图像像素值不变；
 *												当梯度大于指定阈值时，源图像像素值用对应梯度代替；
 *
 * 返回值:
 *   int              - 0:正常； -1：内存分配失败；-2：图片格式不支持 -3：模板参数出错
 *
 * 说明:
 *   该函数对图像进行梯度锐化。
 *
 ************************************************************************/
	int GradSharp( ListImage srcImg,ListImage &destImg,unsigned int Thre );
	
/*************************************************************************
 *
 * 函数名称：
 *   UnsharpMasking()
 *
  * 参数:
 *   ListImage srcImg		     - 源图像
 *   ListImage &destImg       - 处理后图像的引用
 *   float ratio							  -指定高频部分叠加系数
 *   int mode         - 指定卷积方式
 *								mode = 0 : 边缘像素保留原值；
 *								mode = 1 : 边缘像素补零计算；
 *								mode = 2 : 边缘像素取对边像素计算；
 *
 * 返回值:
 *   int              - 0:正常； -1：内存分配失败；-2：图片格式不支持 -3：模板参数出错
 *
 * 说明:
 *   该函数对图像进行//反锐化掩模(unsharp masking)。
 *
 ************************************************************************/
	int UnsharpMasking( ListImage srcImg,ListImage &destImg, float ratio = 0.7,int mode = 0 );
	int UnsharpMaskingUint_16( ListImage srcImg,ListImage &destImg, float ratio = 0.7,int mode = 0 );	

/*************************************************************************
 *
 * 函数名称：
 *   Roberts()
 *
 * 参数:
 *   ListImage srcImg		     - 源图像
 *   ListImage &destImg       - 处理后图像的引用
 *
 * 返回值:
 *   int              - 0:正常； -1：内存分配失败；-2：图片格式不支持 -3：模板参数出错
 *
 * 说明:
 * 该函数用Robert边缘检测算子对图像进行边缘检测运算。
 * 
 * 要求目标图像为灰度图像。
 ************************************************************************/
	int Roberts( ListImage srcImg,ListImage &destImg);
/*************************************************************************
 *
 * 函数名称：
 *   Sobel()
 *
 * 参数:
 *   ListImage srcImg		     - 源图像
 *   ListImage &destImg       - 处理后图像的引用
 *   int mode         - 指定卷积方式
 *								mode = 0 : 边缘像素保留原值；
 *								mode = 1 : 边缘像素补零计算；
 *								mode = 2 : 边缘像素取对边像素计算；
 *
 * 返回值:
 *   int              - 0:正常； -1：内存分配失败；-2：图片格式不支持 -3：模板参数出错
 *
 * 说明:
 * 该函数用Sobel边缘检测算子对图像进行边缘检测运算。
 * 
 * 要求目标图像为灰度图像。
 ************************************************************************/
	int Sobel( ListImage srcImg,ListImage &destImg, int mode = 0);
/*************************************************************************
 *
 * 函数名称：
 *   Prewitt()
 *
 * 参数:
 *   ListImage srcImg		     - 源图像
 *   ListImage &destImg       - 处理后图像的引用
 *   int mode         - 指定卷积方式
 *								mode = 0 : 边缘像素保留原值；
 *								mode = 1 : 边缘像素补零计算；
 *								mode = 2 : 边缘像素取对边像素计算；
 *
 * 返回值:
 *   int              - 0:正常； -1：内存分配失败；-2：图片格式不支持 -3：模板参数出错
 *
 * 说明:
 * 该函数用Prewitt边缘检测算子对图像进行边缘检测运算。
 * 
 * 要求目标图像为灰度图像。
 ************************************************************************/
	int Prewitt( ListImage srcImg,ListImage &destImg, int mode = 0);
	/*************************************************************************
 *
 * 函数名称：
 *   Kirsch()
 *
 * 参数:
 *   ListImage srcImg		     - 源图像
 *   ListImage &destImg       - 处理后图像的引用
 *   int mode         - 指定卷积方式
 *								mode = 0 : 边缘像素保留原值；
 *								mode = 1 : 边缘像素补零计算；
 *								mode = 2 : 边缘像素取对边像素计算；
 *
 * 返回值:
 *   int              - 0:正常； -1：内存分配失败；-2：图片格式不支持 -3：模板参数出错
 *
 * 说明:
 * 该函数用Kirsch边缘检测算子对图像进行边缘检测运算。
 * 
 * 要求目标图像为灰度图像。
 ************************************************************************/
	int Kirsch( ListImage srcImg,ListImage &destImg, int mode = 0);
	/*************************************************************************
 *
 * 函数名称：
 *   Gauss()
 *
 * 参数:
 *   ListImage srcImg		     - 源图像
 *   ListImage &destImg       - 处理后图像的引用
 *   int mode         - 指定卷积方式
 *								mode = 0 : 边缘像素保留原值；
 *								mode = 1 : 边缘像素补零计算；
 *								mode = 2 : 边缘像素取对边像素计算；
 *
 * 返回值:
 *   int              - 0:正常； -1：内存分配失败；-2：图片格式不支持 -3：模板参数出错
 *
 * 说明:
 * 该函数用Gauss边缘检测算子对图像进行边缘检测运算。
 * 
 * 要求目标图像为灰度图像。
 ************************************************************************/
	int Gauss( ListImage srcImg,ListImage &destImg, int mode = 0);
/*************************************************************************
 *
 *  函数名称：    Canny()
 *
 *  参数:
 *   ListImage srcImg		     - 源图像
 *   ListImage &destImg       - 处理后图像的引用
 *   int mode         - 指定卷积方式
 *								mode = 0 : 边缘像素保留原值；
 *								mode = 1 : 边缘像素补零计算；
 *								mode = 2 : 边缘像素取对边像素计算；
 *	 double dRatioLow         - 低阈值和高阈值之间的比例
 *	 double dRatioHigh        - 高阈值占图象象素总数的比例
 *
 * 返回值:
 *   int              - 0:正常； -1：内存分配失败；-2：图片格式不支持 -3：模板参数出错
 *
 *  说明:
 *   canny分割算子，计算的结果保存在destImg中，
 *   逻辑0(0)表示该点为非边界点。
 *   dRatioLow、dRatioHigh，是需要指定的。这些参数会影响分割后边界点数目的多少
 *
 **************************************************************************/
	int Canny(  ListImage srcImg,ListImage &destImg, int mode = 0,double dRatioLow  = 0.4 ,
		double dRatioHigh = 0.79);

private:
	//模板函数
	int TemplateConv(ListImage *pListImage,FilterKernel &Temp,int mode = 0);
	int TemplateConvUint_8 (ListImage *pListImage,FilterKernel &Temp,int mode = 0);
	int TemplateConvUint_12(ListImage *pListImage,FilterKernel &Temp,int mode = 0);
	int TemplateConvUint_16(ListImage *pListImage,FilterKernel &Temp,int mode = 0);
	int TemplateConvSingle(ListImage *pListImage,FilterKernel &Temp,int mode = 0);

	int MedianFilterUint_8 ( ListImage *pListImage, int mode = 0);
	int MedianFilterUint_12( ListImage *pListImage, int mode = 0);
	int MedianFilterUint_16( ListImage *pListImage, int mode = 0);
	int MedianFilterSingle( ListImage *pListImage, int mode = 0);

	int GradSharpUint_8 ( ListImage *pListImage,unsigned char Thre = 10 );
	int GradSharpUint_12( ListImage *pListImage,unsigned short Thre = 10*16 );
	int GradSharpUint_16( ListImage *pListImage,unsigned short Thre = 10*256);
	int GradSharpSingle( ListImage *pListImage,unsigned int Thre = 10*256*256*256);

	//边缘检测算子
	int RobertsUint_8 ( ListImage *pListImage);
	int SobelUint_8 ( ListImage *pListImage, int mode = 0);
	int PrewittUint_8 ( ListImage *pListImage, int mode = 0);
	int KirschUint_8 ( ListImage *pListImage, int mode = 0);
	int GaussUint_8 ( ListImage *pListImage, int mode = 0);

	int RobertsUint_12( ListImage *pListImage);
	int SobelUint_12( ListImage *pListImage, int mode = 0);
	int PrewittUint_12( ListImage *pListImage, int mode = 0);
	int KirschUint_12( ListImage *pListImage, int mode = 0);
	int GaussUint_12( ListImage *pListImage, int mode = 0);

	//边缘检测算子
	int RobertsUint_16( ListImage *pListImage);
	int SobelUint_16( ListImage *pListImage, int mode = 0);
	int PrewittUint_16( ListImage *pListImage, int mode = 0);
	int KirschUint_16( ListImage *pListImage, int mode = 0);
	int GaussUint_16( ListImage *pListImage, int mode = 0);

	//边缘检测算子
	int RobertsSingle( ListImage *pListImage);
	int SobelSingle( ListImage *pListImage, int mode = 0);
	int PrewittSingle( ListImage *pListImage, int mode = 0);
	int KirschSingle( ListImage *pListImage, int mode = 0);
	int GaussSingle( ListImage *pListImage, int mode = 0);

/*************************************************************************
 *
 * 函数名称：
 *   GetMedianNum()
 *
 * 参数:
 *   unsigned char * bpArray	- 指向要获取中值的数组指针
 *
 * 返回值:
 *   unsigned char      - 返回指定数组的中值。
 *
 * 说明:
 *   该函数用冒泡法对一维数组进行排序，并返回数组元素的中值。
 *   数组长度由m_Temp的大小决定
 *
 ************************************************************************/
	unsigned char GetMedianNum(unsigned char * Array);		//8位
	unsigned short GetMedianNum(unsigned short * Array);	//12 和16位
	unsigned int GetMedianNum(unsigned int * Array);	//32位

	//判断输入图像是否合法
	bool IsImageIllegal( ListImage *pListImage);
	bool IsFilterKernelIllegal( FilterKernel &Temp);
	
	//canny算子子函数
	// 计算方向导数
	int DirGrad(ListImage *pListImage,long *pGradX, long *pGradY) ;
	int DirGradUint_8(ListImage *pListImage, long *pGradX , long *pGradY);
	int DirGradUint_12(ListImage *pListImage, long *pGradX , long *pGradY);
	int DirGradUint_16(ListImage *pListImage, long *pGradX , long *pGradY);
	int DirGradSingle(ListImage *pListImage, long *pGradX , long *pGradY);
	//计算梯度的幅度
	int GradRange(ListImage *pListImage,long *pGradX, long *pGradY, long *pGradRange);
	//抑止梯度图中非局部极值点的象素
	int NonmaxSuppress(ListImage *pListImage,long *pGradX, long *pGradY,long *pGradRange);
	int NonmaxSuppressUint_8(ListImage *pListImage,long *pGradX, long *pGradY,long *pGradRange);
	int NonmaxSuppressUint_12(ListImage *pListImage,long *pGradX, long *pGradY,long *pGradRange);
	int NonmaxSuppressUint_16(ListImage *pListImage,long *pGradX, long *pGradY,long *pGradRange);
	int NonmaxSuppressSingle(ListImage *pListImage,long *pGradX, long *pGradY,long *pGradRange);

	int Hysteresis(ListImage *pListImage,long *pGradRange, double dRatioLow ,double dRatioHigh );
	int HysteresisUint_8(ListImage *pListImage,long *pGradRange, double dRatioLow ,double dRatioHigh );
	int HysteresisUint_12(ListImage *pListImage,long *pGradRange, double dRatioLow ,double dRatioHigh );
	int HysteresisUint_16(ListImage *pListImage,long *pGradRange, double dRatioLow ,double dRatioHigh );
	int HysteresisSingle(ListImage *pListImage,long *pGradRange, double dRatioLow ,double dRatioHigh );

	int EstimateThreshold(ListImage *pListImage, int *pThdHigh,int *pThdLow, 
		double dRatioHigh, double dRatioLow,long* pGradRange);
	int EstimateThresholdUint_8(ListImage *pListImage, int *pThdHigh,int *pThdLow, 
		double dRatioHigh, double dRatioLow,long* pGradRange);
	int EstimateThresholdUint_12(ListImage *pListImage, int *pThdHigh,int *pThdLow, 
		double dRatioHigh, double dRatioLow,long* pGradRange);
	int EstimateThresholdUint_16(ListImage *pListImage, int *pThdHigh,int *pThdLow, 
		double dRatioHigh, double dRatioLow,long* pGradRange);
	int EstimateThresholdSingle(ListImage *pListImage, int *pThdHigh,int *pThdLow, 
		double dRatioHigh, double dRatioLow,long* pGradRange);

	int TraceEdge(ListImage *pListImage,int y, int x, int nLowThd, long *pImgRange);
	int TraceEdgeUint_8 (ListImage *pListImage,int y, int x, int nLowThd, long *pImgRange);
	int TraceEdgeUint_12 (ListImage *pListImage,int y, int x, int nLowThd, long *pImgRange);
	int TraceEdgeUint_16 (ListImage *pListImage,int y, int x, int nLowThd, long *pImgRange);
	int TraceEdgeSingle (ListImage *pListImage,int y, int x, int nLowThd, long *pImgRange);

	/*	int AddOrSubImg(ListImage *pSrc1,ListImage *pSrc2,ListImage *pDest,int mode = 0,
		float ratio1 = 1,float ratio2 = 1);
	int AddOrSubImgUint_8(ListImage *pSrc1,ListImage *pSrc2,ListImage *pDest,int mode = 0,
		float ratio1 = 1,float ratio2 = 1);
	int AddOrSubImgUint_12(ListImage *pSrc1,ListImage *pSrc2,ListImage *pDest,int mode = 0,
		float ratio1 = 1,float ratio2 = 1);
	int AddOrSubImgUint_16(ListImage *pSrc1,ListImage *pSrc2,ListImage *pDest,int mode = 0,
		float ratio1 = 1,float ratio2 = 1);
	int AddOrSubImgSingle(ListImage *pSrc1,ListImage *pSrc2,ListImage *pDest,int mode = 0,
		float ratio1 = 1,float ratio2 = 1);	*/
		//加减图像像素:
	//ratio1为psrc1系数，ratio2为pSrc2系数
	//int SubImgUint_8(ListImage *pSrc1,ListImage *pSrc2,int * pDest,
	//	float ratio1 = 1,float ratio2 = 1);
	//int SubImgUint_12(ListImage *pSrc1,ListImage *pSrc2,int * pDest,
	//	float ratio1 = 1,float ratio2 = 1);
	int SubImgUint_16(ListImage *pSrc1,ListImage *pSrc2,int * pDest,
		float ratio1 = 1,float ratio2 = 1);
	//int SubImgSingle(ListImage *pSrc1,ListImage *pSrc2,int * pDest,
	//	float ratio1 = 1,float ratio2 = 1);

	//int AddImgUint_8(ListImage *pSrc1,int * pSrc2,ListImage *pDest,
	//	float ratio1 = 1,float ratio2 = 1);
	//int AddImgUint_12(ListImage *pSrc1,int* pSrc2,ListImage *pDest,
	//	float ratio1 = 1,float ratio2 = 1);
	int AddImgUint_16(ListImage *pSrc1,int *pSrc2,ListImage *pDest,
		float ratio1 = 1,float ratio2 = 1);
	//int AddImgSingle(ListImage *pSrc1,int *pSrc2,ListImage *pDest,
	//	float ratio1 = 1,float ratio2 = 1);

private:
	FilterKernel m_Temp;			//模板
};
