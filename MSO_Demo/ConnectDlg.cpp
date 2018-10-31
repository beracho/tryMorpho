/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// ConnectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mso_demo.h"
#include "Morpho_Interface.h"
#include "ConnectDlg.h"
#include "DrawTitle.h"
#include "InfoVersion.h"

#include "Winsock2.h"
#include "Sp_ClientPipe.h"
#include "Sp_Pipe_Functions.h"
#include "Pipe_Tcp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CConnectPPageSerialDlg property page

IMPLEMENT_DYNCREATE(CConnectPPageSerialDlg, CPropertyPage)

CConnectPPageSerialDlg::CConnectPPageSerialDlg() : CPropertyPage(CConnectPPageSerialDlg::IDD)
{
	m_psp.pszTitle = COM_RS232;
	m_psp.dwFlags  |= PSP_USETITLE;

	//{{AFX_DATA_INIT(CConnectPPageSerialDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CConnectPPageSerialDlg::~CConnectPPageSerialDlg()
{
}

void CConnectPPageSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectPPageSerialDlg)
	DDX_Control(pDX, IDC_PORT_COM, m_ctlPortCom);
	DDX_Control(pDX, IDC_BAUD_RATE, m_ctlBaudRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectPPageSerialDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CConnectPPageSerialDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectPPageSerialDlg message handlers

BOOL CConnectPPageSerialDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	InitCom();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConnectPPageSerialDlg::InitCom()
{
	int		i, idx;
	CString	str;
	int		rates[]={50,75,100,110,150,200,220,300,400,440,600,880,1200,1440,2000,2400,3600,4000,4800,7200,8000,9600,14400,16000,19200,28800,38400,56000,57600,76800,115200,153600,230400,460800,921600};

	//HANDLE	hCom;

	m_ctlPortCom.ResetContent();
	for (i = 1; i <= 256; i++)
	{
		str.Format("COM%d", i);
		//hCom = CreateFile(str, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		//if (hCom != INVALID_HANDLE_VALUE)
		//{
			//idx = m_ctlPortCom.AddString(str);
			idx = m_ctlPortCom.InsertString((i - 1), str);
			m_ctlPortCom.SetItemData(idx, i);
		//	CloseHandle(hCom);
		//}
	}

	str.Format("COM%d", APP->GetProfileInt( COM_PARAMETERS, PORT_NUMBER, DEFAULT_COM ) );
	m_ctlPortCom.SelectString(-1, str);

	m_ctlBaudRate.ResetContent();
	for (i=0; i<sizeof(rates)/sizeof(int); i++)
	{
		str.Format("%d", rates[i]);
		idx = m_ctlBaudRate.AddString(str);
		m_ctlBaudRate.SetItemData(idx, rates[i]);
	}
	str.Format("%d", APP->GetProfileInt( COM_PARAMETERS, BAUD_RATE,  DEFAULT_BAUD_RATE) );
	m_ctlBaudRate.SelectString(-1, str);

	UpdateData ( FALSE );
}


void CConnectPPageSerialDlg::OnOK()
{
	UpdateData ( TRUE );

	APP->WriteProfileInt( COM_PARAMETERS, PORT_NUMBER, (int)m_ctlPortCom.GetItemData(m_ctlPortCom.GetCurSel()) );
	APP->WriteProfileInt( COM_PARAMETERS, BAUD_RATE, (int)m_ctlBaudRate.GetItemData(m_ctlBaudRate.GetCurSel()) );
	
	int index = m_ctlPortCom.GetItemData(m_ctlPortCom.GetCurSel())-1;
	CString test;
	m_ctlPortCom.GetLBText(index, test);
	APP->WriteProfileString(COM_PARAMETERS, PORT_NAME, test);

}











/////////////////////////////////////////////////////////////////////////////
// CConnectPPageUsbDlg property page

IMPLEMENT_DYNCREATE(CConnectPPageUsbDlg, CPropertyPage)

CConnectPPageUsbDlg::CConnectPPageUsbDlg() : CPropertyPage(CConnectPPageUsbDlg::IDD)
{
	m_psp.pszTitle = COM_USB;
	m_psp.dwFlags  |= PSP_USETITLE;

	//{{AFX_DATA_INIT(CConnectPPageUsbDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CConnectPPageUsbDlg::~CConnectPPageUsbDlg()
{
}

void CConnectPPageUsbDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectPPageUsbDlg)
	DDX_Control(pDX, IDC_SERIAL_NAME, m_ctrl_SerialName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectPPageUsbDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CConnectPPageUsbDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectPPageUsbDlg message handlers

void CConnectPPageUsbDlg::InitCom()
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

	UpdateData ( FALSE );
}

BOOL CConnectPPageUsbDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	InitCom();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConnectPPageUsbDlg::OnOK()
{
	UL	l_ul_Count;
	PC	l_pc_SerialName;

	UpdateData ( TRUE );

	l_ul_Count = m_ctrl_SerialName.GetCurSel();
	l_pc_SerialName = (PC)m_ctrl_SerialName.GetItemDataPtr(l_ul_Count);

	if ( l_pc_SerialName != (PC)-1 )
		APP->WriteProfileString( COM_PARAMETERS, SERIALNUMBER, l_pc_SerialName);
	else
		APP->WriteProfileString( COM_PARAMETERS, SERIALNUMBER, "");
}



















/////////////////////////////////////////////////////////////////////////////
// CConnectPPageSlavePipedlg property page

IMPLEMENT_DYNCREATE(CConnectPPageSlavePipeDlg, CPropertyPage)

CConnectPPageSlavePipeDlg::CConnectPPageSlavePipeDlg() : CPropertyPage(CConnectPPageSlavePipeDlg::IDD)
{
	m_psp.pszTitle = COM_DEMO_PIPE;
	m_psp.dwFlags  |= PSP_USETITLE;

	//{{AFX_DATA_INIT(CConnectPPageSlavePipeDlg)
	m_i_IpPort = 0;
	m_cs_AddressName = _T("");
	m_i_IpFormat = -1;
	//}}AFX_DATA_INIT
}

CConnectPPageSlavePipeDlg::~CConnectPPageSlavePipeDlg()
{
}

void CConnectPPageSlavePipeDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectPPageSlavePipeDlg)
	DDX_Control(pDX, IDC_SERIAL_NAME, m_ctrl_SerialName);
	DDX_Control(pDX, IDC_ADDRESS_NAME, m_ctrl_AdressName);
	DDX_Control(pDX, IDC_IPADDRESS, m_ctrl_IpAddress);
	DDX_Text(pDX, IDC_IPPORT, m_i_IpPort);
	DDX_Text(pDX, IDC_ADDRESS_NAME, m_cs_AddressName);
	DDX_Radio(pDX, IDC_IP_FORMAT, m_i_IpFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectPPageSlavePipeDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CConnectPPageSlavePipeDlg)
	ON_BN_CLICKED(IDC_IP_FORMAT, OnIpFormat)
	ON_BN_CLICKED(IDC_IP_FORMAT2, OnIpFormat)
	ON_BN_CLICKED(IDC_TEST_HOST, OnTestHost)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectPPageSlavePipeDlg message handlers

BOOL CConnectPPageSlavePipeDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	InitCom();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConnectPPageSlavePipeDlg::InitCom()
{
	CString	l_cs_AddressIp;
	int l_b_nField0,l_b_nField1,l_b_nField2,l_b_nField3;

	l_cs_AddressIp = APP->GetProfileString( COM_PARAMETERS, IP_ADDRESS, "127.0.0.1" );
#if (_MSC_VER > 1300)
	if ( sscanf_s(l_cs_AddressIp,"%d.%d.%d.%d",&l_b_nField0,&l_b_nField1,&l_b_nField2,&l_b_nField3) == 4 )
#else
	if ( sscanf(l_cs_AddressIp,"%d.%d.%d.%d",&l_b_nField0,&l_b_nField1,&l_b_nField2,&l_b_nField3) == 4 )
#endif

	{
		m_i_IpFormat = 0;
		m_ctrl_IpAddress.ShowWindow (SW_SHOW);
		m_ctrl_AdressName.ShowWindow (SW_HIDE);
		m_ctrl_IpAddress.SetAddress ( l_b_nField0,l_b_nField1,l_b_nField2,l_b_nField3 );
	}
	else
	{
		m_i_IpFormat = 1;
		m_ctrl_IpAddress.ShowWindow (SW_HIDE);
		m_ctrl_AdressName.ShowWindow (SW_SHOW);
		m_cs_AddressName = l_cs_AddressIp;
	}

	m_i_IpPort = APP->GetProfileInt ( COM_PARAMETERS, IP_PORT, 11011 );

	UpdateData ( FALSE );
}

