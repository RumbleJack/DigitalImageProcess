
// DigitalImageProcessDlg.h : 头文件

#pragma once

#include "SecurityImage.h"

// CDigitalImageProcessDlg 对话框
class CDigitalImageProcessDlg : public CDialog
{
	// 构造
public:
	CDigitalImageProcessDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CDigitalImageProcessDlg();	// 标准析构函数

	// 对话框数据
	enum { IDD = IDD_DIGITALIMAGEPROCESS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	// 实现
protected:
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	// 鼠标事件响应
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	// 主窗口滑动条支持
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// 重新调整窗口尺寸
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// 自定义消息
	afx_msg void OnEnSingleEnergyImageLow();
	afx_msg void OnEnSingleEnergyImageHigh();
	afx_msg void OnEnDualEnergyImageBat();
public:
	//用于显示
	CImage *m_pCImage;
	// 用于读取处理等
	SecurityImage *m_pSecurityImage;

	// 滚动条的左右移动标志
	int vhFlag;	// v表示竖直方向， h表示横向

	// 指定当前显示图像
	enum SHOW_STATUS{ NOTHING, LOW, HIGH, ENHANCED_LOW, ENHANCED_HIGH, ENHANCED_DUAL};
	SHOW_STATUS m_showStatus;


	afx_msg void OnOpenpkg();
	afx_msg void OnShowseilow();
	afx_msg void OnShowseihigh();
	afx_msg void OnEnDualEnergyImage();
};

