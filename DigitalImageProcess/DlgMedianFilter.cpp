// DlgMedianFilter.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "DlgMedianFilter.h"


// CDlgMedianFilter 对话框

IMPLEMENT_DYNAMIC(CDlgMedianFilter, CDialog)

CDlgMedianFilter::CDlgMedianFilter(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMedianFilter::IDD, pParent)
{
	
	m_iTempH = 3;
	m_iTempW = 3;
	m_iTempMX = 1;
	m_iTempMY = 1;
}

CDlgMedianFilter::~CDlgMedianFilter()
{
}

void CDlgMedianFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, m_iTempH);
	DDV_MinMaxInt(pDX, m_iTempH, 1, 10);
	DDX_Text(pDX, IDC_EDIT2, m_iTempW);
	DDV_MinMaxInt(pDX, m_iTempW, 1, 10);
	DDX_Text(pDX, IDC_EDIT3, m_iTempMX);
	DDV_MinMaxInt(pDX, m_iTempMX, 0, 9);
	DDX_Text(pDX, IDC_EDIT4, m_iTempMY);
	DDV_MinMaxInt(pDX, m_iTempMY, 0, 9);
}


BEGIN_MESSAGE_MAP(CDlgMedianFilter, CDialog)
END_MESSAGE_MAP()


// CDlgMedianFilter 消息处理程序
