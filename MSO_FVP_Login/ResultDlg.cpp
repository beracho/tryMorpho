/************************************************************
  Copyright (c) 2002, Morpho
************************************************************/

// ResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_FVP_Login.h"
#include "ResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultDlg dialog

CResultDlg::CResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResultDlg)
	m_cs_DetailedMsg = _T("");
	m_cs_MainMsg = _T("");
	m_cs_StaticCounter = _T("");
	//}}AFX_DATA_INIT

	m_b_YesNoButton = FALSE;
	m_lpctstr_DefaultIcon = IDI_HAND;
}


void CResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResultDlg)
	DDX_Control(pDX, IDC_STATIC_COUNTER, m_ctl_StaticCounter);
	DDX_Control(pDX, IDC_STATIC_ICON, m_ctl_ResDlgIcon);
	DDX_Control(pDX, IDC_STATIC_MAIN_MSG, m_ctl_MainMsg);
	DDX_Control(pDX, IDC_STATIC_DETAILED_MSG, m_ctl_DetailedMsg);
	DDX_Control(pDX, IDC_STATIC_QUALITYSCORE_MSG, m_ctl_QualityScore);
	DDX_Text(pDX, IDC_STATIC_DETAILED_MSG, m_cs_DetailedMsg);
	DDX_Text(pDX, IDC_STATIC_MAIN_MSG, m_cs_MainMsg);
	DDX_Text(pDX, IDC_STATIC_COUNTER, m_cs_StaticCounter);
	DDX_Text(pDX, IDC_STATIC_QUALITYSCORE_MSG, m_cs_QualityScore);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CResultDlg, CDialog)
	//{{AFX_MSG_MAP(CResultDlg)
	ON_MESSAGE(WM_UPDATE_COUNTER,OnUpdateCounter)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResultDlg message handlers

BOOL CResultDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Fonts

	CRect	rRect;
	int	iHeight, iWidth;

	m_ctl_MainMsg.GetWindowRect(&rRect);

	iHeight = rRect.top - rRect.bottom;
	iWidth =  rRect.right - rRect.left;

	if(iHeight < 0) iHeight = 0 - iHeight;
	if(iWidth < 0) iWidth = 0 - iWidth;

	m_fFontBig.Detach();
	m_fFontBig.CreateFont((iHeight - 5),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"");

	m_fFontSmall.Detach();
	m_fFontSmall.CreateFont((iHeight - 15),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"");

	m_ctl_MainMsg.SetFont(&m_fFontBig);
	m_ctl_DetailedMsg.SetFont(&m_fFontSmall);
	m_ctl_StaticCounter.SetFont(&m_fFontSmall);
	m_ctl_QualityScore.SetFont(&m_fFontSmall);
	
	// Buttons & Icon

	if(m_b_YesNoButton)
	{
		GetDlgItem(IDOK)->SetWindowText("Yes");
		GetDlgItem(IDCANCEL)->SetWindowText("No");
		m_ctl_ResDlgIcon.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadStandardIcon(IDI_QUESTION) );
	}
	else
	{
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		m_ctl_ResDlgIcon.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadStandardIcon(m_lpctstr_DefaultIcon) );
	}


	m_nEventTimer = (UINT)SetTimer(0,1000,NULL);
	m_i_Counter = COUNTER;
	m_cs_StaticCounter.Format("%d",m_i_Counter--);
	OnUpdateCounter(0,0);

	//On regle les couleurs
	m_brush.CreateSolidBrush(RGB(5, 5, 5));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResultDlg::ActivateYesNoButtons()
{
	m_b_YesNoButton = TRUE;
}

void CResultDlg::DeactivateYesNoButtons()
{
	m_b_YesNoButton = FALSE;
}

LRESULT CResultDlg::OnUpdateCounter(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);
	return 0L;
}

void CResultDlg::OnOK() 
{
	KillTimer(m_nEventTimer);
	CDialog::OnOK();
}

void CResultDlg::OnCancel() 
{
	KillTimer(m_nEventTimer);
	CDialog::OnCancel();
}

void CResultDlg::Exit() 
{
	CDialog::OnCancel();
}

void CResultDlg::ChangeIcon(LPCTSTR lpszIconName) 
{
	m_lpctstr_DefaultIcon = lpszIconName;	
}

void CResultDlg::OnTimer(UINT_PTR nIDEvent) 
{
	
	m_cs_StaticCounter.Format("%d",m_i_Counter--);
	OnUpdateCounter(0,0);

	if (m_i_Counter < 0)
	{
		Exit();
	}
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CResultDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//// TODO:  Modifier ici les attributs du DC

	//// TODO:  Retourner un autre pinceau si le pinceau par défaut n'est pas souhaité
	//return hbr;

	pDC->SetTextColor(RGB(255,255,255));
	pDC->SetBkColor(RGB(4,4,4));
	

	return m_brush;
}
