// UnsharpMaskingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "UnsharpMaskingDlg.h"


// UnsharpMaskingDlg 对话框

IMPLEMENT_DYNAMIC(UnsharpMaskingDlg, CDialog)

UnsharpMaskingDlg::UnsharpMaskingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(UnsharpMaskingDlg::IDD, pParent)
	, frequencyCoefficient(_T(""))
{

}

UnsharpMaskingDlg::~UnsharpMaskingDlg()
{
}

void UnsharpMaskingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, frequencyCoefficient);
}


BEGIN_MESSAGE_MAP(UnsharpMaskingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &UnsharpMaskingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// UnsharpMaskingDlg 消息处理程序

void UnsharpMaskingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	if (frequencyCoefficient.IsEmpty())
	{
		MessageBox("请输入值");
	}
	
	OnOK();
}
