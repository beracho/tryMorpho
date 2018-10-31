/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// PPageVerify.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "PPageVerify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPageVerify property page

IMPLEMENT_DYNCREATE(CPPageVerify, CPropertyPage)

CPPageVerify::CPPageVerify() : CPropertyPage(CPPageVerify::IDD)
{
	//{{AFX_DATA_INIT(CPPageVerify)
	m_i_InputChoice = 0;
	m_i_NbFingers = 1;
	//}}AFX_DATA_INIT
}

CPPageVerify::~CPPageVerify()
{
}

void CPPageVerify::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPageVerify)
	DDX_Control(pDX, IDC_WORKFLOW_TYPE, m_ctrl_WorkflowType);
	DDX_CBString(pDX, IDC_WORKFLOW_TYPE, m_cs_WorkflowType);
	DDX_CBString(pDX, IDC_OUTPUT_FILE_TYPE, m_cs_TemplateType);
	DDX_Control(pDX, IDC_OUTPUT_FILE_TYPE, m_ctrl_TemplateType);
	DDX_Radio(pDX, IDC_RADIO_INPUTFILE, m_i_InputChoice);
	DDX_Radio(pDX, IDC_RADIO_VERIF_ONEFINGER, m_i_NbFingers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPageVerify, CPropertyPage)
	//{{AFX_MSG_MAP(CPPageVerify)
	ON_BN_CLICKED(IDC_RADIO_INPUTFILE, OnRadioInputfile)
	ON_BN_CLICKED(IDC_RADIO_LOCALBASE, OnRadioLocalbase)
	ON_BN_CLICKED(IDC_RADIO_INPUTSMC, OnRadioInputsmc)
	ON_BN_CLICKED(IDC_RADIO_INPUTSMC_INTERNAL, OnRadioInputsmcInternal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPageVerify message handlers
void CPPageVerify::EnableDisableSmartCardControls(BOOL i_b_EnableOrDisable)
{
	GetDlgItem(IDC_RADIO_INPUTSMC)->EnableWindow(i_b_EnableOrDisable);	
	GetDlgItem(IDC_RADIO_INPUTSMC_INTERNAL)->EnableWindow(i_b_EnableOrDisable);	
}

void CPPageVerify::OnRadioInputfile() 
{
	m_ctrl_TemplateType.EnableWindow ( FALSE );	
	m_ctrl_WorkflowType.EnableWindow ( FALSE );	
	GetDlgItem(IDC_RADIO_VERIF_ONEFINGER )->EnableWindow ( FALSE );	
	GetDlgItem(IDC_RADIO_VERIF_TWOFINGERS )->EnableWindow ( FALSE );	
}

void CPPageVerify::OnRadioLocalbase() 
{
	m_ctrl_TemplateType.EnableWindow ( FALSE );	
	m_ctrl_WorkflowType.EnableWindow ( FALSE );	
	GetDlgItem(IDC_RADIO_VERIF_ONEFINGER )->EnableWindow ( FALSE );	
	GetDlgItem(IDC_RADIO_VERIF_TWOFINGERS )->EnableWindow ( FALSE );	
	
}

void CPPageVerify::OnRadioInputsmc() 
{
	m_ctrl_TemplateType.EnableWindow ( TRUE );	
	m_ctrl_WorkflowType.EnableWindow ( TRUE );	
	GetDlgItem(IDC_RADIO_VERIF_ONEFINGER )->EnableWindow ( TRUE );	
	GetDlgItem(IDC_RADIO_VERIF_TWOFINGERS )->EnableWindow ( TRUE );	
	
}

void CPPageVerify::OnRadioInputsmcInternal() 
{
	m_ctrl_TemplateType.EnableWindow ( TRUE );	
	m_ctrl_WorkflowType.EnableWindow ( TRUE );	
	GetDlgItem(IDC_RADIO_VERIF_ONEFINGER )->EnableWindow ( TRUE );	
	GetDlgItem(IDC_RADIO_VERIF_TWOFINGERS )->EnableWindow ( TRUE );	
	
}
