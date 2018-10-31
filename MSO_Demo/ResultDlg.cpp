/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// ResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_Demo.h"
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
	InitializeMsg();
	//}}AFX_DATA_INIT
}


void CResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResultDlg)
	DDX_Control(pDX, IDC_STATIC_MSG5, m_ctl_Msg5);
	DDX_Control(pDX, IDC_STATIC_MSG4, m_ctl_Msg4);
	DDX_Control(pDX, IDC_STATIC_MSG3, m_ctl_Msg3);
	DDX_Control(pDX, IDC_STATIC_MSG2, m_ctl_Msg2);
	DDX_Control(pDX, IDC_STATIC_MSG1, m_ctl_Msg1);
	DDX_Text(pDX, IDC_STATIC_MSG1, m_cs_Msg1);
	DDX_Text(pDX, IDC_STATIC_MSG2, m_cs_Msg2);
	DDX_Text(pDX, IDC_STATIC_MSG3, m_cs_Msg3);
	DDX_Text(pDX, IDC_STATIC_MSG4, m_cs_Msg4);
	DDX_Text(pDX, IDC_STATIC_MSG5, m_cs_Msg5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResultDlg, CDialog)
	//{{AFX_MSG_MAP(CResultDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResultDlg message handlers

BOOL CResultDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect	rRect;
	int	iHeight, iWidth;

	m_ctl_Msg1.GetWindowRect(&rRect);

	iHeight = rRect.top - rRect.bottom;
	iWidth =  rRect.right - rRect.left;

	if(iHeight < 0) iHeight = 0 - iHeight;
	if(iWidth < 0) iWidth = 0 - iWidth;

	m_fFontBig.Detach();
	m_fFontBig.CreateFont((iHeight - 5),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"");

	m_fFontSmall.Detach();
	m_fFontSmall.CreateFont((iHeight - 20),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"");

	m_ctl_Msg1.SetFont(&m_fFontBig);
	m_ctl_Msg2.SetFont(&m_fFontSmall);
	m_ctl_Msg3.SetFont(&m_fFontSmall);
	m_ctl_Msg4.SetFont(&m_fFontSmall);
	m_ctl_Msg5.SetFont(&m_fFontSmall);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CResultDlg::InitializeMsg() 
{
	m_cs_Msg1 = _T("-");
	m_cs_Msg2 = _T("-");
	m_cs_Msg3 = _T("-");
	m_cs_Msg4 = _T("-");
	m_cs_Msg5 = _T("-");
}
