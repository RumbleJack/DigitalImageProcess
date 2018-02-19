#pragma once


// UnsharpMaskingDlg 对话框

class UnsharpMaskingDlg : public CDialog
{
	DECLARE_DYNAMIC(UnsharpMaskingDlg)

public:
	UnsharpMaskingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~UnsharpMaskingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString frequencyCoefficient;
	afx_msg void OnBnClickedOk();
};
