#pragma once
#include "ListImage.h"
#include  "PointDef.h"
/*************************************************************************
 *
 *   �����ƣ�
 *   ConvFilter
 *
 *  ���Ա��
 *
 *  FilterKernel m_Temp     -������ģ�����ģ��		
 *	
 *	 �������ƣ�
 *
 *  Template()			- ͼ��ģ��任��ͨ���ı�ģ�壬��������ʵ��
 *									  ͼ���ƽ�����񻯡���Եʶ��Ȳ�����
 *  MedianFilter()		- ͼ����ֵ�˲���
 *  GetMedianNum()		- ��ȡ��ֵ��������MedianFilter()����������ֵ��
 *  GradSharp()		    - �ú���������ͼ������ݶ��񻯡�
 *  UnsharpMasking -������ģ
 *
 *  Robert()		    - robert��Ե�������
 *  Sobel()		        - sobel��Ե�������
 *  Prewitt()	        - prewitt��Ե�������
 *  Kirsch()		        - kirsch��Ե�������
 *  Gauss()		        - gauss��Ե�������
 *  Canny()				- Canny��Ե����������:
 *
 *  ˵���������֧�ֵ�ͨ����8λ��12λ��16λ��32λͼ�������
 *	����ֵ:   int              - 0:������ -1���ڴ����ʧ�ܣ�-2��ͼƬ��ʽ��֧�� -3��ģ���������
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
 * �������ƣ�
 *   TemplateConv()				- ͼ��ģ��任��ͨ���ı�ģ�壬��������ʵ��
 *												ͼ���ƽ�����񻯡���Եʶ��Ȳ�����
 *
 * ����:
 *   ListImage srcImg		     - Դͼ��
 *   ListImage &destImg       - �����ͼ�������
 *   int mode      - ָ�������ʽ
 *								mode = 0 : ��Ե���ر���ԭֵ��
 *								mode = 1 : ��Ե���ز�����㣻
 *								mode = 2 : ��Ե����ȡ�Ա����ؼ��㣻
 *
 * ����ֵ:
 *   int              - 0:������ -1���ڴ����ʧ�ܣ�-2��ͼƬ��ʽ��֧�� -3��ģ���������
 *
 * ˵��:
 *   �ú�����ָ����ģ�� m_Temp �������С������ͼ����о����������ͨ��SetFK��������m_Temp��
 *
 ************************************************************************/
	int  TemplateConv( ListImage srcImg,ListImage &destImg,int mode = 0);

/*************************************************************************
 *
 * �������ƣ�
 *   MedianFilter()
 *
  * ����:
 *   ListImage srcImg		     - Դͼ��
 *   ListImage &destImg       - �����ͼ�������
 *   int mode      - ָ�������ʽ
 *								mode = 0 : ��Ե���ر���ԭֵ��
 *								mode = 1 : ��Ե���ز�����㣻
 *								mode = 2 : ��Ե����ȡ�Ա����ؼ��㣻
 *
 * ����ֵ:
 *   int              - 0:������ -1���ڴ����ʧ�ܣ�-2��ͼƬ��ʽ��֧�� -3��ģ���������
 *
 * ˵��:
 *   �ú�����ͼ�������ֵ�˲���
 *   ��ֵ�˲���ģ���Ⱥ͸߶ȿ�����m_Temp�趨��
 *
 ************************************************************************/
	int MedianFilter( ListImage srcImg,ListImage &destImg, int mode = 0);

/*************************************************************************
 *
 * �������ƣ�
 *   GradSharp()
 *
  * ����:
 *   ListImage srcImg		     - Դͼ��
 *   ListImage &destImg       - �����ͼ�������
 *   unsigned int Thre           - ָ������ֵ��
 *												���ݶ�С��ָ����ֵʱ��Դͼ������ֵ���䣻
 *												���ݶȴ���ָ����ֵʱ��Դͼ������ֵ�ö�Ӧ�ݶȴ��棻
 *
 * ����ֵ:
 *   int              - 0:������ -1���ڴ����ʧ�ܣ�-2��ͼƬ��ʽ��֧�� -3��ģ���������
 *
 * ˵��:
 *   �ú�����ͼ������ݶ��񻯡�
 *
 ************************************************************************/
	int GradSharp( ListImage srcImg,ListImage &destImg,unsigned int Thre );
	
