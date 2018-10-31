// TabOptions.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "TabOptions.h"
#include "MORPHO_Interface.h"


// Boîte de dialogue TabOptions

IMPLEMENT_DYNAMIC(TabOptions, CDialog)

TabOptions::TabOptions(CWnd* pParent /*=NULL*/)
	: CDialog(TabOptions::IDD, pParent)
	, m_b_MainViewerActive(FALSE)
	, m_b_MiniViewerActive(FALSE)
	, m_b_AsyncCommand(FALSE)
	, m_b_AsyncEnrollCommand(FALSE)
	, m_b_AsyncDetectQuality(FALSE)
	, m_b_AsyncCodeQuality(FALSE)
	, m_b_Export_Num_Pk(FALSE)
	, m_b_ConfigKeyUser(FALSE)
	, m_b_WakeUpLedOff(FALSE)
	, m_cs_defaultBaudRate(_T(""))
	, m_i_sleepTimeout(0)
	, m_cs_sensorWinPosition(_T(""))
{
}

TabOptions::~TabOptions()
{
}

void TabOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_MAIN_VIEWER_ACTIVE, m_cmd_MainViewerCheckBox);
	DDX_Check(pDX, IDC_CHECK_MAIN_VIEWER_ACTIVE, m_b_MainViewerActive);
	DDX_Control(pDX, IDC_CHECK_MINI_VIEWER_ACTIVE, m_cmd_MiniViewerCheckBox);
	DDX_Check(pDX, IDC_CHECK_MINI_VIEWER_ACTIVE, m_b_MiniViewerActive);
	DDX_Check(pDX, IDC_CHECK_ASYNC_COMMAND, m_b_AsyncCommand);
	DDX_Check(pDX, IDC_CHECK_ENROLL_COMMAND, m_b_AsyncEnrollCommand);
	DDX_Check(pDX, IDC_CHECK_ASYNC_DETECT_QUALITY, m_b_AsyncDetectQuality);
	DDX_Check(pDX, IDC_CHECK_ASYNC_CODE_QUALITY, m_b_AsyncCodeQuality);
	DDX_Check(pDX, IDC_CHECK_EXPORT_NUM_PK, m_b_Export_Num_Pk);
	DDX_Check(pDX, IDC_CHECK_CONFIG_KEYUSER, m_b_ConfigKeyUser);
	DDX_Check(pDX, IDC_CHECK_WAKE_UP_MODE, m_b_WakeUpLedOff);
	DDX_Control(pDX, IDC_CHECK_WAKE_UP_MODE, m_cmd_WakeUpLedOff);
	DDX_Control(pDX, IDC_CONFIG_SENSOR_WIN_POSITION, m_ctrl_sensorWinPosition);
	DDX_CBString(pDX, IDC_CONFIG_SENSOR_WIN_POSITION, m_cs_sensorWinPosition);
}

BEGIN_MESSAGE_MAP(TabOptions, CDialog)
	ON_BN_CLICKED(IDC_CHECK_MAIN_VIEWER_ACTIVE, &TabOptions::OnCheckMainViewerActive)
	ON_BN_CLICKED(IDC_CHECK_MINI_VIEWER_ACTIVE, &TabOptions::OnCheckMiniViewerActive)
	ON_BN_CLICKED(IDC_CHECK_ASYNC_COMMAND, &TabOptions::OnCheckAsyncCommand)
	ON_BN_CLICKED(IDC_CHECK_ENROLL_COMMAND, &TabOptions::OnCheckEnrollCommand)
	ON_BN_CLICKED(IDC_CHECK_ASYNC_DETECT_QUALITY, &TabOptions::OnCheckAsyncDetectQuality)
	ON_BN_CLICKED(IDC_CHECK_ASYNC_CODE_QUALITY, &TabOptions::OnCheckAsyncCodeQuality)
	ON_BN_CLICKED(IDC_CHECK_EXPORT_NUM_PK, &TabOptions::OnCheckExportNumPk)
	ON_BN_CLICKED(IDC_CHECK_CONFIG_KEYUSER, &TabOptions::OnCheckConfigKeyUser)
	ON_BN_CLICKED(IDC_CHECK_WAKE_UP_MODE, &TabOptions::OnCheckWakeUpMode)
	ON_CBN_SELCHANGE(IDC_CONFIG_SENSOR_WIN_POSITION, &TabOptions::OnCbnSelchangeConfigSensorWinPosition)
END_MESSAGE_MAP()


// Gestionnaires de messages de TabOptions

