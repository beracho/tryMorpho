/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

// MSO_Demo.h : main header file for the MSO_DEMO application
//

#if !defined(AFX_MSO_DEMO_H__A0D6E416_8A0F_4FA3_A514_3AA6B400F145__INCLUDED_)
#define AFX_MSO_DEMO_H__A0D6E416_8A0F_4FA3_A514_3AA6B400F145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "MORPHO_RegisterKeyDef.h"

#define APP		((CMSO_DemoApp*)AfxGetApp()) 

/////////////////////////////////////////////////////////////////////////////
#define TEMPLATE_BUFFER_MAX_SIZE	5120 // 5ko : the new format PK_FVP_MATCH can be huge
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#define STR_FIRST_FINGER			"First finger"
#define STR_SECOND_FINGER			"Second finger"

#define STR_PKCOMP					"SAGEM PkComp"
#define STR_PKCOMPNORM				"SAGEM PkComp Norm"
#define STR_PKMAT					"SAGEM PkMat"
#define STR_PKMATNORM				"SAGEM PkMat Norm"
#define STR_PKLITE					"SAGEM PkLite"
#define STR_ANSI378					"ANSI INCITS 378"
#define STR_ANSI378_2009			"ANSI INCITS 378-2009"
#define STR_ISO_FMC_NS				"ISO 19794-2, FMC Normal Size"
#define STR_ISO_FMC_CS				"ISO 19794-2, FMC Compact Size"
#define STR_ISO_FMR					"ISO 19794-2"
#define STR_ISO_FMR_2011			"ISO 19794-2-2011"
#define STR_ILO_FMR					"ILO International Labour Organisation"
#define STR_MINEX_A					"MINEX A"
#define STR_PKS						"SAGEM Pks"
#define STR_PK_MOC					"SAGEM PKMOC"
#define STR_PK_DIN_V66400_CS		"DIN V66400 Compact Size"
#define STR_PK_DIN_V66400_CS_AA		"DIN V66400 Compact Size, ordered by Ascending Angle"
#define STR_ISO_FMC_CS_AA			"ISO 19794-2, FMC Compact Size, ordered by Ascending Angle"
#define STR_NO_PK_FP                "NO PK FP"
#define STR_PK_FVP                  "SAGEM PkFVP"
#define STR_NO_FVP                  "NO PK FVP"
//New format PK_FVP_MATCH
#define STR_PK_FVP_MATCH           "SAGEM PkFVP Match"

#define STR_NO_IMAGE                "NO IMAGE"
#define STR_ISO19794_FIR_RAW		"ISO-19794-4 FIR RAW"
#define STR_ISO19794_FIR_BMP		"ISO-19794-4 FIR BMP"
#define STR_ISO19794_FIR_WSQ		"ISO-19794-4 FIR WSQ"
#define STR_ISO19794_FIR_JPEG		"ISO-19794-4 FIR JPEG"
#define STR_ISO19794_FIR_JPEG2000	"ISO-19794-4 FIR JPEG 2000"
#define STR_ISO19794_FIR_PNG		"ISO-19794-4 FIR PNG"


/////////////////////////////////////////////////////////////////////////////
#define S1							"Sagem ypsID S1"
#define morphoS2					"Morpho ypsID S2 (div.)"
#define morphoS3					"Morpho ypsID S3"
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CreateEmbeddedPropertySheet
BOOL CreateEmbeddedPropertySheet(	CWnd* pParent, CPropertySheet* pPSheet,
									DWORD dwStyle = WS_CHILD | WS_VISIBLE, DWORD dwExStyle = 0 );

/////////////////////////////////////////////////////////////////////////////
// PositionEmbeddedPropertySheet
void PositionEmbeddedPropertySheet(	CWnd* pParent, CPropertySheet* pPSheet,
										CRect rcNewPosition );

/////////////////////////////////////////////////////////////////////////////
// PositionEmbeddedPropertySheet
void PositionEmbeddedPropertySheet(	CWnd* pParent, CPropertySheet* pPSheet,
										UINT nIDPSheetArea );


/////////////////////////////////////////////////////////////////////////////
#define STR_WORKFLOWTYPE_SAGEM_OPUCE		"SAGEM Opuce Demo"
#define STR_WORKFLOWTYPE_CLIENT_GB			"Client GB"
/////////////////////////////////////////////////////////////////////////////

// CMSO_DemoApp:
// See MSO_Demo.cpp for the implementation of this class
//

class CMSO_DemoApp : public CWinApp
{
public:
	CMSO_DemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSO_DemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMSO_DemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSO_DEMO_H__A0D6E416_8A0F_4FA3_A514_3AA6B400F145__INCLUDED_)