/*************************************************************************
 *
 * �������ƣ�
 *   UnsharpMasking()
 *
  * ����:
 *   ListImage srcImg		     - Դͼ��
 *   ListImage &destImg       - �����ͼ�������
 *   float ratio							  -ָ����Ƶ���ֵ���ϵ��
 *   int mode         - ָ�������ʽ
 *								mode = 0 : ��Ե���ر���ԭֵ��
 *								mode = 1 : ��Ե���ز�����㣻
 *								mode = 2 : ��Ե����ȡ�Ա����ؼ��㣻
 *
 * ����ֵ:
 *   int              - 0:������ -1���ڴ����ʧ�ܣ�-2��ͼƬ��ʽ��֧�� -3��ģ���������
 *
 * ˵��:
 *   �ú�����ͼ�����//������ģ(unsharp masking)��
 *
 ************************************************************************/
	int UnsharpMasking( ListImage srcImg,ListImage &destImg, float ratio = 0.7,int mode = 0 );
	int UnsharpMaskingUint_16( ListImage srcImg,ListImage &destImg, float ratio = 0.7,int mode = 0 );	

/*************************************************************************
 *
 * �������ƣ�
 *   Roberts()
 *
 * ����:
 *   ListImage srcImg		     - Դͼ��
 *   ListImage &destImg       - �����ͼ�������
 *
 * ����ֵ:
 *   int              - 0:������ -1���ڴ����ʧ�ܣ�-2��ͼƬ��ʽ��֧�� -3��ģ���������
 *
 * ˵��:
 * �ú�����Robert��Ե������Ӷ�ͼ����б�Ե������㡣
 * 
 * Ҫ��Ŀ��ͼ��Ϊ�Ҷ�ͼ��
 ************************************************************************/
	int Roberts( ListImage srcImg,ListImage &destImg);
/*************************************************************************
 *
 * �������ƣ�
 *   Sobel()
 *
 * ����:
 *   ListImage srcImg		     - Դͼ��
 *   ListImage &destImg       - �����ͼ�������
 *   int mode         - ָ�������ʽ
 *								mode = 0 : ��Ե���ر���ԭֵ��
 *								mode = 1 : ��Ե���ز�����㣻
 *								mode = 2 : ��Ե����ȡ�Ա����ؼ��㣻
 *
 * ����ֵ:
 *   int              - 0:������ -1���ڴ����ʧ�ܣ�-2��ͼƬ��ʽ��֧�� -3��ģ���������
 *
 * ˵��:
 * �ú�����Sobel��Ե������Ӷ�ͼ����б�Ե������㡣
 * 
 * Ҫ��Ŀ��ͼ��Ϊ�Ҷ�ͼ��
 ************************************************************************/
	int Sobel( ListImage srcImg,ListImage &destImg, int mode = 0);
/*************************************************************************
 *
 * �������ƣ�
 *   Prewitt()
 *
 * ����:
 *   ListImage srcImg		     - Դͼ��
 *   ListImage &destImg       - �����ͼ�������
 *   int mode         - ָ�������ʽ
 *								mode = 0 : ��Ե���ر���ԭֵ��
 *								mode = 1 : ��Ե���ز�����㣻
 *								mode = 2 : ��Ե����ȡ�Ա����ؼ��㣻
 *
 * ����ֵ:
 *   int              - 0:������ -1���ڴ����ʧ�ܣ�-2��ͼƬ��ʽ��֧�� -3��ģ���������
 *
 * ˵��:
 * �ú�����Prewitt��Ե������Ӷ�ͼ����б�Ե������㡣
 * 
 * Ҫ��Ŀ��ͼ��Ϊ�Ҷ�ͼ��
 ************************************************************************/
	int Prewitt( ListImage srcImg,ListImage &destImg, int mode = 0);
	/*************************************************************************
 *
 * �������ƣ�
 *   Kirsch()
 *
 * ����:
 *   ListImage srcImg		     - Դͼ��
 *   ListImage &destImg       - �����ͼ�������
 *   int mode         - ָ�������ʽ
 *								mode = 0 : ��Ե���ر���ԭֵ��
 *								mode = 1 : ��Ե���ز�����㣻
 *								mode = 2 : ��Ե����ȡ�Ա����ؼ��㣻
 *
 * ����ֵ:
 *   int              - 0:������ -1���ڴ����ʧ�ܣ�-2��ͼƬ��ʽ��֧�� -3��ģ���������
 *
 * ˵��:
 * �ú�����Kirsch��Ե������Ӷ�ͼ����б�Ե������㡣
 * 
 * Ҫ��Ŀ��ͼ��Ϊ�Ҷ�ͼ��
 ************************************************************************/
	int Kirsch( ListImage srcImg,ListImage &destImg, int mode = 0);
	/*************************************************************************
 *
 * �������ƣ�
 *   Gauss()
 *
 * ����:
 *   ListImage srcImg		     - Դͼ��
 *   ListImage &destImg       - �����ͼ�������
 *   int mode         - ָ�������ʽ
 *								mode = 0 : ��Ե���ر���ԭֵ��
 *								mode = 1 : ��Ե���ز�����㣻
 *								mode = 2 : ��Ե����ȡ�Ա����ؼ��㣻
 *
 * ����ֵ:
 *   int              - 0:������ -1���ڴ����ʧ�ܣ�-2��ͼƬ��ʽ��֧�� -3��ģ���������
 *
 * ˵��:
 * �ú�����Gauss��Ե������Ӷ�ͼ����б�Ե������㡣
 * 
 * Ҫ��Ŀ��ͼ��Ϊ�Ҷ�ͼ��
 ************************************************************************/
	int Gauss( ListImage srcImg,ListImage &destImg, int mode = 0);
