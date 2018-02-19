#pragma once


// GradSharpDlg 对话框

class GradSharpDlg : public CDialog
{
	DECLARE_DYNAMIC(GradSharpDlg)

public:
	GradSharpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GradSharpDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString GradSharpThre;
	afx_msg void OnBnClickedOk();
};
