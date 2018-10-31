// TabBioSettings.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "TabBioSettings.h"

// Boîte de dialogue TabBioSettings

IMPLEMENT_DYNAMIC(TabBioSettings, CDialog)

TabBioSettings::TabBioSettings(CWnd* pParent /*=NULL*/)
	: CDialog(TabBioSettings::IDD, pParent)
	, m_i_Threshold(0)
	, m_i_Timeout(0)
	, m_str_CoderChoice(_T(""))
	, m_str_FFDSecuLevel(_T(""))
	, m_str_BioMode(_T(""))
	, m_str_StrategyAcquisitionMode(_T(""))
	, m_cmd_ForceFingerPlacement(TRUE)
{
	
}

TabBioSettings::~TabBioSettings()
{
}

void TabBioSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_i_Threshold);
	DDX_Text(pDX, IDC_EDIT_TIMEOUT, m_i_Timeout);
	DDX_CBString(pDX, IDC_CODER_CHOICE, m_str_CoderChoice);
	DDX_Control(pDX, IDC_CODER_CHOICE, m_cmd_CoderChoice);
	DDX_Control(pDX, IDC_FFD_SECU_LEVEL, m_cmd_FFDSecuLevel);
	DDX_CBString(pDX, IDC_FFD_SECU_LEVEL, m_str_FFDSecuLevel);
	DDX_CBString(pDX, IDC_BIO_MODE, m_str_BioMode);
	DDX_Control(pDX, IDC_BIO_MODE, m_cmd_BioMode);
	DDX_Check(pDX, IDC_CHECK_FFPLACEMENT_ON_TOP, m_cmd_ForceFingerPlacement);
	DDX_Check(pDX, IDC_CHECK_ASLC, m_cmd_ASLC);
	DDX_Check(pDX, IDC_CHECK_FINGERPRINT_QUALITY, m_cmd_FingerPrint_Quality);
	DDX_Control(pDX, IDC_EDIT_FINGERPRINT_QUALITY, m_i_EditFingerPrintQuality);
	DDX_CBString(pDX, IDC_STRATEGY_MODE, m_str_StrategyAcquisitionMode);
	DDX_Control(pDX, IDC_STRATEGY_MODE, m_cmd_StrategyAcquisitionMode);
}


BEGIN_MESSAGE_MAP(TabBioSettings, CDialog)
	ON_CBN_SELCHANGE(IDC_CODER_CHOICE, &TabBioSettings::OnSelchangeCoderChoice)
	ON_CBN_SELCHANGE(IDC_FFD_SECU_LEVEL, &TabBioSettings::OnSelchangeFfdSecuLevel)
	ON_CBN_SELCHANGE(IDC_BIO_MODE, &TabBioSettings::OnSelchangeBioMode)
	ON_EN_CHANGE(IDC_EDIT_TIMEOUT, &TabBioSettings::OnEnChangeEditTimeout)
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD, &TabBioSettings::OnEnChangeEditThreshold)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_BN_CLICKED(IDC_CHECK_FINGERPRINT_QUALITY, &TabBioSettings::OnBnClickedCheckFingerprintQuality)
	ON_CBN_SELCHANGE(IDC_STRATEGY_MODE, &TabBioSettings::OnCbnSelchangeStrategyAcquisitionMode)
	ON_BN_CLICKED(IDC_CHECK_FFPLACEMENT_ON_TOP, &TabBioSettings::OnBnClickedCheckFfplacementOnTop)
END_MESSAGE_MAP()


BOOL TabBioSettings::OnInitDialog()
{
	CDialog::OnInitDialog();
	EnableToolTips(TRUE); // enable tool tips for view
	return TRUE; // return TRUE unless you set the focus to a control

}

