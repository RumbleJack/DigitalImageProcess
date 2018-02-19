// ThresholdDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "ThresholdDialog.h"


// ThresholdDialog �Ի���

IMPLEMENT_DYNAMIC(ThresholdDialog, CDialog)

ThresholdDialog::ThresholdDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ThresholdDialog::IDD, pParent)
	, ThresholdValue(_T(""))
{
	flag = true;
}

ThresholdDialog::~ThresholdDialog()
{
}

void ThresholdDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, ThresholdValue);
}


BEGIN_MESSAGE_MAP(ThresholdDialog, CDialog)
	ON_BN_CLICKED(IDOK, &ThresholdDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ThresholdDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// ThresholdDialog ��Ϣ�������

void ThresholdDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	flag = true;

	OnOK();
}

void ThresholdDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	flag = false;

	OnCancel();
}
