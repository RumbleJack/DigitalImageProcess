// DigitalImageProcessDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "DigitalImageProcessDlg.h"

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


CDigitalImageProcessDlg* CDigitalImageProcessDlg::pThis = NULL;

// CDigitalImageProcessDlg �Ի���
CDigitalImageProcessDlg::CDigitalImageProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDigitalImageProcessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDigitalImageProcessDlg::~CDigitalImageProcessDlg()
{
	if(!img.IsNull())
	{
		img.Destroy();
	}
	delete m_SecurityImage;
	delete m_ListImage;
	delete pCF;
	delete pTemp;
	delete pMorphy;
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
	ON_COMMAND(ID_GrayStretch, &CDigitalImageProcessDlg::SelectGrayStretch) //����λ����
	ON_COMMAND(ID_OpenRaw, &CDigitalImageProcessDlg::OnOpenRaw)
	ON_COMMAND(ID_RecoveryOriginImage, &CDigitalImageProcessDlg::OnRecoveryOriginImage)
	ON_COMMAND(ID_DrawIntensity, &CDigitalImageProcessDlg::OnDrawIntensity)
	ON_COMMAND(ID_AnyFilter, &CDigitalImageProcessDlg::OnAnyFilter)
	ON_COMMAND(ID_OpenPkg, &CDigitalImageProcessDlg::OnOpenPkg)
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

	int   cx, cy;
	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN) - 40;

	//����MoveWindow
	CRect   rcTemp;
	rcTemp.BottomRight() = CPoint(cx, cy);
	rcTemp.TopLeft() = CPoint(0, 0);
	MoveWindow(&rcTemp);

	m_ListImage = new ListImage();
	pCF = new ConvFilter();
	pTemp = new FilterKernel();
	pMorphy = new Morphy();

	//m_ListImage = img;
	mode = 0;

	// Ĭ�Ϲ���ͼƬ���򻬶�
	vhFlag = 1;

	// ��ָ̬�븳ֵ
	pThis = this;

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

		if (!img.IsNull())
		{
			// ��ȡ���������߼�λ��posH��posV
			SCROLLINFO vinfoH;
			SCROLLINFO vinfoV;
			GetScrollInfo(SB_HORZ, &vinfoH,SIF_ALL);
			GetScrollInfo(SB_VERT, &vinfoV, SIF_ALL);
			int posH = vinfoH.nPos;
			int posV = vinfoV.nPos;

			// ������Ҫ��ʾͼ��ߴ綨����Σ�����2����ά���꣩
			CRect rect;
			rect.SetRect(x, y, m_ImageW, m_ImageH);

			// ����ͼƬ���Ʒ�Χ������2����ά���꣩����0.8������posH��posV���ֵ��0.8Max
			rect.SetRect(rect.TopLeft().x - (m_ImageW / (vinfoH.nMax*0.8) )*posH,
				rect.TopLeft().y - (m_ImageH / (vinfoV.nMax*0.8) )*posV,
				rect.BottomRight().x - (m_ImageW / (vinfoH.nMax*0.8))*posH,
				rect.BottomRight().y - (m_ImageH / (vinfoV.nMax*0.8))*posV);

			/*GetClientRect(&rect);*/
			// ʹ��CImage����ͼƬ
			img.Draw(dc.GetSafeHdc(), rect);
		}

		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CDigitalImageProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDigitalImageProcessDlg::OnOpenRaw()
{
	// �½����ļ��Ի���
	CFileDialog file(1);
	file.m_ofn.lpstrTitle = _T("��ͼƬ");

	// ���δ���ȷ���򷵻�
	if (file.DoModal() != IDOK)
		return;

	// ��ȡ·��
	CString fileName = file.GetPathName();

	// CImage����ͼ��CImage����֧�ָ�ʽ��
	HRESULT ret = img.Load(fileName);
	if (!FAILED(ret))
	{
		// ����ͼ��
		return;
	}

	// ProcessImage����ͼ���Զ����ʽ��
	processImg.readFile(fileName);
	// ����ͼ��ߴ����
	x = 0;
	y = 0;
	m_ImageW = 2048;
	m_ImageH = 2099;
	if (!img.IsNull())
		img.Destroy();
	img.Create(m_ImageW, m_ImageH, 24);

	processImg.flag = true;
	//���ٴ�rawͼƬ
	processImg.setCImageFast(img);

	// ʹ�������ڿͻ�����Ч
	Invalidate();
}

// ����Ҷ��������ʼ��ͽ�����
void CDigitalImageProcessDlg::SelectGrayStretch()
{
	if (img.IsNull())
		return;

	//���ó�Ա��������һ����ģ̬�Ի���  
	pGSD = new GrayStretchDlg(); //��ָ������ڴ�  
	pGSD->Create(IDD_GrayStretchDlg); //����һ����ģ̬�Ի���  
	pGSD->ShowWindow(SW_SHOWNORMAL); //��ʾ��ģ̬�Ի��� 
	pGSD->setImageFlag(0);			//ÿ�δ���ԭʼͼ��
}