void CConnectPPageSlavePipeDlg::OnIpFormat() 
{
	UpdateData ( TRUE );

	TCP_Start();

	if ( m_i_IpFormat == 0 )
	{
		struct hostent*		l_px_HostEnt;
		ULONG*				l_pul_Addr;

		l_px_HostEnt   = NULL;
		l_pul_Addr     = NULL;

		l_px_HostEnt = (struct hostent*) gethostbyname ( m_cs_AddressName );
		if ( l_px_HostEnt != NULL )
		{
			l_pul_Addr                     = (ULONG*) l_px_HostEnt->h_addr_list[ 0];
			m_ctrl_IpAddress.SetAddress ( htonl (*l_pul_Addr) );
		}

		m_ctrl_IpAddress.ShowWindow (SW_SHOW);
		m_ctrl_AdressName.ShowWindow (SW_HIDE);
	}
	else
	{
		struct hostent*		l_px_HostEnt;
		CString				l_cs_AddressIp;
		unsigned int		addr;

		BYTE l_b_nField0,l_b_nField1,l_b_nField2,l_b_nField3;

		m_ctrl_IpAddress.GetAddress ( l_b_nField0, l_b_nField1, l_b_nField2, l_b_nField3 );
		l_cs_AddressIp.Format ( "%d.%d.%d.%d", l_b_nField0, l_b_nField1, l_b_nField2, l_b_nField3 );
		addr = inet_addr(l_cs_AddressIp);

		l_px_HostEnt = gethostbyaddr((char *)&addr,4,AF_INET);

		if ( l_px_HostEnt != NULL )
		{
			l_px_HostEnt = gethostbyaddr((char *)&addr,4,AF_INET);
			m_cs_AddressName = l_px_HostEnt->h_name;
		}


		m_ctrl_IpAddress.ShowWindow (SW_HIDE);
		m_ctrl_AdressName.ShowWindow (SW_SHOW);
	}
	
	TCP_Stop();

	UpdateData ( FALSE );
}

void CConnectPPageSlavePipeDlg::OnOK()
{
	BYTE	l_b_nField0,l_b_nField1,l_b_nField2,l_b_nField3;
	PC		l_pc_SerialName;
	UL		l_ul_Count;

	UpdateData ( TRUE );

	if ( m_i_IpFormat == 0 )
	{
		m_ctrl_IpAddress.GetAddress ( l_b_nField0, l_b_nField1, l_b_nField2, l_b_nField3 );
		m_cs_AddressName.Format ( "%d.%d.%d.%d", l_b_nField0, l_b_nField1, l_b_nField2, l_b_nField3 );
	}

	APP->WriteProfileString(COM_PARAMETERS, IP_ADDRESS, m_cs_AddressName);
	APP->WriteProfileInt( COM_PARAMETERS, IP_PORT, m_i_IpPort );

	l_ul_Count = m_ctrl_SerialName.GetCurSel();
	l_pc_SerialName = (PC)m_ctrl_SerialName.GetItemDataPtr(l_ul_Count);

	if ( l_pc_SerialName != (PC)-1 )
		APP->WriteProfileString( COM_PARAMETERS, SERIALNUMBER, l_pc_SerialName);
	else
		APP->WriteProfileString( COM_PARAMETERS, SERIALNUMBER, "");
}

