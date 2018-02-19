// WindowDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "WindowDialog.h"


// WindowDialog 对话框

IMPLEMENT_DYNAMIC(WindowDialog, CDialog)

WindowDialog::WindowDialog(CWnd* pParent /*=NULL*/)
	: CDialog(WindowDialog::IDD, pParent)
	, UpValue(_T(""))
{
	flag = true;
}

WindowDialog::~WindowDialog()
{
}

void WindowDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, UpValue);
	DDX_Text(pDX, IDC_EDIT2, DownValue);
}


BEGIN_MESSAGE_MAP(WindowDialog, CDialog)
	ON_BN_CLICKED(IDOK, &WindowDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &WindowDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// WindowDialog 消息处理程序

void WindowDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	flag = true;

	OnOK();
}

void WindowDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	flag = false;

	OnCancel();
}
