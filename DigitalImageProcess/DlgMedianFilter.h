#pragma once


// CDlgMedianFilter �Ի���

class CDlgMedianFilter : public CDialog
{
	DECLARE_DYNAMIC(CDlgMedianFilter)

public:
	CDlgMedianFilter(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMedianFilter();

// �Ի�������
	enum { IDD = IDD_MedianFilter };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_iTempH;
	int m_iTempW;
	int m_iTempMX;
	int m_iTempMY;
};
