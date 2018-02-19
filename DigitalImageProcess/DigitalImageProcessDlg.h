
// DigitalImageProcessDlg.h : ͷ�ļ�
//

#pragma once

// �Ի���ͷ�ļ�
#include "ThresholdDialog.h"
#include "GrayStretchDialog.h"
#include "WindowDialog.h"
#include "pointTrans.h"
#include "IntensityDlg.h"
#include "DlgMedianFilter.h"
#include "DlgAnyFilter.h"
#include "UnsharpMaskingDlg.h"
#include "GradSharpDlg.h"

// ͼ������
#include  "PointDef.h"
#include  "ConvFilter.h"
#include  "Morphy.h"

// ͼ��������
#include "ProcessImage.h"
#include "SecurityImage.h"


class GrayStretchDialog;
// CDigitalImageProcessDlg �Ի���
class CDigitalImageProcessDlg : public CDialog
{
	// ����
public:
	CDigitalImageProcessDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CDigitalImageProcessDlg();	// ��׼��������

	// �Ի�������
	enum { IDD = IDD_DIGITALIMAGEPROCESS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// �����ڻ�����֧��
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// ���µ������ڳߴ�
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
	//������ʾ
	CImage img;
	//GUIͼ����
	ProcessImage processImg;
	SecurityImage *m_SecurityImage;
	//ͼ�������
	ListImage *m_ListImage;

	//��任��
	PointTrans *pPT;
	//�����
	ConvFilter *pCF;
	//��̬ѧ�任��
	Morphy *pMorphy;
	Morphy morphy;

	//���ģʽ
	int mode;
	
	//���ģ��
	FilterKernel *pTemp;

	// �������������ƶ���־
	int vhFlag;	// v��ʾ��ֱ���� h��ʾ����


	// ͼ��ߴ����
	unsigned int x;
	unsigned int y;
	unsigned int m_ImageW;
	unsigned int m_ImageH;

public:
	// ͬ�ڴ�����ģ̬�Ի���
	GrayStretchDialog *pTD;
	// �˶Ի���ľ�ָ̬�룬���ھ�̬�����ڲ�
	static CDigitalImageProcessDlg* pThis;
	//flag == 0 ����recoverdata��flag == 1 ���� mypixel
	static void callBackFun(int low, int high, int flag = 0);
public:
	afx_msg void OnOpenPkg();
};