void CDigitalImageProcessDlg::OnRecoveryOriginImage()
{
	if (img.IsNull())
		return;

	if (processImg.m_pixel != NULL)
		delete processImg.m_pixel;

	int len = 2 * img.GetHeight() * img.GetWidth();
	processImg.m_pixel = new unsigned short[len / 2];
	for (int i = 0; i < len / 2; i++)
		processImg.m_pixel[i] = processImg.originData[i];

	processImg.setCImageFast(img);

	Invalidate();
}

void CDigitalImageProcessDlg::OnDrawIntensity()
{
	if (img.IsNull())
		return;

	//�����Ի���
	CIntensityDlg dlgdraw;

	//��ʼ������ֵ
	dlgdraw.m_lpDIBBits = new unsigned char[2048 * 2099];
	for (int i = 0; i < 2048 * 2099; i++)
	{
		dlgdraw.m_lpDIBBits[i] = (unsigned char)(processImg.m_pixel[i] / 256);
	}
	//dlgdraw.m_lpDIBBits = processImg.m_pixel;
	dlgdraw.m_lWidth = 2048;
	dlgdraw.m_lHeight = 2099;
	dlgdraw.m_iLowGray = 0;
	dlgdraw.m_iUpGray = 65535;

	//��ʾ�Ի���
	if (dlgdraw.DoModal() != IDOK)
	{
		return;
	}
}

void CDigitalImageProcessDlg::OnAnyFilter()
{
	if (img.IsNull())
		return;

	//�����Ի���
	CAnyFilterDlg smooth;

	//��ʾ�Ի���
	if (smooth.DoModal() != IDOK)
	{
		return;
	}

	//��ȡģ�����
	pTemp->fCoef = smooth.m_fTempC;
	pTemp->iTempH = smooth.m_iTempH;
	pTemp->iTempW = smooth.m_iTempW;
	pTemp->iTempMX = smooth.m_iTempMX;
	pTemp->iTempMY = smooth.m_iTempMY;
	pTemp->fpArray = new float[pTemp->iTempH * pTemp->iTempW];
	for (int i = 0; i < pTemp->iTempH * pTemp->iTempW; i++)
	{
		pTemp->fpArray[i] = smooth.m_fpArray[i];
	}

	//�Ƿ����ͼ��
	if (img.IsNull())
		return;

	//���ݴ���
	m_ListImage->SetSize(2048, 2099);
	m_ListImage->SetChannel(1);
	m_ListImage->SetType(uint_16);
	m_ListImage->SetData((unsigned char*)processImg.m_pixel, 2048 * 2099 * 2);

	//�������ͼ������
	ListImage destListImage;
	pCF->SetFK(*pTemp);
	pCF->TemplateConv(*m_ListImage, destListImage, mode);
	*m_ListImage = destListImage;

	//����д��
	memcpy(processImg.m_pixel, m_ListImage->GetImgBuffer(), 2048 * 2099 * 2);
	processImg.setCImageFast(img);

	//ͼ����ʾ
	Invalidate();//ǿ�Ƶ���OnDraw����

	MessageBox("�˲������");
}

BOOL CDigitalImageProcessDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	// return CDialog::OnEraseBkgnd(pDC);
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

void  CDigitalImageProcessDlg::callBackFun(int low, int high, int flag)
{
	if (pThis->img.IsNull())
		return;

	//��ʼ��
	int countOfPix = 2048 * 2099;
	unsigned short *srcData = new unsigned short[countOfPix];

	if (flag == 0){
		memcpy(srcData, pThis->processImg.originData, countOfPix * 2);
	}
	else
	{
		memcpy(srcData, pThis->processImg.m_lung, countOfPix * 2);
	}

	//���ݴ���
	pThis->m_ListImage->SetSize(2048, 2099);
	pThis->m_ListImage->SetChannel(1);
	pThis->m_ListImage->SetType(uint_16);
	pThis->m_ListImage->SetData((unsigned char*)srcData, countOfPix * 2);

	pThis->pPT->GrayStretchUint_16(pThis->m_ListImage,
		(unsigned short)low, 0,
		(unsigned short)high, 65535);

	//����д��
	memcpy((unsigned char*)pThis->processImg.m_pixel, pThis->m_ListImage->GetImgBuffer(), countOfPix * 2);
	pThis->processImg.setCImageFast(pThis->img);

	delete[] srcData;

	//ͼ����ʾ
	pThis->Invalidate();//ǿ�Ƶ���OnDraw����
}

void CDigitalImageProcessDlg::OnOpenPkg()
{
	// TODO:  �ڴ���������������
}
