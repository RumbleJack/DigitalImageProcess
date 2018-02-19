// DlgAnyFilter.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "AnyFilterDlg.h"


// CAnyFilterDlg �Ի���

IMPLEMENT_DYNAMIC(CAnyFilterDlg, CDialog)

CAnyFilterDlg::CAnyFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnyFilterDlg::IDD, pParent)
{
	m_intType = -1;
	m_iTempH = 0;
	m_iTempW = 0;
	m_iTempMX = 0;
	m_iTempMY = 0;
	m_fTempC = 0.0f;
	m_fpArray = new float[25];
}

CAnyFilterDlg::~CAnyFilterDlg()
{
}

void CAnyFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, m_fTempC);
	DDX_Text(pDX, IDC_EDIT2, m_iTempH);
	DDV_MinMaxInt(pDX, m_iTempH, 2, 5);
	DDX_Text(pDX, IDC_EDIT3, m_iTempW);
	DDV_MinMaxInt(pDX, m_iTempW, 2, 5);
	DDX_Text(pDX, IDC_EDIT4, m_iTempMX);
	DDX_Text(pDX, IDC_EDIT5, m_iTempMY);

	DDX_Text(pDX, IDC_EDIT6, m_fpArray[0]);
	DDX_Text(pDX, IDC_EDIT7, m_fpArray[1]);
	DDX_Text(pDX, IDC_EDIT8, m_fpArray[2]);
	DDX_Text(pDX, IDC_EDIT9, m_fpArray[3]);
	DDX_Text(pDX, IDC_EDIT10, m_fpArray[4]);
	DDX_Text(pDX, IDC_EDIT11, m_fpArray[5]);
	DDX_Text(pDX, IDC_EDIT12, m_fpArray[6]);
	DDX_Text(pDX, IDC_EDIT13, m_fpArray[7]);
	DDX_Text(pDX, IDC_EDIT14, m_fpArray[8]);
	DDX_Text(pDX, IDC_EDIT15, m_fpArray[9]);
	DDX_Text(pDX, IDC_EDIT16, m_fpArray[10]);
	DDX_Text(pDX, IDC_EDIT17, m_fpArray[11]);
	DDX_Text(pDX, IDC_EDIT18, m_fpArray[12]);
	DDX_Text(pDX, IDC_EDIT19, m_fpArray[13]);
	DDX_Text(pDX, IDC_EDIT20, m_fpArray[14]);
	DDX_Text(pDX, IDC_EDIT21, m_fpArray[15]);
	DDX_Text(pDX, IDC_EDIT22, m_fpArray[16]);
	DDX_Text(pDX, IDC_EDIT23, m_fpArray[17]);
	DDX_Text(pDX, IDC_EDIT24, m_fpArray[18]);
	DDX_Text(pDX, IDC_EDIT25, m_fpArray[19]);
	DDX_Text(pDX, IDC_EDIT26, m_fpArray[20]);
	DDX_Text(pDX, IDC_EDIT27, m_fpArray[21]);
	DDX_Text(pDX, IDC_EDIT28, m_fpArray[22]);
	DDX_Text(pDX, IDC_EDIT29, m_fpArray[23]);
	DDX_Text(pDX, IDC_EDIT30, m_fpArray[24]);
	
}



BEGIN_MESSAGE_MAP(CAnyFilterDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO2, &CAnyFilterDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CAnyFilterDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CAnyFilterDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDOK, &CAnyFilterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAnyFilterDlg ��Ϣ�������

void CAnyFilterDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// 3��3ƽ��ģ��
	m_intType = 0;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC  = (FLOAT) (1.0 / 9.0);
	
	// ����ģ��Ԫ��
	m_fpArray[0] = 1.0;
	m_fpArray[1] = 1.0;
	m_fpArray[2] = 1.0;
	m_fpArray[3] = 1.0;
	m_fpArray[4] = 1.0;
	m_fpArray[5] = 1.0;
	m_fpArray[6] = 1.0;
	m_fpArray[7] = 1.0;
	m_fpArray[8] = 1.0;
	m_fpArray[9] = 0.0;
	m_fpArray[10] = 0.0;
	m_fpArray[11] = 0.0;
	m_fpArray[12] = 0.0;
	m_fpArray[13] = 0.0;
	m_fpArray[14] = 0.0;
	m_fpArray[15] = 0.0;
	m_fpArray[16] = 0.0;
	m_fpArray[17] = 0.0;
	m_fpArray[18] = 0.0;
	m_fpArray[19] = 0.0;
	m_fpArray[20] = 0.0;
	m_fpArray[21] = 0.0;
	m_fpArray[22] = 0.0;
	m_fpArray[23] = 0.0;
	m_fpArray[24] = 0.0;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}

void CAnyFilterDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// 3��3��˹ģ��
	m_intType = 1;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC  = (FLOAT) (1.0 / 16.0);
	
	// ����ģ��Ԫ��
	m_fpArray[0] = 1.0;
	m_fpArray[1] = 2.0;
	m_fpArray[2] = 1.0;
	m_fpArray[3] = 2.0;
	m_fpArray[4] = 4.0;
	m_fpArray[5] = 2.0;
	m_fpArray[6] = 1.0;
	m_fpArray[7] = 2.0;
	m_fpArray[8] = 1.0;
	m_fpArray[9] = 0.0;
	m_fpArray[10] = 0.0;
	m_fpArray[11] = 0.0;
	m_fpArray[12] = 0.0;
	m_fpArray[13] = 0.0;
	m_fpArray[14] = 0.0;
	m_fpArray[15] = 0.0;
	m_fpArray[16] = 0.0;
	m_fpArray[17] = 0.0;
	m_fpArray[18] = 0.0;
	m_fpArray[19] = 0.0;
	m_fpArray[20] = 0.0;
	m_fpArray[21] = 0.0;
	m_fpArray[22] = 0.0;
	m_fpArray[23] = 0.0;
	m_fpArray[24] = 0.0;
	
	// �����ı���״̬
	UpdateEdit();
	
	// ����
	UpdateData(FALSE);
}



void CAnyFilterDlg::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// �Զ���ģ��
	m_intType = 2;
	
	m_iTempH  = 5;
	m_iTempW  = 5;
	m_iTempMX = 0;
	m_iTempMY = 0;
	m_fTempC  = (FLOAT) (1.0);
	
	// ����ģ��Ԫ��
	m_fpArray[0] = 0.0;
	m_fpArray[1] = 0.0;
	m_fpArray[2] = 0.0;
	m_fpArray[3] = 0.0;
	m_fpArray[4] = 0.0;
	m_fpArray[5] = 0.0;
	m_fpArray[6] = 0.0;
	m_fpArray[7] = 0.0;
	m_fpArray[8] = 0.0;
	m_fpArray[9] = 0.0;
	m_fpArray[10] = 0.0;
	m_fpArray[11] = 0.0;
	m_fpArray[12] = 0.0;
	m_fpArray[13] = 0.0;
	m_fpArray[14] = 0.0;
	m_fpArray[15] = 0.0;
	m_fpArray[16] = 0.0;
	m_fpArray[17] = 0.0;
	m_fpArray[18] = 0.0;
	m_fpArray[19] = 0.0;
	m_fpArray[20] = 0.0;
	m_fpArray[21] = 0.0;
	m_fpArray[22] = 0.0;
	m_fpArray[23] = 0.0;
	m_fpArray[24] = 1.0;
	// �����ı���״̬
	UpdateData(FALSE);
	UpdateData(TRUE);
	UpdateEdit();

}
void CAnyFilterDlg::UpdateEdit()
{
	
	BOOL	bEnable;
	
	// ѭ������
	int		i;
	int		j;
	
	// �ж��ǲ����Զ���ģ��
	if (m_intType == 2) 
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	// �����ı������״̬
	(CEdit *) GetDlgItem(IDC_EDIT1)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT4)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT5)->EnableWindow(bEnable);
	
	// IDC_EDIT_V0��ID��ʵ��һ��������������ֵ������Resource.h�ж��塣
	
	// ����ģ��Ԫ���ı���Enable״̬
	for (i = IDC_EDIT6; i <= IDC_EDIT6 + 24; i++)
	{
		// �����ı��򲻿ɱ༭
		(CEdit *) GetDlgItem(i)->EnableWindow(bEnable);
	}
	
	// ��ʾӦ�ÿɼ���ģ��Ԫ���ı���
	for (i = 0; i < m_iTempH; i++)
	{
		for (j = 0; j < m_iTempW; j++)
		{
			// �����ı���ɼ�
			(CEdit *) GetDlgItem(IDC_EDIT6 + i*5 + j)->ShowWindow(SW_SHOW);
		}
	}
}
void CAnyFilterDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ��ȡ�û����ã����£�
	UpdateData(TRUE);
	
	
	// �ж������Ƿ���Ч
	if ((m_iTempMX < 0) || (m_iTempMX > m_iTempW - 1) ||
		(m_iTempMY < 0) || (m_iTempMY > m_iTempH - 1))
	{
		// ��ʾ�û��������ô���
		
		MessageBox(_T("����Ԫ�ز������ô���"), _T("ϵͳ��ʾ") , 
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// ����
	UpdateData(FALSE);
	
	// �˳�
	OnOK();
}