#pragma once


// CDlgAnyFilter �Ի���
class CDlgAnyFilter : public CDialog
{
	DECLARE_DYNAMIC(CDlgAnyFilter)

public:
	CDlgAnyFilter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAnyFilter();

// �Ի�������
	enum { IDD = IDD_AnyFilter };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio3();	
	afx_msg void OnBnClickedOk();
public:
	void UpdateEdit();
public:
		int		m_intType;		// ģ������

	int		m_iTempH;
	int		m_iTempW;
	int		m_iTempMX;
	int		m_iTempMY;
	float	m_fTempC;

	FLOAT * m_fpArray;

};
