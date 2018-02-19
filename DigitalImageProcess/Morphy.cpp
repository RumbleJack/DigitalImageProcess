#include "stdafx.h"
#include "Morphy.h"
#include <vector>
using std::vector;

inline unsigned int ab(unsigned int x)
{
	return x>0? x:-x;
}

Structure::Structure()
{
	width=3;
	height=3;
	shape=empty;
	center.x=1;
	center.y=1;
	lpStr=new unsigned int[width*height];
	for (int i=0;i<width;i++)
		for(int j=0;j<height;j++)
			*(lpStr+j*width+i)=(unsigned int)0;
}
Structure::Structure(int** array,unsigned int width,unsigned int height)
{
	this->width=width;
	this->height=height;
	this->center.x=width/2;
	this->center.y=height/2;
	this->shape=others;
	lpStr=new unsigned int[width*height];
	for (int i=0;i<width;i++)
		for(int j=0;j<height;j++)
			lpStr[j*width+i]=(unsigned int)array[i][j];
}
Structure::Structure(unsigned int width,unsigned int height)
{
	this->width=width;
	this->height=height;
	this->center.x=width/2;
	this->center.y=height/2;
	this->shape=empty;
	lpStr=new unsigned int[width*height];
	for (int i=0;i<width;i++)
		for(int j=0;j<height;j++)
			*(lpStr+j*width+i)=(unsigned int)0;
}



Structure::~Structure()
{
	delete[]this->lpStr;
}


int Structure::Disk(unsigned int R,unsigned int value)
{
	this->width=2*R+1;
	this->height=2*R+1;
	this->center.x=R;
	this->center.y=R;
	if(lpStr!=NULL)
		delete[]lpStr;
	this->lpStr=new unsigned int[width*height];
	int k=0;
	this->shape=disk;
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			if( (i-R)*(i-R)+(j-R)*(j-R)>R*R )
			{
				lpStr[k]=0;
			}
			else
				lpStr[k]=value;
		}
	}

	return 0;
}

int	Structure::Diamond(unsigned int R,unsigned int value)
{

	this->width=2*R+1;
	this->height=2*R+1;
	this->center.x=R;
	this->center.y=R;
	if(lpStr!=NULL)
		delete[]lpStr;
	this->lpStr=new unsigned int[width*height];
	int k=0;
	this->shape=diamond;
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			if( ab(i-R)+ab(j-R)>R )
			{
				lpStr[k]=0;
			}
			else
				lpStr[k]=value;
		}
	}

	return 0;
}
int	Structure::Rectangle(unsigned int width,unsigned int height,unsigned int value)
{
	this->width=width;
	this->height=height;
	this->center.x=width/2;
	this->center.y=height/2;
	if(lpStr!=NULL)
		delete[]lpStr;
	this->lpStr=new unsigned int[width*height];
	int k=0;
	this->shape=rectangle;
	if (width==height)
		this->shape=square;
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			lpStr[k]=value;
		}
	}

	return 0;
}
int	Structure::Square(unsigned int R,unsigned int value)
{
	return this->Rectangle(R,R,value);
}


int	Structure::Line(unsigned int lengh,int x,int y,unsigned int value)
{
	this->width=(lengh-1)*x;
	this->height=(lengh-1)*y;
	this->center.x=width/2;
	this->center.y=height/2;
	if(lpStr!=NULL)
		delete[]lpStr;
	this->lpStr=new unsigned int[width*height];
	int k=0;
	this->shape=line;
	for (int i=0;i<width;i=i+x)
	{
		for(int j=0;j<height;j=j+y)
		{
			k=j*width+i;
			lpStr[k]=value;
		}
	}

	return 0;

}

int Structure::setValue(unsigned int x,unsigned int y,unsigned int value)
{
	if(x>=width||y>=height) 
		return -3;
	else
	{
		unsigned int k=y*width+x;
		if (lpStr[k]^value==1)
		{
			this->shape=others;
		} 
		lpStr[k]=(unsigned int)value;
		return 0;

	}
	
}
int Structure::setValue(unsigned int value)
{
	unsigned int k=0;
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			lpStr[k]=(unsigned int)value;
		}
	}
	this->shape=rectangle;
	return 0;
}
int Structure::setCenter(point2di p)
{
	if(p.x>=width||p.y>=height||p.x<0||p.y<0) 
		return -3;
	else 
		this->center=p;
	return 0;

}
int Structure::setCenter(unsigned int x,unsigned int y)
{
	if(x>=width||y>=height) 
		return -3;
	else 
		center.x=x;
	center.y=y;
	return 0;
}
Structure & Structure:: operator = (Structure &str)
{
	if (this!=&str)
	{
		if(lpStr!=NULL)
			delete[]lpStr;
		width=str.getWidth();
		height=str.getHeight();
		shape=str.getShape();
		center=str.getCenter();
		lpStr=new unsigned int[width*height];
		memcpy(lpStr,str.lpStr,width*height);
	}
	return *this;
}


unsigned char Index(unsigned char neighbour[][5],unsigned int x)
{
	unsigned int index=x%10;
	switch (index)
	{

	case 0: 
		return neighbour[2][2];
		break;
	case 1: 
		return neighbour[2][2];
		break;
	case 2: 
		return neighbour[1][2];
		break;
	case 3: 
		return neighbour[1][1];
		break;
	case 4: 
		return neighbour[2][1];
		break;
	case 5: 
		return neighbour[3][1];
		break;
	case 6: 
		return neighbour[3][2];
		break;
	case 7: 
		return neighbour[3][3];
		break;
	case 8: 
		return neighbour[2][3];
		break;
	case 9: 
		return neighbour[1][3];
		break;
	default:
		return -1;
		break;
	}
}
unsigned char NZ(unsigned char neighbour[][5])
{
	return
		(Index(neighbour,2)+Index(neighbour,3)+Index(neighbour,4)+Index(neighbour,5)+
		Index(neighbour,6)+Index(neighbour,7)+Index(neighbour,8)+Index(neighbour,9));
}

unsigned char Z0(unsigned char neighbour[][5],unsigned int x)
{
	unsigned char result=0;
	unsigned char index=x%10;
	int i=0;
	int j=0;
	switch (index)
	{

	case 0: 
		i=2;j=2;break;
	case 1: 
		i=2;j=2;break;
	case 2: 
		i=1;j=2;break;
	case 3: 
		i=1;j=1;break;
	case 4: 
		i=2;j=1;break;
	case 5: 
		i=3;j=1;break;
	case 6: 
		i=3;j=2;break;
	case 7: 
		i=3;j=3;break;
	case 8: 
		i=2;j=3;break;
	case 9: 
		i=1;j=3;break;
	default:
		break;
	}
	if (neighbour[i-1][j]==0&&neighbour[i-1][j-1]==1)
		result++;
	if (neighbour[i-1][j-1]==0&&neighbour[i][j-1]==1)
		result++;
	if (neighbour[i][j-1]==0&&neighbour[i+1][j-1]==1)
		result++;
	if (neighbour[i+1][j-1]==0&&neighbour[i+1][j]==1)
		result++;
	if (neighbour[i+1][j]==0&&neighbour[i+1][j+1]==1)
		result++;
	if (neighbour[i+1][j+1]==0&&neighbour[i][j+1]==1)
		result++;
	if (neighbour[i][j+1]==0&&neighbour[i-1][j+1]==1)
		result++;
	if (neighbour[i-1][j+1]==0&&neighbour[i-1][j]==1)
		result++;


	return result;

}



unsigned char Index(unsigned int neighbour[][5],unsigned int x)
{
	unsigned int index=x%10;
	switch (index)
	{

	case 0: 
		return neighbour[2][2];
		break;
	case 1: 
		return neighbour[2][2];
		break;
	case 2: 
		return neighbour[1][2];
		break;
	case 3: 
		return neighbour[1][1];
		break;
	case 4: 
		return neighbour[2][1];
		break;
	case 5: 
		return neighbour[3][1];
		break;
	case 6: 
		return neighbour[3][2];
		break;
	case 7: 
		return neighbour[3][3];
		break;
	case 8: 
		return neighbour[2][3];
		break;
	case 9: 
		return neighbour[1][3];
		break;
	default:
		return -1;
		break;
	}
}
unsigned char NZ(unsigned int neighbour[][5])
{
	return
		(Index(neighbour,2)+Index(neighbour,3)+Index(neighbour,4)+Index(neighbour,5)+
		Index(neighbour,6)+Index(neighbour,7)+Index(neighbour,8)+Index(neighbour,9));
}

unsigned char Z0(unsigned int neighbour[][5],unsigned int x)
{
	unsigned char result=0;
	unsigned char index=x%10;
	int i=0;
	int j=0;
	switch (index)
	{

	case 0: 
		i=2;j=2;break;
	case 1: 
		i=2;j=2;break;
	case 2: 
		i=1;j=2;break;
	case 3: 
		i=1;j=1;break;
	case 4: 
		i=2;j=1;break;
	case 5: 
		i=3;j=1;break;
	case 6: 
		i=3;j=2;break;
	case 7: 
		i=3;j=3;break;
	case 8: 
		i=2;j=3;break;
	case 9: 
		i=1;j=3;break;
	default:
		break;
	}
	if (neighbour[i-1][j]==0&&neighbour[i-1][j-1]==1)
		result++;
	if (neighbour[i-1][j-1]==0&&neighbour[i][j-1]==1)
		result++;
	if (neighbour[i][j-1]==0&&neighbour[i+1][j-1]==1)
		result++;
	if (neighbour[i+1][j-1]==0&&neighbour[i+1][j]==1)
		result++;
	if (neighbour[i+1][j]==0&&neighbour[i+1][j+1]==1)
		result++;
	if (neighbour[i+1][j+1]==0&&neighbour[i][j+1]==1)
		result++;
	if (neighbour[i][j+1]==0&&neighbour[i-1][j+1]==1)
		result++;
	if (neighbour[i-1][j+1]==0&&neighbour[i-1][j]==1)
		result++;


	return result;

}



int Morphy::Logical_8( ListImage limage,ListImage& resultlimage,unsigned int value)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if( limage.GetImgDataType()!=uint_8 )
		return -2;
	resultlimage=limage;
	unsigned char* buffer=(unsigned char*)resultlimage.GetImgBuffer();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int k=0;
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			if(buffer[k]!=0)
				buffer[k]=(unsigned char)value;
		}
	}
		return 0;
}
int Morphy::Logical_12( ListImage limage,ListImage& resultlimage,unsigned int value)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if( limage.GetImgDataType()!=uint_12 )
		return -2;
	resultlimage=limage;
	value=value&0x0fff;
	short* buffer=(short*)resultlimage.GetImgBuffer();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int k=0;
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			if(buffer[k]!=0)
				buffer[k]=(short)value;

		}
	}
		return 0;
}

int Morphy::Logical_16( ListImage limage,ListImage& resultlimage,unsigned int value)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if( limage.GetImgDataType()!=uint_16 )
		return -2;
	resultlimage=limage;
	short* buffer=(short*)resultlimage.GetImgBuffer();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int k=0;
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			if(buffer[k]!=0)
				buffer[k]=(short)value;
		}
	}
		return 0;
}

int	Morphy::setValue_8( ListImage limage,ListImage& resultlimage,unsigned int x,unsigned int y,unsigned char value)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if( limage.GetImgDataType()!=uint_8 )
		return -2;
	if (x>=limage.GetImgWidth()||limage.GetImgHeight())
		return -3;
	resultlimage=limage;
	unsigned char* buffer=(unsigned char*)resultlimage.GetImgBuffer();
	unsigned int width=resultlimage.GetImgWidth();
	buffer[y*width+x]=value;
	return 0;


}

