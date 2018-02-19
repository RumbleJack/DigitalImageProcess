// DigitalImageProcessDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "DigitalImageProcessDlg.h"

#include "ShowSIDlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDigitalImageProcessDlg �Ի���
CDigitalImageProcessDlg::CDigitalImageProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDigitalImageProcessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDigitalImageProcessDlg::~CDigitalImageProcessDlg()
{
	if(!m_pCImage->IsNull())
	{
		m_pCImage->Destroy();
	}
	delete m_pSecurityImage;
}

void CDigitalImageProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDigitalImageProcessDlg, CDialog)
	ON_WM_SYSCOMMAND() // �������Ҽ��˵�
	ON_WM_PAINT()		// �ͻ����ػ�
	ON_WM_QUERYDRAGICON()// �϶���С������

	// ������
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	// ����¼�
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	// �������ڴ�С
	ON_WM_SIZE()
	// ����������Ϣ��Ӧ
	ON_WM_ERASEBKGND()

	// �Զ����¼���Ӧ
	ON_COMMAND(ID_EnSEILow, &CDigitalImageProcessDlg::OnEnSingleEnergyImageLow)
	ON_COMMAND(ID_EnSEIHigh, &CDigitalImageProcessDlg::OnEnSingleEnergyImageHigh)
	ON_COMMAND(ID_EnDEIBat, &CDigitalImageProcessDlg::OnEnDualEnergyImageBat)
	
END_MESSAGE_MAP()


// CDigitalImageProcessDlg ��Ϣ�������

BOOL CDigitalImageProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	// ���崰�ڳ�ʼ����ʾ��ʽ
	ShowWindow(SW_SHOWMAXIMIZED);
	//// ��ʼ���Ի���λ��
	//int   cx, cy;
	//cx = GetSystemMetrics(SM_CXSCREEN);
	//cy = GetSystemMetrics(SM_CYSCREEN)-40 ;
	////����MoveWindow
	//CRect   rcTemp;
	//rcTemp.TopLeft() = CPoint(0, 0);
	//rcTemp.BottomRight() = CPoint(cx, cy);
	//MoveWindow(&rcTemp);

	// ���Ի�����ӹ����� SB_VERT����ֱ����SB_HORZ��ˮƽ����
	CRect rc;
	GetClientRect(&rc);
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nPos = 0;
	si.nMin = 0;			// �˴�nMin��nMax��nPosΪ�߼�ֵ����ʵ�ʴ��ڳߴ��޹أ�ʹ�ÿͻ����ڳߴ�ֻ�����ο�
	si.nMax = rc.Height();   
	si.nPage = rc.Height()/5;
	SetScrollInfo(SB_VERT, &si, FALSE);
	si.nMax = rc.Width();
	si.nPage = rc.Width()/5;
	SetScrollInfo(SB_HORZ, &si, FALSE);
	// Ĭ�Ϲ���ͼƬ���򻬶�
	vhFlag = 1;

	// ָ�������ʼ��
	m_pCImage = new CImage;
	m_pSecurityImage = new SecurityImage();

	// ָ����ǰ��ʾͼ��
	m_showStatus = SHOW_STATUS::NOTHING;

	// ���ǽ��������õ��ؼ������򷵻� TRUE
	return TRUE;
}

void CDigitalImageProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

