
// DigitalImageProcessDlg.h : ͷ�ļ�

#pragma once

#include "SecurityImage.h"

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

	DECLARE_MESSAGE_MAP()
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	// ����¼���Ӧ
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	// �����ڻ�����֧��
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// ���µ������ڳߴ�
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// �Զ�����Ϣ
	afx_msg void OnEnSingleEnergyImageLow();
	afx_msg void OnEnSingleEnergyImageHigh();
	afx_msg void OnEnDualEnergyImageBat();
public:
	//������ʾ
	CImage *m_pCImage;
	// ���ڶ�ȡ�����
	SecurityImage *m_pSecurityImage;

	// �������������ƶ���־
	int vhFlag;	// v��ʾ��ֱ���� h��ʾ����

	// ָ����ǰ��ʾͼ��
	enum SHOW_STATUS{ NOTHING, LOW, HIGH, ENHANCED_LOW, ENHANCED_HIGH, ENHANCED_DUAL};
	SHOW_STATUS m_showStatus;


	afx_msg void OnOpenpkg();
	afx_msg void OnShowseilow();
	afx_msg void OnShowseihigh();
	afx_msg void OnEnDualEnergyImage();
};

