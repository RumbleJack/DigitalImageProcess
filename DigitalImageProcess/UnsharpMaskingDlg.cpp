// UnsharpMaskingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "UnsharpMaskingDlg.h"


// UnsharpMaskingDlg �Ի���

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


// UnsharpMaskingDlg ��Ϣ�������

void UnsharpMaskingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);

	if (frequencyCoefficient.IsEmpty())
	{
		MessageBox("������ֵ");
	}
	
	OnOK();
}