//  �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDigitalImageProcessDlg::OnPaint()
{
	Invalidate(1);

	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// ��ȡ�豸������
		CPaintDC dc(this);

		SetStretchBltMode(dc.GetSafeHdc(), HALFTONE);
		// SetBrushOrgEx(dc.GetSafeHdc(),0,0,NULL);

		if (!m_pCImage->IsNull())
		{
			// ��ȡ���������߼�λ��posH��posV
			SCROLLINFO vinfoH;
			SCROLLINFO vinfoV;
			GetScrollInfo(SB_HORZ, &vinfoH,SIF_ALL);
			GetScrollInfo(SB_VERT, &vinfoV, SIF_ALL);
			int posH = vinfoH.nPos;
			int posV = vinfoV.nPos;

			// ������Ҫ��ʾͼ��ߴ綨����Σ�����2����ά���꣩
			int width = m_pSecurityImage->col;
			int height = m_pSecurityImage->row;
			CRect rect;
			rect.SetRect(0,0, width, height);

			// ����ͼƬ���Ʒ�Χ������2����ά���꣩����0.8������posH��posV���ֵ��0.8Max
			rect.SetRect(rect.TopLeft().x - (width / (vinfoH.nMax*0.8))*posH,
				rect.TopLeft().y - (height / (vinfoV.nMax*0.8))*posV,
				rect.BottomRight().x - (width / (vinfoH.nMax*0.8))*posH,
				rect.BottomRight().y - (height / (vinfoV.nMax*0.8))*posV);
			//GetClientRect(&rect);
			// ʹ��CImage����ͼƬ
			m_pCImage->Draw(dc.GetSafeHdc(), rect);
		}

		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CDigitalImageProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CDigitalImageProcessDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CDialog::OnEraseBkgnd(pDC);
	return true;
}

