/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// PPageIdentify.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "PPageIdentify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPageIdentify property page

IMPLEMENT_DYNCREATE(CPPageIdentify, CPropertyPage)

CPPageIdentify::CPPageIdentify() : CPropertyPage(CPPageIdentify::IDD)
{
	//{{AFX_DATA_INIT(CPPageIdentify)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPPageIdentify::~CPPageIdentify()
{
}

void CPPageIdentify::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPageIdentify)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPageIdentify, CPropertyPage)
	//{{AFX_MSG_MAP(CPPageIdentify)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPageIdentify message handlers
