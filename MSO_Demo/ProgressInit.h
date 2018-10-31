/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

// ProgressInit.h : header file
//

#if !defined(AFX_PROGRESSINIT_H__94AC751B_54FD_485F_B187_27C19DFABF22__INCLUDED_)
#define AFX_PROGRESSINIT_H__94AC751B_54FD_485F_B187_27C19DFABF22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CProgressInit dialog

class CProgressInit : public CDialog
{
// Construction
public:
	CProgressInit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressInit)
	enum { IDD = IDD_PROGRESS_INIT };
	CStatic	m_static_BioAvi;
	CString	m_cs_Mess;
	CString	m_cs_Mess2;
	CString	m_cs_Mess3;
	CString	m_cs_Mess4;
	CString	m_cs_Mess5;
	CString	m_cs_Mess10;
	CString	m_cs_Mess11;
	CString	m_cs_Mess12;
	CString	m_cs_Mess13;
	CString	m_cs_Mess14;
	CString	m_cs_MainMess;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressInit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressInit)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSINIT_H__94AC751B_54FD_485F_B187_27C19DFABF22__INCLUDED_)