int	Morphy::setValue_12( ListImage limage,ListImage& resultlimage,unsigned int x,unsigned int y,short value)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if( limage.GetImgDataType()!=uint_12)
		return -2;
	if (x>=limage.GetImgWidth()||limage.GetImgHeight())
		return -3;
	resultlimage=limage;
	value=value&0x0fff;
	short* buffer=(short*)resultlimage.GetImgBuffer();
	unsigned int width=resultlimage.GetImgWidth();
	buffer[y*width+x]=value;
	return 0;


}
int	Morphy::setValue_16( ListImage limage,ListImage& resultlimage,unsigned int x,unsigned int y,short value)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if( limage.GetImgDataType()!=uint_16)
		return -2;
	if (x>=limage.GetImgWidth()||limage.GetImgHeight())
		return -3;
	resultlimage=limage;
	short* buffer=(short*)resultlimage.GetImgBuffer();
	unsigned int width=resultlimage.GetImgWidth();
	buffer[y*width+x]=value;
	return 0;


}
int	Morphy::getValue_8( ListImage limage,unsigned int x,unsigned int y)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if( limage.GetImgDataType()!=uint_8 )
		return -2;
	if (x>=limage.GetImgWidth()||y>=limage.GetImgHeight())
		return -3;

	unsigned char* buffer=(unsigned char*)limage.GetImgBuffer();
	unsigned int width=limage.GetImgWidth();

	return buffer[y*width+x];


}

int	Morphy::getValue_12( ListImage limage,unsigned int x,unsigned int y)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if( limage.GetImgDataType()!=uint_12 )
		return -2;
	if (x>=limage.GetImgWidth()||y>=limage.GetImgHeight())
		return -3;

	short* buffer=(short*)limage.GetImgBuffer();
	unsigned int width=limage.GetImgWidth();

	return buffer[y*width+x];


}

int	Morphy::getValue_16( ListImage limage,unsigned int x,unsigned int y)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if( limage.GetImgDataType()!=uint_16 )
		return -2;
	if (x>=limage.GetImgWidth()||y>=limage.GetImgHeight())
		return -3;

	short* buffer=(short*)limage.GetImgBuffer();
	unsigned int width=limage.GetImgWidth();

	return buffer[y*width+x];


}


int Morphy::Equal_8( ListImage limageA, ListImage limageB)
{
	if (limageA.GetImgChannel()!=1||limageA.GetImgChannel()!=1)
	{
		return -1;
	}
	if (limageA.GetImgDataType()!=uint_8||limageB.GetImgDataType()!=uint_8)
	{
		return -2;
	}

	unsigned int width=limageA.GetImgWidth();
	unsigned int height=limageA.GetImgHeight();
	if (width!=limageB.GetImgWidth()||height!=limageB.GetImgHeight())
	{
		return 1;
	}

	unsigned char* buffera=(unsigned char*)(limageA.GetImgBuffer());
	unsigned char* bufferb=(unsigned char*)(limageB.GetImgBuffer());
	unsigned int k=0;
	for(int j=0;j<height;j++)
	{
		for (int i=0;i<width;i++)
		{
			k=j*width+i;
			if (buffera[k]!=bufferb[k])
				return 1;
		}
	}

		return 0;
}


int Morphy::Equal_12( ListImage limageA, ListImage limageB)
{
	if (limageA.GetImgChannel()!=1||limageA.GetImgChannel()!=1)
	{
		return -1;
	}
	if (limageA.GetImgDataType()!=uint_12||limageB.GetImgDataType()!=uint_12)
	{
		return -2;
	}

	unsigned int width=limageA.GetImgWidth();
	unsigned int height=limageA.GetImgHeight();
	if (width!=limageB.GetImgWidth()||height!=limageB.GetImgHeight())
	{
		return 1;
	}

	short* buffera=(short*)(limageA.GetImgBuffer());
	short* bufferb=(short*)(limageB.GetImgBuffer());
	unsigned int k=0;
	for(int j=0;j<height;j++)
	{
		for (int i=0;i<width;i++)
		{
			k=j*width+i;
			if (buffera[k]!=bufferb[k])
				return 1;
		}
	}

		return 0;
}


int Morphy::Equal_16( ListImage limageA, ListImage limageB)
{
	if (limageA.GetImgChannel()!=1||limageA.GetImgChannel()!=1)
	{
		return -1;
	}
	if (limageA.GetImgDataType()!=uint_16||limageB.GetImgDataType()!=uint_16)
	{
		return -2;
	}

	unsigned int width=limageA.GetImgWidth();
	unsigned int height=limageA.GetImgHeight();
	if (width!=limageB.GetImgWidth()||height!=limageB.GetImgHeight())
	{
		return 1;
	}

	short* buffera=(short*)(limageA.GetImgBuffer());
	short* bufferb=(short*)(limageB.GetImgBuffer());
	unsigned int k=0;
	for(int j=0;j<height;j++)
	{
		for (int i=0;i<width;i++)
		{
			k=j*width+i;
			if (buffera[k]!=bufferb[k])
				return 1;
		}
	}

		return 0;
}


int Morphy::Complement_8( ListImage limage,ListImage& resultlimage)

{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_8)
		return -2;
	resultlimage=limage;
	unsigned char* buffer=(unsigned char*)(limage.GetImgBuffer());
	unsigned char* resultbuffer=(unsigned char*)(resultlimage.GetImgBuffer());
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int k=0;//下标索引

	for(int j=0;j<height;j++)
	{
		for (int i=0;i<width;i++)
		{
			k=j*width+i;
			resultbuffer[k]=255-buffer[k];

		}
	}
	return 0;
}

int Morphy::Complement_12( ListImage limage,ListImage& resultlimage)

{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_12)
		return -2;
	resultlimage=limage;
	short* buffer=(short*)(limage.GetImgBuffer());
	short* resultbuffer=(short*)(resultlimage.GetImgBuffer());
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int k=0;//下标索引

	for(int j=0;j<height;j++)
	{
		for (int i=0;i<width;i++)
		{
			k=j*width+i;
			resultbuffer[k]=4095-buffer[k];

		}
	}
	return 0;
}

int Morphy::Complement_16( ListImage limage,ListImage& resultlimage)

{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_16)
		return -2;
	resultlimage=limage;
	short* buffer=(short*)(limage.GetImgBuffer());
	short* resultbuffer=(short*)(resultlimage.GetImgBuffer());
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int k=0;//下标索引

	for(int j=0;j<height;j++)
	{
		for (int i=0;i<width;i++)
		{
			k=j*width+i;
			resultbuffer[k]=65535-buffer[k];

		}
	}
	return 0;
}

int Morphy::Complement_8( ListImage limageA, ListImage limageB,ListImage& resultlimage)
{
	if (limageA.GetImgChannel()!=1||limageB.GetImgChannel()!=1 )
		return -1;
	if (limageA.GetImgDataType()!=uint_8||limageB.GetImgDataType()!=uint_8 )
		return -2;

	resultlimage=limageA;
	unsigned char *bufferB=limageB.GetImgBuffer();
	unsigned char *resultbuffer=resultlimage.GetImgBuffer();
	unsigned int widthA=limageA.GetImgWidth();
	unsigned int heightA=limageA.GetImgHeight();
	unsigned int widthB=limageB.GetImgWidth();
	unsigned int heightB=limageB.GetImgHeight();
	unsigned int width=min(widthA,widthB);
	unsigned int height=min(heightA,heightB);
	unsigned int kA=0;
	unsigned int kB=0;


	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)

		{
			kA=j*widthA+i;
			kB=j*widthB+i;
			if (bufferB[kB]!=0)
				resultbuffer[kA]=0;
		}
	}

	return 0;

}
int Morphy::Complement_12( ListImage limageA, ListImage limageB,ListImage& resultlimage)
{
	if (limageA.GetImgChannel()!=1||limageB.GetImgChannel()!=1 )
		return -1;
	if (limageA.GetImgDataType()!=uint_12||limageB.GetImgDataType()!=uint_12 )
		return -2;

	resultlimage=limageA;
	short* buffer=(short*)limageA.GetImgBuffer();
	short *bufferB=(short*)limageB.GetImgBuffer();
	short *resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int widthA=limageA.GetImgWidth();
	unsigned int heightA=limageA.GetImgHeight();
	unsigned int widthB=limageB.GetImgWidth();
	unsigned int heightB=limageB.GetImgHeight();
	unsigned int width=min(widthA,widthB);
	unsigned int height=min(heightA,heightB);
	unsigned int kA=0;
	unsigned int kB=0;


	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)

		{
			kA=j*widthA+i;
			kB=j*widthB+i;
			if (bufferB[kB]!=0)
				resultbuffer[kA]=0;
		}
	}

	return 0;

}




int Morphy::Complement_16( ListImage limageA, ListImage limageB,ListImage& resultlimage)
{
	if (limageA.GetImgChannel()!=1||limageB.GetImgChannel()!=1 )
		return -1;
	if (limageA.GetImgDataType()!=uint_16||limageB.GetImgDataType()!=uint_16 )
		return -2;

	resultlimage=limageA;
	short* buffer=(short*)limageA.GetImgBuffer();
	short *bufferB=(short*)limageB.GetImgBuffer();
	short *resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int widthA=limageA.GetImgWidth();
	unsigned int heightA=limageA.GetImgHeight();
	unsigned int widthB=limageB.GetImgWidth();
	unsigned int heightB=limageB.GetImgHeight();
	unsigned int width=min(widthA,widthB);
	unsigned int height=min(heightA,heightB);
	unsigned int kA=0;
	unsigned int kB=0;


	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)

		{
			kA=j*widthA+i;
			kB=j*widthB+i;
			if (bufferB[kB]!=0)
				resultbuffer[kA]=0;
		}
	}

	return 0;

}




int Morphy::And_8( ListImage limageA, ListImage limageB,ListImage& resultlimage)
{
	if (limageA.GetImgChannel()!=1||limageB.GetImgChannel()!=1 )
		return -1;
	if (limageA.GetImgDataType()!=uint_8||limageB.GetImgDataType()!=uint_8 )
		return -2;

	resultlimage=limageA;
	unsigned char *bufferB=limageB.GetImgBuffer();
	unsigned char *resultbuffer=resultlimage.GetImgBuffer();
	unsigned int widthA=limageA.GetImgWidth();
	unsigned int heightA=limageA.GetImgHeight();
	unsigned int widthB=limageB.GetImgWidth();
	unsigned int heightB=limageB.GetImgHeight();
	unsigned int width=min(widthA,widthB);
	unsigned int height=min(widthA,widthB);
	unsigned int kA=0;
	unsigned int kB=0;


	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)

		{
			kA=j*widthA+i;
			kB=j*widthB+i;
			if (bufferB[kB]==0||resultbuffer[kA]==0)
				resultbuffer[kA]=0;
		}
	}

	return 0;
}


int Morphy::And_12( ListImage limageA, ListImage limageB,ListImage& resultlimage)
{
	if (limageA.GetImgChannel()!=1||limageB.GetImgChannel()!=1 )
		return -1;
	if (limageA.GetImgDataType()!=uint_12||limageB.GetImgDataType()!=uint_12 )
		return -2;

	resultlimage=limageA;
	short *bufferB=(short*)limageB.GetImgBuffer();
	short *resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int widthA=limageA.GetImgWidth();
	unsigned int heightA=limageA.GetImgHeight();
	unsigned int widthB=limageB.GetImgWidth();
	unsigned int heightB=limageB.GetImgHeight();
	unsigned int width=min(widthA,widthB);
	unsigned int height=min(widthA,widthB);
	unsigned int kA=0;
	unsigned int kB=0;


	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)

		{
			kA=j*widthA+i;
			kB=j*widthB+i;
			if (bufferB[kB]==0||resultbuffer[kA]==0)
				resultbuffer[kA]=0;
		}
	}

	return 0;
}


int Morphy::And_16( ListImage limageA, ListImage limageB,ListImage& resultlimage)
{
	if (limageA.GetImgChannel()!=1||limageB.GetImgChannel()!=1 )
		return -1;
	if (limageA.GetImgDataType()!=uint_16||limageB.GetImgDataType()!=uint_16 )
		return -2;

	resultlimage=limageA;
	short *bufferB=(short*)limageB.GetImgBuffer();
	short *resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int widthA=limageA.GetImgWidth();
	unsigned int heightA=limageA.GetImgHeight();
	unsigned int widthB=limageB.GetImgWidth();
	unsigned int heightB=limageB.GetImgHeight();
	unsigned int width=min(widthA,widthB);
	unsigned int height=min(widthA,widthB);
	unsigned int kA=0;
	unsigned int kB=0;


	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)

		{
			kA=j*widthA+i;
			kB=j*widthB+i;
			if (bufferB[kB]==0||resultbuffer[kA]==0)
				resultbuffer[kA]=0;
		}
	}

	return 0;
}


