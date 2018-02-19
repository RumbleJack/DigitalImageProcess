// ShowSIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "ShowSIDlg.h"
#include "afxdialogex.h"


// ShowSIDlg 对话框

IMPLEMENT_DYNAMIC(ShowSIDlg, CDialogEx)

ShowSIDlg::ShowSIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ShowSIDlg::IDD, pParent)
{

}

ShowSIDlg::~ShowSIDlg()
{

}

void ShowSIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ShowSIDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// ShowSIDlg 消息处理程序


void ShowSIDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	CRect rect;
	rect.SetRect(0, 0, m_CImg->GetWidth(), m_CImg->GetHeight());
	//GetClientRect(&rect);
	if (m_CImg!=NULL)
		m_CImg->Draw(dc.GetSafeHdc(), rect);
}
