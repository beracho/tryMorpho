/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

#if !defined(AFX_PPAGEIDENTIFY_H__DDE8D387_456B_4F43_A9F7_E7108AA1DCF5__INCLUDED_)
#define AFX_PPAGEIDENTIFY_H__DDE8D387_456B_4F43_A9F7_E7108AA1DCF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPageIdentify.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPageIdentify dialog

class CPPageIdentify : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPageIdentify)

// Construction
public:
	CPPageIdentify();
	~CPPageIdentify();

// Dialog Data
	//{{AFX_DATA(CPPageIdentify)
	enum { IDD = IDD_PPAGE_IDENTIFY };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPageIdentify)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPageIdentify)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPAGEIDENTIFY_H__DDE8D387_456B_4F43_A9F7_E7108AA1DCF5__INCLUDED_)