int Morphy::Or_8( ListImage limageA, ListImage limageB,ListImage& resultlimage,unsigned int value)
{
	if (limageA.GetImgChannel()!=1||limageB.GetImgChannel()!=1 )
		return -1;
	if (limageA.GetImgDataType()!=uint_8||limageB.GetImgDataType()!=uint_8 )
		return -2;

	resultlimage=limageA;
	unsigned char *bufferB=limageB.GetImgBuffer();
	unsigned char *resultbuffer=resultlimage.GetImgBuffer();
	unsigned int widthA=limageA.GetImgWidth();
	unsigned int heightA=limageA.GetImgHeight();
	unsigned int widthB=limageB.GetImgWidth();
	unsigned int heightB=limageB.GetImgHeight();
	unsigned int width=min(widthA,widthB);
	unsigned int height=min(widthA,widthB);
	unsigned int kA=0;
	unsigned int kB=0;


	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)

		{
			kA=j*widthA+i;
			kB=j*widthB+i;
			if (bufferB[kB]!=0||resultbuffer[kA]!=0)
				resultbuffer[kA]=(unsigned char)value;
		}
	}

	return 0;
}



int Morphy::Or_12( ListImage limageA, ListImage limageB,ListImage& resultlimage,unsigned int value)
{
	if (limageA.GetImgChannel()!=1||limageB.GetImgChannel()!=1 )
		return -1;
	if (limageA.GetImgDataType()!=uint_12||limageB.GetImgDataType()!=uint_12 )
		return -2;

	resultlimage=limageA;
	short *bufferB=(short*)limageB.GetImgBuffer();
	short *resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int widthA=limageA.GetImgWidth();
	unsigned int heightA=limageA.GetImgHeight();
	unsigned int widthB=limageB.GetImgWidth();
	unsigned int heightB=limageB.GetImgHeight();
	unsigned int width=min(widthA,widthB);
	unsigned int height=min(widthA,widthB);
	unsigned int kA=0;
	unsigned int kB=0;
	value=value&0x0fff;

	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)

		{
			kA=j*widthA+i;
			kB=j*widthB+i;
			if (bufferB[kB]!=0||resultbuffer[kA]!=0)
				resultbuffer[kA]=(short)value;
		}
	}

	return 0;
}



int Morphy::Or_16( ListImage limageA, ListImage limageB,ListImage& resultlimage,unsigned int value)
{
	if (limageA.GetImgChannel()!=1||limageB.GetImgChannel()!=1 )
		return -1;
	if (limageA.GetImgDataType()!=uint_16||limageB.GetImgDataType()!=uint_16 )
		return -2;

	resultlimage=limageA;
	short *bufferB=(short*)limageB.GetImgBuffer();
	short *resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int widthA=limageA.GetImgWidth();
	unsigned int heightA=limageA.GetImgHeight();
	unsigned int widthB=limageB.GetImgWidth();
	unsigned int heightB=limageB.GetImgHeight();
	unsigned int width=min(widthA,widthB);
	unsigned int height=min(widthA,widthB);
	unsigned int kA=0;
	unsigned int kB=0;


	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)

		{
			kA=j*widthA+i;
			kB=j*widthB+i;
			if (bufferB[kB]!=0||resultbuffer[kA]!=0)
				resultbuffer[kA]=(short)value;
		}
	}

	return 0;
}



int Morphy::Count_8( ListImage limage,unsigned char value)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_8)
		return -2;

	unsigned int count=0;
	unsigned char* buffer=(unsigned char*)limage.GetImgBuffer();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			if(buffer[j*width+i]>value)
				count++;
		}
	}

		return count;
}



int Morphy::Count_12( ListImage limage,short value)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_12)
		return -2;

	unsigned int count=0;
	short* buffer=(short*)limage.GetImgBuffer();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	value=value&0x0fff;
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			if(buffer[j*width+i]>value)
				count++;
		}
	}

		return count;
}



int Morphy::Count_16( ListImage limage,short value)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_16)
		return -2;

	unsigned int count=0;
	short* buffer=(short*)limage.GetImgBuffer();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			if(buffer[j*width+i]>value)
				count++;
		}
	}

		return count;
}




int Morphy::Erode_8( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_8)
		return -2;

	resultlimage=limage;
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int strwidth=this->structure->getWidth();
	unsigned int strheight=this->structure->getHeight();
	unsigned char* buffer=limage.GetImgBuffer();
	unsigned char* resultbuffer=resultlimage.GetImgBuffer();
	unsigned int kofupleft=0;//掩模左上角索引
	unsigned int k=0;//掩模遍历索引
	unsigned int kofcenter=0;//掩模原点索引
	for (int i=0;i<width-strwidth+1;i++)
		for(int j=0;j<height-strheight+1;j++)
		{
			kofupleft=j*(width)+i;
			kofcenter=kofupleft+this->structure->getCenter().y*(width)+this->structure->getCenter().x;

			for (int x=0;x<strwidth;x++)
				for(int y=0;y<strheight;y++)
				{
					k=kofupleft+y*(width)+x;

					if ( (this->structure->getValue(x,y)!=0) && (buffer[k]==0) ) 
					{
						resultbuffer[kofcenter]=0;
						x=strwidth;
						y=strheight;
					}

				}
		}

		return 0;
}


int Morphy::Erode_12( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_12)
		return -2;

	resultlimage=limage;
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int strwidth=this->structure->getWidth();
	unsigned int strheight=this->structure->getHeight();
	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int kofupleft=0;//掩模左上角索引
	unsigned int k=0;//掩模遍历索引
	unsigned int kofcenter=0;//掩模原点索引
	for (int i=0;i<width-strwidth+1;i++)
	{
		for(int j=0;j<height-strheight+1;j++)
		{
			kofupleft=j*width+i;
			kofcenter=kofupleft+this->structure->getCenter().y*width+this->structure->getCenter().x;

			for (int x=0;x<strwidth;x++)
				for(int y=0;y<strheight;y++)
				{
					k=kofupleft+y*width+x;

					if ( (this->structure->getValue(x,y)!=0) && (buffer[k]==0) ) 
					{
						resultbuffer[kofcenter]=0;
						x=strwidth;
						y=strheight;
					}

				}
		}
	}

		return 0;
}


int Morphy::Erode_16( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_16)
		return -2;

	resultlimage=limage;
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int strwidth=this->structure->getWidth();
	unsigned int strheight=this->structure->getHeight();
	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int kofupleft=0;//掩模左上角索引
	unsigned int k=0;//掩模遍历索引
	unsigned int kofcenter=0;//掩模原点索引
	for (int i=0;i<width-strwidth+1;i++)
	{
		for(int j=0;j<height-strheight+1;j++)
		{
			kofupleft=j*width+i;
			kofcenter=kofupleft+this->structure->getCenter().y*width+this->structure->getCenter().x;

			for (int x=0;x<strwidth;x++)
				for(int y=0;y<strheight;y++)
				{
					k=kofupleft+y*width+x;

					if ( (this->structure->getValue(x,y)!=0) && (buffer[k]==0) ) 
					{
						resultbuffer[kofcenter]=0;
						x=strwidth;
						y=strheight;
					}

				}
		}
	}

		return 0;
}


int Morphy::Dialate_8( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_8)
		return -2;

	resultlimage=limage;
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int strwidth=this->structure->getWidth();
	unsigned int strheight=this->structure->getHeight();
	unsigned char* buffer=limage.GetImgBuffer();
	unsigned char* resultbuffer=resultlimage.GetImgBuffer();
	unsigned int kofupleft=0;//掩模左上角索引
	unsigned int k=0;//掩模遍历索引
	unsigned int kofcenter=0;//掩模原点索引
	for (int i=0;i<width-strwidth+1;i++)
	{
		for(int j=0;j<height-strheight+1;j++)
		{
			kofupleft=j*width+i;
			kofcenter=kofupleft+this->structure->getCenter().y*width+this->structure->getCenter().x;
			if (buffer[kofcenter]!=0)
			{
				for (int x=0;x<strwidth;x++)
					for(int y=0;y<strheight;y++)
					{
						k=kofupleft+y*width+x;
						if ( this->structure->getValue(x,y)!=0 ) 		
							resultbuffer[k]=255;
					}
			}

		}
	}

		return 0;
}

int Morphy::Dialate_12( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_12)
		return -2;

	resultlimage=limage;
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int strwidth=this->structure->getWidth();
	unsigned int strheight=this->structure->getHeight();
	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int kofupleft=0;//掩模左上角索引
	unsigned int k=0;//掩模遍历索引
	unsigned int kofcenter=0;//掩模原点索引
	for (int i=0;i<width-strwidth+1;i++)
	{
		for(int j=0;j<height-strheight+1;j++)
		{
			kofupleft=j*width+i;
			kofcenter=kofupleft+this->structure->getCenter().y*width+this->structure->getCenter().x;
			if (buffer[kofcenter]!=0)
			{
				for (int x=0;x<strwidth;x++)
					for(int y=0;y<strheight;y++)
					{
						k=kofupleft+y*width+x;
						if ( this->structure->getValue(x,y)!=0 ) 		
							resultbuffer[k]=4095;
					}
			}

		}
	}

		return 0;
}

int Morphy::Dialate_16( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_16)
		return -2;

	resultlimage=limage;
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int strwidth=this->structure->getWidth();
	unsigned int strheight=this->structure->getHeight();
	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int kofupleft=0;//掩模左上角索引
	unsigned int k=0;//掩模遍历索引
	unsigned int kofcenter=0;//掩模原点索引
	for (int i=0;i<width-strwidth+1;i++)
	{
		for(int j=0;j<height-strheight+1;j++)
		{
			kofupleft=j*width+i;
			kofcenter=kofupleft+this->structure->getCenter().y*width+this->structure->getCenter().x;
			if (buffer[kofcenter]!=0)
			{
				for (int x=0;x<strwidth;x++)
					for(int y=0;y<strheight;y++)
					{
						k=kofupleft+y*width+x;
						if ( this->structure->getValue(x,y)!=0 ) 		
							resultbuffer[k]=65535;
					}
			}

		}
	}

		return 0;
}

int Morphy::Erodegray_8( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_8)
		return -2;

	resultlimage=limage;
	resultlimage.SetZero();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int strwidth=structure->getWidth();
	unsigned int strheight=structure->getHeight();
	unsigned int center_x=structure->getCenter().x;
	unsigned int center_y=structure->getCenter().y;
	unsigned char* buffer=limage.GetImgBuffer();
	unsigned char* resultbuffer=resultlimage.GetImgBuffer();
	unsigned char minimun;
	unsigned char mini2;
	unsigned int kofcenter=0;//掩模左上角索引
	unsigned int k=0;//掩模遍历索引
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			kofcenter=j*width+i;
			minimun=buffer[kofcenter];
			for (int m=0;m<strwidth;m++)
				for (int n=0;n<strheight;n++)
				{
					int s=i+m-center_x;
					int t=j+n-center_y;
					if (s<width&&s>=0&&t<height&&t>=0)
					{
						k=t*width+s;
						mini2=buffer[k]-structure->getValue(m,n);
						if (mini2<0)
							mini2=0;
						if (mini2<minimun)
							minimun=mini2;
					}

				}

				resultbuffer[kofcenter]=minimun;
		}
	}
		return 0;
}

