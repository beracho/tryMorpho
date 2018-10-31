// ConnectPPageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mso_demo.h"
#include "ConnectPPageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectPPageDlg property page

IMPLEMENT_DYNCREATE(CConnectPPageDlg, CPropertyPage)

CConnectPPageDlg::CConnectPPageDlg() : CPropertyPage(CConnectPPageDlg::IDD)
{
	//{{AFX_DATA_INIT(CConnectPPageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CConnectPPageDlg::~CConnectPPageDlg()
{
}

void CConnectPPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectPPageDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectPPageDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CConnectPPageDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectPPageDlg message handlers