void TabOptions::OnCheckMainViewerActive()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateData();
	if(m_b_MainViewerActive)
	{
		GetDlgItem(IDC_CHECK_MINI_VIEWER_ACTIVE)->EnableWindow( FALSE );
		*m_pul_CallbackMask |= MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD;
		m_b_MiniViewerActive = FALSE;
	}
	else
	{
		m_b_MiniViewerActive = FALSE;
		GetDlgItem(IDC_CHECK_MINI_VIEWER_ACTIVE)->EnableWindow( TRUE );
		*m_pul_CallbackMask &= ~MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD;
	}
	UpdateData(FALSE);
	m_pcw_Mso_DemoDlg->UpdateData(FALSE);
}

void TabOptions::OnCheckMiniViewerActive()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateData();
	if(m_b_MiniViewerActive)
	{
		m_b_MainViewerActive = TRUE;
		GetDlgItem(IDC_CHECK_MAIN_VIEWER_ACTIVE)->EnableWindow( FALSE );
		*m_pul_CallbackMask |= MORPHO_CALLBACK_IMAGE_CMD;
	}
	else
	{
		m_b_MainViewerActive = FALSE;
		*m_pul_CallbackMask &= ~MORPHO_CALLBACK_IMAGE_CMD; 
		GetDlgItem(IDC_CHECK_MAIN_VIEWER_ACTIVE)->EnableWindow( TRUE );
	}
	UpdateData(FALSE);
	m_pcw_Mso_DemoDlg->UpdateData(FALSE);
}

void TabOptions::OnCheckAsyncCommand()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateData(TRUE);
	if (m_b_AsyncCommand == FALSE)
	{
		*m_pul_CallbackMask &= ~MORPHO_CALLBACK_COMMAND_CMD;
	}
	else 
	{
		*m_pul_CallbackMask |= MORPHO_CALLBACK_COMMAND_CMD;
	}
}

void TabOptions::OnCheckEnrollCommand()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateData(TRUE);
	if (m_b_AsyncEnrollCommand == FALSE)
	{
		*m_pul_CallbackMask &= ~MORPHO_CALLBACK_ENROLLMENT_CMD;
	}
	else 
	{
		*m_pul_CallbackMask |= MORPHO_CALLBACK_ENROLLMENT_CMD;
	}
}

void TabOptions::OnCheckAsyncDetectQuality()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateData(TRUE);

	if (m_b_AsyncDetectQuality == FALSE)
	{
		*m_pul_CallbackMask &= ~MORPHO_CALLBACK_DETECTQUALITY;
	}
	else 
	{
		*m_pul_CallbackMask |= MORPHO_CALLBACK_DETECTQUALITY;
	}
}

void TabOptions::OnCheckAsyncCodeQuality()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateData(TRUE);
	if (m_b_AsyncCodeQuality == FALSE)
	{
		*m_pul_CallbackMask &= ~MORPHO_CALLBACK_CODEQUALITY;
	}
	else 
	{
		*m_pul_CallbackMask |= MORPHO_CALLBACK_CODEQUALITY;
	}
}

void TabOptions::OnCheckConfigKeyUser()
{
	UpdateData(TRUE);
	int l_i_ret = 0;
	int l_i_val = 0;



	if (m_b_ConfigKeyUser == FALSE)
	{
			l_i_val = 0;
			MessageBox("You must restart the sensor through \"Reboot soft\" function before using this parameter!", 0);
	}
	else
	{
			l_i_val = 1;
			MessageBox("Caution, for this value, the device should be powered over 4,5V.\n You must restart the sensor through \"Reboot soft\" function before using this parameter!", 0);
	}

	

	l_i_ret = m_x_pMorphoSmart->SetConfigParam(CONFIG_KEY_USER_TAG, l_i_val);
	if (l_i_ret != MORPHO_OK)
	{
		AfxMessageBox("An error occured while setting configuration parameter.", MB_ICONERROR);
		m_b_ConfigKeyUser = !m_b_ConfigKeyUser;
		UpdateData(FALSE);
	}
	else
	{
		l_i_ret = m_x_pMorphoSmart->GetConfigParam(CONFIG_KEY_USER_TAG, &l_i_val);

		if (l_i_ret != MORPHO_OK)
		{
			AfxMessageBox("An error occured while reading configuration parameter.", MB_ICONERROR);
			m_b_ConfigKeyUser = !m_b_ConfigKeyUser;
			UpdateData(FALSE);
		}
		else
		{
			if (l_i_val == 0)
			{
				m_b_ConfigKeyUser = false;
			}
			else if (l_i_val == 1)
			{
				m_b_ConfigKeyUser = true;
			}
			
			UpdateData(FALSE);
		}
	}
}
void TabOptions::OnCheckExportNumPk()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateData(TRUE);
	if (m_b_Export_Num_Pk == FALSE)
	{
		m_b_Export_Num_Pk = FALSE;		
	}
	else 
	{
		m_b_Export_Num_Pk = TRUE;		
	}
}