int Morphy::Erodegray_12( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_12)
		return -2;

	resultlimage=limage;
	resultlimage.SetZero();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int strwidth=structure->getWidth();
	unsigned int strheight=structure->getHeight();
	unsigned int center_x=structure->getCenter().x;
	unsigned int center_y=structure->getCenter().y;
	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();
	short minimun;
	short mini2;
	unsigned int kofcenter=0;//掩模左上角索引
	unsigned int k=0;//掩模遍历索引
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			kofcenter=j*width+i;
			minimun=buffer[kofcenter];
			for (int m=0;m<strwidth;m++)
				for (int n=0;n<strheight;n++)
				{
					int s=i+m-center_x;
					int t=j+n-center_y;
					if (s<width&&s>=0&&t<height&&t>=0)
					{
						k=t*width+s;
						mini2=buffer[k]-structure->getValue(m,n);
						if (mini2<0)
							mini2=0;
						if (mini2<minimun)
							minimun=mini2;
					}

				}

				resultbuffer[kofcenter]=minimun;
		}
	}
		return 0;
}

int Morphy::Erodegray_16( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_16)
		return -2;

	resultlimage=limage;
	resultlimage.SetZero();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int strwidth=structure->getWidth();
	unsigned int strheight=structure->getHeight();
	unsigned int center_x=structure->getCenter().x;
	unsigned int center_y=structure->getCenter().y;
	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();
	short minimun;
	short mini2;
	unsigned int kofcenter=0;//掩模左上角索引
	unsigned int k=0;//掩模遍历索引
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			kofcenter=j*width+i;
			minimun=buffer[kofcenter];
			for (int m=0;m<strwidth;m++)
				for (int n=0;n<strheight;n++)
				{
					int s=i+m-center_x;
					int t=j+n-center_y;
					if (s<width&&s>=0&&t<height&&t>=0)
					{
						k=t*width+s;
						mini2=buffer[k]-structure->getValue(m,n);
						if (mini2<0)
							mini2=0;
						if (mini2<minimun)
							minimun=mini2;
					}

				}

				resultbuffer[kofcenter]=minimun;
		}
	}
		return 0;
}

int Morphy::Dialategray_8( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_8)
		return -2;

	resultlimage=limage;
	resultlimage.SetZero();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int strwidth=structure->getWidth();
	unsigned int strheight=structure->getHeight();
	unsigned int center_x=structure->getCenter().x;
	unsigned int center_y=structure->getCenter().y;
	unsigned char* buffer=limage.GetImgBuffer();
	unsigned char* resultbuffer=resultlimage.GetImgBuffer();
	unsigned char maximum;
	unsigned char max2;
	unsigned int kofcenter=0;//掩模左上角索引
	unsigned int k=0;//掩模遍历索引
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			kofcenter=j*width+i;
			maximum=buffer[kofcenter];
			for (int m=0;m<strwidth;m++)
				for (int n=0;n<strheight;n++)
				{
					int s=i-(m-center_x);
					int t=j-(n-center_y);
					if (s<width&&s>=0&&t<height&&t>=0)
					{
						k=t*width+s;
						max2=buffer[k]+structure->getValue(m,n);
						if (max2>255)
							max2=255;
						if (max2>maximum)
							maximum=max2;
					}

				}

				resultbuffer[kofcenter]=maximum;
		}
	}
		return 0;
}


int Morphy::Dialategray_12( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_12)
		return -2;

	resultlimage=limage;
	resultlimage.SetZero();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int strwidth=structure->getWidth();
	unsigned int strheight=structure->getHeight();
	unsigned int center_x=structure->getCenter().x;
	unsigned int center_y=structure->getCenter().y;
	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();
	short maximum;
	short max2;
	unsigned int kofcenter=0;//掩模左上角索引
	unsigned int k=0;//掩模遍历索引
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			kofcenter=j*width+i;
			maximum=buffer[kofcenter];
			for (int m=0;m<strwidth;m++)
				for (int n=0;n<strheight;n++)
				{
					int s=i-(m-center_x);
					int t=j-(n-center_y);
					if (s<width&&s>=0&&t<height&&t>=0)
					{
						k=t*width+s;
						max2=buffer[k]+structure->getValue(m,n);
						if (max2>4095)
							max2=4095;
						if (max2>maximum)
							maximum=max2;
					}

				}

				resultbuffer[kofcenter]=maximum;
		}
	}
		return 0;
}


int Morphy::Dialategray_16( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_16)
		return -2;

	resultlimage=limage;
	resultlimage.SetZero();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int strwidth=structure->getWidth();
	unsigned int strheight=structure->getHeight();
	unsigned int center_x=structure->getCenter().x;
	unsigned int center_y=structure->getCenter().y;
	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();
	short maximum;
	short max2;
	unsigned int kofcenter=0;//掩模左上角索引
	unsigned int k=0;//掩模遍历索引
	for (int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			kofcenter=j*width+i;
			maximum=buffer[kofcenter];
			for (int m=0;m<strwidth;m++)
				for (int n=0;n<strheight;n++)
				{
					int s=i-(m-center_x);
					int t=j-(n-center_y);
					if (s<width&&s>=0&&t<height&&t>=0)
					{
						k=t*width+s;
						max2=buffer[k]+structure->getValue(m,n);
						if (max2>65535)
							max2=65535;
						if (max2>maximum)
							maximum=max2;
					}

				}

				resultbuffer[kofcenter]=maximum;
		}
	}
		return 0;
}


int Morphy::Open_8( ListImage limage,ListImage& resultlimage)
{
	int erode=Erode_8(limage,resultlimage);
	if (erode==0)
		return Dialate_8(limage,resultlimage);
	else 
		return erode;
}

int Morphy::Open_12( ListImage limage,ListImage& resultlimage)
{
	int erode=Erode_12(limage,resultlimage);
	if (erode==0)
		return Dialate_12(limage,resultlimage);
	else 
		return erode;
}


int Morphy::Open_16( ListImage limage,ListImage& resultlimage)
{
	int erode=Erode_16(limage,resultlimage);
	if (erode==0)
		return Dialate_16(limage,resultlimage);
	else 
		return erode;
}


int Morphy::Close_8( ListImage limage,ListImage& resultlimage)
{
	int dialate=Dialate_8(limage,resultlimage);
	if (dialate==0)
		return Erode_8(limage,resultlimage);
	else 
		return dialate;
}

int Morphy::Close_12( ListImage limage,ListImage& resultlimage)
{
	int dialate=Dialate_12(limage,resultlimage);
	if (dialate==0)
		return Erode_12(limage,resultlimage);
	else 
		return dialate;
}

int Morphy::Close_16( ListImage limage,ListImage& resultlimage)
{
	int dialate=Dialate_16(limage,resultlimage);
	if (dialate==0)
		return Erode_16(limage,resultlimage);
	else 
		return dialate;
}

int Morphy::Opengray_8( ListImage limage,ListImage& resultlimage)
{
	ListImage bufflimage;
	int erode=Erodegray_8(limage,bufflimage);
	if (erode==0)
		return Dialategray_8(bufflimage,resultlimage);
	else 
		return erode;
}
int Morphy::Opengray_12( ListImage limage,ListImage& resultlimage)
{
	ListImage bufflimage;
	int erode=Erodegray_12(limage,bufflimage);
	if (erode==0)
		return Dialategray_12(bufflimage,resultlimage);
	else 
		return erode;
}

int Morphy::Opengray_16( ListImage limage,ListImage& resultlimage)
{
	ListImage bufflimage;
	int erode=Erodegray_16(limage,bufflimage);
	if (erode==0)
		return Dialategray_16(bufflimage,resultlimage);
	else 
		return erode;
}

int Morphy::Closegray_8( ListImage limage,ListImage& resultlimage)
{
	ListImage bufflimage;
	int dialate=Dialategray_8(limage,bufflimage);
	if (dialate==0)		
		return Erodegray_8(bufflimage,resultlimage);	
	else 
		return dialate;
}
int Morphy::Closegray_12( ListImage limage,ListImage& resultlimage)
{
	ListImage bufflimage;
	int dialate=Dialategray_12(limage,bufflimage);
	if (dialate==0)		
		return Erodegray_12(bufflimage,resultlimage);	
	else 
		return dialate;
}
int Morphy::Closegray_16( ListImage limage,ListImage& resultlimage)
{
	ListImage bufflimage;
	int dialate=Dialategray_16(limage,bufflimage);
	if (dialate==0)		
		return Erodegray_16(bufflimage,resultlimage);	
	else 
		return dialate;
}
int Morphy::Thining_8( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_8)
		return -2;

	unsigned char neighbour[5][5];
	bool Condition1=false;
	bool Condition2=false;
	bool Condition3=false;
	bool Condition4=false;
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned char* buffer=limage.GetImgBuffer();
	resultlimage=limage;
	unsigned char* resultbuffer=resultlimage.GetImgBuffer();
	unsigned int kofupleft=0;//掩模左上角索引
	unsigned int kofcenter=0;//掩模中心索引
	unsigned int k=0;//掩模遍历索引

	for (int i=0;i<width-4;i++)
	{
		for (int j=0;j<height-4;j++)
		{
			kofupleft=j*width+i;
			kofcenter=(j+2)*width+i+2;				
			if(buffer[kofcenter]!=0)
			{
				for (int x=0;x<5;x++)
					for (int y=0;y<5;y++)
					{
						k=kofupleft+y*width+x;
						if (buffer[k]==0)
						{
							neighbour[x][y]=0;
						}
						else 
							neighbour[x][y]=1;
					}
					if(NZ(neighbour)>=2&&NZ(neighbour)<=6) 
						Condition1=true;
					else 
						Condition1=false;
					if(Z0(neighbour,1)==1) 
						Condition2=true;
					else 
						Condition2=false;
					if(Z0(neighbour,2)!=1||Index(neighbour,2)*Index(neighbour,4)*Index(neighbour,8)==0)
						Condition3=true;
					else 
						Condition3=false;
					if(Z0(neighbour,4)!=1||Index(neighbour,2)*Index(neighbour,4)*Index(neighbour,6)==0)
						Condition4=true;
					else 
						Condition4=false;
					if (Condition1&&Condition2&&Condition3&&Condition4)
						resultbuffer[kofcenter]=0;

			}


		}
	}
		return 0;

}

int Morphy::Thining_12( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_12)
		return -2;

	unsigned int neighbour[5][5];
	bool Condition1=false;
	bool Condition2=false;
	bool Condition3=false;
	bool Condition4=false;
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	short* buffer=(short*)limage.GetImgBuffer();
	resultlimage=limage;
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int kofupleft=0;//掩模左上角索引
	unsigned int kofcenter=0;//掩模中心索引
	unsigned int k=0;//掩模遍历索引

	for (int i=0;i<width-4;i++)
	{
		for (int j=0;j<height-4;j++)
		{
			kofupleft=j*width+i;
			kofcenter=(j+2)*width+i+2;				
			if(buffer[kofcenter]!=0)
			{
				for (int x=0;x<5;x++)
					for (int y=0;y<5;y++)
					{
						k=kofupleft+y*width+x;
						if (buffer[k]==0)
						{
							neighbour[x][y]=0;
						}
						else 
							neighbour[x][y]=1;
					}
					if(NZ(neighbour)>=2&&NZ(neighbour)<=6) 
						Condition1=true;
					else 
						Condition1=false;
					if(Z0(neighbour,1)==1) 
						Condition2=true;
					else 
						Condition2=false;
					if(Z0(neighbour,2)!=1||Index(neighbour,2)*Index(neighbour,4)*Index(neighbour,8)==0)
						Condition3=true;
					else 
						Condition3=false;
					if(Z0(neighbour,4)!=1||Index(neighbour,2)*Index(neighbour,4)*Index(neighbour,6)==0)
						Condition4=true;
					else 
						Condition4=false;
					if (Condition1&&Condition2&&Condition3&&Condition4)
						resultbuffer[kofcenter]=0;

			}


		}
	}
	return 0;

}


