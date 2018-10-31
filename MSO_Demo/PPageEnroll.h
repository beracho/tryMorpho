/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

#include "afxwin.h"

#if !defined(AFX_PPAGEENROLL_H__03ADBCE1_D7E5_418C_B129_AF04DB99752E__INCLUDED_)
#define AFX_PPAGEENROLL_H__03ADBCE1_D7E5_418C_B129_AF04DB99752E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPageEnroll.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPageEnroll dialog

class CPPageEnroll : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPageEnroll)

// Construction
public:
	CPPageEnroll();
	~CPPageEnroll();

// Dialog Data
	//{{AFX_DATA(CPPageEnroll)
	enum { IDD = IDD_PPAGE_ENROLL };
	CString	m_cs_Firstname;
	CString	m_cs_ID;
	CString	m_cs_Lastname;
	int		m_i_NbFingers;
	bool m_bl_FVP;
	bool UpdateCtr;
	//}}AFX_DATA
	

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPageEnroll)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPageEnroll)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CButton m_ctl_SavePkInDB;
	BOOL m_b_SavePkInDB;
	afx_msg void OnSavePkInBDClickCheck();
	CComboBox m_ctrl_TemplateType;
	CComboBox m_ctrl_FVP_TemplateType;
	CComboBox m_cmd_CompressionType;
	CString m_cs_TemplateType;
	CString m_cs_FVP_TemplateType;
	afx_msg void OnCbnSelchangeCompressionType();
	int		    m_i_CompressionType;
	int         m_i_CompressionRate;
	CString m_str_CompressionType;
	afx_msg void OnCbnSelchangeOutputFileType();
	afx_msg void OnCbnSelchangeOutputFvpFileType();

	BOOL m_b_updateFinger;
	CComboBox m_ctrl_updateFingerIndex;
	CString m_str_selectedUpdateIndex;
	afx_msg void OnBnClickedRadioEnrollOnefinger();
	afx_msg void OnBnClickedRadioEnrollTwofingers();
	afx_msg void OnBnClickedCheckUpdateFinger();
	afx_msg void OnCbnSelchangeFingerIndex();
};





/////////////////////////////////////////////////////////////////////////////
// CUpdateUserDlg dialog

class CUpdateUserDlg : public CDialog
{
// Construction
public:
	CUpdateUserDlg(bool i_b_activateUSerID,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUpdateUserDlg)
	enum { IDD = IDD_UPDATE_USER_DLG };
	CString	m_cs_Firstname;
	CString	m_cs_ID;
	CString	m_cs_Lastname;
	bool m_b_activateUSerID;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpdateUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUpdateUserDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	CEdit m_ctl_ID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPAGEENROLL_H__03ADBCE1_D7E5_418C_B129_AF04DB99752E__INCLUDED_)
