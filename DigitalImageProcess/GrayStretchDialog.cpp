// GrayStretchDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "GrayStretchDialog.h"


// GrayStretchDialog 对话框

IMPLEMENT_DYNAMIC(GrayStretchDialog, CDialog)

GrayStretchDialog::GrayStretchDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GrayStretchDialog::IDD, pParent)
	, X1(_T(""))
	, X2(_T(""))
{
	flag = true;
	imageFlag = 0;
}

GrayStretchDialog::~GrayStretchDialog()
{
}

void GrayStretchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, X1);
	DDX_Text(pDX, IDC_EDIT2, X2);
	DDX_Control(pDX, IDC_SLIDER1, stretchGrayLow);
	DDX_Control(pDX, IDC_SLIDER2, stretchGrayHigh);

	stretchGrayLow.SetRange(0,65535);//设置滑动范围
	stretchGrayLow.SetTicFreq(1);//每1个单位画一刻度
	stretchGrayLow.SetPos(0);
	 stretchGrayHigh.SetRange(0,65535);//设置滑动范围
     stretchGrayHigh.SetTicFreq(1);//每1个单位画一刻度
	 stretchGrayHigh.SetPos(65535);
}


BEGIN_MESSAGE_MAP(GrayStretchDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GrayStretchDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &GrayStretchDialog::OnBnClickedCancel)
	ON_WM_HSCROLL()
	
END_MESSAGE_MAP()


// GrayStretchDialog 消息处理程序

void GrayStretchDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	flag = true;
    
	UpdateData(TRUE);
	int low = atoi(X1);
	int high = atoi(X2);
	CallBankFunc(CDigitalImageProcessDlg::callBackFun,low,high);
   
	//设定位置
	stretchGrayLow.SetPos(low);
    stretchGrayHigh.SetPos(high);
}

void GrayStretchDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	flag = false;


	OnCancel();
}

void GrayStretchDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int low = stretchGrayLow.GetPos( );
	int high =stretchGrayHigh.GetPos( );
	CallBankFunc(CDigitalImageProcessDlg::callBackFun,low,high);
	
	X1.Format("%d", low);
	X2.Format("%d", high);
	UpdateData(FALSE);

	//设定位置
	stretchGrayLow.SetPos(low);
    stretchGrayHigh.SetPos(high);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


