// GradSharpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "GradSharpDlg.h"


// GradSharpDlg �Ի���

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


// GradSharpDlg ��Ϣ�������

void GradSharpDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);

	if (GradSharpThre.IsEmpty())
	{
		MessageBox("������ֵ");
	}

	OnOK();
}
