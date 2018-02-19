#pragma once


// CIntensityDlg 对话框

class CIntensityDlg : public CDialog
{
	DECLARE_DYNAMIC(CIntensityDlg)

public:
	CIntensityDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIntensityDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
		// 当前鼠标拖动状态，0表示未拖动，1表示正在拖动下限，2表示正在拖动上限。
	int		m_iIsDraging;
	
	// 相应鼠标事件的矩形区域
	CRect	m_MouseRect;
	
	// DIB的高度
	LONG	m_lHeight;
	
	// DIB的宽度
	LONG	m_lWidth;
	
	// 指向当前DIB象素的指针
	unsigned char *	m_lpDIBBits;
	
	// 各个灰度值的计数
	LONG	m_lCount[256];

	// 显示灰度区间的下限
	int		m_iLowGray;
	
	// 显示灰度区间的上限
	int		m_iUpGray;
};
