#pragma once
#include "afxcmn.h"
#include "DigitalImageProcessDlg.h"
class DigitalImageProcessDlg;
// GrayStretchDlg �Ի���
typedef void (*FPtr)(int,int,int);
class GrayStretchDlg : public CDialog
{
	DECLARE_DYNAMIC(GrayStretchDlg)

public:
	GrayStretchDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GrayStretchDlg();

	// �Ի�������
	enum { IDD = IDD_GrayStretchDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ��ʼ��ĺ�����
	CString X1;
	// ������ĺ�����
	CString X2;

	bool flag;  //����Ի����ȷ������ȡ����ť
	int   imageFlag; //����ͼ��ѡ��

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	// �Ҷ��������ֵ
	CSliderCtrl stretchGrayLow;
	// �Ҷ��������ֵ
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