BOOL TabBioSettings::OnToolTipNotify(UINT i_ui_id, NMHDR *i_PNMHDR_Nmhdr, LRESULT *o_PLRESULT_Result)
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* l_PTT_TextA = (TOOLTIPTEXTA*)i_PNMHDR_Nmhdr;
	TOOLTIPTEXTW* l_PTT_TextW  = (TOOLTIPTEXTW*)i_PNMHDR_Nmhdr;
	CString l_str_TipText;
	UI l_ui_IdControl = i_PNMHDR_Nmhdr->idFrom;
	
	if (i_PNMHDR_Nmhdr->code == TTN_NEEDTEXTA && (l_PTT_TextA->uFlags & TTF_IDISHWND) ||
	i_PNMHDR_Nmhdr->code == TTN_NEEDTEXTW && (l_PTT_TextW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		l_ui_IdControl = ::GetDlgCtrlID((HWND)l_ui_IdControl);
	}

	if (l_ui_IdControl  == IDC_CHECK_FFPLACEMENT_ON_TOP) 
	{
		if(m_bl_isCBMV)
		{
			l_str_TipText.Format("This option is not compatible with CBM-V");
		}
		else
		{		
			l_str_TipText.Format("Force the finger to cover the top of the capture area to increase quality");
		}
	}

	if (i_PNMHDR_Nmhdr->code == TTN_NEEDTEXTA)
	{
		lstrcpyn(l_PTT_TextA->szText, l_str_TipText, sizeof(l_PTT_TextA->szText));
	}
	else
	{
		_mbstowcsz(l_PTT_TextW->szText, l_str_TipText, sizeof(l_PTT_TextW->szText));
	}
	*o_PLRESULT_Result = 0;

return TRUE; // message was handled
}

// Gestionnaires de messages de TabBioSettings

void TabBioSettings::OnSelchangeCoderChoice()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateData(TRUE);
	m_cmd_CoderChoice.GetLBText(m_cmd_CoderChoice.GetCurSel(), m_str_CoderChoice);


	if (strcmp(m_str_CoderChoice, "Default") == 0)//The default coder is choice by the MorphoSmart™ (the choice is the Standard coder)
	{
		m_i_CoderChoice = MORPHO_DEFAULT_CODER;
	}
	else if(strcmp(m_str_CoderChoice, "Standard") == 0)
	{
		m_i_CoderChoice = MORPHO_MSO_V9_CODER;
	}
	else if(strcmp(m_str_CoderChoice, "Juvenile") == 0)
	{
		m_i_CoderChoice = MORPHO_MSO_V9_JUV_CODER;
	}
	else if(strcmp(m_str_CoderChoice, "Thin finger") == 0)
	{
		m_i_CoderChoice = MORPHO_MSO_V9_THIN_FINGER_CODER;
	}
	else
	{
		m_i_CoderChoice = MORPHO_MSO_V9_CODER;
	}
}

void TabBioSettings::OnSelchangeFfdSecuLevel()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	int l_i_FFDSecuLevel;
	UpdateData(TRUE);
	m_cmd_FFDSecuLevel.GetLBText(m_cmd_FFDSecuLevel.GetCurSel(), m_str_FFDSecuLevel);		

	// FVP
	if (m_bl_FVP)
	{
			if (strcmp(m_str_FFDSecuLevel, "Standard") == 0)
			{
				l_i_FFDSecuLevel = MULTIMODAL_SECURITY_STANDARD;
			}
			else if(strcmp(m_str_FFDSecuLevel, "Medium") == 0)
			{
				l_i_FFDSecuLevel = MULTIMODAL_SECURITY_MEDIUM;
			}
			else if(strcmp(m_str_FFDSecuLevel, "High") == 0)
			{
				l_i_FFDSecuLevel = MULTIMODAL_SECURITY_HIGH;
			}
			/*else if (strcmp(m_str_FFDSecuLevel, "Critical") == 0)
			{
				l_i_FFDSecuLevel = MULTIMODAL_SECURITY_CRITICAL;
			}*/
			m_px_pMorphoSmart->Set_FFD_SecurityLevel(l_i_FFDSecuLevel);
	}
	else //MSO
	{
			if (strcmp(m_str_FFDSecuLevel, "Low") == 0)
			{
				l_i_FFDSecuLevel = FFD_SECURITY_LEVEL_LOW_HOST;
			}
			else if(strcmp(m_str_FFDSecuLevel, "Medium") == 0)
			{
				l_i_FFDSecuLevel = FFD_SECURITY_LEVEL_MEDIUM_HOST;
			}
			else if(strcmp(m_str_FFDSecuLevel, "High") == 0)
			{
				l_i_FFDSecuLevel = FFD_SECURITY_LEVEL_HIGH_HOST;
			}
			else if (strcmp(m_str_FFDSecuLevel, "Critical") == 0)
			{
				l_i_FFDSecuLevel = FFD_SECURITY_LEVEL_CRITICAL_HOST;
			}
			else if(strcmp(m_str_FFDSecuLevel, "Default") == 0)
			{
				l_i_FFDSecuLevel = FFD_SECURITY_LEVEL_DEFAULT_HOST;
			}
			m_px_pMorphoSmart->Set_FFD_SecurityLevel(l_i_FFDSecuLevel);
	}	
}

