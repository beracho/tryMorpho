/************************************************************
  Copyright (c) 2002, Morpho 
************************************************************/

// MSO_FVP_Login.h : main header file for the MSO_FVP_Login application
//

#if !defined(AFX_MSO_FVP_Login_H__B607B28F_2FC0_4294_B43F_B02B19BC1B57__INCLUDED_)
#define AFX_MSO_FVP_Login_H__B607B28F_2FC0_4294_B43F_B02B19BC1B57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "MORPHO_RegisterKeyDef.h"

#define APP		((CMSO_FVP_LoginApp*)AfxGetApp()) 

//#define APP_MODE_1

#define	DB_FIELD_MAX_SIZE	15
#define	DB_NB_RECORDS		500
#define	DB_NB_FINGERS		2

/////////////////////////////////////////////////////////////////////////////
// CMSO_FVP_LoginApp:
// See MSO_FVP_Login.cpp for the implementation of this class
//

class CMSO_FVP_LoginApp : public CWinApp
{
public:
	CMSO_FVP_LoginApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSO_FVP_LoginApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMSO_FVP_LoginApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSO_FVP_Login_H__B607B28F_2FC0_4294_B43F_B02B19BC1B57__INCLUDED_)
