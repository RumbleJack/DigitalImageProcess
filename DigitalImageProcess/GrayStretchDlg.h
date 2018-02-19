#pragma once
#include "afxcmn.h"
#include "DigitalImageProcessDlg.h"
class DigitalImageProcessDlg;
// GrayStretchDlg 对话框
typedef void (*FPtr)(int,int,int);
class GrayStretchDlg : public CDialog
{
	DECLARE_DYNAMIC(GrayStretchDlg)

public:
	GrayStretchDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GrayStretchDlg();

	// 对话框数据
	enum { IDD = IDD_GrayStretchDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 起始点的横坐标
	CString X1;
	// 结束点的横坐标
	CString X2;

	bool flag;  //点击对话框的确定还是取消按钮
	int   imageFlag; //处理图像选择

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	// 灰度拉伸低阈值
	CSliderCtrl stretchGrayLow;
	// 灰度拉伸高阈值
	CSliderCtrl stretchGrayHigh;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void CallBankFunc(FPtr ptr,int low,int high)
	{
		ptr(low,high,imageFlag);
	}
	void setImageFlag(int arg)
	{
      imageFlag	= arg;
	}
};
