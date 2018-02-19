// DigitalImageProcessDlg.cpp : 实现文件

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "DigitalImageProcessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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

// CDigitalImageProcessDlg 对话框
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
	ON_WM_SYSCOMMAND() // 标题栏右键菜单
	ON_WM_PAINT()		// 客户区重绘
	ON_WM_QUERYDRAGICON()// 拖动最小化窗口

	// 滚动条
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	// 鼠标事件
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	// 调整窗口大小
	ON_WM_SIZE()
	// 背景擦除消息响应
	ON_WM_ERASEBKGND()

	// 自定义事件响应
	ON_COMMAND(ID_GrayStretch, &CDigitalImageProcessDlg::SelectGrayStretch) //窗宽窗位调整
	ON_COMMAND(ID_OpenRaw, &CDigitalImageProcessDlg::OnOpenRaw)
	ON_COMMAND(ID_RecoveryOriginImage, &CDigitalImageProcessDlg::OnRecoveryOriginImage)
	ON_COMMAND(ID_DrawIntensity, &CDigitalImageProcessDlg::OnDrawIntensity)
	ON_COMMAND(ID_AnyFilter, &CDigitalImageProcessDlg::OnAnyFilter)
	ON_COMMAND(ID_OpenPkg, &CDigitalImageProcessDlg::OnOpenPkg)
END_MESSAGE_MAP()


// CDigitalImageProcessDlg 消息处理程序

BOOL CDigitalImageProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	// 定义窗口初始化显示方式
	ShowWindow(SW_SHOWMAXIMIZED);

	// 给对话框添加滚动条 SB_VERT：垂直方向，SB_HORZ：水平方向
	CRect rc;
	GetClientRect(&rc);
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	si.nPos = 0;
	si.nMin = 0;			// 此处nMin、nMax、nPos为逻辑值，与实际窗口尺寸无关，使用客户窗口尺寸只是作参考
	si.nMax = rc.Height();   
	si.nPage = rc.Height()/5;
	SetScrollInfo(SB_VERT, &si, FALSE);
	si.nMax = rc.Width();
	si.nPage = rc.Width()/5;
	SetScrollInfo(SB_HORZ, &si, FALSE);

	int   cx, cy;
	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN) - 40;

	//再用MoveWindow
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

	// 默认滚轮图片纵向滑动
	vhFlag = 1;

	// 静态指针赋值
	pThis = this;

	// 除非将焦点设置到控件，否则返回 TRUE
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

