#pragma once


// CIntensityDlg �Ի���

class CIntensityDlg : public CDialog
{
	DECLARE_DYNAMIC(CIntensityDlg)

public:
	CIntensityDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIntensityDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusEditLowgray();
	afx_msg void OnEnKillfocusEditUpgray();
	afx_msg void OnBnClickedOk();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP();

public:
		// ��ǰ����϶�״̬��0��ʾδ�϶���1��ʾ�����϶����ޣ�2��ʾ�����϶����ޡ�
	int		m_iIsDraging;
	
	// ��Ӧ����¼��ľ�������
	CRect	m_MouseRect;
	
	// DIB�ĸ߶�
	LONG	m_lHeight;
	
	// DIB�Ŀ��
	LONG	m_lWidth;
	
	// ָ��ǰDIB���ص�ָ��
	unsigned char *	m_lpDIBBits;
	
	// �����Ҷ�ֵ�ļ���
	LONG	m_lCount[256];

	// ��ʾ�Ҷ����������
	int		m_iLowGray;
	
	// ��ʾ�Ҷ����������
	int		m_iUpGray;
};
