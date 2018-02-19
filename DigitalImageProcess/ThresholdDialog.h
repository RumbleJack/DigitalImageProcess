#pragma once


// ThresholdDialog 对话框

class ThresholdDialog : public CDialog
{
	DECLARE_DYNAMIC(ThresholdDialog)

public:
	ThresholdDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ThresholdDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 用户输入的阈值
	CString ThresholdValue;
	afx_msg void OnBnClickedOk();

	bool flag;  //点击对话框的确定还是取消按钮

	afx_msg void OnBnClickedCancel();
};
