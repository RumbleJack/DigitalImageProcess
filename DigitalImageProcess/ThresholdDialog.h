#pragma once


// ThresholdDialog �Ի���

class ThresholdDialog : public CDialog
{
	DECLARE_DYNAMIC(ThresholdDialog)

public:
	ThresholdDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ThresholdDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �û��������ֵ
	CString ThresholdValue;
	afx_msg void OnBnClickedOk();

	bool flag;  //����Ի����ȷ������ȡ����ť

	afx_msg void OnBnClickedCancel();
};
