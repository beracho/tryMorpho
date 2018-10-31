// SecurityLvlDlg.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "MSO_FVP_Login.h"
#include "SecurityLvlDlg.h"


// Boîte de dialogue CSecurityLvlDlg

//IMPLEMENT_DYNCREATE(CSecurityLvlDlg, CDHtmlDialog)

CSecurityLvlDlg::CSecurityLvlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSecurityLvlDlg::IDD, pParent)
{

}

CSecurityLvlDlg::~CSecurityLvlDlg()
{
}

void CSecurityLvlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_COMBO_SECURITY_LEVEL, m_ctrl_SecurityLevel);
}

BOOL CSecurityLvlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//On regle les couleurs
	m_brush.CreateSolidBrush(RGB(5, 5, 5));


	// Remplissage du ComboBox
  if (m_bl_FvpStatus) // Case FVP
  {
	m_ctrl_SecurityLevel.AddString("Default");
	m_ctrl_SecurityLevel.AddString("Standard");
	m_ctrl_SecurityLevel.AddString("Medium");
	m_ctrl_SecurityLevel.AddString("High");
	//m_ctrl_SecurityLevel.AddString("Critical");
	m_ctrl_SecurityLevel.UpdateData(TRUE);

	m_i_CancelSecurityLevel = m_i_SecurityLevel;
	if (m_i_SecurityLevel == SECURITY_LEVEL_DEFAULT)
	{
		m_ctrl_SecurityLevel.SelectString(0, "Default");
	}
	else if (m_i_SecurityLevel == SECURITY_LEVEL_STANDARD)
	{
		m_ctrl_SecurityLevel.SelectString(0, "Standard");
	}
	else if  (m_i_SecurityLevel == SECURITY_LEVEL_MEDIUM)
	{
		m_ctrl_SecurityLevel.SelectString(0, "Medium");
	}
	else if(m_i_SecurityLevel == SECURITY_LEVEL_HIGH)
	{
		m_ctrl_SecurityLevel.SelectString(0, "High");
	}
	/*else if (m_i_SecurityLevel == SECURITY_LEVEL_CRITICAL)
	{
		m_ctrl_SecurityLevel.SelectString(0, "Critical");
	}*/
  }else // Case MSO
  {
	m_ctrl_SecurityLevel.AddString("Default");
	m_ctrl_SecurityLevel.AddString("Low");
	m_ctrl_SecurityLevel.AddString("Medium");
	m_ctrl_SecurityLevel.AddString("High");
	m_ctrl_SecurityLevel.AddString("Critical");
	m_ctrl_SecurityLevel.UpdateData(TRUE);

	m_i_CancelSecurityLevel = m_i_SecurityLevel;
	if (m_i_SecurityLevel == FFD_SECURITY_LEVEL_DEFAULT_HOST)
	{
		m_ctrl_SecurityLevel.SelectString(0, "Default");
	}	
	else if (m_i_SecurityLevel == FFD_SECURITY_LEVEL_LOW_HOST)
	{
		m_ctrl_SecurityLevel.SelectString(0, "Low");
	}
	else if  (m_i_SecurityLevel == FFD_SECURITY_LEVEL_MEDIUM_HOST)
	{
		m_ctrl_SecurityLevel.SelectString(0, "Medium");
	}
	else if(m_i_SecurityLevel == FFD_SECURITY_LEVEL_HIGH_HOST)
	{
		m_ctrl_SecurityLevel.SelectString(0, "High");
	}
	else if (m_i_SecurityLevel == FFD_SECURITY_LEVEL_CRITICAL_HOST)
	{
		m_ctrl_SecurityLevel.SelectString(0, "Critical");
	}
  }

	return TRUE;  // retourne TRUE, sauf si vous avez défini le focus sur un contrôle
}

BEGIN_MESSAGE_MAP(CSecurityLvlDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CSecurityLvlDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_SECURITY_LEVEL, &CSecurityLvlDlg::OnCbnSelchangeComboSecurityLevel)
	ON_BN_CLICKED(IDCANCEL, &CSecurityLvlDlg::OnBnClickedCancel)
END_MESSAGE_MAP()





HBRUSH CSecurityLvlDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{


	// TODO:  Modifier ici les attributs du DC

	// TODO:  Retourner un autre pinceau si le pinceau par défaut n'est pas souhaité
	pDC->SetTextColor(RGB(255,255,255));
	pDC->SetBkColor(RGB(4,4,4));
	

	return m_brush;
}

void CSecurityLvlDlg::OnBnClickedOk()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	OnOK();
}

void CSecurityLvlDlg::OnCbnSelchangeComboSecurityLevel()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	CString l_cs_SelectedSecurityLevel;
	m_i_CancelSecurityLevel = m_i_SecurityLevel;

	m_ctrl_SecurityLevel.GetLBText(m_ctrl_SecurityLevel.GetCurSel(), l_cs_SelectedSecurityLevel);
  if (m_bl_FvpStatus)
  {
	if (strcmp(l_cs_SelectedSecurityLevel, "Critical") == 0)
	{
		m_i_SecurityLevel = SECURITY_LEVEL_CRITICAL;
	}
	else  if (strcmp(l_cs_SelectedSecurityLevel, "High") == 0)
	{
		m_i_SecurityLevel = SECURITY_LEVEL_HIGH;
	}
	else if (strcmp(l_cs_SelectedSecurityLevel, "Medium") == 0)
	{
		m_i_SecurityLevel = SECURITY_LEVEL_MEDIUM;
	}
	else if (strcmp(l_cs_SelectedSecurityLevel, "Standard") == 0)
	{
		m_i_SecurityLevel = SECURITY_LEVEL_STANDARD;
	}else if (strcmp(l_cs_SelectedSecurityLevel, "Default") == 0)
	{
		m_i_SecurityLevel = SECURITY_LEVEL_DEFAULT;
	}
  }
  else
  {
	if (strcmp(l_cs_SelectedSecurityLevel, "Critical") == 0)
	{
		m_i_SecurityLevel = FFD_SECURITY_LEVEL_CRITICAL_HOST;
	}
	else if (strcmp(l_cs_SelectedSecurityLevel, "High") == 0)
	{
		m_i_SecurityLevel = FFD_SECURITY_LEVEL_HIGH_HOST;
	}
	else if (strcmp(l_cs_SelectedSecurityLevel, "Medium") == 0)
	{
		m_i_SecurityLevel = FFD_SECURITY_LEVEL_MEDIUM_HOST;
	}
	else if (strcmp(l_cs_SelectedSecurityLevel, "Low") == 0)
	{
		m_i_SecurityLevel = FFD_SECURITY_LEVEL_LOW_HOST;
	}else if (strcmp(l_cs_SelectedSecurityLevel, "Default") == 0)
	{
		m_i_SecurityLevel = FFD_SECURITY_LEVEL_DEFAULT_HOST;
	}
  }
}

void CSecurityLvlDlg::OnBnClickedCancel()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	m_i_SecurityLevel = m_i_CancelSecurityLevel;
	OnCancel();
}

void CSecurityLvlDlg::SetFVPStatus(BOOL i_bl_FVP)
{
	m_bl_FvpStatus = i_bl_FVP;
}
