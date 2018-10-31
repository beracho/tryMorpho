/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

#if !defined(AFX_RESULTDLG_H__3E3D300A_1D66_4522_84CE_7262C6270442__INCLUDED_)
#define AFX_RESULTDLG_H__3E3D300A_1D66_4522_84CE_7262C6270442__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResultDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResultDlg dialog

class CResultDlg : public CDialog
{
// Construction
public:
	CResultDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CResultDlg)
	enum { IDD = IDD_MSO_DISPLAY_RESULT };
	CStatic	m_ctl_Msg5;
	CStatic	m_ctl_Msg4;
	CStatic	m_ctl_Msg3;
	CStatic	m_ctl_Msg2;
	CStatic	m_ctl_Msg1;
	CString	m_cs_Msg1;
	CString	m_cs_Msg2;
	CString	m_cs_Msg3;
	CString	m_cs_Msg4;
	CString	m_cs_Msg5;
	//}}AFX_DATA
	void InitializeMsg(void);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResultDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CFont	m_fFontBig, m_fFontSmall;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTDLG_H__3E3D300A_1D66_4522_84CE_7262C6270442__INCLUDED_)
