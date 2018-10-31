/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

#if !defined(AFX_BIOOPERATIONPSHEET_H__FE077309_7F89_4BA4_8FBD_6541E0FF43C3__INCLUDED_)
#define AFX_BIOOPERATIONPSHEET_H__FE077309_7F89_4BA4_8FBD_6541E0FF43C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BioOperationPSheet.h : header file
//

#include "PPageEnroll.h"
#include "PPageIdentify.h"
#include "PPageCode.h"
#include "PPageVerify.h"
#include "PPageGetImage.h"
#include "PPageProcess.h"
#include "PPageMoc.h"

/////////////////////////////////////////////////////////////////////////////
// CBioOperationPSheet

class CBioOperationPSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CBioOperationPSheet)

// Construction
public:
	CBioOperationPSheet(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CBioOperationPSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CBioOperationPSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	
// Attributes
public:
	CPPageEnroll	m_EnrollPPage;
	CPPageIdentify	m_IdentifyPPage;
	CPPageCode		m_CodePPage;
	CPPageVerify	m_VerifyPPage;
	CPPageGetImage	m_GetImagePPage;
	CPPageProcess*	m_pProcessPPage;
	CPPageMoc		m_MocPPage;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBioOperationPSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	void HidePage(CPropertyPage *PageToRemove);
	void DisplayPage(CPropertyPage *PageToAdd);
	virtual ~CBioOperationPSheet();
	void EnableDisableSmartCardControls(BOOL i_b_EnableOrDisable);

	// Generated message map functions
protected:
	//{{AFX_MSG(CBioOperationPSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BIOOPERATIONPSHEET_H__FE077309_7F89_4BA4_8FBD_6541E0FF43C3__INCLUDED_)
