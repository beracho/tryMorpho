/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// MSO_Demo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "MSO_DemoDlg.h"
#include "ConnectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMSO_DemoApp

BEGIN_MESSAGE_MAP(CMSO_DemoApp, CWinApp)
	//{{AFX_MSG_MAP(CMSO_DemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSO_DemoApp construction

CMSO_DemoApp::CMSO_DemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMSO_DemoApp object

CMSO_DemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMSO_DemoApp initialization

BOOL CMSO_DemoApp::InitInstance()
{
#if (_MSC_VER > 1300)
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();
#else
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

#endif 

	//********************************************
	
		SetRegistryKey("SAGEM\\MSO");

		///////////////////////////////////////////
		// Connection Dialog					
		///////////////////////////////////////////
		
		CConnectDlg ConnectionDlg;
		INT_PTR nResponse = ConnectionDlg.DoModal();
		if (nResponse == IDCANCEL)
		{
			return FALSE;
		}

	//********************************************

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	CMSO_DemoDlg dlg;
	m_pMainWnd = &dlg;
	
	/*int*/ nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	CoUninitialize();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
