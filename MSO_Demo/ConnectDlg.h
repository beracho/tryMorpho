/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

#if !defined(AFX_CONNECTDLG_H__768118D4_7EDE_4FC9_B5EE_775E8BEDD288__INCLUDED_)
#define AFX_CONNECTDLG_H__768118D4_7EDE_4FC9_B5EE_775E8BEDD288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnectDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CConnectPPageSerialDlg dialog

class CConnectPPageSerialDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CConnectPPageSerialDlg)

// Construction
public:
	CConnectPPageSerialDlg();
	~CConnectPPageSerialDlg();

	void OnOK();

	void InitCom();

// Dialog Data
	//{{AFX_DATA(CConnectPPageSerialDlg)
	enum { IDD = IDD_CONNECTION_PSHEET_SERIAL };
	CComboBox	m_ctlPortCom;
	CComboBox	m_ctlBaudRate;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConnectPPageSerialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConnectPPageSerialDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.













/////////////////////////////////////////////////////////////////////////////
// CConnectPPageUsbDlg dialog

class CConnectPPageUsbDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CConnectPPageUsbDlg)

// Construction
public:
	CConnectPPageUsbDlg();
	~CConnectPPageUsbDlg();

	void OnOK();

	void InitCom();

// Dialog Data
	//{{AFX_DATA(CConnectPPageUsbDlg)
	enum { IDD = IDD_CONNECTION_PSHEET_USB };
	CComboBox	m_ctrl_SerialName;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConnectPPageUsbDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConnectPPageUsbDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	C_MORPHO_Device	m_x_MsoDevice;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.













/////////////////////////////////////////////////////////////////////////////
// CConnectPPageSlavePipeDlg dialog

class CConnectPPageSlavePipeDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CConnectPPageSlavePipeDlg)

// Construction
public:
	CConnectPPageSlavePipeDlg();
	~CConnectPPageSlavePipeDlg();

	void OnOK();

	void InitCom();

// Dialog Data
	//{{AFX_DATA(CConnectPPageSlavePipeDlg)
	enum { IDD = IDD_CONNECTION_PSHEET_SLAVEPIPE };
	CComboBox	m_ctrl_SerialName;
	CEdit	m_ctrl_AdressName;
	CIPAddressCtrl	m_ctrl_IpAddress;
	int		m_i_IpPort;
	CString	m_cs_AddressName;
	int		m_i_IpFormat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConnectPPageSlavePipeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConnectPPageSlavePipeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnIpFormat();
	afx_msg void OnTestHost();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.












/////////////////////////////////////////////////////////////////////////////
// CConnectDlg dialog

class CConnectDlg : public CDialog
{
// Construction
public:
	CConnectDlg(CWnd* pParent = NULL);   // standard constructor
	void InitInterface();

// Dialog Data
	//{{AFX_DATA(CConnectDlg)
	enum { IDD = IDD_CONNECTION };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConnectDlg)
	afx_msg void OnPaint();
	afx_msg void OnRs232();
	afx_msg void OnUsb();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CString		m_strInterface;

	CConnectPPageSerialDlg		m_x_ConnectPPageSerialDlg;
	CConnectPPageUsbDlg			m_x_ConnectPPageUsbDlg;
	CConnectPPageSlavePipeDlg	m_x_ConnectPPageSlavePipeDlg;

	CPropertySheet			m_x_PSheet;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTDLG_H__768118D4_7EDE_4FC9_B5EE_775E8BEDD288__INCLUDED_)
