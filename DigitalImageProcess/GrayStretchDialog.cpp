// GrayStretchDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "GrayStretchDialog.h"


// GrayStretchDialog �Ի���

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

	stretchGrayLow.SetRange(0,65535);//���û�����Χ
	stretchGrayLow.SetTicFreq(1);//ÿ1����λ��һ�̶�
	stretchGrayLow.SetPos(0);
	 stretchGrayHigh.SetRange(0,65535);//���û�����Χ
     stretchGrayHigh.SetTicFreq(1);//ÿ1����λ��һ�̶�
	 stretchGrayHigh.SetPos(65535);
}


BEGIN_MESSAGE_MAP(GrayStretchDialog, CDialog)
	ON_BN_CLICKED(IDOK, &GrayStretchDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &GrayStretchDialog::OnBnClickedCancel)
	ON_WM_HSCROLL()
	
END_MESSAGE_MAP()


// GrayStretchDialog ��Ϣ�������

void GrayStretchDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	flag = true;
    
	UpdateData(TRUE);
	int low = atoi(X1);
	int high = atoi(X2);
	CallBankFunc(CDigitalImageProcessDlg::callBackFun,low,high);
   
	//�趨λ��
	stretchGrayLow.SetPos(low);
    stretchGrayHigh.SetPos(high);
}

void GrayStretchDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	flag = false;


	OnCancel();
}

void GrayStretchDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int low = stretchGrayLow.GetPos( );
	int high =stretchGrayHigh.GetPos( );
	CallBankFunc(CDigitalImageProcessDlg::callBackFun,low,high);
	
	X1.Format("%d", low);
	X2.Format("%d", high);
	UpdateData(FALSE);

	//�趨λ��
	stretchGrayLow.SetPos(low);
    stretchGrayHigh.SetPos(high);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


