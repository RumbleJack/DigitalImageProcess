
// DigitalImageProcessDlg.h : 头文件
//

#pragma once

// 对话框头文件
#include "ThresholdDialog.h"
#include "GrayStretchDialog.h"
#include "WindowDialog.h"
#include "pointTrans.h"
#include "IntensityDlg.h"
#include "DlgMedianFilter.h"
#include "DlgAnyFilter.h"
#include "UnsharpMaskingDlg.h"
#include "GradSharpDlg.h"

// 图像处理类
#include  "PointDef.h"
#include  "ConvFilter.h"
#include  "Morphy.h"

// 图像数据类
#include "ProcessImage.h"
#include "SecurityImage.h"


class GrayStretchDialog;
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

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// 主窗口滑动条支持
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// 重新调整窗口尺寸
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnOpenRaw();

	afx_msg void SelectThreshold();
	afx_msg void SelectGrayStretch();
	afx_msg void SelectWindow();
	afx_msg void OnInteEqualize();
	afx_msg void OnRecovery();
	afx_msg void OnGrayReverse();
	afx_msg void OnDrawIntensity();
	afx_msg void OnMedianFilter();
	afx_msg void OnAnyFilter();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUnsharpMasking();
	afx_msg void OnRobert();
	afx_msg void OnGauss();
	afx_msg void OnGardSharp();
	afx_msg void OnCanny();

	afx_msg void OnLungFetch();
	afx_msg void OnReplaceImage();

public:
	//用于显示
	CImage img;
	//GUI图像类
	ProcessImage processImg;
	SecurityImage *m_SecurityImage;
	//图像处理库类
	ListImage *m_ListImage;

	//点变换类
	PointTrans *pPT;
	//卷积类
	ConvFilter *pCF;
	//形态学变换类
	Morphy *pMorphy;
	Morphy morphy;

	//卷积模式
	int mode;
	
	//卷积模板
	FilterKernel *pTemp;

	// 滚动条的左右移动标志
	int vhFlag;	// v表示竖直方向， h表示横向


	// 图像尺寸参数
	unsigned int x;
	unsigned int y;
	unsigned int m_ImageW;
	unsigned int m_ImageH;

public:
	// 同于创建非模态对话框
	GrayStretchDialog *pTD;
	// 此对话框的静态指针，用于静态函数内部
	static CDigitalImageProcessDlg* pThis;
	//flag == 0 处理recoverdata，flag == 1 处理 mypixel
	static void callBackFun(int low, int high, int flag = 0);
public:
	afx_msg void OnOpenPkg();
};