/*************************************************************************
 *
 *  �������ƣ�    Canny()
 *
 *  ����:
 *   ListImage srcImg		     - Դͼ��
 *   ListImage &destImg       - �����ͼ�������
 *   int mode         - ָ�������ʽ
 *								mode = 0 : ��Ե���ر���ԭֵ��
 *								mode = 1 : ��Ե���ز�����㣻
 *								mode = 2 : ��Ե����ȡ�Ա����ؼ��㣻
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *
 * ����ֵ:
 *   int              - 0:������ -1���ڴ����ʧ�ܣ�-2��ͼƬ��ʽ��֧�� -3��ģ���������
 *
 *  ˵��:
 *   canny�ָ����ӣ�����Ľ��������destImg�У�
 *   �߼�0(0)��ʾ�õ�Ϊ�Ǳ߽�㡣
 *   dRatioLow��dRatioHigh������Ҫָ���ġ���Щ������Ӱ��ָ��߽����Ŀ�Ķ���
 *
 **************************************************************************/
	int Canny(  ListImage srcImg,ListImage &destImg, int mode = 0,double dRatioLow  = 0.4 ,
		double dRatioHigh = 0.79);

private:
	//ģ�庯��
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

	//��Ե�������
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

	//��Ե�������
	int RobertsUint_16( ListImage *pListImage);
	int SobelUint_16( ListImage *pListImage, int mode = 0);
	int PrewittUint_16( ListImage *pListImage, int mode = 0);
	int KirschUint_16( ListImage *pListImage, int mode = 0);
	int GaussUint_16( ListImage *pListImage, int mode = 0);

	//��Ե�������
	int RobertsSingle( ListImage *pListImage);
	int SobelSingle( ListImage *pListImage, int mode = 0);
	int PrewittSingle( ListImage *pListImage, int mode = 0);
	int KirschSingle( ListImage *pListImage, int mode = 0);
	int GaussSingle( ListImage *pListImage, int mode = 0);

/*************************************************************************
 *
 * �������ƣ�
 *   GetMedianNum()
 *
 * ����:
 *   unsigned char * bpArray	- ָ��Ҫ��ȡ��ֵ������ָ��
 *
 * ����ֵ:
 *   unsigned char      - ����ָ���������ֵ��
 *
 * ˵��:
 *   �ú�����ð�ݷ���һά����������򣬲���������Ԫ�ص���ֵ��
 *   ���鳤����m_Temp�Ĵ�С����
 *
 ************************************************************************/
	unsigned char GetMedianNum(unsigned char * Array);		//8λ
	unsigned short GetMedianNum(unsigned short * Array);	//12 ��16λ
	unsigned int GetMedianNum(unsigned int * Array);	//32λ

	//�ж�����ͼ���Ƿ�Ϸ�
	bool IsImageIllegal( ListImage *pListImage);
	bool IsFilterKernelIllegal( FilterKernel &Temp);
	
	//canny�����Ӻ���
	// ���㷽����
	int DirGrad(ListImage *pListImage,long *pGradX, long *pGradY) ;
	int DirGradUint_8(ListImage *pListImage, long *pGradX , long *pGradY);
	int DirGradUint_12(ListImage *pListImage, long *pGradX , long *pGradY);
	int DirGradUint_16(ListImage *pListImage, long *pGradX , long *pGradY);
	int DirGradSingle(ListImage *pListImage, long *pGradX , long *pGradY);
	//�����ݶȵķ���
	int GradRange(ListImage *pListImage,long *pGradX, long *pGradY, long *pGradRange);
	//��ֹ�ݶ�ͼ�зǾֲ���ֵ�������
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
		//�Ӽ�ͼ������:
	//ratio1Ϊpsrc1ϵ����ratio2ΪpSrc2ϵ��
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
	FilterKernel m_Temp;			//ģ��
};