void TabOptions::OnCheckWakeUpMode()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateData(TRUE);

	if (m_b_WakeUpLedOff == FALSE)
	{
		*m_pul_WakeUpMode = 0;
	}
	else 
	{
		*m_pul_WakeUpMode = MORPHO_WAKEUP_LED_OFF;
	}
}

void TabOptions::InitConfParams()
{
	int l_i_ret = 0;
	int l_i_val = 0;
	int l_k_ret = 0;
	int l_k_val = 0;
	//SENSOR_WIN_POSITION
	l_i_ret = m_x_pMorphoSmart->GetConfigParam(CONFIG_SENSOR_WIN_POSITION_TAG,
									&l_i_val);

	if(l_i_ret==MORPHO_OK)
	{ 
		if(l_i_val==0)
		{
		  m_cs_sensorWinPosition ="0°";
		}
		else if(l_i_val==1)
		{
			m_cs_sensorWinPosition ="180°";
		}
		else if(l_i_val==2)
		{
			m_cs_sensorWinPosition ="0°-Non Oriented Matching";
		}
		else if(l_i_val==3)
		{
			m_cs_sensorWinPosition ="180°-Non Oriented Matching";
		}
	}
	else
	{
		m_cs_sensorWinPosition ="";
		GetDlgItem(IDC_CONFIG_SENSOR_WIN_POSITION)->EnableWindow(FALSE);
	}

	l_k_ret = m_x_pMorphoSmart->GetConfigParam(CONFIG_KEY_USER_TAG,
		&l_k_val);

	if (l_k_ret == MORPHO_OK)
	{
		if (l_k_val == 0)
		{
			m_b_ConfigKeyUser = false;
		}
		else if (l_k_val == 1)
		{
			m_b_ConfigKeyUser = true;
		}
		
	}
	else
	{
		GetDlgItem(IDC_CHECK_CONFIG_KEYUSER)->EnableWindow(FALSE);
		
	}
	UpdateData(FALSE);		
}

void TabOptions::OnCbnSelchangeConfigSensorWinPosition()
{
	int l_i_ret = 0;
	int l_i_val = 0;

	UpdateData(TRUE);
	
	if (m_cs_sensorWinPosition == "0°")
	{
		l_i_val = 0x00;
	}
	else if (m_cs_sensorWinPosition == "180°")
	{
		l_i_val = 0x01;
	}
	else if (m_cs_sensorWinPosition == "0°-Non Oriented Matching")
	{
		l_i_val =0x02;
	}
	else if (m_cs_sensorWinPosition == "180°-Non Oriented Matching")
	{
		l_i_val =0x03;
	}
	else
		GetDlgItem(IDC_CONFIG_SENSOR_WIN_POSITION)->EnableWindow(FALSE);

	l_i_ret = m_x_pMorphoSmart->SetConfigParam(CONFIG_SENSOR_WIN_POSITION_TAG, l_i_val);

	if (l_i_ret != MORPHO_OK)
	{
		AfxMessageBox("An error occured while writing configuration parameter.", MB_ICONERROR);
		m_ctrl_sensorWinPosition.SetCurSel(-1);
		UpdateData(TRUE);
	}
	else
	{
		l_i_ret = m_x_pMorphoSmart->GetConfigParam(CONFIG_SENSOR_WIN_POSITION_TAG, &l_i_val);

		if (l_i_ret != MORPHO_OK)
		{
			AfxMessageBox("An error occured while reading configuration parameter.", MB_ICONERROR);
			m_ctrl_sensorWinPosition.SetCurSel(-1);
			UpdateData(TRUE);
		}
		else
		{
			if (l_i_val == 0)
			{
				m_cs_sensorWinPosition = "0°";
			}
			else if (l_i_val == 1)
			{
				m_cs_sensorWinPosition = "180°";
			}
			else if (l_i_val == 2)
			{
				m_cs_sensorWinPosition = "0°-Non Oriented Matching";
			}
			else if (l_i_val == 3)
			{
			   m_cs_sensorWinPosition = "180°-Non Oriented Matching";
			}
			
			UpdateData(FALSE);
		}
	}
}