//  如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CDigitalImageProcessDlg::OnPaint()
{
	Invalidate(1);

	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 获取设备上下文
		CPaintDC dc(this);

		SetStretchBltMode(dc.GetSafeHdc(), HALFTONE);
		// SetBrushOrgEx(dc.GetSafeHdc(),0,0,NULL);

		if (!img.IsNull())
		{
			// 获取滚动条的逻辑位置posH、posV
			SCROLLINFO vinfoH;
			SCROLLINFO vinfoV;
			GetScrollInfo(SB_HORZ, &vinfoH,SIF_ALL);
			GetScrollInfo(SB_VERT, &vinfoV, SIF_ALL);
			int posH = vinfoH.nPos;
			int posV = vinfoV.nPos;

			// 根据需要显示图像尺寸定义矩形（设置2个二维坐标）
			CRect rect;
			rect.SetRect(x, y, m_ImageW, m_ImageH);

			// 设置图片绘制范围（设置2个二维坐标）乘以0.8是由于posH，posV最大值是0.8Max
			rect.SetRect(rect.TopLeft().x - (m_ImageW / (vinfoH.nMax*0.8) )*posH,
				rect.TopLeft().y - (m_ImageH / (vinfoV.nMax*0.8) )*posV,
				rect.BottomRight().x - (m_ImageW / (vinfoH.nMax*0.8))*posH,
				rect.BottomRight().y - (m_ImageH / (vinfoV.nMax*0.8))*posV);

			/*GetClientRect(&rect);*/
			// 使用CImage绘制图片
			img.Draw(dc.GetSafeHdc(), rect);
		}

		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CDigitalImageProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDigitalImageProcessDlg::OnOpenRaw()
{
	// 新建打开文件对话框
	CFileDialog file(1);
	file.m_ofn.lpstrTitle = _T("打开图片");

	// 如果未点击确定则返回
	if (file.DoModal() != IDOK)
		return;

	// 获取路径
	CString fileName = file.GetPathName();

	// CImage读入图像（CImage本身支持格式）
	HRESULT ret = img.Load(fileName);
	if (!FAILED(ret))
	{
		// 处理图像
		return;
	}

	// ProcessImage读入图像（自定义格式）
	processImg.readFile(fileName);
	// 设置图像尺寸参数
	x = 0;
	y = 0;
	m_ImageW = 2048;
	m_ImageH = 2099;
	if (!img.IsNull())
		img.Destroy();
	img.Create(m_ImageW, m_ImageH, 24);

	processImg.flag = true;
	//快速打开raw图片
	processImg.setCImageFast(img);

	// 使整个窗口客户区无效
	Invalidate();
}

// 输入灰度拉伸的起始点和结束点
void CDigitalImageProcessDlg::SelectGrayStretch()
{
	if (img.IsNull())
		return;

	//采用成员变量创建一个非模态对话框  
	pGSD = new GrayStretchDlg(); //给指针分配内存  
	pGSD->Create(IDD_GrayStretchDlg); //创建一个非模态对话框  
	pGSD->ShowWindow(SW_SHOWNORMAL); //显示非模态对话框 
	pGSD->setImageFlag(0);			//每次处理原始图像
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

	//创建对话框
	CIntensityDlg dlgdraw;

	//初始化变量值
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

	//显示对话框
	if (dlgdraw.DoModal() != IDOK)
	{
		return;
	}
}

void CDigitalImageProcessDlg::OnAnyFilter()
{
	if (img.IsNull())
		return;

	//创建对话框
	CAnyFilterDlg smooth;

	//显示对话框
	if (smooth.DoModal() != IDOK)
	{
		return;
	}

	//获取模板参数
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

	//是否存在图像
	if (img.IsNull())
		return;

	//数据处理
	m_ListImage->SetSize(2048, 2099);
	m_ListImage->SetChannel(1);
	m_ListImage->SetType(uint_16);
	m_ListImage->SetData((unsigned char*)processImg.m_pixel, 2048 * 2099 * 2);

	//处理过的图像数据
	ListImage destListImage;
	pCF->SetFK(*pTemp);
	pCF->TemplateConv(*m_ListImage, destListImage, mode);
	*m_ListImage = destListImage;

	//数据写回
	memcpy(processImg.m_pixel, m_ListImage->GetImgBuffer(), 2048 * 2099 * 2);
	processImg.setCImageFast(img);

	//图像显示
	Invalidate();//强制调用OnDraw函数

	MessageBox("滤波已完成");
}

BOOL CDigitalImageProcessDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
	case SB_THUMBTRACK: //拖动滚动块	
		ScrollWindow(-((int)nPos - pos), 0);
		SetScrollPos(SB_HORZ, nPos);
		break;
	case SB_LINELEFT: //单击左箭头
		if (pos != 0)
		{
			ScrollWindow(1, 0);
			SetScrollPos(SB_HORZ, pos - 1);
		}
		break;
	case SB_LINERIGHT: //单击右箭头
		if (pos + thumbwidth <= max)
		{
			SetScrollPos(SB_HORZ, pos + 1);
			ScrollWindow(-1, 0);
		}
		break;
	case SB_PAGELEFT: //在滚动块的左方空白滚动区域单击
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
	case SB_PAGERIGHT: //在滚动块的右方空白滚动区域单击
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
	case SB_THUMBTRACK:		// 拖动滑块
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
	case SB_PAGELEFT: //在滚动块的上方空白滚动区域单击
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
	case SB_PAGERIGHT: //在滚动块的下方空白滚动区域单击
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

	// TODO: 在此处添加消息处理程序代码

}

void  CDigitalImageProcessDlg::callBackFun(int low, int high, int flag)
{
	if (pThis->img.IsNull())
		return;

	//初始化
	int countOfPix = 2048 * 2099;
	unsigned short *srcData = new unsigned short[countOfPix];

	if (flag == 0){
		memcpy(srcData, pThis->processImg.originData, countOfPix * 2);
	}
	else
	{
		memcpy(srcData, pThis->processImg.m_lung, countOfPix * 2);
	}

	//数据处理
	pThis->m_ListImage->SetSize(2048, 2099);
	pThis->m_ListImage->SetChannel(1);
	pThis->m_ListImage->SetType(uint_16);
	pThis->m_ListImage->SetData((unsigned char*)srcData, countOfPix * 2);

	pThis->pPT->GrayStretchUint_16(pThis->m_ListImage,
		(unsigned short)low, 0,
		(unsigned short)high, 65535);

	//数据写回
	memcpy((unsigned char*)pThis->processImg.m_pixel, pThis->m_ListImage->GetImgBuffer(), countOfPix * 2);
	pThis->processImg.setCImageFast(pThis->img);

	delete[] srcData;

	//图像显示
	pThis->Invalidate();//强制调用OnDraw函数
}

void CDigitalImageProcessDlg::OnOpenPkg()
{
	// TODO:  在此添加命令处理程序代码
}
