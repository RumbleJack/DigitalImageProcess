#pragma once

#define PI 3.141592653589793238462643383279
#define Er 1E-10
class point2df;


class  point2di
{
public:
	point2di(){x=0;y=0;};
	point2di(int ix, int iy){x=ix;y=iy;};
	point2di(float fx, float fy){x=fx;y=fy;};
	point2di(point2di &pt){x=pt.x;y=pt.y;};
	point2di(point2df &pt);

	~point2di(){};

	point2di operator + (point2di &a) {return point2di(x+a.x,y+a.y);};
	point2di operator - (point2di &a) {return point2di(x-a.x,y-a.y);};
	point2di operator * (point2di &a) {return point2di(x*a.x,y*a.y);};
	point2di operator / (point2di &a) {return point2di(x/a.x,y/a.y);};
	point2di & operator = (point2di &a) {x=a.x;y=a.y;return *this;};

	point2di operator + (int i) {return point2di(x+i,y+i);};
	point2di operator - (int i) {return point2di(x-i,y-i);};
	point2di operator * (int i) {return point2di(x*i,y*i);};
	point2di operator / (int i) {return point2di(x/i,y/i);};
	point2di & operator = (int i) {x=i;y=i;return *this;};

	point2di operator + (float f) {return point2di(x+f,y+f);};
	point2di operator - (float f) {return point2di(x-f,y-f);};
	point2di operator * (float f) {return point2di(x*f,y*f);};
	point2di operator / (float f) {return point2di(x/f,y/f);};
	point2di & operator = (float f) {x=f;y=f;return *this;};

	bool operator == (point2di &a) {return (x==a.x) && (y==a.y);}; 
	bool operator != (point2di &a) {return (x!=a.x) || (y!=a.y);}; 
	

	/*
	�жϲ������Ƿ��ڵ�ǰ������
	���򷵻�1�����򷵻�-1���������������򷵻�0
	*/
	int IsLeft(point2di a);

	/*
	�жϲ������Ƿ��ڵ�ǰ����Ϸ�
	���򷵻�1�����򷵻�-1���������������򷵻�0
	*/
	int IsUp(point2di a);

	/*��������֮��ľ���*/
	float Distance(point2di a);

	/*��ǰ��ļ���*/
	float Angle();

	/*�����㣨ʼ������ǰ�㣨�գ��������Ƕ�*/
	float Angle(point2di a);


public:
	int x;
	int y;
};

class  point2df
{
public:
	point2df(){x=0;y=0;};
	point2df(int ix, int iy){x=ix;y=iy;};
	point2df(float fx, float fy){x=fx;y=fy;};
	point2df(point2di &pt){x=pt.x;y=pt.y;};
	point2df(point2df &pt){x=pt.x;y=pt.y;};

	~point2df(){};

	point2df operator + (point2df &a) {return point2df(x+a.x,y+a.y);};
	point2df operator - (point2df &a) {return point2df(x-a.x,y-a.y);};
	point2df operator * (point2df &a) {return point2df(x*a.x,y*a.y);};
	point2df operator / (point2df &a) {return point2df(x/a.x,y/a.y);};
	point2df & operator = (point2df &a) {x=a.x;y=a.y;return *this;};

	point2df operator + (int i) {return point2df(x+i,y+i);};
	point2df operator - (int i) {return point2df(x-i,y-i);};
	point2df operator * (int i) {return point2df(x*i,y*i);};
	point2df operator / (int i) {return point2df(x/i,y/i);};
	point2df & operator = (int i) {x=i;y=i;return *this;};

	point2df operator + (float f) {return point2df(x+f,y+f);};
	point2df operator - (float f) {return point2df(x-f,y-f);};
	point2df operator * (float f) {return point2df(x*f,y*f);};
	point2df operator / (float f) {return point2df(x/f,y/f);};
	point2df & operator = (float f) {x=f;y=f;return *this;};


	/*ͬpoint2di�ڵĶ�������*/
	int IsLeft(point2df a);
	int IsUp(point2df a);
	float Distance(point2df a);
	float Angle();
	float Angle(point2df a);

public:
	float x;
	float y;
};

class  FilterKernel
{
public:
	int iTempH;				//ģ��ĸ߶�		
	int iTempW;				//ģ��Ŀ��
	int iTempMX;			//ģ������Ԫ��X����( <iTempH - 1 )
	int iTempMY;			//ģ������Ԫ��Y����( <iTempW - 1 )
	float* fpArray;			//ָ��ģ�������ָ�룻
	float fCoef;			//ģ��ϵ��
	FilterKernel()
	{
		fCoef = (float)(1.0 / 9.0);
		iTempH = 3;
		iTempW = 3;
		iTempMX = 1;
		iTempMY = 1;
		fpArray = new float[iTempH * iTempW];
		
		for( int i = 0; i < iTempH * iTempW; i++ )
		{
			fpArray[i] = 1;
		}
	}
	FilterKernel & operator = (FilterKernel &Temp) 
	{
		fCoef = Temp.fCoef;
		iTempH = Temp.iTempH;
		iTempW = Temp.iTempW;
		iTempMX = Temp.iTempMX;
		iTempMY = Temp.iTempMY;
		if( Temp.fpArray != NULL )
		{
			fpArray = new float[iTempH * iTempW];
			for( int i = 0; i < iTempH * iTempW; i++ )
			{
				fpArray[i] = Temp.fpArray[i];
			}
		}
		return *this;
	};
	int SetFilterKernel( int H,int W,int MX,int MY,float* fA = NULL,float fc = 0.11 )
	{
		fCoef = fc;
		iTempH = H;
		iTempW = W;
		iTempMX = MX;
		iTempMY = MY;
		if( fA != NULL )
		{
			fpArray = new float[iTempH * iTempW];
			
			for( int i = 0; i < iTempH * iTempW; i++ )
			{
				fpArray[i] = fA[i];
			}
		}
		return 1;
	}
	~FilterKernel()
	{
		if(fpArray != NULL)
		{
			delete []fpArray;
		}
		
	}
};

