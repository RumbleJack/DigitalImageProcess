// ShowSIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "ShowSIDlg.h"
#include "afxdialogex.h"


// ShowSIDlg �Ի���

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


// ShowSIDlg ��Ϣ�������


void ShowSIDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	CRect rect;
	rect.SetRect(0, 0, m_CImg->GetWidth(), m_CImg->GetHeight());
	//GetClientRect(&rect);
	if (m_CImg!=NULL)
		m_CImg->Draw(dc.GetSafeHdc(), rect);
}