int Morphy::Thining_16( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_16)
		return -2;

	unsigned int neighbour[5][5];
	bool Condition1=false;
	bool Condition2=false;
	bool Condition3=false;
	bool Condition4=false;
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	short* buffer=(short*)limage.GetImgBuffer();
	resultlimage=limage;
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int kofupleft=0;//掩模左上角索引
	unsigned int kofcenter=0;//掩模中心索引
	unsigned int k=0;//掩模遍历索引

	for (int i=0;i<width-4;i++)
	{
		for (int j=0;j<height-4;j++)
		{
			kofupleft=j*width+i;
			kofcenter=(j+2)*width+i+2;				
			if(buffer[kofcenter]!=0)
			{
				for (int x=0;x<5;x++)
					for (int y=0;y<5;y++)
					{
						k=kofupleft+y*width+x;
						if (buffer[k]==0)
						{
							neighbour[x][y]=0;
						}
						else 
							neighbour[x][y]=1;
					}
					if(NZ(neighbour)>=2&&NZ(neighbour)<=6) 
						Condition1=true;
					else 
						Condition1=false;
					if(Z0(neighbour,1)==1) 
						Condition2=true;
					else 
						Condition2=false;
					if(Z0(neighbour,2)!=1||Index(neighbour,2)*Index(neighbour,4)*Index(neighbour,8)==0)
						Condition3=true;
					else 
						Condition3=false;
					if(Z0(neighbour,4)!=1||Index(neighbour,2)*Index(neighbour,4)*Index(neighbour,6)==0)
						Condition4=true;
					else 
						Condition4=false;
					if (Condition1&&Condition2&&Condition3&&Condition4)
						resultbuffer[kofcenter]=0;

			}


		}
	}
		return 0;

}


int Morphy::ContourDraw_8( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_8)
		return -2;

	resultlimage=limage;
	int erode=Erode_8(limage,resultlimage);
	if (erode==0)
		return Complement_8(limage,resultlimage,resultlimage);	
	else 
		return erode;
}
int Morphy::ContourDraw_12( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_12)
		return -2;

	resultlimage=limage;
	int erode=Erode_12(limage,resultlimage);
	if (erode==0)
		return Complement_12(limage,resultlimage,resultlimage);	
	else 
		return erode;
}
int Morphy::ContourDraw_16( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_16)
		return -2;

	resultlimage=limage;
	int erode=Erode_16(limage,resultlimage);
	if (erode==0)
		return Complement_16(limage,resultlimage,resultlimage);	
	else 
		return erode;
}

