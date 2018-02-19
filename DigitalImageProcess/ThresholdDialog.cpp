// ThresholdDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "ThresholdDialog.h"


// ThresholdDialog 对话框

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


// ThresholdDialog 消息处理程序

void ThresholdDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	flag = true;

	OnOK();
}

void ThresholdDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	flag = false;

	OnCancel();
}
