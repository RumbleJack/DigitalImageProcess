#pragma once
#include "resource.h"

// ShowSIDlg 对话框

class ShowSIDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ShowSIDlg)

public:
	ShowSIDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ShowSIDlg();

// 对话框数据
	enum { IDD = IDD_ShowSIDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CImage *m_CImg;
	afx_msg void OnPaint();
};