void CConnectPPageSlavePipeDlg::OnTestHost() 
{
	SOCKET			l_x_Sk;
	sockaddr_in		l_x_Adress;
	int				l_i_Ret;
	// Correction fait par C++Test [BD-PB-NOTINIT-1]
	char			l_c_Tag=0;
	UL				l_ul_Count;
	C				l_c_NbUsbDevice;
	PC				l_pc_SerialNumber = NULL;
	UL				l_ul_SerialNumberLen;
	I				l_i_LenReceived;
	int				idx;

	OnOK();

	//	Enable TCP functionnalities
	TCP_Start ( TCP_LOG_ON );

	// Establishes a connection to a Server, specified by its Address, and it's Port Number
	l_i_Ret = TCP_Client_Connect ( (char *)(LPCTSTR)m_cs_AddressName, m_i_IpPort, &l_x_Sk, &l_x_Adress );

	if ( l_i_Ret == 0 )
	{
		l_c_Tag = SP_PIPE_TAG_MSO_LIST;
		l_i_Ret = TCP_Send ( l_x_Sk, &l_c_Tag, sizeof (char) );
	}

	if ( l_i_Ret == 0 )
	{
		l_i_Ret = TCP_Recv ( l_x_Sk, &l_c_Tag, sizeof (l_c_Tag), &l_i_LenReceived, 7000 );
	}

	if ( l_c_Tag == SP_PIPE_TAG_MSO_LIST )
	{
		if ( l_i_Ret == 0 )
		{
			l_i_Ret = Sp_Pipe_Receive_Status ( l_x_Sk );
		}
		else
		{
			l_i_Ret = -1;
		}

		if ( l_i_Ret == 0 )
		{
			l_i_Ret = TCP_Recv ( l_x_Sk, &l_c_NbUsbDevice, sizeof (l_c_NbUsbDevice), &l_i_LenReceived, 7000 );
		}

		if ( l_i_Ret == 0 )
		{
			m_ctrl_SerialName.ResetContent();
			for ( l_ul_Count=0; l_ul_Count < (UC)l_c_NbUsbDevice; l_ul_Count++)
			{
				l_i_Ret = Sp_Pipe_Receive_LenAndDatas ( l_x_Sk, &l_pc_SerialNumber, &l_ul_SerialNumberLen );
				idx = m_ctrl_SerialName.AddString(l_pc_SerialNumber);

				m_ctrl_SerialName.SetItemDataPtr(idx,l_pc_SerialNumber);
			}

			if(l_c_NbUsbDevice > 0)
			{
				idx = m_ctrl_SerialName.SelectString(-1, l_pc_SerialNumber);
				if ( idx == -1 )
					m_ctrl_SerialName.SetCurSel ( 0 );
			}

		}
	}

	// Closes the socket
	l_i_Ret = TCP_Client_Close ( &l_x_Sk );

	// Close the TCP functionnalities
	TCP_Stop();

	UpdateData ( FALSE );

}











/////////////////////////////////////////////////////////////////////////////
// CConnectDlg dialog


CConnectDlg::CConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CConnectDlg message handlers

BOOL CConnectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitInterface();

	SetTitleBar ( m_hWnd );

	UpdateData(FALSE);	
	return TRUE;  
}

void CConnectDlg::InitInterface()
{
	m_x_PSheet.AddPage(&m_x_ConnectPPageSerialDlg);

	m_x_PSheet.AddPage(&m_x_ConnectPPageUsbDlg);

	m_x_PSheet.AddPage(&m_x_ConnectPPageSlavePipeDlg);

	//---------------------------------------------
	// Create Property Sheet
	//---------------------------------------------
	CreateEmbeddedPropertySheet(this, &m_x_PSheet);
	PositionEmbeddedPropertySheet(this, &m_x_PSheet, IDC_PSHEET_AREA);

	m_strInterface = APP->GetProfileString(COM_PARAMETERS, COM_INTERFACE,DEFAULT_COM_INTERFACE);
	if ( strcmp ( m_strInterface, COM_DEMO_PIPE ) == 0 )
	{
		m_x_PSheet.SetActivePage( &(m_x_ConnectPPageSlavePipeDlg) );	
	}
	else if ( strcmp ( m_strInterface, COM_RS232 ) == 0 )
	{
		m_x_PSheet.SetActivePage( &(m_x_ConnectPPageSerialDlg) );	
	}
	else
	{
		m_x_PSheet.SetActivePage( &(m_x_ConnectPPageUsbDlg) );	
	}

}

void CConnectDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Draw vertical title
	CDrawTitle::DrawVerticalTitle(this, &dc, "Connection", FALSE);
}

void CConnectDlg::OnOK() 
{

	CString		l_cs_Message;
	
	CPropertyPage	*l_px_CurPPage;

	l_px_CurPPage = m_x_PSheet.GetActivePage ( );

	// Save new parameters
	APP->WriteProfileString(COM_PARAMETERS, COM_INTERFACE, l_px_CurPPage->m_psp.pszTitle );

	l_px_CurPPage->OnOK();

	CDialog::OnOK();
}

