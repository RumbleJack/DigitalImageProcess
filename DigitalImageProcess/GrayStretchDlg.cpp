// GrayStretchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "GrayStretchDlg.h"


// GrayStretchDlg �Ի���

IMPLEMENT_DYNAMIC(GrayStretchDlg, CDialog)

GrayStretchDlg::GrayStretchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GrayStretchDlg::IDD, pParent)
	, X1(_T(""))
	, X2(_T(""))
{
	flag = true;
	imageFlag = 0;
}

GrayStretchDlg::~GrayStretchDlg()
{
}

void GrayStretchDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(GrayStretchDlg, CDialog)
	ON_BN_CLICKED(IDOK, &GrayStretchDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &GrayStretchDlg::OnBnClickedCancel)
	ON_WM_HSCROLL()
	
END_MESSAGE_MAP()


// GrayStretchDlg ��Ϣ�������

void GrayStretchDlg::OnBnClickedOk()
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

void GrayStretchDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	flag = false;


	OnCancel();
}

void GrayStretchDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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


