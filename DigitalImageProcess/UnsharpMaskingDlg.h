#pragma once


// UnsharpMaskingDlg �Ի���

class UnsharpMaskingDlg : public CDialog
{
	DECLARE_DYNAMIC(UnsharpMaskingDlg)

public:
	UnsharpMaskingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~UnsharpMaskingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString frequencyCoefficient;
	afx_msg void OnBnClickedOk();
};
