#pragma once
#include "resource.h"

// ShowSIDlg �Ի���

class ShowSIDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ShowSIDlg)

public:
	ShowSIDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ShowSIDlg();

// �Ի�������
	enum { IDD = IDD_ShowSIDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CImage *m_CImg;
	afx_msg void OnPaint();
};
