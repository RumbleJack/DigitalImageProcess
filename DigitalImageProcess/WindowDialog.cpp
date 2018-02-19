// WindowDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "WindowDialog.h"


// WindowDialog �Ի���

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


// WindowDialog ��Ϣ�������

void WindowDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	flag = true;

	OnOK();
}

void WindowDialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	flag = false;

	OnCancel();
}
