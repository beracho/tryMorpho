/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

#if !defined(AFX_PPAGEVERIFY_H__964C27D4_DA33_434E_BD9A_4EB357CED3DB__INCLUDED_)
#define AFX_PPAGEVERIFY_H__964C27D4_DA33_434E_BD9A_4EB357CED3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPageVerify.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPageVerify dialog

class CPPageVerify : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPageVerify)

// Construction
public:
	CPPageVerify();
	~CPPageVerify();
	void EnableDisableSmartCardControls(BOOL i_b_EnableOrDisable);

// Dialog Data
	//{{AFX_DATA(CPPageVerify)
	enum { IDD = IDD_PPAGE_VERIFY };
	CComboBox	m_ctrl_WorkflowType;
	CComboBox	m_ctrl_TemplateType;
	int		m_i_InputChoice;
	int		m_i_NbFingers;
	CString	m_cs_TemplateType;
	CString	m_cs_WorkflowType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPageVerify)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPageVerify)
	afx_msg void OnRadioInputfile();
	afx_msg void OnRadioLocalbase();
	afx_msg void OnRadioInputsmc();
	afx_msg void OnRadioInputsmcInternal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPAGEVERIFY_H__964C27D4_DA33_434E_BD9A_4EB357CED3DB__INCLUDED_)
