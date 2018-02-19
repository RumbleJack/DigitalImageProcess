
// DigitalImageProcessDlg.h : ͷ�ļ�
//

#pragma once

// �Ի���ͷ�ļ�
#include "GrayStretchDlg.h"
#include "IntensityDlg.h"
#include "AnyFilterDlg.h"

// ͼ������
#include  "PointDef.h"
#include "pointTrans.h"
#include  "ConvFilter.h"
#include  "Morphy.h"

// ͼ��������
#include "ProcessImage.h"
#include "SecurityImage.h"


class GrayStretchDlg;
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

	// ����¼���Ӧ
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	// �����ڻ�����֧��
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// ���µ������ڳߴ�
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnOpenPkg();
	afx_msg void OnOpenRaw();
	afx_msg void SelectGrayStretch();
	afx_msg void OnRecoveryOriginImage();
	afx_msg void OnDrawIntensity();
	afx_msg void OnAnyFilter();

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
	GrayStretchDlg *pGSD;
	// �˶Ի���ľ�ָ̬�룬���ھ�̬�����ڲ�
	static CDigitalImageProcessDlg* pThis;
	//flag == 0 ����recoverdata��flag == 1 ���� mypixel
	static void callBackFun(int low, int high, int flag = 0);

};

