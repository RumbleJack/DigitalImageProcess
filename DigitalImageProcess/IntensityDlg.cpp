// IntensityDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitalImageProcess.h"
#include "IntensityDlg.h"


// CIntensityDlg �Ի���

IMPLEMENT_DYNAMIC(CIntensityDlg, CDialog)

CIntensityDlg::CIntensityDlg(CWnd* pParent /*=NULL*/)
: CDialog(CIntensityDlg::IDD, pParent)
{
	m_iLowGray = 0;
	m_iUpGray = 65535;
	
}

BOOL CIntensityDlg::OnInitDialog() 
{
	// ָ��Դͼ�����ص�ָ��
	unsigned char * lpSrc;

	// ѭ������
	LONG i;
	LONG j;

	// ����Ĭ��OnInitDialog����
	CDialog::OnInitDialog();

	// ��ȡ����ֱ��ͼ�ı�ǩ
	CWnd* pWnd = GetDlgItem(IDC_COORD);

	// �����������¼�����Ч����
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);

	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);

	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;

	// ���ý�������¼�����Ч����
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;

	// ���ü���Ϊ0
	for (i = 0; i < 256; i ++)
	{
		// ����
		m_lCount[i] = 0;
	}


	// ��������Ҷ�ֵ�ļ���
	for (i = 0; i < m_lHeight; i ++)
	{
		for (j = 0; j < m_lWidth; j ++)
		{
			lpSrc = (unsigned char *)m_lpDIBBits +2048 * i + j;

			// ������1
			m_lCount[*(lpSrc)]++;
		}
	}

	// ��ʼ���϶�״̬
	m_iIsDraging = 0;

	// ����TRUE
	return TRUE;
}
CIntensityDlg::~CIntensityDlg()
{
}

void CIntensityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//�󶨱�����ؼ�
	DDX_Text(pDX, IDC_EDIT_LowGray, m_iLowGray);
	DDV_MinMaxInt(pDX, m_iLowGray, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_UpGray, m_iUpGray);
	DDV_MinMaxInt(pDX, m_iUpGray, 0, 65535);
}


BEGIN_MESSAGE_MAP(CIntensityDlg, CDialog)

	ON_WM_PAINT()

	ON_EN_KILLFOCUS(IDC_EDIT_LowGray, &CIntensityDlg::OnEnKillfocusEditLowgray)
	ON_EN_KILLFOCUS(IDC_EDIT_UpGray, &CIntensityDlg::OnEnKillfocusEditUpgray)
	ON_BN_CLICKED(IDOK, &CIntensityDlg::OnBnClickedOk)

	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	END_MESSAGE_MAP();


// CIntensityDlg ��Ϣ�������
void CIntensityDlg::OnPaint() 
{
	m_iLowGray	/= 256;	
	m_iUpGray 	/= 256;	

	// �ַ���
	CString str;

	// ѭ������
	LONG i;

	// ������
	LONG lMaxCount = 0;

	// �豸������
	CPaintDC dc(this);

	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_COORD);

	// ָ��
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	pDC->Rectangle(0,0,330,300);

	// �������ʶ���
	CPen* pPenRed = new CPen;

	// ��ɫ����
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));

	// �������ʶ���
	CPen* pPenBlue = new CPen;

	// ��ɫ����
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));

	// �������ʶ���
	CPen* pPenGreen = new CPen;

	// ��ɫ����
	pPenGreen->CreatePen(PS_DOT,1,RGB(0,255,0));

	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);

	// ����������
	pDC->MoveTo(10,10);

	// ��ֱ��
	pDC->LineTo(10,280);

	// ˮƽ��
	pDC->LineTo(320,280);

	// дX��̶�ֵ
	str.Format("0");
	pDC->TextOut(10, 283, str);
	str.Format("13107");
	pDC->TextOut(40, 283, str);
	str.Format("26214");
	pDC->TextOut(90, 283, str);
	str.Format("39321");
	pDC->TextOut(140, 283, str);
	str.Format("52482");
	pDC->TextOut(190, 283, str);
	str.Format("65535");
	pDC->TextOut(245, 283, str);

	// ����X��̶�
	for (i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10�ı���
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 284);
		}
		else
		{
			// 10�ı���
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 282);
		}
	}

	// ����X���ͷ
	pDC->MoveTo(315,275);
	pDC->LineTo(320,280);
	pDC->LineTo(315,285);

	// ����X���ͷ
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);

	// ����������ֵ
	for (i = m_iLowGray; i <= m_iUpGray; i ++)
	{
		// �ж��Ƿ���ڵ�ǰ���ֵ
		if (m_lCount[i] > lMaxCount)
		{
			// �������ֵ
			lMaxCount = m_lCount[i];
		}
	}

	// ���������ֵ
	pDC->MoveTo(10, 25);
	pDC->LineTo(14, 25);
	str.Format("%d", lMaxCount);
	pDC->TextOut(11, 26, str);

	// ���ĳ���ɫ����
	pDC->SelectObject(pPenGreen);	

	// ���ƴ���������
	pDC->MoveTo(m_iLowGray + 10, 25);
	pDC->LineTo(m_iLowGray + 10, 280);

	pDC->MoveTo(m_iUpGray + 10, 25);
	pDC->LineTo(m_iUpGray + 10, 280);

	// ���ĳ���ɫ����
	pDC->SelectObject(pPenBlue);	

	// �ж��Ƿ��м���
	if (lMaxCount > 0)
	{
		// ����ֱ��ͼ
		for (i = m_iLowGray; i <= m_iUpGray; i ++)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 281 - (int) (m_lCount[i] * 256 / lMaxCount));
		}
	}

	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	

	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
}

