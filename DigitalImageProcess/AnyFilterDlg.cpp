// DlgAnyFilter.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "AnyFilterDlg.h"


// CAnyFilterDlg 对话框

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


// CAnyFilterDlg 消息处理程序

void CAnyFilterDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	// 3×3平均模板
	m_intType = 0;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC  = (FLOAT) (1.0 / 9.0);
	
	// 设置模板元素
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
	
	// 更新文本框状态
	UpdateEdit();
	
	// 更新
	UpdateData(FALSE);
}

void CAnyFilterDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	// 3×3高斯模板
	m_intType = 1;
	m_iTempH  = 3;
	m_iTempW  = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
	m_fTempC  = (FLOAT) (1.0 / 16.0);
	
	// 设置模板元素
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
	
	// 更新文本框状态
	UpdateEdit();
	
	// 更新
	UpdateData(FALSE);
}



void CAnyFilterDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	// 自定义模板
	m_intType = 2;
	
	m_iTempH  = 5;
	m_iTempW  = 5;
	m_iTempMX = 0;
	m_iTempMY = 0;
	m_fTempC  = (FLOAT) (1.0);
	
	// 设置模板元素
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
	// 更新文本框状态
	UpdateData(FALSE);
	UpdateData(TRUE);
	UpdateEdit();

}
void CAnyFilterDlg::UpdateEdit()
{
	
	BOOL	bEnable;
	
	// 循环变量
	int		i;
	int		j;
	
	// 判断是不是自定义模板
	if (m_intType == 2) 
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	// 设置文本框可用状态
	(CEdit *) GetDlgItem(IDC_EDIT1)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	(CEdit *) GetDlgItem(IDC_EDIT4)->EnableWindow(bEnable);
	(CEdit *) GetDlgItem(IDC_EDIT5)->EnableWindow(bEnable);
	
	// IDC_EDIT_V0等ID其实是一个整数，它的数值定义在Resource.h中定义。
	
	// 设置模板元素文本框Enable状态
	for (i = IDC_EDIT6; i <= IDC_EDIT6 + 24; i++)
	{
		// 设置文本框不可编辑
		(CEdit *) GetDlgItem(i)->EnableWindow(bEnable);
	}
	
	// 显示应该可见的模板元素文本框
	for (i = 0; i < m_iTempH; i++)
	{
		for (j = 0; j < m_iTempW; j++)
		{
			// 设置文本框可见
			(CEdit *) GetDlgItem(IDC_EDIT6 + i*5 + j)->ShowWindow(SW_SHOW);
		}
	}
}
void CAnyFilterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// 获取用户设置（更新）
	UpdateData(TRUE);
	
	
	// 判断设置是否有效
	if ((m_iTempMX < 0) || (m_iTempMX > m_iTempW - 1) ||
		(m_iTempMY < 0) || (m_iTempMY > m_iTempH - 1))
	{
		// 提示用户参数设置错误
		
		MessageBox(_T("中心元素参数设置错误！"), _T("系统提示") , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 更新
	UpdateData(FALSE);
	
	// 退出
	OnOK();
}