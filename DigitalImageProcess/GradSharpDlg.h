#pragma once


// GradSharpDlg �Ի���

class GradSharpDlg : public CDialog
{
	DECLARE_DYNAMIC(GradSharpDlg)

public:
	GradSharpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GradSharpDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString GradSharpThre;
	afx_msg void OnBnClickedOk();
};