int Morphy::ContourTrace_8( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_8)
		return -2;

	point2di p;
	point2di lastP;
	point2di nextP;
	nextP.x=limage.GetImgWidth();
	nextP.y=limage.GetImgHeight();
	int direction[8][2]=
	{ {-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0} };
	int begin=0;
	resultlimage=limage;
	resultlimage.SetZero();//像素值全部为0	unsigned char* buffer;
	unsigned char* buffer=limage.GetImgBuffer();
	unsigned char* resultbuffer=resultlimage.GetImgBuffer();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int k=0;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;

			if(buffer[k]!=0)
			{

				p.x=i;
				p.y=j;
				j=height;
				i=width;
				resultbuffer[k]=255;
			}
		}
	}
	
		lastP=p;
		unsigned char temp;
		while(nextP!=p)
		{

			begin=begin%8;
			do 
			{
				nextP.x=lastP.x+direction[begin][0];
				nextP.y=lastP.y+direction[begin][1];
				k=nextP.y*width+nextP.x;
				begin=(begin+1)%8;
				temp=buffer[k];


			} while (temp==0);

			resultbuffer[k]=255;
			begin=begin+4;
			lastP=nextP;
		}
	
	return 0;
}
int Morphy::ContourTrace_12( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_12)
		return -2;

	point2di p;
	point2di lastP;
	point2di nextP;
	nextP.x=limage.GetImgWidth();
	nextP.y=limage.GetImgHeight();
	int direction[8][2]=
	{ {-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0} };
	int begin=0;
	resultlimage=limage;
	resultlimage.SetZero();//像素值全部为0	unsigned char* buffer;
	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int k=0;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;

			if(buffer[k]!=0)
			{

				p.x=i;
				p.y=j;
				j=height;
				i=width;
				resultbuffer[k]=4095;
			}
		}
		lastP=p;
		short temp;
		while(nextP!=p)
		{

			begin=begin%8;
			do 
			{
				nextP.x=lastP.x+direction[begin][0];
				nextP.y=lastP.y+direction[begin][1];
				k=nextP.y*width+nextP.x;
				begin=(begin+1)%8;
				temp=buffer[k];


			} while (temp==0);

			resultbuffer[k]=4095;
			begin=begin+4;
			lastP=nextP;
		}
	}
		return 0;
}
int Morphy::ContourTrace_16( ListImage limage,ListImage& resultlimage)
{
	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_16)
		return -2;

	point2di p;
	point2di lastP;
	point2di nextP;
	nextP.x=limage.GetImgWidth();
	nextP.y=limage.GetImgHeight();
	int direction[8][2]=
	{ {-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0} };
	int begin=0;
	resultlimage=limage;
	resultlimage.SetZero();//像素值全部为0	unsigned char* buffer;
	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();
	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();
	unsigned int k=0;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;

			if(buffer[k]!=0)
			{

				p.x=i;
				p.y=j;
				j=height;
				i=width;
				resultbuffer[k]=65535;
			}
		}
		lastP=p;
		short temp;
		while(nextP!=p)
		{

			begin=begin%8;
			do 
			{
				nextP.x=lastP.x+direction[begin][0];
				nextP.y=lastP.y+direction[begin][1];
				k=nextP.y*width+nextP.x;
				begin=(begin+1)%8;
				temp=buffer[k];


			} while (temp==0);

			resultbuffer[k]=65535;
			begin=begin+4;
			lastP=nextP;
		}
	}
		return 0;
}
int Morphy::Fill_8( ListImage limage,ListImage& resultlimage,point2di seed,int allow) //以种子为基础值填充全域
{
	int Value=getValue_8(limage,seed.x,seed.y);
	if (Value<0)
		return Value;
	else
	{
		resultlimage=limage;
		resultlimage.SetZero();
		unsigned char* buffer=(unsigned char*)(limage.GetImgBuffer());
		unsigned char* resultbuffer=(unsigned char*)(resultlimage.GetImgBuffer());
		unsigned int width=limage.GetImgWidth();
		unsigned int height=limage.GetImgHeight();
		int diffrence=0; //与基础值的差值
		if(allow<0)
			allow=-allow;
		unsigned int k=0;
		for(int j=0;j<height;j++)
		{
			for (int i=0;i<width;i++)
			{
				k=j*width+i;
				diffrence=buffer[k]-Value;
				if (diffrence<=allow&&diffrence>=-allow)
				{

					resultbuffer[k]=255;
				}

			}
		}
			return 0;
	}


}
int Morphy::Fill_12( ListImage limage,ListImage& resultlimage,point2di seed,int allow) //以种子为基础值填充全域
{
	int Value=getValue_12(limage,seed.x,seed.y);
	if (Value<0)
		return Value;
	else
	{
		resultlimage=limage;
		resultlimage.SetZero();
		short* buffer=(short*)(limage.GetImgBuffer());
		short* resultbuffer=(short*)(resultlimage.GetImgBuffer());
		unsigned int width=limage.GetImgWidth();
		unsigned int height=limage.GetImgHeight();
		int diffrence=0; //与基础值的差值
		if(allow<0)
			allow=-allow;
		unsigned int k=0;
		for(int j=0;j<height;j++)
		{
			for (int i=0;i<width;i++)
			{
				k=j*width+i;
				diffrence=buffer[k]-Value;
				if (diffrence<=allow&&diffrence>=-allow)
				{

					resultbuffer[k]=4095;
				}

			}
		}
			return 0;
	}


}
int Morphy::Fill_16( ListImage limage,ListImage& resultlimage,point2di seed,int allow) //以种子为基础值填充全域
{
	int Value=getValue_16(limage,seed.x,seed.y);
	if (Value<0)
		return Value;
	else
	{
		resultlimage=limage;
		resultlimage.SetZero();
		short* buffer=(short*)(limage.GetImgBuffer());
		short* resultbuffer=(short*)(resultlimage.GetImgBuffer());
		unsigned int width=limage.GetImgWidth();
		unsigned int height=limage.GetImgHeight();
		int diffrence=0; //与基础值的差值
		if(allow<0)
			allow=-allow;
		unsigned int k=0;
		for(int j=0;j<height;j++)
		{
			for (int i=0;i<width;i++)
			{
				k=j*width+i;
				diffrence=buffer[k]-Value;
				if (diffrence<=allow&&diffrence>=-allow)
				{

					resultbuffer[k]=65535;
				}

			}
		}
			return 0;
	}


}
int Morphy::Fillneighbour1_8( ListImage limage,ListImage& resultlimage,point2di seed,int allow) //以种子为基础值填充邻域
{
	int Value=getValue_8(limage,seed.x,seed.y);
	if (Value<0)
		return Value;
	else
	{
		resultlimage=limage;
		resultlimage.SetZero();
		unsigned char* buffer=(unsigned char*)(limage.GetImgBuffer());
		unsigned char* resultbuffer=(unsigned char*)(resultlimage.GetImgBuffer());
		unsigned int width=limage.GetImgWidth();
		unsigned int height=limage.GetImgHeight();
		int diffrence=0; //与基础值的差值
		if(allow<0)
			allow=-allow;
		unsigned int k=0;
		point2di* Seeds=new point2di[width*height]; //填充栈
		unsigned int StackLeft=1;
		Seeds[StackLeft].x=seed.x;
		Seeds[StackLeft].y=seed.y;
		point2di StackTop;
		while (StackLeft!=0)
		{
			StackTop=Seeds[StackLeft];
			k=StackTop.y*width+StackTop.x;
			resultbuffer[k]=255;
			StackLeft--;

			//判断左边的点
			if (StackTop.x>0)
			{
				diffrence=buffer[k-1]-Value;
				if (resultbuffer[k-1]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x-1;
					Seeds[StackLeft].y=StackTop.y;
				}
			}


			//判断上边的点
			if (StackTop.y>0)
			{
				diffrence=buffer[k-width]-Value;
				if (resultbuffer[k-width]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x;
					Seeds[StackLeft].y=StackTop.y-1;
				}
			}

			//判断右边的点
			if (StackTop.x<width-1)
			{
				diffrence=buffer[k+1]-Value;
				if (resultbuffer[k+1]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x+1;
					Seeds[StackLeft].y=StackTop.y;
				}
			}

			//判断下边的点
			if (StackTop.y<height-1)
			{
				diffrence=buffer[k+width]-Value;
				if (resultbuffer[k+width]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x;
					Seeds[StackLeft].y=StackTop.y+1;
				}
			}

		}

		return 0;
	}
}
int Morphy::Fillneighbour1_12( ListImage limage,ListImage& resultlimage,point2di seed,int allow) //以种子为基础值填充邻域
{
	int Value=getValue_12(limage,seed.x,seed.y);
	if (Value<0)
		return Value;
	else
	{
		resultlimage=limage;
		resultlimage.SetZero();
		short* buffer=(short*)(limage.GetImgBuffer());
		short* resultbuffer=(short*)(resultlimage.GetImgBuffer());
		unsigned int width=limage.GetImgWidth();
		unsigned int height=limage.GetImgHeight();
		int diffrence=0; //与基础值的差值
		if(allow<0)
			allow=-allow;
		unsigned int k=0;
		point2di* Seeds=new point2di[width*height]; //填充栈
		unsigned int StackLeft=1;
		Seeds[StackLeft].x=seed.x;
		Seeds[StackLeft].y=seed.y;
		point2di StackTop;
		while (StackLeft!=0)
		{
			StackTop=Seeds[StackLeft];
			k=StackTop.y*width+StackTop.x;
			resultbuffer[k]=4095;
			StackLeft--;

			//判断左边的点
			if (StackTop.x>0)
			{
				diffrence=buffer[k-1]-Value;
				if (resultbuffer[k-1]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x-1;
					Seeds[StackLeft].y=StackTop.y;
				}
			}


			//判断上边的点
			if (StackTop.y>0)
			{
				diffrence=buffer[k-width]-Value;
				if (resultbuffer[k-width]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x;
					Seeds[StackLeft].y=StackTop.y-1;
				}
			}

			//判断右边的点
			if (StackTop.x<width-1)
			{
				diffrence=buffer[k+1]-Value;
				if (resultbuffer[k+1]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x+1;
					Seeds[StackLeft].y=StackTop.y;
				}
			}

			//判断下边的点
			if (StackTop.y<height-1)
			{
				diffrence=buffer[k+width]-Value;
				if (resultbuffer[k+width]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x;
					Seeds[StackLeft].y=StackTop.y+1;
				}
			}

		}

		return 0;
	}
}
int Morphy::Fillneighbour1_16( ListImage limage,ListImage& resultlimage,point2di seed,int allow) //以种子为基础值填充邻域
{
	int Value=getValue_16(limage,seed.x,seed.y);
	if (Value<0)
		return Value;
	else
	{
		resultlimage=limage;
		resultlimage.SetZero();
		unsigned short* buffer=(unsigned short*)(limage.GetImgBuffer());
		unsigned short* resultbuffer=(unsigned short*)(resultlimage.GetImgBuffer());
		unsigned int width=limage.GetImgWidth();
		unsigned int height=limage.GetImgHeight();
		int diffrence=0; //与基础值的差值
		if(allow<0)
			allow=-allow;
		unsigned int k=0;
		point2di* Seeds=new point2di[width*height]; //填充栈
		unsigned int StackLeft=1;
		Seeds[StackLeft].x=seed.x;
		Seeds[StackLeft].y=seed.y;
		point2di StackTop;
		while (StackLeft!=0)
		{
			StackTop=Seeds[StackLeft];
			k=StackTop.y*width+StackTop.x;
			resultbuffer[k]=65535;
			StackLeft--;

			//判断左边的点
			if (StackTop.x>0)
			{
				diffrence=buffer[k-1]-Value;
				if (resultbuffer[k-1]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x-1;
					Seeds[StackLeft].y=StackTop.y;
				}
			}


			//判断上边的点
			if (StackTop.y>0)
			{
				diffrence=buffer[k-width]-Value;
				if (resultbuffer[k-width]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x;
					Seeds[StackLeft].y=StackTop.y-1;
				}
			}

			//判断右边的点
			if (StackTop.x<width-1)
			{
				diffrence=buffer[k+1]-Value;
				if (resultbuffer[k+1]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x+1;
					Seeds[StackLeft].y=StackTop.y;
				}
			}

			//判断下边的点
			if (StackTop.y<height-1)
			{
				diffrence=buffer[k+width]-Value;
				if (resultbuffer[k+width]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x;
					Seeds[StackLeft].y=StackTop.y+1;
				}
			}

		}

		return 0;
	}
}
int Morphy::Fillneighbour2_8( ListImage limage,ListImage& resultlimage,point2di seed,int allow) //以每一填充点为基础值填充邻域
{
	int Value=getValue_8(limage,seed.x,seed.y);
	if (Value<0)
		return Value;
	else
	{
		int center_value=Value;
		resultlimage=limage;
		resultlimage.SetZero();
		unsigned char* buffer=(unsigned char*)(limage.GetImgBuffer());
		unsigned char* resultbuffer=(unsigned char*)(resultlimage.GetImgBuffer());
		unsigned int width=limage.GetImgWidth();
		unsigned int height=limage.GetImgHeight();
		int diffrence=0; //与基础值的差值
		if(allow<0)
			allow=-allow;
		unsigned int k=0;
		point2di* Seeds=new point2di[width*height]; //填充栈
		unsigned int StackLeft=1;
		Seeds[StackLeft].x=seed.x;
		Seeds[StackLeft].y=seed.y;
		point2di StackTop;
		while (StackLeft!=0)
		{
			StackTop=Seeds[StackLeft];
			k=StackTop.y*width+StackTop.x;
			center_value=buffer[k];
			resultbuffer[k]=255;
			StackLeft--;

			//判断左边的点
			if (StackTop.x>0)
			{
				diffrence=buffer[k-1]-center_value;
				if (resultbuffer[k-1]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x-1;
					Seeds[StackLeft].y=StackTop.y;
				}
			}


			//判断上边的点
			if (StackTop.y>0)
			{
				diffrence=buffer[k-width]-center_value;
				if (resultbuffer[k-width]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x;
					Seeds[StackLeft].y=StackTop.y-1;
				}
			}

			//判断右边的点
			if (StackTop.x<width-1)
			{
				diffrence=buffer[k+1]-center_value;
				if (resultbuffer[k+1]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x+1;
					Seeds[StackLeft].y=StackTop.y;
				}
			}

			//判断下边的点
			if (StackTop.y<height-1)
			{
				diffrence=buffer[k+width]-center_value;
				if (resultbuffer[k+width]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x;
					Seeds[StackLeft].y=StackTop.y+1;
				}
			}

		}

		return 0;
	}
}
int Morphy::Fillneighbour2_12( ListImage limage,ListImage& resultlimage,point2di seed,int allow) //以每一填充点为基础值填充邻域
{
	int Value=getValue_12(limage,seed.x,seed.y);
	if (Value<0)
		return Value;
	else
	{
		int center_value=Value;
		resultlimage=limage;
		resultlimage.SetZero();
		short* buffer=(short*)(limage.GetImgBuffer());
		short* resultbuffer=(short*)(resultlimage.GetImgBuffer());
		unsigned int width=limage.GetImgWidth();
		unsigned int height=limage.GetImgHeight();
		int diffrence=0; //与基础值的差值
		if(allow<0)
			allow=-allow;
		unsigned int k=0;
		point2di* Seeds=new point2di[width*height]; //填充栈
		unsigned int StackLeft=1;
		Seeds[StackLeft].x=seed.x;
		Seeds[StackLeft].y=seed.y;
		point2di StackTop;
		while (StackLeft!=0)
		{
			StackTop=Seeds[StackLeft];
			k=StackTop.y*width+StackTop.x;
			center_value=buffer[k];
			resultbuffer[k]=4095;
			StackLeft--;

			//判断左边的点
			if (StackTop.x>0)
			{
				diffrence=buffer[k-1]-center_value;
				if (resultbuffer[k-1]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x-1;
					Seeds[StackLeft].y=StackTop.y;
				}
			}


			//判断上边的点
			if (StackTop.y>0)
			{
				diffrence=buffer[k-width]-center_value;
				if (resultbuffer[k-width]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x;
					Seeds[StackLeft].y=StackTop.y-1;
				}
			}

			//判断右边的点
			if (StackTop.x<width-1)
			{
				diffrence=buffer[k+1]-center_value;
				if (resultbuffer[k+1]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x+1;
					Seeds[StackLeft].y=StackTop.y;
				}
			}

			//判断下边的点
			if (StackTop.y<height-1)
			{
				diffrence=buffer[k+width]-center_value;
				if (resultbuffer[k+width]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x;
					Seeds[StackLeft].y=StackTop.y+1;
				}
			}

		}

		return 0;
	}
}
int Morphy::Fillneighbour2_16( ListImage limage,ListImage& resultlimage,point2di seed,int allow) //以每一填充点为基础值填充邻域
{
	int Value=getValue_16(limage,seed.x,seed.y);
	if (Value<0)
		return Value;
	else
	{
		int center_value=Value;
		resultlimage=limage;
		resultlimage.SetZero();
		short* buffer=(short*)(limage.GetImgBuffer());
		short* resultbuffer=(short*)(resultlimage.GetImgBuffer());
		unsigned int width=limage.GetImgWidth();
		unsigned int height=limage.GetImgHeight();
		int diffrence=0; //与基础值的差值
		if(allow<0)
			allow=-allow;
		unsigned int k=0;
		point2di* Seeds=new point2di[width*height]; //填充栈
		unsigned int StackLeft=1;
		Seeds[StackLeft].x=seed.x;
		Seeds[StackLeft].y=seed.y;
		point2di StackTop;
		while (StackLeft!=0)
		{
			StackTop=Seeds[StackLeft];
			k=StackTop.y*width+StackTop.x;
			center_value=buffer[k];
			resultbuffer[k]=65535;
			StackLeft--;

			//判断左边的点
			if (StackTop.x>0)
			{
				diffrence=buffer[k-1]-center_value;
				if (resultbuffer[k-1]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x-1;
					Seeds[StackLeft].y=StackTop.y;
				}
			}


			//判断上边的点
			if (StackTop.y>0)
			{
				diffrence=buffer[k-width]-center_value;
				if (resultbuffer[k-width]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x;
					Seeds[StackLeft].y=StackTop.y-1;
				}
			}

			//判断右边的点
			if (StackTop.x<width-1)
			{
				diffrence=buffer[k+1]-center_value;
				if (resultbuffer[k+1]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x+1;
					Seeds[StackLeft].y=StackTop.y;
				}
			}

			//判断下边的点
			if (StackTop.y<height-1)
			{
				diffrence=buffer[k+width]-center_value;
				if (resultbuffer[k+width]==0&&diffrence>=-allow&&diffrence<=allow)
				{
					StackLeft++;
					Seeds[StackLeft].x=StackTop.x;
					Seeds[StackLeft].y=StackTop.y+1;
				}
			}

		}

		return 0;
	}
}
int Morphy::MarkConnect4_8( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num)
{

	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_8)
		return -2;

	resultlimage=limage;
	resultlimage.SetZero();
	
	unsigned char* buffer=limage.GetImgBuffer();
	unsigned char* resultbuffer=resultlimage.GetImgBuffer();

	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();

	int connectcount=0;  //连通域个数
	unsigned int pixelnum=0;     //每个连通域的非零像素数
	vector<unsigned int> uintvec;  //统计每个连通域非零像素数的数组
	unsigned int Value=0;//标记变量
	point2di* Seeds=new point2di[width*height];
	unsigned int StackLeft=1;
	point2di StackTop;
	unsigned int k=0;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			if (buffer[k]!=0)
			{
				StackLeft=1;
				Seeds[StackLeft].x=i;
				Seeds[StackLeft].y=j;
				Value++;
				pixelnum=0;

				while (StackLeft!=0)
				{
					StackTop=Seeds[StackLeft];
					k=StackTop.y*width+StackTop.x;
					resultbuffer[k]=Value;
					pixelnum++;
					StackLeft--;

					//判断左边的点
					if (StackTop.x>0)
					{

						if (buffer[k-1]!=0&&resultbuffer[k-1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x-1;
							Seeds[StackLeft].y=StackTop.y;
							resultbuffer[k-1]=Value;
						}
					}


					//判断上边的点
					if (StackTop.y>0)
					{
						if (buffer[k-width]!=0&&resultbuffer[k-width]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x;
							Seeds[StackLeft].y=StackTop.y-1;
							resultbuffer[k-width]=Value;
						}
					}

					//判断右边的点
					if (StackTop.x<width-1)
					{

						if (buffer[k+1]!=0&&resultbuffer[k+1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x+1;
							Seeds[StackLeft].y=StackTop.y;
							resultbuffer[k+1]=Value;
						}
					}

					//判断下边的点
					if (StackTop.y<height-1)
					{
						if (buffer[k+width]!=0&&resultbuffer[k+width]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x;
							Seeds[StackLeft].y=StackTop.y+1;
							resultbuffer[k+width]=Value;
						}
					}



				}
				Complement_8(limage,resultlimage,limage);

				if (pixelnum>num)
				{
					uintvec.push_back(pixelnum);
				}
			}

		}
	}
		connectcount=uintvec.size();
		statistic=new unsigned int[connectcount];
		for (int k=0;k<connectcount;k++)
		{
			statistic[k]=uintvec[k];
		}

		return connectcount;
}
int Morphy::MarkConnect4_12( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num)
{

	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_12)
		return -2;

	resultlimage=limage;
	resultlimage.SetZero();

	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();

	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();

	
	int connectcount=0;  //连通域个数
	unsigned int pixelnum=0;     //每个连通域的非零像素数
	vector<unsigned int> uintvec;  //统计每个连通域非零像素数的数组
	unsigned int Value=0;//标记变量
	point2di* Seeds=new point2di[width*height];
	unsigned int StackLeft=1;
	point2di StackTop;
	unsigned int k=0;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			if (buffer[k]!=0)
			{
				StackLeft=1;
				Seeds[StackLeft].x=i;
				Seeds[StackLeft].y=j;
				Value++;
				pixelnum=0;

				while (StackLeft!=0)
				{
					StackTop=Seeds[StackLeft];
					k=StackTop.y*width+StackTop.x;
					resultbuffer[k]=Value;
					pixelnum++;
					StackLeft--;

					//判断左边的点
					if (StackTop.x>0)
					{

						if (buffer[k-1]!=0&&resultbuffer[k-1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x-1;
							Seeds[StackLeft].y=StackTop.y;
							resultbuffer[k-1]=Value;
						}
					}


					//判断上边的点
					if (StackTop.y>0)
					{
						if (buffer[k-width]!=0&&resultbuffer[k-width]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x;
							Seeds[StackLeft].y=StackTop.y-1;
							resultbuffer[k-width]=Value;
						}
					}

					//判断右边的点
					if (StackTop.x<width-1)
					{

						if (buffer[k+1]!=0&&resultbuffer[k+1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x+1;
							Seeds[StackLeft].y=StackTop.y;
							resultbuffer[k+1]=Value;
						}
					}

					//判断下边的点
					if (StackTop.y<height-1)
					{
						if (buffer[k+width]!=0&&resultbuffer[k+width]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x;
							Seeds[StackLeft].y=StackTop.y+1;
							resultbuffer[k+width]=Value;
						}
					}



				}
				Complement_12(limage,resultlimage,limage);

				if (pixelnum>num)
				{
					uintvec.push_back(pixelnum);
				}
			}

		}
	}
		connectcount=uintvec.size();
		statistic=new unsigned int[connectcount];
		for (int k=0;k<connectcount;k++)
		{
			statistic[k]=uintvec[k];
		}

		return connectcount;
}
int Morphy::MarkConnect4_16( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num)
{

	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_16)
		return -2;

	resultlimage=limage;
	resultlimage.SetZero();

	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();

	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();

	int connectcount=0;  //连通域个数
	unsigned int pixelnum=0;     //每个连通域的非零像素数
	vector<unsigned int> uintvec;  //统计每个连通域非零像素数的数组
	unsigned int Value=0;//标记变量
	point2di* Seeds=new point2di[width*height];
	unsigned int StackLeft=1;
	point2di StackTop;
	unsigned int k=0;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			if (buffer[k]!=0)
			{
				StackLeft=1;
				Seeds[StackLeft].x=i;
				Seeds[StackLeft].y=j;
				Value++;
				pixelnum=0;

				while (StackLeft!=0)
				{
					StackTop=Seeds[StackLeft];
					k=StackTop.y*width+StackTop.x;
					resultbuffer[k]=Value;
					pixelnum++;
					StackLeft--;

					//判断左边的点
					if (StackTop.x>0)
					{

						if (buffer[k-1]!=0&&resultbuffer[k-1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x-1;
							Seeds[StackLeft].y=StackTop.y;
							resultbuffer[k-1]=Value;
						}
					}


					//判断上边的点
					if (StackTop.y>0)
					{
						if (buffer[k-width]!=0&&resultbuffer[k-width]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x;
							Seeds[StackLeft].y=StackTop.y-1;
							resultbuffer[k-width]=Value;
						}
					}

					//判断右边的点
					if (StackTop.x<width-1)
					{

						if (buffer[k+1]!=0&&resultbuffer[k+1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x+1;
							Seeds[StackLeft].y=StackTop.y;
							resultbuffer[k+1]=Value;
						}
					}

					//判断下边的点
					if (StackTop.y<height-1)
					{
						if (buffer[k+width]!=0&&resultbuffer[k+width]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x;
							Seeds[StackLeft].y=StackTop.y+1;
							resultbuffer[k+width]=Value;
						}
					}



				}
				Complement_16(limage,resultlimage,limage);

				if (pixelnum>num)
				{
					uintvec.push_back(pixelnum);
				}
			}

		}
		connectcount=uintvec.size();
		statistic=new unsigned int[connectcount];
		for (int k=0;k<connectcount;k++)
		{
			statistic[k]=uintvec[k];
		}
	}

		return connectcount;
}
int Morphy::MarkConnect8_8( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num)
{

	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_8)
		return -2;

	resultlimage=limage;
	resultlimage.SetZero();
	unsigned char* buffer=limage.GetImgBuffer();
	unsigned char* resultbuffer=resultlimage.GetImgBuffer();

	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();

	
	int connectcount=0;  //连通域个数
	unsigned int pixelnum=0;     //每个连通域的非零像素数
	vector<unsigned int> uintvec;  //统计每个连通域非零像素数的数组
	unsigned int Value=0;//标记变量
	point2di* Seeds=new point2di[width*height];
	unsigned int StackLeft=1;
	point2di StackTop;
	unsigned int k=0;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			if (buffer[k]!=0)
			{
				StackLeft=1;
				Seeds[StackLeft].x=i;
				Seeds[StackLeft].y=j;
				Value++;
				pixelnum=0;

				while (StackLeft!=0)
				{
					StackTop=Seeds[StackLeft];
					k=StackTop.y*width+StackTop.x;
					resultbuffer[k]=Value;
					pixelnum++;
					StackLeft--;

					//判断左边的点
					if (StackTop.x>0)
					{

						if (buffer[k-1]!=0&&resultbuffer[k-1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x-1;
							Seeds[StackLeft].y=StackTop.y;
							resultbuffer[k-1]=Value;
						}
					}

					//判断左上边的点
					if (StackTop.x>0&&StackTop.y>0)
					{

						if (buffer[k-width-1]!=0&&resultbuffer[k-width-1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x-1;
							Seeds[StackLeft].y=StackTop.y-1;
							resultbuffer[k-width-1]=Value;
						}
					}


					//判断上边的点
					if (StackTop.y>0)
					{
						if (buffer[k-width]!=0&&resultbuffer[k-width]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x;
							Seeds[StackLeft].y=StackTop.y-1;
							resultbuffer[k-width]=Value;
						}
					}
					//判断右上边的点
					if (StackTop.x<width-1&&StackTop.y>0)
					{

						if (buffer[k-width+1]!=0&&resultbuffer[k-width+1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x+1;
							Seeds[StackLeft].y=StackTop.y-1;
							resultbuffer[k-width+1]=Value;
						}
					}
					//判断右边的点
					if (StackTop.x<width-1)
					{

						if (buffer[k+1]!=0&&resultbuffer[k+1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x+1;
							Seeds[StackLeft].y=StackTop.y;
							resultbuffer[k+1]=Value;
						}
					}
					//判断右下边的点
					if (StackTop.x<width-1&&StackTop.y<height-1)
					{

						if (buffer[k+width+1]!=0&&resultbuffer[k+width+1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x+1;
							Seeds[StackLeft].y=StackTop.y+1;
							resultbuffer[k+width+1]=Value;
						}
					}

					//判断下边的点
					if (StackTop.y<height-1)
					{
						if (buffer[k+width]!=0&&resultbuffer[k+width]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x;
							Seeds[StackLeft].y=StackTop.y+1;
							resultbuffer[k+width]=Value;
						}
					}
					//判断左下边的点
					if (StackTop.x>0&&StackTop.y<height-1)
					{

						if (buffer[k+width-1]!=0&&resultbuffer[k+width-1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x-1;
							Seeds[StackLeft].y=StackTop.y+1;
							resultbuffer[k+width-1]=Value;
						}
					}



				}
				Complement_8(limage,resultlimage,limage);

				if (pixelnum>num)
				{
					uintvec.push_back(pixelnum);
				}
			}

		}
	}
		
		connectcount=uintvec.size();
		statistic=new unsigned int[connectcount];
		for (int k=0;k<connectcount;k++)
		{
			statistic[k]=uintvec[k];
		}

		return connectcount;
}
int Morphy::MarkConnect8_12( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num)
{

	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_12)
		return -2;

	resultlimage=limage;
	resultlimage.SetZero();
	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();

	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();

	int connectcount=0;  //连通域个数
	unsigned int pixelnum=0;     //每个连通域的非零像素数
	vector<unsigned int> uintvec;  //统计每个连通域非零像素数的数组
	unsigned int Value=0;//标记变量
	point2di* Seeds=new point2di[width*height];
	unsigned int StackLeft=1;
	point2di StackTop;
	unsigned int k=0;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			if (buffer[k]!=0)
			{
				StackLeft=1;
				Seeds[StackLeft].x=i;
				Seeds[StackLeft].y=j;
				Value++;
				pixelnum=0;

				while (StackLeft!=0)
				{
					StackTop=Seeds[StackLeft];
					k=StackTop.y*width+StackTop.x;
					resultbuffer[k]=Value;
					pixelnum++;
					StackLeft--;

					//判断左边的点
					if (StackTop.x>0)
					{

						if (buffer[k-1]!=0&&resultbuffer[k-1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x-1;
							Seeds[StackLeft].y=StackTop.y;
							resultbuffer[k-1]=Value;
						}
					}

					//判断左上边的点
					if (StackTop.x>0&&StackTop.y>0)
					{

						if (buffer[k-width-1]!=0&&resultbuffer[k-width-1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x-1;
							Seeds[StackLeft].y=StackTop.y-1;
							resultbuffer[k-width-1]=Value;
						}
					}


					//判断上边的点
					if (StackTop.y>0)
					{
						if (buffer[k-width]!=0&&resultbuffer[k-width]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x;
							Seeds[StackLeft].y=StackTop.y-1;
							resultbuffer[k-width]=Value;
						}
					}
					//判断右上边的点
					if (StackTop.x<width-1&&StackTop.y>0)
					{

						if (buffer[k-width+1]!=0&&resultbuffer[k-width+1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x+1;
							Seeds[StackLeft].y=StackTop.y-1;
							resultbuffer[k-width+1]=Value;
						}
					}
					//判断右边的点
					if (StackTop.x<width-1)
					{

						if (buffer[k+1]!=0&&resultbuffer[k+1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x+1;
							Seeds[StackLeft].y=StackTop.y;
							resultbuffer[k+1]=Value;
						}
					}
					//判断右下边的点
					if (StackTop.x<width-1&&StackTop.y<height-1)
					{

						if (buffer[k+width+1]!=0&&resultbuffer[k+width+1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x+1;
							Seeds[StackLeft].y=StackTop.y+1;
							resultbuffer[k+width+1]=Value;
						}
					}

					//判断下边的点
					if (StackTop.y<height-1)
					{
						if (buffer[k+width]!=0&&resultbuffer[k+width]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x;
							Seeds[StackLeft].y=StackTop.y+1;
							resultbuffer[k+width]=Value;
						}
					}
					//判断左下边的点
					if (StackTop.x>0&&StackTop.y<height-1)
					{

						if (buffer[k+width-1]!=0&&resultbuffer[k+width-1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x-1;
							Seeds[StackLeft].y=StackTop.y+1;
							resultbuffer[k+width-1]=Value;
						}
					}



				}
				Complement_12(limage,resultlimage,limage);

				if (pixelnum>num)
				{
					uintvec.push_back(pixelnum);
				}
			}
		}
	}
		connectcount=uintvec.size();
		statistic=new unsigned int[connectcount];
		for (int k=0;k<connectcount;k++)
		{
			statistic[k]=uintvec[k];
		}

		return connectcount;
}
int Morphy::MarkConnect8_16( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num)
{

	if( limage.GetImgChannel()!=1 )
		return -1;
	if(limage.GetImgDataType()!=uint_16)
		return -2;

	resultlimage=limage;
	resultlimage.SetZero();
	short* buffer=(short*)limage.GetImgBuffer();
	short* resultbuffer=(short*)resultlimage.GetImgBuffer();

	unsigned int width=limage.GetImgWidth();
	unsigned int height=limage.GetImgHeight();

	int connectcount=0;  //连通域个数
	unsigned int pixelnum=0;     //每个连通域的非零像素数
	vector<unsigned int> uintvec;  //统计每个连通域非零像素数的数组
	unsigned int Value=0;//标记变量
	point2di* Seeds=new point2di[width*height];
	unsigned int StackLeft=1;
	point2di StackTop;
	unsigned int k=0;
	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			k=j*width+i;
			if (buffer[k]!=0)
			{
				StackLeft=1;
				Seeds[StackLeft].x=i;
				Seeds[StackLeft].y=j;
				Value++;
				pixelnum=0;

				while (StackLeft!=0)
				{
					StackTop=Seeds[StackLeft];
					k=StackTop.y*width+StackTop.x;
					resultbuffer[k]=Value;
					pixelnum++;
					StackLeft--;

					//判断左边的点
					if (StackTop.x>0)
					{

						if (buffer[k-1]!=0&&resultbuffer[k-1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x-1;
							Seeds[StackLeft].y=StackTop.y;
							resultbuffer[k-1]=Value;
						}
					}

					//判断左上边的点
					if (StackTop.x>0&&StackTop.y>0)
					{

						if (buffer[k-width-1]!=0&&resultbuffer[k-width-1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x-1;
							Seeds[StackLeft].y=StackTop.y-1;
							resultbuffer[k-width-1]=Value;
						}
					}


					//判断上边的点
					if (StackTop.y>0)
					{
						if (buffer[k-width]!=0&&resultbuffer[k-width]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x;
							Seeds[StackLeft].y=StackTop.y-1;
							resultbuffer[k-width]=Value;
						}
					}
					//判断右上边的点
					if (StackTop.x<width-1&&StackTop.y>0)
					{

						if (buffer[k-width+1]!=0&&resultbuffer[k-width+1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x+1;
							Seeds[StackLeft].y=StackTop.y-1;
							resultbuffer[k-width+1]=Value;
						}
					}
					//判断右边的点
					if (StackTop.x<width-1)
					{

						if (buffer[k+1]!=0&&resultbuffer[k+1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x+1;
							Seeds[StackLeft].y=StackTop.y;
							resultbuffer[k+1]=Value;
						}
					}
					//判断右下边的点
					if (StackTop.x<width-1&&StackTop.y<height-1)
					{

						if (buffer[k+width+1]!=0&&resultbuffer[k+width+1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x+1;
							Seeds[StackLeft].y=StackTop.y+1;
							resultbuffer[k+width+1]=Value;
						}
					}

					//判断下边的点
					if (StackTop.y<height-1)
					{
						if (buffer[k+width]!=0&&resultbuffer[k+width]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x;
							Seeds[StackLeft].y=StackTop.y+1;
							resultbuffer[k+width]=Value;
						}
					}
					//判断左下边的点
					if (StackTop.x>0&&StackTop.y<height-1)
					{

						if (buffer[k+width-1]!=0&&resultbuffer[k+width-1]==0)
						{
							StackLeft++;
							Seeds[StackLeft].x=StackTop.x-1;
							Seeds[StackLeft].y=StackTop.y+1;
							resultbuffer[k+width-1]=Value;
						}
					}



				}
				Complement_16(limage,resultlimage,limage);

				if (pixelnum>num)
				{
					uintvec.push_back(pixelnum);
				}
			}

		}
	}
		connectcount=uintvec.size();
		statistic=new unsigned int[connectcount];
		for (int k=0;k<connectcount;k++)
		{
			statistic[k]=uintvec[k];
		}

		return connectcount;
}
int Morphy::Logical( ListImage limage,ListImage& resultlimage,unsigned int value)
{
	ImgDataType limgtype=limage.GetImgDataType();

		switch (limgtype)
		{
			case uint_8:
				return this->Logical_8(limage,resultlimage,value);
				break;
			case uint_12:
				return this->Logical_12(limage,resultlimage,value);
				break;
			case uint_16:
				return this->Logical_16(limage,resultlimage,value);
				break;
			default:
				return -5;
				break;
		}
				
	

}

int	Morphy::setValue( ListImage limage,ListImage& resultlimage,unsigned int x,unsigned int y,short value)
{
	ImgDataType limgtype=limage.GetImgDataType();
	
		switch (limgtype)
		{
		case uint_8:
			return this->setValue_8(limage,resultlimage,x,y,value);
			break;
		case uint_12:
			return this->setValue_12(limage,resultlimage,x,y,value);
			break;
		case uint_16:
			return this->setValue_16(limage,resultlimage,x,y,value);
			break;
		default:
			return -5;
			break;
		}

	

}

int	Morphy::getValue( ListImage limage,unsigned int x,unsigned int y)
{
	ImgDataType limgtype=limage.GetImgDataType();
	
	
		switch (limgtype)
		{
		case uint_8:
			return this->getValue_8(limage,x,y);
			break;
		case uint_12:
			return this->getValue_12(limage,x,y);
			break;
		case uint_16:
			return this->getValue_16(limage,x,y);
			break;
		default:
			return -5;
			break;
		}

	

}

int Morphy::Complement( ListImage limageA, ListImage limageB,ListImage& resultlimage)
{
	ImgDataType limgtype=limageA.GetImgDataType();
	
	
		switch (limgtype)
		{
		case uint_8:
			return this->Complement_8(limageA,limageB,resultlimage);
			break;
		case uint_12:
			return this->Complement_12(limageA,limageB,resultlimage);
			break;
		case uint_16:
			return this->Complement_16(limageA,limageB,resultlimage);
			break;
		default:
			return -5;
			break;
		}

	
}

int Morphy::Complement( ListImage limage,ListImage& resultlimage)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		
		return this->Complement_8(limage,resultlimage);
		break;
	case uint_12:
		return this->Complement_12(limage,resultlimage);
		break;
	case uint_16:
		return this->Complement_16(limage,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}
int Morphy::And( ListImage limageA, ListImage limageB,ListImage& resultlimage)
{
	ImgDataType limgtype=limageA.GetImgDataType();


	switch (limgtype)
	{
	case uint_8:

		return this->And_8(limageA,limageB,resultlimage);
		break;
	case uint_12:
		return this->And_12(limageA,limageB,resultlimage);
		break;
	case uint_16:
		return this->And_16(limageA,limageB,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}


int Morphy::Or( ListImage limageA, ListImage limageB,ListImage& resultlimage,unsigned int value)
{
	ImgDataType limgtype=limageA.GetImgDataType();


	switch (limgtype)
	{
	case uint_8:

		return this->Or_8(limageA,limageB,resultlimage);
		break;
	case uint_12:
		return this->Or_12(limageA,limageB,resultlimage);
		break;
	case uint_16:
		return this->Or_16(limageA,limageB,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}


int Morphy::Equal( ListImage limageA, ListImage limageB)
{
	ImgDataType limgtype=limageA.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:

		return this->Erode_8(limageA,limageB);
		break;
	case uint_12:
		return this->Erode_12(limageA,limageB);
		break;
	case uint_16:
		return this->Erode_16(limageA,limageB);
		break;
	default:
		return -5;
		break;
	}
}

int Morphy::Count( ListImage limage,unsigned int value)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Count_8(limage,value);
		break;
	case uint_12:
		return this->Count_12(limage,value);
		break;
	case uint_16:
		return this->Count_16(limage,value);
		break;
	default:
		return -5;
		break;
	}
}

int Morphy::Erode( ListImage limage,ListImage& resultlimage)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Erode_8(limage,resultlimage);
		break;
	case uint_12:
		return this->Erode_12(limage,resultlimage);
		break;
	case uint_16:
		return this->Erode_16(limage,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}

int Morphy::Dialate( ListImage limage,ListImage& resultlimage)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Dialate_8(limage,resultlimage);
		break;
	case uint_12:
		return this->Dialate_12(limage,resultlimage);
		break;
	case uint_16:
		return this->Dialate_16(limage,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}

int Morphy::Erodegray( ListImage limage,ListImage& resultlimage)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Erodegray_8(limage,resultlimage);
		break;
	case uint_12:
		return this->Erodegray_12(limage,resultlimage);
		break;
	case uint_16:
		return this->Erodegray_16(limage,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}
int Morphy::Dialategray( ListImage limage,ListImage& resultlimage)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Dialategray_8(limage,resultlimage);
		break;
	case uint_12:
		return this->Dialategray_12(limage,resultlimage);
		break;
	case uint_16:
		return this->Dialategray_16(limage,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}

int Morphy::Open( ListImage limage,ListImage& resultlimage)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Open_8(limage,resultlimage);
		break;
	case uint_12:
		return this->Open_12(limage,resultlimage);
		break;
	case uint_16:
		return this->Open_16(limage,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}
int Morphy::Close( ListImage limage,ListImage& resultlimage)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Close_8(limage,resultlimage);
		break;
	case uint_12:
		return this->Close_12(limage,resultlimage);
		break;
	case uint_16:
		return this->Close_16(limage,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}

int Morphy::Opengray( ListImage limage,ListImage& resultlimage)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Opengray_8(limage,resultlimage);
		break;
	case uint_12:
		return this->Opengray_12(limage,resultlimage);
		break;
	case uint_16:
		return this->Opengray_16(limage,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}

int Morphy::Closegray( ListImage limage,ListImage& resultlimage)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Closegray_8(limage,resultlimage);
		break;
	case uint_12:
		return this->Closegray_12(limage,resultlimage);
		break;
	case uint_16:
		return this->Closegray_16(limage,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}

int Morphy::Thining( ListImage limage,ListImage& resultlimage)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Thining_8(limage,resultlimage);
		break;
	case uint_12:
		return this->Thining_12(limage,resultlimage);
		break;
	case uint_16:
		return this->Thining_16(limage,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}


int Morphy::MarkConnect4( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->MarkConnect4_8(limage,resultlimage,statistic,num);
		break;
	case uint_12:
		return this->MarkConnect4_12(limage,resultlimage,statistic,num);
		break;
	case uint_16:
		return this->MarkConnect4_16(limage,resultlimage,statistic,num);
		break;
	default:
		return -5;
		break;
	}
}
int Morphy::MarkConnect8( ListImage limage,ListImage& resultlimage,unsigned int* statistic,unsigned int num)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->MarkConnect8_8(limage,resultlimage,statistic,num);
		break;
	case uint_12:
		return this->MarkConnect8_12(limage,resultlimage,statistic,num);
		break;
	case uint_16:
		return this->MarkConnect8_16(limage,resultlimage,statistic,num);
		break;
	default:
		return -5;
		break;
	}
}

int Morphy::ContourDraw( ListImage limage,ListImage& resultlimage)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->ContourDraw_8(limage,resultlimage);
		break;
	case uint_12:
		return this->ContourDraw_12(limage,resultlimage);
		break;
	case uint_16:
		return this->ContourDraw_16(limage,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}


int Morphy::ContourTrace( ListImage limage,ListImage& resultlimage)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->ContourTrace_8(limage,resultlimage);
		break;
	case uint_12:
		return this->ContourTrace_12(limage,resultlimage);
		break;
	case uint_16:
		return this->ContourTrace_16(limage,resultlimage);
		break;
	default:
		return -5;
		break;
	}
}

int Morphy::Fillneighbour1( ListImage limage,ListImage& resultlimage,point2di seed,int allow)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Fillneighbour1_8(limage,resultlimage,seed,allow);
		break;
	case uint_12:
		return this->Fillneighbour1_12(limage,resultlimage,seed,allow);
		break;
	case uint_16:
		return this->Fillneighbour1_16(limage,resultlimage,seed,allow);
		break;
	default:
		return -5;
		break;
	}
}

int Morphy::Fillneighbour2( ListImage limage,ListImage& resultlimage,point2di seed,int allow)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Fillneighbour2_8(limage,resultlimage,seed,allow);
		break;
	case uint_12:
		return this->Fillneighbour2_12(limage,resultlimage,seed,allow);
		break;
	case uint_16:
		return this->Fillneighbour2_16(limage,resultlimage,seed,allow);
		break;
	default:
		return -5;
		break;
	}
}

int Morphy::Fill( ListImage limage,ListImage& resultlimage,point2di seed,int allow)
{
	ImgDataType limgtype=limage.GetImgDataType();

	switch (limgtype)
	{
	case uint_8:
		return this->Fill_8(limage,resultlimage,seed,allow);
		break;
	case uint_12:
		return this->Fill_12(limage,resultlimage,seed,allow);
		break;
	case uint_16:
		return this->Fill_16(limage,resultlimage,seed,allow);
		break;
	default:
		return -5;
		break;
	}
}

