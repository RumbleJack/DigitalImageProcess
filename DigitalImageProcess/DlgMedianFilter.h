#pragma once


// CDlgMedianFilter 对话框

class CDlgMedianFilter : public CDialog
{
	DECLARE_DYNAMIC(CDlgMedianFilter)

public:
	CDlgMedianFilter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMedianFilter();

// 对话框数据
	enum { IDD = IDD_MedianFilter };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iTempH;
	int m_iTempW;
	int m_iTempMX;
	int m_iTempMY;
};