void TabBioSettings::OnSelchangeBioMode()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateData(TRUE);
	m_cmd_BioMode.GetLBText(m_cmd_BioMode.GetCurSel(), m_str_BioMode);

	if (strcmp(m_str_BioMode, "Default") == 0)
	{
		m_i_MatchingStrategy = MORPHO_STANDARD_MATCHING_STRATEGY;
	}
	else if(strcmp(m_str_BioMode, "Advanced") == 0)
	{
		m_i_MatchingStrategy = MORPHO_ADVANCED_MATCHING_STRATEGY;
	}
	else
	{
		m_i_MatchingStrategy = MORPHO_STANDARD_MATCHING_STRATEGY;
	}
}

void TabBioSettings::OnEnChangeEditTimeout()
{
	UpdateData(TRUE);
}

void TabBioSettings::OnEnChangeEditThreshold()
{	
	UpdateData(TRUE);
}

void TabBioSettings::OnBnClickedCheckFingerprintQuality()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	
	CString l_cs_FingerQuality;
	I l_i_FingerQuality;

	GetDlgItem(IDC_EDIT_FINGERPRINT_QUALITY)->GetWindowText(l_cs_FingerQuality);
	l_i_FingerQuality = atoi(l_cs_FingerQuality);

	if(m_cmd_FingerPrint_Quality) 
	{
		GetDlgItem(IDC_EDIT_FINGERPRINT_QUALITY)->EnableWindow( FALSE );
	}else 
	{
		GetDlgItem(IDC_EDIT_FINGERPRINT_QUALITY)->EnableWindow( TRUE  );
	}
	UpdateData();
}

void TabBioSettings::OnCbnSelchangeStrategyAcquisitionMode()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_cmd_StrategyAcquisitionMode.GetLBText(m_cmd_StrategyAcquisitionMode.GetCurSel(), m_str_StrategyAcquisitionMode);		
	if (strcmp(m_str_StrategyAcquisitionMode, "Expert(Default)") == 0)
	{
		m_uc_StrategyAcquisitionMode = MORPHO_ACQ_EXPERT_MODE;
		m_px_pMorphoSmart->SetStrategyAcquisitionMode(MORPHO_ACQ_EXPERT_MODE);
	}
	else if(strcmp(m_str_StrategyAcquisitionMode, "Fast(Standard)") == 0)
	{
		m_uc_StrategyAcquisitionMode = MORPHO_ACQ_UNIVERSAL_FAST_MODE;
		m_px_pMorphoSmart->SetStrategyAcquisitionMode(MORPHO_ACQ_UNIVERSAL_FAST_MODE);
	}
	else if(strcmp(m_str_StrategyAcquisitionMode, "Slow(Accurate)") == 0)
	{
		m_uc_StrategyAcquisitionMode = MORPHO_ACQ_UNIVERSAL_ACCURATE_MODE;
		m_px_pMorphoSmart->SetStrategyAcquisitionMode(MORPHO_ACQ_UNIVERSAL_ACCURATE_MODE);
	}
	else if(strcmp(m_str_StrategyAcquisitionMode, "Full MultiModal") == 0)
	{
		m_uc_StrategyAcquisitionMode = MORPHO_ACQ_FULL_MULTIMODAL_MODE;
		m_px_pMorphoSmart->SetStrategyAcquisitionMode(MORPHO_ACQ_FULL_MULTIMODAL_MODE);
	}
	else if(strcmp(m_str_StrategyAcquisitionMode, "Anti Spoofing") == 0)
	{
		m_uc_StrategyAcquisitionMode = MORPHO_ACQ_ANTI_SPOOFING_MODE;
		m_px_pMorphoSmart->SetStrategyAcquisitionMode(MORPHO_ACQ_ANTI_SPOOFING_MODE);
	}
	UpdateData(TRUE);
}

void TabBioSettings::OnBnClickedCheckFfplacementOnTop()
{
	UpdateData(TRUE);
}
