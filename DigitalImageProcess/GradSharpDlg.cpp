// GradSharpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "GradSharpDlg.h"


// GradSharpDlg 对话框

IMPLEMENT_DYNAMIC(GradSharpDlg, CDialog)

GradSharpDlg::GradSharpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GradSharpDlg::IDD, pParent)
	, GradSharpThre(_T(""))
{

}

GradSharpDlg::~GradSharpDlg()
{
}

void GradSharpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, GradSharpThre);
}


BEGIN_MESSAGE_MAP(GradSharpDlg, CDialog)
	ON_BN_CLICKED(IDOK, &GradSharpDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// GradSharpDlg 消息处理程序

void GradSharpDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	if (GradSharpThre.IsEmpty())
	{
		MessageBox("请输入值");
	}

	OnOK();
}
