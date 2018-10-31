/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// PPageEnroll.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "PPageEnroll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPageEnroll property page

IMPLEMENT_DYNCREATE(CPPageEnroll, CPropertyPage)

CPPageEnroll::CPPageEnroll() : CPropertyPage(CPPageEnroll::IDD)
, m_b_SavePkInDB(FALSE)
, m_cs_TemplateType(_T(""))
, m_cs_FVP_TemplateType(_T(""))
, m_str_CompressionType(_T(""))
, m_b_updateFinger(FALSE)
, m_str_selectedUpdateIndex(_T(""))
{
	//{{AFX_DATA_INIT(CPPageEnroll)
	m_cs_Firstname = _T("");
	m_cs_ID = _T("");
	m_cs_Lastname = _T("");
	m_i_NbFingers = 0;
	m_cs_TemplateType = _T("");
	m_cs_FVP_TemplateType = _T("");
	m_i_CompressionType = 0;
	m_i_CompressionRate = 10;
	UpdateCtr = true;
	//}}AFX_DATA_INIT
}

CPPageEnroll::~CPPageEnroll()
{
}

void CPPageEnroll::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPageEnroll)
	DDX_Text(pDX, IDC_EDIT_ENROLL_FIRSTNAME, m_cs_Firstname);
	DDV_MaxChars(pDX, m_cs_Firstname, 14);
	DDX_Text(pDX, IDC_EDIT_ENROLL_ID, m_cs_ID);
	DDV_MaxChars(pDX, m_cs_ID, 14);
	DDX_Text(pDX, IDC_EDIT_ENROLL_LASTNAME, m_cs_Lastname);
	DDV_MaxChars(pDX, m_cs_Lastname, 14);
	DDX_Radio(pDX, IDC_RADIO_ENROLL_ONEFINGER, m_i_NbFingers);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SAVE_IN_BD_CHECK, m_ctl_SavePkInDB);
	DDX_Check(pDX, IDC_SAVE_IN_BD_CHECK, m_b_SavePkInDB);
	DDX_Control(pDX, IDC_OUTPUT_FILE_TYPE, m_ctrl_TemplateType);
	DDX_CBString(pDX, IDC_OUTPUT_FILE_TYPE, m_cs_TemplateType);

	DDX_Control(pDX, IDC_OUTPUT_FVP_FILE_TYPE, m_ctrl_FVP_TemplateType);
	DDX_CBString(pDX, IDC_OUTPUT_FVP_FILE_TYPE, m_cs_FVP_TemplateType);
	DDX_CBString(pDX, IDC_COMPRESSION_TYPE, m_str_CompressionType);
	DDX_Control(pDX, IDC_COMPRESSION_TYPE, m_cmd_CompressionType);

	DDX_Check(pDX, IDC_CHECK_UPDATE_FINGER, m_b_updateFinger);
	DDX_Control(pDX, IDC_FINGER_INDEX, m_ctrl_updateFingerIndex);
	DDX_CBString(pDX, IDC_FINGER_INDEX, m_str_selectedUpdateIndex);

	if(UpdateCtr)
	{
		CString l_cs_string;

		m_ctrl_updateFingerIndex.ResetContent();
		l_cs_string.Format("%s", STR_FIRST_FINGER);
		m_ctrl_updateFingerIndex.AddString(l_cs_string);
		l_cs_string.Format("%s", STR_SECOND_FINGER);
		m_ctrl_updateFingerIndex.AddString(l_cs_string);
		m_ctrl_updateFingerIndex.SelectString(0, STR_FIRST_FINGER);

		// Template Type

		m_ctrl_TemplateType.ResetContent();

		l_cs_string.Format("%s", STR_PKCOMP);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_PKCOMPNORM);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_PKMAT);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_PKMATNORM);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_PKLITE);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_PKS);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ANSI378);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ANSI378_2009);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO_FMR);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO_FMR_2011);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO_FMC_NS);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO_FMC_CS);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_MINEX_A);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_PK_MOC);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_PK_DIN_V66400_CS);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_PK_DIN_V66400_CS_AA);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO_FMC_CS_AA);
		m_ctrl_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_NO_PK_FP);
		m_ctrl_TemplateType.AddString(l_cs_string);

		m_ctrl_TemplateType.SelectString(0, STR_NO_PK_FP);

		//--------------------------
		//FVP
		//--------------------------

		m_ctrl_FVP_TemplateType.ResetContent();
		l_cs_string.Format("%s", STR_PK_FVP);	
		m_ctrl_FVP_TemplateType.AddString(l_cs_string);

		l_cs_string.Format("%s",STR_NO_FVP);
		m_ctrl_FVP_TemplateType.AddString(l_cs_string);

		m_ctrl_FVP_TemplateType.SelectString(0, STR_NO_FVP);
		

		// Image compression
		m_cmd_CompressionType.ResetContent();

		l_cs_string.Format("%s", STR_NO_IMAGE);
		m_cmd_CompressionType.AddString(l_cs_string);

		l_cs_string.Format("%s", "RAW");
		m_cmd_CompressionType.AddString(l_cs_string);

		l_cs_string.Format("%s", "SAGEM_V1");
		m_cmd_CompressionType.AddString(l_cs_string);

		l_cs_string.Format("%s", "WSQ");
		m_cmd_CompressionType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO19794_FIR_RAW);
		m_cmd_CompressionType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO19794_FIR_BMP);
		m_cmd_CompressionType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO19794_FIR_WSQ);
		m_cmd_CompressionType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO19794_FIR_JPEG);
		m_cmd_CompressionType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO19794_FIR_JPEG2000);
		m_cmd_CompressionType.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO19794_FIR_PNG);
		m_cmd_CompressionType.AddString(l_cs_string);

		m_cmd_CompressionType.SelectString(0, STR_NO_IMAGE);

		if(!m_bl_FVP)
		{
			GetDlgItem(IDC_OUTPUT_FVP_FILE_TYPE)->EnableWindow ( FALSE );
			GetDlgItem(IDC_OUTPUT_FVP_FILE_TYPE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_FVP)->ShowWindow(SW_SHOW);
			m_bl_FVP = FALSE;
		}
		else
		{
			m_bl_FVP = TRUE;
			GetDlgItem(IDC_OUTPUT_FVP_FILE_TYPE)->EnableWindow ( TRUE );
			GetDlgItem(IDC_OUTPUT_FVP_FILE_TYPE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_FVP)->ShowWindow(SW_SHOW);
		}
	}
}


