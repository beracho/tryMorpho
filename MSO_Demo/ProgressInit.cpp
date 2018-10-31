/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// ProgressInit.cpp : implementation file
//

#include "stdafx.h"
#include "mso_demo.h"
#include "ProgressInit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressInit dialog


CProgressInit::CProgressInit(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressInit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressInit)
	m_cs_Mess = _T("");
	m_cs_Mess2 = _T("");
	m_cs_Mess3 = _T("");
	m_cs_Mess4 = _T("");
	m_cs_Mess5 = _T("");
	m_cs_Mess10 = _T("");
	m_cs_Mess11 = _T("");
	m_cs_Mess12 = _T("");
	m_cs_Mess13 = _T("");
	m_cs_Mess14 = _T("");
	m_cs_MainMess = _T("");
	//}}AFX_DATA_INIT

	Create(CProgressInit::IDD,pParent);
}


void CProgressInit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressInit)
	DDX_Control(pDX, IDC_STATIC_BIOAVI, m_static_BioAvi);
	DDX_Text(pDX, IDC_STATIC_MESS, m_cs_Mess);
	DDX_Text(pDX, IDC_STATIC_MESS2, m_cs_Mess2);
	DDX_Text(pDX, IDC_STATIC_MESS3, m_cs_Mess3);
	DDX_Text(pDX, IDC_STATIC_MESS4, m_cs_Mess4);
	DDX_Text(pDX, IDC_STATIC_MESS5, m_cs_Mess5);
	DDX_Text(pDX, IDC_STATIC_MESS10, m_cs_Mess10);
	DDX_Text(pDX, IDC_STATIC_MESS11, m_cs_Mess11);
	DDX_Text(pDX, IDC_STATIC_MESS12, m_cs_Mess12);
	DDX_Text(pDX, IDC_STATIC_MESS13, m_cs_Mess13);
	DDX_Text(pDX, IDC_STATIC_MESS14, m_cs_Mess14);
	DDX_Text(pDX, IDC_STATIC_MAINMESS, m_cs_MainMess);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProgressInit, CDialog)
	//{{AFX_MSG_MAP(CProgressInit)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressInit message handlers
