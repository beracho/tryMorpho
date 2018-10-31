/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// BioOperationPSheet.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "BioOperationPSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBioOperationPSheet

IMPLEMENT_DYNAMIC(CBioOperationPSheet, CPropertySheet)

CBioOperationPSheet::CBioOperationPSheet(CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet("BioOperationPSheet", pParentWnd, iSelectPage)
{
	DisplayPage(&m_CodePPage);		// First page
	DisplayPage(&m_VerifyPPage );	// Second page
	DisplayPage(&m_GetImagePPage);	// Third page
	DisplayPage(&m_EnrollPPage);	// Fourth page
	DisplayPage(&m_IdentifyPPage);	// Fifth page
	DisplayPage(&m_MocPPage);		// Fifth page

	m_pProcessPPage = NULL;
}

CBioOperationPSheet::CBioOperationPSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CBioOperationPSheet::CBioOperationPSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CBioOperationPSheet::~CBioOperationPSheet()
{
}


BEGIN_MESSAGE_MAP(CBioOperationPSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CBioOperationPSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBioOperationPSheet message handlers
void CBioOperationPSheet::DisplayPage(CPropertyPage *PageToAdd)
{
	AddPage(PageToAdd);
}

void CBioOperationPSheet::HidePage(CPropertyPage *PageToRemove)
{
	RemovePage(PageToRemove);
}

void CBioOperationPSheet::EnableDisableSmartCardControls(BOOL i_b_EnableOrDisable)
{
	m_CodePPage.EnableDisableSmartCardControls(i_b_EnableOrDisable);
	m_VerifyPPage.EnableDisableSmartCardControls(i_b_EnableOrDisable);
}