#pragma once
#include "Listimage.h"
#include "PointDef.h"

enum Shape{
	line,	//����
	rectangle,//����
	square,	//����
	diamond,//����	
	disk,	//Բ
	empty,
	others,
};

class  Structure
{	
private:
	unsigned int width;
	unsigned int height;
	Shape shape;
	point2di center;
	unsigned int* lpStr;
public:
	Structure();
	Structure(int** array,unsigned int width,unsigned int height);
	Structure(unsigned int width,unsigned int height);
	~Structure();

public:

	int	 Disk(unsigned int R,unsigned int value=1);
	int	 Diamond(unsigned int R,unsigned int value=1);
	int	 Rectangle(unsigned int width,unsigned int height,unsigned int value=1);
	int  Square(unsigned int R,unsigned int value=1);
	int	 Line(unsigned int lengh=3,int x=3,int y=3,unsigned int value=1);
	Structure & operator = (Structure &str);
	int setValue(unsigned int x,unsigned int y,unsigned int value);
	int setValue(unsigned int value);
	int setCenter(point2di p);
	int setCenter(unsigned int x,unsigned int y);

	unsigned int getWidth(){return width;}
	unsigned int getHeight(){return height;}
	point2di getCenter(){return center;}
	unsigned int getValue(unsigned int x,unsigned int y){return lpStr[y*width+x];}
	Shape getShape(){return shape;}
};