BEGIN_MESSAGE_MAP(CPPageEnroll, CPropertyPage)
	//{{AFX_MSG_MAP(CPPageEnroll)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SAVE_IN_BD_CHECK, &CPPageEnroll::OnSavePkInBDClickCheck)
	ON_CBN_SELCHANGE(IDC_COMPRESSION_TYPE, &CPPageEnroll::OnCbnSelchangeCompressionType)
	ON_CBN_SELCHANGE(IDC_OUTPUT_FILE_TYPE, &CPPageEnroll::OnCbnSelchangeOutputFileType)
	ON_CBN_SELCHANGE(IDC_OUTPUT_FVP_FILE_TYPE, &CPPageEnroll::OnCbnSelchangeOutputFvpFileType)
	ON_BN_CLICKED(IDC_RADIO_ENROLL_ONEFINGER, &CPPageEnroll::OnBnClickedRadioEnrollOnefinger)
	ON_BN_CLICKED(IDC_RADIO_ENROLL_TWOFINGERS, &CPPageEnroll::OnBnClickedRadioEnrollTwofingers)
	ON_BN_CLICKED(IDC_CHECK_UPDATE_FINGER, &CPPageEnroll::OnBnClickedCheckUpdateFinger)
	ON_CBN_SELCHANGE(IDC_FINGER_INDEX, &CPPageEnroll::OnCbnSelchangeFingerIndex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPageEnroll message handlers

BOOL CUpdateUserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ctl_ID.EnableWindow(m_b_activateUSerID);
	if(m_b_activateUSerID)
	{		
		this->SetWindowTextA("Add User");
	}
	else
	{
		this->SetWindowTextA("Update User");
	}
	
	return TRUE; 
}


/////////////////////////////////////////////////////////////////////////////
// CUpdateUserDlg dialog


CUpdateUserDlg::CUpdateUserDlg(bool i_b_activateUSerID,CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpdateUserDlg)
	m_cs_Firstname = _T("");
	m_cs_ID = _T("");
	m_cs_Lastname = _T("");
	m_b_activateUSerID = i_b_activateUSerID;

	//}}AFX_DATA_INIT
}

void CUpdateUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpdateUserDlg)
	DDX_Text(pDX, IDC_EDIT_ENROLL_FIRSTNAME, m_cs_Firstname);
	DDV_MaxChars(pDX, m_cs_Firstname, 14);
	DDX_Text(pDX, IDC_EDIT_ENROLL_ID, m_cs_ID);
	DDV_MaxChars(pDX, m_cs_ID, 14);
	DDX_Text(pDX, IDC_EDIT_ENROLL_LASTNAME, m_cs_Lastname);
	DDV_MaxChars(pDX, m_cs_Lastname, 14);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_ENROLL_ID, m_ctl_ID);

	
}


BEGIN_MESSAGE_MAP(CUpdateUserDlg, CDialog)
	//{{AFX_MSG_MAP(CUpdateUserDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CUpdateUserDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpdateUserDlg message handlers

void CUpdateUserDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CPPageEnroll::OnSavePkInBDClickCheck()
{
	CButton *l_px_updateCheck = (CButton*)GetDlgItem(IDC_CHECK_UPDATE_FINGER);
	CComboBox *l_px_updateIndex = (CComboBox*)GetDlgItem(IDC_FINGER_INDEX);
	int l_i_selection = l_px_updateIndex->GetCurSel();
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateData();

	
	l_px_updateCheck->EnableWindow(m_b_SavePkInDB);
	l_px_updateIndex->EnableWindow(m_b_SavePkInDB && (m_i_NbFingers == 0));
	l_px_updateIndex->SetCurSel(l_i_selection);
	/*if(!m_b_SavePkInDB)
	{		
			GetDlgItem(IDC_OUTPUT_FILE_TYPE)->EnableWindow( TRUE );
			if(m_bl_FVP)
				GetDlgItem(IDC_OUTPUT_FVP_FILE_TYPE)->EnableWindow( TRUE );
	        GetDlgItem(IDC_COMPRESSION_TYPE)->EnableWindow( TRUE );	
	}
	else
	{
			GetDlgItem(IDC_OUTPUT_FILE_TYPE)->EnableWindow( FALSE );
	        GetDlgItem(IDC_OUTPUT_FVP_FILE_TYPE)->EnableWindow( FALSE );
	        GetDlgItem(IDC_COMPRESSION_TYPE)->EnableWindow( FALSE );
	}

	UpdateData(FALSE);*/ 
}

void CPPageEnroll::OnCbnSelchangeCompressionType()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateCtr = false;
	UpdateData(TRUE);
	
	m_cmd_CompressionType.GetLBText(m_cmd_CompressionType.GetCurSel(), m_str_CompressionType);

	if (strcmp(m_str_CompressionType, "RAW") == 0)//The default compression is choice by the MorphoSmart™ (the choice is No compression)
	{
		m_i_CompressionType = 0 ;
		m_i_CompressionRate = 0;
		
	}
	else if(strcmp(m_str_CompressionType, "SAGEM_V1") == 0)
	{
		m_i_CompressionType = 1;
		m_i_CompressionRate = 0;
	}
	else
	{
		m_i_CompressionType = 0;
	}
	
}

void CPPageEnroll::OnCbnSelchangeOutputFileType()
{
	UpdateCtr= false;
	UpdateData ( TRUE );
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}

void CPPageEnroll::OnCbnSelchangeOutputFvpFileType()
{
	UpdateCtr = false;
	UpdateData ( TRUE );
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}

void CPPageEnroll::OnBnClickedRadioEnrollOnefinger()
{
	CComboBox *l_px_updateIndex = (CComboBox*)GetDlgItem(IDC_FINGER_INDEX);
	l_px_updateIndex->EnableWindow(m_b_updateFinger && m_b_SavePkInDB);
}

void CPPageEnroll::OnBnClickedRadioEnrollTwofingers()
{
	CComboBox *l_px_updateIndex = (CComboBox*)GetDlgItem(IDC_FINGER_INDEX);
	l_px_updateIndex->EnableWindow(FALSE);
}

void CPPageEnroll::OnBnClickedCheckUpdateFinger()
{
	int l_i_selection;
	CComboBox *l_px_updateIndex = (CComboBox*)GetDlgItem(IDC_FINGER_INDEX);
	l_i_selection = l_px_updateIndex->GetCurSel();
	UpdateData(TRUE);
	l_px_updateIndex->EnableWindow(m_b_updateFinger && (m_i_NbFingers == 0));
	l_px_updateIndex->SetCurSel(l_i_selection);
}


void CPPageEnroll::OnCbnSelchangeFingerIndex()
{
	UpdateCtr = false;
	UpdateData();
}
