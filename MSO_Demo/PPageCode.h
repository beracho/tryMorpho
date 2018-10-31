/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

#include "afxwin.h"
#if !defined(AFX_PPAGECODE_H__A0347DC7_DD4F_4BD9_B492_6B88538D510C__INCLUDED_)
#define AFX_PPAGECODE_H__A0347DC7_DD4F_4BD9_B492_6B88538D510C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPageCode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPageCode dialog

class CPPageCode : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPageCode)

// Construction
public:
	CPPageCode();
	~CPPageCode();
	void EnableDisableSmartCardControls(BOOL i_b_EnableOrDisable);

// Dialog Data
	//{{AFX_DATA(CPPageCode)
	enum { IDD = IDD_PPAGE_CODE };
	CComboBox	m_ctrl_WorkflowType;
	CString	m_cs_WorkflowType;
	CButton	m_Ctrl_TokenBio;
	CComboBox	m_ctrl_TemplateType;
	CButton	m_ctrl_LatentDetect;
	CString	m_cs_Firstname;
	CString	m_cs_ID;
	CString	m_cs_Lastname;
	int		m_i_NbFingers;
	int		m_i_OutputChoice;
	int		m_i_CaptureType;
	BOOL	m_b_LatentDetect;
	CString	m_cs_TemplateType;
	BOOL	m_b_TokenBio;
	bool	m_bl_FVP;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPageCode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPageCode)
	afx_msg void OnRadioCaptureVerif();
	afx_msg void OnRadioCaptureEnroll();
	afx_msg void OnSelchangeOutputFileType();
	afx_msg void OnRadioCodeOutputsmc();
	afx_msg void OnRadioCodeOutputfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_ctrl_FVP_TemplateType;
	CString m_cs_FVP_TemplateType;
	afx_msg void OnCbnSelchangeOutputFvpFileType();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPAGECODE_H__A0347DC7_DD4F_4BD9_B492_6B88538D510C__INCLUDED_)