BOOL CDigitalImageProcessDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta < 0)
	{
		if (vhFlag == 1)
		{
			OnVScroll(SB_LINEDOWN, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));
		}
		if (vhFlag == 2)
		{
			OnHScroll(SB_LINEDOWN, GetScrollPos(SB_HORZ), GetScrollBarCtrl(SB_HORZ));
		}
	}
	else if (zDelta > 0)
	{
		if (vhFlag == 1)
		{
			OnVScroll(SB_LINEUP, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));
		}
		if (vhFlag == 2)
		{
			OnHScroll(SB_LINEUP, GetScrollPos(SB_HORZ), GetScrollBarCtrl(SB_HORZ));
		}
	}

	//vhFlag = 1;
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CDigitalImageProcessDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (nChar == VK_SHIFT)
	{
		vhFlag = 2;
		// AfxMessageBox(_T("Shift"));
	}
	//vhFlag = 1;

	if (nChar == VK_CONTROL)
	{
		// vhFlag = 2;
		// AfxMessageBox(_T("Ctrl"));
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDigitalImageProcessDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (nChar == VK_SHIFT)
	{
		vhFlag = 1;
		// AfxMessageBox(_T("Shift"));
	}
	//vhFlag = 1;

	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CDigitalImageProcessDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int pos, min, max, thumbwidth;

	SCROLLINFO vinfo;
	GetScrollInfo(SB_HORZ, &vinfo);

	pos = vinfo.nPos;
	min = vinfo.nMin;
	max = vinfo.nMax;
	thumbwidth = vinfo.nPage;

	switch (nSBCode)
	{
	case SB_THUMBTRACK: //�϶�������	
		ScrollWindow(-((int)nPos - pos), 0);
		SetScrollPos(SB_HORZ, nPos);
		break;
	case SB_LINELEFT: //�������ͷ
		if (pos != 0)
		{
			ScrollWindow(1, 0);
			SetScrollPos(SB_HORZ, pos - 1);
		}
		break;
	case SB_LINERIGHT: //�����Ҽ�ͷ
		if (pos + thumbwidth <= max)
		{
			SetScrollPos(SB_HORZ, pos + 1);
			ScrollWindow(-1, 0);
		}
		break;
	case SB_PAGELEFT: //�ڹ�������󷽿հ׹������򵥻�
		if (pos >= thumbwidth)
		{
			ScrollWindow(thumbwidth, 0);
			SetScrollPos(SB_HORZ, pos - thumbwidth);
		}
		else
		{
			ScrollWindow(pos, 0);
			SetScrollPos(SB_HORZ, 0);
		}
		break;
	case SB_PAGERIGHT: //�ڹ�������ҷ��հ׹������򵥻�
		if (pos + thumbwidth <= max - thumbwidth)
		{
			ScrollWindow(-thumbwidth, 0);
			SetScrollPos(SB_HORZ, pos + thumbwidth);
		}
		else
		{
			ScrollWindow(-(max - (pos + thumbwidth)), 0);
			SetScrollPos(SB_HORZ, max - thumbwidth);
		}
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDigitalImageProcessDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int pos, min, max, thumbwidth;

	SCROLLINFO vinfo;
	GetScrollInfo(SB_VERT, &vinfo);

	pos = vinfo.nPos;
	min = vinfo.nMin;
	max = vinfo.nMax;
	thumbwidth = vinfo.nPage;

	switch (nSBCode)
	{
	case SB_THUMBTRACK:		// �϶�����
		ScrollWindow(0, -((int)nPos - pos));
		SetScrollPos(SB_VERT, nPos);
		break;
	case SB_LINELEFT:
		SetScrollPos(SB_VERT, pos - 1);
		if (pos != 0)
			ScrollWindow(0, 1);
		break;
	case SB_LINERIGHT:
		SetScrollPos(SB_VERT, pos + 1);
		if (pos + thumbwidth < max)
			ScrollWindow(0, -1);
		break;
	case SB_PAGELEFT: //�ڹ�������Ϸ��հ׹������򵥻�
		if (pos >= thumbwidth)
		{
			ScrollWindow(0, thumbwidth);
			SetScrollPos(SB_VERT, pos - thumbwidth);
		}
		else
		{
			ScrollWindow(0, pos);
			SetScrollPos(SB_VERT, 0);
		}
		break;
	case SB_PAGERIGHT: //�ڹ�������·��հ׹������򵥻�
		if (pos + thumbwidth <= max - thumbwidth)
		{
			ScrollWindow(0, -thumbwidth);
			SetScrollPos(SB_VERT, pos + thumbwidth);
		}
		else
		{
			ScrollWindow(0, -(max - (pos + thumbwidth)));
			SetScrollPos(SB_VERT, max - thumbwidth);
		}
		break;
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDigitalImageProcessDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

}

void CDigitalImageProcessDlg::OnEnSingleEnergyImageLow()
{
	// �½����ļ��Ի���
	CFileDialog file(1);
	file.m_ofn.lpstrTitle = _T("��ͼƬ");

	// ���δ���ȷ���򷵻�
	if (file.DoModal() != IDOK)
		return;

	// ��ȡ·��
	CString fileName = file.GetPathName();

	// ProcessImage����ͼ���Զ����ʽ��
	m_pSecurityImage->readImageFromFile(fileName);

	// ���CImage
	if (!m_pCImage->IsNull())
		m_pCImage->Destroy();
	m_pCImage->Create(m_pSecurityImage->col, m_pSecurityImage->row, 24);
	m_pSecurityImage->setCImageFast(m_pSecurityImage->getLowEnergyImage(),*m_pCImage);

	// ʹ�������ڿͻ�����Ч
	Invalidate();
}

void CDigitalImageProcessDlg::OnEnSingleEnergyImageHigh()
{
	// �½����ļ��Ի���
	CFileDialog file(1);
	file.m_ofn.lpstrTitle = _T("��ͼƬ");

	// ���δ���ȷ���򷵻�
	if (file.DoModal() != IDOK)
		return;

	// ��ȡ·��
	CString fileName = file.GetPathName();

	// ProcessImage����ͼ���Զ����ʽ��
	m_pSecurityImage->readImageFromFile(fileName);

	// ���CImage
	if (!m_pCImage->IsNull())
		m_pCImage->Destroy();
	m_pCImage->Create(m_pSecurityImage->col, m_pSecurityImage->row, 24);
	m_pSecurityImage->setCImageFast(m_pSecurityImage->getHighEnergyImage(), *m_pCImage);

	//// ��ʾ��ָ���Ի�����
	//ShowSIDlg showSIDlg;
	//showSIDlg.m_CImg = m_pCImage;
	////��ʾ�Ի���
	//if (showSIDlg.DoModal() != IDOK)
	//	return;

	// ʹ�������ڿͻ�����Ч
	Invalidate();
}

void CDigitalImageProcessDlg::OnEnDualEnergyImageBat()
{
	// �½����ļ��Ի���
	CFileDialog file(1);
	file.m_ofn.lpstrTitle = _T("��·���ļ�");

	// ���δ���ȷ���򷵻�
	if (file.DoModal() != IDOK)
		return;

	// ��ȡ·��
	CString fileName = file.GetPathName();

	// ��������ͼ��
	m_pSecurityImage->enhanceALL(string(fileName));
}