class  Morphy
	//��̬ѧ�任������
{	

public:
	Structure* structure;  //Ψһ��Ա


public:
	//���캯��
	Morphy(){structure=new Structure();}

	~Morphy(){delete structure;}

public:
	//ת��Ϊ��ֵͼ��
	int Logical_8( ListImage limage,ListImage& resultlimage,unsigned int value=255);
	int Logical_12( ListImage limage,ListImage& resultlimage,unsigned int value=4095);
	int Logical_16( ListImage limage,ListImage& resultlimage,unsigned int value=65535);
	int Logical( ListImage limage,ListImage& resultlimage,unsigned int value=255);

	int	setValue_8( ListImage limage,ListImage& resultlimage,unsigned int x,unsigned int y,unsigned char value);
	int	setValue_12( ListImage limage,ListImage& resultlimage,unsigned int x,unsigned int y,short value);
	int	setValue_16( ListImage limage,ListImage& resultlimage,unsigned int x,unsigned int y,short value);
	int	setValue( ListImage limage,ListImage& resultlimage,unsigned int x,unsigned int y,short value);

	int	getValue_8( ListImage limage,unsigned int x,unsigned int y);
	int	getValue_12( ListImage limage,unsigned int x,unsigned int y);
	int	getValue_16( ListImage limage,unsigned int x,unsigned int y);
	int	getValue( ListImage limage,unsigned int x,unsigned int y);

	//������
	int Complement_8( ListImage limageA, ListImage limageB,ListImage& resultlimage);
	int Complement_12( ListImage limageA, ListImage limageB,ListImage& resultlimage);
	int Complement_16( ListImage limageA, ListImage limageB,ListImage& resultlimage);
	int Complement( ListImage limageA, ListImage limageB,ListImage& resultlimage);

	int Complement_8( ListImage limage,ListImage& resultlimage);
	int Complement_12( ListImage limage,ListImage& resultlimage);
	int Complement_16( ListImage limage,ListImage& resultlimage);
	int Complement( ListImage limage,ListImage& resultlimage);


	int And_8( ListImage limageA, ListImage limageB,ListImage& resultlimage);
	int And_12( ListImage limageA, ListImage limageB,ListImage& resultlimage);
	int And_16( ListImage limageA, ListImage limageB,ListImage& resultlimage);
	int And( ListImage limageA, ListImage limageB,ListImage& resultlimage);

	int Or_8( ListImage limageA, ListImage limageB,ListImage& resultlimage,unsigned int value=255);
	int Or_12( ListImage limageA, ListImage limageB,ListImage& resultlimage,unsigned int value=4095);
	int Or_16( ListImage limageA, ListImage limageB,ListImage& resultlimage,unsigned int value=65535);
	int Or( ListImage limageA, ListImage limageB,ListImage& resultlimage,unsigned int value=255);


	int Equal_8( ListImage limageA, ListImage limageB);
	int Equal_12( ListImage limageA, ListImage limageB);
	int Equal_16( ListImage limageA, ListImage limageB);
	int Equal( ListImage limageA, ListImage limageB);


	int Count_8( ListImage limage,unsigned char value=0);
	int Count_12( ListImage limage,short value=0);
	int Count_16( ListImage limage,short value=0);
	int Count( ListImage limage,unsigned int value=0);

	//��ֵͼ�����ͺ͸�ʴ
	int Erode_8( ListImage limage,ListImage& resultlimage);
	int Erode_12( ListImage limage,ListImage& resultlimage);
	int Erode_16( ListImage limage,ListImage& resultlimage);
	int Erode( ListImage limage,ListImage& resultlimage);
	int Dialate_8( ListImage limage,ListImage& resultlimage);
	int Dialate_12( ListImage limage,ListImage& resultlimage);
	int Dialate_16( ListImage limage,ListImage& resultlimage);
	int Dialate( ListImage limage,ListImage& resultlimage);


	//�Ҷ�ͼ�����ͺ͸�ʴ
	int Erodegray_8( ListImage limage,ListImage& resultlimage);
	int Dialategray_8( ListImage limage,ListImage& resultlimage);
	int Erodegray_16( ListImage limage,ListImage& resultlimage);
	int Dialategray_16( ListImage limage,ListImage& resultlimage);
	int Erodegray_12( ListImage limage,ListImage& resultlimage);
	int Dialategray_12( ListImage limage,ListImage& resultlimage);
	int Erodegray( ListImage limage,ListImage& resultlimage);
	int Dialategray( ListImage limage,ListImage& resultlimage);


	//��ֵͼ�񿪺ͱ�
	int Open_8( ListImage limage,ListImage& resultlimage);
	int Close_8( ListImage limage,ListImage& resultlimage);
	int Open_12( ListImage limage,ListImage& resultlimage);
	int Close_12( ListImage limage,ListImage& resultlimage);
	int Open_16( ListImage limage,ListImage& resultlimage);
	int Close_16( ListImage limage,ListImage& resultlimage);
	int Open( ListImage limage,ListImage& resultlimage);
	int Close( ListImage limage,ListImage& resultlimage);


	//�Ҷ�ͼ�񿪺ͱ�
	int Opengray_8( ListImage limage,ListImage& resultlimage);
	int Closegray_8( ListImage limage,ListImage& resultlimage);
	int Opengray_12( ListImage limage,ListImage& resultlimage);
	int Closegray_12( ListImage limage,ListImage& resultlimage);
	int Opengray_16( ListImage limage,ListImage& resultlimage);
	int Closegray_16( ListImage limage,ListImage& resultlimage);
	int Opengray( ListImage limage,ListImage& resultlimage);
	int Closegray( ListImage limage,ListImage& resultlimage);


	//ϸ��
	int Thining_8( ListImage limage,ListImage& resultlimage);
	int Thining_12( ListImage limage,ListImage& resultlimage);
	int Thining_16( ListImage limage,ListImage& resultlimage);
	int Thining( ListImage limage,ListImage& resultlimage);


	//��ͨ�����
	int MarkConnect4_8( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num=0);
	int MarkConnect8_8( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num=0);
	int MarkConnect4_12( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num=0);
	int MarkConnect8_12( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num=0);
	int MarkConnect4_16( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num=0);
	int MarkConnect8_16( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num=0);
	int MarkConnect4( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num=0);
	int MarkConnect8( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num=0);

	//������ȡ����������
	int ContourDraw_8( ListImage limage,ListImage& resultlimage);
	int ContourTrace_8( ListImage limage,ListImage& resultlimage);
	int ContourDraw_12( ListImage limage,ListImage& resultlimage);
	int ContourTrace_12( ListImage limage,ListImage& resultlimage);
	int ContourDraw_16( ListImage limage,ListImage& resultlimage);
	int ContourTrace_16( ListImage limage,ListImage& resultlimage);
	int ContourDraw( ListImage limage,ListImage& resultlimage);
	int ContourTrace( ListImage limage,ListImage& resultlimage);


	//�������
	int Fillneighbour1_8( ListImage limage,ListImage& resultlimage,point2di seed,int allow=0);  //������Ϊ����ֵ�������
	int Fillneighbour2_8( ListImage limage,ListImage& resultlimage,point2di seed,int allow=0);  //��ÿһ����Ϊ����ֵ�������
	int Fillneighbour1_12( ListImage limage,ListImage& resultlimage,point2di seed,int allow=0);  //������Ϊ����ֵ�������
	int Fillneighbour2_12( ListImage limage,ListImage& resultlimage,point2di seed,int allow=0);  //��ÿһ����Ϊ����ֵ�������
	int Fillneighbour1_16( ListImage limage,ListImage& resultlimage,point2di seed,int allow=0);  //������Ϊ����ֵ�������
	int Fillneighbour2_16( ListImage limage,ListImage& resultlimage,point2di seed,int allow=0);  //��ÿһ����Ϊ����ֵ�������
	int Fill_8( ListImage limage,ListImage& resultlimage,point2di seed,int allow=0);  //������Ϊ����ֵ���ȫ��
	int Fill_12( ListImage limage,ListImage& resultlimage,point2di seed,int allow=0);  //������Ϊ����ֵ���ȫ��
	int Fill_16( ListImage limage,ListImage& resultlimage,point2di seed,int allow=0);  //������Ϊ����ֵ���ȫ��
	int Fillneighbour1( ListImage limage,ListImage& resultlimage,point2di seed,int allow=0);  //������Ϊ����ֵ�������
	int Fillneighbour2( ListImage limage,ListImage& resultlimage,point2di seed,int allow=0);  //��ÿһ����Ϊ����ֵ�������
	int Fill( ListImage limage,ListImage& resultlimage,point2di seed,int allow=0);  //������Ϊ����ֵ���ȫ��
};