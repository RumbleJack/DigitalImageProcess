#pragma once
#include "afxwin.h"


// WindowDialog �Ի���

class WindowDialog : public CDialog
{
	DECLARE_DYNAMIC(WindowDialog)

public:
	WindowDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WindowDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	// ���ڵ�����ֵ
	CString UpValue;
	// ���ڵ�����ֵ
	CString DownValue;

	bool flag;  //����Ի����ȷ������ȡ����ť

	afx_msg void OnBnClickedCancel();
};
