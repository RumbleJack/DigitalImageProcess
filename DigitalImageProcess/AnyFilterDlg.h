#pragma once


// CAnyFilterDlg 对话框
class CAnyFilterDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnyFilterDlg)

public:
	CAnyFilterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnyFilterDlg();

// 对话框数据
	enum { IDD = IDD_AnyFilter };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio3();	
	afx_msg void OnBnClickedOk();
public:
	void UpdateEdit();
public:
		int		m_intType;		// 模板类型

	int		m_iTempH;
	int		m_iTempW;
	int		m_iTempMX;
	int		m_iTempMY;
	float	m_fTempC;

	FLOAT * m_fpArray;

};
