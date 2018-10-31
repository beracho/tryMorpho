/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// PPageCode.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "PPageCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPageCode property page

IMPLEMENT_DYNCREATE(CPPageCode, CPropertyPage)

CPPageCode::CPPageCode() : CPropertyPage(CPPageCode::IDD)
, m_cs_FVP_TemplateType(_T(""))
{
	//{{AFX_DATA_INIT(CPPageCode)
	m_cs_Firstname = _T("");
	m_cs_ID = _T("");
	m_cs_Lastname = _T("");
	m_i_NbFingers = 0;
	m_i_OutputChoice = 0;
	m_i_CaptureType = 0;
	m_b_LatentDetect = FALSE;
	m_cs_TemplateType = _T("");
	m_cs_FVP_TemplateType = _T("");
	m_b_TokenBio = FALSE;
	//}}AFX_DATA_INIT
}

CPPageCode::~CPPageCode()
{
}

void CPPageCode::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPageCode)
	DDX_Control(pDX, IDC_WORKFLOW_TYPE, m_ctrl_WorkflowType);
	DDX_CBString(pDX, IDC_WORKFLOW_TYPE, m_cs_WorkflowType);
	DDX_Control(pDX, IDC_CHECK_CODE_TOKEN_BIO, m_Ctrl_TokenBio);
	DDX_Control(pDX, IDC_OUTPUT_FILE_TYPE, m_ctrl_TemplateType);
	DDX_Control(pDX, IDC_LATENT_DETECT, m_ctrl_LatentDetect);
	DDX_Text(pDX, IDC_EDIT_CODE_FIRSTNAME, m_cs_Firstname);
	DDV_MaxChars(pDX, m_cs_Firstname, 14);
	DDX_Text(pDX, IDC_EDIT_CODE_ID, m_cs_ID);
	DDV_MaxChars(pDX, m_cs_ID, 14);
	DDX_Text(pDX, IDC_IDC_EDIT_CODE_LASTNAME, m_cs_Lastname);
	DDV_MaxChars(pDX, m_cs_Lastname, 14);
	DDX_Radio(pDX, IDC_RADIO_CODE_ONEFINGER, m_i_NbFingers);
	DDX_Radio(pDX, IDC_RADIO_CODE_OUTPUTFILE, m_i_OutputChoice);
	DDX_Radio(pDX, IDC_RADIO_CAPTURE_ENROLL, m_i_CaptureType);
	DDX_Check(pDX, IDC_LATENT_DETECT, m_b_LatentDetect);
	DDX_CBString(pDX, IDC_OUTPUT_FILE_TYPE, m_cs_TemplateType);
	DDX_Check(pDX, IDC_CHECK_CODE_TOKEN_BIO, m_b_TokenBio);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_OUTPUT_FVP_FILE_TYPE, m_ctrl_FVP_TemplateType);
	DDX_CBString(pDX, IDC_OUTPUT_FVP_FILE_TYPE, m_cs_FVP_TemplateType);
}


BEGIN_MESSAGE_MAP(CPPageCode, CPropertyPage)
	//{{AFX_MSG_MAP(CPPageCode)
	ON_BN_CLICKED(IDC_RADIO_CAPTURE_VERIF, OnRadioCaptureVerif)
	ON_BN_CLICKED(IDC_RADIO_CAPTURE_ENROLL, OnRadioCaptureEnroll)
	ON_CBN_SELCHANGE(IDC_OUTPUT_FILE_TYPE, OnSelchangeOutputFileType)
	ON_BN_CLICKED(IDC_RADIO_CODE_OUTPUTSMC, OnRadioCodeOutputsmc)
	ON_BN_CLICKED(IDC_RADIO_CODE_OUTPUTFILE, OnRadioCodeOutputfile)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_OUTPUT_FVP_FILE_TYPE, &CPPageCode::OnCbnSelchangeOutputFvpFileType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPageCode message handlers

void CPPageCode::OnRadioCaptureVerif() 
{
	UpdateData ( TRUE );
	m_ctrl_LatentDetect.EnableWindow ( TRUE );	
	//m_ctrl_FVP_TemplateType.EnableWindow ( TRUE );
	m_b_LatentDetect = TRUE;
	UpdateData ( FALSE );
	//m_ctrl_FVP_TemplateType.SelectString(0,STR_NO_FVP);
	//FVP 1.6
	// Prise en compte du nouveau format
	if(m_bl_FVP)
	{
		//--------------------------
		//FVP
		//--------------------------
		CString l_cs_string;
		m_ctrl_FVP_TemplateType.ResetContent();

		l_cs_string.Format("%s",STR_NO_FVP);
		m_ctrl_FVP_TemplateType.AddString(l_cs_string);

		//FVP 1.6
		//Ajout du nouvel identifiant du format PK_FVP_MATCH
		l_cs_string.Format("%s",STR_PK_FVP_MATCH);
		m_ctrl_FVP_TemplateType.AddString(l_cs_string);

		m_ctrl_FVP_TemplateType.EnableWindow ( TRUE );
		m_ctrl_FVP_TemplateType.SelectString(0,STR_NO_FVP);
	}
}

void CPPageCode::OnRadioCaptureEnroll() 
{
	UpdateData ( TRUE );
	m_ctrl_LatentDetect.EnableWindow ( FALSE );
	m_b_LatentDetect = FALSE;
	UpdateData ( FALSE );
	if(m_bl_FVP)
	{
		
		//--------------------------
		//FVP 1.6
		//--------------------------
		CString l_cs_string;
		m_ctrl_FVP_TemplateType.ResetContent();

		l_cs_string.Format("%s",STR_NO_FVP);
		m_ctrl_FVP_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s",STR_PK_FVP);
		m_ctrl_FVP_TemplateType.AddString(l_cs_string);

		m_ctrl_FVP_TemplateType.EnableWindow ( TRUE );
		m_ctrl_FVP_TemplateType.SelectString(0,STR_PK_FVP);
	}
}

void CPPageCode::EnableDisableSmartCardControls(BOOL i_b_EnableOrDisable)
{
	GetDlgItem(IDC_RADIO_CODE_OUTPUTSMC)->EnableWindow(i_b_EnableOrDisable);	
}


void CPPageCode::OnSelchangeOutputFileType() 
{
	UpdateData(TRUE);

	if (m_cs_TemplateType == STR_PKS)
	{
		m_Ctrl_TokenBio.EnableWindow(FALSE);
		m_Ctrl_TokenBio.SetCheck(0);
	}
	else
	{
		m_Ctrl_TokenBio.EnableWindow(TRUE);
	}
}

void CPPageCode::OnRadioCodeOutputsmc() 
{
	m_Ctrl_TokenBio.SetCheck(0);
	m_Ctrl_TokenBio.EnableWindow(FALSE);
	m_ctrl_WorkflowType.EnableWindow(TRUE);
}

void CPPageCode::OnRadioCodeOutputfile() 
{
	m_Ctrl_TokenBio.EnableWindow(TRUE);	
	m_ctrl_WorkflowType.EnableWindow(FALSE);
}

void CPPageCode::OnCbnSelchangeOutputFvpFileType()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}
