#pragma once
#include "afxwin.h"


// WindowDialog 对话框

class WindowDialog : public CDialog
{
	DECLARE_DYNAMIC(WindowDialog)

public:
	WindowDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WindowDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	// 窗口的上限值
	CString UpValue;
	// 窗口的下限值
	CString DownValue;

	bool flag;  //点击对话框的确定还是取消按钮

	afx_msg void OnBnClickedCancel();
};
