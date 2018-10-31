/************************************************************
  Copyright (c) 2002, Morpho
************************************************************/

// ConnectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_FVP_Login.h"
#include "Morpho_Interface.h"
#include "ConnectDlg.h"
#include "InfoVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg dialog


CConnectDlg::CConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectDlg)
	m_ul_Timeout = DEFAULT_TIMEOUT;
	//}}AFX_DATA_INIT
}


void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectDlg)
	DDX_Control(pDX, IDC_SERIAL_NAME, m_ctrl_SerialName);
	DDX_Control(pDX, IDC_PORT_COM, m_ctlPortCom);
	DDX_Control(pDX, IDC_BAUD_RATE, m_ctlBaudRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_RS232, OnRs232)
	ON_BN_CLICKED(IDC_RADIO_USB, OnUsb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL VFillGradientRect(CDC* pDC, CRect rect, COLORREF ColorBas, COLORREF ColorHaut)
{
	int			i;
	int			nFrame;
	int			vRR, vRRh, vRRtt;
	int			vGG, vGGh, vGGtt;
	int			vBB, vBBh, vBBtt;
	CRect		PaintRect;
	CBrush		PaintBrush;
	CBrush*		pOldBrush;

	if (rect.Height() < 64)		
		nFrame = 32;
	else
		nFrame = 64;

	PaintRect.CopyRect( &rect);
	vRRh = GetRValue( ColorHaut);
	vGGh = GetGValue( ColorHaut);
	vBBh = GetBValue( ColorHaut);
	vRRtt = GetRValue( ColorBas) - vRRh;
	vGGtt = GetGValue( ColorBas) - vGGh;
	vBBtt = GetBValue( ColorBas) - vBBh;
	// First Etape
	PaintRect.bottom = rect.top + MulDiv( rect.Height(), 1, nFrame);
	PaintBrush.CreateSolidBrush( ColorHaut);
	pOldBrush = pDC->SelectObject( &PaintBrush);
	pDC->FillRect( &PaintRect, &PaintBrush);
	pDC->SelectObject( pOldBrush);
	PaintBrush.DeleteObject();
	// Step
	for (i = 1;i < nFrame - 1; i++)
	{
		PaintRect.top = rect.top + MulDiv( rect.Height(), i, nFrame);
		PaintRect.bottom = rect.top + MulDiv( rect.Height(), i + 1, nFrame);
		vRR = vRRh + MulDiv( vRRtt, i, nFrame);
		vGG = vGGh + MulDiv( vGGtt, i, nFrame);
		vBB = vBBh + MulDiv( vBBtt, i, nFrame);
		PaintBrush.CreateSolidBrush( RGB( vRR, vGG, vBB));
		pOldBrush = pDC->SelectObject( &PaintBrush);
		pDC->FillRect( &PaintRect, &PaintBrush);
		pDC->SelectObject( pOldBrush);
		PaintBrush.DeleteObject();
	}
	// Last Step
	PaintRect.top = rect.top + MulDiv( rect.Height(), nFrame - 1, nFrame);
	PaintRect.bottom = rect.bottom;
	PaintBrush.CreateSolidBrush( ColorBas);
	pOldBrush = pDC->SelectObject( &PaintBrush);
	pDC->FillRect( &PaintRect, &PaintBrush);
	pDC->SelectObject( pOldBrush);
	PaintBrush.DeleteObject();
	return TRUE;
}

void DrawVerticalTitle( CWnd* pWnd, CDC* pDC, CString titre, BOOL bDevMode) 
{
	CFont		font, *OldFont;
	CBrush		brush, *OldBrush;
	CPen		pen, *OldPen;
	CRect		rect,Rect2;
	int			OldBkMode;
	COLORREF	gris;

	pWnd->GetClientRect( &rect);

	// Create drawing tools
	gris = RGB(128, 128, 128);
	font.CreateFont(20, 0, 900, 0, FW_EXTRABOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, "Arial");
	brush.CreateSolidBrush( gris);
	pen.CreatePen( PS_SOLID, 0, gris);

	OldBrush = (CBrush*) pDC->SelectObject( &brush);
	OldPen = (CPen*) pDC->SelectObject( &pen);
	OldFont = (CFont*) pDC->SelectObject( &font);

	if (bDevMode)
	{
		Rect2 = CRect::CRect( 0, 1, 26, rect.bottom);
		VFillGradientRect( pDC, Rect2, RGB( 0, 128, 128),  RGB( 128, 0, 128));
	}
	else
	{
		Rect2 = CRect::CRect( 0, 1, 26, rect.bottom);
		VFillGradientRect( pDC, Rect2, RGB( 0, 0, 255),  RGB( 48, 48, 48));
	}

	OldBkMode = pDC->SetBkMode( TRANSPARENT);
	pDC->SetBkColor(gris);
	pDC->SetTextColor(RGB(255, 255, 255)); // blanc
	pDC->TextOut(2, rect.bottom-7, titre);

	// RAZ du contexte
	pDC->SetBkMode( OldBkMode);
	pDC->SelectObject(OldFont);
	pDC->SelectObject(OldBrush);
	pDC->SelectObject(OldPen);
}

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg message handlers

void CConnectDlg::OnRs232() 
{
	UpdateData(TRUE);
	m_strInterface = COM_RS232;	
	DisplayCom();	
}

void CConnectDlg::OnUsb() 
{
	UpdateData(TRUE);
	m_strInterface = COM_USB;
	DisplayCom();
}

void CConnectDlg::DisplayCom(void)
{
	if (strcmp(m_strInterface, COM_RS232) == 0)
	{
		this->CheckRadioButton( IDC_RADIO_RS232,IDC_RADIO_USB,IDC_RADIO_RS232 );
		GetDlgItem(IDC_STATIC_COM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PORT_COM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BAUD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BAUD_RATE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SERIAL_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SERIALNAME)->ShowWindow(SW_HIDE);
	}
	else 
	{
		InitSerialName();
		this->CheckRadioButton( IDC_RADIO_RS232,IDC_RADIO_USB,IDC_RADIO_USB );
		GetDlgItem(IDC_STATIC_COM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PORT_COM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_BAUD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BAUD_RATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SERIAL_NAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SERIALNAME)->ShowWindow(SW_SHOW);
	}		
}

void CConnectDlg::OnOK() 
{
	UL	l_ul_Count;
	PC	l_pc_SerialName;

	// Save new parameters
	APP->WriteProfileString(COM_PARAMETERS, COM_INTERFACE, m_strInterface);
	if(m_strInterface == COM_USB)
	{
		l_ul_Count = m_ctrl_SerialName.GetCurSel();
		l_pc_SerialName = (PC)m_ctrl_SerialName.GetItemDataPtr(l_ul_Count);
		if ( l_pc_SerialName != (PC)-1 )
		{
			APP->WriteProfileString( COM_PARAMETERS, SERIALNUMBER, l_pc_SerialName);
			APP->WriteProfileInt( COM_PARAMETERS, TIMEOUT, m_ul_Timeout);
		}
		else
		{	
			APP->WriteProfileString( COM_PARAMETERS, SERIALNUMBER, "");
			APP->WriteProfileInt( COM_PARAMETERS, TIMEOUT, m_ul_Timeout);
		}
	}
	else
	{
		APP->WriteProfileInt( COM_PARAMETERS, PORT_NUMBER, (int)m_ctlPortCom.GetItemData(m_ctlPortCom.GetCurSel()) );
		APP->WriteProfileInt( COM_PARAMETERS, BAUD_RATE, (int)m_ctlBaudRate.GetItemData(m_ctlBaudRate.GetCurSel()) );
	}	
	CDialog::OnOK();
}

BOOL CConnectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitInterface();
	InitCom();
	InitBaudRate();

	DisplayCom();

	SetTitleBar ( m_hWnd );

	UpdateData(FALSE);	
	return TRUE;  
}

