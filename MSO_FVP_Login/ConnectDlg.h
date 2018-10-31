/************************************************************
  Copyright (c) 2002, Morpho
************************************************************/

#if !defined(AFX_CONNECTDLG_H__CA905E2A_958D_4328_BC35_E80C17DAD19E__INCLUDED_)
#define AFX_CONNECTDLG_H__CA905E2A_958D_4328_BC35_E80C17DAD19E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg dialog

class CConnectDlg : public CDialog
{
// Construction
public:
	CConnectDlg(CWnd* pParent = NULL);   // standard constructor
	void InitInterface();
	void InitCom();
	void InitBaudRate();
	void InitSerialName();

// Dialog Data
	//{{AFX_DATA(CConnectDlg)
	enum { IDD = IDD_CONNECTION };
	CComboBox	m_ctrl_SerialName;
	CComboBox	m_ctlPortCom;
	CComboBox	m_ctlBaudRate;
	DWORD	m_ul_Timeout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString			m_strInterface;
	C_MORPHO_Device	m_x_MsoDevice;

	// Generated message map functions
	//{{AFX_MSG(CConnectDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRs232();
	afx_msg void OnUsb();
	//}}AFX_MSG
	void DisplayCom(void);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTDLG_H__CA905E2A_958D_4328_BC35_E80C17DAD19E__INCLUDED_)