void CIntensityDlg::OnEnKillfocusEditLowgray()
{
	// ����䶯���ؼ�����ֵ���浽��Ա�����У�
	UpdateData(TRUE);


	// �ж��Ƿ����޳�������
	if (m_iLowGray > m_iUpGray)
	{
		// ����
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;

		// ���£���Ա��������ֵ���浽�ؼ��У�
		UpdateData(FALSE);
	}

	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);
}

void CIntensityDlg::OnEnKillfocusEditUpgray()
{
	// ����䶯
	UpdateData(TRUE);

	// �ж��Ƿ����޳�������
	if (m_iLowGray > m_iUpGray)
	{
		// ����
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;

		// ����
		UpdateData(FALSE);
	}

	// �ػ�ֱ��ͼ
	InvalidateRect(m_MouseRect, TRUE);
}

void CIntensityDlg::OnBnClickedOk()
{
	// �ж��Ƿ����޳�������
	if (m_iLowGray > m_iUpGray)
	{
		// ����
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
	}

	// ����
	//CDialog::OnOK();
}

void CIntensityDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// ���û�������������ʼ�϶�

	// �ж��Ƿ��ڽ�������¼�����Ч������
	if(m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_iLowGray))
		{

			// �����϶�״̬1���϶�����
			m_iIsDraging = 1;

			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		else if (point.x == (m_MouseRect.left + m_iUpGray))
		{

			// �����϶�״̬Ϊ2���϶�����
			m_iIsDraging = 2;

			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}

	// Ĭ�ϵ��������������¼�
	CDialog::OnLButtonDown(nFlags, point);
}

void CIntensityDlg::OnMouseMove(UINT nFlags, CPoint point) 
{

	// �ж��Ƿ��ڽ�������¼�����Ч������
	if(m_MouseRect.PtInRect(point))
	{
		// �ж��Ƿ������϶�
		if (m_iIsDraging != 0)
		{
			// �ж������϶����޻�������
			if (m_iIsDraging == 1)
			{
				// �ж��Ƿ�����<����
				if (point.x - m_MouseRect.left < m_iUpGray)
				{
					// ��������
					m_iLowGray = point.x - m_MouseRect.left;
				}
				else
				{
					// �����Ϲ����ޣ�����Ϊ����-1
					m_iLowGray = m_iUpGray - 1;

					// �������λ��
					point.x = m_MouseRect.left + m_iUpGray - 1;
				}
			}
			else
			{
				// �����϶�����

				// �ж��Ƿ�����>����
				if (point.x - m_MouseRect.left > m_iLowGray)
				{
					// ��������
					m_iUpGray = point.x - m_MouseRect.left;
				}
				else
				{
					// �����Ϲ����ޣ�����Ϊ���ޣ�1
					m_iUpGray = m_iLowGray + 1;

					// �������λ��
					point.x = m_MouseRect.left + m_iLowGray + 1;
				}
			}

			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));

			// ����
			UpdateData(FALSE);

			// �ػ�ֱ��ͼ
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_iLowGray) || point.x == (m_MouseRect.left + m_iUpGray))
		{
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}

	// Ĭ������ƶ������¼�
	CDialog::OnMouseMove(nFlags, point);
}

void CIntensityDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// ���û��ͷ�������ֹͣ�϶�
	if (m_iIsDraging != 0)
	{
		// �����϶�״̬
		m_iIsDraging = 0;
	}

	// Ĭ���ͷ������������¼�
	CDialog::OnLButtonUp(nFlags, point);
}