void CConnectDlg::InitInterface()
{
	m_strInterface = APP->GetProfileString(COM_PARAMETERS, COM_INTERFACE,DEFAULT_COM_INTERFACE);
	if (strcmp(m_strInterface, COM_RS232) == 0)
		this->CheckRadioButton( IDC_RADIO_RS232,IDC_RADIO_USB, IDC_RADIO_RS232 );
	else
		this->CheckRadioButton( IDC_RADIO_RS232,IDC_RADIO_USB, IDC_RADIO_USB );
}

void CConnectDlg::InitCom()
{
	int		i, idx;
	CString	str;

	m_ctlPortCom.ResetContent();
	for (i=1; i<10; i++)
	{
		str.Format("COM%d", i);
		{
			idx = m_ctlPortCom.AddString(str);
			m_ctlPortCom.SetItemData(idx, i);
		}
	}

	str.Format("COM%d", APP->GetProfileInt( COM_PARAMETERS, PORT_NUMBER, DEFAULT_COM ) );
	m_ctlPortCom.SelectString(-1, str);
}

void CConnectDlg::InitBaudRate()
{
	int		rates[]={50,75,100,110,150,200,220,300,400,440,600,880,1200,1440,2000,2400,3600,4000,4800,7200,8000,9600,14400,16000,19200,28800,38400,56000,57600,76800,115200,153600,230400,460800,921600};
	int		idx, i; 
	CString	str;

	m_ctlBaudRate.ResetContent();
	for (i=0; i<sizeof(rates)/sizeof(int); i++)
	{
		str.Format("%d", rates[i]);
		idx = m_ctlBaudRate.AddString(str);
		m_ctlBaudRate.SetItemData(idx, rates[i]);
	}
	str.Format("%d", APP->GetProfileInt( COM_PARAMETERS, BAUD_RATE,  DEFAULT_BAUD_RATE) );
	m_ctlBaudRate.SelectString(-1, str);
}

void CConnectDlg::InitSerialName()
{
	int				idx; 
	UL				l_ul_Count;
	CString			str;
	UL				l_ul_NbUsbDevice;
	PC				l_pc_UsbDeviceName;
	PC				l_pc_UsbDeviceProperties;
	I				l_i_Index;

	m_x_MsoDevice.InitUsbDevicesNameEnum(&l_ul_NbUsbDevice);

	m_ctrl_SerialName.ResetContent();
	for (l_ul_Count=0; l_ul_Count<l_ul_NbUsbDevice; l_ul_Count++)
	{
		m_x_MsoDevice.GetUsbDevicesNameEnum(l_ul_Count,l_pc_UsbDeviceName,l_pc_UsbDeviceProperties);

		idx = m_ctrl_SerialName.AddString(l_pc_UsbDeviceName);

		m_ctrl_SerialName.SetItemDataPtr(idx,l_pc_UsbDeviceName);
	}

	str.Format("%s", APP->GetProfileString( COM_PARAMETERS, SERIALNUMBER, "" ) );
	l_i_Index = m_ctrl_SerialName.SelectString(-1, str);
	if ( l_i_Index == -1 )
		m_ctrl_SerialName.SetCurSel ( 0 );
}

void CConnectDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Draw vertical title
	DrawVerticalTitle(this, &dc, "Connection", FALSE);
}

