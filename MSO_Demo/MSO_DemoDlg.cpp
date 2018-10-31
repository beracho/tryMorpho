/************************************************************
Copyright (c) 2002-2005, Morpho
************************************************************/

// MSO_DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include <direct.h>
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <iostream>
#include "MSO_Demo.h"
#include "MSO_DemoDlg.h"
#include "ResultDlg.h"
#include "PPageProcess.h"
#include "MSOSECU.h"
#include "ProgressInit.h"
#include "ConvertSDKError.h"
#include "DisplayMSOConfDlg.h"
#include "InfoVersion.h"
#include "ConvertRawToBmp.h"
#include "DBaseCreateOption.h"
#include "GB_BioDemoCard.h"
#include "SagemBioDemoCard.h"
#include "Smartcard_error.h"
#include "SmartcardYpsID.h"
#include "GetInfoDlg.h"

#include "ImageCompress.h"

#include "Winsock2.h"
#include "Sp_ClientPipe.h"

#include "SCardReader.h"

#include "MORPHO_Utils.h"
#include "ISO19794_FIR_InfoDlg.h"

#include "DiversificationData.h"

#include "TlHelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UC  l_uc_image;
CString		g_str_ProductDesc = "";
CString		g_str_SoftwareDesc = "";
CString		g_str_SensorDesc = "";
C			g_str_path[_MAX_PATH];
int         g_test = 0;
PUC			g_puc_LogFileName = NULL;

static CString g_x_BiometricFilesExtention("All Biometric Files |*.iso-fmr-2011; *.ansi-fmr-2009; *.pks; *.tkb; *.pkc; *.pkcn; *.pkmat; *.pkm; *.pkmn; *.pkl; *.pklite; *.ansi-fmr; *.minex-a; *.iso-fmr; *.iso-fmc-ns; *.iso-fmc-cs; *.pkv10; *.moc; *.din-cs; *.fvp; *.fvp-m|Pks Files (*.pks)|*.pks|Token Bio Files (*.tkb)|*.tkb|PkComp Files (*.pkc)|*.pkc|PkComp Norm Files (*.pkcn)|*.pkcn|PkLite Files (*.pkl;*.pklite)|*.pkl; *.pklite|PkMat Files (*.pkmat)|*.pkmat|PkMat Files (*.pkm)|*.pkm|PkMat Norm Files (*.pkmn)|*.pkmn|ANSI 378 FMR (*.ansi-fmr)|*.ansi-fmr|ANSI 378 FMR 2009 (*.ansi-fmr-2009)|*.ansi-fmr-2009|MINEX-A (*.minex-a)|*.minex-a|ISO 19794-2 FMR (*.iso-fmr)|*.iso-fmr|ISO 19794-2 FMR 2011 (*.iso-fmr-2011)|*.iso-fmr-2011|ISO 19794 FMC Normal Size (*.iso-fmc-ns)|*.iso-fmc-ns|ISO 19794 FMC Compact Size (*.iso-fmc-cs)|*.iso-fmc-cs|PKV10 (*.pkV10)|*.pkv10|SAGEM PKMOC (*.moc)|*.moc|DIN-V66400 Compact Size (*.din-cs)|*.din-cs|Multimodal (*.fvp)|*.fvp|Multimodal (*.fvp-m)|*.fvp-m|All Files (*.*)|*.*||");

static CString g_x_certifFilter("Certificate Files (*.der)|*.der|All Files (*.*)|*.*||");

#define SAV_PATH		_getcwd(g_str_path, _MAX_PATH );
#define RESTORE_PATH	_chdir(g_str_path);

#define	TIMEOUT_THREAD		1000

#define JMARK_BIT_TEMPLATE_VALUE_OFFSET_REFJMARKOBJECT	2
#define JMARK_BIT_TEMPLATE_VALUE_OFFSET_REFFINGER		18
#define JMARK_BIT_TEMPLATE_VALUE_OFFSET_PKFORMAT		21
#define JMARK_BIT_TEMPLATE_VALUE_PKFORMAT_ISO			"\x00\x1D"
#define JMARK_BIT_TEMPLATE_VALUE_PKFORMAT_ISO_LEN		(sizeof(JMARK_BIT_TEMPLATE_VALUE_PKFORMAT_ISO) - 1)
#define JMARK_BIT_TEMPLATE_VALUE						"\x83\x01\x00" \
	"\x06\x06\x60\x86\x48\x01\x65\x03" \
	"\xA1\x17" \
	"\x81\x01\x08" \
	"\x82\x01\x00" \
	"\x87\x02\x00\x1A" \
	"\x88\x02\x08\x04" \
	"\xB1\x07" \
	"\x81\x02\x0C\x38" \
	"\x82\x01\x0D"
#define JMARK_BIT_TEMPLATE_VALUE_LEN					(sizeof(JMARK_BIT_TEMPLATE_VALUE) - 1)

#define ANSI_FMR_378_PK_NUMBER_OFFSET		0x18
#define ANSI_FMR_378_2009_PK_NUMBER_OFFSET	0x13

#define	CCID_MAX_DATA_LENGTH	2048
#define APDU_HEADER_LENGTH		5

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_str_sensor_desc;
	CString	m_str_product_desc;
	CString	m_str_software_desc;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
public:
#if (_MSC_VER > 1300)
	virtual INT_PTR DoModal();
#else
	virtual INT DoModal();
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_str_sensor_desc = _T("");
	m_str_product_desc = _T("");
	m_str_software_desc = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_SENSOR_DESC, m_str_sensor_desc);
	DDX_Text(pDX, IDC_PRODUCT_DESC, m_str_product_desc);
	DDX_Text(pDX, IDC_SOFTWARE_DESC, m_str_software_desc);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#if (_MSC_VER > 1300)
INT_PTR CAboutDlg::DoModal()
#else
INT CAboutDlg::DoModal()
#endif
{
	m_str_product_desc = g_str_ProductDesc;
	m_str_software_desc = g_str_SoftwareDesc;
	m_str_sensor_desc = g_str_SensorDesc;

	return CDialog::DoModal();
}

//////////////////////////////////////////////////////////////////
I GetAndWriteFFDLogs(C_MORPHO_Device *i_Device)
{
	FILE*	l_px_file;		// Log file
	PUC		l_puc_log;		// Pointer on the total log buffer
	I		l_i_Ret;		// Return from called functions
	UL		l_ul_loglength;	// Total log length

	l_i_Ret = MORPHO_OK;
	l_ul_loglength = 0;
	l_puc_log = NULL;
	// If log file name valid
	if (g_puc_LogFileName != NULL)
	{
		l_puc_log = NULL;
		// Get all the log lines
		l_i_Ret = (*i_Device).GetFFDLogs(&l_ul_loglength, &l_puc_log);
		// If read log OK and there is a log to write
		if ((l_i_Ret == MORPHO_OK) && (l_puc_log != NULL))
		{
			// Open log file
			fopen_s(&l_px_file, (char*)g_puc_LogFileName, "at");
			// If open OK
			if (l_px_file != NULL)
			{
				// Write log in file
				fwrite(l_puc_log, l_ul_loglength, 1, l_px_file);
				// Close log file
				fclose(l_px_file);
			}
		}
		// Release the log buffer
		(*i_Device).ReleaseFFDLogs(l_puc_log);
	}
	// Return
	return l_i_Ret;
}

CString GetfileExtension(CString filename)
{
	CString l_cstr_extension = "";
	I l_i_extension_position;
	l_i_extension_position = filename.ReverseFind('.'); //".xxx"
	if (l_i_extension_position != -1)
	{
		l_cstr_extension = filename.Right(filename.GetLength() - l_i_extension_position - 1);
	}
	return l_cstr_extension;
}

CString GetfileNameFromPath(CString path)
{
	CString l_cstr_filename = "";
	I	l_i_position;
	l_i_position = path.ReverseFind('\\');

	if (l_i_position != -1)
	{
		l_cstr_filename = path.Right(path.GetLength() - l_i_position - 1);
	}

	return l_cstr_filename;
}

BOOL findSagemSecurite_Licence_Protection()
{
	BOOL l_b_Ret = false;
	PROCESSENTRY32 l_x_Process;
	HANDLE l_x_Snapshot = (HANDLE)CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if ((int)l_x_Snapshot != -1)
	{
		l_x_Process.dwSize = sizeof(l_x_Process);
		BOOL r = Process32First(l_x_Snapshot, &l_x_Process);
		do
		{
			if (strcmp("SagemSecurite_Licence_Protection.exe", l_x_Process.szExeFile) == 0)
			{
				l_b_Ret = true;
			}
			else if (strcmp("SagemSecurite_License_Protection.exe", l_x_Process.szExeFile) == 0)
			{
				l_b_Ret = true;
			}

			r = Process32Next(l_x_Snapshot, &l_x_Process);
		} while (r&(!l_b_Ret));
	}
	CloseHandle(l_x_Snapshot);
	return l_b_Ret;
}

//////////////////////////////////////////////////////////////////
I BiometricCallback(PVOID						i_pv_context,
	T_MORPHO_CALLBACK_COMMAND	i_i_command,
	PVOID						i_pv_param
	)
{

	PUC l_puc_EnrollmentCmd;
	CString l_cs_PositionMessage, l_cs_CmdMessage;
	CMSO_DemoDlg* MSO_DemoDlg = (CMSO_DemoDlg *)i_pv_context;
	T_MORPHO_CALLBACK_IMAGE_STATUS l_x_ImageStructure;
	CDC	*l_p_Cdc;
	CRect	myRect;
	UC l_uc_quality;
	//BUG CORRIGE PAR C++TEST [BD-PB-CC-2] 
	memset(&l_x_ImageStructure, 0x00, sizeof(T_MORPHO_CALLBACK_IMAGE_STATUS));
	//-----------------------------------------------------------
	// Asynchronous Position Messages
	//-----------------------------------------------------------

	if (i_i_command == MORPHO_CALLBACK_COMMAND_CMD)
	{
		if ((*(PI)i_pv_param != MORPHO_MOVE_NO_FINGER) && (MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage != NULL))
		{
			MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->setColor(2);
		}

		switch (*(PI)i_pv_param)
		{
		case MORPHO_REMOVE_FINGER:
			MSO_DemoDlg->HideDirection();
			MSO_DemoDlg->ShowAnimation("image");
			MSO_DemoDlg->m_cs_AsyncEnrollMsg.Format("REMOVE FINGER");
			MSO_DemoDlg->m_cs_AsyncCmdMsg.Format("");
			MSO_DemoDlg->m_bl_FVP_Live_isRunning = FALSE;
			Beep(BEEP_FREQUENCY, BEEP_DURATION);
			break;
		case MORPHO_MOVE_NO_FINGER:
			MSO_DemoDlg->HideDirection();
			MSO_DemoDlg->m_cs_AsyncEnrollMsg.Format("");
			MSO_DemoDlg->m_cs_AsyncCmdMsg.Format("");
			MSO_DemoDlg->ShowAnimation("capture");
			MSO_DemoDlg->m_bl_FVP_Live_isRunning = FALSE;
			if (MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage != NULL)
				MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->setColor(0);
			break;
		case MORPHO_LATENT:
			MSO_DemoDlg->m_cs_AsyncCmdMsg.Format("LATENT");
			break;
		case MORPHO_MOVE_FINGER_UP:
			MSO_DemoDlg->m_cs_AsyncCmdMsg.Format("MOVE UP");
			MSO_DemoDlg->ShowDirection(UP);
			if (!MSO_DemoDlg->m_bl_FVP_Live_isRunning)
			{
				MSO_DemoDlg->HideAnimation();
			}
			break;
		case MORPHO_MOVE_FINGER_DOWN:
			MSO_DemoDlg->m_cs_AsyncCmdMsg.Format("MOVE DOWN");
			MSO_DemoDlg->ShowDirection(DOWN);
			if (!MSO_DemoDlg->m_bl_FVP_Live_isRunning)
			{
				MSO_DemoDlg->HideAnimation();
			}
			break;
		case MORPHO_MOVE_FINGER_LEFT:
			MSO_DemoDlg->m_cs_AsyncCmdMsg.Format("MOVE LEFT");
			MSO_DemoDlg->ShowDirection(LEFT);
			if (!MSO_DemoDlg->m_bl_FVP_Live_isRunning)
			{
				MSO_DemoDlg->HideAnimation();
			}
			break;
		case MORPHO_MOVE_FINGER_RIGHT:
			MSO_DemoDlg->m_cs_AsyncCmdMsg.Format("MOVE RIGHT");
			MSO_DemoDlg->ShowDirection(RIGHT);
			if (!MSO_DemoDlg->m_bl_FVP_Live_isRunning)
			{
				MSO_DemoDlg->HideAnimation();
			}
			break;
		case MORPHO_PRESS_FINGER_HARDER:
			MSO_DemoDlg->m_cs_AsyncCmdMsg.Format("PRESS HARDER");
			if (!MSO_DemoDlg->m_bl_FVP_Live_isRunning)
			{
				MSO_DemoDlg->HideAnimation();
			}
			break;
		case MORPHO_FINGER_OK:
			MSO_DemoDlg->HideDirection();
			MSO_DemoDlg->HideAnimation();
			MSO_DemoDlg->ShowAnimation("image_Ok");
			MSO_DemoDlg->m_bl_FVP_Live_isRunning = FALSE;
			if ((*(PI)i_pv_param != MORPHO_MOVE_NO_FINGER) && (MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage != NULL) && MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_AsyncEnrollCommand)
			{
				MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->setColor(1);
				MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->drawRectangle();
			}
			MSO_DemoDlg->m_cs_AsyncCmdMsg.Format("ACQUISITION OK");
			break;
		case MORPHO_FINGER_MISPLACED:
			MSO_DemoDlg->HideDirection();
			// On reçoit ce message uniquement si la DTPR hard est activée sur le FVP
			MSO_DemoDlg->m_cs_AsyncEnrollMsg.Format("BAD FINGER PLACEMENT");
			break;
		case MORPHO_FINGER_DETECTED:
			if ((MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive == FALSE) && (MSO_DemoDlg->m_bl_FVP == true))
			{
				MSO_DemoDlg->ShowAnimation("maillage");
			}
			//else
			//{
			//	MSO_DemoDlg->HideAnimation();
			//};
			break;
		case MORPHO_LIVE_OK:
			MSO_DemoDlg->HideDirection();
			MSO_DemoDlg->m_cs_AsyncEnrollMsg.Format("FINGER DETECTED,\nPROCESSING ...");
			MSO_DemoDlg->m_bl_FVP_Live_isRunning = TRUE;
			MSO_DemoDlg->HideAnimation();
			MSO_DemoDlg->ShowAnimation("maillage");
			break;
		}

		MSO_DemoDlg->PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
	}

	//-----------------------------------------------------------
	// Asynchronous SendQualityCode Message
	//-----------------------------------------------------------
	if (i_i_command == MORPHO_CALLBACK_CODEQUALITY)
	{
		l_uc_quality = *(PUC)i_pv_param;
		if (MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage != NULL)
		{
			MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_cs_AsyncCmdMsg.Format("CODED QUALITY: %d", l_uc_quality);
		}
		MSO_DemoDlg->m_cs_QualityMsg.Format("%d", l_uc_quality);
		MSO_DemoDlg->PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
		/////////////////////////////
		if (!MSO_DemoDlg->m_tbCtrl.m_TabBioSettingsDlg->m_cmd_FingerPrint_Quality)
		{
			if (((int)l_uc_quality) >20)
			{
				MSO_DemoDlg->m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(0, 255, 0));
			}
			else
			{
				MSO_DemoDlg->m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(0, 0, 255));
			}
		}
		else
		{
			CString		l_cs_FingerQuality;
			I			l_i_FingerQuality;
			l_i_FingerQuality = 0;

			MSO_DemoDlg->m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_EDIT_FINGERPRINT_QUALITY)->GetWindowText(l_cs_FingerQuality);
			l_i_FingerQuality = atoi(l_cs_FingerQuality);

			if (((int)l_uc_quality) > l_i_FingerQuality)
			{
				MSO_DemoDlg->m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(0, 255, 0));
			}
			else
			{
				MSO_DemoDlg->m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(0, 0, 255));
			}

		}

		MSO_DemoDlg->m_qualiteProgress.ShowWindow(SWP_SHOWWINDOW);
		MSO_DemoDlg->m_qualiteProgress.SetPos((int)l_uc_quality);
		///////////////////////
	}

	if (i_i_command == MORPHO_CALLBACK_DETECTQUALITY)
	{
		l_uc_quality = *(PUC)i_pv_param;
		if (MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage != NULL)
		{
			MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_cs_AsyncCmdMsg.Format("DETECTED QUALITY: %d", l_uc_quality);
		}
		MSO_DemoDlg->m_cs_QualityMsg.Format("%d", l_uc_quality);
		MSO_DemoDlg->PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);

		if (!MSO_DemoDlg->m_tbCtrl.m_TabBioSettingsDlg->m_cmd_FingerPrint_Quality)
		{
			if (((int)l_uc_quality) >20)
			{
				MSO_DemoDlg->m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(0, 255, 0));
			}
			else
			{
				MSO_DemoDlg->m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(0, 0, 255));
			}
		}
		else
		{
			CString		l_cs_FingerQuality;
			I			l_i_FingerQuality;
			l_i_FingerQuality = 0;

			MSO_DemoDlg->m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_EDIT_FINGERPRINT_QUALITY)->GetWindowText(l_cs_FingerQuality);
			l_i_FingerQuality = atoi(l_cs_FingerQuality);

			if (((int)l_uc_quality) > l_i_FingerQuality)
			{
				MSO_DemoDlg->m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(0, 255, 0));
			}
			else
			{
				MSO_DemoDlg->m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(0, 0, 255));
			}

		}

		MSO_DemoDlg->m_qualiteProgress.ShowWindow(SWP_SHOWWINDOW);
		MSO_DemoDlg->m_qualiteProgress.SetPos((int)l_uc_quality);
	}

	//-----------------------------------------------------------
	// Asynchronous Command Messages
	//-----------------------------------------------------------

	// l_puc_EnrollmentCmd[0] : Current Finger number enrolled(1 or 2) 
	// l_puc_EnrollmentCmd[1] : Number of fingers to enroll (1 or 2)
	// l_puc_EnrollmentCmd[2] : Current acquisition number (1 to 3) 
	// l_puc_EnrollmentCmd[3] : Total number of acquisition (3)

	if (i_i_command == MORPHO_CALLBACK_ENROLLMENT_CMD)
	{
		MSO_DemoDlg->HideDirection();
		l_puc_EnrollmentCmd = (PUC)i_pv_param;
		switch (l_puc_EnrollmentCmd[0])
		{
		case 1:
			switch (l_puc_EnrollmentCmd[2])
			{
			case 1:
				MSO_DemoDlg->m_cs_AsyncEnrollMsg.Format("PLACE Finger #1\nCAPTURE %d/%d", l_puc_EnrollmentCmd[2], l_puc_EnrollmentCmd[3]);
				if (MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
				{
					MSO_DemoDlg->m_ctl_CurrentMiniViewer = &(MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_ctl_MiniViewer_1_1);
				}
				MSO_DemoDlg->m_i_CurrentViewerIndex = 0;
				if (MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
				{
					MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_i_ViewerToColorIdx = 0;
				}
				break;

			case 2:
				MSO_DemoDlg->m_cs_AsyncEnrollMsg.Format("Finger #1 AGAIN\nCAPTURE %d/%d", l_puc_EnrollmentCmd[2], l_puc_EnrollmentCmd[3]);
				if (MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
				{
					MSO_DemoDlg->m_ctl_CurrentMiniViewer = &(MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_ctl_MiniViewer_1_2);
				}
				MSO_DemoDlg->m_i_CurrentViewerIndex = 1;
				if (MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
				{
					MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_i_ViewerToColorIdx = 1;
				}
				break;

			case 3:
				MSO_DemoDlg->m_cs_AsyncEnrollMsg.Format("Finger #1 AGAIN\nCAPTURE %d/%d", l_puc_EnrollmentCmd[2], l_puc_EnrollmentCmd[3]);
				if (MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
				{
					MSO_DemoDlg->m_ctl_CurrentMiniViewer = &(MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_ctl_MiniViewer_1_3);
				}
				MSO_DemoDlg->m_i_CurrentViewerIndex = 2;
				if (MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
				{
					MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_i_ViewerToColorIdx = 2;
				}
				break;
			}
			break;

		case 2:
			switch (l_puc_EnrollmentCmd[2])
			{
			case 1:
				MSO_DemoDlg->m_cs_AsyncEnrollMsg.Format("PLACE Finger #2\nCAPTURE %d/%d", l_puc_EnrollmentCmd[2], l_puc_EnrollmentCmd[3]);
				if (MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
				{
					MSO_DemoDlg->m_ctl_CurrentMiniViewer = &(MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_ctl_MiniViewer_2_1);
				}
				MSO_DemoDlg->m_i_CurrentViewerIndex = 3;
				if (MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
				{
					MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_i_ViewerToColorIdx = 3;
				}
				break;

			case 2:
				MSO_DemoDlg->m_cs_AsyncEnrollMsg.Format("Finger #2 AGAIN\nCAPTURE %d/%d", l_puc_EnrollmentCmd[2], l_puc_EnrollmentCmd[3]);
				if (MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
				{
					MSO_DemoDlg->m_ctl_CurrentMiniViewer = &(MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_ctl_MiniViewer_2_2);
				}
				MSO_DemoDlg->m_i_CurrentViewerIndex = 4;
				if (MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
				{
					MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_i_ViewerToColorIdx = 4;
				}
				break;

			case 3:
				MSO_DemoDlg->m_cs_AsyncEnrollMsg.Format("Finger #2 AGAIN\nCAPTURE %d/%d", l_puc_EnrollmentCmd[2], l_puc_EnrollmentCmd[3]);
				if (MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
				{
					MSO_DemoDlg->m_ctl_CurrentMiniViewer = &(MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_ctl_MiniViewer_2_3);
				}
				MSO_DemoDlg->m_i_CurrentViewerIndex = 5;
				if (MSO_DemoDlg->m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
				{
					MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->m_i_ViewerToColorIdx = 5;
				}
				break;
			}
			break;
		}
		MSO_DemoDlg->PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
	}

	//-----------------------------------------------------------
	// Asynchronous Images 
	//-----------------------------------------------------------

	if (i_i_command == MORPHO_CALLBACK_IMAGE_CMD)
	{
		memcpy(&l_x_ImageStructure.m_x_ImageHeader, (T_MORPHO_IMAGE_HEADER*)i_pv_param, sizeof(T_MORPHO_IMAGE_HEADER));
		l_x_ImageStructure.m_puc_Image = (PUC)i_pv_param + sizeof(T_MORPHO_IMAGE_HEADER);
		if (MSO_DemoDlg->m_ctl_CurrentMiniViewer != NULL)
		{
			l_p_Cdc = MSO_DemoDlg->m_ctl_CurrentMiniViewer->GetDC();
			MSO_DemoDlg->m_ctl_CurrentMiniViewer->GetClientRect(&myRect);

			MSO_DemoDlg->m_px_Image->SetImage(
				MSO_DemoDlg->m_i_CurrentViewerIndex,	// UL	i_ul_ImageIndex,
				l_x_ImageStructure,						// T_MORPHO_CALLBACK_IMAGE_STATUS	i_x_Image,
				0,										// UL	i_ul_imageFilter,
				l_p_Cdc,								// CDC	*i_p_Cdc,
				myRect									// CRect &i_p_Rect
				);

			MSO_DemoDlg->ReleaseDC(l_p_Cdc);

			MSO_DemoDlg->m_ctl_CurrentMiniViewer->SetImage(l_x_ImageStructure);

			MSO_DemoDlg->m_BioOpPSheet.m_pProcessPPage->PostMessage(WM_PAINT, 0, 0);
		}

		//Pas de live en mode FVP
		if (!MSO_DemoDlg->m_bl_FVP)
		{
			l_p_Cdc = MSO_DemoDlg->m_ctl_AnimationPic.GetDC();
			MSO_DemoDlg->m_ctl_AnimationPic.GetClientRect(&myRect);
			MSO_DemoDlg->m_px_Image->SetImage(
				MSO_DemoDlg->m_i_CurrentViewerIndex,	// UL	i_ul_ImageIndex,
				l_x_ImageStructure,						// T_MORPHO_CALLBACK_IMAGE_STATUS	i_x_Image,
				0,										// UL	i_ul_imageFilter,
				l_p_Cdc,								// CDC	*i_p_Cdc,
				myRect									// CRect &i_p_Rect
				);
			MSO_DemoDlg->ReleaseDC(l_p_Cdc);
		}

		MSO_DemoDlg->m_cs_ImageInfo.Format("Size:%d*%d pix, Res:%d*%d dpi, %d bits/pixels", l_x_ImageStructure.m_x_ImageHeader.m_us_NbCol, l_x_ImageStructure.m_x_ImageHeader.m_us_NbRow, l_x_ImageStructure.m_x_ImageHeader.m_us_ResX, l_x_ImageStructure.m_x_ImageHeader.m_us_ResY, l_x_ImageStructure.m_x_ImageHeader.m_uc_NbBitsPerPixel);
		MSO_DemoDlg->PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
	}

	if (i_i_command == MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD)
	{

		memcpy(&l_x_ImageStructure.m_x_ImageHeader, (T_MORPHO_IMAGE_HEADER*)i_pv_param, sizeof(T_MORPHO_IMAGE_HEADER));
		l_x_ImageStructure.m_puc_Image = (PUC)i_pv_param + sizeof(T_MORPHO_IMAGE_HEADER);

		//Pas de live en mode FVP
		if (!MSO_DemoDlg->m_bl_FVP)
		{
			l_p_Cdc = MSO_DemoDlg->m_ctl_AnimationPic.GetDC();
			MSO_DemoDlg->m_ctl_AnimationPic.GetClientRect(&myRect);
			MSO_DemoDlg->m_px_Image->SetImage(
				MSO_DemoDlg->m_i_CurrentViewerIndex,	// UL	i_ul_ImageIndex,
				l_x_ImageStructure,						// T_MORPHO_CALLBACK_IMAGE_STATUS	i_x_Image,
				0,										// UL	i_ul_imageFilter,
				l_p_Cdc,								// CDC	*i_p_Cdc,
				myRect									// CRect &i_p_Rect
				);

			MSO_DemoDlg->ReleaseDC(l_p_Cdc);
		}

		MSO_DemoDlg->m_cs_ImageInfo.Format("Size:%d*%d pix, Res:%d*%d dpi, %d bits/pixels", l_x_ImageStructure.m_x_ImageHeader.m_us_NbCol, l_x_ImageStructure.m_x_ImageHeader.m_us_NbRow, l_x_ImageStructure.m_x_ImageHeader.m_us_ResX, l_x_ImageStructure.m_x_ImageHeader.m_us_ResY, l_x_ImageStructure.m_x_ImageHeader.m_uc_NbBitsPerPixel);
		MSO_DemoDlg->PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);

	}
	//--------------------------------------------------------

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMSO_DemoDlg dialog

CMSO_DemoDlg::CMSO_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMSO_DemoDlg::IDD, pParent)
	, m_cs_animationPic(_T(""))
{
	int l_i_ThreadIndex;

	//{{AFX_DATA_INIT(CMSO_DemoDlg)
	m_cs_MaxNbRecords = _T("");

	m_tbCtrl.m_TabDbInfoDlg->m_cs_MaxNbRecords = m_cs_MaxNbRecords;

	m_tbCtrl.m_TabDbInfoDlg->m_NbFingers = _T("");

	m_CurrentNbRecords = _T("");
	m_tbCtrl.m_TabDbInfoDlg->m_CurrentNbRecords = m_CurrentNbRecords;

	m_tbCtrl.m_TabDbInfoDlg->m_cs_encryptDB = _T("");

	m_tbCtrl.m_TabBioSettingsDlg->m_i_Timeout = 0;
	m_tbCtrl.m_TabBioSettingsDlg->m_i_Threshold = 5;
	m_cs_AsyncEnrollMsg = _T("");
	m_cs_QualityMsg = _T("");
	m_cs_AsyncCmdMsg = _T("");

	m_tbCtrl.m_TabOptionsDlg->m_b_MainViewerActive = TRUE;
	m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive = TRUE;

	m_cs_animationPic = _T("");
	m_b_NoCheck = FALSE;
	m_tbCtrl.m_TabBioSettingsDlg->m_str_FFDSecuLevel = _T("Standard");

	m_tbCtrl.m_TabOptionsDlg->m_b_Export_Num_Pk = FALSE;
	m_tbCtrl.m_TabOptionsDlg->m_b_ConfigKeyUser = FALSE;
	m_tbCtrl.m_TabBioSettingsDlg->m_str_CoderChoice = _T("Default");
	m_tbCtrl.m_TabOptionsDlg->m_b_WakeUpLedOff = FALSE;

	m_tbCtrl.m_TabOptionsDlg->m_b_AsyncCodeQuality = TRUE;
	m_tbCtrl.m_TabOptionsDlg->m_b_AsyncCommand = TRUE;

	m_tbCtrl.m_TabOptionsDlg->m_b_AsyncDetectQuality = TRUE;

	m_tbCtrl.m_TabOptionsDlg->m_b_AsyncEnrollCommand = TRUE;

	m_tbCtrl.m_TabBioSettingsDlg->m_str_BioMode = _T("Default");
	m_cs_ImageInfo = _T("");

	m_tbCtrl.m_TabBioSettingsDlg->m_str_StrategyAcquisitionMode = _T("Expert(Default)");

	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_b_BiometricOperationProcessing = FALSE;
	m_ctl_CurrentMiniViewer = NULL;

	m_px_Image = NULL;

	for (l_i_ThreadIndex = 0; l_i_ThreadIndex<NB_THREADS; l_i_ThreadIndex++)
	{
		m_ppx_Threads[l_i_ThreadIndex] = NULL;
	}

	g_str_ProductDesc = "";
	g_str_SoftwareDesc = "";
	g_str_SensorDesc = "";
	m_cs_Interface = "";
	m_cs_MsoName = "";
	m_ul_TimeOut = 30;
	m_s_PortCom = NULL;
	m_i_BaudRate = 0;
	m_b_EndThread = TRUE;
	m_cs_IpAdress = "";
	m_ul_PortNumber = 0;
	m_bl_FVP = FALSE;
	m_i_SecurityLevel = MULTIMODAL_SECURITY_STANDARD;

	m_vidPlayer[0] = NULL;
	m_vidPlayer[1] = NULL;

	m_IsDataEncryption = FALSE;
	m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_DISABLED;
	m_IsPrivacyListClear = FALSE;
}

void CMSO_DemoDlg::DoDataExchange(CDataExchange* pDX)
{

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMSO_DemoDlg)
	DDX_Control(pDX, IDC_BUTTON_DESTROY_BASE, m_cmd_DestroyBase);
	DDX_Control(pDX, IDC_BUTTON_VERIF_MATCH, m_cmd_VerifMatch);
	DDX_Control(pDX, IDC_BUTTON_START, m_cmd_Start);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_USER, m_cmd_RemoveUser);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_ALL, m_cmd_RemoveAll);
	DDX_Control(pDX, IDC_BUTTON_IDENT_MATCH, m_cmd_IdentMatch);
	DDX_Control(pDX, IDC_BUTTON_CREATE_BASE, m_cmd_CreateBase);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_cmd_Close);
	DDX_Control(pDX, IDC_BUTTON_ADD_USER, m_cmd_AddUser);
	DDX_Control(pDX, IDC_STATIC_BMP_BASE, m_ctl_BmpBaseStatus);
	DDX_Control(pDX, IDC_PROGRESS_ADD_USER, m_ctl_ProgressCtrl);
	DDX_Control(pDX, IDC_STATIC_ASYNC_CMD_MSG, m_ctl_AsyncCmdMsg);
	DDX_Control(pDX, IDC_STATIC_ASYNC_ENROLL_MSG, m_ctl_AsyncEnrollMsg);
	DDX_Control(pDX, IDC_LIST_USER, m_listCtrl);
	DDX_Text(pDX, IDC_STATIC_ASYNC_ENROLL_MSG, m_cs_AsyncEnrollMsg);
	DDX_Text(pDX, IDC_STATIC_QUALITY_MSG, m_cs_QualityMsg);
	DDX_Text(pDX, IDC_STATIC_ASYNC_CMD_MSG, m_cs_AsyncCmdMsg);
	DDX_Check(pDX, IDC_CHECK_NO_CHECK, m_b_NoCheck);
	DDX_Text(pDX, IDC_IMAGE_INFO, m_cs_ImageInfo);
	DDX_Control(pDX, IDC_STATIC_ANIMATION_PIC, m_ctl_AnimationPic);
	DDX_Control(pDX, IDC_TABCTRL, m_tbCtrl);
	DDX_Control(pDX, IDC_QUALITE_PROGRESS, m_qualiteProgress);
	DDX_Control(pDX, IDC_STATIC_ASYNC_ENROLL_DIRECTION_PIC, m_ctl_AsyncEnrollDirectionPic);
	//}}AFX_DATA_MAP

	//Permet de mettre à jour les informations BaseDonnées
	if (m_tbCtrl.m_TabDbInfoDlg->m_hWnd != NULL)
	{
		m_tbCtrl.m_TabDbInfoDlg->UpdateData(0);
	}

	DDX_Text(pDX, IDC_STATIC_ANIMATION_PIC, m_cs_animationPic);
}

BEGIN_MESSAGE_MAP(CMSO_DemoDlg, CDialog)
	//{{AFX_MSG_MAP(CMSO_DemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USER, OnDblclkListUser)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEUSER, OnButtonRemoveUser)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEALL, OnButtonRemoveAllUsers)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_BASE, OnButtonCreateBase)
	ON_BN_CLICKED(IDC_BUTTON_DESTROY_BASE, OnButtonDestroyBase)
	ON_BN_CLICKED(IDC_PSHEET_AREA, OnClickOnPsheetArea)
	ON_BN_CLICKED(IDC_BUTTON_ADD_USER, OnButtonAddUser)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_WM_CLOSE()
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_BUTTON_IDENT_MATCH, OnButtonIdentMatch)
	ON_BN_CLICKED(IDC_BUTTON_VERIF_MATCH, OnButtonVerifMatch)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_USER, OnColumnclickListUser)
	ON_COMMAND(ID_LOAD_PKCS12, OnLoadPkcs12)
	ON_COMMAND(ID_READ_CERTIFICAT, OnReadCertificat)
	ON_COMMAND(ID_READ_CONFIGURATION, OnReadConfiguration)
	ON_COMMAND(ID_STORE_CERTIFICAT, OnStoreCertificat)
	ON_COMMAND(ID_VERIF_SIGN_X984, OnVerifSignX984)
	ON_COMMAND(ID_EXTRACT_SIGNER_ID, OnExtractSignerId)
	ON_COMMAND(ID_LOAD_MASTER_KEY, OnLoadMasterKey)
	ON_COMMAND(ID_LOAD_MOC_KENC, OnLoadMocKEnc)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_USER, OnButtonUpdateUser)
	ON_MESSAGE(WM_UPDATE_DATA_FALSE, OnUpdateDataFalse)
	ON_MESSAGE(WM_UPDATE_DATA_TRUE, OnUpdateDataTrue)
	ON_MESSAGE(WM_ADD_PROCESS_PAGE, OnAddProcessPage)
	ON_MESSAGE(WM_REMOVE_PROCESS_PAGE, OnRemoveProcessPage)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_UNCOMPRESS_IMAGE, OnUncompressImage)
	ON_COMMAND(ID_FVP_MMI_FILE_LOAD, MMIFileLoad)
	ON_COMMAND(ID_FVP_MMI_FILE_SAVEAS, MMIFileSaveAs)
	ON_COMMAND(ID_FVP_MMI_FILE_RESET, MMIFileReset)
	ON_COMMAND(ID_CONVERT_ISO19794_FIR, OnConvertISO19794_FIR)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_DISPLAY_MOC_KENC_KCV, &CMSO_DemoDlg::OnDisplayMocKencKcv)
	ON_BN_CLICKED(IDC_REBOOTSOFT, &CMSO_DemoDlg::OnBnClickedRebootsoft)
	ON_COMMAND(ID_SECURITY_LOAD_KS, OnLoadKs)
	ON_COMMAND(ID_SECURITY_LOAD_KS_ASYMMSEC, OnLoadKsAsymmSec)
	ON_COMMAND(ID_SECURITY_LOAD_KS_SYMMSEC, OnLoadKsSymmSec)
	ON_COMMAND(ID_SECURITY_DISPLAY_KS_KCV, OnDisplayKsKcv)
	ON_COMMAND(ID_SECURITY_DISPLAY_KPRIVACY_KCV, OnDisplayKprivacyKcv)
	ON_COMMAND(ID_SECURITY_ENABLE_BIO_DATA_ENCRYPTION, OnEnableBioDataEncryption)
	ON_COMMAND(ID_SECURITY_DECRYPT_BIOMETRIC_DATA, OnDecryptBiometricData)
	ON_COMMAND(ID_SECURITY_ENCRYPT_AES_DATA, OnEncryptAESData)
	ON_COMMAND(ID_SECURITY_DECRYPT_AES_DATA, OnDecryptAESData)
	//ON_COMMAND(ID_SECURITY_PRIVACY_MODE, OnPrivacyMode)
	ON_COMMAND(ID_SECURITY_LOAD_KPRIVACY, OnLoadKprivacy)
	ON_COMMAND(ID_SECURITY_LOAD_KPRIVACY_SYMMSEC, OnLoadKprivacySymmSec)
	ON_COMMAND(ID_SECURITY_PRIVACY_MODE_Enable_NoOperationDB, OnPrivacyMode_Enable_NoOperationDB)
	ON_COMMAND(ID_SECURITY_PRIVACY_MODE_Enable_EraseDB, OnPrivacyMode_Enable_EraseDB)
	ON_COMMAND(ID_SECURITY_PRIVACY_MODE_Enable_CipherDB, OnPrivacyMode_Enable_CipherDB)
	ON_COMMAND(ID_SECURITY_PRIVACY_MODE_Disable_NoOperationDB, OnPrivacyMode_Disable_NoOperationDB)
	ON_COMMAND(ID_SECURITY_PRIVACY_MODE_Disable_EraseDB, OnPrivacyMode_Disable_EraseDB)
	ON_COMMAND(ID_SECURITY_PRIVACY_MODE_Disable_CipherDB, OnPrivacyMode_Disable_CipherDB)
	ON_COMMAND(ID_SECURITY_PRIVACY_MODE_PartialEnable_NoOperationDB, OnPrivacyMode_PartialEnable_NoOperationDB)
	ON_COMMAND(ID_SECURITY_PRIVACY_MODE_PartialEnable_EraseDB, OnPrivacyMode_PartialEnable_EraseDB)
	ON_COMMAND(ID_SECURITY_PRIVACY_MODE_PartialEnable_CipherDB, OnPrivacyMode_PartialEnable_CipherDB)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CreateEmbeddedPropertySheet

BOOL CreateEmbeddedPropertySheet(CWnd* pParent, CPropertySheet* pPSheet,
	DWORD dwStyle, DWORD dwExStyle)
{
	ASSERT_VALID(pParent);
	ASSERT_VALID(pPSheet);

	// 1 - Create the embedded property sheet window
	if (!pPSheet->Create(pParent, dwStyle, dwExStyle))
	{
		TRACE0("Embedded property sheet creation failed\n");
		return FALSE;
	}

	// 2 - Add WS_TABSTOP and WS_EX_CONTROLPARENT to the property sheet styles
	pPSheet->ModifyStyle(0, WS_TABSTOP);
	pPSheet->ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	// 3 - Add WS_EX_CONTROLPARENT to the parent window styles
	pParent->ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// PositionEmbeddedPropertySheet

void PositionEmbeddedPropertySheet(CWnd* pParent, CPropertySheet* pPSheet,
	CRect rcNewPosition)
{
	ASSERT_VALID(pParent);
	ASSERT_VALID(pPSheet);

	// 1 - Get current coordinates of tab control
	// and property sheet window
	CTabCtrl* pTabCtrl = pPSheet->GetTabControl();
	ASSERT(pTabCtrl != NULL);

	CRect rcTabCtrl;
	pTabCtrl->GetWindowRect(&rcTabCtrl);
	pParent->ScreenToClient(&rcTabCtrl);

	CRect rcPSheet;
	pPSheet->GetWindowRect(&rcPSheet);
	pParent->ScreenToClient(&rcPSheet);

	// 2 - Calculate margin between property sheet
	// and tab control
	int dcx = rcPSheet.Width() - rcTabCtrl.Width();
	int dcy = rcPSheet.Height() - rcTabCtrl.Height();

	// 3 - Move and resize property sheet window
	// (also moves the tab window because it is a child
	// of the property sheet window)
	pPSheet->MoveWindow(rcNewPosition.left, rcNewPosition.top,
		rcNewPosition.Width(), rcNewPosition.Height());

	// 4 - Resize tab control window to restore
	// right / bottom margins
	pTabCtrl->SetWindowPos(NULL,
		0, 0,
		rcNewPosition.Width() - dcx,
		rcNewPosition.Height() - dcy,
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);

	// 5 - Activate each property page to prevent drawing
	// problem
	int nCurrentPage = pPSheet->GetActiveIndex();
	for (int i = 0; i < pPSheet->GetPageCount(); ++i)
	{
		pPSheet->SetActivePage(i);
	}

	pPSheet->SetActivePage(nCurrentPage);
}

/////////////////////////////////////////////////////////////////////////////
void PositionEmbeddedPropertySheet(CWnd* pParent, CPropertySheet* pPSheet,
	UINT nIDPSheetArea)
{
	ASSERT_VALID(pParent);
	ASSERT_VALID(pPSheet);

	// 1 - Retrieve property sheet destination position
	CRect rcNewPosition;
	CWnd* pWndNewArea = pParent->GetDlgItem(nIDPSheetArea);
	if (pWndNewArea == NULL)
	{
		ASSERT(FALSE);	// Invalid nIDPSheetArea
		return;
	}

	pWndNewArea->GetWindowRect(&rcNewPosition);
	pParent->ScreenToClient(&rcNewPosition);

	// 2 - Call overloaded function
	PositionEmbeddedPropertySheet(pParent, pPSheet, rcNewPosition);
}

/////////////////////////////////////////////////////////////////////////////
// CMSO_DemoDlg message handlers
int FindMenuItem(CMenu* Menu, LPCTSTR MenuString)
{
	ASSERT(Menu);
	ASSERT(::IsMenu(Menu->GetSafeHmenu()));

	int count = Menu->GetMenuItemCount();
	for (int ii = 0; ii < count; ii++)
	{
		CString str;
		if (Menu->GetMenuString(ii, str, MF_BYPOSITION) &&
			str.Compare(MenuString) == 0)
			return ii;
	}

	return -1;
}
BOOL CMSO_DemoDlg::OnInitDialog()
{
	CString l_cs_Message;
	I		l_i_Ret;
	UL		l_ul_nbUsedRecord;
	UL		l_ul_sizeDatabase;
	UC		l_uc_NbFinger;
	UL		l_ul_NbField;
	T_MORPHO_FIELD_ATTRIBUTE	l_uc_fieldAttribute;
	US							l_us_fieldMaxSize;
	UC							l_auc_fieldName[MORPHO_FIELD_NAME_LEN];
	UL							l_ul_fieldIndex;

	UC							l_auc_SecuSerialNumber[MSO_SERIAL_NUMBER_LEN + 1];
	T_MORPHO_FAR				l_x_SecuFar;

	I							l_i_BaudRate;

	C		l_pc_inifilename[MAX_PATH];
	C		l_pc_ffdlogfilename[MAX_PATH];
	FILE	*l_x_dummyfile;
	C		*l_pc_balladeur;


	CDialog::OnInitDialog();
	CProgressInit *ProgressInit;

	ProgressInit = new CProgressInit(this);
	ProgressInit->CenterWindow();
	ProgressInit->ShowWindow(SW_SHOW);
	ProgressInit->SetIcon(m_hIcon, FALSE);

	ProgressInit->m_cs_MainMess = "MSO analyse, please wait..............";

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	l_uc_image = 0;
	m_puc_MsoCertif = NULL;
	m_ul_MsoCertifSize = 0;
	m_tbCtrl.m_TabBioSettingsDlg->m_i_CoderChoice = MORPHO_DEFAULT_CODER;
	m_tbCtrl.m_TabBioSettingsDlg->m_i_MatchingStrategy = MORPHO_STANDARD_MATCHING_STRATEGY;
	m_tbCtrl.m_TabBioSettingsDlg->m_uc_StrategyAcquisitionMode = MORPHO_ACQ_EXPERT_MODE;
	m_ul_WakeUpMode = 0;

	m_ul_CallbackMask = MORPHO_CALLBACK_IMAGE_CMD |
		MORPHO_CALLBACK_ENROLLMENT_CMD |
		MORPHO_CALLBACK_COMMAND_CMD |
		MORPHO_CALLBACK_CODEQUALITY |
		MORPHO_CALLBACK_DETECTQUALITY; //| 
	//MORPHO_CALLBACK_LAST_IMAGE_FULL_RES_CMD;


	/////


	/*tab*************************************************************/
	m_tbCtrl.InitDialogs();
	m_tbCtrl.InsertItem(0, "Database Information");
	m_tbCtrl.InsertItem(1, "Biometric settings");
	m_tbCtrl.InsertItem(3, "Options");
	m_tbCtrl.ActivateTabDialogs();
	m_tbCtrl.m_TabOptionsDlg->m_x_pMorphoSmart = &m_x_MorphoSmart;
	m_tbCtrl.m_TabBioSettingsDlg->m_px_pMorphoSmart = &m_x_MorphoSmart;
	//Option tab variables
	m_tbCtrl.m_TabOptionsDlg->m_b_MainViewerActive = TRUE;
	m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive = TRUE;
	m_tbCtrl.m_TabOptionsDlg->m_pb_AsyncCommand = &m_b_AsyncCommand;
	m_tbCtrl.m_TabOptionsDlg->m_pb_AsyncEnrollCommand = &m_b_AsyncEnrollCommand;
	m_tbCtrl.m_TabOptionsDlg->m_pb_AsyncDetectQuality = &m_b_AsyncDetectQuality;
	m_tbCtrl.m_TabOptionsDlg->m_pb_AsyncCodeQuality = &m_b_AsyncCodeQuality;
	m_tbCtrl.m_TabOptionsDlg->m_b_Export_Num_Pk = FALSE;
	m_tbCtrl.m_TabOptionsDlg->m_pb_WakeUpLedOff = &m_b_WakeUpLedOff;
	m_tbCtrl.m_TabOptionsDlg->m_pul_WakeUpMode = &m_ul_WakeUpMode;
	m_tbCtrl.m_TabOptionsDlg->m_pul_CallbackMask = &m_ul_CallbackMask;
	m_tbCtrl.m_TabOptionsDlg->m_pcw_Mso_DemoDlg = this;
	/*tab*************************************************************/

	m_qualiteProgress.SetRange(LIVE_QUALITY_MIN, LIVE_QUALITY_MAX);
	m_qualiteProgress.ShowWindow(SWP_HIDEWINDOW);


	//*************************************************************
	// Morpho Logo Transparent	
	m_Static.SubclassDlgItem(IDC_BITMAP_LOGO_SAGEM, this);
	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_BITMAP_LOGO_SAGEM_BLUE), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);
	hBmp = m_Static.SetBitmap(hBmp);
	//*************************************************************

	//Result and direction icons 
	m_hBitmap_Ok = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP_OK));
	m_hBitmap_Ko = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP_KO));
	m_hBitmap_Up = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_UP_BMP));
	m_hBitmap_Right = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_RIGHT_BMP));
	m_hBitmap_Down = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_DOWN_BMP));
	m_hBitmap_Left = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LEFT_BMP));

	((CStatic*)(GetDlgItem(IDC_STATIC_ASYNC_ENROLL_DIRECTION_PIC)))->ModifyStyle(0, SS_CENTERIMAGE | SS_BITMAP);

	m_tbCtrl.m_TabBioSettingsDlg->m_cmd_ForceFingerPlacement = TRUE; //Activée par défaut

	m_tbCtrl.m_TabBioSettingsDlg->m_i_EditFingerPrintQuality.SetWindowText("-");


	/************************************************/


	// Tool Tips

	m_ToolTips.Create(this);
	m_ToolTips.AddTool(&m_ctl_ProgressCtrl, IDS_TT_PROGRESS_CTRL);
	//m_ToolTips.AddTool(&(m_tbCtrl.m_TabOptionsDlg->m_cmd_MainViewerCheckBox), IDS_TT_VIEWER_CHECKBOX);
	//m_ToolTips.AddTool(&(m_tbCtrl.m_TabOptionsDlg->m_cmd_MiniViewerCheckBox), IDS_TT_MINI_VIEWERCHECKBOX);
	m_ToolTips.AddTool(&m_cmd_IdentMatch, IDS_TT_BUTTON_IDENT_MATCH);
	m_ToolTips.AddTool(&m_cmd_VerifMatch, IDS_TT_BUTTON_VERIF_MATCH);
	m_ToolTips.AddTool(&m_cmd_RemoveUser, IDS_TT_BUTTON_REMOVE_USER);
	m_ToolTips.AddTool(&m_cmd_RemoveAll, IDS_TT_BUTTON_REMOVE_ALL);
	m_ToolTips.AddTool(&m_cmd_AddUser, IDS_TT_BUTTON_ADD_USER);
	m_ToolTips.AddTool(&m_cmd_CreateBase, IDS_TT_BUTTON_CREATE_BASE);
	m_ToolTips.AddTool(&m_cmd_DestroyBase, IDS_TT_BUTTON_DESTROY_BASE);
	m_ToolTips.AddTool(&m_cmd_Start, IDS_TT_BUTTON_START);
	m_ToolTips.AddTool(&m_cmd_Close, IDS_TT_BUTTON_CLOSE);
	m_ToolTips.Activate(TRUE);

	//*************************************************************
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	m_menu.LoadMenu(IDR_MENU_SECU);

	SetMenu(&m_menu);

	//*************************************************************
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//**************************************************
	//

	//---------------------------------------------
	// Create Property Sheet
	//---------------------------------------------
	CreateEmbeddedPropertySheet(this, &m_BioOpPSheet);
	PositionEmbeddedPropertySheet(this, &m_BioOpPSheet, IDC_PSHEET_AREA);

	//---------------------------------------------
	// Fill the Output File Capture Combo Box 
	//---------------------------------------------

	CString l_cs_string;

	m_BioOpPSheet.m_EnrollPPage.m_ctrl_updateFingerIndex.ResetContent();
	l_cs_string.Format("%s", STR_FIRST_FINGER);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_updateFingerIndex.AddString(l_cs_string);
	l_cs_string.Format("%s", STR_SECOND_FINGER);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_updateFingerIndex.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_updateFingerIndex.SelectString(0, STR_FIRST_FINGER);

	//Template Type
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.ResetContent();
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.ResetContent();
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.ResetContent();
	//m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.SetHorizontalExtent(WS_HSCROLL );
	l_cs_string.Format("%s", STR_PKCOMP);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_PKCOMPNORM);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_PKMAT);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_PKMATNORM);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_PKLITE);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_PKS);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ANSI378);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ANSI378_2009);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ISO_FMR);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ISO_FMR_2011);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ISO_FMC_NS);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ISO_FMC_CS);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_MINEX_A);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_PK_MOC);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_PK_DIN_V66400_CS);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_PK_DIN_V66400_CS_AA);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ISO_FMC_CS_AA);
	InsertText(&m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType, l_cs_string);
	//m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);
	//m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	InsertText(&m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType, l_cs_string);
	//m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	InsertText(&m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType, l_cs_string);

	l_cs_string.Format("%s", STR_NO_PK_FP);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);

	m_BioOpPSheet.m_VerifyPPage.m_ctrl_TemplateType.SelectString(0, STR_PKCOMP);

	m_BioOpPSheet.m_EnrollPPage.m_ctrl_TemplateType.SelectString(0, STR_NO_PK_FP);
	//m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.	

	// Image compression
	m_BioOpPSheet.m_EnrollPPage.m_cmd_CompressionType.ResetContent();

	l_cs_string.Format("%s", STR_NO_IMAGE);
	m_BioOpPSheet.m_EnrollPPage.m_cmd_CompressionType.AddString(l_cs_string);

	l_cs_string.Format("%s", "RAW");
	m_BioOpPSheet.m_EnrollPPage.m_cmd_CompressionType.AddString(l_cs_string);

	l_cs_string.Format("%s", "SAGEM_V1");
	m_BioOpPSheet.m_EnrollPPage.m_cmd_CompressionType.AddString(l_cs_string);

	l_cs_string.Format("%s", "WSQ");
	m_BioOpPSheet.m_EnrollPPage.m_cmd_CompressionType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ISO19794_FIR_RAW);
	m_BioOpPSheet.m_EnrollPPage.m_cmd_CompressionType.AddString(l_cs_string);
	m_BioOpPSheet.m_GetImagePPage.m_cmd_CompressionType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ISO19794_FIR_BMP);
	m_BioOpPSheet.m_EnrollPPage.m_cmd_CompressionType.AddString(l_cs_string);
	m_BioOpPSheet.m_GetImagePPage.m_cmd_CompressionType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ISO19794_FIR_WSQ);
	m_BioOpPSheet.m_EnrollPPage.m_cmd_CompressionType.AddString(l_cs_string);
	m_BioOpPSheet.m_GetImagePPage.m_cmd_CompressionType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ISO19794_FIR_JPEG);
	m_BioOpPSheet.m_EnrollPPage.m_cmd_CompressionType.AddString(l_cs_string);
	m_BioOpPSheet.m_GetImagePPage.m_cmd_CompressionType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ISO19794_FIR_JPEG2000);
	m_BioOpPSheet.m_EnrollPPage.m_cmd_CompressionType.AddString(l_cs_string);
	m_BioOpPSheet.m_GetImagePPage.m_cmd_CompressionType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_ISO19794_FIR_PNG);
	InsertText(&m_BioOpPSheet.m_EnrollPPage.m_cmd_CompressionType, l_cs_string);
	InsertText(&m_BioOpPSheet.m_GetImagePPage.m_cmd_CompressionType, l_cs_string);

	m_BioOpPSheet.m_EnrollPPage.m_cmd_CompressionType.SelectString(0, STR_NO_IMAGE);

	//--------------------------
	//FVP
	//--------------------------
	m_BioOpPSheet.m_CodePPage.m_ctrl_FVP_TemplateType.ResetContent();
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_FVP_TemplateType.ResetContent();

	l_cs_string.Format("%s", STR_PK_FVP);
	m_BioOpPSheet.m_CodePPage.m_ctrl_FVP_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_FVP_TemplateType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_NO_FVP);
	m_BioOpPSheet.m_CodePPage.m_ctrl_FVP_TemplateType.AddString(l_cs_string);
	m_BioOpPSheet.m_EnrollPPage.m_ctrl_FVP_TemplateType.AddString(l_cs_string);

	//--------------------------
	//end FVP section
	//--------------------------

	//Enroll
	m_BioOpPSheet.m_EnrollPPage.m_b_SavePkInDB = TRUE;

	// WorkFlow Card Type
	m_BioOpPSheet.m_CodePPage.m_ctrl_WorkflowType.ResetContent();
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_WorkflowType.ResetContent();

	l_cs_string.Format("%s", STR_WORKFLOWTYPE_SAGEM_OPUCE);
	m_BioOpPSheet.m_CodePPage.m_ctrl_WorkflowType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_WorkflowType.AddString(l_cs_string);

	l_cs_string.Format("%s", STR_WORKFLOWTYPE_CLIENT_GB);
	m_BioOpPSheet.m_CodePPage.m_ctrl_WorkflowType.AddString(l_cs_string);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_WorkflowType.AddString(l_cs_string);

	m_BioOpPSheet.m_CodePPage.m_ctrl_WorkflowType.SelectString(0, STR_WORKFLOWTYPE_SAGEM_OPUCE);
	m_BioOpPSheet.m_VerifyPPage.m_ctrl_WorkflowType.SelectString(0, STR_WORKFLOWTYPE_SAGEM_OPUCE);

	//---------------------------------------------
	// Big Font for asynchronous messages 
	//---------------------------------------------

	CRect	rRect;
	int	iHeight;

	m_ctl_AsyncCmdMsg.GetWindowRect(&rRect);

	iHeight = rRect.top - rRect.bottom;
	if (iHeight < 0) iHeight = 20;
	m_fFont.Detach();
	m_fFont.CreateFont((iHeight - 7), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "");

	m_ctl_AsyncCmdMsg.SetFont(&m_fFont);
	m_ctl_AsyncEnrollMsg.SetFont(&m_fFont);
	m_ctl_AnimationPic.SetFont(&m_fFont);

	UpdateData(FALSE);

	SetTitleBar(m_hWnd);

	//---------------------------------------------
	// Open Com
	//---------------------------------------------

	CString l_cs_Interface;
	S		l_s_PortCom;

	l_cs_Interface = APP->GetProfileString(COM_PARAMETERS, COM_INTERFACE, DEFAULT_COM_INTERFACE);
	m_cs_Interface = l_cs_Interface;

	if (l_cs_Interface == COM_USB)
	{
		ProgressInit->m_cs_Mess.Format("Open %s", l_cs_Interface);
		ProgressInit->UpdateData(FALSE);

		m_cs_MsoName = APP->GetProfileString(COM_PARAMETERS, SERIALNUMBER, DEFAULT_SERIALNUMBER);

		if (m_cs_MsoName == "")
		{
			l_i_Ret = m_x_MorphoSmart.OpenDevice(-1);
		}
		else
		{
			l_i_Ret = m_x_MorphoSmart.OpenUsbDevice(
				(PC)(LPCTSTR)m_cs_MsoName,	//PC								i_pc_MsoName,
				30,							//UL								i_ul_TimeOut,
				//Wait time out for USB device connection
				NULL						//PVOID								io_pv_param
				);
		}

		if (l_i_Ret == MORPHOERR_NO_SERVER)
		{
			l_cs_Message.Format("Server stopped or not installed.\nPlease start or install the SAGEM MorphoSmart Service Provider Usb Server");
			AfxMessageBox(l_cs_Message);
			CDialog::OnCancel();
			delete(ProgressInit);
			return FALSE;
		}

		if ((l_i_Ret != MORPHO_OK) && (l_i_Ret != MORPHOERR_NO_SERVER))
		{
			l_cs_Message.Format("The connection with the MSO failed. Please\ncheck the device is correctly plugged");
			AfxMessageBox(l_cs_Message);
			CDialog::OnCancel();
			delete(ProgressInit);
			return FALSE;
		}
	}
	else if (l_cs_Interface == COM_DEMO_PIPE)
	{
		T_CLIENTPIPE_CALLBACKCOMOPEN_PARAMETERS l_x_OpenParameters;

		m_cs_IpAdress = APP->GetProfileString(COM_PARAMETERS, IP_ADDRESS, DEFAULT_IP_ADDRESS);
		m_ul_PortNumber = APP->GetProfileInt(COM_PARAMETERS, IP_PORT, DEFAULT_IP_PORT);
		m_cs_MsoName = APP->GetProfileString(COM_PARAMETERS, SERIALNUMBER, DEFAULT_SERIALNUMBER);

		ProgressInit->m_cs_Mess.Format("Open Pipe %s %d", m_cs_IpAdress, m_ul_PortNumber);
		ProgressInit->UpdateData(FALSE);

		l_x_OpenParameters.m_ul_TimeOut = 10000;
		l_x_OpenParameters.m_ul_MSOSerialNumberLen = m_cs_MsoName.GetLength();
		if (l_x_OpenParameters.m_ul_MSOSerialNumberLen >= 0)
		{
			l_x_OpenParameters.m_ul_MSOSerialNumberLen++; // To Add '\0'
		}

		strncpy_s(
			l_x_OpenParameters.m_auc_MSOSerialNumber,
			sizeof(l_x_OpenParameters.m_auc_MSOSerialNumber),
			m_cs_MsoName,
			l_x_OpenParameters.m_ul_MSOSerialNumberLen
			);

		l_i_Ret = m_x_MorphoSmart.OpenDevice(
			ClientPipe_CallbackComOpen,
			ClientPipe_CallbackComSend,
			ClientPipe_CallbackComReceive,
			ClientPipe_CallbackComReceiveFree,
			ClientPipe_CallbackComClose,
			(PC)(LPCTSTR)m_cs_IpAdress,
			m_ul_PortNumber,
			(PVOID)&l_x_OpenParameters,	// Time Out for Pipe connection
			C_MORPHO_Device::TCP_COM_TYPE
			);

		if (l_i_Ret != MORPHO_OK)
		{
			l_cs_Message.Format("The connection with the MSO failed.");
			AfxMessageBox(l_cs_Message);
			CDialog::OnCancel();
			delete(ProgressInit);
			return FALSE;
		}
	}
	else
	{
		l_i_BaudRate = APP->GetProfileInt(COM_PARAMETERS, BAUD_RATE, DEFAULT_BAUD_RATE);
		CString strComName;
		strComName = APP->GetProfileString(COM_PARAMETERS, PORT_NAME);
		m_i_BaudRate = l_i_BaudRate;
		m_s_PortCom = l_s_PortCom;

		ProgressInit->m_cs_Mess.Format("Open %s COM%d", l_cs_Interface, l_s_PortCom);
		ProgressInit->UpdateData(FALSE);

		//l_i_Ret = m_x_MorphoSmart.OpenDevice(l_s_PortCom,l_i_BaudRate);
		l_i_Ret = m_x_MorphoSmart.OpenDevice((LPTSTR)(LPCTSTR)strComName, l_i_BaudRate);

		if (l_i_Ret != MORPHO_OK)
		{
			l_cs_Message.Format("The connection with the MSO failed. Please\ncheck the device is correctly plugged on COM%d", l_s_PortCom);
			AfxMessageBox(l_cs_Message);
			CDialog::OnCancel();
			delete(ProgressInit);
			return FALSE;
		}
	}
	//---------------------------------------------
	// Setup security
	//---------------------------------------------

	ProgressInit->m_cs_Mess10 = ": OK";
	ProgressInit->m_cs_Mess2.Format("MSO configuration");
	ProgressInit->UpdateData(FALSE);

	l_i_Ret = m_x_MorphoSmart.GetSecuConfig(
		l_auc_SecuSerialNumber,		//UC			o_auc_SecuSerialNumber[MSO_SERIAL_NUMBER_LEN],
		&m_uc_SecuConfig,			//PUC			o_puc_SecuConfig
		&l_x_SecuFar				//PT_MORPHO_FAR	o_px_SecuFar
		);

	if (l_i_Ret == MORPHO_OK)
	{
		if (m_uc_SecuConfig&SECU_TUNNELING)
		{
			ProgressInit->m_cs_Mess11 = ": Tunneling";
			ProgressInit->m_cs_Mess3.Format("Host Certification");
			ProgressInit->UpdateData(FALSE);

			l_i_Ret = MSOSECU_GetHostCertif(
				&m_ul_CertifHostSize,	//PUL			o_pul_CertifHostSize,
				&m_puc_CertifHost		//PUC*		o_ppuc_CertifHost
				);

			if (l_i_Ret)
			{
				l_cs_Message.Format("Cannot read host certificate (error %d).\nPlease check that:\n1/ You have generated the HOST keys (with the MKMS or other)\n2/ You have copied host.der (HOST public certificate), host.key (HOST private key), ca.crt (CA public key that can be found in the MKMS station) in the directory .\\openssl\\keys.", l_i_Ret);
				AfxMessageBox(l_cs_Message, MB_ICONERROR);
				OnCancel();
				delete(ProgressInit);
				return FALSE;
			}

			ProgressInit->m_cs_Mess12 = ": OK";
			ProgressInit->m_cs_Mess4.Format("Tunneling communication initialisation");
			ProgressInit->m_cs_Mess13.Format(": please wait a few seconds...");
			ProgressInit->UpdateData(FALSE);

			l_i_Ret = m_x_MorphoSmart.TunnelingOpen(
				MSOSECU_TripleDesSign,		//T_MORPHO_CALLBACK_TRIPLE_DES_SIGN		i_pf_CallbackTripleDesSign,
				MSOSECU_TripleDesCrypt,		//T_MORPHO_CALLBACK_TRIPLE_DES_CRYPT		i_pf_CallbackTripleDesCrypt,
				MSOSECU_TripleDesVerifSign,	//T_MORPHO_CALLBACK_TRIPLE_DES_VERIF_SIGN	i_pf_CallbackTripleDesVerifSign,
				MSOSECU_TripleDesDecrypt,	//T_MORPHO_CALLBACK_TRIPLE_DES_DECRYPT	i_pf_CallbackTripleDesDecrypt,
				MSOSECU_TunnelingInit1,		//T_MORPHO_CALLBACK_TUNNELING_INIT1		i_pf_CallbackTunnelingInit1,
				MSOSECU_TunnelingInit2,		//T_MORPHO_CALLBACK_TUNNELING_INIT2		i_pf_CallbackTunnelingInit2,
				m_ul_CertifHostSize,		//UL										i_ul_HostCertificateSize,
				m_puc_CertifHost			//PUC										i_puc_HostCertificate
				);

			if (l_i_Ret)
			{
				ShowErrorBox("Security Tunneling Initialization failed.\nPlease check that:\n1/ You have generated the HOST keys (with the MKMS or other)\n2/ You have copied host.der (HOST public certificate), host.key (HOST private key), ca.crt (CA public key that can be found in the MKMS station) in the directory .\\openssl\\keys.", l_i_Ret);
				OnCancel();
				delete(ProgressInit);
				return FALSE;
			}
			ProgressInit->m_cs_Mess13 = ": OK";
			ProgressInit->UpdateData(FALSE);

			if (findSagemSecurite_Licence_Protection())
			{
				if (AfxMessageBox("The service \"Sagem Licence Service\" is not compatible with products with Tunneling configuration , it may cause problems in the enrollment process. Please stop this service before using MSO_Demo.\n\nContinue anyway ?", MB_YESNO | MB_ICONERROR) == IDNO)
				{
					OnCancel();
					delete(ProgressInit);
					return FALSE;
				}
			}
		}
		else if (m_uc_SecuConfig&SECU_OFFERED_SECURITY)
		{
			ProgressInit->m_cs_Mess11 = ": Offered";
			ProgressInit->UpdateData(FALSE);

			ProgressInit->m_cs_Mess3.Format("The MSO Certification");
			ProgressInit->m_cs_Mess4.Format("Offered communication initialisation");
			ProgressInit->UpdateData(FALSE);

			l_i_Ret = m_x_MorphoSmart.OfferedSecuOpen(
				MSOSECU_GenRandom,						//T_MORPHO_CALLBACK_RAND					i_pf_CallbackGenRandom,
				MSOSECU_VerifOfferedSecuritySignature,	//T_MORPHO_CALLBACK_VERIF_OFFERED_SECU_SIGN	i_pf_CallbackVerifOfferedSecuritySignature
				MSOSECU_VerifCertif						//T_MORPHO_CALLBACK_VERIF_CERTIF			i_pf_CallbackVerifCertif
				);

			if (l_i_Ret)
			{
				ShowErrorBox("Offered Security Initialization failed", l_i_Ret);
				if (AfxMessageBox("The MorphoSmart Certificate has not been authenticated.\nPlease check that you have copied ca.crt (CA public key that can be found in the MKMS station) in the directory .\\openssl\\keys.\nContinue anyway ?", MB_YESNO) == IDYES)
				{
					l_i_Ret = MORPHO_OK;
				}
				else
				{
					OnCancel();
					delete(ProgressInit);
					return FALSE;
				}
			}

			ProgressInit->m_cs_Mess12 = ": OK";
			ProgressInit->m_cs_Mess13 = ": OK";
			ProgressInit->UpdateData(FALSE);
		}
		else
		{
			ProgressInit->m_cs_Mess11.Format("The communication is not secured");
			ProgressInit->UpdateData(FALSE);
		}

		/* If device is FFD capable */
		//if(m_uc_SecuConfig&SECU_FFD)
		{
			/* Build ini file name */
			GetModuleFileName(NULL, l_pc_inifilename, MAX_PATH);
			l_pc_balladeur = l_pc_inifilename + strlen(l_pc_inifilename) - 1;

			while (*l_pc_balladeur != '.')
				l_pc_balladeur--;
			strcpy_s(l_pc_balladeur, 5, ".ini");

			/* If [Setup] / LogFilePath exists in ini file */
			if (GetPrivateProfileString("Setup",
				"LogFilePath",
				"",
				l_pc_ffdlogfilename,
				MAX_PATH - 15 - MSO_SERIAL_NUMBER_LEN,
				l_pc_inifilename) != 0)
			{

				/* Build log file full name */
				if (l_pc_ffdlogfilename[strlen(l_pc_ffdlogfilename) - 1] == '\\')
				{
					sprintf_s(l_pc_ffdlogfilename + strlen(l_pc_ffdlogfilename),
						15 + MSO_SERIAL_NUMBER_LEN,
						"MSO_%s_Audit.log",
						(char*)(&l_auc_SecuSerialNumber[0]));
				}
				else
				{
					sprintf_s(l_pc_ffdlogfilename + strlen(l_pc_ffdlogfilename),
						16 + MSO_SERIAL_NUMBER_LEN,
						"\\MSO_%s_Audit.log",
						(char*)(&l_auc_SecuSerialNumber[0]));
				}

				/* If the log file does not exist */
				fopen_s(&l_x_dummyfile, l_pc_ffdlogfilename, "rt");
				if (l_x_dummyfile == NULL)
				{
					/* Try to create it */
					fopen_s(&l_x_dummyfile, l_pc_ffdlogfilename, "wt");
					if (l_x_dummyfile != NULL)
					{
						fclose(l_x_dummyfile);
					}
				}
				else
				{
					fclose(l_x_dummyfile);
				}

				/* If the log file exists */
				fopen_s(&l_x_dummyfile, l_pc_ffdlogfilename, "rt");
				if (l_x_dummyfile != NULL)
				{
					fclose(l_x_dummyfile);
					/* Update log file name */
					g_puc_LogFileName = (PUC)malloc(MAX_PATH);
					strcpy_s((char*)g_puc_LogFileName,
						strlen(l_pc_ffdlogfilename) + 1,
						l_pc_ffdlogfilename);
				}
			}
		}

		if (m_uc_SecuConfig)
		{
			// device is secure: loading Ks using unsecure mode will be forbidden
			GetMenu()->EnableMenuItem(ID_SECURITY_LOAD_KS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		else
		{
			// device is not secure: Ks cannot be loaded using asymmetric secure mode
			GetMenu()->EnableMenuItem(ID_SECURITY_LOAD_KS_ASYMMSEC, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
	}

	CMenu *cMainMenu = GetMenu()->GetSubMenu(0);
	int SubMenuPosition = FindMenuItem(cMainMenu, _T("Privacy Mode"));
	CMenu *CMenuPrivacyMode = cMainMenu->GetSubMenu(SubMenuPosition);
	// Get Privacy mode status
	C_MORPHO_Device::T_DEVICE_PRIVACY_MODE_STATUS status = C_MORPHO_Device::PRIVACY_MODE_DISABLED;
	l_i_Ret = m_x_MorphoSmart.GetPrivacyModeStatus(status);

	if (MORPHO_OK == l_i_Ret)
	{
		switch (status)
		{
		case C_MORPHO_Device::PRIVACY_MODE_ENABLED:
			m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_ENABLED;
			SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Privacy Mode"));
			CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

			SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Partial Privacy Mode"));
			CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

			break;
		case C_MORPHO_Device::PRIVACY_MODE_DISABLED:
			// Privacy mode is enabled on device : mark button as checked
			m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_DISABLED;
			SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Disable Privacy Mode"));
			CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);
			break;
		case C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED:
			// Privacy mode is enabled on device : mark button as checked
			m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED;
			SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Privacy Mode"));
			CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

			SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Partial Privacy Mode"));
			CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

			break;
		}
	}
	else if (MORPHOERR_UNAVAILABLE == l_i_Ret)
	{
		m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_DISABLED;
		// Privacy mode status is not supported on this device : disable button
		cMainMenu->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);
	}

	DrawMenuBar();
				

	g_str_ProductDesc = m_x_MorphoSmart.m_puc_ProductDescriptor;
	g_str_SoftwareDesc = m_x_MorphoSmart.m_puc_SoftwareDescriptor;
	g_str_SensorDesc = m_x_MorphoSmart.m_puc_SensorDescriptor;

	//---------------------------------------------
	// Enable/Disable SmartCard Controls
	//---------------------------------------------

	EnableDisableSmartCardControls();

	//---------------------------------------------
	// If database does not exist, create a m_x_Database.
	// If database already exists, check also if it has 
	// the format expected : 100 users, 2 fingers, 2 additionnal fields
	// if the soft supporte WSQ coding don't get the data base
	//---------------------------------------------

	GetFVP_Status();
	GetProcduct_Type();
	CheckIfCBMV();

	// 
	if (m_bl_FVP == true)
	{
		m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.SelectString(0, STR_NO_PK_FP);
	}
	else
	{
		m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.SelectString(0, STR_PKCOMP);
		m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_CHECK_ASLC)->EnableWindow(FALSE);
	}
	UpdateData(TRUE);
	// this param is set to 1 

	if ((m_bl_FVP == true) && (m_bl_Product_Type == false))
	{
		m_tbCtrl.m_TabBioSettingsDlg->m_cmd_FFDSecuLevel.AddString("Standard");
		m_tbCtrl.m_TabBioSettingsDlg->UpdateData(0);
	}
	else if (m_bl_FVP == false)
	{
		// MSO STD && ITO
		m_tbCtrl.m_TabBioSettingsDlg->m_cmd_FFDSecuLevel.InsertString(0, "Critical");
		m_tbCtrl.m_TabBioSettingsDlg->m_cmd_FFDSecuLevel.AddString("Low");
		m_tbCtrl.m_TabBioSettingsDlg->m_str_FFDSecuLevel = _T("Default");
		m_tbCtrl.m_TabBioSettingsDlg->m_cmd_FFDSecuLevel.AddString("Default");
		m_tbCtrl.m_TabBioSettingsDlg->UpdateData(0);
	}

	//Initialize MSO Conf params
	m_tbCtrl.m_TabOptionsDlg->InitConfParams();


	/*Video player***********************************/
	if (m_bl_FVP)
	{
		((CStatic*)(GetDlgItem(IDC_STATIC_ANIMATION_PIC)))->ModifyStyle(0, SS_CENTERIMAGE | SS_BITMAP);

		CRect rect;
		m_ctl_AnimationPic.GetWindowRect(&rect);
		ScreenToClient(&rect);

		RECT Recto = { 0, 0, (long)rect.Width() - 5, (long)rect.Height() - 5 }; //hight and width of m_ctl_AnimationPic

		m_vidPlayer[0] = new GUIMediaVideo(&m_ctl_AnimationPic, &Recto, ".\\Media\\_capture.avi");
		m_vidPlayer[1] = new GUIMediaVideo(&m_ctl_AnimationPic, &Recto, ".\\Media\\_maillage.avi");
	}
	/***************************************************/


	if (!m_bl_FVP)
	{
		m_BioOpPSheet.m_CodePPage.m_ctrl_FVP_TemplateType.EnableWindow(FALSE);
		m_BioOpPSheet.m_CodePPage.GetDlgItem(IDC_OUTPUT_FVP_FILE_TYPE)->ShowWindow(SW_SHOW);
		m_BioOpPSheet.m_CodePPage.GetDlgItem(IDC_STATIC_FVP)->ShowWindow(SW_SHOW);
		m_BioOpPSheet.m_CodePPage.m_ctrl_FVP_TemplateType.SelectString(0, STR_NO_FVP);
		m_BioOpPSheet.m_CodePPage.m_bl_FVP = FALSE;

		m_BioOpPSheet.m_EnrollPPage.GetDlgItem(IDC_OUTPUT_FVP_FILE_TYPE)->ShowWindow(SW_SHOW);
		m_BioOpPSheet.m_EnrollPPage.GetDlgItem(IDC_STATIC_FVP)->ShowWindow(SW_SHOW);

		//Enroll
		m_BioOpPSheet.m_EnrollPPage.m_bl_FVP = FALSE;
	}
	else
	{
		/*l_cs_string.Format("%s", STR_NO_PK_FP);
		m_BioOpPSheet.m_CodePPage.m_ctrl_TemplateType.AddString(l_cs_string);*/

		//Enroll
		m_BioOpPSheet.m_EnrollPPage.m_bl_FVP = TRUE;
		m_BioOpPSheet.m_CodePPage.m_ctrl_FVP_TemplateType.EnableWindow(TRUE);
		m_BioOpPSheet.m_EnrollPPage.m_ctrl_FVP_TemplateType.EnableWindow(TRUE);
		m_BioOpPSheet.m_CodePPage.GetDlgItem(IDC_OUTPUT_FVP_FILE_TYPE)->ShowWindow(SW_SHOW);
		m_BioOpPSheet.m_CodePPage.GetDlgItem(IDC_STATIC_FVP)->ShowWindow(SW_SHOW);
		m_BioOpPSheet.m_CodePPage.m_ctrl_FVP_TemplateType.SelectString(0, STR_PK_FVP);
		m_BioOpPSheet.m_CodePPage.m_bl_FVP = TRUE;
	}


	GetBaseBioValue();
	if (m_bl_BaseBio == false)
		m_BioOpPSheet.m_VerifyPPage.GetDlgItem(IDC_RADIO_LOCALBASE)->EnableWindow(FALSE);

	if ((m_bl_BaseBio == false) && (m_bl_FVP == false)){

		ProgressInit->m_cs_Mess5.Format("no detected database");
		m_b_BaseAvailable = FALSE;
		delete(ProgressInit);
		GetDlgItem(IDC_BUTTON_CREATE_BASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DESTROY_BASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ADD_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_NO_CHECK)->EnableWindow(FALSE);
		m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_EnrollPPage));
		m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_IdentifyPPage));
		m_btm_BitmapBaseStatus.LoadBitmap(IDB_BITMAP_LRED);
		m_ctl_BmpBaseStatus.SetBitmap((HBITMAP)m_btm_BitmapBaseStatus);
		UpdateData(FALSE);
		return FALSE;
	}
	else {
		ProgressInit->m_cs_Mess5.Format("The database configuration");
		ProgressInit->UpdateData(FALSE);

		//- - - - - - - - - - - -
		// Get Database Object
		//- - - - - - - - - - - -

		l_i_Ret = m_x_MorphoSmart.GetDatabase(0, m_x_Database);

		if (l_i_Ret != MORPHO_OK && l_i_Ret != MORPHOERR_BASE_NOT_FOUND)
		{
			ShowErrorBox("An error occured while calling C_MORPHO_Device::GetDatabase() function", l_i_Ret);
			OnCancel();
			delete(ProgressInit);
			return FALSE;
		}

		l_i_Ret = m_x_Database.GetNbUsedRecord(l_ul_nbUsedRecord);

		if (l_i_Ret == MORPHOERR_DEVICE_LOCKED)
		{
			l_i_Ret = UnlockDevice();
			if (l_i_Ret != MORPHO_OK)
			{
				ShowErrorBox("An error occured during Device Unlocking", l_i_Ret);
				{
					OnCancel();
					delete(ProgressInit);
					return FALSE;
				}
			}
		}

		if (l_i_Ret != MORPHO_OK)
		{
			if (l_i_Ret == MORPHOERR_BASE_NOT_FOUND)
			{
				l_cs_Message.Format("There is currently no internal database available in the MorphoSmart.\nWould you like to create one ? ");

				if (AfxMessageBox(l_cs_Message, MB_YESNO | MB_ICONQUESTION) != IDYES)
				{
					InitializeListCtrl();
					ProgressInit->DestroyWindow();
					SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
					delete(ProgressInit);
					//DestroyDatabase();
					GetDlgItem(IDC_BUTTON_CREATE_BASE)->EnableWindow(TRUE);
					GetDlgItem(IDC_BUTTON_DESTROY_BASE)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_ADD_USER)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
					GetDlgItem(IDC_CHECK_NO_CHECK)->EnableWindow(FALSE);
					m_listCtrl.DeleteAllItems();
					GetDlgItem(IDC_LIST_USER)->EnableWindow(FALSE);
					UpdateNumberOfRecords();
					m_cs_MaxNbRecords.Format("%d", 0);
					m_tbCtrl.m_TabDbInfoDlg->m_cs_MaxNbRecords = m_cs_MaxNbRecords;
					m_tbCtrl.m_TabDbInfoDlg->SetParam("EncryptedDB", "No");
					m_NbFingers.Format("%d", 0);
					m_tbCtrl.m_TabDbInfoDlg->m_NbFingers.Format("%d", 0);
					m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_EnrollPPage));
					m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_IdentifyPPage));
					m_b_BaseAvailable = FALSE;
					return TRUE;
				}
				else
				{
					CreateDatabase();
					l_i_Ret = MORPHO_OK;
				}
			}
			else if ((m_x_MorphoSmart.GetInternalError() == -12)
				&& (l_i_Ret == MORPHOERR_STATUS))
			{
				l_cs_Message.Format("Echec when reading Data Base");
				AfxMessageBox(l_cs_Message);
				delete(ProgressInit);
				OnCancel();

				return FALSE;
			}
			else
			{
				ShowErrorBox("An error occured while calling C_MORPHO_Database::GetNbUsedRecord() function", l_i_Ret);
				OnCancel();
				delete(ProgressInit);
				return FALSE;
			}
		}

		//- - - - - - - - - - - - - - - - - - - - - - - -	
		// Is the database valid ??? => we need 2 fields
		//- - - - - - - - - - - - - - - - - - - - - - - -

		l_i_Ret = m_x_Database.GetNbField(l_ul_NbField);

		if (l_i_Ret != MORPHO_OK)
		{
			ShowErrorBox("An error occured while calling C_MORPHO_Database::GetNbField() function", l_i_Ret);
			OnCancel();
			delete(ProgressInit);
			return FALSE;
		}
		else if (l_ul_NbField < 2)
		{
			l_cs_Message.Format("the database format contain less than 2 Field.\nPress Yes if you want to recreate a new base with 2 Field \nThere is currently %d records saved in the Database.\n\nErase and recreate base ? ", l_ul_nbUsedRecord);

			if (AfxMessageBox(l_cs_Message, MB_YESNO | MB_ICONQUESTION) != IDYES)
			{
				OnCancel();
				delete(ProgressInit);
				return FALSE;
			}
			else
			{
				DestroyDatabase();
				CreateDatabase();
			}

			l_i_Ret = m_x_Database.GetNbField(l_ul_NbField);

			if (l_i_Ret != MORPHO_OK)
			{
				ShowErrorBox("An error occured while calling C_MORPHO_Database::GetNbField() function", l_i_Ret);
				OnCancel();
				delete(ProgressInit);
				return FALSE;
			}
		}

		//- - - - - - - - - - - - - - - - - - - - - - - -	
		// Is the database valid ??? => we need 2 public fields
		//- - - - - - - - - - - - - - - - - - - - - - - -

		for (l_ul_fieldIndex = 1; l_ul_fieldIndex<l_ul_NbField + 1; l_ul_fieldIndex++)
		{
			l_i_Ret = m_x_Database.GetField(
				l_ul_fieldIndex,		//UL							i_ul_fieldIndex,
				l_uc_fieldAttribute,	//T_MORPHO_FIELD_ATTRIBUTE &	o_uc_fieldAttribute, 
				l_us_fieldMaxSize,		//US	&						o_us_fieldMaxSize,
				l_auc_fieldName			//UC							o_puc_fieldName[MORPHO_FIELD_NAME_LEN]);
				);

			if (l_i_Ret != MORPHO_OK)
			{
				ShowErrorBox("An error occured while calling C_MORPHO_Database::GetField() function", l_i_Ret);
				OnCancel();
				delete(ProgressInit);
				return FALSE;
			}
			else if (l_uc_fieldAttribute != MORPHO_PUBLIC_FIELD || l_us_fieldMaxSize < DB_FIELD_MAX_SIZE)
			{
				if (l_uc_fieldAttribute != MORPHO_PUBLIC_FIELD)
				{
					l_cs_Message.Format("The format of the database does not appear to be valid for this demo.\n\nMSO_Demo need 2 public fields.\n\nPress Yes if you want to recreate a valid base, but note that all eventual\ncurrent records will be erased\n\nThere is currently %d records saved in the Database.\n\nErase and recreate base ? (answering no will exit the application)", l_ul_nbUsedRecord);
				}
				else
				{
					CString	l_cs_fieldName(l_auc_fieldName);
					l_cs_Message.Format("The format of the database does not appear to be valid for this demo.\n\nThe maximum size of the field (%s) is less than %d characters.\n\nPress Yes if you want to recreate a valid base, but note that all eventual\ncurrent records will be erased\n\nThere is currently %d records saved in the Database.\n\nErase and recreate base ? (answering no will exit the application)", l_cs_fieldName.Mid(0, 6), DB_FIELD_MAX_SIZE, l_ul_nbUsedRecord);
				}

				if (AfxMessageBox(l_cs_Message, MB_YESNO | MB_ICONQUESTION) != IDYES)
				{
					OnCancel();
					delete(ProgressInit);
					return FALSE;
				}
				else
				{
					DestroyDatabase();
					CreateDatabase();
					break;
				}
			}
		}

		m_x_Database.GetNbUsedRecord(l_ul_nbUsedRecord);

		ProgressInit->m_cs_Mess14.Format(": OK");
		ProgressInit->UpdateData(FALSE);

		//////////////////////////////////////////////////////////////

		m_b_BaseAvailable = TRUE;
		m_CurrentNbRecords.Format("%d", l_ul_nbUsedRecord);
		m_tbCtrl.m_TabDbInfoDlg->SetParam("CurrentNbRecords", m_CurrentNbRecords);
		UpdateNumberOfRecords();

		m_x_Database.GetNbTotalRecord(l_ul_sizeDatabase);
		m_cs_MaxNbRecords.Format("%d", l_ul_sizeDatabase);
		m_tbCtrl.m_TabDbInfoDlg->SetParam("MaxNbRecords", m_cs_MaxNbRecords);

		m_x_Database.GetNbFinger(l_uc_NbFinger);
		m_NbFingers.Format("%d", l_uc_NbFinger);
		m_tbCtrl.m_TabDbInfoDlg->SetParam("NbFingers", m_NbFingers);

		UC  l_uc_DbEcryption;
		if (m_x_Database.GetDbEncryptionStatus(l_uc_DbEcryption) != MORPHO_OK)
			m_DbEncryption.Format("N/A");
		else
		{
			if (C_MORPHO_Device::PRIVACY_MODE_DISABLED == m_PrivacyModeStatus)
				m_DbEncryption.Format("%s", (l_uc_DbEcryption ? "Yes" : "No"));
			else
				m_DbEncryption.Format("%s", "Yes (Privacy Mode)");
		}
		m_tbCtrl.m_TabDbInfoDlg->SetParam("EncryptedDB", m_DbEncryption);

		UpdateData(FALSE);

		if (l_ul_nbUsedRecord == 0)
		{
			GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(FALSE);
		}

		//--------------------------------------------------------------
		// Display all Users Lastname and Firstname in the List Ctrl
		//--------------------------------------------------------------

		InitializeListCtrl();
		RefreshListCtrl();

		PostMessage(WM_PAINT, 0, 0);
		//**************************************************

		ProgressInit->DestroyWindow();
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		delete(ProgressInit);
	}


	return TRUE;  // return TRUE  unless you set the focus to a control
}

/////////////////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

/////////////////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDC		*l_p_Cdc;
		CRect	myRect;
		I		l_i_LastImageIndex;

		if (m_px_Image != NULL)
		{
			if (m_BioOpPSheet.m_pProcessPPage != NULL)
			{
				l_i_LastImageIndex = 5;
			}
			else
			{
				l_i_LastImageIndex = 0;
			}

			//-------------------------------
			// Refresh Async Images Viewer 
			//-------------------------------	

			l_p_Cdc = m_ctl_AnimationPic.GetDC();
			m_ctl_AnimationPic.GetClientRect(&myRect);
			m_px_Image->RefreshViewer(l_i_LastImageIndex, myRect, l_p_Cdc);
			ReleaseDC(l_p_Cdc);
		}

		CDialog::OnPaint();

		if (m_vidPlayer[0])
			m_vidPlayer[0]->RepaintVideo();
		if (m_vidPlayer[1])
			m_vidPlayer[1]->RepaintVideo();
	}
}

/////////////////////////////////////////////////////////////////////////////
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMSO_DemoDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

/////////////////////////////////////////////////////////////////////////////
UINT TaskCode(LPVOID pParam)
{
	bool l_b_Ret;
	CMSO_DemoDlg *MSO_DemoDlg = (CMSO_DemoDlg*)pParam;

	MSO_DemoDlg->DisableButtons();

	l_b_Ret = MSO_DemoDlg->_Capture();

	MSO_DemoDlg->EnableButtons();
	MSO_DemoDlg->InitQualityBar();
	MSO_DemoDlg->InitGuidingAnimations();

	return l_b_Ret;
}

/////////////////////////////////////////////////////////////////////////////
UINT TaskVerify(LPVOID pParam)
{
	bool l_b_Ret;
	CMSO_DemoDlg *MSO_DemoDlg = (CMSO_DemoDlg*)pParam;

	MSO_DemoDlg->DisableButtons();

	l_b_Ret = MSO_DemoDlg->_Verify();

	MSO_DemoDlg->EnableButtons();
	MSO_DemoDlg->InitQualityBar();
	MSO_DemoDlg->InitGuidingAnimations();

	return l_b_Ret;
}

/////////////////////////////////////////////////////////////////////////////
UINT TaskEnroll(LPVOID pParam)
{
	CMSO_DemoDlg *MSO_DemoDlg = (CMSO_DemoDlg*)pParam;
	return MSO_DemoDlg->_Enroll();
}

/////////////////////////////////////////////////////////////////////////////
UINT TaskIdentify(LPVOID pParam)
{
	CMSO_DemoDlg *MSO_DemoDlg = (CMSO_DemoDlg*)pParam;
	return MSO_DemoDlg->_Identify();
}

/////////////////////////////////////////////////////////////////////////////
UINT TaskGetImage(LPVOID pParam)
{
	CMSO_DemoDlg *MSO_DemoDlg = (CMSO_DemoDlg*)pParam;
	return MSO_DemoDlg->_GetImage();
}

/////////////////////////////////////////////////////////////////////////////
UINT TaskMatchOnCard(LPVOID pParam)
{
	bool l_b_Ret;

	CMSO_DemoDlg *MSO_DemoDlg = (CMSO_DemoDlg*)pParam;

	MSO_DemoDlg->DisableButtons();

	l_b_Ret = MSO_DemoDlg->_MatchOnCard();

	MSO_DemoDlg->EnableButtons();
	MSO_DemoDlg->InitQualityBar();
	MSO_DemoDlg->InitGuidingAnimations();

	return l_b_Ret;
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnButtonStart()
{
	CPropertyPage* l_px_PPage;

	//En mode traitement ce test permet d'arreter l'operation.
	if (m_b_BiometricOperationProcessing)
	{
		m_x_MorphoSmart.CancelLiveAcquisition();
		m_b_BiometricOperationProcessing = FALSE;
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		HideAnimation();
		return;
	}

	OnUpdateDataTrue(0, 0);

	l_px_PPage = m_BioOpPSheet.GetActivePage();

	InitQualityBar();

	m_tbCtrl.m_TabBioSettingsDlg->UpdateData(1);

	//--------------------------------
	// CODE 
	//--------------------------------

	if (l_px_PPage == &m_BioOpPSheet.m_CodePPage)
	{
		m_ppx_Threads[ID_THREAD_CAPTURE] = AfxBeginThread(
			TaskCode,		//AFX_THREADPROC pfnThreadProc, 
			(LPVOID)this	//LPVOID pParam, 
			);
	}

	//--------------------------------
	// VERIFY
	//--------------------------------

	else if (l_px_PPage == &m_BioOpPSheet.m_VerifyPPage)
	{
		m_ppx_Threads[ID_THREAD_VERIFY] = AfxBeginThread(
			TaskVerify,		//AFX_THREADPROC pfnThreadProc, 
			(LPVOID)this	//LPVOID pParam, 
			);

	}

	//--------------------------------
	// GET_IMAGE
	//--------------------------------

	else if (l_px_PPage == &m_BioOpPSheet.m_GetImagePPage)
	{
		m_ppx_Threads[ID_THREAD_GETIMAGE] = AfxBeginThread(
			TaskGetImage,		//AFX_THREADPROC pfnThreadProc, 
			(LPVOID)this	//LPVOID pParam, 
			);
	}
	//--------------------------------
	// ENROLLMENT
	//--------------------------------

	else if (l_px_PPage == &m_BioOpPSheet.m_EnrollPPage)
	{
		m_ppx_Threads[ID_THREAD_ENROLL] = AfxBeginThread(
			TaskEnroll,		//AFX_THREADPROC pfnThreadProc, 
			(LPVOID)this	//LPVOID pParam, 
			);
	}

	//--------------------------------
	// IDENTIFY
	//--------------------------------

	else if (l_px_PPage == &m_BioOpPSheet.m_IdentifyPPage)
	{
		m_ppx_Threads[ID_THREAD_IDENTIFY] = AfxBeginThread(
			TaskIdentify,	//AFX_THREADPROC pfnThreadProc, 
			(LPVOID)this	//LPVOID pParam, 
			);
	}
	//--------------------------------
	// Match On Card
	//--------------------------------

	else if (l_px_PPage == &m_BioOpPSheet.m_MocPPage)
	{
		m_ppx_Threads[ID_THREAD_IDENTIFY] = AfxBeginThread(
			TaskMatchOnCard,	//AFX_THREADPROC pfnThreadProc, 
			(LPVOID)this	//LPVOID pParam, 
			);
	}
}

//////////////////////////////////////////////////////////////////

void CMSO_DemoDlg::OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	m_BioOpPSheet.SetActivePage(&(m_BioOpPSheet.m_VerifyPPage));
	m_BioOpPSheet.m_VerifyPPage.m_i_InputChoice = 1;
	m_BioOpPSheet.m_VerifyPPage.UpdateData(FALSE);

	OnButtonStart();
}

//////////////////////////////////////////////////////////////////

void CMSO_DemoDlg::OnButtonRemoveUser()
{
	I			l_i_Ret;
	CString		l_cs_Message;
	POSITION	l_pos;
	I			l_i_Indice;
	CString		l_cs_ID;
	UC			l_uc_lenUserID;
	UC			l_puc_UserID[25];
	UC			l_puc_CryptedUserID[255];
	BOOL		l_b_OneItemSelectedAtLeast = FALSE;
	UL			l_ul_nbUsedRecord;
	

	std::string	sl_encrypt_Key;
	BOOL isPrivacyModeEnabled = (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED) || (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED);
	UL random;
	BOOL RetKey;
	if (isPrivacyModeEnabled)
	{
		RetKey = GetEncryptedKey(sl_encrypt_Key);
		if (!RetKey)
		{
			AfxMessageBox("Error occured when loading encryption key.");
			return;
		}
		random = time(NULL);
	}

	if (m_CurrentNbRecords == "0")
	{
		AfxMessageBox("The Database is empty !\nNo record to remove.", MB_ICONINFORMATION);
	}

	l_pos = m_listCtrl.GetFirstSelectedItemPosition();
	
	do
	{
		l_i_Indice = m_listCtrl.GetNextSelectedItem(l_pos);
		if (l_i_Indice != -1)
		{
			C_MORPHO_User	l_x_User;
			l_b_OneItemSelectedAtLeast = TRUE;
			if (isPrivacyModeEnabled)
			{
				if (m_IsPrivacyListClear)
				{
					l_cs_ID = m_listCtrl.GetItemText((int)l_i_Indice, 0);
					l_uc_lenUserID = l_cs_ID.GetLength();
					memcpy(l_puc_UserID, l_cs_ID.GetBuffer(l_uc_lenUserID), l_uc_lenUserID);
				}
				else
				{
					CString tmp = m_listCtrl.GetItemText((int)l_i_Indice, 0);
					UL l_ul_data_len = tmp.GetLength();
					PUC o_ppuc_Aes128CbcDeCryptedData = NULL;
					UL o_pul_Aes128CbcDeCryptedDataLen = 0;

					UL crc32 = 0, random = 0, index = 0, dataSize = 0;

					bool l_b_decryptRet = _DecryptAESData((UC*)tmp.GetBuffer(), l_ul_data_len, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
					if (!l_b_decryptRet || o_pul_Aes128CbcDeCryptedDataLen == 0)
					{
						AfxMessageBox("Decrypting data error !", MB_ICONERROR);
						return;
					}					

					memcpy(&crc32, o_ppuc_Aes128CbcDeCryptedData, 4); // Get CRC32
					memcpy(&random, o_ppuc_Aes128CbcDeCryptedData + 4, 4); // GET Random
					memcpy(&index, o_ppuc_Aes128CbcDeCryptedData + 8, 4); // Get Index
					memcpy(&dataSize, o_ppuc_Aes128CbcDeCryptedData + 12, 4); // Get Size

					memcpy(l_puc_UserID, o_ppuc_Aes128CbcDeCryptedData + 16, dataSize);
					l_uc_lenUserID = dataSize;
					l_puc_UserID[l_uc_lenUserID] = 0;

					if (NULL != o_ppuc_Aes128CbcDeCryptedData)
						MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
				}
			}
			else
			{
				l_cs_ID = m_listCtrl.GetItemText((int)l_i_Indice, 0);
				l_uc_lenUserID = l_cs_ID.GetLength();
				memcpy(l_puc_UserID, l_cs_ID.GetBuffer(l_uc_lenUserID), l_uc_lenUserID);
				l_puc_UserID[l_uc_lenUserID++] = 0;
			}
			
			

			//--------------------------------------------------//
			// Remove a user thanks to its ID					//
			//--------------------------------------------------//
			if (isPrivacyModeEnabled)
			{
				_PreparePrivacyData((C*)l_puc_UserID, l_uc_lenUserID, sl_encrypt_Key, random, TRUE, l_puc_CryptedUserID, (UL*)&l_uc_lenUserID);
				l_i_Ret = m_x_Database.GetUser(
					l_uc_lenUserID,				// UC				i_uc_lenUserID,
					l_puc_CryptedUserID,				// PUC				i_puc_UserID,
					l_x_User					// C_MORPHO_User & o_x_User 
					);
			}
			else 
			{
				l_i_Ret = m_x_Database.GetUser(
					l_uc_lenUserID,				// UC				i_uc_lenUserID,
					l_puc_UserID,				// PUC				i_puc_UserID,
					l_x_User					// C_MORPHO_User & o_x_User 
					);
			}
			
			if (l_i_Ret != MORPHO_OK)
			{
				ShowErrorBox("An error occured while calling C_MORPHO_Database::GetUser() function", l_i_Ret);
				return;
			}

			l_i_Ret = l_x_User.DbDelete();

			if (l_i_Ret != MORPHO_OK)
			{
				ShowErrorBox("An error occured while calling C_MORPHO_User::DbDelete() function", l_i_Ret);
				return;
			}
			//--------------------------------------------------//
		}
		else
		{
			if (l_pos == NULL)
			{
				if (!l_b_OneItemSelectedAtLeast)
				{
					AfxMessageBox("Select a user first.", MB_ICONINFORMATION);
				}
				return;
			}
		}
	} while (l_i_Indice != -1);

	UpdateNumberOfRecords();

	l_i_Ret = m_x_Database.GetNbUsedRecord(l_ul_nbUsedRecord);

	if (l_i_Ret == MORPHO_OK && l_ul_nbUsedRecord == 0)
	{
		GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(FALSE);
	}

	RefreshListCtrl(sl_encrypt_Key);
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnButtonUpdateUser()
{
	I			l_i_Ret;
	CString		l_cs_Message;
	POSITION	l_pos;
	I			l_i_Indice;
	CString		l_cs_ID;
	UC			l_uc_lenUserID;
	UC			l_puc_UserID[100];
	BOOL		l_b_OneItemSelectedAtLeast = FALSE;
	UL			l_ul_lenBuf;
	UC			l_puc_Buf[100];

	BOOL isInPrivacyMode = (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED) || (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED);
	
	UI l_i_Random = time(NULL);
	PUC l_puc_Aes128CbcDeCryptedData = NULL;
	UL	l_ul_Aes128CbcDeCryptedDataLen = 0;
	
	UL l_ul_index = 0, l_ul_crc32 = 0, l_ul_random = 0;

	UC l_puc_tempData[255];
	UL l_ul_tempDataLen;
	UL l_ul_dataSize;

	bool _decrypt = false;
	std::string	sl_encrypt_Key="";
	bool RetKey = false;
	if (isInPrivacyMode)
	{
		AfxMessageBox("AES 128 Key is required for update ");
		{
			RetKey = GetEncryptedKey(sl_encrypt_Key);
			if (!RetKey)
			{
				AfxMessageBox("Error occured when loading encryption key.");
				return;
			}

				if(!m_IsPrivacyListClear)
				_decrypt = true;
		}
	}

	if (m_CurrentNbRecords == "0")
	{
		AfxMessageBox("The Database is empty !\nNo record to update.", MB_ICONINFORMATION);
	}

	l_pos = m_listCtrl.GetFirstSelectedItemPosition();

	do
	{
		l_i_Indice = m_listCtrl.GetNextSelectedItem(l_pos);
		if (l_i_Indice != -1)
		{
			C_MORPHO_User	l_x_User;
			l_b_OneItemSelectedAtLeast = TRUE;
			l_cs_ID = m_listCtrl.GetItemText((int)l_i_Indice, 0);
			l_uc_lenUserID = l_cs_ID.GetLength();
			if (isInPrivacyMode)
			{
				if (!m_IsPrivacyListClear)
				{
					bool b_decRet = _DecryptAESData((PUC)l_cs_ID.GetBuffer(l_uc_lenUserID), l_uc_lenUserID, &l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
					if (!b_decRet || l_ul_Aes128CbcDeCryptedDataLen == 0)
					{
						AfxMessageBox("Decrypting data error !", MB_ICONERROR);
						return;
					}
					
					memcpy(&l_ul_dataSize, l_puc_Aes128CbcDeCryptedData + 12, 4); // Get Size
					memcpy(l_puc_Aes128CbcDeCryptedData, l_puc_Aes128CbcDeCryptedData + 16, l_ul_dataSize);
					l_puc_Aes128CbcDeCryptedData[l_ul_dataSize] = 0;

					_PreparePrivacyData((C*)l_puc_Aes128CbcDeCryptedData, l_ul_dataSize, sl_encrypt_Key, l_i_Random, TRUE, l_puc_tempData, &l_ul_tempDataLen);
					l_uc_lenUserID = l_ul_tempDataLen;
					memcpy(l_puc_UserID, l_puc_tempData, l_ul_tempDataLen);
				}
				else
				{
					_PreparePrivacyData((C*)l_cs_ID.GetBuffer(l_uc_lenUserID), l_uc_lenUserID, sl_encrypt_Key, l_i_Random, TRUE, l_puc_tempData, &l_ul_tempDataLen);
					l_uc_lenUserID = l_ul_tempDataLen;
					memcpy(l_puc_UserID, l_puc_tempData, l_ul_tempDataLen);
				}
			}
			else
			{
				memcpy(l_puc_UserID, l_cs_ID.GetBuffer(l_uc_lenUserID), l_uc_lenUserID);
				l_puc_UserID[l_uc_lenUserID++] = 0;
			}

			//--------------------------------------------------//
			// Remove a user thanks to its ID					//
			//--------------------------------------------------//

			l_i_Ret = m_x_Database.GetUser(
				l_uc_lenUserID,				// UC				i_uc_lenUserID,
				l_puc_UserID,				// PUC				i_puc_UserID,
				l_x_User					// C_MORPHO_User & o_x_User 
				);

			if (l_i_Ret != MORPHO_OK)
			{
				ShowErrorBox("An error occured while calling C_MORPHO_Database::GetUser() function", l_i_Ret);
				return;
			}

			//------------------------------------------------------------
			// Additionnal Fields : Lastname & Firstname
			//------------------------------------------------------------

			CUpdateUserDlg		l_x_UpdateUserDlg(FALSE);

			if ((isInPrivacyMode) && (!m_IsPrivacyListClear) )
			{
				CString l_cs_tempFirstName = m_listCtrl.GetItemText((int)l_i_Indice, 1);
				CString l_cs_tempLastName = m_listCtrl.GetItemText((int)l_i_Indice, 2);
				if (!m_IsPrivacyListClear)
					l_x_UpdateUserDlg.m_cs_ID = l_puc_Aes128CbcDeCryptedData;
				else
					l_x_UpdateUserDlg.m_cs_ID = m_listCtrl.GetItemText((int)l_i_Indice, 0);

				l_ul_tempDataLen = l_cs_tempFirstName.GetLength();
				bool b_decRet = _DecryptAESData((PUC)l_cs_tempFirstName.GetBuffer(l_ul_tempDataLen), l_ul_tempDataLen, &l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
				if (!b_decRet || l_ul_Aes128CbcDeCryptedDataLen == 0)
				{
					AfxMessageBox("Decrypting data error !", MB_ICONERROR);
					EnableButtons();
					return;
				}
				
				{
					l_ul_dataSize = 0;
					memcpy(&l_ul_crc32, l_puc_Aes128CbcDeCryptedData, 4); // Get CRC32
					memcpy(&l_ul_random, l_puc_Aes128CbcDeCryptedData + 4, 4); // GET Random
					memcpy(&l_ul_index, l_puc_Aes128CbcDeCryptedData + 8, 4); // Get Index
					memcpy(&l_ul_dataSize, l_puc_Aes128CbcDeCryptedData + 12, 4); // Get Size

					memcpy(l_puc_tempData, l_puc_Aes128CbcDeCryptedData + 16, l_ul_dataSize);
					l_puc_tempData[l_ul_dataSize] = 0;

					l_x_UpdateUserDlg.m_cs_Firstname = l_puc_tempData;
					
					if (NULL != l_puc_Aes128CbcDeCryptedData)
						MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
				}
				
				l_puc_Aes128CbcDeCryptedData = NULL;
				l_ul_Aes128CbcDeCryptedDataLen = 0;

				l_ul_tempDataLen = l_cs_tempLastName.GetLength();
				b_decRet = _DecryptAESData((PUC)l_cs_tempLastName.GetBuffer(l_ul_tempDataLen), l_ul_tempDataLen, &l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
				if (!b_decRet || l_ul_Aes128CbcDeCryptedDataLen == 0)
				{
					AfxMessageBox("Decrypting data error !", MB_ICONERROR);
					EnableButtons();
					return;
				}

				{
					memcpy(&l_ul_crc32, l_puc_Aes128CbcDeCryptedData, 4); // Get CRC32
					memcpy(&l_ul_random, l_puc_Aes128CbcDeCryptedData + 4, 4); // GET Random
					memcpy(&l_ul_index, l_puc_Aes128CbcDeCryptedData + 8, 4); // Get Index
					memcpy(&l_ul_dataSize, l_puc_Aes128CbcDeCryptedData + 12, 4); // Get Size

					memcpy(l_puc_tempData, l_puc_Aes128CbcDeCryptedData + 16, l_ul_dataSize);
					l_puc_tempData[l_ul_dataSize] = 0;

					l_x_UpdateUserDlg.m_cs_Lastname = l_puc_tempData;
				}

				if (NULL != l_puc_Aes128CbcDeCryptedData)
					MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
			}
			else
			{
				l_x_UpdateUserDlg.m_cs_ID = m_listCtrl.GetItemText((int)l_i_Indice, 0);
				l_x_UpdateUserDlg.m_cs_Firstname = m_listCtrl.GetItemText((int)l_i_Indice, 1);
				l_x_UpdateUserDlg.m_cs_Lastname = m_listCtrl.GetItemText((int)l_i_Indice, 2);
			}
			if (l_x_UpdateUserDlg.DoModal() != IDOK)
				return;

			//- - - - - - - - - -
			// Firstname
			//- - - - - - - - - -
			l_ul_lenBuf = l_x_UpdateUserDlg.m_cs_Firstname.GetLength();
			if (isInPrivacyMode)
			{
				UI index = 1;
				UI l_ui_Size = l_ul_lenBuf;
			
				memcpy(l_puc_tempData, &index, 4);
				memcpy(l_puc_tempData + 4, &l_ui_Size, 4);
				memcpy(l_puc_tempData + 8, l_x_UpdateUserDlg.m_cs_Firstname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf);

				_PreparePrivacyData((C*)l_puc_tempData, l_ul_lenBuf + 8, sl_encrypt_Key, l_i_Random, TRUE, l_puc_Buf, (PUL)&l_ul_lenBuf);
			}
			else
			{
				memcpy(l_puc_Buf, l_x_UpdateUserDlg.m_cs_Firstname.GetBuffer(), l_ul_lenBuf);
				l_puc_Buf[l_ul_lenBuf++] = 0;
			}
			l_i_Ret = l_x_User.PutField(
				1,						// UL		i_ul_indexField,
				l_ul_lenBuf,			// UL		i_ul_lenField,
				l_puc_Buf				// PUC		i_puc_dataField);
				);

			//- - - - - - - - - -
			// Lastname
			//- - - - - - - - - -

			if (l_i_Ret == MORPHO_OK)
			{
				l_ul_lenBuf = l_x_UpdateUserDlg.m_cs_Lastname.GetLength();
				if (isInPrivacyMode)
				{
					UI index = 2;
					UI l_ui_Size = l_ul_lenBuf + 1;
					memcpy(l_puc_tempData, &index, 4);
					memcpy(l_puc_tempData + 4, &l_ui_Size, 4);
					memcpy(l_puc_tempData + 8, l_x_UpdateUserDlg.m_cs_Lastname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf);
					_PreparePrivacyData((C*)l_puc_tempData, l_ul_lenBuf + 8, sl_encrypt_Key, l_i_Random, TRUE, l_puc_Buf, (PUL)&l_ul_lenBuf);
				}
				else
				{
					memcpy(l_puc_Buf, l_x_UpdateUserDlg.m_cs_Lastname.GetBuffer(), l_ul_lenBuf);
					l_puc_Buf[l_ul_lenBuf++] = 0;
				}
				l_i_Ret = l_x_User.PutField(
					2,						// UL		i_ul_indexField,
					l_ul_lenBuf,			// UL		i_ul_lenField,
					l_puc_Buf				// PUC		i_puc_dataField);
					);
			}

			//- - - - - - - - - -
			// Error cases
			//- - - - - - - - - -

			if (l_i_Ret != MORPHO_OK)
			{
				ShowErrorBox("An error occured while calling C_MORPHO_User::PutField() function", l_i_Ret);
				EnableButtons();
				return;
			}
			l_i_Ret = l_x_User.DbUpdatePublicFields();

			if (l_i_Ret != MORPHO_OK)
			{
				ShowErrorBox("An error occured while calling C_MORPHO_User::DbUpdatePublicFields() function", l_i_Ret);
				return;
			}
			//--------------------------------------------------//
		}
		else
		{
			if (l_pos == NULL)
			{
				if (!l_b_OneItemSelectedAtLeast)
				{
					AfxMessageBox("Select a user first.", MB_ICONINFORMATION);
				}
				return;
			}
		}
	} while (l_i_Indice != -1);

	UpdateNumberOfRecords();
	RefreshListCtrl(sl_encrypt_Key);

}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnButtonRemoveAllUsers()
{
	I		l_i_Ret;
	CString l_cs_Message;

	if (m_CurrentNbRecords == "0")
	{
		AfxMessageBox("The Database is empty !\nNo record to remove.", MB_ICONINFORMATION);
	}
	else if (AfxMessageBox("This operation is going to erase the whole Database. Confirm ?", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		//--------------------------------------------------//
		// Remove all users from the database				//
		//--------------------------------------------------//

		l_i_Ret = m_x_Database.DbDelete(
			C_MORPHO_Database::MORPHO_ERASE_BASE // T_MORPHO_TYPE_DELETION		i_uc_DeletionType 
			);

		if (l_i_Ret != MORPHO_OK)
		{
			ShowErrorBox("An error occured while calling C_MORPHO_Database::EraseDataBase() function", l_i_Ret);
		}

		//--------------------------------------------------//
		else
		{
			UpdateNumberOfRecords();
			RefreshListCtrl();

			AfxMessageBox("Database Empty", MB_ICONINFORMATION);

			GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(FALSE);
		}
	}
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::DestroyDatabase()
{
	I		l_i_Ret;
	CString l_cs_Message;

	l_i_Ret = m_x_Database.DbDelete(
		C_MORPHO_Database::MORPHO_DESTROY_BASE // T_MORPHO_TYPE_DELETION		i_uc_DeletionType 
		);
	m_cs_MaxNbRecords.Format("%d", 0);
	m_tbCtrl.m_TabDbInfoDlg->SetParam("MaxNbRecords", m_cs_MaxNbRecords);

	m_NbFingers.Format("%d", 0);
	m_tbCtrl.m_TabDbInfoDlg->SetParam("NbFingers", m_NbFingers);

	m_DbEncryption.Format("%s", "No");
	m_tbCtrl.m_TabDbInfoDlg->SetParam("EncryptedDB", m_DbEncryption);
	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling C_MORPHO_Database::EraseDataBase() function", l_i_Ret);
		return FALSE;
	}

	m_b_BaseAvailable = FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::CreateDatabase()
{
	UL					l_ul_fieldIndex;
	C					l_ac_fieldName[MORPHO_FIELD_NAME_LEN];
	I					l_i_Ret;
	CString				l_cs_Message;
	DBaseCreateOption *	l__base_option;

	l__base_option = new DBaseCreateOption(this);

	if (l__base_option->DoModal() == IDOK)
	{
		//--------------------------------------------------//
		// Create Fields 									//
		//--------------------------------------------------//

		//- - - - - - - - - -
		// Firstname
		//- - - - - - - - - - 
		strcpy_s(l_ac_fieldName, sizeof(l_ac_fieldName), "First");
		l_i_Ret = m_x_Database.PutField(
			MORPHO_PUBLIC_FIELD,	// T_MORPHO_FIELD_ATTRIBUTE	i_uc_fieldAttribute, 
			DB_FIELD_MAX_SIZE,		// US						i_us_fieldMaxSize,
			(PUC)l_ac_fieldName,		// UC						i_puc_fieldName[MORPHO_FIELD_NAME_LEN],
			l_ul_fieldIndex			// UL &						o_ul_fieldIndex
			);

		//- - - - - - - - - -
		// Lastname
		//- - - - - - - - - - 
		if (l_i_Ret == MORPHO_OK)
		{
			strcpy_s(l_ac_fieldName, sizeof(l_ac_fieldName), "Last");
			l_i_Ret = m_x_Database.PutField(
				MORPHO_PUBLIC_FIELD,	// T_MORPHO_FIELD_ATTRIBUTE	i_uc_fieldAttribute, 
				DB_FIELD_MAX_SIZE,		// US						i_us_fieldMaxSize,
				(PUC)l_ac_fieldName,		// UC						i_puc_fieldName[MORPHO_FIELD_NAME_LEN],
				l_ul_fieldIndex			// UL &						o_ul_fieldIndex
				);
		}

		//- - - - - - - - - -
		// Error Case
		//- - - - - - - - - - 

		if (l_i_Ret != MORPHO_OK){
			ShowErrorBox("An error occured while calling C_MORPHO_Database::PutField() function", l_i_Ret);
		}

		//--------------------------------------------------//
		// Create Base 										//
		//--------------------------------------------------//

		l_i_Ret = m_x_Database.DbCreate(
			(UL)l__base_option->m_i_max_record,
			//DB_NB_RECORDS,		// UL							i_ul_NbRecord,
			(UC)l__base_option->m_i_finger_number,
			//DB_NB_FINGERS,		// UC							i_uc_NbFinger	
			MORPHO_PK_COMP,		// T_MORPHO_TYPE_TEMPLATE			i_x_TemplateType
			0,
			l__base_option->m_b_dataEncryption
			);

		if (l_i_Ret == MORPHOERR_DEVICE_LOCKED)
		{
			delete l__base_option;
			l_i_Ret = UnlockDevice();
			if (l_i_Ret != MORPHO_OK)
			{
				ShowErrorBox("An error occured during Device Unlocking", l_i_Ret);
			}
			// Base is not created
			return FALSE;
		}
		else if (l_i_Ret != MORPHO_OK){
			ShowErrorBox("An error occured while calling C_MORPHO_Database::CreateDataBase() function", l_i_Ret);
			delete l__base_option;
			return FALSE;
		}
		else {
			m_cs_MaxNbRecords.Format("%d", l__base_option->m_i_max_record);
			m_tbCtrl.m_TabDbInfoDlg->SetParam("MaxNbRecords", m_cs_MaxNbRecords);
			m_NbFingers.Format("%d", l__base_option->m_i_finger_number);
			m_tbCtrl.m_TabDbInfoDlg->SetParam("NbFingers", m_NbFingers);

			CString l_cs_EncryptedDB;
			if (C_MORPHO_Device::PRIVACY_MODE_DISABLED == m_PrivacyModeStatus)
				l_cs_EncryptedDB = (l__base_option->m_b_dataEncryption ? "Yes" : "No");
			else
				l_cs_EncryptedDB = "Yes (Privacy Mode)";
			m_tbCtrl.m_TabDbInfoDlg->SetParam("EncryptedDB", l_cs_EncryptedDB);
		}

		m_b_BaseAvailable = TRUE;
		delete l__base_option;
		return TRUE;
	}
	else
	{
		m_b_BaseAvailable = FALSE;
		delete l__base_option;
		return FALSE;
	}
}

bool CMSO_DemoDlg::ShowUserData(
	C_MORPHO_User & i_x_User
	)
{
	I				l_i_FieldIndex;
	UL				l_ul_LenField;
	PUC				l_puc_DataField;
	UC				l_puc_GenericBuf[100];
	CString			l_cs_Message;
	CString			l_cs_ID, l_cs_Firstname, l_cs_Lastname;
	I				l_i_Ret;
	CResultDlg		l_x_ResultDlg;


	//--------------------------------------------------------------
	// Retrieve ID, Firstname and Lastname of the person identified
	//--------------------------------------------------------------

	//- - - - - - - - - - 
	// ID
	//- - - - - - - - - - 
	PUC o_ppuc_Aes128CbcDeCryptedData = NULL;
	UL	o_pul_Aes128CbcDeCryptedDataLen = 0;

	l_i_FieldIndex = 0;
	l_i_Ret = i_x_User.GetField(
		l_i_FieldIndex,		//I			i_i_FieldIndex,
		l_ul_LenField,		//UL			o_ul_LenField,
		l_puc_DataField		//PUC			o_puc_DataField	
		);

	//Abd decrypt
	bool _decrypt = false;
	std::string	sl_encrypt_Key;
	bool RetKey = false;
	if ((m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED) || (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED))
	{
		if (AfxMessageBox("Decrypt user info ?", MB_YESNO) == IDYES)
		{	
			RetKey = GetEncryptedKey(sl_encrypt_Key);
			if(RetKey) _decrypt = true;
		}
	}
	if (_decrypt)
	{
		bool b_decRet=_DecryptAESData(l_puc_DataField, l_ul_LenField, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
		if (b_decRet)
		{
			memcpy(l_puc_GenericBuf, o_ppuc_Aes128CbcDeCryptedData + 8, o_pul_Aes128CbcDeCryptedDataLen - 8);
			l_puc_GenericBuf[o_pul_Aes128CbcDeCryptedDataLen - 8] = 0;
		}

		if (NULL != o_ppuc_Aes128CbcDeCryptedData)
			MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
	}
	else
	{
		memcpy(l_puc_GenericBuf, l_puc_DataField, l_ul_LenField);
		l_puc_GenericBuf[l_ul_LenField] = 0;
	}
	l_cs_ID.Format("%s", l_puc_GenericBuf);

	//- - - - - - - - - - 
	// Firstname
	//- - - - - - - - - - 

	if (l_i_Ret == MORPHO_OK)
	{
		l_i_FieldIndex = 1;
		l_i_Ret = i_x_User.GetField(
			l_i_FieldIndex,		//I			i_i_FieldIndex,
			l_ul_LenField,		//UL			o_ul_LenField,
			l_puc_DataField		//PUC			o_puc_DataField	
			);

		if (_decrypt)
		{
			bool b_decRet = _DecryptAESData(l_puc_DataField, l_ul_LenField, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
			if (b_decRet)
			{
				memcpy(l_puc_GenericBuf, o_ppuc_Aes128CbcDeCryptedData + 8, o_pul_Aes128CbcDeCryptedDataLen - 8);
				l_puc_GenericBuf[o_pul_Aes128CbcDeCryptedDataLen - 8] = 0;
			}

			if (NULL != o_ppuc_Aes128CbcDeCryptedData)
				MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
		}
		else
		{
			memcpy(l_puc_GenericBuf, l_puc_DataField, l_ul_LenField);
		l_puc_GenericBuf[l_ul_LenField] = 0;
		}
		l_cs_Firstname.Format("%s", l_puc_GenericBuf);
	}

	//- - - - - - - - - - 
	// Lastname
	//- - - - - - - - - - 

	if (l_i_Ret == MORPHO_OK)
	{
		l_i_FieldIndex = 2;
		l_i_Ret = i_x_User.GetField(
			l_i_FieldIndex,		//I			i_i_FieldIndex,
			l_ul_LenField,		//UL			o_ul_LenField,
			l_puc_DataField		//PUC			o_puc_DataField	
			);

		if (_decrypt)
		{
			bool b_decRet = _DecryptAESData(l_puc_DataField, l_ul_LenField, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
			if (b_decRet)
			{
				memcpy(l_puc_GenericBuf, o_ppuc_Aes128CbcDeCryptedData + 8, o_pul_Aes128CbcDeCryptedDataLen - 8);
				l_puc_GenericBuf[o_pul_Aes128CbcDeCryptedDataLen - 8] = 0;
			}
			if (NULL != o_ppuc_Aes128CbcDeCryptedData)
				MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
		}
		else
		{
			memcpy(l_puc_GenericBuf, l_puc_DataField, l_ul_LenField);
		l_puc_GenericBuf[l_ul_LenField] = 0;
		}
		l_cs_Lastname.Format("%s", l_puc_GenericBuf);
	}

	//- - - - - - - - - - 
	// Error case
	//- - - - - - - - - - 

	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling C_MORPHO_User::GetField() function", l_i_Ret);
		EnableButtons();
		RedrawWnd();
		return FALSE;
	}

	//------------------------------------------------------------
	// Display Result
	//------------------------------------------------------------

	Beep(BEEP_FREQUENCY, BEEP_DURATION);
	l_x_ResultDlg.m_cs_Msg1.Format("User identified");
	l_x_ResultDlg.m_cs_Msg2.Format("Firstname : %s", l_cs_Firstname);
	l_x_ResultDlg.m_cs_Msg3.Format("Lastname : %s", l_cs_Lastname);
	l_x_ResultDlg.m_cs_Msg4.Format("ID : %s", l_cs_ID);

	l_x_ResultDlg.DoModal();

	return TRUE;
}
//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::_Identify()
{
	C_MORPHO_User	l_x_User;
	CString			l_cs_Message;
	CString			l_cs_ID, l_cs_Firstname, l_cs_Lastname;
	I				l_i_Ret;
	UL				l_ul_CallbackMask;
	CResultDlg		l_x_ResultDlg;
	bool			l_b_Ret;
	UL				l_ul_DetectModeChoice;

	//------------------------------------------------------------
	// Does not allow other operations
	//------------------------------------------------------------

	DisableButtons();

	//------------------------------------------------------------
	// Viewer or not viewer.... 
	//------------------------------------------------------------

	l_ul_CallbackMask = m_ul_CallbackMask;
	l_ul_CallbackMask &= ~MORPHO_CALLBACK_ENROLLMENT_CMD;

	if (m_tbCtrl.m_TabOptionsDlg->m_b_MainViewerActive)
	{
		m_i_CurrentViewerIndex = 0;
		m_px_Image = new C_MORPHO_Image(1);
	}
	//-----------------------------------------------------------
	// Get security Level Added by M.EL KHACHBI
	//-----------------------------------------------------------
	//m_x_MorphoSmart.Set_FFD_SecurityLevel();
	GetSecurityLevel(0);
	m_x_MorphoSmart.Set_FFD_SecurityLevel(m_i_SecurityLevel);

	//-----------------------------------------------------------
	// FORCE FINGER ON TOP DETECT MODE
	//-----------------------------------------------------------
	if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_ForceFingerPlacement)
	{
		l_ul_DetectModeChoice = MORPHO_ENROLL_DETECT_MODE;
		l_ul_DetectModeChoice |= MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE;
	}
	else
	{
		l_ul_DetectModeChoice = MORPHO_VERIF_DETECT_MODE;
	}

	l_ul_DetectModeChoice = l_ul_DetectModeChoice | m_ul_WakeUpMode;

	if (l_ul_DetectModeChoice > 0xFF)
	{
		l_cs_Message.Format("DetectModeChoice overfloat 0x%08x>0xFF(UC), information lost.", l_ul_DetectModeChoice);
		AfxMessageBox(l_cs_Message, MB_ICONERROR);
	}

	//------------------------------------------------------------
	// Strategy Acquisition Mode
	//------------------------------------------------------------	

	l_i_Ret = m_x_MorphoSmart.SetStrategyAcquisitionMode(m_tbCtrl.m_TabBioSettingsDlg->m_uc_StrategyAcquisitionMode); //FVP 1.10
	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling C_MORPHO_Device::SetStrategyAcquisitionMode() function", l_i_Ret);
		return l_i_Ret;
	}

	//------------------------------------------------------------
	// Identify
	//------------------------------------------------------------
	m_b_BiometricOperationProcessing = TRUE;
	l_i_Ret = m_x_Database.Identify(
		m_tbCtrl.m_TabBioSettingsDlg->m_i_Timeout,						//	US							i_us_Timeout,
		(T_MORPHO_FAR)m_tbCtrl.m_TabBioSettingsDlg->m_i_Threshold,		//	T_MORPHO_FAR				i_us_FAR,
		l_ul_CallbackMask,					//	UL							i_ul_CallbackCmd,
		BiometricCallback,					//	T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
		this,								//	PVOID						i_pv_CallbackArgument,
		l_x_User,							//	C_MORPHO_User &				o_x_User
		NULL,								//	PUL							o_pul_MatchingScore
		m_tbCtrl.m_TabBioSettingsDlg->m_i_CoderChoice,					//	I							i_i_CoderChoice
		l_ul_DetectModeChoice,
		m_tbCtrl.m_TabBioSettingsDlg->m_i_MatchingStrategy
		);
	

	// Get FFD logs and write in log file
	GetAndWriteFFDLogs(&m_x_MorphoSmart);

	m_b_BiometricOperationProcessing = FALSE;
	CleanAsyncMessages();
#if 0
	// test
	C_MORPHO_User l_user_test;//(l_x_User);

	l_user_test = l_x_User;
	C_MORPHO_User l_user_test2(l_x_User);
	// fin de test
#endif
	if (l_i_Ret != MORPHO_OK)
	{
		ShowAnimation("image_Ko");
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_x_ResultDlg.m_cs_Msg1.Format("Identification Failed");

		switch (l_i_Ret)
		{
		case MORPHOERR_CMDE_ABORTED:
			l_x_ResultDlg.m_cs_Msg2.Format("Command Aborted.");
			break;
		case MORPHOERR_FFD:
		case MORPHOERR_MOIST_FINGER:
		case MORPHOERR_MOVED_FINGER:
		case MORPHOERR_SATURATED_FINGER:
		case MORPHOERR_INVALID_FINGER:
		case MORPHOERR_NO_HIT:
			l_x_ResultDlg.m_cs_Msg2.Format("Person not identified.");
			break;
		case MORPHOERR_DB_EMPTY:
			l_x_ResultDlg.m_cs_Msg2.Format("The database is empty !");
			break;
		case MORPHOERR_TIMEOUT:
			l_x_ResultDlg.m_cs_Msg2.Format("Timeout has expired.");
			l_x_ResultDlg.m_cs_Msg3.Format("Command Aborted.");
			break;
		case MORPHOERR_NOT_IMPLEMENTED:
			l_x_ResultDlg.m_cs_Msg2.Format("Not Implemented !!!");
			break;
		case MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN:
			l_x_ResultDlg.m_cs_Msg2.Format("Finger is misplaced or has been");
			l_x_ResultDlg.m_cs_Msg3.Format("withdrawn from sensor during acquisition.");
			break;
		case MORPHOERR_BADPARAMETER:
			l_x_ResultDlg.m_cs_Msg2.Format("One or more input parameters are out of range");
			break;
		default:
			l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
			l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Database::Identify() function");
			ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
		}

		l_b_Ret = FALSE;

		if (m_b_EndThread == TRUE)
		{
			l_x_ResultDlg.DoModal();
		}
	}
	else
	{
		ShowAnimation("image_Ok");
		ShowUserData(l_x_User);

		l_b_Ret = TRUE;
	}

	if (m_px_Image != NULL)
	{
		delete(m_px_Image);
		m_px_Image = NULL;
	}

	CleanAsyncMessages2();
	EnableButtons();
	InitQualityBar();
	InitGuidingAnimations();

	RedrawWnd();
	return l_b_Ret;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::_Enroll()
{
	C_MORPHO_User	l_x_User;
	UC				l_uc_lenBuf;
	UL				l_ul_lenBuf;
	PUC				l_puc_Buf = new UC[1024];
	I				l_i_Ret;
	I				l_i_EtatEnroll;
	CString			l_cs_Message;
	bool			l_b_ReturnCode;
	UL				l_ul_CallbackMask;
	CResultDlg		l_x_ResultDlg;

	UL				            l_ul_DetectModeChoice;
	UC				            l_uc_DetectModeChoice;

	T_MORPHO_COMPRESS_ALGO		l_x_CompressAlgo = MORPHO_NO_COMPRESS;
	UC                          l_uc_CompressRate; //= 0;
	T_MORPHO_TYPE_TEMPLATE      l_x_TypeTemplate;
	T_MORPHO_FVP_TYPE_TEMPLATE	l_x_FVPTemplateType; // = MORPHO_NO_PK_FVP;
	UC	                        l_uc_SaveRecord;
	UC							l_uc_ExportMinutiae;
	C_MORPHO_TemplateList  		l_x_Template;
	T_MORPHO_IMAGE*				l_px_morphoImage;
	PT_MORPHO_MOC_PARAMETERS	l_px_MocParameters = NULL;

	CString		l_cs_Tmp;
	CString		l_cs_Tmp_FVP;
	CString		l_cs_SaveFile;

	I			l_i_Tmp;
	PC			l_auc_PersonnalInformation = new C[1024];
	UL			l_ul_PersonnalInformationSize;

	I                       i;
	UC                      l_uc_NbFingers;
	UL                      l_ul_LenTemplate;
	PUC                     l_puc_DataTemplate;
	PUC						l_auc_FileBuffer = new UC[2048];
	UL						l_ul_FileBufferSize;
	PUC						l_auc_FileBuffer2 = new UC[2048];
	UL						l_ul_FileBufferSize2;
	CString					l_cs_FileID;
	CString					l_cs_FileName;
	UC						l_uc_AdvancedSecurityLevelsRequired;

	// Privacy Mode Decrypt buffer
	PUC			o_ppuc_Aes128CbcDeCryptedData = NULL;
	UL			o_pul_Aes128CbcDeCryptedDataLen = 0;
	UI			l_i_Random = 0;
	std::string	sl_encrypt_Key;
	bool		RetKey = false;

	BOOL	isPrivacyEnabled = (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED || m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED);

	l_i_EtatEnroll = 0;

	// Personnal Information :
	// - Size ID		: 2 bytes
	// - ID				: <Size ID> bytes
	// - Size Firstname : 2 bytes
	// - Firstname		: <Size Firstname> bytes
	// - Size Lastname	: 2 bytes
	// - Lastname		: <Size Lastname> bytes

	//Size ID
	l_i_Tmp = m_BioOpPSheet.m_EnrollPPage.m_cs_ID.GetLength();
	memcpy(l_auc_PersonnalInformation, &l_i_Tmp, 2);
	l_ul_PersonnalInformationSize = 2;

	//ID
	memcpy((PC)l_auc_PersonnalInformation + l_ul_PersonnalInformationSize, m_BioOpPSheet.m_EnrollPPage.m_cs_ID, l_i_Tmp);
	l_ul_PersonnalInformationSize += l_i_Tmp;

	//Size Firstname
	l_i_Tmp = m_BioOpPSheet.m_EnrollPPage.m_cs_Firstname.GetLength();
	memcpy(l_auc_PersonnalInformation + l_ul_PersonnalInformationSize, &l_i_Tmp, 2);
	l_ul_PersonnalInformationSize += 2;

	//Firstname
	memcpy((PC)l_auc_PersonnalInformation + l_ul_PersonnalInformationSize, m_BioOpPSheet.m_EnrollPPage.m_cs_Firstname, l_i_Tmp);
	l_ul_PersonnalInformationSize += l_i_Tmp;

	//Size Lastname
	l_i_Tmp = m_BioOpPSheet.m_EnrollPPage.m_cs_Lastname.GetLength();
	memcpy(l_auc_PersonnalInformation + l_ul_PersonnalInformationSize, &l_i_Tmp, 2);
	l_ul_PersonnalInformationSize += 2;

	//Lastname
	memcpy((PC)l_auc_PersonnalInformation + l_ul_PersonnalInformationSize, m_BioOpPSheet.m_EnrollPPage.m_cs_Lastname, l_i_Tmp);
	l_ul_PersonnalInformationSize += l_i_Tmp;


	//------------------------------------------------------------
	// Check Parameters
	//------------------------------------------------------------

	if (m_BioOpPSheet.m_EnrollPPage.m_cs_ID.Compare("") == 0)
	{
		AfxMessageBox("You must enter an ID.");
		delete l_auc_FileBuffer;
		delete l_auc_FileBuffer2;
		delete l_auc_PersonnalInformation;
		delete l_puc_Buf;
		return FALSE;
	}

	if ((m_BioOpPSheet.m_EnrollPPage.m_i_NbFingers != 0) && (m_BioOpPSheet.m_EnrollPPage.m_i_NbFingers != 1))
	{
		AfxMessageBox("Choose 1 or 2 fingers");
		delete l_auc_FileBuffer;
		delete l_auc_FileBuffer2;
		delete l_auc_PersonnalInformation;
		delete l_puc_Buf;
		return FALSE;
	}
	//------------------------------------------------------------
	// Does not allow other operations
	//------------------------------------------------------------

	DisableButtons();
	//-----------------------------------------------------------
	// Get security Level Added by M.EL KHACHBI
	//-----------------------------------------------------------
	//m_x_MorphoSmart.Set_FFD_SecurityLevel();
	GetSecurityLevel(1);
	m_x_MorphoSmart.Set_FFD_SecurityLevel(m_i_SecurityLevel);

	l_uc_NbFingers = m_BioOpPSheet.m_EnrollPPage.m_i_NbFingers + 1;
	//------------------------------------------------------------
	// Saving in the Data Base case
	//------------------------------------------------------------
	//   if(m_BioOpPSheet.m_EnrollPPage.m_b_SavePkInDB)
	//{
	l_x_CompressAlgo = MORPHO_NO_COMPRESS;
	l_uc_CompressRate = 0;
	l_uc_ExportMinutiae = 0;
	l_x_TypeTemplate = MORPHO_PK_COMP;
	l_x_FVPTemplateType = MORPHO_NO_PK_FVP;
	l_uc_SaveRecord = 1;
	l_px_MocParameters = NULL;

	//------------------------------------------------------------
	// Associate the User Object with the Database Object
	//------------------------------------------------------------

	l_uc_lenBuf = m_BioOpPSheet.m_EnrollPPage.m_cs_ID.GetLength();

	if (isPrivacyEnabled)
	{
		// Getting AES128 Key
		l_i_Random = time(NULL);
		RetKey = GetEncryptedKey(sl_encrypt_Key);
		if (!RetKey)
		{
			AfxMessageBox("Error occured when loading encryption key.");
			delete l_auc_FileBuffer;
			delete l_auc_FileBuffer2;
			delete l_auc_PersonnalInformation;
			delete l_puc_Buf;
			return FALSE;
		}
	}

	if (isPrivacyEnabled)
	{
		_PreparePrivacyData(m_BioOpPSheet.m_EnrollPPage.m_cs_ID.GetBuffer(l_uc_lenBuf), l_uc_lenBuf, sl_encrypt_Key, l_i_Random, TRUE, l_puc_Buf, (UL*)&l_uc_lenBuf);
	}
	else
	{
		memcpy(l_puc_Buf, m_BioOpPSheet.m_EnrollPPage.m_cs_ID.GetBuffer(l_uc_lenBuf), l_uc_lenBuf);
		l_puc_Buf[l_uc_lenBuf++] = 0;
	}

	l_i_Ret = m_x_Database.GetUser(
		l_uc_lenBuf,	// UC			i_uc_lenUserID,
		l_puc_Buf,		// PUC			i_puc_UserID,
		l_x_User			// C_MORPHO_User & o_x_User 
		);

	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling C_MORPHO_Database::GetUser() function", l_i_Ret);
		EnableButtons();
		InitQualityBar();
		InitGuidingAnimations();
		delete l_auc_FileBuffer;
		delete l_auc_FileBuffer2;
		delete l_auc_PersonnalInformation;
		delete l_puc_Buf;
		return FALSE;
	}

	//------------------------------------------------------------
	// Additionnal Fields : Lastname & Firstname
	//------------------------------------------------------------

	//- - - - - - - - - -
	// Firstname
	//- - - - - - - - - -

	l_ul_lenBuf = m_BioOpPSheet.m_EnrollPPage.m_cs_Firstname.GetLength();
	if (isPrivacyEnabled)
	{
		_PreparePrivacyData(m_BioOpPSheet.m_EnrollPPage.m_cs_Firstname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf, sl_encrypt_Key, l_i_Random, TRUE, l_puc_Buf, (UL*)&l_ul_lenBuf);
	}
	else
	{
		memcpy(l_puc_Buf, m_BioOpPSheet.m_EnrollPPage.m_cs_Firstname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf);
		l_puc_Buf[l_ul_lenBuf++] = 0;
	}

	// Provide a field content length of 0 when providing an empty string in case of a fingerprint update to allow preserving previously set data
	if ((m_BioOpPSheet.m_EnrollPPage.m_b_updateFinger) && (l_puc_Buf[0] == 0))
	{
		l_ul_lenBuf = 0;
	}

	l_i_Ret = l_x_User.PutField(
		1,						// UL		i_ul_indexField,
		l_ul_lenBuf,			// UL		i_ul_lenField,
		l_puc_Buf				// PUC		i_puc_dataField);
		);

	//- - - - - - - - - -
	// Lastname
	//- - - - - - - - - -

	if (l_i_Ret == MORPHO_OK)
	{
		l_ul_lenBuf = m_BioOpPSheet.m_EnrollPPage.m_cs_Lastname.GetLength();
		if (isPrivacyEnabled)
		{
			_PreparePrivacyData(m_BioOpPSheet.m_EnrollPPage.m_cs_Lastname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf, sl_encrypt_Key, l_i_Random, TRUE, l_puc_Buf, (UL*)&l_ul_lenBuf);
		}
		else
		{
			memcpy(l_puc_Buf, m_BioOpPSheet.m_EnrollPPage.m_cs_Lastname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf);
			l_puc_Buf[l_ul_lenBuf++] = 0;
		}

		// Provide a field content length of 0 when providing an empty string in case of a fingerprint update to allow preserving previously set data
		if ((m_BioOpPSheet.m_EnrollPPage.m_b_updateFinger) && (l_puc_Buf[0] == 0))
		{
			l_ul_lenBuf = 0;
		}

		l_i_Ret = l_x_User.PutField(
			2,						// UL		i_ul_indexField,
			l_ul_lenBuf,			// UL		i_ul_lenField,
			l_puc_Buf				// PUC		i_puc_dataField);
			);						// I		i_i_indexField,
	}

	//- - - - - - - - - -
	// Error cases
	//- - - - - - - - - -

	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling C_MORPHO_User::PutField() function", l_i_Ret);
		EnableButtons();
		InitQualityBar();
		InitGuidingAnimations();
		delete l_auc_FileBuffer;
		delete l_auc_FileBuffer2;
		delete l_auc_PersonnalInformation;
		delete l_puc_Buf;
		return FALSE;
	}
	//}
	//------------------------------------------------------------
	// End of saving in the Data Base case
	//------------------------------------------------------------

	//------------------------------------------------------------
	// saving in file case
	//------------------------------------------------------------

	if (!m_BioOpPSheet.m_EnrollPPage.m_b_SavePkInDB)
	{
		l_uc_SaveRecord = 0;
	}
	else
	{
		l_uc_SaveRecord = 1;
	}

	//----------------------
	// FP aquisition enabled
	//----------------------

	if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType != STR_NO_PK_FP)
	{
		if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_PKCOMP)
		{
			l_x_TypeTemplate = MORPHO_PK_COMP;
			l_cs_Tmp = "PK_COMP Files (*.pkcEXT_CRYPT)|*.pkcEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_PKCOMPNORM)
		{
			l_x_TypeTemplate = MORPHO_PK_COMP_NORM;
			l_cs_Tmp = "PK_COMP_NORM Files (*.pkcnEXT_CRYPT)|*.pkcnEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_PKMAT)
		{
			l_x_TypeTemplate = MORPHO_PK_MAT;
			l_cs_Tmp = "PK_MAT Files (*.pkmEXT_CRYPT)|*.pkmEXT_CRYPT|PK_MAT Files (*.pkmatEXT_CRYPT)|*.pkmatEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_PKMATNORM)
		{
			l_x_TypeTemplate = MORPHO_PK_MAT_NORM;
			l_cs_Tmp = "PK_MAT_NORM Files (*.pkmnEXT_CRYPT)|*.pkmnEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_PKLITE)
		{
			l_x_TypeTemplate = MORPHO_PK_PKLITE;
			l_cs_Tmp = "PKLITE Files (*.pklEXT_CRYPT)|*.pklEXT_CRYPT|PKLITE Files (*.pkliteEXT_CRYPT)|*.pkliteEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_PKS)
		{
			l_x_TypeTemplate = MORPHO_PK_COMP;
			l_cs_Tmp = "PKs Files (*.pksEXT_CRYPT)|*.pksEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_ANSI378)
		{
			l_x_TypeTemplate = MORPHO_PK_ANSI_378;
			l_cs_Tmp = "ANSI 378 Files Files(*.ansi-fmrEXT_CRYPT)|*.ansi-fmrEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_ANSI378_2009)
		{
			l_x_TypeTemplate = MORPHO_PK_ANSI_378_2009;
			l_cs_Tmp = "ANSI 378 v.2009 Files Files(*.ansi-fmr-2009EXT_CRYPT)|*.ansi-fmr-2009EXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_ISO_FMC_CS)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMC_CS;
			l_cs_Tmp = "ISO 19794-2 FMC-CS Files(*.iso-fmc-csEXT_CRYPT)|*.iso-fmc-csEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_ISO_FMC_NS)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMC_NS;
			l_cs_Tmp = "ISO 19794-2 FMC-NS Files(*.iso-fmc-nsEXT_CRYPT)|*.iso-fmc-nsEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_ISO_FMR)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMR;
			l_cs_Tmp = "ISO 19794-2 Files(*.iso-fmrEXT_CRYPT)|*.iso-fmrEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_ISO_FMR_2011)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMR_2011;
			l_cs_Tmp = "ISO 19794-2 Files(*.iso-fmr-2011EXT_CRYPT)|*.iso-fmr-2011EXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_MINEX_A)
		{
			l_x_TypeTemplate = MORPHO_PK_MINEX_A;
			l_cs_Tmp = "MINEX A Files (*.minex-aEXT_CRYPT)|*.minex-aEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_PK_MOC)
		{
			l_x_TypeTemplate = MORPHO_PK_MOC;
			l_cs_Tmp = "SAGEM PKMOC (*.mocEXT_CRYPT)|*.mocEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_PK_DIN_V66400_CS)
		{
			l_x_TypeTemplate = MORPHO_PK_DIN_V66400_CS;
			l_cs_Tmp = "DIN-V66400 Compact Size400 Compact Size (*.din-csEXT_CRYPT)|*.din-csEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_PK_DIN_V66400_CS_AA)
		{
			l_x_TypeTemplate = MORPHO_PK_DIN_V66400_CS_AA;
			l_cs_Tmp = "DIN-V66400 Compact Size400 Compact Size (*.din-csEXT_CRYPT)|*.din-csEXT_CRYPT|All Files (*.*)|*.*||";
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_ISO_FMC_CS_AA)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMC_CS_AA;
			l_cs_Tmp = "ISO 19794-2 FMC-CS-AA Files(*.iso-fmc-csEXT_CRYPT)|*.iso-fmc-csEXT_CRYPT|All Files (*.*)|*.*||";
		}
	}
	else
	{
		if (m_bl_FVP == FALSE)
		{
			l_x_TypeTemplate = MORPHO_PK_COMP;  //	MSO_Demo always  export templates PK_COMP
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "PKs Files (*.pksEXT_CRYPT)|*.pksEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else
		{
			l_x_TypeTemplate = MORPHO_NO_PK_FP;
		}
	}

	//----------------------
	// FVP aquisition enabled
	//----------------------
	if (m_bl_FVP)
	{
		//FVP 1.6
		/*if(m_BioOpPSheet.m_EnrollPPage.m_cs_FVP_TemplateType == STR_PK_FVP_MATCH){
		l_x_FVPTemplateType = MORPHO_PK_FVP_MATCH;
		l_cs_Tmp_FVP = "SAGEM PkFVP Match (*.fvp-m)|*.fvp-m|All Files (*.*)|*.*||";
		}else{*/
		l_x_FVPTemplateType = MORPHO_PK_FVP;
		l_cs_Tmp_FVP = "SAGEM PkFVP (*.fvp)|*.fvp|All Files (*.*)|*.*||";
		//}
	}
	else
	{
		l_x_FVPTemplateType = MORPHO_NO_PK_FVP;
	}

	if (l_x_TypeTemplate != MORPHO_NO_PK_FP || l_x_FVPTemplateType != MORPHO_NO_PK_FVP)
	{
		// le flag export de PK est positionné
		l_uc_ExportMinutiae = 1;
	}


	//----------------------
	// Image aquisition enabled
	//----------------------
	CString					l_cs_ISO19794_FIR_ImageExtension = "";
	T_MORPHO_COMPRESS_ALGO	l_x_DesiredISO19794_FIR_CompressAlgo;

	if (m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType != STR_NO_IMAGE)
	{
		l_x_Template.SetActiveFullImageRetrieving(true);

		if (m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == "WSQ")
		{
			l_x_CompressAlgo = MORPHO_COMPRESS_WSQ;
			l_uc_CompressRate = 15;
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == "RAW")
		{
			l_x_CompressAlgo = MORPHO_NO_COMPRESS;
			l_uc_CompressRate = 0;
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == "SAGEM_V1")
		{
			l_x_CompressAlgo = MORPHO_COMPRESS_V1;
			l_uc_CompressRate = 0;
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == STR_ISO19794_FIR_RAW)
		{
			l_x_CompressAlgo = MORPHO_NO_COMPRESS;
			l_uc_CompressRate = 0;
			l_cs_ISO19794_FIR_ImageExtension = "RAW";
			l_x_DesiredISO19794_FIR_CompressAlgo = MORPHO_COMPRESS_UNCHANGED;//because we have it already in RAW
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == STR_ISO19794_FIR_BMP)
		{
			l_x_CompressAlgo = MORPHO_NO_COMPRESS;
			l_uc_CompressRate = 0;
			l_cs_ISO19794_FIR_ImageExtension = "BMP";
			l_x_DesiredISO19794_FIR_CompressAlgo = MORPHO_COMPRESS_BMP;
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == STR_ISO19794_FIR_WSQ)
		{
			l_x_CompressAlgo = MORPHO_NO_COMPRESS;
			l_uc_CompressRate = 0;
			l_cs_ISO19794_FIR_ImageExtension = "WSQ";
			l_x_DesiredISO19794_FIR_CompressAlgo = MORPHO_COMPRESS_WSQ;
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == STR_ISO19794_FIR_JPEG)
		{
			l_x_CompressAlgo = MORPHO_NO_COMPRESS;
			l_uc_CompressRate = 0;
			l_cs_ISO19794_FIR_ImageExtension = "JPG";
			l_x_DesiredISO19794_FIR_CompressAlgo = MORPHO_COMPRESS_JPEG;
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == STR_ISO19794_FIR_JPEG2000)
		{
			l_x_CompressAlgo = MORPHO_NO_COMPRESS;
			l_uc_CompressRate = 0;
			l_cs_ISO19794_FIR_ImageExtension = "JP2";
			l_x_DesiredISO19794_FIR_CompressAlgo = MORPHO_COMPRESS_JPEG2000;
		}
		else if (m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == STR_ISO19794_FIR_PNG)
		{
			l_x_CompressAlgo = MORPHO_NO_COMPRESS;
			l_uc_CompressRate = 0;
			l_cs_ISO19794_FIR_ImageExtension = "PNG";
			l_x_DesiredISO19794_FIR_CompressAlgo = MORPHO_COMPRESS_PNG;
		}
		else
		{
			l_x_Template.SetActiveFullImageRetrieving(false);
		}
	}
	//----------------------
	// Image aquisition disabled
	//----------------------
	else
	{
		l_x_CompressAlgo = MORPHO_NO_COMPRESS;
		l_uc_CompressRate = 0;
		l_x_Template.SetActiveFullImageRetrieving(false);
	}


	if (l_cs_ISO19794_FIR_ImageExtension != "" && m_IsDataEncryption)
	{
		AfxMessageBox("The selected image format is not supported with data encryption!!");
		EnableButtons();
		delete l_auc_FileBuffer;
		delete l_auc_FileBuffer2;
		delete l_auc_PersonnalInformation;
		delete l_puc_Buf;
		return FALSE;
	}

	//------------------------------------------------------------
	// Viewer or not viewer.... 
	//------------------------------------------------------------

	l_ul_CallbackMask = m_ul_CallbackMask;

	if (m_tbCtrl.m_TabOptionsDlg->m_b_MainViewerActive)
	{
		m_px_Image = new C_MORPHO_Image(6);
	}

	if (m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
	{
		PostMessage(WM_ADD_PROCESS_PAGE, 3, 0);
	}
	else
	{
		m_ctl_CurrentMiniViewer = NULL;
	}

	//------------------------------------------------------------
	// Detect Mode Choice 
	//------------------------------------------------------------
	l_ul_DetectModeChoice = MORPHO_ENROLL_DETECT_MODE;
	if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_ForceFingerPlacement != 0)
	{
		l_ul_DetectModeChoice |= MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE;
	}

	l_ul_DetectModeChoice = l_ul_DetectModeChoice | m_ul_WakeUpMode;

	if (l_ul_DetectModeChoice > 0xFF)
	{
		l_cs_Message.Format("DetectModeChoice overfloat 0x%08x>0xFF(UC), information lost.", l_ul_DetectModeChoice);
		AfxMessageBox(l_cs_Message, MB_ICONERROR);
	}

	l_uc_DetectModeChoice = (UC)l_ul_DetectModeChoice;

	//------------------------------------------------------------
	// Advanced Security Levels Compatibility Required 
	//------------------------------------------------------------

	if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_ASLC)
	{
		l_uc_AdvancedSecurityLevelsRequired = 1;
	}
	else
	{
		l_uc_AdvancedSecurityLevelsRequired = 0;
	}

	//------------------------------------------------------------
	// Strategy Acquisition Mode
	//------------------------------------------------------------	

	l_i_Ret = m_x_MorphoSmart.SetStrategyAcquisitionMode(m_tbCtrl.m_TabBioSettingsDlg->m_uc_StrategyAcquisitionMode); //FVP 1.10
	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling C_MORPHO_Device::SetStrategyAcquisitionMode() function", l_i_Ret);
		delete l_auc_FileBuffer;
		delete l_auc_FileBuffer2;
		delete l_auc_PersonnalInformation;
		delete l_puc_Buf;
		return l_i_Ret;
	}

	//------------------------------------------------------------
	// Enroll
	//------------------------------------------------------------	

	m_b_BiometricOperationProcessing = TRUE;
	CString		l_cs_FingerQuality;
	I			l_i_FingerQuality;
	l_i_FingerQuality = 0;

	if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_FingerPrint_Quality)
	{
		m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_EDIT_FINGERPRINT_QUALITY)->GetWindowText(l_cs_FingerQuality);
		l_i_FingerQuality = atoi(l_cs_FingerQuality);
	}

	if ((!l_x_Template.GetActiveFullImageRetrieving()) && (l_uc_SaveRecord == 0) && (l_x_TypeTemplate == MORPHO_NO_PK_FP) && (l_x_FVPTemplateType == MORPHO_NO_PK_FVP))
	{
		l_i_Ret = MORPHOERR_BADPARAMETER;
	}
	else
	{
		// Prepare finger update if necessary
		if ((l_uc_SaveRecord == 1) && (m_BioOpPSheet.m_EnrollPPage.m_b_updateFinger))
		{
			// Update both fingers
			if (m_BioOpPSheet.m_EnrollPPage.m_i_NbFingers == 1)
			{
				l_i_Ret = l_x_User.SetTemplateUpdateMask(3);
			}
			// Update first finger only
			else if (m_BioOpPSheet.m_EnrollPPage.m_str_selectedUpdateIndex == STR_FIRST_FINGER)
			{
				l_i_Ret = l_x_User.SetTemplateUpdateMask(1);
			}
			// Update second finger
			else
			{
				l_i_Ret = l_x_User.SetTemplateUpdateMask(2);
			}
		}

		if (l_i_Ret == MORPHO_OK)
		{
			l_i_Ret = l_x_User.Enroll(
				m_tbCtrl.m_TabBioSettingsDlg->m_i_Timeout,		// US							i_us_Timeout,
				(UC)l_i_FingerQuality,							// UC							i_uc_AcquisitionThreshold
				l_uc_AdvancedSecurityLevelsRequired,			// UC							i_uc_AdvancedSecurityLevelsRequired
				l_x_CompressAlgo,                               // T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
				l_uc_CompressRate,								// UC							i_uc_CompressRate,
				l_uc_ExportMinutiae,							// UC							i_uc_ExportMinutiae,
				m_BioOpPSheet.m_EnrollPPage.m_i_NbFingers + 1,	// UC							i_uc_FingerNumber,
				l_x_TypeTemplate,								// T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				l_x_FVPTemplateType,                            // T_MORPHO_FVP_TYPE_TEMPLATE	i_x_FVPTemplateType,
				l_uc_SaveRecord,                                // UC							i_uc_SaveRecord,
				l_ul_CallbackMask,								// UL							i_ul_CallbackCmd,
				BiometricCallback,								// T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
				this,											// PVOID						i_pv_CallbackArgument,
				m_tbCtrl.m_TabBioSettingsDlg->m_i_CoderChoice,	// I						    i_i_CoderChoice,
				(UC)(l_uc_DetectModeChoice | m_ul_WakeUpMode),    // UL         					i_ul_DetectModeChoice,
				l_x_Template,                                   // C_MORPHO_TemplateList &		o_x_Template,	
				l_px_MocParameters                              // PT_MORPHO_MOC_PARAMETERS		i_px_MocParameters
				);

			// Get FFD logs and write in log file
			GetAndWriteFFDLogs(&m_x_MorphoSmart);
		}
	}
	/*	l_i_Ret = l_x_User.Enroll(	m_i_Timeout,					// US							i_us_Timeout,
	m_BioOpPSheet.m_EnrollPPage.m_i_NbFingers + 1,	// UC							i_uc_FingerNumber,
	MORPHO_PK_COMP,									// T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
	l_ul_CallbackMask,								// UL							i_ul_CallbackCmd,
	BiometricCallback,								// T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
	this,											// PVOID						i_pv_CallbackArgument);
	GetCoderChoice(),								//I							i_i_CoderChoice
	MORPHO_ENROLL_DETECT_MODE|m_ul_WakeUpMode
	);*/

	m_b_BiometricOperationProcessing = FALSE;
	CleanAsyncMessages();

	//------------------------------------------------
	// Remove handle on the Miniviewer 
	if (m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
		m_ctl_CurrentMiniViewer = NULL;
	//------------------------------------------------


	l_i_EtatEnroll = l_i_Ret;

	//------------------------------------------------------------
	// Display Result Dialog
	//------------------------------------------------------------
	bool _decrypt = false;
	if (l_i_Ret != MORPHO_OK) // if not ok
	{
		ShowAnimation("image_Ko");
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_x_ResultDlg.m_cs_Msg1.Format("Enrollment failed");
		switch (l_i_Ret)
		{
		case MORPHOERR_CMDE_ABORTED:
			l_x_ResultDlg.m_cs_Msg2.Format("Command Aborted.");
			break;
		case MORPHOERR_NO_HIT:
			l_x_ResultDlg.m_cs_Msg2.Format("Bad Capture Sequence.");
			break;
		case MORPHOERR_ALREADY_ENROLLED:
			l_x_ResultDlg.m_cs_Msg2.Format("User already enrolled.");
			break;
		case MORPHOERR_INVALID_USER_ID:
			l_x_ResultDlg.m_cs_Msg2.Format("ID %s already used.", m_BioOpPSheet.m_EnrollPPage.m_cs_ID);
			break;
		case MORPHOERR_TIMEOUT:
			l_x_ResultDlg.m_cs_Msg2.Format("Timeout has expired.");
			l_x_ResultDlg.m_cs_Msg3.Format("Command aborted.");
			break;
		case MORPHOERR_DB_FULL:
			l_x_ResultDlg.m_cs_Msg2.Format("Cannot enroll more users.");
			l_x_ResultDlg.m_cs_Msg3.Format("Database is full.");
			break;
		case MORPHOERR_FFD:
		case MORPHOERR_MOIST_FINGER:
		case MORPHOERR_MOVED_FINGER:
		case MORPHOERR_SATURATED_FINGER:
		case MORPHOERR_INVALID_FINGER:
			l_x_ResultDlg.m_cs_Msg2.Format("Warning : Low quality finger !!!");
			break;
		case MORPHOERR_BADPARAMETER:
			l_x_ResultDlg.m_cs_Msg2.Format("One or more input parameters are out of range");
			//ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
			break;
		case MORPHOERR_LICENSE_MISSING:
			l_x_ResultDlg.m_cs_Msg2.Format("A required license is missing.");
			break;
		case MORPHOERR_ADVANCED_SECURITY_LEVEL_MISMATCH:
			l_x_ResultDlg.m_cs_Msg2.Format("Failed to make a multimodal template compatible with advanced security levels.");
			break;
		case MORPHOERR_BAD_FINAL_FINGER_PRINT_QUALITY:
			l_x_ResultDlg.m_cs_Msg2.Format("Failed to reach the fingerprint quality threshold.");
			break;
		default:
			l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
			l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_User::Enroll() function");
			ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
		}

		l_b_ReturnCode = FALSE;
		l_x_ResultDlg.DoModal();
		CleanAsyncMessages2();
	}
	else //ok case
	{
		ShowAnimation("image_Ok");

		if (isPrivacyEnabled)
		{
			if (AfxMessageBox("Decrypt Template ?", MB_YESNO) == IDYES)
			{
				_decrypt = true;
			}
		}

		if (m_IsDataEncryption && !m_bl_FVP)
		{
			l_cs_Tmp.Replace("EXT_CRYPT", ".crypt");
		}
		else
		{
			l_cs_Tmp.Replace("EXT_CRYPT", "");
		}

		if (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_PKS)
		{
			if (m_bl_FVP == TRUE && m_BioOpPSheet.m_EnrollPPage.m_cs_FVP_TemplateType != STR_NO_FVP)
			{
				for (i = 0; i<l_uc_NbFingers; ++i)
				{
					UC l_uc_dataIndex;
					UC l_uc_PkFpQuality;
					UC l_uc_AdvancedSecurityLevelsCompatibility;
					l_i_Ret = l_x_Template.GetFVPTemplate(
						i,										// UC	i_uc_indexTemplate,
						l_x_FVPTemplateType,					// T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
						l_ul_LenTemplate,						// UL	&o_ul_lenTemplate,
						l_puc_DataTemplate,						// PUC	&o_puc_dataTemplate
						l_uc_PkFpQuality,
						l_uc_AdvancedSecurityLevelsCompatibility,
						l_uc_dataIndex
						);

					if (l_i_Ret != MORPHO_OK){
						ShowErrorBox("An error occured while calling C_MORPHO_TemplateList::GetFVPTemplate() function", l_i_Ret);
						PostMessage(WM_REMOVE_PROCESS_PAGE, 0, (LPARAM)&(m_BioOpPSheet.m_EnrollPPage));
						delete l_auc_FileBuffer;
						delete l_auc_FileBuffer2;
						delete l_auc_PersonnalInformation;
						delete l_puc_Buf;
						return FALSE;
					}

					l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
					if (i == 0)
					{
						l_x_ResultDlg.m_cs_Msg2.Format("Finger #1 - Quality Score: %u  ", l_uc_PkFpQuality);
					}
					else
					{
						l_x_ResultDlg.m_cs_Msg2.Format("Finger #2 -  Quality Score: %u  ", l_uc_PkFpQuality);
					}

					l_x_ResultDlg.m_cs_Msg3.Format("Advanced Security Levels Compatibility: %s", (l_uc_AdvancedSecurityLevelsCompatibility == 1) ? "Yes" : "No");

					Beep(BEEP_FREQUENCY, BEEP_DURATION);

					l_x_ResultDlg.DoModal();
					l_x_ResultDlg.InitializeMsg();

					// Save first finger (Finger #1) FVP template in a buffer 
					if (i == 0)
					{
						memcpy(l_auc_FileBuffer, l_puc_DataTemplate, l_ul_LenTemplate);
						l_ul_FileBufferSize = l_ul_LenTemplate;
					}
					else if (i == 1)
					{// Save second finger (Finger #2) FVP template in an other buffer 
						memcpy(l_auc_FileBuffer2, l_puc_DataTemplate, l_ul_LenTemplate);
						l_ul_FileBufferSize2 = l_ul_LenTemplate;
					}//end if
					l_cs_FileID = m_BioOpPSheet.m_EnrollPPage.m_cs_ID + "_";
					if (i == 0)
					{ // "finger_1" (capture 1er doigt)
						l_cs_FileID += "finger_1";
					}
					else if (i == 1)
					{ // "finger_2" (capture 1er doigt)
						l_cs_FileID += "finger_2";
					}//end if						

					if (isPrivacyEnabled && !_decrypt)
					{ // "add privacy crypt to ext"
						l_cs_FileID += "_pcrypt";
					}
					CFileDialog l_x_SaveFileDlg(FALSE,
						"",
						l_cs_FileID,
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
						l_cs_Tmp_FVP,
						this);

					SAV_PATH

						if (l_x_SaveFileDlg.DoModal() != IDOK)
						{
							RESTORE_PATH
								l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
							l_x_ResultDlg.m_cs_Msg2.Format("Saving FVP template file");
							l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
							l_b_ReturnCode = FALSE;
						}
						else
						{
							RESTORE_PATH
								l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

							//- - - - - - - - - - - 
							// Write data in File
							//- - - - - - - - - - -

							CString l_cs_File;
							l_cs_File.Format("%s", l_cs_SaveFile);

							CFile l_f_PkFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
							if (i == 0) //"Finger #1" (first finger FVP template)
							{
								if (_decrypt == true)
								{
									bool b_decRet = _DecryptAESData(l_auc_FileBuffer, l_ul_FileBufferSize, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
									if (!b_decRet || o_pul_Aes128CbcDeCryptedDataLen == 0)
									{
										AfxMessageBox("Decrypting data error !", MB_ICONERROR);
										delete l_auc_FileBuffer;
										delete l_auc_FileBuffer2;
										delete l_auc_PersonnalInformation;
										delete l_puc_Buf;
										return FALSE;
									}

									if (!_CheckPrivacyData(o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
									{
										AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
										if (o_ppuc_Aes128CbcDeCryptedData)
											MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
										delete l_auc_FileBuffer;
										delete l_auc_FileBuffer2;
										delete l_auc_PersonnalInformation;
										delete l_puc_Buf;
										return FALSE;
									}

									if (b_decRet)
										l_f_PkFile.Write(o_ppuc_Aes128CbcDeCryptedData + 8, o_pul_Aes128CbcDeCryptedDataLen - 8);

									if (o_ppuc_Aes128CbcDeCryptedData)
										MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
								}
								else
									l_f_PkFile.Write(l_auc_FileBuffer, l_ul_FileBufferSize);

							}
							else if (i == 1)//"Finger #2" (second finger FVP template)
							{
								if (_decrypt == true)
								{
									bool b_decRet = _DecryptAESData(l_auc_FileBuffer2, l_ul_FileBufferSize2, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
									if (!b_decRet || o_pul_Aes128CbcDeCryptedDataLen == 0)
									{
										AfxMessageBox("Decrypting data error !", MB_ICONERROR);
										delete l_auc_FileBuffer;
										delete l_auc_FileBuffer2;
										delete l_auc_PersonnalInformation;
										delete l_puc_Buf;
										return FALSE;
									}
									if (!_CheckPrivacyData(o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
									{
										AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
										if (o_ppuc_Aes128CbcDeCryptedData)
											MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
										delete l_auc_FileBuffer;
										delete l_auc_FileBuffer2;
										delete l_auc_PersonnalInformation;
										delete l_puc_Buf;
										return FALSE;
									}

									if (b_decRet)
										l_f_PkFile.Write(o_ppuc_Aes128CbcDeCryptedData + 8, o_pul_Aes128CbcDeCryptedDataLen - 8);
									if (o_ppuc_Aes128CbcDeCryptedData)
										MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
								}
								else
									l_f_PkFile.Write(l_auc_FileBuffer2, l_ul_FileBufferSize2);
							}
							l_f_PkFile.Close();
							l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
							l_x_ResultDlg.m_cs_Msg3.Format("FVP Template successfully ");
							l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
							l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
						}
					l_x_ResultDlg.DoModal();
					l_x_ResultDlg.InitializeMsg();
				}
			}

			//---------------------
			// Pks File format
			//---------------------
			// the .pks file structure : 
			// - Personnal Information
			// - Nb of templates: 1 byte (1 or 2 fingers)
			// - Size 1st Tplate: 2 bytes
			// - 1st Template	: <Size 1st Tplate> bytes
			// - Size 2nd Tplate: 2 bytes					(if exists)
			// - 2nd Template	: <Size 2nd Tplate> bytes	(if exists)

			//Personnal Information
			memcpy(l_auc_FileBuffer, l_auc_PersonnalInformation, l_ul_PersonnalInformationSize);
			l_ul_FileBufferSize = l_ul_PersonnalInformationSize;

			//Nb of templates
			memcpy(l_auc_FileBuffer + l_ul_FileBufferSize, &l_uc_NbFingers, 1);
			l_ul_FileBufferSize += 1;

			// Save one or two finger templates in a same buffer
			for (i = 0; i<l_uc_NbFingers; ++i)
			{
				l_i_Ret = l_x_Template.GetTemplate(
					i,					// UC	i_uc_indexTemplate,
					l_x_TypeTemplate,	// T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
					l_ul_LenTemplate,	// UL	&o_ul_lenTemplate,
					l_puc_DataTemplate  // PUC	&o_puc_dataTemplate
					);

				if (l_i_Ret != MORPHO_OK)
				{
					ShowErrorBox("An error occured while calling C_MORPHO_TemplateList::GetTemplate() function", l_i_Ret);
					PostMessage(WM_REMOVE_PROCESS_PAGE, 0, (LPARAM)&(m_BioOpPSheet.m_EnrollPPage));
					delete l_auc_FileBuffer;
					delete l_auc_FileBuffer2;
					delete l_auc_PersonnalInformation;
					delete l_puc_Buf;
					return FALSE;
				}

				if (_decrypt == true || isPrivacyEnabled)
				{
					bool b_decRet = _DecryptAESData(l_puc_DataTemplate, l_ul_LenTemplate, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
					if (!b_decRet || o_pul_Aes128CbcDeCryptedDataLen == 0)
					{
						AfxMessageBox("Decrypting data error !", MB_ICONERROR);
						delete l_auc_FileBuffer;
						delete l_auc_FileBuffer2;
						delete l_auc_PersonnalInformation;
						delete l_puc_Buf;
						return FALSE;
					}

					if (!_CheckPrivacyData(o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
					{
						AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
						if (o_ppuc_Aes128CbcDeCryptedData)
							MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
						delete l_auc_FileBuffer;
						delete l_auc_FileBuffer2;
						delete l_auc_PersonnalInformation;
						delete l_puc_Buf;
						return FALSE;
					}
					o_pul_Aes128CbcDeCryptedDataLen -= 8;
					// Template Size
					memcpy(l_auc_FileBuffer + l_ul_FileBufferSize, &o_pul_Aes128CbcDeCryptedDataLen, 2);
					l_ul_FileBufferSize += 2;

					//Template
					memcpy(l_auc_FileBuffer + l_ul_FileBufferSize, o_ppuc_Aes128CbcDeCryptedData + 8, o_pul_Aes128CbcDeCryptedDataLen);
					l_ul_FileBufferSize += o_pul_Aes128CbcDeCryptedDataLen;

					if (o_ppuc_Aes128CbcDeCryptedData)
						MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
				}
				else
				{
					// Template Size
					memcpy(l_auc_FileBuffer + l_ul_FileBufferSize, &l_ul_LenTemplate, 2);
					l_ul_FileBufferSize += 2;

					//Template
					memcpy(l_auc_FileBuffer + l_ul_FileBufferSize, l_puc_DataTemplate, l_ul_LenTemplate);
					l_ul_FileBufferSize += l_ul_LenTemplate;
				}
			}

			if (isPrivacyEnabled && _decrypt == false)
			{
				UC		l_puc_Aes128CbcInputVector[AES128_BLOCK_LENGTH];
				UL		l_ul_Aes128CbcCryptedBufferSize = 0;
				PUC		l_puc_Aes128CbcCryptedBuffer = NULL;
				memset(l_puc_Aes128CbcInputVector, 0, AES128_BLOCK_LENGTH);

				I l_i_ret = MSOSECU_encrypt_aes128_cbc(sl_encrypt_Key.length(), (PUC)sl_encrypt_Key.c_str(),
					l_ul_FileBufferSize, (PUC)l_auc_FileBuffer,
					l_puc_Aes128CbcInputVector, TRUE,
					&l_ul_Aes128CbcCryptedBufferSize, &l_puc_Aes128CbcCryptedBuffer);

				memcpy(l_auc_FileBuffer, l_puc_Aes128CbcCryptedBuffer, l_ul_Aes128CbcCryptedBufferSize);

				l_ul_FileBufferSize = l_ul_Aes128CbcCryptedBufferSize;

				if (l_puc_Aes128CbcCryptedBuffer)
					MSOSECU_Free(&l_puc_Aes128CbcCryptedBuffer);
			}

			l_cs_FileID = m_BioOpPSheet.m_EnrollPPage.m_cs_ID;

			if (isPrivacyEnabled && !_decrypt)
			{ // "privacy crypt"
				l_cs_FileID += "_pcrypt";
			}
			CFileDialog l_x_SaveFileDlg(FALSE,
				"",
				l_cs_FileID,
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
				l_cs_Tmp,
				this);
			SAV_PATH
				if (l_x_SaveFileDlg.DoModal() != IDOK)
				{
					RESTORE_PATH
						l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
					l_x_ResultDlg.m_cs_Msg2.Format("Saving FP template file");
					l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
					l_b_ReturnCode = FALSE;
				}
				else
				{
					RESTORE_PATH
						l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();
					//- - - - - - - - - - - 
					// Write data in File
					//- - - - - - - - - - -

					CString l_cs_File;
					l_cs_File.Format("%s", l_cs_SaveFile);

					CFile l_f_PkFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

					l_f_PkFile.Write(l_auc_FileBuffer, l_ul_FileBufferSize);

					l_f_PkFile.Close();

					//- - - - - - - - - - - -
					l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
					l_x_ResultDlg.m_cs_Msg3.Format("FP Template successfully");
					l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
					l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
					Beep(BEEP_FREQUENCY, BEEP_DURATION);
					l_b_ReturnCode = TRUE;
				}
			l_x_ResultDlg.DoModal();
			l_x_ResultDlg.InitializeMsg();
			CleanAsyncMessages2();
		}
		else
		{
			bool l_b_notify_confirmation = false;

			for (i = 0; i<l_uc_NbFingers; i++)
			{
				//----------------------
				// FVP aquisition enabled
				//----------------------			
				if (((m_bl_FVP == TRUE) && (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType == STR_NO_PK_FP) && (m_BioOpPSheet.m_EnrollPPage.m_cs_FVP_TemplateType == STR_NO_FVP)) || ((m_bl_FVP == TRUE) && (m_BioOpPSheet.m_EnrollPPage.m_cs_FVP_TemplateType != STR_NO_FVP)))
				{
					UC l_uc_dataIndex;
					UC l_uc_PkFpQuality;
					UC l_uc_AdvancedSecurityLevelsCompatibility;
					l_x_Template.GetNbFVPTemplate(l_uc_NbFingers);
					l_i_Ret = l_x_Template.GetFVPTemplate(
						i,					// UC	i_uc_indexTemplate,
						l_x_FVPTemplateType,	// T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
						l_ul_LenTemplate,	// UL	&o_ul_lenTemplate,
						l_puc_DataTemplate,  // PUC	&o_puc_dataTemplate
						l_uc_PkFpQuality,
						l_uc_AdvancedSecurityLevelsCompatibility,
						l_uc_dataIndex
						);
					if (l_i_Ret != MORPHO_OK)
					{
						ShowErrorBox("An error occured while calling C_MORPHO_TemplateList::GetNbFVPTemplate() function", l_i_Ret);
						PostMessage(WM_REMOVE_PROCESS_PAGE, 0, (LPARAM)&(m_BioOpPSheet.m_EnrollPPage));
						delete l_auc_FileBuffer;
						delete l_auc_FileBuffer2;
						delete l_auc_PersonnalInformation;
						delete l_puc_Buf;
						return FALSE;
					}//end if


					l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
					if (i == 0)
					{
						l_x_ResultDlg.m_cs_Msg2.Format("Finger #1 - Quality Score: %u  ", l_uc_PkFpQuality);
					}
					else
					{
						l_x_ResultDlg.m_cs_Msg2.Format("Finger #2 -  Quality Score: %u  ", l_uc_PkFpQuality);
					}
					l_x_ResultDlg.m_cs_Msg3.Format("Advanced Security Levels Compatibility: %s", (l_uc_AdvancedSecurityLevelsCompatibility == 1) ? "Yes" : "No");

					Beep(BEEP_FREQUENCY, BEEP_DURATION);

					l_x_ResultDlg.DoModal();
					l_x_ResultDlg.InitializeMsg();
					l_b_notify_confirmation = true;

					if (m_BioOpPSheet.m_EnrollPPage.m_cs_FVP_TemplateType != STR_NO_FVP) //MSO DEMO propose l’enregistrement des templates PK_FVP que si l’utilisateur demande l’export.
					{
						// Save first finger (Finger #1) FVP template in a buffer 
						if (i == 0)
						{
							memcpy(l_auc_FileBuffer, l_puc_DataTemplate, l_ul_LenTemplate);
							l_ul_FileBufferSize = l_ul_LenTemplate;
						}
						// Save second finger (Finger #2) FVP template in an other buffer 
						else if (i == 1)
						{
							memcpy(l_auc_FileBuffer2, l_puc_DataTemplate, l_ul_LenTemplate);
							l_ul_FileBufferSize2 = l_ul_LenTemplate;
						}//end if

						l_cs_FileID = m_BioOpPSheet.m_EnrollPPage.m_cs_ID + "_";
						if (i == 0) // "_finger_1/2" (capture 1er doigt)
						{
							l_cs_FileID += "finger_1";
						}
						else if (i == 1) // "finger_2" (capture 1er doigt)
						{
							l_cs_FileID += "finger_2";
						}//end if						
						if (isPrivacyEnabled && !_decrypt)
						{ // "add privacy crypt to ext"
							l_cs_FileID += "_pcrypt";
						}

						CFileDialog l_x_SaveFileDlg(FALSE,
							"",
							l_cs_FileID,
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
							l_cs_Tmp_FVP,
							this);
						SAV_PATH
							if (l_x_SaveFileDlg.DoModal() != IDOK)
							{
								RESTORE_PATH
									l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
								l_x_ResultDlg.m_cs_Msg2.Format("Saving FVP template file");
								l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
								l_b_ReturnCode = FALSE;
							}
							else
							{
								RESTORE_PATH
									l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

								//- - - - - - - - - - - 
								// Write data in File
								//- - - - - - - - - - -

								CString l_cs_File;
								l_cs_File.Format("%s", l_cs_SaveFile);

								CFile l_f_PkFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
								if (i == 0) //"Finger #1" (first finger FVP template)
								{
									if (_decrypt == true)
									{
										bool b_decRet = _DecryptAESData(l_auc_FileBuffer, l_ul_FileBufferSize, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
										if (!b_decRet || o_pul_Aes128CbcDeCryptedDataLen == 0)
										{
											AfxMessageBox("Decrypting data error !", MB_ICONERROR);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}

										if (!_CheckPrivacyData(o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
										{
											AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
											if (o_ppuc_Aes128CbcDeCryptedData)
												MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}

										if (b_decRet)
											l_f_PkFile.Write(o_ppuc_Aes128CbcDeCryptedData + 8, o_pul_Aes128CbcDeCryptedDataLen - 8);

										if (o_ppuc_Aes128CbcDeCryptedData)
											MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
									}
									else
										l_f_PkFile.Write(l_auc_FileBuffer, l_ul_FileBufferSize);
								}
								else if (i == 1)//"Finger #2" (second finger FVP template)
								{
									if (_decrypt == true)
									{
										bool b_decRet = _DecryptAESData(l_auc_FileBuffer2, l_ul_FileBufferSize2, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
										if (!b_decRet || o_pul_Aes128CbcDeCryptedDataLen == 0)
										{
											AfxMessageBox("Decrypting data error !", MB_ICONERROR);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}

										if (!_CheckPrivacyData(o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
										{
											AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
											if (o_ppuc_Aes128CbcDeCryptedData)
												MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}

										if (b_decRet)
											l_f_PkFile.Write(o_ppuc_Aes128CbcDeCryptedData + 8, o_pul_Aes128CbcDeCryptedDataLen - 8);

										if (o_ppuc_Aes128CbcDeCryptedData)
											MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
									}
									else
										l_f_PkFile.Write(l_auc_FileBuffer2, l_ul_FileBufferSize2);
								}
								l_f_PkFile.Close();

								l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
								l_x_ResultDlg.m_cs_Msg3.Format("FVP Template successfully");
								l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
								l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
								//- - - - - - - - - - - -
							}//end if
						l_x_ResultDlg.DoModal();
						l_x_ResultDlg.InitializeMsg();
					}

					l_b_ReturnCode = TRUE;
					CleanAsyncMessages2();
				}
				//----------------------
				// FP aquisition enabled
				//----------------------			
				if ((m_bl_FVP == FALSE) || ((m_bl_FVP == TRUE) && (m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType != STR_NO_PK_FP)))
				{
					UC l_uc_dataIndex;
					UC l_uc_PkFpQuality;
					l_x_Template.GetNbTemplate(l_uc_NbFingers);
					l_i_Ret = l_x_Template.GetTemplate(
						i,					// UC	i_uc_indexTemplate,
						l_x_TypeTemplate,	// T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
						l_ul_LenTemplate,	// UL	&o_ul_lenTemplate,
						l_puc_DataTemplate,  // PUC	&o_puc_dataTemplate
						l_uc_PkFpQuality,
						l_uc_dataIndex
						);

					if (l_i_Ret != MORPHO_OK)
					{
						ShowErrorBox("An error occured while calling C_MORPHO_TemplateList::GetTemplate() function", l_i_Ret);
						PostMessage(WM_REMOVE_PROCESS_PAGE, 0, (LPARAM)&(m_BioOpPSheet.m_EnrollPPage));
						delete l_auc_FileBuffer;
						delete l_auc_FileBuffer2;
						delete l_auc_PersonnalInformation;
						delete l_puc_Buf;
						return FALSE;
					}

					if (l_b_notify_confirmation == false)
					{
						l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
						if (i == 0)
						{
							l_x_ResultDlg.m_cs_Msg2.Format("Finger #1 - Quality Score: %u  ", l_uc_PkFpQuality);
						}
						else
						{
							l_x_ResultDlg.m_cs_Msg2.Format("Finger #2 -  Quality Score: %u  ", l_uc_PkFpQuality);
						}

						Beep(BEEP_FREQUENCY, BEEP_DURATION);
						l_x_ResultDlg.DoModal();
						l_x_ResultDlg.InitializeMsg();
					}

					if ((m_BioOpPSheet.m_EnrollPPage.m_cs_TemplateType != STR_NO_PK_FP)) //MSO DEMO provides the registration of templates PK_COMP if the user requests export.
					{

						// Save first finger (Finger #1) template in a buffer 
						if (i == 0)
						{
							memcpy(l_auc_FileBuffer, l_puc_DataTemplate, l_ul_LenTemplate);
							l_ul_FileBufferSize = l_ul_LenTemplate;
						}
						// Save second finger (Finger #2) template in an other buffer 
						else if (i == 1)
						{
							memcpy(l_auc_FileBuffer2, l_puc_DataTemplate, l_ul_LenTemplate);
							l_ul_FileBufferSize2 = l_ul_LenTemplate;
						}

						l_cs_FileID = m_BioOpPSheet.m_EnrollPPage.m_cs_ID + "_";
						if (i == 0) // "finger_1" (capture 1er doigt)
						{
							l_cs_FileID += "finger_1";
						}
						else if (i == 1) // "finger_2" (capture 1er doigt)
						{
							l_cs_FileID += "finger_2";
						}

						if ((l_x_TypeTemplate == MORPHO_PK_DIN_V66400_CS_AA) || (l_x_TypeTemplate == MORPHO_PK_ISO_FMC_CS_AA)) // Pks ordered by ascending angle
						{
							l_cs_FileID += "_aa";
						}
						if (isPrivacyEnabled && !_decrypt)
						{ // "add privacy crypt to ext"
							l_cs_FileID += "_pcrypt";
						}

						CFileDialog l_x_SaveFileDlg(FALSE,
							"",
							l_cs_FileID,
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
							l_cs_Tmp,
							this);
						SAV_PATH
							if (l_x_SaveFileDlg.DoModal() != IDOK)
							{
								RESTORE_PATH
									l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
								l_x_ResultDlg.m_cs_Msg2.Format("Saving FP template file");
								l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
								l_b_ReturnCode = FALSE;
							}
							else
							{
								RESTORE_PATH
									l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

								//- - - - - - - - - - - 
								// Write data in File
								//- - - - - - - - - - -

								CString l_cs_File;
								l_cs_File.Format("%s", l_cs_SaveFile);

								CFile l_f_PkFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
								if (i == 0) //"Finger #1" (first finger template)
								{
									if (_decrypt == true)
									{
										bool b_decRet = _DecryptAESData(l_auc_FileBuffer, l_ul_FileBufferSize, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
										if (!b_decRet || o_pul_Aes128CbcDeCryptedDataLen == 0)
										{
											AfxMessageBox("Decrypting data error !", MB_ICONERROR);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}
										if (!_CheckPrivacyData(o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
										{
											AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
											if (o_ppuc_Aes128CbcDeCryptedData)
												MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}

										if (b_decRet)
											l_f_PkFile.Write(o_ppuc_Aes128CbcDeCryptedData + 8, o_pul_Aes128CbcDeCryptedDataLen - 8);
										if (o_ppuc_Aes128CbcDeCryptedData)
											MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
									}
									else
										l_f_PkFile.Write(l_auc_FileBuffer, l_ul_FileBufferSize);

								}
								else if (i == 1)//"Finger #2" (second finger template)
								{
									if (_decrypt == true)
									{
										bool b_decRet = _DecryptAESData(l_auc_FileBuffer2, l_ul_FileBufferSize2, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
										if (!b_decRet || o_pul_Aes128CbcDeCryptedDataLen == 0)
										{
											AfxMessageBox("Decrypting data error !", MB_ICONERROR);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}
										if (!_CheckPrivacyData(o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
										{
											AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
											if (o_ppuc_Aes128CbcDeCryptedData)
												MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}
										if (b_decRet)
											l_f_PkFile.Write(o_ppuc_Aes128CbcDeCryptedData + 8, o_pul_Aes128CbcDeCryptedDataLen - 8);
										if (o_ppuc_Aes128CbcDeCryptedData) MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
									}
									else
										l_f_PkFile.Write(l_auc_FileBuffer2, l_ul_FileBufferSize2);
								}
								l_f_PkFile.Close();

								//- - - - - - - - - - - -							
								l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
								l_x_ResultDlg.m_cs_Msg3.Format("FP Template successfully");
								l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
								l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
							}
						l_x_ResultDlg.DoModal();
						l_x_ResultDlg.InitializeMsg();
					}
					l_b_ReturnCode = TRUE;
					CleanAsyncMessages2();
				}
			}//end for
		}
		if ((l_uc_SaveRecord == TRUE))
		{
			UpdateNumberOfRecords();
		}
		//--------------------------
		// Image aquisition enabled
		//--------------------------

		if (m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType != STR_NO_IMAGE)
		{
			_decrypt = false;
			if (isPrivacyEnabled)
			{
				if (AfxMessageBox("Privacy Mode : Image data is crypted !\nWould you like to decrypt it before saving ?", MB_YESNO) == IDYES)
				{
					_decrypt = true;
				}
			}

			if ( // image ISO-19794-4 FIR
				l_x_CompressAlgo == MORPHO_NO_COMPRESS &&
				(
				m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == STR_ISO19794_FIR_RAW ||
				m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == STR_ISO19794_FIR_BMP ||
				m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == STR_ISO19794_FIR_WSQ ||
				m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == STR_ISO19794_FIR_JPEG ||
				m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == STR_ISO19794_FIR_JPEG2000 ||
				m_BioOpPSheet.m_EnrollPPage.m_str_CompressionType == STR_ISO19794_FIR_PNG
				)
				)
			{
				l_cs_Tmp = "ISO-19794-4 FIR Files (*.iso-firEXT_CRYPT)|*.iso-firEXT_CRYPT||";
				if (m_IsDataEncryption && !m_bl_FVP)
				{
					l_cs_Tmp.Replace("EXT_CRYPT", ".crypt");
				}
				else
				{
					l_cs_Tmp.Replace("EXT_CRYPT", "");
				}

				l_cs_FileID = m_BioOpPSheet.m_EnrollPPage.m_cs_ID + "_";
				l_cs_FileName = l_cs_FileID + "ISO19794_FIR_Image" + l_cs_ISO19794_FIR_ImageExtension;
				if (isPrivacyEnabled && !_decrypt)
				{ // "add privacy crypt to ext"
					l_cs_FileName += "_pcrypt";
				}

				CFileDialog l_x_SaveFileDlg(FALSE,
					"",
					l_cs_FileName,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
					l_cs_Tmp,
					this);
				SAV_PATH

					if (l_x_SaveFileDlg.DoModal() != IDOK)
					{
						RESTORE_PATH
							l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
						l_x_ResultDlg.m_cs_Msg2.Format("Saving image file");
						l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
						l_b_ReturnCode = FALSE;
					}
					else
					{
						RESTORE_PATH
							// ----- conversion ISO-19794-4 FIR ------ //
							// assing l_px_Image(array) to l_ppx_images(**)
							T_MORPHO_IMAGE** l_ppx_images = new T_MORPHO_IMAGE*[l_uc_NbFingers];
						PT_MORPHO_IMAGE_INFO *	l_ppx_ImagesInfo = new PT_MORPHO_IMAGE_INFO[l_uc_NbFingers];

						PT_MORPHO_ISO19794_FIR	l_px_ISO19794_FIR = new T_MORPHO_ISO19794_FIR;

						for (i = 0; i<l_uc_NbFingers; i++)
						{
							//get morpho image struct
							l_i_Ret = l_x_Template.GetFullImageRetrieving(i, l_px_morphoImage);

							if (l_i_Ret != MORPHO_OK)
							{
								l_x_ResultDlg.m_cs_Msg1.Format("Error Image Unknown");
								break; // error error image unknown
							}

							l_ppx_images[i] = new T_MORPHO_IMAGE;
							l_ppx_images[i]->m_puc_Image = NULL;

							if (isPrivacyEnabled)
							{
								if (_decrypt)
								{
									bool b_decRet = _DecryptAESData(l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_ul_imageSize, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
									if (!b_decRet || o_pul_Aes128CbcDeCryptedDataLen == 0)
									{
										AfxMessageBox("Decrypting data error !", MB_ICONERROR);
										delete l_auc_FileBuffer;
										delete l_auc_FileBuffer2;
										delete l_auc_PersonnalInformation;
										delete l_puc_Buf;
										return FALSE;
									}
									if (!_CheckPrivacyData(o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
									{
										AfxMessageBox("Invalid Image CRC32 !", MB_ICONERROR);
										if (o_ppuc_Aes128CbcDeCryptedData)
											MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
										break;
									}
									T_MORPHO_IMAGE_HEADER	l_tmpHeader;
									T_MORPHO_IMAGE			l_tmpImage;

									memcpy(&l_tmpImage.m_x_ImageHeader, o_ppuc_Aes128CbcDeCryptedData + 8, sizeof(T_MORPHO_IMAGE_HEADER));

									UL l_ul_imageSize = l_tmpImage.m_x_ImageHeader.m_us_NbRow * l_tmpImage.m_x_ImageHeader.m_us_NbCol;
									l_ppx_images[i]->m_puc_Image = new UC[l_ul_imageSize];
									memcpy(l_ppx_images[i]->m_puc_Image, o_ppuc_Aes128CbcDeCryptedData + 8, l_ul_imageSize);

									l_ppx_images[i]->m_ul_imageSize = l_tmpImage.m_ul_imageSize;
									l_ppx_images[i]->m_x_ImageHeader = l_tmpImage.m_x_ImageHeader;
									l_ppx_images[i]->m_x_ImageWSQHeader = l_tmpImage.m_x_ImageWSQHeader;

									l_ppx_ImagesInfo[i] = new T_MORPHO_IMAGE_INFO;
									l_ppx_ImagesInfo[i]->m_uc_FingerImageQuality = 0;
									l_ppx_ImagesInfo[i]->m_uc_FingerPosition = 0;
									l_ppx_ImagesInfo[i]->m_uc_ImpressionType = 0;
								}
							}
							else
							{
								UL l_ul_imageSize = l_px_morphoImage->m_x_ImageHeader.m_us_NbRow * l_px_morphoImage->m_x_ImageHeader.m_us_NbCol;
								l_ppx_images[i]->m_puc_Image = new UC[l_ul_imageSize];
								memcpy(l_ppx_images[i]->m_puc_Image, l_px_morphoImage->m_puc_Image, l_ul_imageSize);

								l_ppx_images[i]->m_ul_imageSize = l_px_morphoImage->m_ul_imageSize;
								l_ppx_images[i]->m_x_ImageHeader = l_px_morphoImage->m_x_ImageHeader;
								l_ppx_images[i]->m_x_ImageWSQHeader = l_px_morphoImage->m_x_ImageWSQHeader;

								l_ppx_ImagesInfo[i] = new T_MORPHO_IMAGE_INFO;
								l_ppx_ImagesInfo[i]->m_uc_FingerImageQuality = 0;
								l_ppx_ImagesInfo[i]->m_uc_FingerPosition = 0;
								l_ppx_ImagesInfo[i]->m_uc_ImpressionType = 0;
							}
						}

						if (l_i_Ret == MORPHO_OK)
						{
							if ((isPrivacyEnabled && _decrypt) || !isPrivacyEnabled)
							{
								// Call CreateISO19794_FIR()
								l_i_Ret = C_MORPHO_Utils::CreateISO19794_FIR(l_ppx_images, l_ppx_ImagesInfo, l_uc_NbFingers, l_px_ISO19794_FIR);

								if (l_i_Ret != MORPHO_OK)
								{
									l_x_ResultDlg.m_cs_Msg1.Format("CreateISO19794_FIR Failed");
									l_b_ReturnCode = FALSE;
								}
							}


							if (l_i_Ret == MORPHO_OK)
							{
								// Get file Path
								l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();
								CString l_cs_File;
								l_cs_File.Format("%s", l_cs_SaveFile);

								if ((isPrivacyEnabled && _decrypt) || !isPrivacyEnabled)
								{
									// Call WriteISO19794_FIR
									l_i_Ret = C_MORPHO_Utils::WriteISO19794_FIR(l_px_ISO19794_FIR, l_x_DesiredISO19794_FIR_CompressAlgo, l_cs_File.GetBuffer(l_cs_File.GetLength()));
								}
								else
								{
									CFile l_f_RawFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
									l_f_RawFile.Write(l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_ul_imageSize);
									l_f_RawFile.Close();
								}

								if (l_i_Ret != MORPHO_OK && l_i_Ret != MORPHOWARNING_WSQ_COMPRESSION_RATIO)
								{
									l_x_ResultDlg.m_cs_Msg1.Format("WriteISO19794_FIR Failed");
									l_b_ReturnCode = FALSE;
								}
								else
								{
									if (l_i_Ret == MORPHOWARNING_WSQ_COMPRESSION_RATIO)
									{
										l_x_ResultDlg.m_cs_Msg2.Format("MORPHOWARNING_WSQ_COMPRESSION_RATIO");
									}

									l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
									l_x_ResultDlg.m_cs_Msg3.Format("Image successfully");
									l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
									l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
									Beep(BEEP_FREQUENCY, BEEP_DURATION);
									l_b_ReturnCode = TRUE;
								}
							}
						}
						// free memory
						for (i = 0; i<l_uc_NbFingers; i++)
						{
							delete[] l_ppx_images[i]->m_puc_Image;
							l_ppx_images[i]->m_puc_Image = NULL;

							delete l_ppx_images[i];
							l_ppx_images[i] = NULL;
							if ((isPrivacyEnabled && _decrypt) || !isPrivacyEnabled)
							{
								delete l_ppx_ImagesInfo[i];
								l_ppx_ImagesInfo[i] = NULL;
							}
						}

						if (l_ppx_images)
						{
							delete[]l_ppx_images;
							l_ppx_images = NULL;
						}
						if (l_ppx_ImagesInfo)
						{
							delete[]l_ppx_ImagesInfo;
							l_ppx_ImagesInfo = NULL;
						}

						if ((isPrivacyEnabled && _decrypt) || !isPrivacyEnabled)
						{
							// free l_px_ISO19794_FIR
							C_MORPHO_Utils::ReleaseISO19794_FIR(l_px_ISO19794_FIR);
							if (l_px_ISO19794_FIR)
							{
								delete l_px_ISO19794_FIR;
								l_px_ISO19794_FIR = NULL;
							}
						}

						l_x_ResultDlg.DoModal();
						l_x_ResultDlg.InitializeMsg();
						CleanAsyncMessages2();
					}
			}
			else //image BMP, RAW, V1,WSQ
			{
				l_cs_FileID = m_BioOpPSheet.m_EnrollPPage.m_cs_ID + "_";
				for (i = 0; i<l_uc_NbFingers; i++)
				{
					if (i == 0) // "finger_1" (capture 1er doigt)
					{
						l_cs_FileName = l_cs_FileID + "finger_1";
					}
					else if (i == 1) // "finger_2" (capture 1er doigt)
					{
						l_cs_FileName = l_cs_FileID + "finger_2";
					}
					if (isPrivacyEnabled && !_decrypt)
					{ // add "privacy crypt" to ext
						l_cs_FileName += "_pcrypt";
					}

					//get morpho image struct
					l_i_Ret = l_x_Template.GetFullImageRetrieving(i, l_px_morphoImage);

					if (l_i_Ret != MORPHO_OK)
					{
						l_x_ResultDlg.m_cs_Msg1.Format("Error Image Unknown");
						break; // error error image unknown
					}

					if (l_x_CompressAlgo == MORPHO_NO_COMPRESS) //image BMP or image RAW
					{
						if ((_decrypt && isPrivacyEnabled) || !isPrivacyEnabled)
							l_cs_Tmp = "Bmp Files (*.bmp)|*.bmp|Raw Files (*.raw)|*.raw|All Files (*.*)|*.*||";
						else
							l_cs_Tmp = "Raw Files (*.raw)|*.raw|All Files (*.*)|*.*||";

						if (m_IsDataEncryption && !m_bl_FVP)
						{
							l_cs_Tmp = "Raw Files (*.raw.crypt)|*.raw.crypt|All Files (*.*)|*.*||";
						}
						else
						{
							if ((_decrypt && isPrivacyEnabled) || !isPrivacyEnabled)
								l_cs_Tmp = "Bmp Files (*.bmp)|*.bmp|Raw Files (*.raw)|*.raw|All Files (*.*)|*.*||";
							else
								l_cs_Tmp = "Raw Files (*.raw)|*.raw|All Files (*.*)|*.*||";
						}

						CFileDialog l_x_SaveFileDlg(FALSE,
							"",
							l_cs_FileName,
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
							l_cs_Tmp,
							this);


						SAV_PATH
							if (l_x_SaveFileDlg.DoModal() != IDOK)
							{
								RESTORE_PATH
									l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
								l_x_ResultDlg.m_cs_Msg2.Format("Saving image file");
								l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
								l_b_ReturnCode = FALSE;
							}
							else
							{
								RESTORE_PATH
									l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

								CString l_cs_CompressedFileExt = l_x_SaveFileDlg.GetFileExt();

								//- - - - - - - - - - -
								// Write data in File
								//- - - - - - - - - - -
								if (m_PrivacyModeStatus)
								{
									if (_decrypt)
									{
										PUC o_puc_Aes128CbcDecryptedImageData = NULL;
										UL o_pul_Aes128CbcDecryptedImageDataSize = 0;
										bool b_decRet = _DecryptAESData(l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_ul_imageSize, &o_puc_Aes128CbcDecryptedImageData, o_pul_Aes128CbcDecryptedImageDataSize, sl_encrypt_Key);
										if (!b_decRet || o_pul_Aes128CbcDecryptedImageDataSize == 0)
										{
											AfxMessageBox("Decrypting data error !", MB_ICONERROR);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}
										if (!_CheckPrivacyData(o_puc_Aes128CbcDecryptedImageData, o_pul_Aes128CbcDecryptedImageDataSize, sl_encrypt_Key))
										{
											AfxMessageBox("Invalid Image CRC32 !", MB_ICONERROR);
											if (o_puc_Aes128CbcDecryptedImageData)
												MSOSECU_Free(&o_puc_Aes128CbcDecryptedImageData);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}

										if (o_puc_Aes128CbcDecryptedImageData)
											o_puc_Aes128CbcDecryptedImageData = o_puc_Aes128CbcDecryptedImageData + 8; // Skip CRC32 and Random32

										o_pul_Aes128CbcDecryptedImageDataSize -= 8;

										if (l_cs_CompressedFileExt == "bmp")
										{
											T_MORPHO_IMAGE l_px_morphoImageTmp;
											l_px_morphoImageTmp.m_puc_CompressedImage = NULL;
											l_px_morphoImageTmp.m_puc_Image = NULL;
											l_px_morphoImageTmp.m_ul_imageSize = 0;

											UL l_i_HeaderSize = sizeof(T_MORPHO_IMAGE_HEADER);
											UL l_i_ImageSize = o_pul_Aes128CbcDecryptedImageDataSize - l_i_HeaderSize;

											memcpy(&l_px_morphoImageTmp.m_x_ImageHeader, o_puc_Aes128CbcDecryptedImageData, sizeof(T_MORPHO_IMAGE_HEADER));

											if (l_px_morphoImageTmp.m_x_ImageHeader.m_uc_CompressionType == 0x2C)
											{
												l_px_morphoImageTmp.m_puc_Image = (PUC)malloc(l_i_ImageSize);
												memcpy(l_px_morphoImageTmp.m_puc_Image, o_puc_Aes128CbcDecryptedImageData + l_i_HeaderSize, l_i_ImageSize);
											}
											else if (l_px_morphoImageTmp.m_x_ImageHeader.m_uc_CompressionType == 0x9C)
											{
												UL l_ul_RawImageAndHeaderSize = 0;
												l_i_Ret = COMPRESS_GetRawImageSize(o_puc_Aes128CbcDecryptedImageData,		//PUC	i_puc_CompressedImageWithHeader,
													l_i_ImageSize + 12,		//UL	i_ul_CompressedImageWithHeaderSize,
													&l_ul_RawImageAndHeaderSize);
											}

											ConvertRawToBmp l__ConvertRawToBmp;
											CString l_cs_File;
											l_cs_File.Format("%s", l_cs_SaveFile);
											l__ConvertRawToBmp.SaveImageBMP(l_cs_File,
												l_px_morphoImageTmp.m_puc_Image,
												l_px_morphoImageTmp.m_x_ImageHeader.m_us_NbRow,
												l_px_morphoImageTmp.m_x_ImageHeader.m_us_NbCol,
												l_px_morphoImageTmp.m_x_ImageHeader.m_us_ResX,
												l_px_morphoImageTmp.m_x_ImageHeader.m_us_ResY);

											if (l_px_morphoImageTmp.m_puc_Image) free(l_px_morphoImageTmp.m_puc_Image);
										}
										else
										{
											CString l_cs_File;
											l_cs_File.Format("%s", l_cs_SaveFile);
											CFile l_f_RawFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
											l_f_RawFile.Write(o_puc_Aes128CbcDecryptedImageData, o_pul_Aes128CbcDecryptedImageDataSize);
											l_f_RawFile.Close();
										}

										if (o_puc_Aes128CbcDecryptedImageData)
										{
											o_puc_Aes128CbcDecryptedImageData = o_puc_Aes128CbcDecryptedImageData - 8;
											MSOSECU_Free(&o_puc_Aes128CbcDecryptedImageData);
										}
									}
									else
									{
										CString l_cs_File;
										l_cs_File.Format("%s", l_cs_SaveFile);
										CFile l_f_RawFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
										l_f_RawFile.Write(l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_ul_imageSize);
										l_f_RawFile.Close();
									}

								}
								else if (l_cs_CompressedFileExt == "bmp" && !m_IsDataEncryption) //image BMP
								{
									ConvertRawToBmp l__ConvertRawToBmp;
									CString l_cs_File;
									l_cs_File.Format("%s", l_cs_SaveFile);
									l__ConvertRawToBmp.SaveImageBMP(l_cs_File, l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_x_ImageHeader.m_us_NbRow, l_px_morphoImage->m_x_ImageHeader.m_us_NbCol, l_px_morphoImage->m_x_ImageHeader.m_us_ResX, l_px_morphoImage->m_x_ImageHeader.m_us_ResY);
								}
								else //image RAW
								{
									CString l_cs_File;
									l_cs_File.Format("%s", l_cs_SaveFile);
									CFile l_f_RawFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
									if (!m_IsDataEncryption)
									{
										l_f_RawFile.Write(l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_x_ImageHeader.m_us_NbRow*l_px_morphoImage->m_x_ImageHeader.m_us_NbCol);
									}
									else
									{
										l_f_RawFile.Write(l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_ul_imageSize);
									}
									l_f_RawFile.Close();
								}
								//- - - - - - - - - - - -
								l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
								l_x_ResultDlg.m_cs_Msg3.Format("Image successfully ");
								l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
								l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
								Beep(BEEP_FREQUENCY, BEEP_DURATION);
								l_b_ReturnCode = TRUE;
							}
						l_x_ResultDlg.DoModal();
						l_x_ResultDlg.InitializeMsg();
						CleanAsyncMessages2();
					}
					else if (l_x_CompressAlgo == MORPHO_COMPRESS_V1) // image V1
					{
						l_cs_Tmp = "Bin Files (*.binEXT_CRYPT)|*.binEXT_CRYPT|All Files (*.*)|*.*||";
						if (m_IsDataEncryption && !m_bl_FVP)
						{
							l_cs_Tmp.Replace("EXT_CRYPT", ".crypt");
						}
						else
						{
							l_cs_Tmp.Replace("EXT_CRYPT", "");
						}

						CFileDialog l_x_SaveFileDlg(FALSE,
							"",
							l_cs_FileName,
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
							l_cs_Tmp,
							this);
						SAV_PATH
							if (l_x_SaveFileDlg.DoModal() != IDOK)
							{
								RESTORE_PATH
									l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
								l_x_ResultDlg.m_cs_Msg2.Format("Saving image file");
								l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
								l_b_ReturnCode = FALSE;
							}
							else
							{
								RESTORE_PATH
									l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

								//- - - - - - - - - - -
								// Write data in File
								//- - - - - - - - - - -

								CString l_cs_File;
								l_cs_File.Format("%s", l_cs_SaveFile);

								CFile l_f_BinFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

								/**/
								if (isPrivacyEnabled)
								{
									PUC o_puc_Aes128CbcDecryptedImageData = NULL;
									UL o_pul_Aes128CbcDecryptedImageDataSize = 0;
									if (_decrypt)
									{
										UL m_ul_RawImageAndHeaderSize;
										PUC m_puc_RawImageAndHeader = NULL;

										bool b_decRet = _DecryptAESData(l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_ul_imageSize, &o_puc_Aes128CbcDecryptedImageData, o_pul_Aes128CbcDecryptedImageDataSize, sl_encrypt_Key);

										if (!b_decRet || o_pul_Aes128CbcDecryptedImageDataSize == 0)
										{
											AfxMessageBox("Decrypting image data error !", MB_ICONERROR);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}

										if (!_CheckPrivacyData(o_puc_Aes128CbcDecryptedImageData, o_pul_Aes128CbcDecryptedImageDataSize, sl_encrypt_Key))
										{
											AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
											if (o_puc_Aes128CbcDecryptedImageData)
												MSOSECU_Free(&o_puc_Aes128CbcDecryptedImageData);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}

										PUC l_puc_Image = o_puc_Aes128CbcDecryptedImageData + 8;
										UL l_ul_imageSize = o_pul_Aes128CbcDecryptedImageDataSize - 8;

										UL l_i_Ret2 = COMPRESS_GetRawImageSize(
											l_puc_Image,			//PUC	i_puc_CompressedImageWithHeader,
											l_ul_imageSize,		//UL	i_ul_CompressedImageWithHeaderSize,
											&(m_ul_RawImageAndHeaderSize)	//PUL	o_pul_UnCompressedImageWithHeaderSize
											);

										m_puc_RawImageAndHeader = (PUC)malloc(m_ul_RawImageAndHeaderSize);

										l_i_Ret2 = COMPRESS_UnCompressImage(
											l_puc_Image,			//PUC	i_puc_CompressedImageWithHeader,
											l_ul_imageSize,		//UL	i_ul_CompressedImageWithHeaderSize,
											m_puc_RawImageAndHeader,			//PUC	o_puc_RawImageWithHeader,
											&(m_ul_RawImageAndHeaderSize)		//PUL	io_pul_RawImageWithHeaderSize
											);

										if (l_i_Ret2 == MORPHO_OK)
										{
											l_puc_Image = m_puc_RawImageAndHeader + sizeof(T_MORPHO_IMAGE_HEADER);
										}

										l_f_BinFile.Write(l_puc_Image, l_ul_imageSize);
									}
									else
										l_f_BinFile.Write(l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_ul_imageSize);

									if (o_puc_Aes128CbcDecryptedImageData)
										MSOSECU_Free(&o_puc_Aes128CbcDecryptedImageData);


								} /**/
								else
									l_f_BinFile.Write(l_px_morphoImage->m_puc_CompressedImage, l_px_morphoImage->m_ul_imageSize);
								l_f_BinFile.Close();

								//- - - - - - - - - - - -
								l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
								l_x_ResultDlg.m_cs_Msg3.Format("Image successfully ");
								l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
								l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
								Beep(BEEP_FREQUENCY, BEEP_DURATION);
								l_b_ReturnCode = TRUE;
							}
						l_x_ResultDlg.DoModal();
						l_x_ResultDlg.InitializeMsg();
						CleanAsyncMessages2();
					}
					else if (l_x_CompressAlgo == MORPHO_COMPRESS_WSQ) // image WSQ
					{
						l_cs_Tmp = "Wsq Files (*.wsqEXT_CRYPT)|*.wsqEXT_CRYPT|All Files (*.*)|*.*||";
						if (m_IsDataEncryption && !m_bl_FVP)
						{
							l_cs_Tmp.Replace("EXT_CRYPT", ".crypt");
						}
						else
						{
							l_cs_Tmp.Replace("EXT_CRYPT", "");
						}

						CFileDialog l_x_SaveFileDlg(FALSE,
							"",
							l_cs_FileName,
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
							l_cs_Tmp,
							this);
						SAV_PATH
							if (l_x_SaveFileDlg.DoModal() != IDOK)
							{
								RESTORE_PATH
									l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
								l_x_ResultDlg.m_cs_Msg2.Format("Saving image file");
								l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
								l_b_ReturnCode = FALSE;
							}
							else
							{
								RESTORE_PATH
									l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

								//- - - - - - - - - - -
								// Write data in File
								//- - - - - - - - - - -

								CString l_cs_File;
								l_cs_File.Format("%s", l_cs_SaveFile);

								CFile l_f_RawFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

								PUC l_puc_Aes128CbcDeCryptedData = NULL;
								UL l_ul_Aes128CbcDeCryptedDataLen = 0;

								if (isPrivacyEnabled)
								{
									if (_decrypt == true)
									{
										bool b_decRet = _DecryptAESData(l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_ul_imageSize, &l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
										if (!b_decRet || l_ul_Aes128CbcDeCryptedDataLen == 0)
										{
											AfxMessageBox("Decrypting image data error !", MB_ICONERROR);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}
										if (!_CheckPrivacyData(l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
										{
											AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
											if (l_puc_Aes128CbcDeCryptedData)
												MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
											delete l_auc_FileBuffer;
											delete l_auc_FileBuffer2;
											delete l_auc_PersonnalInformation;
											delete l_puc_Buf;
											return FALSE;
										}
									}
								}

								if (!m_IsDataEncryption)
								{
									if (isPrivacyEnabled)
									{
										if (_decrypt == true)
										{
											l_f_RawFile.Write(l_puc_Aes128CbcDeCryptedData + 8, l_ul_Aes128CbcDeCryptedDataLen - 8);

											if (l_puc_Aes128CbcDeCryptedData)
												MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
										}
										else
											l_f_RawFile.Write(l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_ul_imageSize);
									}
									else
										l_f_RawFile.Write(l_px_morphoImage->m_puc_CompressedImage, l_px_morphoImage->m_x_ImageWSQHeader.m_ul_WSQImageSize);
								}
								else
								{
									if (isPrivacyEnabled)
									{
										if (_decrypt == true)
										{
											l_f_RawFile.Write(l_puc_Aes128CbcDeCryptedData + 8, l_ul_Aes128CbcDeCryptedDataLen - 8);

											if (l_puc_Aes128CbcDeCryptedData)
												MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
										}
										else
										{
											l_f_RawFile.Write(l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_ul_imageSize);
										}
									}
									else
										l_f_RawFile.Write(l_px_morphoImage->m_puc_Image, l_px_morphoImage->m_ul_imageSize);

								}

								l_f_RawFile.Close();

								//- - - - - - - - - - - -
								l_x_ResultDlg.m_cs_Msg1.Format("Enroll Successful");
								l_x_ResultDlg.m_cs_Msg3.Format("Image successfully ");
								l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
								l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
								Beep(BEEP_FREQUENCY, BEEP_DURATION);
								l_b_ReturnCode = TRUE;
							}
						l_x_ResultDlg.DoModal();
						CleanAsyncMessages2();
					}
				} // end for
			}
			//-------------------------------------
			// End of Image aquisition enabled case
			//-------------------------------------
			//------------------------------------------------------------
			// End of saving in file case
			//------------------------------------------------------------
		}
	}//End of ok case

	if (m_px_Image != NULL)
	{
		delete(m_px_Image);
		m_px_Image = NULL;
	}

	if (m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
	{
		PostMessage(WM_REMOVE_PROCESS_PAGE, 3, (LPARAM)&(m_BioOpPSheet.m_EnrollPPage));
	}

	PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
	if (l_uc_SaveRecord && l_i_EtatEnroll == MORPHO_OK)
	{
		RefreshListCtrl(sl_encrypt_Key);
	}

	m_BioOpPSheet.m_EnrollPPage.m_cs_ID = "";

	EnableButtons();
	InitQualityBar();
	InitGuidingAnimations();
	RedrawWnd();

	delete l_auc_FileBuffer;
	delete l_auc_FileBuffer2;
	delete l_auc_PersonnalInformation;
	delete l_puc_Buf;
	return l_b_ReturnCode;
}

//////////////////////////////////////////////////////////////////
I CMSO_DemoDlg::Verify(
	C_MORPHO_TemplateList &		i_x_TemplateList,
	UL							i_ul_CallbackMask
	)
{
	CResultDlg				l_x_ResultDlg;
	CString					l_cs_Temp;
	I	l_i_Ret;

	//------------------------------------------------------------
	// Verify
	//------------------------------------------------------------
	m_b_BiometricOperationProcessing = TRUE;

	if (m_tbCtrl.m_TabOptionsDlg->m_b_Export_Num_Pk == FALSE)
	{
		m_puc_ExportNumPk = NULL;
		m_uc_ExportNumPk = 0xFF;
	}
	else
	{
		m_puc_ExportNumPk = &m_uc_ExportNumPk;
	}

	//------------------------------------------------------------
	// Strategy Acquisition Mode
	//------------------------------------------------------------	
	l_i_Ret = m_x_MorphoSmart.SetStrategyAcquisitionMode(m_tbCtrl.m_TabBioSettingsDlg->m_uc_StrategyAcquisitionMode); //FVP 1.10
	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling C_MORPHO_Device::SetStrategyAcquisitionMode() function", l_i_Ret);
		return l_i_Ret;
	}

	l_i_Ret = m_x_MorphoSmart.Verify(
		m_tbCtrl.m_TabBioSettingsDlg->m_i_Timeout,							// US							i_us_Timeout,
		(T_MORPHO_FAR)m_tbCtrl.m_TabBioSettingsDlg->m_i_Threshold,			// T_MORPHO_FAR					i_us_FAR,
		i_x_TemplateList,						// C_MORPHO_TemplateList &		i_x_TemplateList,
		i_ul_CallbackMask,						// UL							i_ul_CallbackStatus,
		BiometricCallback,						// T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
		this,									// PVOID						i_pv_CallbackArgument
		NULL,
		m_puc_ExportNumPk,
		m_tbCtrl.m_TabBioSettingsDlg->m_i_CoderChoice,
		MORPHO_VERIF_DETECT_MODE | m_ul_WakeUpMode,
		m_tbCtrl.m_TabBioSettingsDlg->m_i_MatchingStrategy
		);

	// Get FFD logs and write in log file
	GetAndWriteFFDLogs(&m_x_MorphoSmart);

	m_b_BiometricOperationProcessing = FALSE;
	CleanAsyncMessages();

	if (l_i_Ret == MORPHOERR_DEVICE_LOCKED)
	{
		I l_i_Ret2 = UnlockDevice();
		if (l_i_Ret2 != MORPHO_OK)
		{
			ShowErrorBox("An error occured during Device Unlocking", l_i_Ret2);
		}
	}
	else if (l_i_Ret != MORPHO_OK)
	{
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		ShowAnimation("image_Ko");
		l_x_ResultDlg.m_cs_Msg1.Format("Verification Failed");
		switch (l_i_Ret)
		{
		case MORPHOERR_CMDE_ABORTED:
			l_x_ResultDlg.m_cs_Msg2.Format("Command Aborted.");
			break;
		case MORPHOERR_NO_HIT:
		case MORPHOERR_FFD:
		case MORPHOERR_MOIST_FINGER:
		case MORPHOERR_MOVED_FINGER:
		case MORPHOERR_SATURATED_FINGER:
		case MORPHOERR_INVALID_FINGER:
			l_x_ResultDlg.m_cs_Msg2.Format("Person not Authenticated.");
			break;
		case MORPHOERR_TIMEOUT:
			l_x_ResultDlg.m_cs_Msg2.Format("Timeout has expired.");
			l_x_ResultDlg.m_cs_Msg3.Format("Command Aborted.");
			break;
		case MORPHOERR_BADPARAMETER:
			l_x_ResultDlg.m_cs_Msg2.Format("One or more input parameters are out of range");
			break;
		case MORPHOERR_NOT_IMPLEMENTED:
			l_x_ResultDlg.m_cs_Msg2.Format("Not Implemented !!!");
			break;
		case MORPHOERR_FVP_MINUTIAE_SECURITY_MISMATCH:
			l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
			l_x_ResultDlg.m_cs_Msg2.Format("C_MORPHO_Database::Verify() function");
			l_x_ResultDlg.m_cs_Msg4.Format("MORPHOERR_FVP_MINUTIAE_SECURITY_MISMATCH.");
			break;
		case MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN:
			l_x_ResultDlg.m_cs_Msg2.Format("Finger is misplaced or has been");
			l_x_ResultDlg.m_cs_Msg3.Format("withdrawn from sensor during acquisition.");
			break;
		case MORPHOERR_LICENSE_MISSING:
			l_x_ResultDlg.m_cs_Msg2.Format("A required license is missing.");
			break;
		default:
			l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
			l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Database::Verify() function");
			ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
		}

		if (m_b_EndThread == TRUE)
		{
			l_x_ResultDlg.DoModal();
			CleanAsyncMessages2();
		}
	}

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::_VerifyWithFile(UL	i_ul_CallbackMask)
{
	CString					l_cs_Message;
	CString					l_cs_Message2;
	I						l_i_Ret;
	CString					l_cs_OpenFile;
	CResultDlg				l_x_ResultDlg;
	CString					l_cs_FileExt;
	C_MORPHO_TemplateList	l_x_TemplateList;
	CString					l_cs_FilesType;
	CString					l_cs_listfile[MAX_TEMPLATE];
	T_INFO_USER				l_x_ListinfoUsers[MAX_TEMPLATE];
	I						l_i_indexUser;
	I						l_nb_file;
	
	//------------------------------------------------------------
	// Privacy Preparation
	//------------------------------------------------------------
	I						l_i_Random;
	std::string				sl_encrypt_Key = "";
	BOOL					isPrivacyEnabled = (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED || m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED);

	if (isPrivacyEnabled)
	{
		l_i_Random = time(NULL);
		l_i_Ret = GetEncryptedKey(sl_encrypt_Key);
		if (!l_i_Ret)
		{
			AfxMessageBox("You must select a key for encryption.");
			return FALSE;
		}
	}

	l_nb_file = 0;
	l_cs_Message = "";
	l_cs_Message2 = "";

	l_cs_FilesType = g_x_BiometricFilesExtention;
	if (GetFilesAndExtarctTemplates(l_cs_FilesType, l_cs_listfile, l_x_ListinfoUsers, l_nb_file, l_x_TemplateList, isPrivacyEnabled, sl_encrypt_Key, l_i_Random))
	{
		l_i_Ret = Verify(
			l_x_TemplateList,
			i_ul_CallbackMask
			);

		if (l_i_Ret != MORPHO_OK)
		{
			if (l_i_Ret == MORPHOERR_CERTIF_UNKNOW)
			{
				//the MSO can not verify the X984 biometric token, the MSO does not have the certificate.
				//the new certificate is stored in the MSO.
				//the new X984 biometric token can be used.
				if (!ExtractAndStoreCertif(l_cs_listfile[0]))
					return FALSE;

				AfxMessageBox("The new certificate is stored in the MSO\nThe Token is sent again");

				l_i_Ret = Verify(
					l_x_TemplateList,
					i_ul_CallbackMask
					);

				if (l_i_Ret)
					return FALSE;
			}
			else if (l_i_Ret == MORPHOERR_BADPARAMETER)
			{
				l_x_ResultDlg.m_cs_Msg1.Format("Verification Failed");
				l_x_ResultDlg.m_cs_Msg2.Format("One or more input parameters are out of range");
				l_x_ResultDlg.DoModal();
				CleanAsyncMessages2();
				return FALSE;
			}
		}

		if (l_i_Ret == MORPHO_OK)
		{
			Beep(BEEP_FREQUENCY, BEEP_DURATION);
			ShowAnimation("image_Ok");
			l_x_ResultDlg.m_cs_Msg1.Format("User authenticated");

			l_i_indexUser = -1;

			if (m_puc_ExportNumPk != NULL)
			{
				if (m_uc_ExportNumPk >= 0 && m_uc_ExportNumPk < 20)
				{
					l_i_indexUser = m_uc_ExportNumPk;
				}
				l_cs_Message.Format("Matching Pk Number : %d", m_uc_ExportNumPk);
			}
			else
			{
				if (l_nb_file == 1)
				{
					l_i_indexUser = 0;
				}
			}

			if (l_i_indexUser != -1)
			{
				//Abd
				l_x_ResultDlg.m_cs_Msg2.Format("Firstname : %s", l_x_ListinfoUsers[l_i_indexUser].m_cs_Firstname);
				l_x_ResultDlg.m_cs_Msg3.Format("Lastname : %s", l_x_ListinfoUsers[l_i_indexUser].m_cs_Lastname);
				l_x_ResultDlg.m_cs_Msg5.Format("ID : %s", l_x_ListinfoUsers[l_i_indexUser].m_cs_ID);
				l_cs_Message2.Format("Matching File : [%s]", l_cs_listfile[l_i_indexUser]);
			}
			else
			{
				l_x_ResultDlg.m_cs_Msg3.Format("You should enable the");
				l_x_ResultDlg.m_cs_Msg5.Format("\"Export Matching Pk Number\"");
				l_x_ResultDlg.m_cs_Msg4.Format("to retrieve the information about the matching file");
			}

			l_x_ResultDlg.m_cs_Msg4.Format("%s\r\n%s", l_cs_Message, l_cs_Message2);
			l_x_ResultDlg.DoModal();
		}
	}

	CleanAsyncMessages2();
	return TRUE;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::_VerifyInBase(UL	i_ul_CallbackMask)
{
	C_MORPHO_User	l_x_User;
	I				l_i_Ret;
	CString			l_cs_Message;
	CString			l_cs_ID;
	UC				l_uc_lenUserID;
	UC				l_puc_UserID[100];
	CResultDlg		l_x_ResultDlg;
	UL				l_ul_LenField;
	PUC				l_puc_DataField;
	UC				l_puc_GenericBuf[100];
	I				j;
	UL				l_ul_DetectModeChoice;

	//---------------------------------------------
	// Check if a user is selected in the list 
	//---------------------------------------------

	if (!CheckIfOneUserSelectedInList(l_cs_ID))
		return FALSE;

	//---------------------------------------------
	// Retrieve User Object
	//---------------------------------------------
	BOOL isInPrivacyMode = (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED) || (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED);


	UI l_i_Random = time(NULL);
	PUC o_ppuc_Aes128CbcDeCryptedData = NULL;
	UL	o_pul_Aes128CbcDeCryptedDataLen = 0;

	bool _decrypt = false;
	std::string	sl_encrypt_Key;
	bool RetKey = false;
	if (isInPrivacyMode)
	{
		AfxMessageBox("user info will be decrypted to be verified");
		{
			RetKey = GetEncryptedKey(sl_encrypt_Key);
			if (!RetKey)
			{
				AfxMessageBox("Error occured when loading encryption key.");
				return false;
			}

				if (!m_IsPrivacyListClear)
					_decrypt = true;
		}
	}
	l_uc_lenUserID = l_cs_ID.GetLength();
	if (isInPrivacyMode)
	{
		if (!m_IsPrivacyListClear)
		{
			bool b_decRet = _DecryptAESData((PUC)l_cs_ID.GetBuffer(l_uc_lenUserID), l_uc_lenUserID, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
			if (!b_decRet || o_pul_Aes128CbcDeCryptedDataLen == 0)
			{
				AfxMessageBox("Decrypting data error !", MB_ICONERROR);
				return FALSE;
			}
			if (b_decRet)
			{
				UL dataSize;
				memcpy(&dataSize, o_ppuc_Aes128CbcDeCryptedData + 12, 4); // Get Size

				memcpy(o_ppuc_Aes128CbcDeCryptedData, o_ppuc_Aes128CbcDeCryptedData + 16, dataSize);
				o_ppuc_Aes128CbcDeCryptedData[dataSize] = 0;

				UC tmpData[255];
				UL tmpDataLen;
				_PreparePrivacyData((C*)o_ppuc_Aes128CbcDeCryptedData, dataSize, sl_encrypt_Key, l_i_Random, FALSE, tmpData, &tmpDataLen);
				l_uc_lenUserID = tmpDataLen;
				memcpy(l_puc_UserID, tmpData, tmpDataLen);
			}
			//if (NULL != o_ppuc_Aes128CbcDeCryptedData)						MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
		}
		else
		{
			UC tmpData[255];
			UL tmpDataLen;
			_PreparePrivacyData((C*)l_cs_ID.GetBuffer(l_uc_lenUserID), l_uc_lenUserID, sl_encrypt_Key, l_i_Random, TRUE, tmpData, &tmpDataLen);
			l_uc_lenUserID = tmpDataLen;
			memcpy(l_puc_UserID, tmpData, tmpDataLen);
		}
	}
	else
	{
		memcpy(l_puc_UserID, l_cs_ID.GetBuffer(l_uc_lenUserID), l_uc_lenUserID);
		l_puc_UserID[l_uc_lenUserID++] = 0;
	}	

	l_i_Ret = m_x_Database.GetUser(
		l_uc_lenUserID,				// UC				i_uc_lenUserID,
		l_puc_UserID,				// PUC				i_puc_UserID,
		l_x_User					// C_MORPHO_User &	o_x_User 
		);

	if (l_i_Ret != MORPHO_OK)
	{
		ShowAnimation("image_Ko");
		ShowErrorBox("An error occured while calling C_MORPHO_Database::GetUser() function", l_i_Ret);
		return FALSE;
	}


	//-----------------------------------------------------------
	// FORCE FINGER ON TOP DETECT MODE
	//-----------------------------------------------------------
	if (m_tbCtrl.m_TabOptionsDlg->m_b_Export_Num_Pk == FALSE)
	{
		m_puc_ExportNumPk = NULL;
		m_uc_ExportNumPk = 0xFF;
	}
	else
	{
		m_puc_ExportNumPk = &m_uc_ExportNumPk;
	}


	if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_ForceFingerPlacement)
	{
		l_ul_DetectModeChoice = MORPHO_ENROLL_DETECT_MODE;
		l_ul_DetectModeChoice |= MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE;
	}
	else
	{
		l_ul_DetectModeChoice = MORPHO_VERIF_DETECT_MODE;
	}

	l_ul_DetectModeChoice = l_ul_DetectModeChoice | m_ul_WakeUpMode;

	if (l_ul_DetectModeChoice > 0xFF)
	{
		l_cs_Message.Format("DetectModeChoice overfloat 0x%08x>0xFF(UC), information lost.", l_ul_DetectModeChoice);
		AfxMessageBox(l_cs_Message, MB_ICONERROR);
	}


	//------------------------------------------------------------
	// Verify 
	//------------------------------------------------------------
	m_b_BiometricOperationProcessing = TRUE;
	l_i_Ret = l_x_User.Verify(
		m_tbCtrl.m_TabBioSettingsDlg->m_i_Timeout,								//US							i_us_Timeout,
		(T_MORPHO_FAR)m_tbCtrl.m_TabBioSettingsDlg->m_i_Threshold,				//T_MORPHO_FAR					i_us_FAR,
		i_ul_CallbackMask,							//UL							i_ul_CallbackCmd,
		BiometricCallback,							//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
		this,										//PVOID							i_pv_CallbackArgument
		NULL,										//PUL							o_pul_MatchingScore,
		m_puc_ExportNumPk,							//PUC							o_puc_ExportNumPk,
		MORPHO_MSO_V9_CODER,						//I								i_i_CoderChoice,
		l_ul_DetectModeChoice,	//UL							i_ul_DetecModeChoice,
		m_tbCtrl.m_TabBioSettingsDlg->m_i_MatchingStrategy									//UL							i_ul_ModeChoice
		);

	// Get FFD logs and write in log file
	GetAndWriteFFDLogs(&m_x_MorphoSmart);

	m_b_BiometricOperationProcessing = FALSE;
	CleanAsyncMessages();

	if (l_i_Ret != MORPHO_OK)
	{
		ShowAnimation("image_Ko");
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_x_ResultDlg.m_cs_Msg1.Format("Verification Failed");
		switch (l_i_Ret)
		{
		case MORPHOERR_CMDE_ABORTED:
			l_x_ResultDlg.m_cs_Msg2.Format("Command Aborted.");
			break;
		case MORPHOERR_NO_HIT:
		case MORPHOERR_FFD:
		case MORPHOERR_MOIST_FINGER:
		case MORPHOERR_MOVED_FINGER:
		case MORPHOERR_SATURATED_FINGER:
		case MORPHOERR_INVALID_FINGER:
			l_x_ResultDlg.m_cs_Msg2.Format("Person not Authenticated.");
			break;
		case MORPHOERR_TIMEOUT:
			l_x_ResultDlg.m_cs_Msg2.Format("Timeout has expired.");
			l_x_ResultDlg.m_cs_Msg3.Format("Command Aborted.");
			break;
		case MORPHOERR_NOT_IMPLEMENTED:
			l_x_ResultDlg.m_cs_Msg2.Format("Not Implemented !!!");
			break;
		case MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN:
			l_x_ResultDlg.m_cs_Msg2.Format("Finger is misplaced or has been");
			l_x_ResultDlg.m_cs_Msg3.Format("withdrawn from sensor during acquisition.");
			break;
		case MORPHOERR_BADPARAMETER:
			l_x_ResultDlg.m_cs_Msg2.Format("One or more input parameters are out of range");
			break;
		default:
			l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
			l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Database::Verify() function");
			ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
		}

		if (m_b_EndThread == TRUE)
		{
			l_x_ResultDlg.DoModal();
			CleanAsyncMessages2();
		}
		return FALSE;
	}

	if ((m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED) || (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED))
	{
		if (AfxMessageBox("Privacy mode : User data info is crypted !\nWould you like te decrypt it ?", MB_YESNO) == IDYES)
		{
			RetKey = GetEncryptedKey(sl_encrypt_Key);
			if (RetKey) _decrypt = true;
		}
	}
	//------------------------------------------------------------
	// If cmd successful, retrieve lastname, firstname, ID 
	//------------------------------------------------------------

	for (j = 0; j <= 2; j++)
	{
		l_i_Ret = l_x_User.GetField(
			j,					//I			i_i_FieldIndex,
			l_ul_LenField,		//UL		o_ul_LenField,
			l_puc_DataField		//PUC		o_puc_DataField	
			);

		
		if (_decrypt)
		{
			bool b_decRet = _DecryptAESData(l_puc_DataField, l_ul_LenField, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
			if (!b_decRet || o_pul_Aes128CbcDeCryptedDataLen == 0)
			{
				AfxMessageBox("Decrypting data error !", MB_ICONERROR);
				return FALSE;
			}
			
			memcpy(l_puc_GenericBuf, o_ppuc_Aes128CbcDeCryptedData + 8, o_pul_Aes128CbcDeCryptedDataLen - 8);
			l_puc_GenericBuf[o_pul_Aes128CbcDeCryptedDataLen - 8] = 0;

			if (NULL != o_ppuc_Aes128CbcDeCryptedData)
				MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
		}
		else
		{
			memcpy(l_puc_GenericBuf, l_puc_DataField, l_ul_LenField);
			l_puc_GenericBuf[l_ul_LenField] = 0;
		}

		switch (j)
		{
		case 0:
			l_x_ResultDlg.m_cs_Msg4.Format("ID : %s", l_puc_GenericBuf);
			break;
		case 1:
			l_x_ResultDlg.m_cs_Msg2.Format("Firstname : %s", l_puc_GenericBuf);
			break;
		case 2:
			l_x_ResultDlg.m_cs_Msg3.Format("Lastname : %s", l_puc_GenericBuf);
			break;
		}
	}

	if (m_puc_ExportNumPk != NULL)
	{
		l_x_ResultDlg.m_cs_Msg5.Format("Matching Pk Number : %d", m_uc_ExportNumPk);
	}

	Beep(BEEP_FREQUENCY, BEEP_DURATION);
	ShowAnimation("image_Ok");
	l_x_ResultDlg.m_cs_Msg1.Format("User authenticated");
	l_x_ResultDlg.DoModal();
	CleanAsyncMessages2();

	return TRUE;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::_Verify()
{

	UL		l_ul_CallbackMask;
	CString	l_cs_Message;
	I		l_i_Ret;

	//------------------------------------------------------------
	// Viewer or not viewer.... 
	//------------------------------------------------------------

	l_ul_CallbackMask = m_ul_CallbackMask;
	l_ul_CallbackMask &= ~MORPHO_CALLBACK_ENROLLMENT_CMD;

	if (m_tbCtrl.m_TabOptionsDlg->m_b_MainViewerActive)
	{
		m_i_CurrentViewerIndex = 0;
		m_px_Image = new C_MORPHO_Image(1);
	}
	//-----------------------------------------------------------
	// Get security Level Added by M.EL KHACHBI
	//-----------------------------------------------------------
	//m_x_MorphoSmart.Set_FFD_SecurityLevel();
	GetSecurityLevel(0);
	m_x_MorphoSmart.Set_FFD_SecurityLevel(m_i_SecurityLevel);
	//------------------------------------------------------------
	// Strategy Acquisition Mode
	//------------------------------------------------------------	

	l_i_Ret = m_x_MorphoSmart.SetStrategyAcquisitionMode(m_tbCtrl.m_TabBioSettingsDlg->m_uc_StrategyAcquisitionMode); //FVP 1.10
	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling C_MORPHO_Device::SetStrategyAcquisitionMode() function", l_i_Ret);
		return l_i_Ret;
	}
	//------------------------------------------------------------
	// Authentication Mode ?
	//------------------------------------------------------------
	switch (m_BioOpPSheet.m_VerifyPPage.m_i_InputChoice)
	{

		// File
	case 0:
		_VerifyWithFile(l_ul_CallbackMask);
		break;
	case 1:
		_VerifyInBase(l_ul_CallbackMask);
		break;
		// Smart Card External
	case 2:
		_VerifyWithSmartCardExternal(l_ul_CallbackMask);
		break;
		// Smart Card Internal
	case 3:
		_VerifyWithSmartCardInternal(l_ul_CallbackMask);
		break;
		// Base
	}

	if (m_px_Image != NULL)
	{
		delete(m_px_Image);
		m_px_Image = NULL;
	}

	RedrawWnd();
	return TRUE;
}
//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::GetEncryptedKey(std::string	&l_encrypt_Key)
{

	int							l_i_ret = 0;
	CString						l_cs_fname, l_cs_fname1, l_cs_fext, l_cs_fext1;
	CResultDlg l_x_result_dlg;

	l_x_result_dlg.m_cs_Msg1.Format("Encrypt AES Data");

	AfxMessageBox("Please select the AES 128 Key file");
	// read encrypt key from file
	if (GetFilesNameToOpen("key.bin", l_cs_fname, l_cs_fext, "AES 128 Key Files (*.*)|*.*||", false))
	{
		std::ifstream l_x_fs((LPCTSTR)l_cs_fname, std::ios::in | std::ios::binary);
		l_encrypt_Key.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
		l_x_fs.close();
		if (l_encrypt_Key.size() != AES128_BLOCK_LENGTH)
		{
			l_x_result_dlg.m_cs_Msg3.Format("error: key is not 128 bits long");
			l_i_ret = -1;
			return false;
		}
	}
	else
	{
		l_x_result_dlg.m_cs_Msg3.Format("Error while getting KPRIVACY file");
		l_i_ret = -1;
		return false;
	}
	return true;
}

bool CMSO_DemoDlg::_CheckPrivacyData(PUC l_pc_data_to_check, UL l_ul_data_to_checkLen, std::string l_encrypt_Key)
{
	UL CRC32 = 0, RND32 = 0, crc32_check;
	UC l_puc_tmp_data[4];
	I l_i_ret = 0;

	memcpy(l_puc_tmp_data, l_pc_data_to_check, 4);

	UC l_uc_tmp = l_puc_tmp_data[0];
	l_puc_tmp_data[0] = l_puc_tmp_data[3];
	l_puc_tmp_data[3] = l_uc_tmp;

	l_uc_tmp = l_puc_tmp_data[1];
	l_puc_tmp_data[1] = l_puc_tmp_data[2];
	l_puc_tmp_data[2] = l_uc_tmp;

	memcpy(&CRC32, l_puc_tmp_data, 4);

	l_i_ret = MSOSECU_computeCRC32(l_ul_data_to_checkLen - 4, l_pc_data_to_check + 4, 0x04C11DB7, 0xFFFFFFFF, true, true, 0xFFFFFFFF, &crc32_check);

	if (l_i_ret == 0 && CRC32 == crc32_check)
	{
		return true;
	}

	return false;
}

bool CMSO_DemoDlg::_PreparePrivacyData(PC l_pc_data_to_encrypt, UL l_ul_data_to_encryptLen, std::string	l_encrypt_Key, UI i_random, BOOL i_b_addStringTerminator, PUC l_encrypted_data, PUL l_encrypted_data_len)
{
	PUC l_puc_Aes128CbcCryptedData = NULL; 
	UL	l_ul_Aes128CbcCryptedDataLen = 0;
	BOOL l_b_isNewBuffer = FALSE;
	
	PC l_pc_data = l_pc_data_to_encrypt;
	UL l_ul_dataSize = l_ul_data_to_encryptLen;
	
	if (l_ul_data_to_encryptLen == 0 && i_b_addStringTerminator)
	{
		l_ul_dataSize++;
		l_pc_data = new C[l_ul_dataSize];
		l_pc_data[0] = '\0';
		l_b_isNewBuffer = TRUE;
	}
	else if (l_ul_data_to_encryptLen > 0 && i_b_addStringTerminator && l_pc_data[l_ul_data_to_encryptLen - 1] != '\0')
	{
		l_ul_dataSize++;
		l_pc_data = new C[l_ul_dataSize];
		memcpy(l_pc_data, l_pc_data_to_encrypt, l_ul_data_to_encryptLen);
		l_pc_data[l_ul_data_to_encryptLen] = '\0';
		l_b_isNewBuffer = TRUE;
	}
	_EncryptAESData(l_pc_data, l_ul_dataSize, &l_puc_Aes128CbcCryptedData, &l_ul_Aes128CbcCryptedDataLen, l_encrypt_Key, i_random);
	memcpy(l_encrypted_data, l_puc_Aes128CbcCryptedData, l_ul_Aes128CbcCryptedDataLen);
	*l_encrypted_data_len = l_ul_Aes128CbcCryptedDataLen;

	if (NULL != l_puc_Aes128CbcCryptedData)
		MSOSECU_Free(&l_puc_Aes128CbcCryptedData);

	if (l_b_isNewBuffer ==TRUE && l_pc_data != NULL)
	{
		delete l_pc_data;
	}
	return TRUE;
}

bool CMSO_DemoDlg::_EncryptAESData(PC l_data_to_encrypt, UL l_data_to_encryptLen, PUC *o_ppuc_Aes128CbcCryptedData, PUL	o_pul_Aes128CbcCryptedDataLen, std::string	l_encrypt_Key, UI i_random)
{
	int							l_i_ret = 0;
	int							l_i_cryptlen = 0;
	bool	b_ret = false;
	FILE * pFile;

	if (NULL == l_data_to_encrypt) return false;

	UC *data_buffer = new UC[4 + 4 + l_data_to_encryptLen];
	UC *ptr = data_buffer;
	UC tmp;
	memcpy(data_buffer, &i_random, 4);

	memcpy(data_buffer + 4, l_data_to_encrypt, l_data_to_encryptLen);
	UL crc;

	l_i_ret = MSOSECU_computeCRC32(4 + l_data_to_encryptLen, data_buffer, 0x04C11DB7, 0xFFFFFFFF, true, true, 0xFFFFFFFF, &crc);

	memcpy(data_buffer, &crc, 4);

	tmp = data_buffer[0];
	data_buffer[0] = data_buffer[3];
	data_buffer[3] = tmp;

	tmp = data_buffer[1];
	data_buffer[1] = data_buffer[2];
	data_buffer[2] = tmp;
	memcpy(data_buffer + 4, &i_random, 4);
	memcpy(data_buffer + 8, l_data_to_encrypt, l_data_to_encryptLen);

	UC		i_puc_Aes128CbcInputVector[AES128_BLOCK_LENGTH];
	memset(i_puc_Aes128CbcInputVector, 0, AES128_BLOCK_LENGTH);
	if (l_i_ret == 0)
	{
		l_i_ret = MSOSECU_encrypt_aes128_cbc(
			l_encrypt_Key.length(),
			(PUC)l_encrypt_Key.c_str(),
			4 + 4 + l_data_to_encryptLen,
			(PUC)data_buffer,
			i_puc_Aes128CbcInputVector, TRUE, o_pul_Aes128CbcCryptedDataLen, o_ppuc_Aes128CbcCryptedData);
	}

	if (l_i_ret == 0)
	{
		b_ret = true;
	}
	else
	{
		b_ret = false;
	}

	if (data_buffer)
		delete data_buffer;

	return b_ret;

}
bool CMSO_DemoDlg::_DecryptAESData(PUC _auc_FileBuffer, UL _ul_FileBufferSize, PUC	*o_ppuc_Aes128CbcClearData, UL	&o_pul_Aes128CbcClearDataLen, std::string	l_encrypt_Key)
{
	UC		i_puc_Aes128CbcInputVector[AES128_BLOCK_LENGTH];
	CString						l_cs_fname, l_cs_fname1, l_cs_fext, l_cs_fext1;
	int							l_i_ret = 0;
	int							l_i_cryptlen = 0;

	if (NULL==_auc_FileBuffer) return false;

	CResultDlg l_x_result_dlg;
	l_x_result_dlg.m_cs_Msg1.Format("Decrypt AES Data");

	memset(i_puc_Aes128CbcInputVector, 0, AES128_BLOCK_LENGTH);

	l_i_ret = MSOSECU_decrypt_aes128_cbc(l_encrypt_Key.length(), (PUC)l_encrypt_Key.c_str(), _ul_FileBufferSize, _auc_FileBuffer, i_puc_Aes128CbcInputVector, TRUE, &o_pul_Aes128CbcClearDataLen, o_ppuc_Aes128CbcClearData);
	if (l_i_ret == 0)
	{
		if (o_pul_Aes128CbcClearDataLen > 0)
		{
			return true;
		}

	}
	else
	{
		l_x_result_dlg.m_cs_Msg1.Format("Decrypt Failed %d %X", l_i_ret, l_i_ret);
		l_x_result_dlg.DoModal();
	}
	return false;
}
//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::_Capture()
{
	I						i;
	UL						l_ul_LenTemplate;
	PUC						l_puc_DataTemplate;
	UC						l_auc_FileBuffer[TEMPLATE_BUFFER_MAX_SIZE];//5ko
	UL						l_ul_FileBufferSize;
	T_MORPHO_TYPE_TEMPLATE	l_t_TypeTemplate;
	T_MORPHO_FVP_TYPE_TEMPLATE l_t_FVPTypeTemplate;
	I						l_i_Ret;
	CString					l_cs_Message;
	UL						l_ul_CallbackMask;
	bool					l_b_Ret;
	CResultDlg				l_x_ResultDlg;
	UL						l_ul_pk_X984Size;
	PUC						l_puc_pk_X984;
	UL						l_ul_fvp_X984Size;
	PUC						l_puc_fvp_X984;
	I						l_i_MagicNumber;
	T_MORPHO_TEMPLATE_ENVELOP	l_x_TypEnvelop;
	PT_TKB_HDR				l_px_TkbHdr;
	C						l_auc_PersonnalInformation[1024];
	UL						l_ul_PersonnalInformationSize;
	UC						l_auc_X984Data[1024];
	UL						l_ul_X984DataSize = 0;
	UC						l_uc_NbFingers;
	I						l_i_Tmp;
	T_MORPHO_TYPE_TEMPLATE	l_x_TypeTemplate;
	T_MORPHO_FVP_TYPE_TEMPLATE l_x_FVPTypeTemplate;
	US						l_us_MaxSizeTemplate;
	UC						l_auc_FileBuffer2[TEMPLATE_BUFFER_MAX_SIZE];
	UL						l_ul_FileBufferSize2;
	CString					l_cs_FileID;
	CBioDemoCard			*l_px_BioDemoCard;
	UL				        l_ul_DetectModeChoice;
	UC				        l_uc_DetectModeChoice;
	UC						l_uc_AdvancedSecurityLevelsRequired;
	bool					l_b_notify_confirmation = false;

	bool					_decrypt = false;
	PUC						l_puc_Aes128CbcDeCryptedData = NULL;
	UL						l_ul_Aes128CbcDeCryptedDataLen = 0;
	bool					RetKey = false;
	std::string				sl_encrypt_Key;
	BOOL					isPrivacyEnabled = (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED || m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED);


	l_b_Ret = TRUE;

	//------------------------------------------------------------
	// Exit now if no ID
	//------------------------------------------------------------

	if ((m_BioOpPSheet.m_CodePPage.m_cs_ID.GetLength()) == 0)
	{
		AfxMessageBox("You must at least type an ID.");
		return FALSE;
	}

	if (isPrivacyEnabled)
	{
		RetKey = GetEncryptedKey(sl_encrypt_Key);
		if (RetKey == false)
		{
			AfxMessageBox("Error occured when loading encryption key.");
			return FALSE;
		}
	}
	

	//-----------------------------------------------------------
	// Get security Level Added by M.EL KHACHBI
	//-----------------------------------------------------------
	//m_x_MorphoSmart.Set_FFD_SecurityLevel();
	GetSecurityLevel(1);
	m_x_MorphoSmart.Set_FFD_SecurityLevel(m_i_SecurityLevel);
	//------------------------------------------------------------
	// Write personnal data in the the buffer.
	//------------------------------------------------------------

	// Personnal Information :
	// - Size ID		: 2 bytes
	// - ID				: <Size ID> bytes
	// - Size Firstname : 2 bytes
	// - Firstname		: <Size Firstname> bytes
	// - Size Lastname	: 2 bytes
	// - Lastname		: <Size Lastname> bytes

	//Size ID	
	l_i_Tmp = m_BioOpPSheet.m_CodePPage.m_cs_ID.GetLength();
	memcpy(l_auc_PersonnalInformation, &l_i_Tmp, 2);
	l_ul_PersonnalInformationSize = 2;



	//ID
	memcpy((PC)l_auc_PersonnalInformation + l_ul_PersonnalInformationSize, m_BioOpPSheet.m_CodePPage.m_cs_ID, l_i_Tmp);
	l_ul_PersonnalInformationSize += l_i_Tmp;

	//Size Firstname
	l_i_Tmp = m_BioOpPSheet.m_CodePPage.m_cs_Firstname.GetLength();
	memcpy(l_auc_PersonnalInformation + l_ul_PersonnalInformationSize, &l_i_Tmp, 2);
	l_ul_PersonnalInformationSize += 2;

	//Firstname
	memcpy((PC)l_auc_PersonnalInformation + l_ul_PersonnalInformationSize, m_BioOpPSheet.m_CodePPage.m_cs_Firstname, l_i_Tmp);
	l_ul_PersonnalInformationSize += l_i_Tmp;

	//Size Lastname
	l_i_Tmp = m_BioOpPSheet.m_CodePPage.m_cs_Lastname.GetLength();
	memcpy(l_auc_PersonnalInformation + l_ul_PersonnalInformationSize, &l_i_Tmp, 2);
	l_ul_PersonnalInformationSize += 2;

	//Lastname
	memcpy((PC)l_auc_PersonnalInformation + l_ul_PersonnalInformationSize, m_BioOpPSheet.m_CodePPage.m_cs_Lastname, l_i_Tmp);
	l_ul_PersonnalInformationSize += l_i_Tmp;

	//---------------------------------
	// Write in file have been selected
	//---------------------------------
	CString		l_cs_Tmp;
	CString		l_cs_Tmp_FVP;

	//---------------------------------
	//X984 selected
	//---------------------------------
	if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == TRUE)
	{
		//------------------------------------------------------------
		// Set the template envelop type
		//------------------------------------------------------------
		l_x_TypEnvelop = MORPHO_X984_SIGNED_TEMPLATE;

		if (m_puc_MsoCertif == NULL)
		{
			//read the MorphoSmartTM certification 
			//the MorphoSmartTM certification is stored in file .tkb
			l_i_Ret = m_x_MorphoSmart.SecuReadCertificate(
				0,						//MorphoSmartTM level	//UC			i_uc_index,			
				&m_ul_MsoCertifSize,	//PUL			o_pul_CertifSize,
				&m_puc_MsoCertif		//PUC*			o_ppuc_Certif
				);

			if (l_i_Ret != MORPHO_OK)
			{
				//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
				l_x_ResultDlg.m_cs_Msg1.Format("Read certificate Failed.");
				l_x_ResultDlg.m_cs_Msg2.Format("Please check that the device is secured.");
				l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Device::SecuReadCertificate()");
				ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
				l_x_ResultDlg.DoModal();
				return FALSE;
			}
		}

		//------------------------------------------------------------
		// Write X984 data in the the buffer.
		//------------------------------------------------------------
		// X984 Data:
		// _ Magic Number		: 4 bytes
		// _ Personnal Information

		l_i_MagicNumber = MAGIC_NB_DEMO;
		memcpy(l_auc_X984Data, &l_i_MagicNumber, sizeof(I));
		l_ul_X984DataSize = sizeof(I);

		memcpy(l_auc_X984Data + l_ul_X984DataSize, l_auc_PersonnalInformation, l_ul_PersonnalInformationSize);
		l_ul_X984DataSize += l_ul_PersonnalInformationSize;

		l_cs_Tmp = "Token Bio Files (*.tkbEXT_CRYPT)|*.tkbEXT_CRYPT|All Files (*.*)|*.*||";
	}

	//---------------------------------
	//X984 not selected
	//---------------------------------
	else
	{
		//------------------------------------------------------------
		// Set the template envelop type
		//------------------------------------------------------------
		l_x_TypEnvelop = MORPHO_RAW_TEMPLATE;

		l_ul_X984DataSize = 0;
		memset(l_auc_X984Data, 0, sizeof(l_auc_X984Data));
	}
	//----------------------
	// FP aquisition enabled
	//----------------------

	if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType != STR_NO_PK_FP)
	{
		if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_PKCOMP)
		{
			l_x_TypeTemplate = MORPHO_PK_COMP;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "PK_COMP Files (*.pkcEXT_CRYPT)|*.pkcEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_PKCOMPNORM)
		{
			l_x_TypeTemplate = MORPHO_PK_COMP_NORM;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "PK_COMP_NORM Files (*.pkcnEXT_CRYPT)|*.pkcnEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_PKMAT)
		{
			l_x_TypeTemplate = MORPHO_PK_MAT;
			l_us_MaxSizeTemplate = 1;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "PK_MAT Files (*.pkmEXT_CRYPT)|*.pkmEXT_CRYPT|PK_MAT Files (*.pkmatEXT_CRYPT)|*.pkmatEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_PKMATNORM)
		{
			l_x_TypeTemplate = MORPHO_PK_MAT_NORM;
			l_us_MaxSizeTemplate = 1;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "PK_MAT_NORM Files (*.pkmnEXT_CRYPT)|*.pkmnEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_PKLITE)
		{
			l_x_TypeTemplate = MORPHO_PK_PKLITE;
			l_us_MaxSizeTemplate = 1;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "PKLITE Files (*.pklEXT_CRYPT)|*.pklEXT_CRYPT|PKLITE Files (*.pkliteEXT_CRYPT)|*.pkliteEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_PKS)
		{
			l_x_TypeTemplate = MORPHO_PK_COMP;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "PKs Files (*.pksEXT_CRYPT)|*.pksEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_ANSI378)
		{
			l_x_TypeTemplate = MORPHO_PK_ANSI_378;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "ANSI 378 Files Files(*.ansi-fmrEXT_CRYPT)|*.ansi-fmrEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_ANSI378_2009)
		{
			l_x_TypeTemplate = MORPHO_PK_ANSI_378_2009;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "ANSI 378 V.2009 Files Files(*.ansi-fmr-2009EXT_CRYPT)|*.ansi-fmr-2009EXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_ISO_FMC_CS)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMC_CS;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "ISO 19794-2 FMC-CS Files(*.iso-fmc-csEXT_CRYPT)|*.iso-fmc-csEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_ISO_FMC_NS)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMC_NS;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "ISO 19794-2 FMC-NS Files(*.iso-fmc-nsEXT_CRYPT)|*.iso-fmc-nsEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_ISO_FMR)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMR;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "ISO 19794-2 Files(*.iso-fmrEXT_CRYPT)|*.iso-fmrEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_ISO_FMR_2011)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMR_2011;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "ISO 19794-2 Files(*.iso-fmr-2011EXT_CRYPT)|*.iso-fmr-2011EXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_MINEX_A)
		{
			l_x_TypeTemplate = MORPHO_PK_MINEX_A;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "MINEX A Files (*.minex-aEXT_CRYPT)|*.minex-aEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_PK_MOC)
		{
			l_x_TypeTemplate = MORPHO_PK_MOC;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "SAGEM PKMOC (*.mocEXT_CRYPT)|*.mocEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_PK_DIN_V66400_CS)
		{
			l_x_TypeTemplate = MORPHO_PK_DIN_V66400_CS;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "DIN-V66400 Compact Size400 Compact Size (*.din-csEXT_CRYPT)|*.din-csEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_PK_DIN_V66400_CS_AA)
		{
			l_x_TypeTemplate = MORPHO_PK_DIN_V66400_CS_AA;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "DIN-V66400 Compact Size400 Compact Size (*.din-csEXT_CRYPT)|*.din-csEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
		else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_ISO_FMC_CS_AA)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMC_CS_AA;
			l_us_MaxSizeTemplate = 255;
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == FALSE)
			{
				l_cs_Tmp = "ISO 19794-2 FMC-CS-AA Files(*.iso-fmc-csEXT_CRYPT)|*.iso-fmc-csEXT_CRYPT|All Files (*.*)|*.*||";
			}
		}
	}
	else
	{
		if (m_bl_FVP == FALSE)
		{
			l_x_TypeTemplate = MORPHO_PK_COMP;  //	MSO_Demo demandera toujours l’export des templates PK_COMP 
		}
		else
		{
			l_x_TypeTemplate = MORPHO_NO_PK_FP;
		}
		l_cs_Tmp = "PK_COMP Files (*.pkcEXT_CRYPT)|*.pkcEXT_CRYPT|All Files (*.*)|*.*||";
		l_us_MaxSizeTemplate = 255;
	}

	if (m_bl_FVP)
	{
		//FVP 1.6
		if (m_BioOpPSheet.m_CodePPage.m_i_CaptureType != 1){
			l_x_FVPTypeTemplate = MORPHO_PK_FVP;
			l_cs_Tmp_FVP = "SAGEM PkFVP (*.fvp)|*.fvp|All Files (*.*)|*.*||";
		}
		else {
			l_x_FVPTypeTemplate = MORPHO_PK_FVP_MATCH;
			l_cs_Tmp_FVP = "SAGEM PkFVP Match (*.fvp-m)|*.fvp-m|All Files (*.*)|*.*||";
		}
	}
	else
	{
		l_x_FVPTypeTemplate = MORPHO_NO_PK_FVP;
	}

	//------------------------------------------------------------
	// Viewer or not viewer.... 
	//------------------------------------------------------------

	l_ul_CallbackMask = m_ul_CallbackMask;

	if (m_tbCtrl.m_TabOptionsDlg->m_b_MainViewerActive)
	{
		m_px_Image = new C_MORPHO_Image(6);
	}

	if (m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
	{
		PostMessage(WM_ADD_PROCESS_PAGE, 0, 0);
	}
	else
	{
		m_ctl_CurrentMiniViewer = NULL;
	}

	//------------------------------------------------------------
	// Number of finger
	//------------------------------------------------------------

	l_uc_NbFingers = m_BioOpPSheet.m_CodePPage.m_i_NbFingers + 1;

	//------------------------------------------------------------
	// Detect Mode Choice 
	//------------------------------------------------------------
	l_ul_DetectModeChoice = MORPHO_ENROLL_DETECT_MODE;
	if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_ForceFingerPlacement != 0)
	{
		l_ul_DetectModeChoice |= MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE;
	}

	l_ul_DetectModeChoice = l_ul_DetectModeChoice | m_ul_WakeUpMode;

	if (l_ul_DetectModeChoice > 0xFF)
	{
		l_cs_Message.Format("DetectModeChoice overfloat 0x%08x>0xFF(UC), information lost.", l_ul_DetectModeChoice);
		AfxMessageBox(l_cs_Message, MB_ICONERROR);
	}

	l_uc_DetectModeChoice = (UC)l_ul_DetectModeChoice;

	//------------------------------------------------------------
	// Advanced Security Levels Compatibility Required 
	//------------------------------------------------------------
	if (m_BioOpPSheet.m_CodePPage.m_i_CaptureType != 1){
		// Capture 3 poses
		if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_ASLC){
			l_uc_AdvancedSecurityLevelsRequired = 1;
		}
		else {
			l_uc_AdvancedSecurityLevelsRequired = 0;
		}
	}
	else {
		// Capture 1-poses
		l_uc_AdvancedSecurityLevelsRequired = 0xFF;
		if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_ASLC){
			l_uc_AdvancedSecurityLevelsRequired = 1;
		}
	}

	//------------------------------------------------------------
	// Strategy Acquisition Mode
	//------------------------------------------------------------	

	l_i_Ret = m_x_MorphoSmart.SetStrategyAcquisitionMode(m_tbCtrl.m_TabBioSettingsDlg->m_uc_StrategyAcquisitionMode); // FVP 1.10
	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling C_MORPHO_Device::SetStrategyAcquisitionMode() function", l_i_Ret);
		return l_i_Ret;
	}
	//------------------------------------------------------------
	// Extract Templates
	//------------------------------------------------------------
	C_MORPHO_TemplateList 	l_x_TemplateList;

	m_b_BiometricOperationProcessing = TRUE;


	CString		l_cs_FingerQuality;
	I			l_i_FingerQuality;
	l_i_FingerQuality = 0;
	if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_FingerPrint_Quality)
	{
		m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_EDIT_FINGERPRINT_QUALITY)->GetWindowText(l_cs_FingerQuality);
		l_i_FingerQuality = atoi(l_cs_FingerQuality);
	}

	l_i_Ret = m_x_MorphoSmart.Capture(
		m_tbCtrl.m_TabBioSettingsDlg->m_i_Timeout,	//US							i_us_Timeout,
		(UC)l_i_FingerQuality,											//UC						    i_uc_AcquisitionThreshold,
		l_uc_AdvancedSecurityLevelsRequired,		//UC						    i_uc_AdvancedSecurityLevelsRequired,
		l_uc_NbFingers,								//UC							i_uc_FingerNumber,
		l_x_TypeTemplate,							//T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
		l_x_FVPTypeTemplate,                        //T_MORPHO_FVP_TYPE_TEMPLATE    i_x_FVPTypeTemplate;
		l_us_MaxSizeTemplate,						//US							i_us_MaxSizeTemplate,	
		m_BioOpPSheet.m_CodePPage.m_i_CaptureType,	//UC							i_uc_enrolType (1), 
		l_ul_CallbackMask,							//UL							i_ul_CallbackCmd,
		BiometricCallback,							//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback, 
		this,										//PVOID							i_pv_CallbackArgument,
		l_x_TemplateList,							//C_MORPHO_TemplateList &		o_x_Template;
		l_x_TypEnvelop,								//T_MORPHO_TEMPLATE_ENVELOP		i_x_typEnvelop,
		l_ul_X984DataSize,							//I								i_i_ApplicationDataLen,
		l_auc_X984Data,								//PUC							i_puc_ApplicationData
		m_BioOpPSheet.m_CodePPage.m_b_LatentDetect,	//UC							i_uc_LatentDetection
		m_tbCtrl.m_TabBioSettingsDlg->m_i_CoderChoice,							//I								i_i_CoderChoice
		(UC)(l_uc_DetectModeChoice | m_ul_WakeUpMode),//MORPHO_ENROLL_DETECT_MODE|m_ul_WakeUpMode,	//UL							i_ul_DetectModeChoice
		0,											//UC							i_uc_SaveIndexImage
		NULL,										//PT_MORPHO_MOC_PARAMETERS		i_px_MocParameters
		MORPHO_NO_COMPRESS,							//T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
		0											//UC							i_uc_CompressRate
		);

	// (1) :	- m_i_CaptureType = 0 => capture based on 3 acquisitions + templates consolidation to compute the best reference template
	//			- m_i_CaptureType = 1 => capture based on 1 single acquisition to compute a template for verification

	m_b_BiometricOperationProcessing = FALSE;
	CleanAsyncMessages();

	// Get FFD logs and write in log file
	GetAndWriteFFDLogs(&m_x_MorphoSmart);

	//------------------------------------------------
	// Remove handle on the Miniviewer 
	m_ctl_CurrentMiniViewer = NULL;
	//------------------------------------------------

	if (l_i_Ret == MORPHOERR_DEVICE_LOCKED)
	{
		l_i_Ret = UnlockDevice();
		if (l_i_Ret != MORPHO_OK)
		{
			ShowErrorBox("An error occured during Device Unlocking", l_i_Ret);
		}
		l_b_Ret = FALSE;
	}
	else if (l_i_Ret != MORPHO_OK)
	{
		ShowAnimation("image_Ko");
		switch (l_i_Ret)
		{
		case MORPHOERR_NO_HIT:
			l_x_ResultDlg.m_cs_Msg2.Format("Bad Capture Sequence.");
			break;
		case MORPHOERR_CMDE_ABORTED:
			l_x_ResultDlg.m_cs_Msg2.Format("Command aborted by user.");
			break;
		case MORPHOERR_TIMEOUT:
			l_x_ResultDlg.m_cs_Msg2.Format("Timeout has expired.");
			l_x_ResultDlg.m_cs_Msg3.Format("Command aborted.");
			break;
		case MORPHOERR_FFD:
		case MORPHOERR_MOIST_FINGER:
		case MORPHOERR_MOVED_FINGER:
		case MORPHOERR_SATURATED_FINGER:
		case MORPHOERR_INVALID_FINGER:
			l_x_ResultDlg.m_cs_Msg2.Format("Warning : Low quality finger !!!");
			break;
		case MORPHOERR_BADPARAMETER:
			l_x_ResultDlg.m_cs_Msg2.Format("One or more input parameters are out of range");
			//ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
			break;
		case MORPHOERR_LICENSE_MISSING:
			l_x_ResultDlg.m_cs_Msg2.Format("A required license is missing.");
			break;
		case MORPHOERR_ADVANCED_SECURITY_LEVEL_NOT_AVAILABLE:
			l_x_ResultDlg.m_cs_Msg2.Format("Cannot make a multimodal template compatible with advanced security levels.");
			break;
		case MORPHOERR_ADVANCED_SECURITY_LEVEL_MISMATCH:
			l_x_ResultDlg.m_cs_Msg2.Format("Failed to make a multimodal template compatible with advanced security levels.");
			break;
		case MORPHOERR_BAD_FINAL_FINGER_PRINT_QUALITY:
			l_x_ResultDlg.m_cs_Msg2.Format("Failed to reach the fingerprint quality threshold.");
			break;
		default:
			l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
			l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Device::Capture() function");
			ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
		}
		l_x_ResultDlg.m_cs_Msg1.Format("Capture Failed");
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_x_ResultDlg.DoModal();
		CleanAsyncMessages2();
		l_b_Ret = FALSE;
	}
	else
	{
		ShowAnimation("image_Ok");

		if (m_IsDataEncryption && !m_bl_FVP)
		{
			l_cs_Tmp.Replace("EXT_CRYPT", ".crypt");
		}
		else
		{
			l_cs_Tmp.Replace("EXT_CRYPT", "");
		}

		if (isPrivacyEnabled)
		{
			if (AfxMessageBox("Privacy Mode : template data is crypted !\nWould you like to decrypt it before saving ?", MB_YESNO) == IDYES)
			{
				RetKey = GetEncryptedKey(sl_encrypt_Key);
				if (RetKey) _decrypt = true;
			}
		}

		if (m_BioOpPSheet.m_CodePPage.m_i_OutputChoice == 0)
		{
			CString		l_cs_SaveFile;
			//---------------------
			// Write in File 
			//---------------------
			if (m_BioOpPSheet.m_CodePPage.m_b_TokenBio == TRUE)
			{
				CString l_tkp_file_name = "";

				l_cs_Tmp = "Token Bio Files (*.tkb)|*.tkb|All Files (*.*)|*.*||";
				
				if (l_x_TypeTemplate != MORPHO_NO_PK_FP)
				{

					//---------------------
					// Token Bio File 
					//---------------------
					// the .tkb file structure : 
					// _ the .tkb Header File (see the T_TKB_HDR structure)
					// _ the X984 Biometric Token
					// _ the X509 Mso certificate

					l_x_TemplateList.GetX984(
						l_ul_pk_X984Size,
						l_puc_pk_X984
						);

					// set the .tkb Header File
					l_px_TkbHdr = (PT_TKB_HDR)l_auc_FileBuffer;
					l_px_TkbHdr->m_ul_MagicNb = MAGIC_NB_DEMO;
					l_px_TkbHdr->m_ul_Version = TKB_FILE_VERSION;
					l_px_TkbHdr->m_ul_SizeTkb = l_ul_pk_X984Size;
					l_px_TkbHdr->m_ul_SizeCertif = m_ul_MsoCertifSize;
					l_ul_FileBufferSize = sizeof(T_TKB_HDR);

					//the X984 Biometric Token
					memcpy(l_auc_FileBuffer + l_ul_FileBufferSize, l_puc_pk_X984, l_ul_pk_X984Size);
					l_ul_FileBufferSize += l_ul_pk_X984Size;

					//the X509 Mso certificate
					memcpy(l_auc_FileBuffer + l_ul_FileBufferSize, m_puc_MsoCertif, m_ul_MsoCertifSize);
					l_ul_FileBufferSize += m_ul_MsoCertifSize;

					l_tkp_file_name = m_BioOpPSheet.m_CodePPage.m_cs_ID + "_fp";
					CFileDialog l_x_SaveFileDlg(FALSE,
						"",
						l_tkp_file_name,
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
						l_cs_Tmp,
						this);
					SAV_PATH
						if (l_x_SaveFileDlg.DoModal() != IDOK)
						{
							RESTORE_PATH
								l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
							l_x_ResultDlg.m_cs_Msg2.Format("Saving FP template file");
							l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
							l_b_Ret = FALSE;
						}
						else
						{
							RESTORE_PATH
								l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

							//- - - - - - - - - - - 
							// Write data in File
							//- - - - - - - - - - -

							CString l_cs_File;
							l_cs_File.Format("%s", l_cs_SaveFile);

							CFile l_f_PkFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
							l_f_PkFile.Write(l_auc_FileBuffer, l_ul_FileBufferSize);

							l_f_PkFile.Close();

							//- - - - - - - - - - - -
							l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
							l_x_ResultDlg.m_cs_Msg3.Format("FP template with a X9.84 envelop ");
							l_x_ResultDlg.m_cs_Msg5.Format("successfully exported in file");
							l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
							Beep(BEEP_FREQUENCY, BEEP_DURATION);
							l_b_Ret = TRUE;
						}
					l_x_ResultDlg.DoModal();
					l_x_ResultDlg.InitializeMsg();
					CleanAsyncMessages2();
				}

				if (m_bl_FVP)
				{
					l_ul_FileBufferSize2 = 0;

					l_x_TemplateList.GetFVPX984(
						l_ul_fvp_X984Size,
						l_puc_fvp_X984
						);

					// set the .tkb Header File
					l_px_TkbHdr = (PT_TKB_HDR)l_auc_FileBuffer2;
					l_px_TkbHdr->m_ul_MagicNb = MAGIC_NB_DEMO;
					l_px_TkbHdr->m_ul_Version = TKB_FILE_VERSION;
					l_px_TkbHdr->m_ul_SizeTkb = l_ul_fvp_X984Size;
					l_px_TkbHdr->m_ul_SizeCertif = m_ul_MsoCertifSize;
					l_ul_FileBufferSize2 = sizeof(T_TKB_HDR);

					//the X984 Biometric Token
					memcpy(l_auc_FileBuffer2 + l_ul_FileBufferSize2, l_puc_fvp_X984, l_ul_fvp_X984Size);
					l_ul_FileBufferSize2 += l_ul_fvp_X984Size;

					//the X509 Mso certificate
					memcpy(l_auc_FileBuffer2 + l_ul_FileBufferSize2, m_puc_MsoCertif, m_ul_MsoCertifSize);
					l_ul_FileBufferSize2 += m_ul_MsoCertifSize;

					l_tkp_file_name = m_BioOpPSheet.m_CodePPage.m_cs_ID + "_fvp";

					CFileDialog l_x_SaveFileDlg(FALSE,
						"",
						l_tkp_file_name,
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
						l_cs_Tmp,
						this);

					SAV_PATH

						if (l_x_SaveFileDlg.DoModal() != IDOK)
						{
							RESTORE_PATH
								l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
							l_x_ResultDlg.m_cs_Msg2.Format("Saving FVP template file");
							l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
							l_b_Ret = FALSE;
						}
						else
						{
							RESTORE_PATH
								l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

							//- - - - - - - - - - - 
							// Write data in File
							//- - - - - - - - - - -

							CString l_cs_File;
							l_cs_File.Format("%s", l_cs_SaveFile);

							CFile l_f_PkFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
							l_f_PkFile.Write(l_auc_FileBuffer2, l_ul_FileBufferSize2);

							l_f_PkFile.Close();

							//- - - - - - - - - - - -
							l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
							l_x_ResultDlg.m_cs_Msg3.Format("FVP template with a X9.84 envelop ");
							l_x_ResultDlg.m_cs_Msg5.Format("successfully exported in file");
							l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
							Beep(BEEP_FREQUENCY, BEEP_DURATION);
							l_b_Ret = TRUE;
						}
					l_x_ResultDlg.DoModal();
					l_x_ResultDlg.InitializeMsg();
					CleanAsyncMessages2();
				}
			}
			else if (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_PKS) // Not Token Bio File 
			{
				if (m_bl_FVP == TRUE && m_BioOpPSheet.m_CodePPage.m_cs_FVP_TemplateType != STR_NO_FVP)
				{
					for (i = 0; i<l_uc_NbFingers; ++i)
					{
						UC l_uc_dataIndex;
						UC l_uc_PkFpQuality;
						UC l_uc_AdvancedSecurityLevelsCompatibility;
						l_i_Ret = l_x_TemplateList.GetFVPTemplate(
							i,										// UC	i_uc_indexTemplate,
							l_t_FVPTypeTemplate,					// T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
							l_ul_LenTemplate,						// UL	&o_ul_lenTemplate,
							l_puc_DataTemplate,						// PUC	&o_puc_dataTemplate
							l_uc_PkFpQuality,
							l_uc_AdvancedSecurityLevelsCompatibility,
							l_uc_dataIndex
							);

						if (l_i_Ret != MORPHO_OK){
							ShowErrorBox("An error occured while calling C_MORPHO_TemplateList::GetFVPTemplate() function", l_i_Ret);
							PostMessage(WM_REMOVE_PROCESS_PAGE, 0, (LPARAM)&(m_BioOpPSheet.m_CodePPage));
							return FALSE;
						}

						l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
						if (i == 0)
						{
							l_x_ResultDlg.m_cs_Msg2.Format("Finger #1 - Quality Score: %u  ", l_uc_PkFpQuality);
						}
						else
						{
							l_x_ResultDlg.m_cs_Msg2.Format("Finger #2 -  Quality Score: %u  ", l_uc_PkFpQuality);
						}
						if (m_BioOpPSheet.m_CodePPage.m_i_CaptureType == 0){
							l_x_ResultDlg.m_cs_Msg3.Format("Advanced Security Levels Compatibility: %s", (l_uc_AdvancedSecurityLevelsCompatibility == 1) ? "Yes" : "No");
						}

						Beep(BEEP_FREQUENCY, BEEP_DURATION);

						l_x_ResultDlg.DoModal();
						l_x_ResultDlg.InitializeMsg();

						// Save first finger (Finger #1) FVP template in a buffer 
						if (i == 0)
						{
							if (_decrypt == true)
							{
								bool b_decRet = _DecryptAESData(l_puc_DataTemplate, l_ul_LenTemplate, &l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
								if (!b_decRet || l_ul_Aes128CbcDeCryptedDataLen == 0)
								{
									AfxMessageBox("Decrypting data error !", MB_ICONERROR);
									return FALSE;
								}

								if (!_CheckPrivacyData(l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
								{
									AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
									if (l_puc_Aes128CbcDeCryptedData)
										MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
									return FALSE;
								}

								memcpy(l_auc_FileBuffer, l_puc_Aes128CbcDeCryptedData + 8, l_ul_Aes128CbcDeCryptedDataLen-8);
								l_ul_FileBufferSize = l_ul_Aes128CbcDeCryptedDataLen - 8;

								if (l_puc_Aes128CbcDeCryptedData)
									MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
							}
							else
							{
								memcpy(l_auc_FileBuffer, l_puc_DataTemplate, l_ul_LenTemplate);
								l_ul_FileBufferSize = l_ul_LenTemplate;
							}
							
						}
						else if (i == 1)
						{// Save second finger (Finger #2) FVP template in an other buffer 
							if (_decrypt == true)
							{
								bool b_decRet = _DecryptAESData(l_puc_DataTemplate, l_ul_LenTemplate, &l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
								if (!b_decRet || l_ul_Aes128CbcDeCryptedDataLen == 0)
								{
									AfxMessageBox("Decrypting data error !", MB_ICONERROR);
									return FALSE;
								}

								if (!_CheckPrivacyData(l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
								{
									AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
									if (l_puc_Aes128CbcDeCryptedData)
										MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
									return FALSE;
								}

								memcpy(l_auc_FileBuffer2, l_puc_Aes128CbcDeCryptedData + 8, l_ul_Aes128CbcDeCryptedDataLen - 8);
								l_ul_FileBufferSize2 = l_ul_Aes128CbcDeCryptedDataLen - 8;

								if (l_puc_Aes128CbcDeCryptedData)
									MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
							}
							else
							{
								memcpy(l_auc_FileBuffer2, l_puc_DataTemplate, l_ul_LenTemplate);
								l_ul_FileBufferSize2 = l_ul_LenTemplate;
							}
						}//end if
						l_cs_FileID = m_BioOpPSheet.m_CodePPage.m_cs_ID + "_";
						if (i == 0)
						{ // "finger_1" (capture 1er doigt)
							l_cs_FileID += "finger_1";
						}
						else if (i == 1)
						{ // "finger_2" (capture 1er doigt)
							l_cs_FileID += "finger_2";
						}//end if						

						CFileDialog l_x_SaveFileDlg(FALSE,
							"",
							l_cs_FileID,
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
							l_cs_Tmp_FVP,
							this);

						SAV_PATH

							if (l_x_SaveFileDlg.DoModal() != IDOK)
							{
								RESTORE_PATH
									l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
								l_x_ResultDlg.m_cs_Msg2.Format("Saving FVP template file");
								l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
								l_b_Ret = FALSE;
							}
							else
							{
								RESTORE_PATH
									l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

								//- - - - - - - - - - - 
								// Write data in File
								//- - - - - - - - - - -

								CString l_cs_File;
								l_cs_File.Format("%s", l_cs_SaveFile);

								CFile l_f_PkFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
								if (i == 0) //"Finger #1" (first finger FVP template)
								{
									l_f_PkFile.Write(l_auc_FileBuffer, l_ul_FileBufferSize);
								}
								else if (i == 1)//"Finger #2" (second finger FVP template)
								{
									l_f_PkFile.Write(l_auc_FileBuffer2, l_ul_FileBufferSize2);
								}
								l_f_PkFile.Close();
								l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
								l_x_ResultDlg.m_cs_Msg3.Format("FVP Template successfully ");
								l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
								l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
							}
						l_x_ResultDlg.DoModal();
						l_x_ResultDlg.InitializeMsg();
					}
				}

				//---------------------
				// Pks File format
				//---------------------
				// the .pks file structure : 
				// - Personnal Information
				// - Nb of templates: 1 byte (1 or 2 fingers)
				// - Size 1st Tplate: 2 bytes
				// - 1st Template	: <Size 1st Tplate> bytes
				// - Size 2nd Tplate: 2 bytes					(if exists)
				// - 2nd Template	: <Size 2nd Tplate> bytes	(if exists)

				//Personnal Information
				memcpy(l_auc_FileBuffer, l_auc_PersonnalInformation, l_ul_PersonnalInformationSize);
				l_ul_FileBufferSize = l_ul_PersonnalInformationSize;

				//Nb of templates
				memcpy(l_auc_FileBuffer + l_ul_FileBufferSize, &l_uc_NbFingers, 1);
				l_ul_FileBufferSize += 1;

				// Save one or two finger templates in a same buffer
				for (i = 0; i<l_uc_NbFingers; ++i)
				{
					l_i_Ret = l_x_TemplateList.GetTemplate(
						i,					// UC	i_uc_indexTemplate,
						l_t_TypeTemplate,	// T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
						l_ul_LenTemplate,	// UL	&o_ul_lenTemplate,
						l_puc_DataTemplate  // PUC	&o_puc_dataTemplate
						);

					if (l_i_Ret != MORPHO_OK)
					{
						ShowErrorBox("An error occured while calling C_MORPHO_TemplateList::GetTemplate() function", l_i_Ret);
						PostMessage(WM_REMOVE_PROCESS_PAGE, 0, (LPARAM)&(m_BioOpPSheet.m_CodePPage));
						return FALSE;
					}

					if (_decrypt == true || isPrivacyEnabled)
					{
						bool b_decRet = _DecryptAESData(l_puc_DataTemplate, l_ul_LenTemplate, &l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
						if (!b_decRet || l_ul_Aes128CbcDeCryptedDataLen == 0)
						{
							AfxMessageBox("Decrypting data error !", MB_ICONERROR);
							return FALSE;
						}

						if (!_CheckPrivacyData(l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
						{
							AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
							if (l_puc_Aes128CbcDeCryptedData)
								MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
							return FALSE;
						}
						l_ul_Aes128CbcDeCryptedDataLen -= 8;
						// Template Size
						memcpy(l_auc_FileBuffer + l_ul_FileBufferSize, &l_ul_Aes128CbcDeCryptedDataLen, 2);
						l_ul_FileBufferSize += 2;

						//Template
						memcpy(l_auc_FileBuffer + l_ul_FileBufferSize, l_puc_Aes128CbcDeCryptedData + 8, l_ul_Aes128CbcDeCryptedDataLen);
						l_ul_FileBufferSize += l_ul_Aes128CbcDeCryptedDataLen;

						if (l_puc_Aes128CbcDeCryptedData)
							MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
					}
					else
					{
						// Template Size
						memcpy(l_auc_FileBuffer + l_ul_FileBufferSize, &l_ul_LenTemplate, 2);
						l_ul_FileBufferSize += 2;

						//Template
						memcpy(l_auc_FileBuffer + l_ul_FileBufferSize, l_puc_DataTemplate, l_ul_LenTemplate);
						l_ul_FileBufferSize += l_ul_LenTemplate;
					}
				}

				if (isPrivacyEnabled && _decrypt == false)
				{
					UC		l_puc_Aes128CbcInputVector[AES128_BLOCK_LENGTH];
					UL		l_ul_Aes128CbcCryptedBufferSize = 0;
					PUC		l_puc_Aes128CbcCryptedBuffer = NULL;
					memset(l_puc_Aes128CbcInputVector, 0, AES128_BLOCK_LENGTH);

					I l_i_ret = MSOSECU_encrypt_aes128_cbc(sl_encrypt_Key.length(), (PUC)sl_encrypt_Key.c_str(),
						l_ul_FileBufferSize, (PUC)l_auc_FileBuffer,
						l_puc_Aes128CbcInputVector, TRUE,
						&l_ul_Aes128CbcCryptedBufferSize, &l_puc_Aes128CbcCryptedBuffer);

					memcpy(l_auc_FileBuffer, l_puc_Aes128CbcCryptedBuffer, l_ul_Aes128CbcCryptedBufferSize);

					l_ul_FileBufferSize = l_ul_Aes128CbcCryptedBufferSize;

					if (l_puc_Aes128CbcCryptedBuffer)
						MSOSECU_Free(&l_puc_Aes128CbcCryptedBuffer);
				}

				l_cs_FileID = m_BioOpPSheet.m_CodePPage.m_cs_ID;
				CFileDialog l_x_SaveFileDlg(FALSE,
					"",
					l_cs_FileID,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
					l_cs_Tmp,
					this);
				SAV_PATH
					if (l_x_SaveFileDlg.DoModal() != IDOK)
					{
						RESTORE_PATH
							l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
						l_x_ResultDlg.m_cs_Msg2.Format("Saving FP template file");
						l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
						l_b_Ret = FALSE;
					}
					else
					{
						RESTORE_PATH
							l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();
						//- - - - - - - - - - - 
						// Write data in File
						//- - - - - - - - - - -

						CString l_cs_File;
						l_cs_File.Format("%s", l_cs_SaveFile);

						CFile l_f_PkFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
						l_f_PkFile.Write(l_auc_FileBuffer, l_ul_FileBufferSize);

						l_f_PkFile.Close();

						//- - - - - - - - - - - -
						l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
						l_x_ResultDlg.m_cs_Msg3.Format("FP Template successfully");
						l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
						l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
						Beep(BEEP_FREQUENCY, BEEP_DURATION);
						l_b_Ret = TRUE;
					}
				l_x_ResultDlg.DoModal();
				l_x_ResultDlg.InitializeMsg();
				CleanAsyncMessages2();
			}
			else
			{
				for (i = 0; i<l_uc_NbFingers; ++i)
				{
					//---------------------
					// FVP
					//---------------------
					if (((m_bl_FVP == TRUE) &&
						(m_BioOpPSheet.m_CodePPage.m_cs_TemplateType == STR_NO_PK_FP) &&
						(m_BioOpPSheet.m_CodePPage.m_cs_FVP_TemplateType == STR_NO_FVP)
						) ||
						((m_bl_FVP == TRUE) &&
						(m_BioOpPSheet.m_CodePPage.m_cs_FVP_TemplateType != STR_NO_FVP)
						)
						)
					{
						UC l_uc_dataIndex;
						UC l_uc_PkFpQuality;
						UC l_uc_AdvancedSecurityLevelsCompatibility;
						//l_x_TemplateList.GetNbFVPTemplate(l_uc_NbFingers);
						l_i_Ret = l_x_TemplateList.GetFVPTemplate(
							i,					// UC	i_uc_indexTemplate,
							l_t_FVPTypeTemplate,	// T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
							l_ul_LenTemplate,	// UL	&o_ul_lenTemplate,
							l_puc_DataTemplate,  // PUC	&o_puc_dataTemplate
							l_uc_PkFpQuality,
							l_uc_AdvancedSecurityLevelsCompatibility,
							l_uc_dataIndex
							);

						if (l_i_Ret != MORPHO_OK){
							ShowErrorBox("An error occured while calling C_MORPHO_TemplateList::GetNbFVPTemplate() function", l_i_Ret);
							PostMessage(WM_REMOVE_PROCESS_PAGE, 0, (LPARAM)&(m_BioOpPSheet.m_CodePPage));
							return FALSE;
						}//end if

						l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
						if (i == 0){
							l_x_ResultDlg.m_cs_Msg2.Format("Finger #1 - Quality Score: %u  ", l_uc_PkFpQuality);
						}
						else{
							l_x_ResultDlg.m_cs_Msg2.Format("Finger #2 -  Quality Score: %u  ", l_uc_PkFpQuality);
						}
						if (m_BioOpPSheet.m_CodePPage.m_i_CaptureType == 0){
							l_x_ResultDlg.m_cs_Msg3.Format("Advanced Security Levels Compatibility: %s", (l_uc_AdvancedSecurityLevelsCompatibility == 1) ? "Yes" : "No");
						}

						Beep(BEEP_FREQUENCY, BEEP_DURATION);

						l_x_ResultDlg.DoModal();
						l_x_ResultDlg.InitializeMsg();
						l_b_notify_confirmation = true;
						//MSO DEMO propose l’enregistrement des templates PK_FVP que si l’utilisateur demande l’export.
						if (m_BioOpPSheet.m_CodePPage.m_cs_FVP_TemplateType != STR_NO_FVP)
						{
							// Save first finger (Finger #1) FVP template in a buffer 
							if (i == 0){
								if (_decrypt == true)
								{
									bool b_decRet = _DecryptAESData(l_puc_DataTemplate, l_ul_LenTemplate, &l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
									if (!b_decRet || l_ul_Aes128CbcDeCryptedDataLen == 0)
									{
										AfxMessageBox("Decrypting data error !", MB_ICONERROR);
										return FALSE;
									}

									if (!_CheckPrivacyData(l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
									{
										AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
										if (l_puc_Aes128CbcDeCryptedData)
											MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
										return FALSE;
									}

									memcpy(l_auc_FileBuffer, l_puc_Aes128CbcDeCryptedData + 8, l_ul_Aes128CbcDeCryptedDataLen - 8);
									l_ul_FileBufferSize = l_ul_Aes128CbcDeCryptedDataLen - 8;

									if (l_puc_Aes128CbcDeCryptedData)
										MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
								}
								else
								{
									memcpy(l_auc_FileBuffer, l_puc_DataTemplate, l_ul_LenTemplate);
									l_ul_FileBufferSize = l_ul_LenTemplate;
								}

							}
							else if (i == 1)	{// Save second finger (Finger #2) FVP template in an other buffer 
								if (_decrypt == true)
								{
									bool b_decRet = _DecryptAESData(l_puc_DataTemplate, l_ul_LenTemplate, &l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
									if (!b_decRet || l_ul_Aes128CbcDeCryptedDataLen == 0)
									{
										AfxMessageBox("Decrypting data error !", MB_ICONERROR);
										return FALSE;
									}

									if (!_CheckPrivacyData(l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
									{
										AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
										if (l_puc_Aes128CbcDeCryptedData)
											MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
										return FALSE;
									}

									memcpy(l_auc_FileBuffer2, l_puc_Aes128CbcDeCryptedData + 8, l_ul_Aes128CbcDeCryptedDataLen - 8);
									l_ul_FileBufferSize2 = l_ul_Aes128CbcDeCryptedDataLen - 8;

									if (l_puc_Aes128CbcDeCryptedData)
										MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
								}
								else
								{
									memcpy(l_auc_FileBuffer2, l_puc_DataTemplate, l_ul_LenTemplate);
									l_ul_FileBufferSize2 = l_ul_LenTemplate;
								}
							}//end if
							l_cs_FileID = m_BioOpPSheet.m_CodePPage.m_cs_ID + "_";
							if (i == 0){ // "finger_1" (capture 1er doigt)
								l_cs_FileID += "finger_1";
							}
							else if (i == 1){ // "finger_2" (capture 1er doigt)
								l_cs_FileID += "finger_2";
							}//end if						

							CFileDialog l_x_SaveFileDlg(FALSE,
								"",
								l_cs_FileID,
								OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
								l_cs_Tmp_FVP,
								this);
							SAV_PATH
								if (l_x_SaveFileDlg.DoModal() != IDOK)
								{
									RESTORE_PATH
										l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
									l_x_ResultDlg.m_cs_Msg2.Format("Saving FVP template file");
									l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
									l_b_Ret = FALSE;
								}
								else
								{
									RESTORE_PATH
										l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

									//- - - - - - - - - - - 
									// Write data in File
									//- - - - - - - - - - -

									CString l_cs_File;
									l_cs_File.Format("%s", l_cs_SaveFile);

									CFile l_f_PkFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
									if (i == 0) //"Finger #1" (first finger FVP template)
									{
										l_f_PkFile.Write(l_auc_FileBuffer, l_ul_FileBufferSize);
									}
									else if (i == 1)//"Finger #2" (second finger FVP template)
									{
										l_f_PkFile.Write(l_auc_FileBuffer2, l_ul_FileBufferSize2);
									}
									l_f_PkFile.Close();
									l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
									l_x_ResultDlg.m_cs_Msg3.Format("FVP Template successfully ");
									l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
									l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
								}
							l_x_ResultDlg.DoModal();
							l_x_ResultDlg.InitializeMsg();
						}
					}
					l_b_Ret = TRUE;
					CleanAsyncMessages2();
					if ((m_bl_FVP == FALSE) || ((m_bl_FVP == TRUE) && (m_BioOpPSheet.m_CodePPage.m_cs_TemplateType != STR_NO_PK_FP)))
					{
						//---------------------
						// Others formats 
						//---------------------
						UC l_uc_dataIndex;
						UC l_uc_PkFpQuality;
						//l_x_TemplateList.GetNbTemplate(l_uc_NbFingers);
						l_i_Ret = l_x_TemplateList.GetTemplate(
							i,					// UC	i_uc_indexTemplate,
							l_x_TypeTemplate,	// T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
							l_ul_LenTemplate,	// UL	&o_ul_lenTemplate,
							l_puc_DataTemplate,  // PUC	&o_puc_dataTemplate
							l_uc_PkFpQuality,
							l_uc_dataIndex
							);
						if (l_i_Ret != MORPHO_OK)
						{
							ShowErrorBox("An error occured while calling C_MORPHO_TemplateList::GetTemplate() function", l_i_Ret);
							PostMessage(WM_REMOVE_PROCESS_PAGE, 0, (LPARAM)&(m_BioOpPSheet.m_CodePPage));
							return FALSE;
						}

						if (l_b_notify_confirmation == false)
						{
							l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
							if (i == 0)
							{
								l_x_ResultDlg.m_cs_Msg2.Format("Finger #1 - Quality Score: %u  ", l_uc_PkFpQuality);
							}
							else
							{
								l_x_ResultDlg.m_cs_Msg2.Format("Finger #2 -  Quality Score: %u  ", l_uc_PkFpQuality);
							}
							Beep(BEEP_FREQUENCY, BEEP_DURATION);
							l_x_ResultDlg.DoModal();
							l_x_ResultDlg.InitializeMsg();
						}
						if ((m_BioOpPSheet.m_CodePPage.m_cs_TemplateType != STR_NO_PK_FP)) //MSO DEMO propose l’enregistrement des templates PK_COMP que si l’utilisateur demande l’export.
						{
							// Save first finger (Finger #1) template in a buffer 
							if (i == 0)
							{
								if (_decrypt == true)
								{
									bool b_decRet = _DecryptAESData(l_puc_DataTemplate, l_ul_LenTemplate, &l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
									if (!b_decRet || l_ul_Aes128CbcDeCryptedDataLen == 0)
									{
										AfxMessageBox("Decrypting data error !", MB_ICONERROR);
										return FALSE;
									}

									if (!_CheckPrivacyData(l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
									{
										AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
										if (l_puc_Aes128CbcDeCryptedData)
											MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
										return FALSE;
									}

									memcpy(l_auc_FileBuffer, l_puc_Aes128CbcDeCryptedData + 8, l_ul_Aes128CbcDeCryptedDataLen - 8);
									l_ul_FileBufferSize = l_ul_Aes128CbcDeCryptedDataLen - 8;

									if (l_puc_Aes128CbcDeCryptedData)
										MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
								}
								else
								{
									memcpy(l_auc_FileBuffer, l_puc_DataTemplate, l_ul_LenTemplate);
									l_ul_FileBufferSize = l_ul_LenTemplate;
								}

							}
							// Save second finger (Finger #2) template in an other buffer 
							else if (i == 1)
							{
								if (_decrypt == true)
								{
									bool b_decRet = _DecryptAESData(l_puc_DataTemplate, l_ul_LenTemplate, &l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
									if (!b_decRet || l_ul_Aes128CbcDeCryptedDataLen == 0)
									{
										AfxMessageBox("Decrypting data error !", MB_ICONERROR);
										return FALSE;
									}

									if (!_CheckPrivacyData(l_puc_Aes128CbcDeCryptedData, l_ul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
									{
										AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
										if (l_puc_Aes128CbcDeCryptedData)
											MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
										return FALSE;
									}

									memcpy(l_auc_FileBuffer2, l_puc_Aes128CbcDeCryptedData + 8, l_ul_Aes128CbcDeCryptedDataLen - 8);
									l_ul_FileBufferSize2 = l_ul_Aes128CbcDeCryptedDataLen - 8;

									if (l_puc_Aes128CbcDeCryptedData)
										MSOSECU_Free(&l_puc_Aes128CbcDeCryptedData);
								}
								else
								{
									memcpy(l_auc_FileBuffer2, l_puc_DataTemplate, l_ul_LenTemplate);
									l_ul_FileBufferSize2 = l_ul_LenTemplate;
								}
							}

							l_cs_FileID = m_BioOpPSheet.m_CodePPage.m_cs_ID + "_";
							if (i == 0) // "finger_1" (capture 1er doigt)
							{
								l_cs_FileID += "finger_1";
							}
							else if (i == 1) // "finger_2" (capture 1er doigt)
							{
								l_cs_FileID += "finger_2";
							}

							if ((l_x_TypeTemplate == MORPHO_PK_DIN_V66400_CS_AA) || (l_x_TypeTemplate == MORPHO_PK_ISO_FMC_CS_AA)) // Pks ordered by ascending angle
							{
								l_cs_FileID += "_aa";
							}

							CFileDialog l_x_SaveFileDlg(FALSE,
								"",
								l_cs_FileID,
								OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
								l_cs_Tmp,
								this);
							SAV_PATH
								if (l_x_SaveFileDlg.DoModal() != IDOK)
								{
									RESTORE_PATH
										l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
									l_x_ResultDlg.m_cs_Msg2.Format("Saving FP template file");
									l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
									l_b_Ret = FALSE;
								}
								else
								{
									RESTORE_PATH
										l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

									//- - - - - - - - - - - 
									// Write data in File
									//- - - - - - - - - - -

									CString l_cs_File;
									l_cs_File.Format("%s", l_cs_SaveFile);

									CFile l_f_PkFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

									if (i == 0) //"Finger #1" (first finger template)
									{
										l_f_PkFile.Write(l_auc_FileBuffer, l_ul_FileBufferSize);
									}
									else if (i == 1)//"Finger #2" (second finger template)
									{
										l_f_PkFile.Write(l_auc_FileBuffer2, l_ul_FileBufferSize2);
									}

									l_f_PkFile.Close();

									//- - - - - - - - - - - -
									l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
									l_x_ResultDlg.m_cs_Msg3.Format("FP Template successfully");
									l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
									l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
								}
							l_x_ResultDlg.DoModal();
							l_x_ResultDlg.InitializeMsg();
						}
						l_b_Ret = TRUE;
						CleanAsyncMessages2();
					}
					//--------------------------
					// END Other formats section 
					//--------------------------
				}//end for
			}
		}
		else
		{
			//---------------------
			// Write in Smart Card 
			//---------------------
			T_USER_DATA		l_x_UserData;

			l_x_UserData.m_uc_NbFingers = m_BioOpPSheet.m_CodePPage.m_i_NbFingers;
			//l_x_UserData.m_uc_NbFingers = 0 ou 1 (means 1 or 2 fingers)
			l_x_UserData.m_px_Buffer = (PT_BUFFER)malloc((l_x_UserData.m_uc_NbFingers + 1) * sizeof(T_BUFFER));

			for (i = 0; i <= l_x_UserData.m_uc_NbFingers; ++i)
			{
				l_i_Ret = l_x_TemplateList.GetTemplate(
					i,					// UC	i_uc_indexTemplate,
					l_t_TypeTemplate,	// T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
					l_ul_LenTemplate,	// UL	&o_ul_lenTemplate,
					l_puc_DataTemplate  // PUC	&o_puc_dataTemplate
					);

				if (l_i_Ret != MORPHO_OK)
				{
					l_cs_Message.Format("An error occured while calling C_MORPHO_TemplateList::GetTemplate() function (Error %d).", l_i_Ret);
					AfxMessageBox(l_cs_Message);
					EnableButtons();
					InitQualityBar();
					InitGuidingAnimations();
					PostMessage(WM_REMOVE_PROCESS_PAGE, 0, (LPARAM)&(m_BioOpPSheet.m_CodePPage));
					m_b_BiometricOperationProcessing = FALSE;
					return FALSE;
				}

				l_x_UserData.m_px_Buffer[i].m_us_Size = (S)l_ul_LenTemplate;
				l_x_UserData.m_px_Buffer[i].m_puc_Buffer = (PUC)malloc(l_ul_LenTemplate);

				memcpy(l_x_UserData.m_px_Buffer[i].m_puc_Buffer, l_puc_DataTemplate, l_ul_LenTemplate);
			}


			l_x_UserData.m_cs_ID = m_BioOpPSheet.m_CodePPage.m_cs_ID;
			l_x_UserData.m_cs_Firstname = m_BioOpPSheet.m_CodePPage.m_cs_Firstname;
			l_x_UserData.m_cs_Lastname = m_BioOpPSheet.m_CodePPage.m_cs_Lastname;

			//------------------------------------------------------------
			// Save Data
			//------------------------------------------------------------

			//- - - - - - - - - 
			// Open File
			//- - - - - - - - - 

			GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

			if (m_BioOpPSheet.m_CodePPage.m_cs_WorkflowType == STR_WORKFLOWTYPE_SAGEM_OPUCE)
			{
				l_px_BioDemoCard = new CSagemBioDemoCard;
				l_i_Ret = OpenSmartCardAndInsertData(&l_x_UserData, l_px_BioDemoCard, FALSE);
			}
			else if (m_BioOpPSheet.m_CodePPage.m_cs_WorkflowType == STR_WORKFLOWTYPE_CLIENT_GB)
			{
				l_px_BioDemoCard = new CGB_BioDemoCard;
				l_i_Ret = OpenSmartCardAndInsertData(&l_x_UserData, l_px_BioDemoCard, TRUE);
			}
			else
			{
				AfxMessageBox("Choose a Card Workflow Type valid");
				l_i_Ret = MORPHOERR_BADPARAMETER;
			}

			delete l_px_BioDemoCard;

			if (l_i_Ret != SMARTCARD_OK)
			{
				l_x_ResultDlg.m_cs_Msg1.Format("Capture Failed");
				l_x_ResultDlg.m_cs_Msg2.Format("Error while exported data in smart card");
				l_x_ResultDlg.m_cs_Msg3.Format("Error 0x%08X", l_i_Ret);
				l_b_Ret = FALSE;
				switch (l_i_Ret)
				{
				case  SCARD_W_REMOVED_CARD:
					l_x_ResultDlg.m_cs_Msg4.Format("Smart card removed");
					break;
				case SCARD_W_UNRESPONSIVE_CARD:
					l_x_ResultDlg.m_cs_Msg4.Format("Smart card not responding to a reset");
					break;
				case 0x8010002F: //SCARD_E_COMM_DATA_LOST:in scarerr.h
					l_x_ResultDlg.m_cs_Msg4.Format("COM error with smart card detected");
					break;
				case 0x8010002e://SCARD_E_NO_READERS_AVAILABLE
					l_x_ResultDlg.m_cs_Msg4.Format("Cannot find a smart card reader");
					break;
				default:
					l_x_ResultDlg.m_cs_Msg3.Format("Error 0x%08X. Check that", l_i_Ret);
					l_x_ResultDlg.m_cs_Msg4.Format("the card is compatible with the demo", l_i_Ret);
					break;
				}
				//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);

			}
			else
			{
				l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
				l_x_ResultDlg.m_cs_Msg2.Format("Template successfully ");
				l_x_ResultDlg.m_cs_Msg3.Format("exported in smart card");
				Beep(BEEP_FREQUENCY, BEEP_DURATION);
				l_b_Ret = TRUE;
			}
			l_x_ResultDlg.DoModal();
			l_x_ResultDlg.InitializeMsg();
			CleanAsyncMessages2();
			for (i = 0; i <= l_x_UserData.m_uc_NbFingers; ++i)
			{
				free(l_x_UserData.m_px_Buffer[i].m_puc_Buffer);
			}
			free(l_x_UserData.m_px_Buffer);
		}
	}


	m_b_BiometricOperationProcessing = FALSE;


	if (m_px_Image != NULL)
	{
		delete(m_px_Image);
		m_px_Image = NULL;
	}

	if (m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
	{
		PostMessage(WM_REMOVE_PROCESS_PAGE, 0, (LPARAM)&(m_BioOpPSheet.m_CodePPage));
	}

	PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
	RedrawWnd();
	return l_b_Ret;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::_GetImage()
{
	UL				            l_ul_CallbackMask;
	I				            l_i_Ret;
	T_MORPHO_IMAGE	            l_x_Image;
	T_MORPHO_COMPRESS_ALGO		l_x_CompressAlgo;
	CResultDlg		            l_x_ResultDlg;
	bool			            l_b_Ret;
	UL				            l_ul_DetectModeChoice;
	UC				            l_uc_DetectModeChoice;
	UC				            l_uc_LatentDetection;
	CString			            l_cs_Message;
	T_MORPHO_COMPRESS_ALGO		l_x_DesiredISO19794_FIR_CompressAlgo;
	CString						l_cs_ISO19794_FIR_ImageExtension;
	I							l_i_FingerQuality;
	CString						l_cs_FingerQuality;

	bool _decrypt = false;
	PUC o_ppuc_Aes128CbcDeCryptedData = NULL;
	UL o_pul_Aes128CbcDeCryptedDataLen = 0;
	std::string sl_encrypt_Key;
	BOOL					isPrivacyEnabled = (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED || m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED);


	// check if "Force Finger Placement on Top" is désactivated
	if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_ForceFingerPlacement && m_BioOpPSheet.m_GetImagePPage.m_i_DetectModeChoice == 0)
	{
		AfxMessageBox("GetImage en \"Verify detection mode\"  and \"Force Finger Placement on Top\" option cannot be used together.\nUncheck this option before proceeding ...");
		return true;
	}

	//------------------------------------------------------------
	// Does not allow other operations
	//------------------------------------------------------------

	DisableButtons();

	//------------------------------------------------------------
	// Viewer or not viewer.... 
	//------------------------------------------------------------

	l_ul_CallbackMask = m_ul_CallbackMask;
	l_ul_CallbackMask &= ~MORPHO_CALLBACK_ENROLLMENT_CMD;

	if (m_tbCtrl.m_TabOptionsDlg->m_b_MainViewerActive)
	{
		m_i_CurrentViewerIndex = 0;
		m_px_Image = new C_MORPHO_Image(1);
	}

	//------------------------------------------------------------
	// Get Finger Quality 
	//------------------------------------------------------------
	l_i_FingerQuality = 0;

	if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_FingerPrint_Quality)
	{
		m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_EDIT_FINGERPRINT_QUALITY)->GetWindowText(l_cs_FingerQuality);
		l_i_FingerQuality = atoi(l_cs_FingerQuality);
	}

	//-----------------------------------------------------------
	// Get security Level Added by M.EL KHACHBI
	//-----------------------------------------------------------
	//m_x_MorphoSmart.Set_FFD_SecurityLevel();
	GetSecurityLevel(1);
	m_x_MorphoSmart.Set_FFD_SecurityLevel(m_i_SecurityLevel);
	//------------------------------------------------------------
	// get compression type
	//------------------------------------------------------------
	//solution provisoire
	if (strcmp(m_BioOpPSheet.m_GetImagePPage.m_str_CompressionType, "WSQ") != 0){
		m_BioOpPSheet.m_GetImagePPage.m_i_CompressionRate = 0;
	}
	if (m_BioOpPSheet.m_GetImagePPage.m_i_CompressionType == 0)
	{
		l_x_CompressAlgo = MORPHO_NO_COMPRESS;
	}
	else if (m_BioOpPSheet.m_GetImagePPage.m_i_CompressionType == 1)
	{
		l_x_CompressAlgo = MORPHO_COMPRESS_V1;
	}
	else if (m_BioOpPSheet.m_GetImagePPage.m_i_CompressionType == 2)
	{

		l_x_CompressAlgo = MORPHO_COMPRESS_WSQ;
		if ((m_BioOpPSheet.m_GetImagePPage.m_i_CompressionRate < 2) ||
			(m_BioOpPSheet.m_GetImagePPage.m_i_CompressionRate > 255))
		{
			AfxMessageBox("bad Compression Rate parameter");
			EnableButtons();
			InitQualityBar();
			InitGuidingAnimations();
			RedrawWnd();
			return true;
		}

	}
	else if (m_BioOpPSheet.m_GetImagePPage.m_i_CompressionType == 3)
	{
		l_x_CompressAlgo = MORPHO_NO_COMPRESS;
		l_x_DesiredISO19794_FIR_CompressAlgo = MORPHO_COMPRESS_UNCHANGED;//because we have it already in RAW
		l_cs_ISO19794_FIR_ImageExtension = "RAW";
	}
	else if (m_BioOpPSheet.m_GetImagePPage.m_i_CompressionType == 4)
	{
		l_x_CompressAlgo = MORPHO_NO_COMPRESS;
		l_x_DesiredISO19794_FIR_CompressAlgo = MORPHO_COMPRESS_BMP;
		l_cs_ISO19794_FIR_ImageExtension = "BMP";
	}
	else if (m_BioOpPSheet.m_GetImagePPage.m_i_CompressionType == 5)
	{
		l_x_CompressAlgo = MORPHO_NO_COMPRESS;
		l_x_DesiredISO19794_FIR_CompressAlgo = MORPHO_COMPRESS_JPEG2000;
		l_cs_ISO19794_FIR_ImageExtension = "JP2";
	}
	else if (m_BioOpPSheet.m_GetImagePPage.m_i_CompressionType == 6)
	{
		l_x_CompressAlgo = MORPHO_NO_COMPRESS;
		l_x_DesiredISO19794_FIR_CompressAlgo = MORPHO_COMPRESS_JPEG;
		l_cs_ISO19794_FIR_ImageExtension = "JPG";
	}
	else if (m_BioOpPSheet.m_GetImagePPage.m_i_CompressionType == 7)
	{
		l_x_CompressAlgo = MORPHO_NO_COMPRESS;
		l_x_DesiredISO19794_FIR_CompressAlgo = MORPHO_COMPRESS_PNG;
		l_cs_ISO19794_FIR_ImageExtension = "PNG";
	}
	else if (m_BioOpPSheet.m_GetImagePPage.m_i_CompressionType == 8)
	{
		l_x_CompressAlgo = MORPHO_NO_COMPRESS;
		l_x_DesiredISO19794_FIR_CompressAlgo = MORPHO_COMPRESS_WSQ;
		l_cs_ISO19794_FIR_ImageExtension = "WSQ";
	}
	else
	{
		l_x_CompressAlgo = MORPHO_NO_COMPRESS;
	}
	//------------------------------------------------------------
	// Get Image
	//------------------------------------------------------------

	if (m_BioOpPSheet.m_GetImagePPage.m_i_DetectModeChoice == 0)
	{
		l_ul_DetectModeChoice = MORPHO_VERIF_DETECT_MODE;
	}
	else if (m_BioOpPSheet.m_GetImagePPage.m_i_DetectModeChoice == 1)
	{
		l_ul_DetectModeChoice = MORPHO_ENROLL_DETECT_MODE;
	}
	else
	{
		l_ul_DetectModeChoice = MORPHO_ENROLL_DETECT_MODE;
	}

	if (m_BioOpPSheet.m_GetImagePPage.m_b_GetImageLatentDetect == 0)
	{
		l_uc_LatentDetection = LATENT_DETECT_DISABLE;
	}
	else
	{
		l_uc_LatentDetection = LATENT_DETECT_ENABLE;
	}

	if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_ForceFingerPlacement != 0)
	{
		l_ul_DetectModeChoice |= MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE;
	}

	l_ul_DetectModeChoice = l_ul_DetectModeChoice | m_ul_WakeUpMode;

	if (l_ul_DetectModeChoice > 0xFF)
	{
		l_cs_Message.Format("DetectModeChoice overfloat 0x%08x>0xFF(UC), information lost.", l_ul_DetectModeChoice);
		AfxMessageBox(l_cs_Message, MB_ICONERROR);
	}

	l_uc_DetectModeChoice = (UC)l_ul_DetectModeChoice;

	m_b_BiometricOperationProcessing = TRUE;
	l_i_Ret = m_x_MorphoSmart.GetImage(
		m_tbCtrl.m_TabBioSettingsDlg->m_i_Timeout,				// US							i_us_Timeout, 
		(UC)l_i_FingerQuality,									// UC							i_uc_AcquisitionThreshold,
		l_x_CompressAlgo,										// T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
		(UC)m_BioOpPSheet.m_GetImagePPage.m_i_CompressionRate,  // UC							i_uc_CompressRate,
		l_ul_CallbackMask,										// UL							i_ul_CallbackCmd,
		BiometricCallback,										// T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback, 
		this,													// PVOID						i_pv_CallbackArgument,
		&l_x_Image,												//T_MORPHO_IMAGE				*o_px_Image
		(UC)(l_uc_DetectModeChoice | m_ul_WakeUpMode),			//UC							i_uc_DetectModeChoice
		l_uc_LatentDetection
		);

	m_b_BiometricOperationProcessing = FALSE;
	CleanAsyncMessages();

	//------------------------------------------------
	// Remove handle on the Miniviewer 
	m_ctl_CurrentMiniViewer = NULL;
	//------------------------------------------------

	if (l_i_Ret == MORPHOERR_DEVICE_LOCKED)
	{
		I l_i_Ret2 = UnlockDevice();
		if (l_i_Ret2 != MORPHO_OK)
		{
			ShowErrorBox("An error occured during Device Unlocking", l_i_Ret2);
		}
		l_b_Ret = FALSE;
	}
	else if (l_i_Ret != MORPHO_OK)
	{
		ShowAnimation("image_Ko");
		switch (l_i_Ret)
		{
		case MORPHOERR_NO_HIT:
			l_x_ResultDlg.m_cs_Msg2.Format("Bad GetImage Sequence.");
			break;
		case MORPHOERR_CMDE_ABORTED:
			l_x_ResultDlg.m_cs_Msg2.Format("Command aborted by user.");
			break;
		case MORPHOERR_TIMEOUT:
			l_x_ResultDlg.m_cs_Msg2.Format("Timeout has expired.");
			l_x_ResultDlg.m_cs_Msg3.Format("Command aborted.");
			break;
		case MORPHOERR_FFD:
		case MORPHOERR_MOIST_FINGER:
		case MORPHOERR_MOVED_FINGER:
		case MORPHOERR_SATURATED_FINGER:
		case MORPHOERR_INVALID_FINGER:
			l_x_ResultDlg.m_cs_Msg2.Format("Warning : Low quality finger !!!");
			break;
		case MORPHOERR_LICENSE_MISSING:
			l_x_ResultDlg.m_cs_Msg2.Format("A required license is missing.");
			break;
		default:
			l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
			l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Device::GetImage() function");
			ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
		}

		l_x_ResultDlg.m_cs_Msg1.Format("GetImage Failed");
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_b_Ret = FALSE;
	}
	//	else
	if ((l_i_Ret == MORPHO_OK)
		|| (l_i_Ret == MORPHOERR_FFD)
		|| (l_i_Ret == MORPHOERR_MOIST_FINGER)
		|| (l_i_Ret == MORPHOERR_MOVED_FINGER)
		|| (l_i_Ret == MORPHOERR_SATURATED_FINGER)
		|| (l_i_Ret == MORPHOERR_INVALID_FINGER))
	{
		ShowAnimation("image_Ok");
		
		if (isPrivacyEnabled)
		{
			if (AfxMessageBox("Privacy Mode : Image data is crypted !\nWould you like to decrypt it before saving ?", MB_YESNO) == IDYES)
			{
				if (GetEncryptedKey(sl_encrypt_Key))
					_decrypt = true;
				
				bool b_decRet = _DecryptAESData(l_x_Image.m_puc_Image, l_x_Image.m_ul_imageSize, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);

				if (!b_decRet || o_pul_Aes128CbcDeCryptedDataLen == 0)
				{
					AfxMessageBox("Decrypting image data error !", MB_ICONERROR);
					return FALSE;
				}

				if (!_CheckPrivacyData(o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key))
				{
					AfxMessageBox("Invalid CRC32 !", MB_ICONERROR);
					if (o_ppuc_Aes128CbcDeCryptedData) 
						MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
					return FALSE;
				}

				l_x_Image.m_puc_Image = o_ppuc_Aes128CbcDeCryptedData + 8;
				l_x_Image.m_ul_imageSize = o_pul_Aes128CbcDeCryptedDataLen - 8;

				UL m_ul_RawImageAndHeaderSize;
				PUC m_puc_RawImageAndHeader = NULL;

				UL l_i_Ret2 = COMPRESS_GetRawImageSize(
					l_x_Image.m_puc_Image,			//PUC	i_puc_CompressedImageWithHeader,
					l_x_Image.m_ul_imageSize,		//UL	i_ul_CompressedImageWithHeaderSize,
					&(m_ul_RawImageAndHeaderSize)	//PUL	o_pul_UnCompressedImageWithHeaderSize
					);

				if (l_x_CompressAlgo == MORPHO_NO_COMPRESS)
				{
					m_puc_RawImageAndHeader = (PUC)malloc(m_ul_RawImageAndHeaderSize);
					memcpy(m_puc_RawImageAndHeader, l_x_Image.m_puc_Image, m_ul_RawImageAndHeaderSize);

					memcpy(&l_x_Image.m_x_ImageHeader, m_puc_RawImageAndHeader, sizeof(T_MORPHO_IMAGE_HEADER));
					l_x_Image.m_puc_Image = m_puc_RawImageAndHeader + sizeof(T_MORPHO_IMAGE_HEADER);
				}
				else if (l_x_CompressAlgo == MORPHO_COMPRESS_V1)
				{
					m_puc_RawImageAndHeader = (PUC)malloc(m_ul_RawImageAndHeaderSize);

					l_i_Ret2 = COMPRESS_UnCompressImage(
						l_x_Image.m_puc_Image,			//PUC	i_puc_CompressedImageWithHeader,
						l_x_Image.m_ul_imageSize,		//UL	i_ul_CompressedImageWithHeaderSize,
						m_puc_RawImageAndHeader,		//PUC	o_puc_RawImageWithHeader,
						&(m_ul_RawImageAndHeaderSize)	//PUL	io_pul_RawImageWithHeaderSize
						);

					if (l_i_Ret2 == MORPHO_OK)
					{
						memcpy(&l_x_Image.m_x_ImageHeader, m_puc_RawImageAndHeader, sizeof(T_MORPHO_IMAGE_HEADER));
						l_x_Image.m_puc_Image = m_puc_RawImageAndHeader + sizeof(T_MORPHO_IMAGE_HEADER);
						l_x_Image.m_puc_CompressedImage = l_x_Image.m_puc_Image;
					}
				}
				else
				{
					memcpy(&l_x_Image.m_x_ImageHeader, l_x_Image.m_puc_Image, sizeof(T_MORPHO_IMAGE_HEADER));
					l_x_Image.m_puc_CompressedImage = l_x_Image.m_puc_Image + sizeof(T_MORPHO_IMAGE_HEADER);

					memcpy(&l_x_Image.m_x_ImageWSQHeader, l_x_Image.m_puc_CompressedImage, sizeof(T_MORPHO_IMAGE_WSQ_HEADER));
					l_x_Image.m_puc_CompressedImage = l_x_Image.m_puc_CompressedImage + sizeof(T_MORPHO_IMAGE_WSQ_HEADER);
				}
			}
		}

		//------------------------------------------------------------
		// Display Data
		//------------------------------------------------------------
		if (l_x_CompressAlgo != MORPHO_COMPRESS_WSQ)
		{
			CDisplayImageDlg l_x_DisplayImageDlg(&l_x_Image);
			l_x_DisplayImageDlg.DoModal();
		}

		//------------------------------------------------------------
		// Save Data
		//------------------------------------------------------------

		//- - - - - - - - - 
		// Open File
		//- - - - - - - - - 
		CString		l_cs_SaveFile;

		// add code de choix de type de fichier
		if (m_BioOpPSheet.m_GetImagePPage.m_i_CompressionType == 0)//RAW
		{
			CString extension_title, default_extension, extension_list;
			
			if (isPrivacyEnabled && _decrypt == false)
			{
				extension_title = "Binary";
				default_extension = "*.raw";
				extension_list = "Raw Files (*.raw)|*.raw|All Files (*.*)|*.*||";
			}
			else
			{
				extension_title = "Image";
				default_extension = "*.bmp";
				extension_list = "Bmp Files (*.bmp)|*.bmp|Raw Files (*.raw)|*.raw|All Files (*.*)|*.*||";
			}

			CFileDialog l_x_SaveFileDlg(FALSE,
				default_extension,
				extension_title,
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
				extension_list,
				this);

			SAV_PATH
				if (l_x_SaveFileDlg.DoModal() != IDOK)
				{
					RESTORE_PATH
						l_x_ResultDlg.m_cs_Msg1.Format("GetImage Successful");
					l_x_ResultDlg.m_cs_Msg2.Format("Saving image file");
					l_x_ResultDlg.m_cs_Msg3.Format("aborted by user.");
					if (l_i_Ret != MORPHO_OK)
					{
						l_x_ResultDlg.m_cs_Msg4.Format("Saving low quality finger failed.");
					}
					l_b_Ret = FALSE;
				}
				else
				{
					RESTORE_PATH
						l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

					CString l_cs_CompressedFileExt = l_x_SaveFileDlg.GetFileExt();

					//- - - - - - - - - - - 
					// Write data in File
					//- - - - - - - - - - -
					if (l_cs_CompressedFileExt == "bmp")
					{
						ConvertRawToBmp l__ConvertRawToBmp;
						CString l_cs_File;
						l_cs_File.Format("%s", l_cs_SaveFile);
						l__ConvertRawToBmp.SaveImageBMP(l_cs_File, l_x_Image.m_puc_Image, l_x_Image.m_x_ImageHeader.m_us_NbRow, l_x_Image.m_x_ImageHeader.m_us_NbCol, l_x_Image.m_x_ImageHeader.m_us_ResX, l_x_Image.m_x_ImageHeader.m_us_ResY);
					}
					else
					{
						CString l_cs_File;
						l_cs_File.Format("%s", l_cs_SaveFile);

						CFile l_f_RawFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
						if (isPrivacyEnabled && _decrypt == false)
						{
							l_f_RawFile.Write(l_x_Image.m_puc_Image, l_x_Image.m_ul_imageSize);
						}
						else
						{
							l_f_RawFile.Write(l_x_Image.m_puc_Image, l_x_Image.m_x_ImageHeader.m_us_NbRow*l_x_Image.m_x_ImageHeader.m_us_NbCol);
						}

						l_f_RawFile.Close();
					}


					//- - - - - - - - - - - -

					l_x_ResultDlg.m_cs_Msg1.Format("GetImage Successful");
					l_x_ResultDlg.m_cs_Msg3.Format("Image successfully ");
					l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
					l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
					if (l_i_Ret != MORPHO_OK)
					{
						l_x_ResultDlg.m_cs_Msg4.Format("Low quality finger saved.");
					}

					Beep(BEEP_FREQUENCY, BEEP_DURATION);
					l_b_Ret = TRUE;
				}
		}
		else if (m_BioOpPSheet.m_GetImagePPage.m_i_CompressionType == 1) //sagem_v1
		{
			CFileDialog l_x_SaveFileDlg(FALSE,
				"*.bin",
				"Image.bin",
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
				"Bin Files (*.bin)|*.bin|All Files (*.*)|*.*||",
				this);
			SAV_PATH
				if (l_x_SaveFileDlg.DoModal() != IDOK)
				{
					RESTORE_PATH
						l_x_ResultDlg.m_cs_Msg1.Format("GetImage Successful");
					l_x_ResultDlg.m_cs_Msg2.Format("Saving image file");
					l_x_ResultDlg.m_cs_Msg2.Format("aborted by user.");
					if (l_i_Ret != MORPHO_OK)
					{
						l_x_ResultDlg.m_cs_Msg3.Format("Saving low quality finger failed.");
					}

					l_b_Ret = FALSE;
				}
				else
				{
					RESTORE_PATH
						l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

					//- - - - - - - - - - - 
					// Write data in File
					//- - - - - - - - - - -

					CString l_cs_File;
					l_cs_File.Format("%s", l_cs_SaveFile);

					CFile l_f_BinFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);


					l_f_BinFile.Write(l_x_Image.m_puc_CompressedImage, l_x_Image.m_ul_imageSize);


					l_f_BinFile.Close();

					//- - - - - - - - - - - -

					l_x_ResultDlg.m_cs_Msg1.Format("GetImage Successful");
					l_x_ResultDlg.m_cs_Msg3.Format("Image successfully");
					l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
					l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
					if (l_i_Ret != MORPHO_OK)
					{
						l_x_ResultDlg.m_cs_Msg4.Format("Low quality finger saved.");
					}

					Beep(BEEP_FREQUENCY, BEEP_DURATION);
					l_b_Ret = TRUE;
				}

		}
		else if (m_BioOpPSheet.m_GetImagePPage.m_i_CompressionType == 2) //WSQ
		{
			CFileDialog l_x_SaveFileDlg(FALSE,
				"*.wsq",
				"Image.wsq",
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
				"Wsq Files (*.wsq)|*.wsq|All Files (*.*)|*.*||",
				this);
			SAV_PATH
				if (l_x_SaveFileDlg.DoModal() != IDOK)
				{
					RESTORE_PATH
						l_x_ResultDlg.m_cs_Msg1.Format("GetImage Successful");
					l_x_ResultDlg.m_cs_Msg2.Format("Saving image file");
					l_x_ResultDlg.m_cs_Msg2.Format("aborted by user.");
					if (l_i_Ret != MORPHO_OK)
					{
						l_x_ResultDlg.m_cs_Msg3.Format("Saving low quality finger failed.");
					}
					l_b_Ret = FALSE;
				}
				else
				{
					RESTORE_PATH
						l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

					//- - - - - - - - - - - 
					// Write data in File
					//- - - - - - - - - - -

					CString l_cs_File;
					l_cs_File.Format("%s", l_cs_SaveFile);

					CFile l_f_RawFile(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);

					l_f_RawFile.Write(l_x_Image.m_puc_CompressedImage, l_x_Image.m_x_ImageWSQHeader.m_ul_WSQImageSize);

					l_f_RawFile.Close();

					//- - - - - - - - - - - -

					l_x_ResultDlg.m_cs_Msg1.Format("GetImage Successful");
					l_x_ResultDlg.m_cs_Msg3.Format("Image successfully");
					l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
					l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
					if (l_i_Ret != MORPHO_OK)
					{
						l_x_ResultDlg.m_cs_Msg4.Format("Low quality finger saved.");
					}

					Beep(BEEP_FREQUENCY, BEEP_DURATION);
					l_b_Ret = TRUE;
				}
		}
		else //ISO-19794-4 FIR: RAW JPG JP2 PNG BMP WSQ
		{
			CString l_cs_FileName = "ISO19794_FIR_Image" + l_cs_ISO19794_FIR_ImageExtension;
			CFileDialog l_x_SaveFileDlg(FALSE,
				"*.iso-fir",
				l_cs_FileName,
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
				"ISO-19794-4 FIR Files (*.iso-fir)|*.iso-fir||",
				this);
			SAV_PATH
				if (l_x_SaveFileDlg.DoModal() != IDOK)
				{
					RESTORE_PATH
						l_x_ResultDlg.m_cs_Msg1.Format("GetImage Successful");
					l_x_ResultDlg.m_cs_Msg2.Format("Saving image file");
					l_x_ResultDlg.m_cs_Msg2.Format("aborted by user.");
					if (l_i_Ret != MORPHO_OK)
					{
						l_x_ResultDlg.m_cs_Msg3.Format("Saving low quality finger failed.");
					}
					l_b_Ret = FALSE;
				}
				else
				{
					RESTORE_PATH
						l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

					//- - - - - - - - - - - 
					// Write data in File
					//- - - - - - - - - - -

					// ----- conversion ISO-19794-4 FIR ------ //

					PT_MORPHO_IMAGE_INFO l_px_ImagesInfo = new T_MORPHO_IMAGE_INFO;

					l_px_ImagesInfo->m_uc_FingerImageQuality = 0;
					l_px_ImagesInfo->m_uc_FingerPosition = 0;
					l_px_ImagesInfo->m_uc_ImpressionType = 0;

					T_MORPHO_IMAGE * l_px_Image = new T_MORPHO_IMAGE;

					UL l_ul_imageSize = l_x_Image.m_x_ImageHeader.m_us_NbRow * l_x_Image.m_x_ImageHeader.m_us_NbCol;
					l_px_Image->m_puc_Image = new UC[l_ul_imageSize];
					memcpy(l_px_Image->m_puc_Image, l_x_Image.m_puc_Image, l_ul_imageSize);
					l_px_Image->m_ul_imageSize = l_x_Image.m_ul_imageSize;
					l_px_Image->m_x_ImageHeader = l_x_Image.m_x_ImageHeader;
					l_px_Image->m_x_ImageWSQHeader = l_x_Image.m_x_ImageWSQHeader;

					PT_MORPHO_ISO19794_FIR	l_px_ISO19794_FIR = new T_MORPHO_ISO19794_FIR;

					// Call CreateISO19794_FIR()
					l_i_Ret = C_MORPHO_Utils::CreateISO19794_FIR(&l_px_Image, &l_px_ImagesInfo, 1, l_px_ISO19794_FIR);

					if (l_i_Ret != MORPHO_OK)
					{
						l_x_ResultDlg.m_cs_Msg1.Format("CreateISO19794_FIR Failed");
						l_b_Ret = FALSE;
					}
					else
					{
						// Get file Path
						l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();
						CString l_cs_File;
						l_cs_File.Format("%s", l_cs_SaveFile);

						// Call WriteISO19794_FIR
						l_i_Ret = C_MORPHO_Utils::WriteISO19794_FIR(l_px_ISO19794_FIR, l_x_DesiredISO19794_FIR_CompressAlgo,
							l_cs_File.GetBuffer(l_cs_File.GetLength()));
						if (l_i_Ret != MORPHO_OK && l_i_Ret != MORPHOWARNING_WSQ_COMPRESSION_RATIO)
						{
							l_x_ResultDlg.m_cs_Msg1.Format("WriteISO19794_FIR Failed");
							l_b_Ret = FALSE;
						}
						else
						{
							if (l_i_Ret == MORPHOWARNING_WSQ_COMPRESSION_RATIO)
							{
								l_x_ResultDlg.m_cs_Msg2.Format("MORPHOWARNING_WSQ_COMPRESSION_RATIO");
							}
							l_x_ResultDlg.m_cs_Msg1.Format("GetImage Successful");
							l_x_ResultDlg.m_cs_Msg3.Format("Image successfully");
							l_x_ResultDlg.m_cs_Msg5.Format("exported in file");
							l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
							Beep(BEEP_FREQUENCY, BEEP_DURATION);
							l_b_Ret = TRUE;
						}
					}

					// free memory
					if (l_px_Image)
					{
						delete[] l_px_Image->m_puc_Image;
						l_px_Image->m_puc_Image = NULL;
						delete l_px_Image;
						l_px_Image = NULL;
					}

					if (l_px_ImagesInfo)
					{
						delete l_px_ImagesInfo;
						l_px_ImagesInfo = NULL;
					}

					// free l_px_ISO19794_FIR
					C_MORPHO_Utils::ReleaseISO19794_FIR(l_px_ISO19794_FIR);

					if (l_px_ISO19794_FIR)
					{
						delete l_px_ISO19794_FIR;
						l_px_ISO19794_FIR = NULL;
					}

				}
		}
	}
	
	if (o_ppuc_Aes128CbcDeCryptedData)
	{
		o_ppuc_Aes128CbcDeCryptedData = NULL;
		MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
	}

	if (m_b_EndThread == TRUE)
	{
		l_x_ResultDlg.DoModal();
		CleanAsyncMessages2();
	}

	if (m_px_Image != NULL)
	{
		delete(m_px_Image);
		m_px_Image = NULL;
	}

	EnableButtons();
	InitQualityBar();
	InitGuidingAnimations();
	RedrawWnd();

	return TRUE;
}

UC ChangeFingerNumToBiometricData
(
I	i_i_Finger
)
{
	switch (i_i_Finger)
	{
	case 0:		return (0x02 | 0x14); break;
	case 1:		return (0x02 | 0x10); break;
	case 2:		return (0x02 | 0x0C); break;
	case 3:		return (0x02 | 0x08); break;
	case 4:		return (0x02 | 0x04); break;
	case 5:		return (0x01 | 0x04); break;
	default:
	case 6:		return (0x01 | 0x08); break;
	case 7:		return (0x01 | 0x0C); break;
	case 8:		return (0x01 | 0x10); break;
	case 9:		return (0x01 | 0x14); break;
	}
}

BOOL AscToHexa(char *pIn, char *pOut, int LgMax)
{
	int	cpt_s, cpt_d;

	if (pOut == NULL)
	{
		return FALSE;
	}
	cpt_s = cpt_d = 0;
	while (cpt_d < LgMax)
	{
		if (pIn[cpt_s] == 0)
		{
			return FALSE;
		}
		/* first character process */
		if (pIn[cpt_s] >= '0' && pIn[cpt_s] <= '9')
		{
			pOut[cpt_d] = (pIn[cpt_s++] - 0x30) << 4;
		}
		else if (pIn[cpt_s] >= 'A' && pIn[cpt_s] <= 'F')
		{
			pOut[cpt_d] = (pIn[cpt_s++] - 0x37) << 4;
		}
		else if (pIn[cpt_s] >= 'a' && pIn[cpt_s] <= 'f')
		{
			pOut[cpt_d] = (pIn[cpt_s++] - 0x57) << 4;
		}
		else if (pIn[cpt_s] == ' ')
		{
			cpt_s++;
			continue;
		}
		else
		{
			return FALSE;
		}
		/* second character process */
		if (pIn[cpt_s] >= '0' && pIn[cpt_s] <= '9')
		{
			pOut[cpt_d++] |= pIn[cpt_s++] - 0x30;
		}
		else if (pIn[cpt_s] >= 'A' && pIn[cpt_s] <= 'F')
		{
			pOut[cpt_d++] |= pIn[cpt_s++] - 0x37;
		}
		else if (pIn[cpt_s] >= 'a' && pIn[cpt_s] <= 'f')
		{
			pOut[cpt_d++] |= pIn[cpt_s++] - 0x57;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::_MatchOnCard()
{

	if (m_BioOpPSheet.m_MocPPage.m_b_GetInfo)
		return GetInfoCCID();
	else
		return MatchAndStoreOnCard();

}


//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::GetInfoCCID()
{
	B							l_b_Ret = TRUE;
	I							l_i_Ret = MORPHO_OK;
	I							l_i_InternalErrorCode = 0;
	CString						l_cs_Message = CString("");
	CResultDlg					l_x_ResultDlg;
	GetInfoDlg					l_x_InfoDlg;
	B							l_b_runUnlockDeviceProcess = FALSE;
	SmartCardCtx				l_sCardCtx;
	CSCardReader				l_x_CardReader(SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1);
	CString						l_str_ReaderName = CString("");
	I							l_i_NbReader = 0;

	T_BUFFER 					l_x_GetInfoBuffer;
	T__BUF 						l_x_GetInfoResponse;

	memset(&l_sCardCtx, 0, sizeof(SmartCardCtx));

	//Enumerate connected smart card reader
	l_i_Ret = l_x_CardReader.EnumReader(&l_i_NbReader);

	//select first smart card reader connected
	if (l_i_Ret == MORPHO_OK)
	{
		l_str_ReaderName = l_x_CardReader.GetReaderName(0);

		//Power on the card
		l_i_Ret = l_x_CardReader.PowerOn(l_str_ReaderName);
	}


	if (l_i_Ret == MORPHO_OK)
	{
		UL	l_ul_CmdID = CMDID_BIOPAD_GET_FEATURE_INFOS;
		I	l_i_offset = 0;

		//Build CCID command
		l_x_GetInfoBuffer.m_puc_Buffer = new UC[CCID_MAX_DATA_LENGTH];

		memcpy(l_x_GetInfoBuffer.m_puc_Buffer, &l_ul_CmdID, sizeof(l_ul_CmdID)); //dwCmdID
		l_i_offset = sizeof(l_ul_CmdID);

		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)TAG_FORMAT_TEXT; //TAG_FORMAT_TEXT
		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)BINDESC_VERSION; //TAG_BINDESC_VERSION 
		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)BINDESC_SOFT; //TAG_BINDESC_SOFT 
		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)BINDESC_FLASH_SIZE; //TAG_BINDESC_FLASH_SIZE
		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)BINDESC_PRODUCT_NAME; //TAG_BINDESC_PRODUCT_NAME 
		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)BINDESC_PID; //TAG_BINDESC_PID  
		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)BINDESC_SN; //TAG_BINDESC_SN  
		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)BINDESC_OEM_PID; //TAG_BINDESC_OEM_PID 
		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)BINDESC_OEM_SN; //TAG_BINDESC_OEM_SN
		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)BINDESC_SENSOR_ID; //TAG_BINDESC_SENSOR_ID
		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)BINDESC_SENSOR_SN; //TAG_BINDESC_SENSOR_SN
		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)BINDESC_LICENSES; //TAG_BINDESC_LICENSES
		l_x_GetInfoBuffer.m_puc_Buffer[l_i_offset++] = (UC)BINDESC_CUSTOM_DESCRIPTOR; //TAG_BINDESC_CUSTOM_DESCRIPTOR

		l_x_GetInfoBuffer.m_us_Size = l_i_offset;

		//Build CCID response
		l_x_GetInfoResponse.m_ul_Size = CCID_MAX_DATA_LENGTH; //Taille max d'une reponse GetIinfo: 2048
		l_x_GetInfoResponse.m_puc_Buf = new UC[CCID_MAX_DATA_LENGTH];
		memset(l_x_GetInfoResponse.m_puc_Buf, 0, l_x_GetInfoResponse.m_ul_Size);

		//Send Escape commande
		l_i_Ret = l_x_CardReader.SendEscapeCmd(l_x_GetInfoBuffer.m_puc_Buffer, l_x_GetInfoBuffer.m_us_Size, l_x_GetInfoResponse.m_puc_Buf, &(l_x_GetInfoResponse.m_ul_Size));

		//Analyse CCID response
		if (l_i_Ret != MORPHO_OK)
		{
			memcpy(&l_i_InternalErrorCode, l_x_GetInfoResponse.m_puc_Buf, 4);
		}
		else
		{
			CString l_aux_Description = CString("");
			UC		l_uc_Tag;
			I		l_i_Length = 0;
			C		l_c_Value[CCID_MAX_DATA_LENGTH]; //Taille max d'un champs 2048
			I		l_i_Counter = 0;

			while (l_i_Counter < l_x_GetInfoResponse.m_ul_Size)
			{
				l_uc_Tag = l_x_GetInfoResponse.m_puc_Buf[l_i_Counter];
				memcpy(&l_i_Length, l_x_GetInfoResponse.m_puc_Buf + (++l_i_Counter), 2);
				l_i_Counter += 2;

				if (l_i_Length > l_x_GetInfoResponse.m_ul_Size - l_i_Counter)
				{
					l_i_Ret = MORPHOERR_BADPARAMETER;
					break;
				}

				if (l_uc_Tag == BINDESC_FLASH_SIZE)
				{
					I		l_i_FlashSize = 0;
					CString l_cs_TempMsg;
					memcpy(&l_i_FlashSize, l_x_GetInfoResponse.m_puc_Buf + l_i_Counter, l_i_Length);
					l_cs_TempMsg.Format("Flash size: \t %d MiB \r\n", l_i_FlashSize / 1024 / 1024);
					l_aux_Description.Append(l_cs_TempMsg);
				}
				else
				{
					switch (l_uc_Tag)
					{
					case TAG_DESC_PRODUCT:
						l_aux_Description.Append("Product description \r\n");
						break;
					case TAG_DESC_SENSOR:
						l_aux_Description.Append("Sensor description \r\n");
						break;
					case TAG_DESC_SOFTWARE:
						l_aux_Description.Append("Software description \r\n");
						break;
					case BINDESC_VERSION:
						l_aux_Description.Append("Version: \t");
						break;
					case BINDESC_SOFT:
						l_aux_Description.Append("Soft: \t");
						break;
					case BINDESC_PRODUCT_NAME:
						l_aux_Description.Append("Product name: \t");
						break;
					case BINDESC_PID:
						l_aux_Description.Append("PID: \t");
						break;
					case BINDESC_SN:
						l_aux_Description.Append("SN: \t");
						break;
					case BINDESC_OEM_PID:
						l_aux_Description.Append("OEM PID: \t");
						break;
					case BINDESC_OEM_SN:
						l_aux_Description.Append("OEM SN: \t");
						break;
					case BINDESC_SENSOR_ID:
						l_aux_Description.Append("Sensor ID: \t");
						break;
					case BINDESC_SENSOR_SN:
						l_aux_Description.Append("Sensor SN: \t");
						break;
					case BINDESC_LICENSES:
						l_aux_Description.Append("Licences: \t");
						break;
					case BINDESC_CUSTOM_DESCRIPTOR:
						l_aux_Description.Append("Custom description: \t");
						break;
					default:
						l_aux_Description.Append("autre: \t");
						break;
					}
					memcpy(l_c_Value, l_x_GetInfoResponse.m_puc_Buf + l_i_Counter, l_i_Length);
					l_aux_Description.Append(CString(l_c_Value, l_i_Length));
					l_aux_Description.Append("\r\n");
				}
				l_i_Counter += l_i_Length;
			}

			l_x_InfoDlg.m_cs_Msg = CString(l_aux_Description);
		}

		//Memory free
		free(l_x_GetInfoBuffer.m_puc_Buffer);
		l_x_GetInfoBuffer.m_puc_Buffer = NULL;
		l_x_GetInfoBuffer.m_us_Size = 0;

		free(l_x_GetInfoResponse.m_puc_Buf);
		l_x_GetInfoResponse.m_puc_Buf = NULL;
		l_x_GetInfoResponse.m_ul_Size = 0;
	}

	//Result Display
	if ((l_i_Ret <= SMARTCARD_BUSY) && (l_i_Ret >= SMARTCARD_BAD_IDENTIFIER_BUFFER_SIZE))
	{
		if (l_i_Ret == SMARTCARD_STATUS_BYTES_ERROR)
			l_x_ResultDlg.m_cs_Msg1.Format("SmartCard Error returned");
		else if (l_i_Ret == SMARTCARD_READER_ERROR)
		{
			l_x_ResultDlg.m_cs_Msg1.Format("SCard Reader Error returned");
			l_cs_Message.Format("FW Internal Error code = %d", l_i_InternalErrorCode);
			l_x_ResultDlg.m_cs_Msg4 = l_cs_Message;
		}
		else
			l_x_ResultDlg.m_cs_Msg1.Format("SmartCard access failed");
		l_x_ResultDlg.m_cs_Msg3.Format("Err N°0x%08X", l_i_Ret);
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_b_Ret = FALSE;
		l_x_CardReader.ConvertSCardError(l_i_Ret, "", l_x_ResultDlg.m_cs_Msg5);
	}
	else if (l_i_Ret == MORPHOERR_DEVICE_LOCKED)
	{
		l_b_runUnlockDeviceProcess = TRUE;
		l_b_Ret = FALSE;
	}
	else if (l_i_Ret != MORPHO_OK)
	{
		ShowAnimation("image_Ko");
		l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
		l_x_ResultDlg.m_cs_Msg3.Format("MSO_DemoDlg::GetInfoCCID() function");
		ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
		l_x_ResultDlg.m_cs_Msg1.Format("Get Info Failed");
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_b_Ret = FALSE;
	}

	//Need to unlock locked device
	if (l_b_runUnlockDeviceProcess == TRUE)
	{
		I l_i_Ret2 = UnlockDevice();
		if (l_i_Ret2 != MORPHO_OK)
		{
			ShowErrorBox("An error occured during Device Unlocking", l_i_Ret2);
		}
	}
	if (m_b_EndThread == TRUE)
	{
		if (l_b_runUnlockDeviceProcess == FALSE)
		{
			if (l_i_Ret != MORPHO_OK)
				l_x_ResultDlg.DoModal();
			else
				l_x_InfoDlg.DoModal();
		}
		CleanAsyncMessages2();
	}

	RedrawWnd();

	return !!l_b_Ret;
}



//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::MatchAndStoreOnCard()
{
	I							l_i_Ret;
	I							l_i_InternalErrorCode = 0;
	CString						l_cs_Message;
	UL							l_ul_CallbackMask;
	B							l_b_Ret;
	CResultDlg					l_x_ResultDlg;
	T_MORPHO_TEMPLATE_ENVELOP	l_x_TypEnvelop;
	UC							l_uc_NbFingers;
	T_MORPHO_TYPE_TEMPLATE		l_x_TypeTemplate;
	US							l_us_MaxSizeTemplate;
	CString						l_cs_FileID;

	C_MORPHO_TemplateList 		l_x_TemplateList;

	T_MORPHO_MOC_PARAMETERS			l_x_MocParameters;
	T_MORPHO_MOC_APDU_PARAMETERS	l_x_MocApduParameters;

	UC l_auc_ApduToTrack[] = { 0x81, 0x82, 0x83, 0x84, 0x01, 0x00 };	// Anything that can pass throuh Scard... fonction
	UC l_auc_ApduHeaderToSend_SmartCardTypeV1_Enroll[] = { 0x00, 0x24, 0x01, 0x9C, 0x00 };
	UC l_auc_ApduHeaderToSend_SmartCardTypeV1_Verif[] = { 0x00, 0x20, 0x00, 0x9C, 0x00 };
	UC l_auc_ApduHeaderToSend_SmartCardTypeV2_Enroll[] = { 0x00, 0x24, 0x01, 0x00, 0x00 };
	UC l_auc_ApduHeaderToSend_SmartCardTypeV2_Verif[] = { 0x00, 0x20, 0x01, 0x08, 0x00 };
	UC l_auc_ApduHeaderToSend_Unique[APDU_HEADER_LEN];
	UC l_auc_ApduHeaderToSend_Other[APDU_HEADER_LEN];
	UC l_auc_Diversification[16];
	UC l_auc_DataToWriteBeforeTemplate[255];
	UC l_auc_DataToWriteAfterTemplate[255];
	UL l_ul_CmdID;
	UC l_uc_CaptureStatus = 0; //bCaptureStatus

	T_MORPHO_TYPE_TEMPLATE		l_t_TypeTemplate;
	UL							l_ul_LenTemplate;
	PUC							l_puc_DataTemplate;

	C							l_ac_DiversificationBytes[4];
	B							l_b_ConvRet;


	B							l_b_runUnlockDeviceProcess = FALSE;

	l_b_Ret = TRUE;

	//------------------------------------------------------------
	// Viewer or not viewer.... 
	//------------------------------------------------------------

	l_ul_CallbackMask = m_ul_CallbackMask;

	if (m_tbCtrl.m_TabOptionsDlg->m_b_MainViewerActive)
	{
		m_px_Image = new C_MORPHO_Image(6);
	}

	if (m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
	{
		PostMessage(WM_ADD_PROCESS_PAGE, 0, 0);
	}
	else
	{
		m_ctl_CurrentMiniViewer = NULL;
	}

	if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_PKCOMP)
	{
		l_x_TypeTemplate = MORPHO_PK_COMP;
		l_us_MaxSizeTemplate = 255;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_PKCOMPNORM)
	{
		l_x_TypeTemplate = MORPHO_PK_COMP_NORM;
		l_us_MaxSizeTemplate = 255;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_PKMAT)
	{
		l_x_TypeTemplate = MORPHO_PK_MAT;
		l_us_MaxSizeTemplate = 1;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_PKMATNORM)
	{
		l_x_TypeTemplate = MORPHO_PK_MAT_NORM;
		l_us_MaxSizeTemplate = 1;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_PKLITE)
	{
		l_x_TypeTemplate = MORPHO_PK_PKLITE;
		l_us_MaxSizeTemplate = 1;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_PKS)
	{
		l_x_TypeTemplate = MORPHO_PK_COMP;
		l_us_MaxSizeTemplate = 255;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_ANSI378)
	{
		l_x_TypeTemplate = MORPHO_PK_ANSI_378;
		l_us_MaxSizeTemplate = 255;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_ANSI378_2009)
	{
		l_x_TypeTemplate = MORPHO_PK_ANSI_378_2009;
		l_us_MaxSizeTemplate = 255;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_ISO_FMC_CS)
	{
		l_x_TypeTemplate = MORPHO_PK_ISO_FMC_CS;
		l_us_MaxSizeTemplate = 255;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_ISO_FMC_NS)
	{
		l_x_TypeTemplate = MORPHO_PK_ISO_FMC_NS;
		l_us_MaxSizeTemplate = 255;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_ISO_FMR)
	{
		l_x_TypeTemplate = MORPHO_PK_ISO_FMR;
		l_us_MaxSizeTemplate = 255;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_ISO_FMR_2011)
	{
		l_x_TypeTemplate = MORPHO_PK_ISO_FMR_2011;
		l_us_MaxSizeTemplate = 255;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_MINEX_A)
	{
		l_x_TypeTemplate = MORPHO_PK_MINEX_A;
		l_us_MaxSizeTemplate = 255;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_PK_MOC)
	{
		l_x_TypeTemplate = MORPHO_PK_MOC;
		l_us_MaxSizeTemplate = 255;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_PK_DIN_V66400_CS)
	{
		l_x_TypeTemplate = MORPHO_PK_DIN_V66400_CS;
		l_us_MaxSizeTemplate = 56;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_PK_DIN_V66400_CS_AA)
	{
		l_x_TypeTemplate = MORPHO_PK_DIN_V66400_CS_AA;
		l_us_MaxSizeTemplate = 56;
	}
	else if (m_BioOpPSheet.m_MocPPage.m_cs_TemplateType == STR_ISO_FMC_CS_AA)
	{
		l_x_TypeTemplate = MORPHO_PK_ISO_FMC_CS_AA;
		l_us_MaxSizeTemplate = 255;
	}

	//------------------------------------------------------------
	// Number of finger
	//------------------------------------------------------------
	l_uc_NbFingers = 1;

	//------------------------------------------------------------
	// Set the template envelop type
	//------------------------------------------------------------
	l_x_TypEnvelop = MORPHO_RAW_TEMPLATE;

	// Set the MatchOnCard Parameters
	memset(&l_x_MocParameters, 0, sizeof(l_x_MocParameters));
	l_x_MocParameters.m_i_TimeOut = 60;


	SmartCardCtx	l_sCardCtx;
	CSCardReader	l_x_CardReader(SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1);
	CString			l_str_ReaderName;
	I				l_i_NbReader;

	memset(&l_sCardCtx, 0, sizeof(SmartCardCtx));
	//Enumerate connected smart card reader
	l_i_Ret = l_x_CardReader.EnumReader(&l_i_NbReader);

	//select first smart card reader connected
	if (l_i_Ret == MORPHO_OK)
	{
		l_str_ReaderName = l_x_CardReader.GetReaderName(0);

		//Power on the card
		l_i_Ret = l_x_CardReader.PowerOn(l_str_ReaderName);
	}

	if (l_i_Ret == MORPHO_OK)
	{
		m_b_BiometricOperationProcessing = TRUE;

		//Call Capture function via ILV
		if (!(m_BioOpPSheet.m_MocPPage.m_b_FullCCID))
		{
			l_i_Ret = m_x_MorphoSmart.Capture(
				m_tbCtrl.m_TabBioSettingsDlg->m_i_Timeout,								//US							i_us_Timeout, 
				l_uc_NbFingers,								//UC							i_uc_FingerNumber,
				l_x_TypeTemplate,							//T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
				l_us_MaxSizeTemplate,						//US							i_us_MaxSizeTemplate,	
				m_BioOpPSheet.m_MocPPage.m_i_CaptureType,	//UC							i_uc_enrolType (1), 
				l_ul_CallbackMask,							//UL							i_ul_CallbackCmd,
				BiometricCallback,							//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback, 
				this,										//PVOID							i_pv_CallbackArgument,
				l_x_TemplateList,							//C_MORPHO_TemplateList &		o_x_Template);
				l_x_TypEnvelop,								//T_MORPHO_TEMPLATE_ENVELOP		i_x_typEnvelop,
				0,											//I								i_i_ApplicationDataLen,
				NULL,										//PUC							i_puc_ApplicationData
				m_BioOpPSheet.m_MocPPage.m_b_LatentDetect,	//UC							i_uc_LatentDetection
				m_tbCtrl.m_TabBioSettingsDlg->m_i_MatchingStrategy,							//I								i_i_CoderChoice
				MORPHO_ENROLL_DETECT_MODE | m_ul_WakeUpMode,
				0,
				&l_x_MocParameters
				);

			// (1) :	- m_i_CaptureType = 0 => capture based on 3 acquisitions + templates consolidation to compute the best reference template
			//			- m_i_CaptureType = 1 => capture based on 1 single acquisition to compute a template for verification

			//m_b_BiometricOperationProcessing = FALSE;
			CleanAsyncMessages();

			//------------------------------------------------
			// Remove handle on the Miniviewer 
			m_ctl_CurrentMiniViewer = NULL;
			//------------------------------------------------
		}
		//Call Capture function via CCID
		else
		{
			T_BUFFER 	l_x_captureBuffer;
			T__BUF 		l_x_captureResponse;
			CString		l_cs_FingerQuality;
			I			l_i_FingerQuality = 0;
			US 			l_us_langID = 0x0409;
			I			l_i_offset;

			GetSecurityLevel(1);
			l_ul_CmdID = CMDID_BIOPAD_FEATURE_CAPTURE_TEMPLATE;

			//Build CCID command
			l_x_captureBuffer.m_puc_Buffer = new UC[CCID_MAX_DATA_LENGTH];

			memcpy(l_x_captureBuffer.m_puc_Buffer, &l_ul_CmdID, sizeof(l_ul_CmdID)); //dwCmdID
			l_i_offset = sizeof(l_ul_CmdID);
			memcpy(l_x_captureBuffer.m_puc_Buffer + l_i_offset, &m_tbCtrl.m_TabBioSettingsDlg->m_i_Timeout, sizeof(US)); //bTimeOut
			l_i_offset += sizeof(US);
			memset(l_x_captureBuffer.m_puc_Buffer + l_i_offset, 0x0000, sizeof(US)); //bTimeOut2
			l_i_offset += sizeof(US);
			l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)0x00; //bNumberMessage
			memcpy(l_x_captureBuffer.m_puc_Buffer + l_i_offset, &l_us_langID, sizeof(l_us_langID)); //wlangID
			l_i_offset += sizeof(l_us_langID);
			l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)0x00; //bMsgIndex1
			l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)0x01; //bMsgIndex2
			l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)0x02; //bMsgIndex3			
			memcpy(l_x_captureBuffer.m_puc_Buffer + l_i_offset, &(l_x_MocParameters.m_i_TimeOut), sizeof(US)); //wMocTimeout
			l_i_offset += sizeof(US);
			if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_FingerPrint_Quality)
			{
				m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_EDIT_FINGERPRINT_QUALITY)->GetWindowText(l_cs_FingerQuality);
				l_i_FingerQuality = atoi(l_cs_FingerQuality);
			}
			l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)l_i_FingerQuality; //bQualityThreshold

			if (m_BioOpPSheet.m_MocPPage.m_i_CaptureType == 1)
				l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)0x01; //bEnrollType
			else
				l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)0x03; //bEnrollType

			l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)l_uc_NbFingers; //bFingerNumber
			l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)l_x_TypeTemplate; //bTemplateType
			l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)l_x_TypEnvelop; //bTemplateEnvType
			l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)m_BioOpPSheet.m_MocPPage.m_i_TemplateMaxSize; //bTemplateMaxSize
			l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)(m_BioOpPSheet.m_MocPPage.m_b_LatentDetect); //bLatentDetection
			l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)(m_tbCtrl.m_TabBioSettingsDlg->m_i_CoderChoice); //bCoderChoice

			if (m_BioOpPSheet.m_MocPPage.m_i_CaptureType == 1)
				l_x_captureBuffer.m_puc_Buffer[l_i_offset] = MORPHO_VERIF_DETECT_MODE;
			// MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE is accepted just in case of enroll mode 
			else
			{
				l_x_captureBuffer.m_puc_Buffer[l_i_offset] = MORPHO_ENROLL_DETECT_MODE;
				if (m_tbCtrl.m_TabBioSettingsDlg->m_cmd_ForceFingerPlacement != 0)
					l_x_captureBuffer.m_puc_Buffer[l_i_offset] |= MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE;
			}
			l_x_captureBuffer.m_puc_Buffer[l_i_offset++] |= (UC)m_ul_WakeUpMode; //bDetectModeChoice
			if (m_i_SecurityLevel == FFD_SECURITY_LEVEL_DEFAULT_HOST)
				l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)FFD_SECURITY_LEVEL_NONE_HOST; //bSecurityLevel
			else
				l_x_captureBuffer.m_puc_Buffer[l_i_offset++] = (UC)m_i_SecurityLevel; //bSecurityLevel

			l_x_captureBuffer.m_us_Size = l_i_offset;

			//Build CCID response
			l_x_captureResponse.m_ul_Size = CCID_MAX_DATA_LENGTH;
			l_x_captureResponse.m_puc_Buf = new UC[CCID_MAX_DATA_LENGTH];

			//Send Escape commande
			l_i_Ret = l_x_CardReader.SendEscapeCmd(l_x_captureBuffer.m_puc_Buffer, l_x_captureBuffer.m_us_Size, l_x_captureResponse.m_puc_Buf, &(l_x_captureResponse.m_ul_Size));

			//Analyse CCID response
			if (l_i_Ret != MORPHO_OK)
			{
				memcpy(&l_i_InternalErrorCode, l_x_captureResponse.m_puc_Buf, sizeof(l_i_InternalErrorCode));
			}
			else
			{
				l_uc_CaptureStatus = (UC)l_x_captureResponse.m_puc_Buf[0];
				memcpy(&l_ul_LenTemplate, l_x_captureResponse.m_puc_Buf + 1, sizeof(l_ul_LenTemplate));
				if ((l_uc_CaptureStatus != 0) || (l_ul_LenTemplate <= 0))
					l_i_Ret = MORPHOERR_STATUS;
				else
				{
					l_puc_DataTemplate = new UC[l_ul_LenTemplate];
					memset(l_puc_DataTemplate, 0, l_ul_LenTemplate);
				}
			}

			//Memory free
			free(l_x_captureBuffer.m_puc_Buffer);
			l_x_captureBuffer.m_puc_Buffer = NULL;
			l_x_captureBuffer.m_us_Size = 0;

			free(l_x_captureResponse.m_puc_Buf);
			l_x_captureResponse.m_puc_Buf = NULL;
			l_x_captureResponse.m_ul_Size = 0;
		}
	}

	// Send APDU commands before MOC
	if (l_i_Ret == MORPHO_OK)
	{
		switch (m_BioOpPSheet.m_MocPPage.m_i_CardType)// SmartCard V1 / V2
		{
		case 0:
		{
			// V1

			// If need to do something in the card before the action on the template. 

			// Select the Application
			UC	l_auc_File[] = { 0xA0, 0x00, 0x00, 0x00, 0x00, 0x59, 0x50, 0x53, 0x49, 0x44 };

			l_i_Ret = l_x_CardReader.Select(l_auc_File, sizeof(l_auc_File), NULL);

			if (m_BioOpPSheet.m_MocPPage.m_i_TemplateActionType == 0)
				// If the action is "Save the Template in the Card"
			{
				UC	l_auc_Data;

				// Present the Pin
				if (l_i_Ret == MORPHO_OK)
				{
					UC	l_auc_PIN[] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38 };
					l_i_Ret = l_x_CardReader.Verify(l_auc_PIN, sizeof(l_auc_PIN), 2);
				}

				// Erase the 4 Biometric Datas
				l_auc_Data = 0;
				if (l_i_Ret == MORPHO_OK)
				{
					l_i_Ret = l_x_CardReader.UpdateBinary255(0x8100, &l_auc_Data, sizeof(l_auc_Data));
				}
				if (l_i_Ret == MORPHO_OK)
				{
					l_i_Ret = l_x_CardReader.UpdateBinary255(0x8101, &l_auc_Data, sizeof(l_auc_Data));
				}

				if (l_i_Ret == MORPHO_OK)
				{
					l_i_Ret = l_x_CardReader.UpdateBinary255(0x8102, &l_auc_Data, sizeof(l_auc_Data));
				}

				if (l_i_Ret == MORPHO_OK)
				{
					l_i_Ret = l_x_CardReader.UpdateBinary255(0x8103, &l_auc_Data, sizeof(l_auc_Data));
				}

				// Update the Biometric Datas
				if (l_i_Ret == MORPHO_OK)
				{
					l_auc_Data = ChangeFingerNumToBiometricData(m_BioOpPSheet.m_MocPPage.m_i_FingerUsed);

					l_i_Ret = l_x_CardReader.UpdateBinary255(0x8100, &l_auc_Data, sizeof(l_auc_Data));
				}
			}
			else
				// if the action is "Match the Template with the one in the Card"
			{
				// Nothing to do
			}
		}
		break;

		case 1:
		{
			// V2
			// Select the Application
			UC	l_auc_File[] = { 0xA0, 0x00, 0x00, 0x02, 0x43, 0x00, 0x15, 0x00, 0x03, 0x01, 0x00, 0x01 };
			UC	l_auc_Temp[255];
			T__BUF		i_x_Buff;

			i_x_Buff.m_puc_Buf = l_auc_Temp;
			i_x_Buff.m_ul_Size = 255;

			l_i_Ret = l_x_CardReader.Select(l_auc_File, sizeof(l_auc_File), &i_x_Buff);

			if (m_BioOpPSheet.m_MocPPage.m_i_TemplateActionType == 0)
				// If the action is "Save the Template in the Card"
			{
				// Present the Pin
				if (l_i_Ret == MORPHO_OK)
				{
					UC	l_auc_PIN[] = { 0x8C, 0x9C, 0x6A, 0x84, 0xDA, 0xF5, 0x1E, 0x95, 0xE0, 0x48, 0xDC, 0xA2, 0xEC, 0x29, 0x5D, 0x25 }; // this buffer is 12345678 ciffered.
					l_i_Ret = l_x_CardReader.Verify(l_auc_PIN, sizeof(l_auc_PIN), 0x01, 0x19);
				}
			}
			else
				// if the action is "Match the Template with the one in the Card"
			{
				// Nothing to do
			}

			if (l_i_Ret == MORPHO_OK)
			{
				if (m_BioOpPSheet.m_MocPPage.m_b_Diversification == TRUE)
				{
					// Convert and check diversification bytes
					memset(l_ac_DiversificationBytes, 0, sizeof(l_ac_DiversificationBytes));
					l_b_ConvRet = TRUE;

					l_b_ConvRet &= AscToHexa((PC)(LPCTSTR)m_BioOpPSheet.m_MocPPage.m_cs_D0, &l_ac_DiversificationBytes[0], 1);
					l_b_ConvRet &= AscToHexa((PC)(LPCTSTR)m_BioOpPSheet.m_MocPPage.m_cs_D1, &l_ac_DiversificationBytes[1], 1);
					l_b_ConvRet &= AscToHexa((PC)(LPCTSTR)m_BioOpPSheet.m_MocPPage.m_cs_D2, &l_ac_DiversificationBytes[2], 1);
					l_b_ConvRet &= AscToHexa((PC)(LPCTSTR)m_BioOpPSheet.m_MocPPage.m_cs_D3, &l_ac_DiversificationBytes[3], 1);
					if (!l_b_ConvRet)
						AfxMessageBox("Diversification data might be wrongly formatted !");

					// Get card serial number
					UC l_auc_CardSerialNumber[8];
					i_x_Buff.m_puc_Buf = l_auc_CardSerialNumber;
					i_x_Buff.m_ul_Size = sizeof(l_auc_CardSerialNumber);
					l_i_Ret = l_x_CardReader.GetData(0x0121, &i_x_Buff);

					// Diversification data : 16 bytes
					// +-------------------------------------------------------------------------------+
					// | 00 | 01 | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 10 | 11 | 12 | 13 | 14 | 15 |
					// +-------------------------------------------------------------------------------+
					// |   CardSerialNumber[2->7]    | D0 | D1 |   CardSerialNumber[2->7]    | D2 | D3 |
					// +-------------------------------------------------------------------------------+
					memcpy(l_auc_Diversification, l_auc_CardSerialNumber + 2, 6);
					l_auc_Diversification[6] = (UC)l_ac_DiversificationBytes[0]; // D0
					l_auc_Diversification[7] = (UC)l_ac_DiversificationBytes[1]; // D1
					memcpy(l_auc_Diversification + 8, l_auc_CardSerialNumber + 2, 6);
					l_auc_Diversification[14] = (UC)l_ac_DiversificationBytes[2]; // D2
					l_auc_Diversification[15] = (UC)l_ac_DiversificationBytes[3]; // D3
				}
			}
		}
		break;

		case 2:
			// Morpho ypsID S2
			Tool_ypsIDS2_SCard_Action(&l_x_CardReader, &l_sCardCtx, ACTION_SELECT_APPLET_PKI | ACTION_GET_SERIALNUMBER, NULL, NULL, 0, NULL, NULL);

			Tool_ypsIDS2_SCard_Action(&l_x_CardReader, &l_sCardCtx,
				((m_BioOpPSheet.m_MocPPage.m_i_TemplateActionType == 0) ? ACTION_VERIFY_PIN_ADMIN : 0) | ACTION_SELECT_APPLET_PKI,
				NULL, NULL, 0, NULL, NULL);

			break;

		case 3:
			// Morpho ypsID S3
			Tool_ypsIDS3_SCard_Action(&l_x_CardReader, &l_sCardCtx, ACTION_SELECT_APPLET_PKI | ACTION_GET_SERIALNUMBER, NULL, NULL, 0, NULL, NULL);

			Tool_ypsIDS3_SCard_Action(&l_x_CardReader, &l_sCardCtx,
				((m_BioOpPSheet.m_MocPPage.m_i_TemplateActionType == 0) ? ACTION_VERIFY_PIN_ADMIN : 0) | ACTION_SELECT_APPLET_PKI,
				NULL, NULL, 0, NULL, NULL);

			break;
		}
	}

	if ((l_i_Ret == MORPHO_OK) && (!(m_BioOpPSheet.m_MocPPage.m_b_FullCCID)))
	{
		l_i_Ret = l_x_TemplateList.GetTemplate(
			0,					// UC	i_uc_indexTemplate,
			l_t_TypeTemplate,	// T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
			l_ul_LenTemplate,	// UL	&o_ul_lenTemplate,
			l_puc_DataTemplate  // PUC	&o_puc_dataTemplate
			);
	}

	// Format the MatchOnCard APDU Parameters with the informations from the Capture
	memset(&l_x_MocApduParameters, 0, sizeof(l_x_MocApduParameters));
	if (l_i_Ret == MORPHO_OK)
	{
		switch (m_BioOpPSheet.m_MocPPage.m_i_CardType)// SmartCard V1 / V2
		{
		case 0:
			// yps ID S1
			// l_x_MocApduParameters.m_puc_ApduHeaderToTrack parameter
			l_x_MocApduParameters.m_puc_ApduToTrack = l_auc_ApduToTrack;

			l_x_MocApduParameters.m_x_DiversificationType = MORPHO_MOC_DIVERSIFICATION_NO;
			l_x_MocApduParameters.m_x_CipheredType = MORPHO_MOC_CIPHERED_NO;
			l_x_MocApduParameters.m_x_PaddingType = MORPHO_MOC_APDU_PADDING_TYPE_NO;
			l_x_MocApduParameters.m_x_ChainType = MORPHO_MOC_APDU_CHAIN_TYPE_1;

			if (m_BioOpPSheet.m_MocPPage.m_i_TemplateActionType == 0) // Save the Template in the Card
			{
				// Enroll
				memcpy(l_auc_ApduHeaderToSend_Unique, l_auc_ApduHeaderToSend_SmartCardTypeV1_Enroll, APDU_HEADER_LEN);
			}
			else
			{
				// Verif
				memcpy(l_auc_ApduHeaderToSend_Unique, l_auc_ApduHeaderToSend_SmartCardTypeV1_Verif, APDU_HEADER_LEN);

				// Add the Template length to the buffer
				l_x_MocApduParameters.m_i_DataToWriteBeforeTemplateLen = 1;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate = l_auc_DataToWriteBeforeTemplate;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[0] = (UC)l_ul_LenTemplate;

				// All Verify template must take 169 bytes, then we must complete with 0x00
				l_x_MocApduParameters.m_i_DataToWriteAfterTemplateLen = 168 - l_ul_LenTemplate;
				l_x_MocApduParameters.m_puc_DataToWriteAfterTemplate = l_auc_DataToWriteAfterTemplate;
				memset(l_x_MocApduParameters.m_puc_DataToWriteAfterTemplate, 0x00, l_x_MocApduParameters.m_i_DataToWriteAfterTemplateLen);
			}

			// Set the Unique APDU parameter
			l_x_MocApduParameters.m_puc_ApduHeaderToSend_Unique = l_auc_ApduHeaderToSend_Unique;

			// Set the other APDU parameters
			// As defined in the Specification document, the Type 1 is for Sagem specific Card.
			memcpy(l_auc_ApduHeaderToSend_Other, l_auc_ApduHeaderToSend_Unique, APDU_HEADER_LEN);
			l_auc_ApduHeaderToSend_Other[0] = l_auc_ApduHeaderToSend_Other[0] | 0x10;

			l_x_MocApduParameters.m_puc_ApduHeaderToSend_First = l_auc_ApduHeaderToSend_Other;
			l_x_MocApduParameters.m_puc_ApduHeaderToSend_Middle = l_auc_ApduHeaderToSend_Other;
			l_x_MocApduParameters.m_puc_ApduHeaderToSend_Last = l_auc_ApduHeaderToSend_Unique;
			break;

		case 1:
			// yps ID S2
			// l_x_MocApduParameters.m_puc_ApduHeaderToTrack parameter
			l_x_MocApduParameters.m_puc_ApduToTrack = l_auc_ApduToTrack;

			l_x_MocApduParameters.m_x_DiversificationType = MORPHO_MOC_DIVERSIFICATION_NO;
			l_x_MocApduParameters.m_x_CipheredType = MORPHO_MOC_CIPHERED_3DES_ECB;
			l_x_MocApduParameters.m_x_PaddingType = MORPHO_MOC_APDU_PADDING_TYPE_1;
			l_x_MocApduParameters.m_x_ChainType = MORPHO_MOC_APDU_CHAIN_TYPE_1;

			if (m_BioOpPSheet.m_MocPPage.m_b_Diversification == TRUE)
			{
				l_x_MocApduParameters.m_x_DiversificationType = MORPHO_MOC_DIVERSIFICATION_TYPE_1;

				l_x_MocApduParameters.m_i_DiversificationDataLen = sizeof(l_auc_Diversification);
				l_x_MocApduParameters.m_puc_DiversificationData = l_auc_Diversification;
			}

			if (m_BioOpPSheet.m_MocPPage.m_i_TemplateActionType == 0) // Save the Template in the Card
			{
				// Enroll
				memcpy(l_auc_ApduHeaderToSend_Unique, l_auc_ApduHeaderToSend_SmartCardTypeV2_Enroll, APDU_HEADER_LEN);
				l_auc_ApduHeaderToSend_Unique[APDU_OFFSET_P2] = 0x08;

				DWORD l_dwPkSize;

				// Add the ASN1 header to the buffer
				l_dwPkSize = 0;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate = l_auc_DataToWriteBeforeTemplate;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[l_dwPkSize++] = (UC)0x7F;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[l_dwPkSize++] = (UC)0x60;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[l_dwPkSize++] = (UC)JMARK_BIT_TEMPLATE_VALUE_LEN;	// 0x24
				memcpy(l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate + l_dwPkSize, JMARK_BIT_TEMPLATE_VALUE, JMARK_BIT_TEMPLATE_VALUE_LEN);
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[l_dwPkSize + JMARK_BIT_TEMPLATE_VALUE_OFFSET_REFJMARKOBJECT] = (UC)0x08; // p_nIndexBIO_ToSet 
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[l_dwPkSize + JMARK_BIT_TEMPLATE_VALUE_OFFSET_REFFINGER] = ChangeFingerNumToBiometricData(m_BioOpPSheet.m_MocPPage.m_i_FingerUsed); // p_byPkRefFingerInfos
				l_dwPkSize += JMARK_BIT_TEMPLATE_VALUE_LEN;

				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[l_dwPkSize++] = 0x5F;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[l_dwPkSize++] = 0x2E;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[l_dwPkSize++] = (UC)0x82;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[l_dwPkSize++] = (UC)(l_ul_LenTemplate >> 8);
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[l_dwPkSize++] = (UC)(l_ul_LenTemplate);

				l_x_MocApduParameters.m_i_DataToWriteBeforeTemplateLen = l_dwPkSize;
			}
			else
			{
				// Verif
				memcpy(l_auc_ApduHeaderToSend_Unique, l_auc_ApduHeaderToSend_SmartCardTypeV2_Verif, APDU_HEADER_LEN);

				// Add the ASN1 header to the buffer
				l_x_MocApduParameters.m_i_DataToWriteBeforeTemplateLen = 4;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate = l_auc_DataToWriteBeforeTemplate;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[0] = (UC)0x5F;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[1] = (UC)0x2E;
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[2] = (UC)l_ul_LenTemplate + 1;

				// Add the Template length to the buffer
				l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate[3] = (UC)l_ul_LenTemplate;

				// Nothing after Template 
				l_x_MocApduParameters.m_i_DataToWriteAfterTemplateLen = 0;
				l_x_MocApduParameters.m_puc_DataToWriteAfterTemplate = NULL;
				memset(l_x_MocApduParameters.m_puc_DataToWriteAfterTemplate, 0x00, l_x_MocApduParameters.m_i_DataToWriteAfterTemplateLen);
			}

			// Set the Unique APDU parameter
			l_x_MocApduParameters.m_puc_ApduHeaderToSend_Unique = l_auc_ApduHeaderToSend_Unique;

			// Set the other APDU parameters
			// As defined in the Specification document, the Type 1 is for Sagem specific Card.
			memcpy(l_auc_ApduHeaderToSend_Other, l_auc_ApduHeaderToSend_Unique, APDU_HEADER_LEN);
			l_auc_ApduHeaderToSend_Other[0] = l_auc_ApduHeaderToSend_Other[0] | 0x10;

			l_x_MocApduParameters.m_puc_ApduHeaderToSend_First = l_auc_ApduHeaderToSend_Other;
			l_x_MocApduParameters.m_puc_ApduHeaderToSend_Middle = l_auc_ApduHeaderToSend_Other;
			l_x_MocApduParameters.m_puc_ApduHeaderToSend_Last = l_auc_ApduHeaderToSend_Unique;
			break;

		case 2:
			// Morpho ypsID S2
			Tool_ypsIDS2_FillParametersD2C(&l_x_MocApduParameters,
				l_puc_DataTemplate, l_ul_LenTemplate,
				0x08 /* ref SDO*/, 0x09 /* Right Finger index*/,
				l_sCardCtx.ddScardSerialNumber, l_sCardCtx.dwScardSerialNumberLength,
				(m_BioOpPSheet.m_MocPPage.m_i_TemplateActionType == 0) ? TRUE : FALSE);

			break;

		case 3:
			// Morpho ypsID S3
			Tool_ypsIDS3_FillParametersD2C(&l_x_MocApduParameters,
				l_puc_DataTemplate, l_ul_LenTemplate,
				0x11 /* ref SDO*/, 0x09 /* Right Finger index*/,
				(m_BioOpPSheet.m_MocPPage.m_i_TemplateActionType == 0) ? TRUE : FALSE);

			break;
		}

		// Send to MSO the Match On Card parameters via ILV
		if (!(m_BioOpPSheet.m_MocPPage.m_b_FullCCID))
		{
			l_i_Ret = m_x_MorphoSmart.SetMocApdu(&l_x_MocApduParameters);
		}
		// Send to MSO the Match On Card parameters via CCID
		else
		{
			I			l_i_offset;
			T_BUFFER	l_x_TemplateToCardBuffer;
			T__BUF 		l_x_TemplateToCardResponse;

			l_ul_CmdID = CMDID_BIOPAD_FEATURE_TEMPLATE_TO_CARD;

			//Update Max autorized length for an Apdu
			Update_ApduMaxLength(m_BioOpPSheet.m_MocPPage.m_i_ApduMaxLength);

			//1ere solution simplifié avant implementation complète du protocole T=1 à enlever
			//Update lengths in Apdu headers
			Tool_Length_ChainedApdu(&l_x_MocApduParameters, l_ul_LenTemplate);

			//Build CCID commande
			l_x_TemplateToCardBuffer.m_puc_Buffer = new UC[CCID_MAX_DATA_LENGTH];

			memcpy(l_x_TemplateToCardBuffer.m_puc_Buffer, &l_ul_CmdID, sizeof(l_ul_CmdID)); //dwCmdID
			l_i_offset = sizeof(l_ul_CmdID);

			if (l_x_CardReader.m_dw_ActiveProtocol == SCARD_PROTOCOL_T1)
				l_x_TemplateToCardBuffer.m_puc_Buffer[l_i_offset++] = (UC)0x01; //bActiveProtocol
			else
				l_x_TemplateToCardBuffer.m_puc_Buffer[l_i_offset++] = (UC)0x00; //bActiveProtocol

			memcpy(l_x_TemplateToCardBuffer.m_puc_Buffer + l_i_offset, l_x_MocApduParameters.m_puc_ApduHeaderToSend_Unique, APDU_HEADER_LENGTH); //ddAPDUheader_Unique
			l_i_offset += APDU_HEADER_LENGTH;
			memcpy(l_x_TemplateToCardBuffer.m_puc_Buffer + l_i_offset, l_x_MocApduParameters.m_puc_ApduHeaderToSend_First, APDU_HEADER_LENGTH); //ddAPDUheader_First
			l_i_offset += APDU_HEADER_LENGTH;
			memcpy(l_x_TemplateToCardBuffer.m_puc_Buffer + l_i_offset, l_x_MocApduParameters.m_puc_ApduHeaderToSend_Middle, APDU_HEADER_LENGTH); //ddAPDUheader_Middle
			l_i_offset += APDU_HEADER_LENGTH;
			memcpy(l_x_TemplateToCardBuffer.m_puc_Buffer + l_i_offset, l_x_MocApduParameters.m_puc_ApduHeaderToSend_Last, APDU_HEADER_LENGTH); //ddAPDUheader_Last
			l_i_offset += APDU_HEADER_LENGTH;
			l_x_TemplateToCardBuffer.m_puc_Buffer[l_i_offset++] = (UC)l_x_MocApduParameters.m_x_PaddingType; //bPaddingType
			l_x_TemplateToCardBuffer.m_puc_Buffer[l_i_offset++] = (UC)l_x_MocApduParameters.m_x_CipheredType; //bCipherType
			l_x_TemplateToCardBuffer.m_puc_Buffer[l_i_offset++] = (UC)l_x_MocApduParameters.m_x_ChainType; //bChainType
			l_x_TemplateToCardBuffer.m_puc_Buffer[l_i_offset++] = (UC)l_x_MocApduParameters.m_x_DiversificationType; //bDiversificationType
			l_x_TemplateToCardBuffer.m_puc_Buffer[l_i_offset++] = (UC)l_x_MocApduParameters.m_i_DiversificationDataLen; //bDiversificationDataLength
			memcpy(l_x_TemplateToCardBuffer.m_puc_Buffer + l_i_offset, l_x_MocApduParameters.m_puc_DiversificationData, l_x_MocApduParameters.m_i_DiversificationDataLen); //ddDiversificationData
			l_i_offset += l_x_MocApduParameters.m_i_DiversificationDataLen;
			l_x_TemplateToCardBuffer.m_puc_Buffer[l_i_offset++] = (UC)l_x_MocApduParameters.m_i_DataToWriteBeforeTemplateLen; //bBeginTemplateHeaderLength
			memcpy(l_x_TemplateToCardBuffer.m_puc_Buffer + l_i_offset, l_x_MocApduParameters.m_puc_DataToWriteBeforeTemplate, l_x_MocApduParameters.m_i_DataToWriteBeforeTemplateLen); //bBeginTemplateHeaderData
			l_i_offset += l_x_MocApduParameters.m_i_DataToWriteBeforeTemplateLen;
			l_x_TemplateToCardBuffer.m_puc_Buffer[l_i_offset++] = (UC)l_x_MocApduParameters.m_i_DataToWriteAfterTemplateLen; //bEndTemplateHeaderLength
			memcpy(l_x_TemplateToCardBuffer.m_puc_Buffer + l_i_offset, l_x_MocApduParameters.m_puc_DataToWriteAfterTemplate, l_x_MocApduParameters.m_i_DataToWriteAfterTemplateLen); //bEndTemplateHeaderData
			l_i_offset += l_x_MocApduParameters.m_i_DataToWriteAfterTemplateLen;

			l_x_TemplateToCardBuffer.m_us_Size = l_i_offset;

			//Build CCID response
			l_x_TemplateToCardResponse.m_ul_Size = CCID_MAX_DATA_LENGTH;
			l_x_TemplateToCardResponse.m_puc_Buf = new UC[CCID_MAX_DATA_LENGTH];

			//Send Escape commande
			l_i_Ret = l_x_CardReader.SendEscapeCmd(l_x_TemplateToCardBuffer.m_puc_Buffer, l_x_TemplateToCardBuffer.m_us_Size, l_x_TemplateToCardResponse.m_puc_Buf, &(l_x_TemplateToCardResponse.m_ul_Size));

			//Memory free
			free(l_x_TemplateToCardBuffer.m_puc_Buffer);
			l_x_TemplateToCardBuffer.m_puc_Buffer = NULL;
			l_x_TemplateToCardBuffer.m_us_Size = 0;

			free(l_x_TemplateToCardResponse.m_puc_Buf);
			l_x_TemplateToCardResponse.m_puc_Buf = NULL;
			l_x_TemplateToCardResponse.m_ul_Size = 0;
		}
	}

	// Performs the Match On Card

	//cas d'ancien workflow via ILV 
	if ((l_i_Ret == MORPHO_OK) && (!(m_BioOpPSheet.m_MocPPage.m_b_FullCCID)))
	{
		T__APDU_IN			l_x_APDU;

		l_x_APDU.m_uc_Cla = l_x_MocApduParameters.m_puc_ApduToTrack[0];
		l_x_APDU.m_uc_Ins = l_x_MocApduParameters.m_puc_ApduToTrack[1];
		l_x_APDU.m_uc_P1 = l_x_MocApduParameters.m_puc_ApduToTrack[2];
		l_x_APDU.m_uc_P2 = l_x_MocApduParameters.m_puc_ApduToTrack[3];
		l_x_APDU.m_us_Le = 0;
		l_x_APDU.m_uc_Lc = l_x_MocApduParameters.m_puc_ApduToTrack[4];

		l_i_Ret = l_x_CardReader.SendAPDU(&l_x_APDU, l_x_MocApduParameters.m_puc_ApduToTrack + 5, l_x_MocApduParameters.m_i_ApduToTrackLen - 5, NULL, NULL);
	}

	else
	{
		//1ere solution simplifié avant implementation complète du protocole T=1 à enlever
		if (l_i_Ret == MORPHO_OK)
		{
			UC l_uc_SW1;
			UC l_uc_SW2;

			switch (m_BioOpPSheet.m_MocPPage.m_i_CardType)// SmartCard V1 / V2
			{
			case 2:
			case 3:
				Tool_Send_ChainedApdu_ToCard(&l_x_CardReader, &l_sCardCtx, &l_x_MocApduParameters, l_puc_DataTemplate, l_ul_LenTemplate, NULL, NULL);
				break;
			}
			l_x_CardReader.GetLastStatusBytesError(&l_uc_SW1, &l_uc_SW2);
			if (l_uc_SW1 != 0x90)
			{
				l_i_Ret = SMARTCARD_STATUS_BYTES_ERROR;
			}
		}
	}

	switch (m_BioOpPSheet.m_MocPPage.m_i_CardType)// SmartCard V1 / V2
	{
	case 2:
	case 3:
		Tool_MorphoParametersD2C_Free(&l_x_MocApduParameters);
		break;
	}

	if ((l_i_Ret <= SMARTCARD_BUSY) && (l_i_Ret >= SMARTCARD_BAD_IDENTIFIER_BUFFER_SIZE))
	{
		if (l_i_Ret == SMARTCARD_STATUS_BYTES_ERROR)
			l_x_ResultDlg.m_cs_Msg1.Format("SmartCard Error returned");
		else if (l_i_Ret == SMARTCARD_READER_ERROR)
		{
			l_x_ResultDlg.m_cs_Msg1.Format("SCard Reader Error returned");
			l_cs_Message.Format("FW Internal Error code = %d", l_i_InternalErrorCode);
			l_x_ResultDlg.m_cs_Msg4 = l_cs_Message;
		}
		else
			l_x_ResultDlg.m_cs_Msg1.Format("SmartCard access failed");
		l_x_ResultDlg.m_cs_Msg3.Format("Err N°0x%08X", l_i_Ret);
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_b_Ret = FALSE;
		l_x_CardReader.ConvertSCardError(l_i_Ret, "", l_x_ResultDlg.m_cs_Msg5);
	}
	else if (l_i_Ret == MORPHOERR_DEVICE_LOCKED)
	{
		l_b_runUnlockDeviceProcess = TRUE;
		l_b_Ret = FALSE;
	}
	else if (l_i_Ret != MORPHO_OK)
	{
		ShowAnimation("image_Ko");
		switch (l_i_Ret)
		{
		case MORPHOERR_STATUS:
			l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
			l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Device::Capture() function");
			ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
			if (m_BioOpPSheet.m_MocPPage.m_b_FullCCID)
				l_cs_Message.Format("Capture status = %d", l_uc_CaptureStatus);
			else
				l_cs_Message.Format("Internal Err = %d", m_x_MorphoSmart.GetInternalError());
			l_x_ResultDlg.m_cs_Msg4 += l_cs_Message;
			break;

		case MORPHOERR_NO_HIT:
			l_x_ResultDlg.m_cs_Msg2.Format("Bad Capture Sequence.");
			break;

		case MORPHOERR_CMDE_ABORTED:
			l_x_ResultDlg.m_cs_Msg2.Format("Command aborted by user.");
			break;

		case MORPHOERR_TIMEOUT:
			l_x_ResultDlg.m_cs_Msg2.Format("Timeout has expired.");
			l_x_ResultDlg.m_cs_Msg3.Format("Command aborted.");
			break;

		default:
			l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
			l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Device::Capture() function");
			ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
		}

		l_x_ResultDlg.m_cs_Msg1.Format("Capture Failed");
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_b_Ret = FALSE;
	}
	else
	{
		UC l_uc_SW1 = 0;
		UC l_uc_SW2 = 0;
		ShowAnimation("image_Ok");
		l_x_ResultDlg.m_cs_Msg1.Format("Capture Successful");
		l_x_ResultDlg.m_cs_Msg2.Format("Template successfully");
		if (m_BioOpPSheet.m_MocPPage.m_i_CaptureType == 1)
			l_x_ResultDlg.m_cs_Msg3.Format("Match on Smart Card Successful");
		else
			l_x_ResultDlg.m_cs_Msg3.Format("Store on Smart Card Successful");
		if (m_BioOpPSheet.m_MocPPage.m_b_FullCCID)
		{
		l_x_CardReader.GetLastStatusBytesError(&l_uc_SW1, &l_uc_SW2);
		l_x_ResultDlg.m_cs_Msg4.Format("CSCardReader SW1=0x%02X SW2=0x%02X", l_uc_SW1, l_uc_SW2);
		}
	}	

	m_b_BiometricOperationProcessing = FALSE;
	//Need to unlock locked device
	if (l_b_runUnlockDeviceProcess == TRUE)
	{
		I l_i_Ret2 = UnlockDevice();
		if (l_i_Ret2 != MORPHO_OK)
		{
			ShowErrorBox("An error occured during Device Unlocking", l_i_Ret2);
		}
	}
	if (m_b_EndThread == TRUE)
	{
		if (l_b_runUnlockDeviceProcess == FALSE)
		{
			l_x_ResultDlg.DoModal();
		}
		CleanAsyncMessages2();
	}

	if (m_px_Image != NULL)
	{
		delete(m_px_Image);
		m_px_Image = NULL;
	}

	if (m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive)
		PostMessage(WM_REMOVE_PROCESS_PAGE, 5, (LPARAM)&(m_BioOpPSheet.m_MocPPage));

	PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
	RedrawWnd();

	return !!l_b_Ret;
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnButtonCreateBase()
{
	CString l_cs_Message;

	/*l_cs_Message.Format("This command is going to create a default\ndatabase with the following parameters :\n\nNumber of fingers per record		: %d\nMaximum number of records		: %d\nNumber of supplementary fields	: %d",
	DB_NB_FINGERS,
	DB_NB_RECORDS,
	2
	);*/

	//AfxMessageBox(l_cs_Message, MB_ICONINFORMATION);

	if (CreateDatabase())
	{

		GetDlgItem(IDC_BUTTON_CREATE_BASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DESTROY_BASE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ADD_USER)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_NO_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_USER)->EnableWindow(TRUE);

		UpdateNumberOfRecords();
		m_BioOpPSheet.m_EnrollPPage.UpdateCtr = true;
		m_BioOpPSheet.DisplayPage(&(m_BioOpPSheet.m_EnrollPPage));

		m_BioOpPSheet.DisplayPage(&(m_BioOpPSheet.m_IdentifyPPage));

	}
}
//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnButtonDestroyBase()
{
	CString l_cs_Message;

	l_cs_Message.Format("This command is going to destroy the current database,\nand the whole content will be erased. Most biometrics operations\nwon't work anymore, unless you create a new base.\n\nConfirm ?");
	if (AfxMessageBox(l_cs_Message, MB_YESNO | MB_ICONQUESTION) != IDYES)
		return;

	if (DestroyDatabase())
	{
		GetDlgItem(IDC_BUTTON_CREATE_BASE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DESTROY_BASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ADD_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_NO_CHECK)->EnableWindow(FALSE);
		m_listCtrl.DeleteAllItems();
		GetDlgItem(IDC_LIST_USER)->EnableWindow(FALSE);
		UpdateNumberOfRecords();

		m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_EnrollPPage));
		m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_IdentifyPPage));
	}
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::UpdateNumberOfRecords()
{
	UL		l_ul_nbUsedRecord;
	I		l_i_Ret;
	CString	l_cs_Message;
	CBitmap BitmapBaseStatus;

	m_btm_BitmapBaseStatus.DeleteObject();

	l_i_Ret = m_x_Database.GetNbUsedRecord(l_ul_nbUsedRecord);

	if (l_i_Ret != MORPHO_OK)
	{
		if (l_i_Ret == MORPHOERR_BASE_NOT_FOUND)
		{
			l_ul_nbUsedRecord = 0;
			m_btm_BitmapBaseStatus.LoadBitmap(IDB_BITMAP_LRED);

		}
		else
		{
			ShowErrorBox("An error occured while calling C_MORPHO_Database::GetNbUsedRecord() function", l_i_Ret);
			return;
		}
	}
	else
	{
		if (l_ul_nbUsedRecord == 0)
		{
			m_btm_BitmapBaseStatus.LoadBitmap(IDB_BITMAP_LYELLOW);
		}
		else
		{
			m_btm_BitmapBaseStatus.LoadBitmap(IDB_BITMAP_LGREEN);
		}
	}


	m_ctl_BmpBaseStatus.SetBitmap((HBITMAP)m_btm_BitmapBaseStatus);

	m_CurrentNbRecords.Format("%d", l_ul_nbUsedRecord);
	m_tbCtrl.m_TabDbInfoDlg->SetParam("CurrentNbRecords", m_CurrentNbRecords);
	PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);

}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnClickOnPsheetArea()
{
	// RFU
}

//////////////////////////////////////////////////////////////////


void CMSO_DemoDlg::OnButtonAddUser()
{
	CString			l_cs_Message;
	CString			l_cs_OpenFile;
	UC				l_uc_indexTemplate;
	UL				l_ul_lenBuf;
	UC				l_uc_lenBuf;
	UC				l_puc_Buf[254];
	I				i;
	I				l_i_Ret;
	UC				l_auc_BufferFile[4096];
	CString			l_cs_FileExt;
	BOOL			l_b_Ret;
	CString			l_cs_FilesType;
	UC				l_uc_NbFingers;
	CResultDlg		l_x_ResultDlg;
	BOOL			l_b_tkb_or_pks = false;
	C_MORPHO_User	l_x_User;
	T_USER_DATA		l_x_UserData;

	UpdateData();

	l_auc_BufferFile[0] = '\0';
	DBaseCreateOption	l__base_option(this);
	m_x_Database.GetNbFinger(l_uc_NbFingers);

	BOOL isInPrivacyMode = (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED || m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED);

	//------------------------------------------------------------
	// Retrieve List of file
	//------------------------------------------------------------	


	l_cs_FilesType = g_x_BiometricFilesExtention;
	CFileDialog l_x_OpenFileDlg(TRUE,
		"",
		"",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT | OFN_ALLOWMULTISELECT,
		l_cs_FilesType,
		this);

	l_x_OpenFileDlg.m_ofn.lpstrFile = (PC)l_auc_BufferFile;
	l_x_OpenFileDlg.m_ofn.nMaxFile = 4096;

	SAV_PATH
		if (l_x_OpenFileDlg.DoModal() != IDOK)
		{
			RESTORE_PATH
				AfxMessageBox("Add User aborted. Press OK to continue...", MB_ICONINFORMATION);
			return;
		}
	RESTORE_PATH

		//---------------------------------------
		// Count number of files to process
		//---------------------------------------

	POSITION l_x_FilePos = l_x_OpenFileDlg.GetStartPosition();
	I	l_i_Counter = 0;

	while (l_x_FilePos != NULL)
	{
		l_cs_OpenFile = l_x_OpenFileDlg.GetNextPathName(l_x_FilePos);
		l_cs_FileExt = GetfileExtension(l_cs_OpenFile);

		if (l_i_Counter == 0)
		{
			if (!l_cs_FileExt.CompareNoCase("pks") || !l_cs_FileExt.CompareNoCase("tkb")) //tkb or pks template
			{
				l_b_tkb_or_pks = true;
			}
		}
		else
		{
			if (
				(!l_b_tkb_or_pks && (!l_cs_FileExt.CompareNoCase("pks") || !l_cs_FileExt.CompareNoCase("tkb")))
				||
				(l_b_tkb_or_pks && (l_cs_FileExt.CompareNoCase("pks") && l_cs_FileExt.CompareNoCase("tkb")))
				)
			{
				l_cs_Message.Format("You can not mix tkb or pks templates with standard templates.");
				AfxMessageBox(l_cs_Message);
				return;
			}
		}

		++l_i_Counter;
	}

	if (!l_b_tkb_or_pks && l_i_Counter > l_uc_NbFingers)
	{
		l_cs_Message.Format("The number of selected templates does not match the database format.");
		AfxMessageBox(l_cs_Message);
		return;
	}

	m_ctl_ProgressCtrl.SetRange(0, l_i_Counter * 3);

	//----------------------------------
	// Start Process
	//----------------------------------

	// Change cursor
	HCURSOR l_h_Cursor;
	l_h_Cursor = AfxGetApp()->LoadStandardCursor(IDC_WAIT);
	SetCursor(l_h_Cursor);

	// Disable Buttons
	DisableButtons();
	RedrawWnd();

	l_i_Ret = MORPHO_OK;

	UI l_i_Random = 0;
	std::string	sl_encrypt_Key;
	bool RetKey = false;

	if (!l_b_tkb_or_pks)
	{
		CUpdateUserDlg		l_x_UpdateUserDlg(TRUE);
		if (l_x_UpdateUserDlg.DoModal() != IDOK)
		{
			EnableButtons();
			return;
		}

		if (isInPrivacyMode)
		{
			l_i_Random = time(NULL);
			RetKey = GetEncryptedKey(sl_encrypt_Key);
			if (!RetKey)
			{
				AfxMessageBox("You must select a key for encryption.");
				EnableButtons();
				return;
			}
		}
		
		l_x_UserData.m_cs_ID = l_x_UpdateUserDlg.m_cs_ID;
		l_x_UserData.m_cs_Firstname = l_x_UpdateUserDlg.m_cs_Firstname;
		l_x_UserData.m_cs_Lastname = l_x_UpdateUserDlg.m_cs_Lastname;
		

		//--------------------------------------------------
		// Create User 
		//--------------------------------------------------

		if (l_i_Ret == MORPHO_OK)
		{
			l_uc_lenBuf = l_x_UserData.m_cs_ID.GetLength();

			if (isInPrivacyMode)
			{
				_PreparePrivacyData(l_x_UserData.m_cs_ID.GetBuffer(l_uc_lenBuf), l_uc_lenBuf, sl_encrypt_Key, l_i_Random, TRUE, l_puc_Buf, (PUL)&l_uc_lenBuf);
			}
			else
			{
				memcpy(l_puc_Buf, l_x_UserData.m_cs_ID.GetBuffer(l_uc_lenBuf), l_uc_lenBuf);
				l_puc_Buf[l_uc_lenBuf++] = 0;
			}

			l_i_Ret = m_x_Database.GetUser(
				l_uc_lenBuf,	// UC				i_uc_lenUserID,
				l_puc_Buf,		// PUC				i_puc_UserID,
				l_x_User			// C_MORPHO_User &	o_x_User 
				);
		}

		//--------------------------------------------------
		// Put Field : Firstname & Lastname
		//--------------------------------------------------

		//- - - - - - - - - - 
		// Firstname
		//- - - - - - - - - - 
		if (l_i_Ret == MORPHO_OK)
		{
			l_ul_lenBuf = l_x_UserData.m_cs_Firstname.GetLength();
			if (isInPrivacyMode)
			{
				_PreparePrivacyData(l_x_UserData.m_cs_Firstname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf, sl_encrypt_Key, l_i_Random, TRUE, l_puc_Buf, &l_ul_lenBuf);
			}
			else
			{
				memcpy(l_puc_Buf, l_x_UserData.m_cs_Firstname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf);
				l_puc_Buf[l_ul_lenBuf++] = 0;
			}

			l_i_Ret = l_x_User.PutField(
				1,						// UL		i_ul_indexField,
				l_ul_lenBuf,			// UL		i_ul_lenField,
				l_puc_Buf				// PUC		i_puc_dataField);
				);
		}
		//- - - - - - - - - - 
		// Lastname
		//- - - - - - - - - - 

		if (l_i_Ret == MORPHO_OK)
		{
			l_ul_lenBuf = l_x_UserData.m_cs_Lastname.GetLength();
			if (isInPrivacyMode)
			{
				_PreparePrivacyData(l_x_UserData.m_cs_Lastname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf, sl_encrypt_Key, l_i_Random, TRUE, l_puc_Buf, &l_ul_lenBuf);
			}
			else
			{
				memcpy(l_puc_Buf, l_x_UserData.m_cs_Lastname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf);
				l_puc_Buf[l_ul_lenBuf++] = 0;
			}

			l_i_Ret = l_x_User.PutField(
				2,						// UL		i_ul_indexField,
				l_ul_lenBuf,			// UL		i_ul_lenField,
				l_puc_Buf				// PUC		i_puc_dataField);
				);
		}

		//- - - - - - - - - - 
		// Error Case
		//- - - - - - - - - - 

		if (l_i_Ret != MORPHO_OK)
		{
			ShowErrorBox("An error occured while calling C_MORPHO_User::PutField() function", l_i_Ret);
		}
	}

	// Go !
	l_x_FilePos = l_x_OpenFileDlg.GetStartPosition();

	while (l_x_FilePos != NULL && l_i_Ret == MORPHO_OK)
	{
		l_cs_OpenFile = l_x_OpenFileDlg.GetNextPathName(l_x_FilePos);
		l_cs_FileExt = GetfileExtension(l_cs_OpenFile);

		// Retrieve Relevant data and store them in a structure					
		l_b_Ret = OpenFileAndExtractData(
			l_cs_OpenFile,
			l_cs_FileExt,
			&l_x_UserData);

		m_ctl_ProgressCtrl.SetStep(1);
		m_ctl_ProgressCtrl.StepIt();

		if (!l_b_tkb_or_pks)
		{
			if (l_x_UserData.m_uc_NbFingers > 1)
			{
				if (l_x_UserData.m_uc_FileType == MORPHO_PK_ANSI_378)
				{
					l_x_UserData.m_uc_NbFingers = 1;
				}
			}

			if (l_x_UserData.m_uc_NbFingers > l_uc_NbFingers)
			{
				l_cs_Message.Format("This file (%s) contains more than %d template", l_cs_OpenFile, l_uc_NbFingers);
				AfxMessageBox(l_cs_Message);
				FreeOpenFileAndExtractData(&l_x_UserData);
				l_i_Ret = -1;
				break;
			}

			if (l_i_Ret == MORPHO_OK)
			{
				if (!(l_cs_FileExt.CompareNoCase("fvp")))
				{
					l_i_Ret = l_x_User.PutFVPTemplate(
						l_x_UserData.m_uc_FVPFiletype,					// T_MORPHO_FVP_TYPE_TEMPLATE  i_uc_FVPFiletype;,
						(UL)l_x_UserData.m_px_Buffer[0].m_us_Size,		// UL			i_ul_lenTemplate,
						l_x_UserData.m_px_Buffer[0].m_puc_Buffer,		// PUC			i_puc_dataTemplate,
						l_uc_indexTemplate								// UC &			o_uc_indexTemplate);
						);
				}
				else
				{
					if (isInPrivacyMode)
					{
						UC* l_uc_encryptedTemplateData = new UC[l_x_UserData.m_px_Buffer[0].m_us_Size * 2];
						UL l_ul_encryptedTemplateDataSize = 0;

						_PreparePrivacyData((C*)l_x_UserData.m_px_Buffer[0].m_puc_Buffer, l_x_UserData.m_px_Buffer[0].m_us_Size, sl_encrypt_Key, l_i_Random, FALSE, l_uc_encryptedTemplateData, &l_ul_encryptedTemplateDataSize);

						l_i_Ret = l_x_User.PutTemplate(
							l_x_UserData.m_uc_FileType,						// T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
							l_ul_encryptedTemplateDataSize,		// UL			i_ul_lenTemplate,
							l_uc_encryptedTemplateData,		// PUC			i_puc_dataTemplate,
							l_uc_indexTemplate								// UC &			o_uc_indexTemplate);
							);
						
						if (l_uc_encryptedTemplateData) 
							delete l_uc_encryptedTemplateData;
					}
					else
						l_i_Ret = l_x_User.PutTemplate(
							l_x_UserData.m_uc_FileType,						// T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
							(UL)l_x_UserData.m_px_Buffer[0].m_us_Size,		// UL			i_ul_lenTemplate,
							l_x_UserData.m_px_Buffer[0].m_puc_Buffer,		// PUC			i_puc_dataTemplate,
							l_uc_indexTemplate								// UC &			o_uc_indexTemplate);
							);
				}
			}
		}
		else
		{
			//--------------------------------------------------
			// Create User 
			//--------------------------------------------------

			if (l_i_Ret == MORPHO_OK)
			{
				l_uc_lenBuf = l_x_UserData.m_cs_ID.GetLength();
				if (isInPrivacyMode)
				{
					_PreparePrivacyData(l_x_UserData.m_cs_ID.GetBuffer(l_uc_lenBuf), l_uc_lenBuf, sl_encrypt_Key, l_i_Random, TRUE, l_puc_Buf, (UL*)&l_uc_lenBuf);
				}
				else
				{
					memcpy(l_puc_Buf, l_x_UserData.m_cs_ID.GetBuffer(l_uc_lenBuf), l_uc_lenBuf);
					l_puc_Buf[l_uc_lenBuf++] = 0;
				}
				

				l_i_Ret = m_x_Database.GetUser(
					l_uc_lenBuf,	// UC				i_uc_lenUserID,
					l_puc_Buf,		// PUC				i_puc_UserID,
					l_x_User			// C_MORPHO_User &	o_x_User 
					);
			}

			//--------------------------------------------------
			// Put Field : Firstname & Lastname
			//--------------------------------------------------

			//- - - - - - - - - - 
			// Firstname
			//- - - - - - - - - - 
			if (l_i_Ret == MORPHO_OK)
			{
				l_ul_lenBuf = l_x_UserData.m_cs_Firstname.GetLength();
				if (isInPrivacyMode)
				{
					_PreparePrivacyData(l_x_UserData.m_cs_Firstname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf, sl_encrypt_Key, l_i_Random, TRUE, l_puc_Buf, &l_ul_lenBuf);
				}
				else
				{
					memcpy(l_puc_Buf, l_x_UserData.m_cs_Firstname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf);
					l_puc_Buf[l_ul_lenBuf++] = 0;
				}

				l_i_Ret = l_x_User.PutField(
					1,						// UL		i_ul_indexField,
					l_ul_lenBuf,			// UL		i_ul_lenField,
					l_puc_Buf				// PUC		i_puc_dataField);
					);
			}

			//- - - - - - - - - - 
			// Lastname
			//- - - - - - - - - - 
			if (l_i_Ret == MORPHO_OK)
			{
				l_ul_lenBuf = l_x_UserData.m_cs_Lastname.GetLength();
				if (isInPrivacyMode)
				{
					_PreparePrivacyData(l_x_UserData.m_cs_Lastname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf, sl_encrypt_Key, l_i_Random, TRUE, l_puc_Buf, &l_ul_lenBuf);
				}
				else
				{
					memcpy(l_puc_Buf, l_x_UserData.m_cs_Lastname.GetBuffer(l_ul_lenBuf), l_ul_lenBuf);
					l_puc_Buf[l_ul_lenBuf++] = 0;
				}

				l_i_Ret = l_x_User.PutField(
					2,						// UL		i_ul_indexField,
					l_ul_lenBuf,			// UL		i_ul_lenField,
					l_puc_Buf				// PUC		i_puc_dataField);
					);
			}

			//- - - - - - - - - - 
			// Error Case
			//- - - - - - - - - - 
			if (l_i_Ret != MORPHO_OK)
			{
				ShowErrorBox("An error occured while calling C_MORPHO_User::PutField() function", l_i_Ret);
			}

			if (l_i_Ret == MORPHO_OK && !l_cs_FileExt.CompareNoCase("tkb")) // template tkb
			{
				l_i_Ret = l_x_User.PutX984(
					l_x_UserData.m_px_pk_X984->m_us_Size,
					l_x_UserData.m_px_pk_X984->m_puc_Buffer
					);

				if (l_i_Ret != MORPHO_OK)
				{
					ShowErrorBox("C_MORPHO_TemplateList::PutX984() failed", l_i_Ret);
				}
			}
			else if (l_i_Ret == MORPHO_OK) // template pks
			{
				for (i = 0; i<l_x_UserData.m_uc_NbFingers; ++i)
				{

					if (l_i_Ret == MORPHO_OK)
					{
						if (!(l_cs_FileExt.CompareNoCase("fvp")))
						{
							l_i_Ret = l_x_User.PutFVPTemplate(
								l_x_UserData.m_uc_FVPFiletype,					// T_MORPHO_FVP_TYPE_TEMPLATE  i_uc_FVPFiletype;,
								(UL)l_x_UserData.m_px_Buffer[i].m_us_Size,		// UL			i_ul_lenTemplate,
								l_x_UserData.m_px_Buffer[i].m_puc_Buffer,		// PUC			i_puc_dataTemplate,
								l_uc_indexTemplate								// UC &			o_uc_indexTemplate);
								);
						}
						else
						{
							if (isInPrivacyMode)
							{
								UC* l_uc_encryptedTemplateData = new UC[l_x_UserData.m_px_Buffer[0].m_us_Size * 2];
								UL l_ul_encryptedTemplateDataSize = 0;

								_PreparePrivacyData((C*)l_x_UserData.m_px_Buffer[i].m_puc_Buffer, l_x_UserData.m_px_Buffer[i].m_us_Size, sl_encrypt_Key, l_i_Random, FALSE, l_uc_encryptedTemplateData, &l_ul_encryptedTemplateDataSize);

								l_i_Ret = l_x_User.PutTemplate(
									l_x_UserData.m_uc_FileType,						// T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
									l_ul_encryptedTemplateDataSize,		// UL			i_ul_lenTemplate,
									l_uc_encryptedTemplateData,		// PUC			i_puc_dataTemplate,
									l_uc_indexTemplate								// UC &			o_uc_indexTemplate);
									);

								if (l_uc_encryptedTemplateData)
									delete l_uc_encryptedTemplateData;
							}
							else
								l_i_Ret = l_x_User.PutTemplate(
									l_x_UserData.m_uc_FileType,						// T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
									(UL)l_x_UserData.m_px_Buffer[i].m_us_Size,		// UL			i_ul_lenTemplate,
									l_x_UserData.m_px_Buffer[i].m_puc_Buffer,		// PUC			i_puc_dataTemplate,
									l_uc_indexTemplate								// UC &			o_uc_indexTemplate);
									);
						}
					}
				}
			}

			m_ctl_ProgressCtrl.SetStep(1);
			m_ctl_ProgressCtrl.StepIt();

			if (l_i_Ret == MORPHO_OK)
			{
				//--------------------------------------------------
				// Add User To Database
				//--------------------------------------------------

				l_x_User.SetNoCheckOnTemplateForDBStore(m_b_NoCheck);

				l_i_Ret = l_x_User.DbStore();
			}
		}

		FreeOpenFileAndExtractData(&l_x_UserData);

		m_ctl_ProgressCtrl.SetStep(1);
		m_ctl_ProgressCtrl.StepIt();
	}

	if (!l_b_tkb_or_pks)
	{
		//--------------------------------------------------
		// Add User To Database
		//--------------------------------------------------

		l_x_User.SetNoCheckOnTemplateForDBStore(m_b_NoCheck);

		l_i_Ret = l_x_User.DbStore();

		m_ctl_ProgressCtrl.SetStep(1);
		m_ctl_ProgressCtrl.StepIt();
	}

	if (l_i_Ret != MORPHO_OK)
	{
		switch (l_i_Ret)
		{
		case MORPHOERR_ALREADY_ENROLLED:
			if (l_i_Counter == 1)
			{
				l_cs_Message.Format("The templates of this user are already in the database !\nWould you like to enroll him again with other fingers ?");
				if (AfxMessageBox(l_cs_Message, MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					l_cs_Message.Format("Choose 1 or 2 fingers in the Enroll Page and press start.");
					AfxMessageBox(l_cs_Message, MB_ICONINFORMATION);
					m_BioOpPSheet.m_EnrollPPage.m_cs_ID = l_x_UserData.m_cs_ID;
					m_BioOpPSheet.m_EnrollPPage.m_cs_Firstname = l_x_UserData.m_cs_Firstname;
					m_BioOpPSheet.m_EnrollPPage.m_cs_Lastname = l_x_UserData.m_cs_Lastname;
					UpdateData(TRUE);
					m_BioOpPSheet.SetActivePage(&(m_BioOpPSheet.m_EnrollPPage));
				}
			}
			else
			{
				l_cs_Message.Format("User %s %s (ID %s) is already enrolled.\nThe file won't be stored.", l_x_UserData.m_cs_Firstname, l_x_UserData.m_cs_Lastname, l_x_UserData.m_cs_ID);
				AfxMessageBox(l_cs_Message, MB_ICONERROR);
			}
			break;

		case MORPHOERR_DB_FULL:
			l_cs_Message.Format("Cannot add more user.\nDatabase is full.");
			AfxMessageBox(l_cs_Message, MB_ICONINFORMATION);
			break;

		case MORPHOERR_INVALID_USER_ID:
			l_cs_Message.Format("The ID %s is already used for a user in the Database.", l_x_UserData.m_cs_ID);
			AfxMessageBox(l_cs_Message);
			break;

		case MORPHOERR_LICENSE_MISSING:
			l_x_ResultDlg.m_cs_Msg1.Format("Failed to Add User");
			l_x_ResultDlg.m_cs_Msg2.Format("A required license is missing.");
			l_x_ResultDlg.DoModal();
			break;

		default:
			ShowErrorBox("An error occured while calling C_MORPHO_User::DbStore() function", l_i_Ret);
		}

		if (l_b_tkb_or_pks)
		{
			RefreshListCtrl(sl_encrypt_Key);
			UpdateNumberOfRecords();
		}
	}
	else
	{
		RefreshListCtrl(sl_encrypt_Key);
		UpdateNumberOfRecords();
	}
	if (!isInPrivacyMode)
		FreeOpenFileAndExtractData(&l_x_UserData);

	l_h_Cursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	SetCursor(l_h_Cursor);
	EnableButtons();
	m_ctl_ProgressCtrl.SetRange(0, 0);
	m_ctl_ProgressCtrl.StepIt();
}

void CMSO_DemoDlg::InitGuidingAnimations()
{
	HideAnimation();
	HideDirection();
	RedrawWnd();
}

void CMSO_DemoDlg::InitQualityBar()
{
	m_qualiteProgress.SetPos(0);
	m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(255, 255, 0));
	RedrawWnd();

}
void CMSO_DemoDlg::ShowAnimation(CString p_st_typeAnim)
{
	if (!m_bl_FVP)
	{
		return;
	}

	//CDC	*l_p_Cdc;
	CRect	myRect;
	if (p_st_typeAnim == "maillage")
	{
		GetDlgItem(IDC_STATIC_ANIMATION_PIC)->ShowWindow(SW_SHOW);
		m_vidPlayer[0]->Stop();
		m_vidPlayer[1]->Play(true);
	}
	else if (p_st_typeAnim == "capture")
	{
		GetDlgItem(IDC_STATIC_ANIMATION_PIC)->ShowWindow(SW_SHOW);
		m_vidPlayer[0]->Play(true);
		m_vidPlayer[1]->Stop();
	}
	else if (p_st_typeAnim == "image_Ok")
	{
		GetDlgItem(IDC_STATIC_ANIMATION_PIC)->ShowWindow(SW_SHOW);
		m_vidPlayer[0]->Stop();
		m_vidPlayer[1]->Stop();
		m_ctl_AnimationPic.SetBitmap(m_hBitmap_Ok);
	}
	else if (p_st_typeAnim == "image_Ko")
	{
		GetDlgItem(IDC_STATIC_ANIMATION_PIC)->ShowWindow(SW_SHOW);
		m_vidPlayer[0]->Stop();
		m_vidPlayer[1]->Stop();
		m_ctl_AnimationPic.SetBitmap(m_hBitmap_Ko);
	}
	//else
	//{
	//	if((l_uc_image == 1) && (m_bl_FVP == true))
	//	{
	//		////GetDlgItem(IDC_STATIC_VIEWER)->ShowWindow(SW_SHOW);
	//		//l_p_Cdc = GetDlgItem(IDC_STATIC_ANIMATION_PIC)->GetDC();
	//		//GetDlgItem(IDC_STATIC_ANIMATION_PIC)->GetClientRect(&myRect);	
	//		//m_px_Image->SetImage(	
	//		//		0,										// UL	i_ul_ImageIndex,				
	//		//		g_x_ImageStructure,						// T_MORPHO_CALLBACK_IMAGE_STATUS	i_x_Image,
	//		//		0,										// UL	i_ul_imageFilter,
	//		//		l_p_Cdc,								// CDC	*i_p_Cdc,
	//		//		myRect									// CRect &i_p_Rect
	//		//		);
	//		//
	//		//ReleaseDC(l_p_Cdc);
	//		m_cs_ImageInfo.Format ( "Size:%d*%d pix, Res:%d*%d dpi, %d bits/pixels", g_x_ImageStructure.m_x_ImageHeader.m_us_NbCol, g_x_ImageStructure.m_x_ImageHeader.m_us_NbRow, g_x_ImageStructure.m_x_ImageHeader.m_us_ResX, g_x_ImageStructure.m_x_ImageHeader.m_us_ResY, g_x_ImageStructure.m_x_ImageHeader.m_uc_NbBitsPerPixel );
	//		
	//		PostMessage(WM_UPDATE_DATA_FALSE,0,0);
	//		m_BioOpPSheet.m_pProcessPPage->PostMessage(WM_PAINT,0,0);
	//		l_uc_image = 0;
	//	}	
	//}
}

void CMSO_DemoDlg::HideAnimation()
{
	if (!m_bl_FVP)
	{
		return;
	}

	m_vidPlayer[0]->Stop();
	m_vidPlayer[1]->Stop();

	m_ctl_AnimationPic.SetBitmap(NULL);
}

void CMSO_DemoDlg::ShowDirection(int p_i_direction)
{
	GetDlgItem(IDC_STATIC_ASYNC_ENROLL_MSG)->ShowWindow(SW_HIDE);
	switch (p_i_direction)
	{
	case UP:
		m_ctl_AsyncEnrollDirectionPic.SetBitmap(m_hBitmap_Up);
		break;
	case RIGHT:
		m_ctl_AsyncEnrollDirectionPic.SetBitmap(m_hBitmap_Right);
		break;
	case DOWN:
		m_ctl_AsyncEnrollDirectionPic.SetBitmap(m_hBitmap_Down);
		break;
	case LEFT:
		m_ctl_AsyncEnrollDirectionPic.SetBitmap(m_hBitmap_Left);
		break;
	}
	GetDlgItem(IDC_STATIC_ASYNC_ENROLL_DIRECTION_PIC)->ShowWindow(SW_SHOW);

}
void CMSO_DemoDlg::HideDirection()
{
	GetDlgItem(IDC_STATIC_ASYNC_ENROLL_MSG)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_ASYNC_ENROLL_DIRECTION_PIC)->ShowWindow(SW_HIDE);
}
//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::DisableButtons()
{
	m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_EDIT_THRESHOLD)->EnableWindow(FALSE);
	m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_EDIT_TIMEOUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CREATE_BASE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DESTROY_BASE)->EnableWindow(FALSE);

	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_MAIN_VIEWER_ACTIVE)->EnableWindow(FALSE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_MINI_VIEWER_ACTIVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START)->SetWindowText("Stop");
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_VERIF_MATCH)->EnableWindow(FALSE);
	m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_FFD_SECU_LEVEL)->EnableWindow(FALSE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_EXPORT_NUM_PK)->EnableWindow(FALSE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_CONFIG_KEYUSER)->EnableWindow(FALSE);
	m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_CODER_CHOICE)->EnableWindow(FALSE);
	m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_BIO_MODE)->EnableWindow(FALSE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_WAKE_UP_MODE)->EnableWindow(FALSE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_ASYNC_COMMAND)->EnableWindow(FALSE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_ENROLL_COMMAND)->EnableWindow(FALSE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_ASYNC_DETECT_QUALITY)->EnableWindow(FALSE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_ASYNC_CODE_QUALITY)->EnableWindow(FALSE);
	m_tbCtrl.m_TabDbInfoDlg->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_NO_CHECK)->EnableWindow(FALSE);

	return TRUE;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::EnableButtons()
{
	unsigned long	l_ul_NbUsedRecords;

	m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_EDIT_THRESHOLD)->EnableWindow(TRUE);
	m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_EDIT_TIMEOUT)->EnableWindow(TRUE);


	if ((m_bl_BaseBio == false) && (m_bl_FVP == false))
	{
		GetDlgItem(IDC_BUTTON_CREATE_BASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CREATE_BASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ADD_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_NO_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(FALSE);
	}
	else
	{
		if (m_b_BaseAvailable)
		{
			GetDlgItem(IDC_BUTTON_DESTROY_BASE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ADD_USER)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_NO_CHECK)->EnableWindow(TRUE);

			m_x_Database.GetNbUsedRecord(l_ul_NbUsedRecords);

			if (l_ul_NbUsedRecords)
			{
				GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(TRUE);
			}
		}
		else
		{
			GetDlgItem(IDC_BUTTON_CREATE_BASE)->EnableWindow(TRUE);
		}
	}


	if ((m_tbCtrl.m_TabOptionsDlg->m_b_MainViewerActive) && !(m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive))
	{
		m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_MINI_VIEWER_ACTIVE)->EnableWindow(FALSE);

	}
	else
	{
		m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_MINI_VIEWER_ACTIVE)->EnableWindow(TRUE);
	}

	if ((m_tbCtrl.m_TabOptionsDlg->m_b_MainViewerActive) && (m_tbCtrl.m_TabOptionsDlg->m_b_MiniViewerActive))
	{
		m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_MAIN_VIEWER_ACTIVE)->EnableWindow(FALSE);
	}
	else
	{
		m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_MAIN_VIEWER_ACTIVE)->EnableWindow(TRUE);
	}

	GetDlgItem(IDC_BUTTON_START)->SetWindowText("Start");
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

	GetDlgItem(IDC_BUTTON_VERIF_MATCH)->EnableWindow(TRUE);
	m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_FFD_SECU_LEVEL)->EnableWindow(TRUE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_EXPORT_NUM_PK)->EnableWindow(TRUE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_CONFIG_KEYUSER)->EnableWindow(TRUE);
	m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_CODER_CHOICE)->EnableWindow(TRUE);
	m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_BIO_MODE)->EnableWindow(TRUE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_WAKE_UP_MODE)->EnableWindow(TRUE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_ASYNC_COMMAND)->EnableWindow(TRUE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_ENROLL_COMMAND)->EnableWindow(TRUE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_ASYNC_DETECT_QUALITY)->EnableWindow(TRUE);
	m_tbCtrl.m_TabOptionsDlg->GetDlgItem(IDC_CHECK_ASYNC_CODE_QUALITY)->EnableWindow(TRUE);

	m_tbCtrl.m_TabDbInfoDlg->EnableWindow(true);


	return TRUE;
}

//////////////////////////////////////////////////////////////////
LRESULT CMSO_DemoDlg::OnUpdateDataFalse(WPARAM wParam, LPARAM lParam)
{
	m_BioOpPSheet.GetActivePage()->UpdateData(FALSE);
	UpdateData(FALSE);
	return 0L;
}

//////////////////////////////////////////////////////////////////
LRESULT CMSO_DemoDlg::OnUpdateDataTrue(WPARAM wParam, LPARAM lParam)
{
	m_BioOpPSheet.GetActivePage()->UpdateData(TRUE);
	UpdateData(TRUE);
	return 0L;
}

//////////////////////////////////////////////////////////////////
LRESULT CMSO_DemoDlg::OnAddProcessPage(WPARAM wParam, LPARAM lParam)
{
	m_BioOpPSheet.m_pProcessPPage = new CPPageProcess(this);
	m_BioOpPSheet.AddPage(m_BioOpPSheet.m_pProcessPPage);
	m_BioOpPSheet.SetActivePage(m_BioOpPSheet.m_pProcessPPage);
	return 0L;
}

//////////////////////////////////////////////////////////////////
LRESULT CMSO_DemoDlg::OnRemoveProcessPage(WPARAM wParam, LPARAM lParam)
{
	CPropertyPage* l_px_PPage;

	m_BioOpPSheet.RemovePage(m_BioOpPSheet.m_pProcessPPage);
	delete(m_BioOpPSheet.m_pProcessPPage);
	m_BioOpPSheet.m_pProcessPPage = NULL;

	l_px_PPage = (CPropertyPage*)lParam;
	m_BioOpPSheet.SetActivePage(l_px_PPage);
	return 0L;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::ExtractCertif(
	CString	i_cs_FileToOpen,
	PUL		o_pul_CertifSize,
	PUC*		o_ppuc_Certif
	)
{
	CFile		l_x_File;
	CString		l_cs_Message;
	T_TKB_HDR	l_x_TkbHdr;
	PUC			l_puc_Certif;

	// the .tkb file structure : 
	// _ the .tkb Header File (see the T_TKB_HDR structure)
	// _ the X984 Biometric Token
	// _ the X509 Mso certificate

	if (!l_x_File.Open(i_cs_FileToOpen, CFile::modeReadWrite | CFile::typeBinary))
	{
		l_cs_Message.Format("Cannot open file %s", i_cs_FileToOpen);
		AfxMessageBox(l_cs_Message);
		return FALSE;
	}


	if (l_x_File.GetLength()<sizeof(T_TKB_HDR))
	{
		AfxMessageBox("bad file .tkb");
		return FALSE;
	}

	l_x_File.Read(&l_x_TkbHdr, sizeof(T_TKB_HDR));

	if (l_x_TkbHdr.m_ul_MagicNb != MAGIC_NB_DEMO || l_x_TkbHdr.m_ul_Version != TKB_FILE_VERSION)
	{
		AfxMessageBox("bad file .tkb");
		return FALSE;
	}

	l_x_File.Seek(sizeof(T_TKB_HDR) + l_x_TkbHdr.m_ul_SizeTkb, CFile::begin);

	l_puc_Certif = (PUC)malloc(l_x_TkbHdr.m_ul_SizeCertif);

	l_x_File.Read(l_puc_Certif, l_x_TkbHdr.m_ul_SizeCertif);

	*o_ppuc_Certif = l_puc_Certif;
	*o_pul_CertifSize = l_x_TkbHdr.m_ul_SizeCertif;
	return TRUE;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::ExtractAndStoreCertif(CString i_cs_FileToOpen)
{
	CFile		l_x_File;
	CString		l_cs_Message;
	PUC			l_puc_Certif;
	I			l_i_Ret;
	UL			l_ul_CertifSize;

	if (!ExtractCertif(
		i_cs_FileToOpen,	//CString	i_cs_FileToOpen,
		&l_ul_CertifSize,	//PUL		o_pul_CertifSize,
		&l_puc_Certif		//PUC*		o_ppuc_Certif
		))
	{
		return FALSE;
	}

	//With this certificate, The MSO can verify the token signature.
	l_i_Ret = m_x_MorphoSmart.SecuStoCertif(l_ul_CertifSize, l_puc_Certif);

	free(l_puc_Certif);

	if (l_i_Ret)
	{
		ShowErrorBox("C_MORPHO_Device::SecuStoCertif failed", l_i_Ret);
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
VOID CMSO_DemoDlg::FreeOpenFileAndExtractData(
	T_USER_DATA *o_px_UserData)
{
	UC	i;

	if (o_px_UserData->m_px_Buffer)
	{
		for (i = 0; i<o_px_UserData->m_uc_NbFingers; ++i)
		{
			if (o_px_UserData->m_px_Buffer[i].m_puc_Buffer)
			{
				free(o_px_UserData->m_px_Buffer[i].m_puc_Buffer);
				o_px_UserData->m_px_Buffer[i].m_puc_Buffer = NULL;
			}
		}

		free(o_px_UserData->m_px_Buffer);
		o_px_UserData->m_px_Buffer = NULL;
	}

	if (o_px_UserData->m_px_pk_X984)
	{
		if (o_px_UserData->m_px_pk_X984->m_puc_Buffer != NULL)
		{
			free(o_px_UserData->m_px_pk_X984->m_puc_Buffer);
			o_px_UserData->m_px_pk_X984->m_puc_Buffer = NULL;
		}
		free(o_px_UserData->m_px_pk_X984);
		o_px_UserData->m_px_pk_X984 = NULL;
	}

	o_px_UserData->m_uc_NbFingers = 0;
	o_px_UserData->m_cs_Firstname = "";
	o_px_UserData->m_uc_FileType = MORPHO_NO_PK_FP;
	o_px_UserData->m_uc_FVPFiletype = MORPHO_NO_PK_FVP;
	o_px_UserData->m_cs_ID = "";
	o_px_UserData->m_cs_Lastname = "";
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::OpenFileAndExtractData(
	CString		i_cs_FileToOpen,
	CString		i_cs_FileExt,
	T_USER_DATA *o_px_UserData
	)
{
	CFile					FileToOpen;
	CString					l_cs_Message;
	US						l_us_Size;
	UL						l_ul_Pos;
	UC						l_auc_Buffer[1024];
	CString					l_cs_ID;
	CString					l_cs_Firstname;
	CString					l_cs_Lastname;
	UL						l_ul_lenTemplate;
	I						i;
	CString					l_cs_Ext;
	C_MORPHO_TemplateList	l_x_TemplateList;
	UL						l_ul_DataSize;
	PUC						l_puc_Data;
	I						l_i_Ret;
	T_TKB_HDR				l_x_TkbHdr;



	o_px_UserData->m_px_Buffer = NULL;
	o_px_UserData->m_px_pk_X984 = NULL;
	o_px_UserData->m_uc_FileType = MORPHO_NO_PK_FP;
	o_px_UserData->m_uc_FVPFiletype = MORPHO_NO_PK_FVP;
	o_px_UserData->m_uc_NbFingers = 0;


	l_puc_Data = NULL;
	l_i_Ret = 0;

	//------------------------------------------------------------
	// Open File and retrieve data
	//------------------------------------------------------------

	o_px_UserData->m_uc_NbFingers = 1;

	if (!FileToOpen.Open(i_cs_FileToOpen, CFile::modeRead | CFile::typeBinary))
	{
		l_cs_Message.Format("Cannot open file %s", i_cs_FileToOpen);
		AfxMessageBox(l_cs_Message, MB_ICONERROR);
		return FALSE;
	}

	if (!(i_cs_FileExt.CompareNoCase("pks")))
	{
		// Format of the .pks structure :
		// - Size ID		: 2 bytes
		// - ID				: <Size ID> bytes
		// - Size Firstname : 2 bytes
		// - Firstname		: <Size Firstname> bytes
		// - Size Lastname	: 2 bytes
		// - Lastname		: <Size Lastname> bytes
		// - Nb of templates: 1 byte (1 or 2 fingers)
		// - Size 1st Tplate: 2 bytes
		// - 1st Template	: <Size 1st Tplate> bytes
		// - Size 2nd Tplate: 2 bytes					(if exists)
		// - 2nd Template	: <Size 2nd Tplate> bytes	(if exists)

		//- - - - - - - - - - - - - - - - - - - - - - - - -
		// Retrieve ID
		//- - - - - - - - - - - - - - - - - - - - - - - - -

		o_px_UserData->m_uc_NbFingers = 0;
		l_ul_Pos = 0;
		FileToOpen.Read((PC)&(l_us_Size), 2);
		l_ul_Pos += 2;
		FileToOpen.Seek((L)l_ul_Pos, CFile::begin);
		FileToOpen.Read((PC)l_auc_Buffer, l_us_Size);
		l_ul_Pos += l_us_Size;
		l_auc_Buffer[l_us_Size] = 0;
		o_px_UserData->m_cs_ID.Format("%s", l_auc_Buffer);

		//- - - - - - - - - - - - - - - - - - - - - - - - -
		// Retrieve Firstname
		//- - - - - - - - - - - - - - - - - - - - - - - - -

		FileToOpen.Seek((L)l_ul_Pos, CFile::begin);
		FileToOpen.Read((PC)&(l_us_Size), 2);
		l_ul_Pos += 2;
		FileToOpen.Seek((L)l_ul_Pos, CFile::begin);
		FileToOpen.Read((PC)l_auc_Buffer, l_us_Size);
		l_ul_Pos += l_us_Size;
		l_auc_Buffer[l_us_Size] = 0;
		o_px_UserData->m_cs_Firstname.Format("%s", l_auc_Buffer);

		//- - - - - - - - - - - - - - - - - - - - - - - - -
		// Retrieve Lastname
		//- - - - - - - - - - - - - - - - - - - - - - - - -

		FileToOpen.Seek((L)l_ul_Pos, CFile::begin);
		FileToOpen.Read((PC)&(l_us_Size), 2);
		l_ul_Pos += 2;
		FileToOpen.Seek((L)l_ul_Pos, CFile::begin);
		FileToOpen.Read((PC)l_auc_Buffer, l_us_Size);
		l_ul_Pos += l_us_Size;
		l_auc_Buffer[l_us_Size] = 0;
		o_px_UserData->m_cs_Lastname.Format("%s", l_auc_Buffer);

		//- - - - - - - - - - - - - - - - - - - - - - - - -
		// Retrieve Pks 
		//- - - - - - - - - - - - - - - - - - - - - - - - -

		FileToOpen.Seek((L)l_ul_Pos, CFile::begin);
		FileToOpen.Read((PC)&(o_px_UserData->m_uc_NbFingers), 1);
		l_ul_Pos += 1;

		o_px_UserData->m_px_Buffer = (PT_BUFFER)malloc(o_px_UserData->m_uc_NbFingers * sizeof(T_BUFFER));

		for (i = 0; i<o_px_UserData->m_uc_NbFingers; ++i)
		{
			FileToOpen.Seek((L)l_ul_Pos, CFile::begin);
			FileToOpen.Read((PC)&(l_us_Size), 2);
			l_ul_Pos += 2;
			l_ul_lenTemplate = l_us_Size;

			FileToOpen.Seek((L)l_ul_Pos, CFile::begin);
			FileToOpen.Read((PC)l_auc_Buffer, l_us_Size);
			l_ul_Pos += l_us_Size;

			o_px_UserData->m_px_Buffer[i].m_us_Size = l_us_Size;
			o_px_UserData->m_px_Buffer[i].m_puc_Buffer = (PUC)malloc(l_us_Size);

			memcpy(o_px_UserData->m_px_Buffer[i].m_puc_Buffer, l_auc_Buffer, l_us_Size);
		}
		o_px_UserData->m_uc_FileType = MORPHO_PK_COMP;
	}

	else if (!(i_cs_FileExt.CompareNoCase("tkb")))
	{
		// the .tkb file structure : 
		// _ the .tkb Header File (see the T_TKB_HDR structure)
		// _ the X984 Biometric Token
		// _ the X509 Mso certificate

		if (FileToOpen.GetLength()<sizeof(T_TKB_HDR))
		{
			AfxMessageBox("bad file .tkb");
			return FALSE;
		}

		l_ul_Pos = 0;
		FileToOpen.Read(&l_x_TkbHdr, sizeof(T_TKB_HDR));
		l_ul_Pos += sizeof(T_TKB_HDR);

		FileToOpen.Seek((L)l_ul_Pos, CFile::begin);

		if (l_x_TkbHdr.m_ul_MagicNb != MAGIC_NB_DEMO || l_x_TkbHdr.m_ul_Version != TKB_FILE_VERSION)
		{
			AfxMessageBox("bad file .tkb");
			return FALSE;
		}

		o_px_UserData->m_px_pk_X984 = (PT_BUFFER)malloc(sizeof(T_BUFFER));

		if (o_px_UserData->m_px_pk_X984 != NULL)
		{
			o_px_UserData->m_px_pk_X984->m_us_Size = (US)l_x_TkbHdr.m_ul_SizeTkb;
			o_px_UserData->m_px_pk_X984->m_puc_Buffer = (PUC)malloc(o_px_UserData->m_px_pk_X984->m_us_Size);

			// Correction fait par C++Test [BD-PB-NP-1] 
			FileToOpen.Read(o_px_UserData->m_px_pk_X984->m_puc_Buffer, o_px_UserData->m_px_pk_X984->m_us_Size);

			l_i_Ret = l_x_TemplateList.ExtractDataX984(
				o_px_UserData->m_px_pk_X984->m_us_Size,
				o_px_UserData->m_px_pk_X984->m_puc_Buffer,
				l_ul_DataSize,
				l_puc_Data);
		}

		if (l_i_Ret || (l_puc_Data == NULL))
		{
			ShowErrorBox("C_MORPHO_TemplateList::ExtractDataX984 failed", l_i_Ret);
			return FALSE;
		}


		o_px_UserData->m_uc_FileType = MORPHO_NO_PK_FP;
		o_px_UserData->m_uc_FVPFiletype = MORPHO_NO_PK_FVP;

		if (*((PI)l_puc_Data) == MAGIC_NB_DEMO)
		{
			// Format of the data :
			// - Magic Number	: 4 bytes
			// - Size ID		: 2 bytes
			// - ID				: <Size ID> bytes
			// - Size Firstname : 2 bytes
			// - Firstname		: <Size Firstname> bytes
			// - Size Lastname	: 2 bytes
			// - Lastname		: <Size Lastname> bytes

			//- - - - - - - - - - - - - - - - - - - - - - - - -
			// Retrieve ID
			//- - - - - - - - - - - - - - - - - - - - - - - - -

			l_ul_Pos = 4;
			memcpy(&l_us_Size, l_puc_Data + l_ul_Pos, sizeof(US));
			l_ul_Pos += 2;
			memcpy(l_auc_Buffer, l_puc_Data + l_ul_Pos, l_us_Size);
			l_ul_Pos += l_us_Size;
			l_auc_Buffer[l_us_Size] = 0;
			o_px_UserData->m_cs_ID.Format("%s", l_auc_Buffer);

			//- - - - - - - - - - - - - - - - - - - - - - - - -
			// Retrieve Firstname
			//- - - - - - - - - - - - - - - - - - - - - - - - -

			memcpy(&l_us_Size, l_puc_Data + l_ul_Pos, sizeof(US));
			l_ul_Pos += 2;
			memcpy(l_auc_Buffer, l_puc_Data + l_ul_Pos, l_us_Size);
			l_ul_Pos += l_us_Size;
			l_auc_Buffer[l_us_Size] = 0;
			o_px_UserData->m_cs_Firstname.Format("%s", l_auc_Buffer);

			//- - - - - - - - - - - - - - - - - - - - - - - - -
			// Retrieve Lastname
			//- - - - - - - - - - - - - - - - - - - - - - - - -

			memcpy(&l_us_Size, l_puc_Data + l_ul_Pos, sizeof(US));
			l_ul_Pos += 2;
			memcpy(l_auc_Buffer, l_puc_Data + l_ul_Pos, l_us_Size);
			l_ul_Pos += l_us_Size;
			l_auc_Buffer[l_us_Size] = 0;
			o_px_UserData->m_cs_Lastname.Format("%s", l_auc_Buffer);

		}
		else
		{
			o_px_UserData->m_cs_ID.Format("Unknown data");
			o_px_UserData->m_cs_Firstname.Format("Unknown data");
			o_px_UserData->m_cs_Lastname.Format("Unknown data");
		}
	}
	else
	{
		l_ul_Pos = 0;
		o_px_UserData->m_px_Buffer = (PT_BUFFER)malloc(o_px_UserData->m_uc_NbFingers * sizeof(T_BUFFER));

		l_ul_lenTemplate = (UL)FileToOpen.GetLength();
		// Correction fait par C++Test [BD-PB-NP-1] 
		if (o_px_UserData->m_px_Buffer != NULL)
		{
			o_px_UserData->m_px_Buffer[0].m_us_Size = (US)l_ul_lenTemplate;
			o_px_UserData->m_px_Buffer[0].m_puc_Buffer = (PUC)malloc(o_px_UserData->m_px_Buffer[0].m_us_Size);

			FileToOpen.Seek((L)l_ul_Pos, CFile::begin);
			FileToOpen.Read((PC)o_px_UserData->m_px_Buffer[0].m_puc_Buffer, o_px_UserData->m_px_Buffer[0].m_us_Size);

			// Retrieve the real number of fingers in ANSI FMR 378 template file (can be > 1)
			if (!(i_cs_FileExt.CompareNoCase("ansi-fmr")) && (o_px_UserData->m_px_Buffer->m_puc_Buffer != NULL))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_ANSI_378;
				o_px_UserData->m_uc_NbFingers = *(o_px_UserData->m_px_Buffer->m_puc_Buffer + ANSI_FMR_378_PK_NUMBER_OFFSET);
			}
			else if (!(i_cs_FileExt.CompareNoCase("ansi-fmr-2009")) && (o_px_UserData->m_px_Buffer->m_puc_Buffer != NULL))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_ANSI_378_2009;
				o_px_UserData->m_uc_NbFingers = *(o_px_UserData->m_px_Buffer->m_puc_Buffer + ANSI_FMR_378_2009_PK_NUMBER_OFFSET);
			}
			else if (!(i_cs_FileExt.CompareNoCase("pkcn")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_COMP_NORM;
			}
			else if (!(i_cs_FileExt.CompareNoCase("pkm")) || !(i_cs_FileExt.CompareNoCase("pkmat")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_MAT;
			}
			else if (!(i_cs_FileExt.CompareNoCase("pkmn")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_MAT_NORM;
			}
			else if (!(i_cs_FileExt.CompareNoCase("minex-a")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_MINEX_A;
			}
			else if (!(i_cs_FileExt.CompareNoCase("iso-fmc-ns")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_ISO_FMC_NS;
			}
			else if (!(i_cs_FileExt.CompareNoCase("iso-fmc-cs")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_ISO_FMC_CS;
			}
			else if (!(i_cs_FileExt.CompareNoCase("iso-fmr")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_ISO_FMR;
			}
			else if (!(i_cs_FileExt.CompareNoCase("iso-fmr-2011")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_ISO_FMR_2011;
			}
			else if (!(i_cs_FileExt.CompareNoCase("pkV10")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_V10;
			}
			else if (!(i_cs_FileExt.CompareNoCase("moc")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_MOC;
			}
			else if (!(i_cs_FileExt.CompareNoCase("din-cs")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_DIN_V66400_CS;
			}
			else if (!(i_cs_FileExt.CompareNoCase("din-cs-aa")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_DIN_V66400_CS_AA;
			}
			else if (!(i_cs_FileExt.CompareNoCase("fvp")))
			{
				o_px_UserData->m_uc_FVPFiletype = MORPHO_PK_FVP;
			}
			// FVP 1.6
			else if (!(i_cs_FileExt.CompareNoCase("fvp-m")))
			{
				o_px_UserData->m_uc_FVPFiletype = MORPHO_PK_FVP_MATCH;
			}
			else if (!(i_cs_FileExt.CompareNoCase("pkl")) || !(i_cs_FileExt.CompareNoCase("pklite")))
			{
				o_px_UserData->m_uc_FileType = MORPHO_PK_PKLITE;
			}
			else{
				o_px_UserData->m_uc_FileType = MORPHO_PK_COMP;
				o_px_UserData->m_uc_FVPFiletype = MORPHO_NO_PK_FVP;
			}
		}
	}
	FileToOpen.Close();

	return TRUE;
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnClose()
{
	int l_i_ThreadIndex;
	int	l_i_Ret;

	if (m_b_BiometricOperationProcessing)
	{
		m_x_MorphoSmart.CancelLiveAcquisition();
		m_b_BiometricOperationProcessing = FALSE;
	}

	m_b_EndThread = FALSE;

	for (l_i_ThreadIndex = 0; l_i_ThreadIndex <NB_THREADS; ++l_i_ThreadIndex)
	{
		if (m_ppx_Threads[l_i_ThreadIndex] != NULL)
		{
			if (m_ppx_Threads[l_i_ThreadIndex]->m_hThread != NULL)
			{
				l_i_Ret = WaitForSingleObject(m_ppx_Threads[l_i_ThreadIndex]->m_hThread, TIMEOUT_THREAD * 3);

				if (l_i_Ret != WAIT_FAILED)
				{
					CloseHandle(m_ppx_Threads[l_i_ThreadIndex]->m_hThread);
				}
			}
		}
	}

	if (m_ctl_CurrentMiniViewer != NULL)
	{
		PostMessage(WM_REMOVE_PROCESS_PAGE, 0, (LPARAM)&(m_BioOpPSheet.m_CodePPage));
		m_ctl_CurrentMiniViewer = NULL;
	}

	if (m_px_Image != NULL)
	{
		delete(m_px_Image);
		m_px_Image = NULL;
	}

	if (m_vidPlayer)
	{
		if (m_vidPlayer[0])
		{
			delete m_vidPlayer[0];
			m_vidPlayer[0] = NULL;
		}

		if (m_vidPlayer[1])
		{
			delete m_vidPlayer[1];
			m_vidPlayer[1] = NULL;
		}
	}

	if (m_uc_SecuConfig&SECU_TUNNELING)
	{
		m_x_MorphoSmart.TunnelingClose();
	}
	else if (m_uc_SecuConfig&SECU_OFFERED_SECURITY)
	{
		m_x_MorphoSmart.OfferedSecuClose();
	}

	m_x_MorphoSmart.CloseDevice();
	m_menu.DestroyMenu();

	OnOK();

	CDialog::OnClose();
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnButtonClose()
{
	OnClose();
	//	OnOK();
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnCancelMode()
{
	CDialog::OnCancelMode();

	// TODO: Add your message handler code here	
}

//////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
//void CMSO_DemoDlg::OnCheckMiniViewerActive() 
//{
//	UpdateData();
//	if(m_b_MiniViewerActive)
//	{
//		m_tbCtrl.m_TabOptionsDlg->m_b_MainViewerActive = TRUE;
//		m_cs_StaticViewer.Format("");
//	}
//	UpdateData(FALSE);
//}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::GetUserList(C_MORPHO_UserList & o_x_UserList)
{
	UC		l_auc_FieldIndexDescriptor[MORPHO_NB_FIELD_MAX_BIT];
	I		l_i_Ret;
	CString	l_cs_Message;

	//::::::::::::::::::::::
	// Index Descriptor
	//::::::::::::::::::::::

	if (GetIndexDescriptor(l_auc_FieldIndexDescriptor))
	{
		//::::::::::::::::::::::
		// Read Public Fields
		//::::::::::::::::::::::

		l_i_Ret = m_x_Database.ReadPublicFields(
			l_auc_FieldIndexDescriptor,	// UC	i_auc_FieldIndexDescriptor[MORPHO_NB_FIELD_MAX/8],	// binary mask with all desired fields
			o_x_UserList				// C_MORPHO_UserList &	o_x_UserList
			);
		if (l_i_Ret == MORPHOERR_USER_NOT_FOUND)
		{
			l_i_Ret = MORPHO_OK;
		}
		if (l_i_Ret != MORPHO_OK)
		{
			switch (l_i_Ret)
			{
			case MORPHOERR_FIELD_NOT_FOUND:
				// DO NOTHING: We don't consider this as an error,
				// the empty fields will just be set to "<None>" in
				// the list view.
				break;
			default:
				ShowErrorBox("An error occured while calling C_MORPHO_Database::ReadPublicFields() function", l_i_Ret);
				return FALSE;
			}
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::GetIndexDescriptor(UC io_auc_FieldIndexDescriptor[MORPHO_NB_FIELD_MAX / 8])
{
	I		l_i_Ret;
	CString	l_cs_Message;

	l_i_Ret = m_x_Database.FillIndexDescriptor(
		TRUE,							//BOOL	l_b_Erase,
		0,								//UC		l_i_index,									
		io_auc_FieldIndexDescriptor);	//UC		io_auc_FieldIndexDescriptor[MORPHO_NB_FIELD_MAX/8]);	

	if (l_i_Ret == MORPHO_OK)
		l_i_Ret = m_x_Database.FillIndexDescriptor(
		FALSE,							//BOOL	l_b_Erase,
		1,								//UC		l_i_index,									
		io_auc_FieldIndexDescriptor);	//UC		io_auc_FieldIndexDescriptor[MORPHO_NB_FIELD_MAX/8]);	

	if (l_i_Ret == MORPHO_OK)
		l_i_Ret = m_x_Database.FillIndexDescriptor(
		FALSE,							//BOOL	l_b_Erase,
		2,								//UC		l_i_index,									
		io_auc_FieldIndexDescriptor);	//UC		io_auc_FieldIndexDescriptor[MORPHO_NB_FIELD_MAX/8]);	

	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling C_MORPHO_Database::FillIndexDescriptor() function", l_i_Ret);
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::InitializeListCtrl()
{
	//::::::::::::::::::::::::
	// Initialize list Ctrl
	//::::::::::::::::::::::::

	m_listCtrl.InsertColumn(0, "ID", LVCFMT_LEFT, 80);
	m_listCtrl.InsertColumn(1, "Firstname", LVCFMT_LEFT, 76);
	m_listCtrl.InsertColumn(2, "Lastname", LVCFMT_LEFT, 88);
	m_listCtrl.SetExtendedStyle(m_listCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::RefreshListCtrl(std::string	_sl_encrypt_Key)
{
	I		l_i_Ret;
	CString	l_cs_Message;
	CString			l_cs_Item;
	C_MORPHO_UserList l_x_UserList;
	C_MORPHO_User*	l_px_User;
	I				l_i_FieldIndex;
	UL				l_ul_LenField;
	PUC				l_puc_DataField;
	UC				l_puc_GenericBuf[100]; //Abd 25
	I				l_i_Indice;
	UL				i, j;
	UL				l_ul_NbUser;

	m_listCtrl.DeleteAllItems();

	//- - - - - - - - - - - 
	// Retrieve Field List
	//- - - - - - - - - - - 

	if (GetUserList(l_x_UserList))
	{
		//- - - - - - - - - - - - - - - - - -
		// Fill List Ctrl on the PC Screen
		//- - - - - - - - - - - - - - - - - - 

		//::::::::::::::::::::::::
		// Get number of user
		//::::::::::::::::::::::::			

		l_i_Ret = l_x_UserList.GetNbUser(
			l_ul_NbUser		// UL &	o_ul_NbUser
			);

		if (l_i_Ret != MORPHO_OK)
		{
			ShowErrorBox("An error occured while calling C_MORPHO_UserList::GetNbUser() function", l_i_Ret);
			return FALSE;
		}

		//:::::::::::::::::::::::::::::::::::
		// Display ID, Firstname, Lastname 
		//:::::::::::::::::::::::::::::::::::

		//Abd decrypt
		PUC o_ppuc_Aes128CbcDeCryptedData = NULL;
		UL	o_pul_Aes128CbcDeCryptedDataLen = 0;
		bool _decrypt = false;
		std::string	sl_encrypt_Key="";
		bool RetKey = false;
		
		UL index = 0, dataSize = 0, crc32 = 0, random = 0;
		unsigned char	l_auc_kcv[KCV_LEN] = { 0 };

		// check Privacy KCV
		UL l_ul_nbUsedRecord = 0;
		UL l_i_ret = m_x_MorphoSmart.GetKCV(ID_KPRIVACY, NULL, l_auc_kcv);
		l_i_Ret = m_x_Database.GetNbUsedRecord(l_ul_nbUsedRecord);
		BOOL DB_Privacy_Key = FALSE;

		if ((l_i_ret == MORPHO_OK) && (l_ul_nbUsedRecord>0) && ((m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED) || (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED)) )
		{
			DB_Privacy_Key = TRUE;
		}

		if (DB_Privacy_Key)
		{
			if (_sl_encrypt_Key!="")
			{
				sl_encrypt_Key = _sl_encrypt_Key;
				_decrypt = true;
				m_IsPrivacyListClear = TRUE;
			}
			else
			if (AfxMessageBox("Privacy Mode : Database contains crypted data !\nDatabase shall be decrypted so as data can be displayed, do you want to proceed with decryption ?", MB_YESNO) == IDYES)
				{
					RetKey = GetEncryptedKey(sl_encrypt_Key);
					if (!RetKey)
					{
						AfxMessageBox("Error occured when loading encryption key.");
						GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
						GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
						GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
						m_BioOpPSheet.m_VerifyPPage.GetDlgItem(IDC_RADIO_LOCALBASE)->EnableWindow(FALSE);
						m_IsPrivacyListClear = FALSE;
						return false;
					}
					if (RetKey) 
						_decrypt = true;
					m_IsPrivacyListClear = TRUE;
				}
				else 
				{
					GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
					m_BioOpPSheet.m_VerifyPPage.GetDlgItem(IDC_RADIO_LOCALBASE)->EnableWindow(FALSE);
					m_IsPrivacyListClear = FALSE;
				}
		}
		
		for (i = 0; i<l_ul_NbUser; ++i)
		{
			l_i_Ret = l_x_UserList.GetUser(
				i,			// UL			i_ul_UserIndex, 
				l_px_User	// C_MORPHO_User* &	o_px_User
				);

			if (l_i_Ret != MORPHO_OK)
			{
				ShowErrorBox("An error occured while calling C_MORPHO_UserList::GetUser() function", l_i_Ret);
				return FALSE;
			}

			for (j = 0; j <= 2; ++j)
			{
				l_i_FieldIndex = j;
				l_i_Ret = l_px_User->GetField(
					l_i_FieldIndex,		//I			i_i_FieldIndex,
					l_ul_LenField,		//UL		o_ul_LenField,
					l_puc_DataField		//PUC		o_puc_DataField	
					);

				if (l_i_Ret != MORPHO_OK)
				{
					switch (l_i_Ret)
					{
					case MORPHOERR_FIELD_NOT_FOUND:
						l_cs_Item.Format("");
						break;
					default:
						ShowErrorBox("An error occured while calling C_MORPHO_User::GetField() function", l_i_Ret);
						return FALSE;
					}
				}
				else
				{
					if (DB_Privacy_Key)
					{
						if (_decrypt)
						{	
							o_ppuc_Aes128CbcDeCryptedData = NULL;
							o_pul_Aes128CbcDeCryptedDataLen = 0;
							bool b_decRet = _DecryptAESData(l_puc_DataField, l_ul_LenField, &o_ppuc_Aes128CbcDeCryptedData, o_pul_Aes128CbcDeCryptedDataLen, sl_encrypt_Key);
						
							if (b_decRet) 
							{
								memcpy(&crc32, o_ppuc_Aes128CbcDeCryptedData, 4); // Get CRC32
								memcpy(&random, o_ppuc_Aes128CbcDeCryptedData + 4, 4); // GET Random
								memcpy(&index, o_ppuc_Aes128CbcDeCryptedData + 8, 4); // Get Index
								memcpy(&dataSize, o_ppuc_Aes128CbcDeCryptedData + 12, 4); // Get Size

								memcpy(l_puc_GenericBuf, o_ppuc_Aes128CbcDeCryptedData + 16, dataSize);
								l_puc_GenericBuf[dataSize] = 0;
							}
							if (NULL != o_ppuc_Aes128CbcDeCryptedData)
								MSOSECU_Free(&o_ppuc_Aes128CbcDeCryptedData);
						}
						else
						{
							l_puc_GenericBuf[0] = 0;
						}
					}
					else
					{
						memcpy(l_puc_GenericBuf, l_puc_DataField, l_ul_LenField);
						l_puc_GenericBuf[l_ul_LenField] = 0;
					}

					l_cs_Item.Format("%s", l_puc_GenericBuf);
				}

				if (j == 0)
				{
					l_i_Indice = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), l_cs_Item);
				}
				else
				{
					if (!DB_Privacy_Key)
					{
						if ((l_cs_Item.Compare("") == 0) || (l_cs_Item.Compare(" ") == 0))
							l_cs_Item.Format("<None>");
					}

					m_listCtrl.SetItemText(l_i_Indice, j, l_cs_Item);
				}
			}
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::CheckIfOneUserSelectedInList(CString &o_cs_ID)
{
	POSITION		l_pos;
	I				l_i_Indice;

	//---------------------------------------------
	// Check if a user is selected in the list 
	//---------------------------------------------

	if (m_listCtrl.GetSelectedCount() > 1)
	{
		AfxMessageBox("Too many records selected.");
		return FALSE;
	}

	if (m_listCtrl.GetSelectedCount() == 0)
	{
		AfxMessageBox("Select a user in the list view.");
		return FALSE;
	}

	l_pos = m_listCtrl.GetFirstSelectedItemPosition();
	l_i_Indice = m_listCtrl.GetNextSelectedItem(l_pos);
	o_cs_ID = m_listCtrl.GetItemText((int)l_i_Indice, 0);

	return TRUE;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::ReadFile(CString &i_cs_FileToOpen, PUC *o_ppuc_buffer, PI o_pi_len)
{
	CFile   l_x_file;

	if (l_x_file.Open(i_cs_FileToOpen.GetBuffer(0), CFile::modeRead))
	{
		*o_pi_len = (I)l_x_file.GetLength();
		*o_ppuc_buffer = (PUC)malloc((*o_pi_len) + 1);
		if (l_x_file.Read(*o_ppuc_buffer, *o_pi_len) && (*o_ppuc_buffer != NULL))
		{
			*(*o_ppuc_buffer + (*o_pi_len)) = 0;
			l_x_file.Close();
		}
		else
			return false;
	}
	else
		return false;

	return true;

}
//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::GetFilesNameToOpen(
	CString i_cs_DefaultFileName,
	CString &o_cs_FileToOpen,
	CString &o_cs_FileExt,
	CString i_cs_FilesType,
	bool i_b_allowMultiSelect = false)
{
	UL l_ul_flag;

	if (i_b_allowMultiSelect)
	{
		l_ul_flag = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
	}
	else
	{
		l_ul_flag = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	}

	CFileDialog l_x_OpenFileDlg(TRUE,
		"",
		i_cs_DefaultFileName,
		l_ul_flag,
		i_cs_FilesType,
		this);

	SAV_PATH

		if (l_x_OpenFileDlg.DoModal() != IDOK)
		{
			RESTORE_PATH
				return FALSE;
		}
	RESTORE_PATH

		o_cs_FileToOpen = l_x_OpenFileDlg.GetPathName();
	o_cs_FileExt = l_x_OpenFileDlg.GetFileExt();

	return TRUE;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::GetFilesNameToOpen(
	CString i_cs_DefaultFileName,
	CString &o_cs_Folder,
	CString &o_cs_FileToOpen,
	CString &o_cs_FileExt,
	CString i_cs_FilesType,
	bool i_b_allowMultiSelect = false)
{
	UL l_ul_flag;

	if (i_b_allowMultiSelect)
	{
		l_ul_flag = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
	}
	else
	{
		l_ul_flag = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	}

	CFileDialog l_x_OpenFileDlg(TRUE,
		"",
		i_cs_DefaultFileName,
		l_ul_flag,
		i_cs_FilesType,
		this);

	SAV_PATH

		if (l_x_OpenFileDlg.DoModal() != IDOK)
		{
			RESTORE_PATH
				return FALSE;
		}
	RESTORE_PATH

		o_cs_Folder = l_x_OpenFileDlg.GetFolderPath();
	o_cs_FileToOpen = l_x_OpenFileDlg.GetFileTitle();
	o_cs_FileExt = l_x_OpenFileDlg.GetFileExt();

	return TRUE;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::FillTemplateListFromUserStruct(T_USER_DATA & i_x_UserData, C_MORPHO_TemplateList & o_x_TemplateList, UC i_uc_Index, BOOL i_b_isPrivacy, std::string i_s_privacy_key, I i_i_random)
{
	I		i;
	I		l_i_Ret;
	B		l_b_RetPrepare;
	CString l_cs_Message;
	UC		l_uc_indexTemplate;
	UC		l_puc_Buf[1024];
	UL		l_ul_lenBuf = 0;

	for (i = 0; i< i_x_UserData.m_uc_NbFingers; i++)
	{
		if (i_x_UserData.m_uc_FVPFiletype == MORPHO_PK_FVP || i_x_UserData.m_uc_FVPFiletype == MORPHO_PK_FVP_MATCH)
		{
			if (i_b_isPrivacy)
			{
				l_b_RetPrepare = _PreparePrivacyData((PC)i_x_UserData.m_px_Buffer[i].m_puc_Buffer,
					i_x_UserData.m_px_Buffer[i].m_us_Size,
					i_s_privacy_key, i_i_random, FALSE, l_puc_Buf, &l_ul_lenBuf);

				if (l_b_RetPrepare)
				{
					l_i_Ret = o_x_TemplateList.PutFVPTemplate(
						i_x_UserData.m_uc_FVPFiletype,								// T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
						l_ul_lenBuf,		// UL			i_ul_lenTemplate,
						l_puc_Buf,	// PUC			i_puc_dataTemplate,
						l_uc_indexTemplate							// UC &			o_uc_indexTemplate);
						);
				}
				else l_i_Ret = MORPHOERR_INTERNAL;
			}
			else
			{
				l_i_Ret = o_x_TemplateList.PutFVPTemplate(
					i_x_UserData.m_uc_FVPFiletype,								// T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
					i_x_UserData.m_px_Buffer[i].m_us_Size,		// UL			i_ul_lenTemplate,
					i_x_UserData.m_px_Buffer[i].m_puc_Buffer,	// PUC			i_puc_dataTemplate,
					l_uc_indexTemplate							// UC &			o_uc_indexTemplate);
					);
			}

		}
		else if (i_x_UserData.m_uc_FileType != MORPHO_NO_PK_FP)
		{
			if (i_b_isPrivacy)
			{
				l_b_RetPrepare = _PreparePrivacyData((PC)i_x_UserData.m_px_Buffer[i].m_puc_Buffer,
					i_x_UserData.m_px_Buffer[i].m_us_Size,
					i_s_privacy_key, i_i_random, FALSE, l_puc_Buf, &l_ul_lenBuf);

				if (l_b_RetPrepare)
				{
					l_i_Ret = o_x_TemplateList.PutTemplate(
						i_x_UserData.m_uc_FileType,								// T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
						l_ul_lenBuf,		// UL			i_ul_lenTemplate,
						l_puc_Buf,	// PUC			i_puc_dataTemplate,
						i_uc_Index,
						l_uc_indexTemplate							// UC &			o_uc_indexTemplate);
						);
				}
				else l_i_Ret = MORPHOERR_INTERNAL;
			}
			else
			{
				l_i_Ret = o_x_TemplateList.PutTemplate(
					i_x_UserData.m_uc_FileType,								// T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
					i_x_UserData.m_px_Buffer[i].m_us_Size,		// UL			i_ul_lenTemplate,
					i_x_UserData.m_px_Buffer[i].m_puc_Buffer,	// PUC			i_puc_dataTemplate,
					i_uc_Index,
					l_uc_indexTemplate							// UC &			o_uc_indexTemplate);
					);
			}
		}
		else
		{
			break;
		}

		if (l_i_Ret != MORPHO_OK)
		{
			ShowErrorBox("An error occured while calling C_MORPHO_TemplateList::PutTemplate() function", l_i_Ret);
			return FALSE;
		}
	}
	if (i_x_UserData.m_px_pk_X984)
	{
		if (i_b_isPrivacy)
		{
			l_b_RetPrepare = _PreparePrivacyData((PC)i_x_UserData.m_px_pk_X984->m_puc_Buffer,
				i_x_UserData.m_px_pk_X984->m_us_Size,
				i_s_privacy_key, i_i_random, FALSE, l_puc_Buf, &l_ul_lenBuf);

			if (l_b_RetPrepare)
			{
				l_i_Ret = o_x_TemplateList.PutX984(
					l_ul_lenBuf,
					l_puc_Buf
					);
			}
			else l_i_Ret = MORPHOERR_INTERNAL;
		}
		else
		{
			l_i_Ret = o_x_TemplateList.PutX984(
				i_x_UserData.m_px_pk_X984->m_us_Size,
				i_x_UserData.m_px_pk_X984->m_puc_Buffer
				);
		}
		if (l_i_Ret != MORPHO_OK)
		{
			ShowErrorBox("An error occured while calling C_MORPHO_TemplateList::PutX984() function", l_i_Ret);
			return FALSE;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::CleanAsyncMessages()
{
	//------------------------------------------------
	// Remove eventual "Garbage" Asynchronous Message 		
	m_cs_AsyncCmdMsg = "";
	m_cs_AsyncEnrollMsg = "";
	m_cs_QualityMsg = "";
	PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
	//------------------------------------------------
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::CleanAsyncMessages2()
{
	//------------------------------------------------
	// Remove eventual "Garbage" Asynchronous Message 	
	if (m_BioOpPSheet.m_pProcessPPage != NULL)
	{
		m_BioOpPSheet.m_pProcessPPage->m_cs_AsyncCmdMsg = "";
	}
	m_cs_ImageInfo = "";
	PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
	//------------------------------------------------
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::RedrawWnd()
{
	//AfxMessageBox("test 1.");
	RedrawWindow();
	//AfxMessageBox("test 2.");
	//OnPaint();
	//AfxMessageBox("test 3.");
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnButtonIdentMatch()
{
	C_MORPHO_User			l_x_User;
	C_MORPHO_TemplateList	l_x_TemplateList;
	I						l_i_Ret;
	CString					l_cs_Message;
	CFile					FileToOpen;
	T_USER_DATA				l_x_UserData;
	CString					l_cs_OpenFile;
	CResultDlg				l_x_ResultDlg;
	CString					l_cs_FileExt;
	UC						l_uc_NbFingers = 0;
	CString					l_cs_FilesType;

	//------------------------------------------------------------
	// Privacy Preparation
	//------------------------------------------------------------
	I						l_i_Random;
	std::string				sl_encrypt_Key;
	BOOL					isPrivacyEnabled = (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED || m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED);

	if (isPrivacyEnabled)
	{
		l_i_Random = time(NULL);
		l_i_Ret = GetEncryptedKey(sl_encrypt_Key);
		if (!l_i_Ret)
		{
			AfxMessageBox("You must select a key for encryption.");
			return;
		}
	}

	//------------------------------------------------------------
	// Fill Template List 
	//------------------------------------------------------------

	//- - - - - - - - - - - - - - - - - - - - - - - - -
	// Retrieve File Name to open and Extract Data
	//- - - - - - - - - - - - - - - - - - - - - - - - -


	l_cs_FilesType = g_x_BiometricFilesExtention;
	if (!GetFileAndExtractData(
		"",							//default file name		CString		i_cs_DefaultFileName, 										
		l_cs_FilesType,				//						CString		i_cs_FilesType,
		l_cs_OpenFile,				//file name				CString		&o_cs_FileToOpen,
		l_cs_FileExt,				//file name extension	CString		&o_cs_FileExt,
		&l_x_UserData				//information			T_USER_DATA *o_px_UserData)
		))
	{
		return;
	}


	if (l_x_UserData.m_uc_NbFingers>1)
	{
		MessageBox("This file contains two templates.\nOnly the first one will be used\nfor the identification.\n\nPress OK to continue...", "Identify Match", MB_ICONINFORMATION);
		if (l_x_UserData.m_uc_FileType == MORPHO_PK_ANSI_378)
		{
			l_uc_NbFingers = 1;
		}
		else
		{
			l_uc_NbFingers = l_x_UserData.m_uc_NbFingers;	// need to remember the actual nb of fingers for the memory deallocation of l_x_UserData.
		}
		l_x_UserData.m_uc_NbFingers = 1;				// IdentMatch only works with one pk. We pretend we just have one here.
	}

	if (!FillTemplateListFromUserStruct(l_x_UserData, l_x_TemplateList, 0xFF, isPrivacyEnabled, sl_encrypt_Key, l_i_Random))
	{
		if (l_uc_NbFingers)
		{
			l_x_UserData.m_uc_NbFingers = l_uc_NbFingers;
		}
		FreeOpenFileAndExtractData(&l_x_UserData);
		return;
	}

	//------------------------------------------------------------
	// Identify Match
	//------------------------------------------------------------

	l_x_ResultDlg.m_cs_Msg1.Format("The file data :");
	l_x_ResultDlg.m_cs_Msg2.Format("Firstname : %s", l_x_UserData.m_cs_Firstname);
	l_x_ResultDlg.m_cs_Msg3.Format("Lastname : %s", l_x_UserData.m_cs_Lastname);
	l_x_ResultDlg.m_cs_Msg4.Format("ID : %s", l_x_UserData.m_cs_ID);
	l_x_ResultDlg.DoModal();

	//-----------------------------------------------------------
	// Get security Level 
	//-----------------------------------------------------------
	GetSecurityLevel(0);
	this->m_x_MorphoSmart.SetSecurityLevel(m_i_SecurityLevel);
	//The MSO doesn't accept several PK, we take the first one only.
	l_x_TemplateList.SetPkX984Index(1);
	l_i_Ret = m_x_Database.IdentifyMatch(
		(T_MORPHO_FAR)m_tbCtrl.m_TabBioSettingsDlg->m_i_Threshold,	// T_MORPHO_FAR			i_us_FAR,	
		l_x_TemplateList,				// C_MORPHO_TemplateList	i_x_TemplateList,
		l_x_User						// C_MORPHO_User &			o_x_User
		);

	if (l_i_Ret == MORPHOERR_DEVICE_LOCKED)
	{
		l_i_Ret = UnlockDevice();
		if (l_i_Ret != MORPHO_OK)
		{
			ShowErrorBox("An error occured during Device Unlocking", l_i_Ret);
		}
	}
	else if (l_i_Ret != MORPHO_OK)
	{
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_x_ResultDlg.m_cs_Msg1.Format("Identification Failed");
		switch (l_i_Ret)
		{
		case MORPHOERR_NO_HIT:
			l_x_ResultDlg.m_cs_Msg2.Format("Person not Identified.");
			break;
		case MORPHOERR_LICENSE_MISSING:
			l_x_ResultDlg.m_cs_Msg2.Format("A required license is missing.");
			l_x_ResultDlg.m_cs_Msg3.Format("-");
			l_x_ResultDlg.m_cs_Msg4.Format("-");
			break;
		default:
			l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
			l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Database::IdentifyMatch() function");
			ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
		}

		l_x_ResultDlg.DoModal();
	}
	else
	{
		ShowUserData(l_x_User);
	}

	if (l_uc_NbFingers)
	{
		l_x_UserData.m_uc_NbFingers = l_uc_NbFingers;
	}

	FreeOpenFileAndExtractData(&l_x_UserData);
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnButtonVerifMatch()
{
	_VerifMatchWithFile();
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::_VerifMatchWithFile()
{
	I						l_i_Ret;
	CString					l_cs_Message;
	CFile					FileToOpen;
	T_USER_DATA				l_x_UserData;
	CString					l_cs_OpenFile;
	C_MORPHO_TemplateList	l_x_TemplateList1;
	C_MORPHO_TemplateList	l_x_TemplateList2;
	CResultDlg				l_x_ResultDlg;
	CString					l_cs_FileExt;
	UC						l_uc_NbFingers = 0;
	CString					l_cs_FilesType;
	CString					l_cs_listfile[MAX_TEMPLATE];
	T_INFO_USER				l_x_ListinfoUsers[MAX_TEMPLATE];
	I						l_nb_file;
	//------------------------------------------------------------
	// Privacy Preparation
	//------------------------------------------------------------
	I						l_i_Random;
	std::string				sl_encrypt_Key;
	BOOL					isPrivacyEnabled = (m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_ENABLED || m_PrivacyModeStatus == C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED);

	if (isPrivacyEnabled)
	{
		l_i_Random = time(NULL);
		l_i_Ret = GetEncryptedKey(sl_encrypt_Key);
		if (!l_i_Ret)
		{
			AfxMessageBox("You must select a key for encryption.");
			return FALSE;
		}
	}
	//------------------------------------------------------------
	// Template List 1
	//------------------------------------------------------------

	l_cs_FilesType = g_x_BiometricFilesExtention;
	//- - - - - - - - - - - - - - - - - - - - - - - - -
	// Retrieve File Name to open and Extract Data
	//- - - - - - - - - - - - - - - - - - - - - - - - -

	if (!GetFileAndExtractData(
		"",							//default file name		CString		i_cs_DefaultFileName, 										
		l_cs_FilesType,				//						CString		i_cs_FilesType,
		l_cs_OpenFile,				//file name				CString		&o_cs_FileToOpen,
		l_cs_FileExt,				//file name extension	CString		&o_cs_FileExt,
		&l_x_UserData				//information			T_USER_DATA *o_px_UserData)
		))
	{
		return FALSE;
	}



	if (l_x_UserData.m_uc_NbFingers > 1)
	{
		MessageBox("This file contains two templates.\nOnly the first one will be used\nfor the verification.\n\nPress OK to continue...", "Verify Match", MB_ICONINFORMATION);
		if (l_x_UserData.m_uc_FileType == MORPHO_PK_ANSI_378)
		{
			l_uc_NbFingers = 1;
		}
		else
		{
			l_uc_NbFingers = l_x_UserData.m_uc_NbFingers;	// need to remember the actual nb of fingers for the memory deallocation of l_x_UserData.
		}
		l_x_UserData.m_uc_NbFingers = 1;				// IdentMatch only works with one pk. We pretend we just have one here.
	}

	if (!FillTemplateListFromUserStruct(l_x_UserData, l_x_TemplateList1, 0, isPrivacyEnabled, sl_encrypt_Key, l_i_Random))
	{
		if (l_uc_NbFingers)
		{
			l_x_UserData.m_uc_NbFingers = l_uc_NbFingers;
		}
		FreeOpenFileAndExtractData(&l_x_UserData);
		return FALSE;
	}

	if (l_uc_NbFingers)
	{
		l_x_UserData.m_uc_NbFingers = l_uc_NbFingers;
	}

	FreeOpenFileAndExtractData(&l_x_UserData);

	//------------------------------------------------------------
	// Template List 2 : Reference templates list
	//------------------------------------------------------------

	if (!GetFilesAndExtarctTemplates(l_cs_FilesType, l_cs_listfile, l_x_ListinfoUsers, l_nb_file, l_x_TemplateList2, isPrivacyEnabled, sl_encrypt_Key, l_i_Random))
	{
		return FALSE;
	}

	//------------------------------------------------------------
	// Verify Match
	//------------------------------------------------------------

	//-----------------------------------------------------------
	// Get security Level 
	//-----------------------------------------------------------
	GetSecurityLevel(0);
	this->m_x_MorphoSmart.SetSecurityLevel(m_i_SecurityLevel);

	//The MSO doesn't accept several PK. Only the first one is used.
	l_x_TemplateList1.SetPkX984Index(1);

	l_i_Ret = m_x_MorphoSmart.VerifyMatch(
		(T_MORPHO_FAR)m_tbCtrl.m_TabBioSettingsDlg->m_i_Threshold,	// T_MORPHO_FAR			i_us_FAR,
		l_x_TemplateList1,				// C_MORPHO_TemplateList &	i_x_TemplateList1,
		l_x_TemplateList2				// C_MORPHO_TemplateList &	i_x_TemplateList2
		);


	if (l_i_Ret == MORPHOERR_DEVICE_LOCKED)
	{
		l_i_Ret = UnlockDevice();
		if (l_i_Ret != MORPHO_OK)
		{
			ShowErrorBox("An error occured during Device Unlocking", l_i_Ret);
			//return FALSE;
		}
		return FALSE;
	}
	else if (l_i_Ret != MORPHO_OK)
	{

		switch (l_i_Ret)
		{
			//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		case MORPHOERR_NO_HIT:
			l_x_ResultDlg.m_cs_Msg1.Format("Authentication Failed");
			l_x_ResultDlg.m_cs_Msg2.Format("Person not authenticated.");
			l_x_ResultDlg.DoModal();
			break;
		case MORPHOERR_LICENSE_MISSING:
			l_x_ResultDlg.m_cs_Msg1.Format("Verification Failed");
			l_x_ResultDlg.m_cs_Msg2.Format("A required license is missing.");
			l_x_ResultDlg.DoModal();
			break;
		default:
			ShowErrorBox("An error occured while calling C_MORPHO_Device::Verify() function", l_i_Ret);
		}

		return FALSE;
	}

	Beep(BEEP_FREQUENCY, BEEP_DURATION);
	l_x_ResultDlg.m_cs_Msg1.Format("User authenticated");
	l_x_ResultDlg.m_cs_Msg2.Format("Firstname : %s", l_x_UserData.m_cs_Firstname);
	l_x_ResultDlg.m_cs_Msg3.Format("Lastname : %s", l_x_UserData.m_cs_Lastname);
	l_x_ResultDlg.m_cs_Msg4.Format("ID : %s", l_x_UserData.m_cs_ID);
	l_x_ResultDlg.DoModal();

	return TRUE;
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::SortListCtrl
(
int i_i_ColumnNumber,	// column to process
bool i_b_Ascending,		// sort order
int i_i_LowRow,			// row to start scanning from, default : 0
int i_i_HighRow			// row to end scan, -1 =  last row
)
{

	//----------------------------------------------
	// Init
	//----------------------------------------------

	if (i_i_ColumnNumber >= ((CListCtrl*)GetDlgItem(IDC_LIST_USER))->GetItemCount())
	{
		return FALSE;
	}

	if (i_i_HighRow == -1)
	{
		i_i_HighRow = m_listCtrl.GetItemCount() - 1;
	}

	int l_i_TempLowRow = i_i_LowRow;
	int l_i_TempHighRow = i_i_HighRow;
	CString l_cs_MiddleItem;

	if (l_i_TempHighRow <= l_i_TempLowRow)
	{
		return FALSE;
	}

	//----------------------------------------------
	// Find Middle Item
	//----------------------------------------------

	l_cs_MiddleItem = m_listCtrl.GetItemText((l_i_TempLowRow + l_i_TempHighRow) / 2, i_i_ColumnNumber);

	//----------------------------------------------
	// Loop through the list until indices cross
	//----------------------------------------------

	while (l_i_TempLowRow <= l_i_TempHighRow)
	{
		CStringArray l_cs_RowText;

		//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// Find the first element that is greater than or equal to 
		// the partition element starting from the left Index.
		//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		if (i_b_Ascending)
		{
			while ((l_i_TempLowRow < i_i_HighRow) && (m_listCtrl.GetItemText(l_i_TempLowRow, i_i_ColumnNumber) < l_cs_MiddleItem))
			{
				l_i_TempLowRow++;
			}
		}
		else
		{
			while ((l_i_TempLowRow < i_i_HighRow) && (m_listCtrl.GetItemText(l_i_TempLowRow, i_i_ColumnNumber) > l_cs_MiddleItem))
			{
				l_i_TempLowRow++;
			}
		}

		//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// Find an element that is smaller than or equal to 
		// the partition element starting from the right Index.
		//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		if (i_b_Ascending)
		{
			while ((l_i_TempHighRow > i_i_LowRow) && (m_listCtrl.GetItemText(l_i_TempHighRow, i_i_ColumnNumber) > l_cs_MiddleItem))
			{
				l_i_TempHighRow--;
			}
		}
		else
		{
			while ((l_i_TempHighRow > i_i_LowRow) && (m_listCtrl.GetItemText(l_i_TempHighRow, i_i_ColumnNumber) < l_cs_MiddleItem))
			{
				l_i_TempHighRow--;
			}
		}

		//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// If the indexes have not crossed, swap
		// and if the items are not equal
		//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		if (l_i_TempLowRow <= l_i_TempHighRow)
		{
			// swap only if the items are not equal
			if (m_listCtrl.GetItemText(l_i_TempLowRow, i_i_ColumnNumber) != m_listCtrl.GetItemText(l_i_TempHighRow, i_i_ColumnNumber))
			{
				// swap the rows
				LV_ITEM lvitemlo, lvitemhi;

				int l_i_ColumnNumberCount =
					((CListCtrl*)GetDlgItem(IDC_LIST_USER))->GetItemCount();
				l_cs_RowText.SetSize(l_i_ColumnNumberCount);
				int i;
				for (i = 0; i<l_i_ColumnNumberCount; i++)
					l_cs_RowText[i] = m_listCtrl.GetItemText(l_i_TempLowRow, i);
				lvitemlo.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
				lvitemlo.iItem = l_i_TempLowRow;
				lvitemlo.iSubItem = 0;
				lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED |
					LVIS_FOCUSED | LVIS_SELECTED |
					LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;

				lvitemhi = lvitemlo;

				lvitemhi.iItem = l_i_TempHighRow;

				m_listCtrl.GetItem(&lvitemlo);
				m_listCtrl.GetItem(&lvitemhi);

				for (i = 0; i<l_i_ColumnNumberCount; i++)
					m_listCtrl.SetItemText(l_i_TempLowRow, i, m_listCtrl.GetItemText(l_i_TempHighRow, i));

				lvitemhi.iItem = l_i_TempLowRow;
				m_listCtrl.SetItem(&lvitemhi);

				for (i = 0; i<l_i_ColumnNumberCount; i++)
					m_listCtrl.SetItemText(l_i_TempHighRow, i, l_cs_RowText[i]);

				lvitemlo.iItem = l_i_TempHighRow;
				m_listCtrl.SetItem(&lvitemlo);
			}

			l_i_TempLowRow++;
			l_i_TempHighRow--;
		}
	}

	//----------------------------------------------------------------
	// If the right index has not reached the left side of array
	// must now sort the left partition.
	//----------------------------------------------------------------

	if (i_i_LowRow < l_i_TempHighRow)
	{
		SortListCtrl(i_i_ColumnNumber, i_b_Ascending, i_i_LowRow, l_i_TempHighRow);
	}

	//----------------------------------------------------------------
	// If the left index has not reached the right side of array
	// must now sort the right partition.
	//----------------------------------------------------------------

	if (l_i_TempLowRow < i_i_HighRow)
	{
		SortListCtrl(i_i_ColumnNumber, i_b_Ascending, l_i_TempLowRow, i_i_HighRow);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnColumnclickListUser(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	SortListCtrl(pNMListView->iSubItem, TRUE, 0, -1);

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////
BOOL CMSO_DemoDlg::PreTranslateMessage(MSG* pMsg)
{
	m_ToolTips.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::ShowErrorBox(CString i_cs_Message, I i_i_Error)
{
	CString l_cs_ErrorText;
	CString l_cs_BoxMessage;

	//------------------------------------------------------------
	// Get string associated with the error code
	// Construct the message
	//------------------------------------------------------------
	ConvertSDKError(i_i_Error, l_cs_ErrorText);

	l_cs_BoxMessage.Format("%s\n%d\t: %s\n", i_cs_Message, i_i_Error, l_cs_ErrorText);

	l_cs_ErrorText.Format("%d\t: Internal error", m_x_MorphoSmart.GetInternalError());
	l_cs_BoxMessage += l_cs_ErrorText;

	AfxMessageBox(l_cs_BoxMessage, MB_ICONERROR);
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnLoadPkcs12()
{
	CString					l_cs_OpenFile;
	CString					l_cs_FileExt;
	I						l_i_Ret, l_i_anti_replay_cnt, l_i_len, l_i_pkcs12_size;
	CResultDlg				l_x_ResultDlg;
	BOOL					l_b_status;
	CFile					l_x_file;
	PUC						l_puc_pkcs12;
	PUC						l_puc_antireplay;


	l_i_Ret = MORPHO_OK;

	CString l_x_p12Filter("PKCS#12 Files (*.p12)|*.p12||");
	CString l_x_a12Filter("Antireplay Counter Files (*.a12)|*.a12||");

	//------------------------------------------------------------
	// Get the p12 token and antireplaycount from files
	//------------------------------------------------------------
	l_b_status = GetFilesNameToOpen("", l_cs_OpenFile, l_cs_FileExt, l_x_p12Filter);

	if (l_b_status)
	{
		l_b_status = ReadFile(l_cs_OpenFile, &l_puc_pkcs12, &l_i_pkcs12_size);
	}
	if (l_b_status)
	{
		l_b_status = GetFilesNameToOpen("", l_cs_OpenFile, l_cs_FileExt, l_x_a12Filter);
	}
	if (l_b_status)
	{
		l_b_status = ReadFile(l_cs_OpenFile, &l_puc_antireplay, &l_i_len);
	}


	//------------------------------------------------------------
	// send the token
	//------------------------------------------------------------
	if (l_b_status)
	{
		//cut the encryption of communication when loading a pkcs12
		if (m_uc_SecuConfig&SECU_TUNNELING)
		{
			l_i_Ret = m_x_MorphoSmart.TunnelingClose();
		}
		else if (m_uc_SecuConfig&SECU_OFFERED_SECURITY)
		{
			l_i_Ret = m_x_MorphoSmart.OfferedSecuClose();
		}

		if (l_i_Ret != MORPHO_OK)
		{
			AfxMessageBox("Error while cutting encryption", MB_ICONERROR);
		}
		else
		{
			memcpy(&l_i_anti_replay_cnt, l_puc_antireplay, sizeof(I));
			l_i_Ret = m_x_MorphoSmart.SecuStoPkcs12(l_i_pkcs12_size, l_puc_pkcs12, l_i_anti_replay_cnt);
			if (l_i_Ret != MORPHO_OK)
			{
				//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
				l_x_ResultDlg.m_cs_Msg1.Format("Load PKCS12 Failed");
				switch (l_i_Ret)
				{
				case MORPHOERR_SECU:
					//probably a bad antireplay 
					l_x_ResultDlg.m_cs_Msg2.Format("Security failure (could be antireplay or crypto error)");
					break;

				default:
					l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
					l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Device::SecuStoPkcs12() function");
					ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
				}

				l_x_ResultDlg.DoModal();

			}
			else
			{
				Beep(BEEP_FREQUENCY, BEEP_DURATION);
				l_x_ResultDlg.m_cs_Msg1.Format("Load PKCS12 OK");
				l_x_ResultDlg.DoModal();
			}
			//put encryption again
			if (m_uc_SecuConfig&SECU_TUNNELING)
			{
				l_i_Ret = m_x_MorphoSmart.TunnelingOpen(
					MSOSECU_TripleDesSign,		//T_MORPHO_CALLBACK_TRIPLE_DES_SIGN		i_pf_CallbackTripleDesSign,
					MSOSECU_TripleDesCrypt,		//T_MORPHO_CALLBACK_TRIPLE_DES_CRYPT		i_pf_CallbackTripleDesCrypt,
					MSOSECU_TripleDesVerifSign,	//T_MORPHO_CALLBACK_TRIPLE_DES_VERIF_SIGN	i_pf_CallbackTripleDesVerifSign,
					MSOSECU_TripleDesDecrypt,	//T_MORPHO_CALLBACK_TRIPLE_DES_DECRYPT	i_pf_CallbackTripleDesDecrypt,
					MSOSECU_TunnelingInit1,		//T_MORPHO_CALLBACK_TUNNELING_INIT1		i_pf_CallbackTunnelingInit1,
					MSOSECU_TunnelingInit2,		//T_MORPHO_CALLBACK_TUNNELING_INIT2		i_pf_CallbackTunnelingInit2,
					m_ul_CertifHostSize,		//UL										i_ul_HostCertificateSize,
					m_puc_CertifHost			//PUC										i_puc_HostCertificate
					);
			}
			else if (m_uc_SecuConfig&SECU_OFFERED_SECURITY)
			{
				l_i_Ret = m_x_MorphoSmart.OfferedSecuOpen(
					MSOSECU_GenRandom,						//T_CallbackGenRandom						i_pf_CallbackGenRandom,
					MSOSECU_VerifOfferedSecuritySignature	//T_CallbackVerifOfferedSecuritySignature	i_pf_CallbackVerifOfferedSecuritySignature
					);
			}
		}
	}
	else
	{
		AfxMessageBox("Error while getting pkcs12 files", MB_ICONERROR);
	}

}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::SaveFile(PUC l_auc_FileBuffer, UL l_ul_FileBufferSize, CString i_cs_default_name, CString i_cs_filter)
{
	CString		l_cs_SaveFile;
	CResultDlg  l_x_ResultDlg;

	//------------------------------------------------------------
	// Save a buffer in a chosen file
	//------------------------------------------------------------
	CFileDialog l_x_SaveFileDlg(FALSE,
		"",
		i_cs_default_name,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
		i_cs_filter,
		this);
	SAV_PATH
		if (l_x_SaveFileDlg.DoModal() != IDOK)
		{
			//User Cancel
			RESTORE_PATH
				l_x_ResultDlg.m_cs_Msg1.Format("Failed");
			l_x_ResultDlg.m_cs_Msg2.Format("Saving file");
			l_x_ResultDlg.m_cs_Msg2.Format("aborted by user.");
			l_x_ResultDlg.DoModal();
			return FALSE;
		}
		else
		{
			RESTORE_PATH
				l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();

			//- - - - - - - - - - - 
			// Write data in File
			//- - - - - - - - - - -

			CString l_cs_File;
			l_cs_File.Format("%s", l_cs_SaveFile);
			CFile l_f_File(l_cs_File, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
			l_f_File.Write(l_auc_FileBuffer, l_ul_FileBufferSize);

			l_f_File.Close();
			return TRUE;
		}
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnReadCertificat()
{
	PUC			l_puc_Certif;
	UL			l_ul_Certif_Size;
	I			l_i_Ret, l_i_ind;
	bool        l_b_ret;
	CString		l_cs_default_name;
	CString     l_cs_filter;
	CResultDlg	l_x_ResultDlg;

	//------------------------------------------------------------
	// Read MSO Certificate and save in files
	//------------------------------------------------------------
	l_i_Ret = m_x_MorphoSmart.SecuReadCertificate(0, &l_ul_Certif_Size, &l_puc_Certif);

	if (l_i_Ret == MORPHO_OK)
	{
		l_cs_default_name.Format("mso_certif");
		l_b_ret = SaveFile(l_puc_Certif, l_ul_Certif_Size, l_cs_default_name, g_x_certifFilter);

		if (l_b_ret)
		{
			// Read Sub authoritie
			l_i_Ret = m_x_MorphoSmart.SecuReadCertificate(1, &l_ul_Certif_Size, &l_puc_Certif);
			if (l_i_Ret == MORPHO_OK)
			{
				l_cs_default_name.Format("certif_path num 1");
				l_b_ret = SaveFile(l_puc_Certif, l_ul_Certif_Size, l_cs_default_name, g_x_certifFilter);
			}
		}
		//------------------------------------------------------------
		// Read Remaining certificates from the certificate path 
		//------------------------------------------------------------
		l_i_ind = 2;
		if ((l_i_Ret == MORPHO_OK) && (l_b_ret == TRUE))
		{
			while (l_i_Ret == MORPHO_OK)
			{
				l_i_Ret = m_x_MorphoSmart.SecuReadCertificate(l_i_ind, &l_ul_Certif_Size, &l_puc_Certif);

				switch (l_i_Ret)
				{
				case MORPHO_OK:
					l_cs_default_name.Format("certif_path num %d", l_i_ind);
					l_b_ret = SaveFile(l_puc_Certif, l_ul_Certif_Size, l_cs_default_name, g_x_certifFilter);
					l_i_ind++;
					break;

				case MORPHOERR_CERTIF_UNKNOW:
					//no more certificate in the path
					Beep(BEEP_FREQUENCY, BEEP_DURATION);
					l_x_ResultDlg.m_cs_Msg1.Format("Read Certificate OK");
					l_x_ResultDlg.DoModal();
					break;

				default:
					//unexpected error
					//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
					l_x_ResultDlg.m_cs_Msg1.Format("Read certificate Failed");
					l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
					l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Device::SecuReadCertificate()");
					ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
					l_x_ResultDlg.DoModal();
					break;

				}

			}
		}
		else
		{
			if (l_b_ret)
			{
				//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
				l_x_ResultDlg.m_cs_Msg1.Format("Read certificate Failed");
				l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
				l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Device::SecuReadCertificate()");
				ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
				l_x_ResultDlg.DoModal();
			}
		}
	}
	else
	{
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_x_ResultDlg.m_cs_Msg1.Format("Read certificate Failed");
		l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
		l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Device::SecuReadCertificate()");
		ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
		l_x_ResultDlg.DoModal();
	}

}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnReadConfiguration()
{
	I                           l_i_Ret;
	UC							l_auc_SecuSerialNumber[MSO_SERIAL_NUMBER_LEN + 1];
	UC							l_uc_SecuConfig;
	T_MORPHO_FAR				l_x_SecuFar;
	UL							l_ul_SecuMinMSL;

	l_auc_SecuSerialNumber[MSO_SERIAL_NUMBER_LEN] = 0;

	//------------------------------------------------------------
	//Retreive security parameters from MSO
	//------------------------------------------------------------

	l_i_Ret = m_x_MorphoSmart.GetSecuConfig(
		l_auc_SecuSerialNumber,
		&l_uc_SecuConfig,
		&l_x_SecuFar,
		&l_ul_SecuMinMSL
		);

	//display on a Dialog
	CDisplayMSOConfDlg	dlg((PC)l_auc_SecuSerialNumber, l_uc_SecuConfig, l_x_SecuFar, l_ul_SecuMinMSL, m_bl_FVP);
	dlg.DoModal();
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnStoreCertificat()
{
	CString					l_cs_OpenFile;
	CString					l_cs_FileExt;
	I						l_i_Ret, l_i_len;
	CResultDlg				l_x_ResultDlg;
	BOOL					l_b_status;
	CFile					l_x_file;
	PUC						l_puc_certif;


	//------------------------------------------------------------
	//Get a certificate from chosen files and send it to MSO
	//------------------------------------------------------------
	l_b_status = GetFilesNameToOpen("", l_cs_OpenFile, l_cs_FileExt, g_x_certifFilter);

	if (l_b_status)
	{
		l_b_status = ReadFile(l_cs_OpenFile, &l_puc_certif, &l_i_len);
	}

	if (l_b_status)
	{
		//send to MSO
		l_i_Ret = m_x_MorphoSmart.SecuStoCertif(l_i_len, l_puc_certif);
		if (l_i_Ret != MORPHO_OK)
		{
			//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
			l_x_ResultDlg.m_cs_Msg1.Format("Store certificate Failed");
			switch (l_i_Ret)
			{
			case MORPHOERR_CERTIF_UNKNOW:
				//probably the authoritie that signed the certificate is unknow
				l_x_ResultDlg.m_cs_Msg2.Format("Signature Certificat Inconnue");
				break;

			default:
				//unknow error
				l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
				l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Device::SecuStoCertificate() function");
				ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
				break;
			}

			l_x_ResultDlg.DoModal();

			return;
		}
		else
		{
			Beep(BEEP_FREQUENCY, BEEP_DURATION);
			l_x_ResultDlg.m_cs_Msg1.Format("Store Certificate OK");
			l_x_ResultDlg.DoModal();
		}
	}
	else
	{
		AfxMessageBox("Error while getting certificate ", MB_ICONERROR);
	}
}


bool CMSO_DemoDlg::GetFilesAndExtarctTemplates(
	CString					i_cs_FilesType,
	CString					o_cs_listfile[MAX_TEMPLATE],
	T_INFO_USER				o_x_ListinfoUsers[MAX_TEMPLATE],
	I						&o_i_nb_file,
	C_MORPHO_TemplateList	&o_px_TemplateList,
	BOOL					i_b_isPrivacy,
	std::string				i_s_privacy_key,
	I						i_i_random
	)
{
	BOOL		l_b_tkb;
	CString		l_cs_FileExt;
	CString		l_cs_OpenFile;
	CString		l_cs_Message;
	T_USER_DATA	l_x_UserData;
	UC			l_uc_indexTemplate;
	I			l_i_Ret;
	BOOL		l_b_Ret;
	I			i;
	POSITION	l_x_FilePos;
	I			l_i_Counter;

	PC			l_pc_SelectedFilePathBuffer;
	DWORD		l_ul_SelectedFilePathBufferLenght;

	PUC			l_puc_Aes128CbcCryptedData = NULL;
	UL			l_ul_Aes128CbcCryptedDataLen = 0;
	UL			l_ul_lenBuf = 0;
	UC			l_puc_Buf[1024];
	
	l_i_Ret = MORPHO_OK;

	CFileDialog l_x_OpenFileDlg(TRUE,
		"",
		"",
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,
		i_cs_FilesType,
		this);

	l_ul_SelectedFilePathBufferLenght = 8192;

	// Allocate temporary OpenFileDialog file path buffer
	l_pc_SelectedFilePathBuffer = new C[l_ul_SelectedFilePathBufferLenght];

	if (l_pc_SelectedFilePathBuffer == NULL)
	{
		return FALSE;
	}

	// Prepare OpenFileDialog
	memset(l_pc_SelectedFilePathBuffer, 0, l_ul_SelectedFilePathBufferLenght * sizeof(C));
	l_x_OpenFileDlg.m_ofn.nMaxFile = l_ul_SelectedFilePathBufferLenght;
	l_x_OpenFileDlg.m_ofn.lpstrFile = l_pc_SelectedFilePathBuffer;

	l_b_Ret = TRUE;

	SAV_PATH
		if (l_x_OpenFileDlg.DoModal() != IDOK)
		{
			RESTORE_PATH
				AfxMessageBox("Verify aborted. Press OK to continue...", MB_ICONINFORMATION);
			l_b_Ret = FALSE;
		}
	RESTORE_PATH

		if (l_b_Ret == TRUE)
		{
			l_x_FilePos = l_x_OpenFileDlg.GetStartPosition();
			l_i_Counter = 0;
			l_b_tkb = false;

			while (l_x_FilePos != NULL)
			{
				l_cs_OpenFile = l_x_OpenFileDlg.GetNextPathName(l_x_FilePos);
				l_cs_FileExt = GetfileExtension(l_cs_OpenFile);
				if (l_i_Counter == 0)
				{
					if (!l_cs_FileExt.CompareNoCase("tkb"))
					{
						l_b_tkb = true;
					}
				}
				else
				{
					if (
						(l_b_tkb && l_cs_FileExt.CompareNoCase("tkb"))
						||
						(!l_b_tkb && !l_cs_FileExt.CompareNoCase("tkb"))
						)
					{
						l_cs_Message.Format("You can not mix tkb template with standard templates.");
						AfxMessageBox(l_cs_Message);
						l_i_Ret = -1;
					}
				}
				l_i_Counter++;

				if (l_i_Ret == MORPHO_OK && l_b_tkb && l_i_Counter > 1)
				{
					l_cs_Message.Format("You cannot use more than one tkb file.");
					AfxMessageBox(l_cs_Message);
					l_i_Ret = -1;
				}
			}

			if (l_i_Ret == MORPHO_OK && l_i_Counter > 20)
			{
				l_cs_Message.Format("The number of templates must be less or equal to 20.");
				AfxMessageBox(l_cs_Message);
				l_i_Ret = -1;
			}

			o_i_nb_file = l_i_Counter;

			l_x_FilePos = l_x_OpenFileDlg.GetStartPosition();
			l_i_Counter = 0;
			while (l_x_FilePos != NULL && l_i_Ret == MORPHO_OK)
			{
				l_cs_OpenFile = l_x_OpenFileDlg.GetNextPathName(l_x_FilePos);
				l_cs_FileExt = GetfileExtension(l_cs_OpenFile);

				// Retrieve Relevant data and store them in a structure
				l_b_Ret = OpenFileAndExtractData(
					l_cs_OpenFile,
					l_cs_FileExt,
					&l_x_UserData);

				if (l_b_Ret == FALSE)
				{
					l_i_Ret = -1;
				}

				if (l_i_Ret == MORPHO_OK)
				{
					if (l_b_tkb)
					{
						o_cs_listfile[0] = l_cs_OpenFile;
						o_x_ListinfoUsers[0].m_cs_ID = l_x_UserData.m_cs_ID;
						o_x_ListinfoUsers[0].m_cs_Firstname = l_x_UserData.m_cs_Firstname;
						o_x_ListinfoUsers[0].m_cs_Lastname = l_x_UserData.m_cs_Lastname;

						o_cs_listfile[1] = l_cs_OpenFile;
						o_x_ListinfoUsers[1].m_cs_ID = l_x_UserData.m_cs_ID;
						o_x_ListinfoUsers[1].m_cs_Firstname = l_x_UserData.m_cs_Firstname;
						o_x_ListinfoUsers[1].m_cs_Lastname = l_x_UserData.m_cs_Lastname;

						if (i_b_isPrivacy)
						{
							_PreparePrivacyData((PC)l_x_UserData.m_px_pk_X984->m_puc_Buffer,
								l_x_UserData.m_px_pk_X984->m_us_Size,
								i_s_privacy_key, i_i_random, FALSE, l_puc_Buf, &l_ul_lenBuf);

							l_i_Ret = o_px_TemplateList.PutX984(
								l_ul_lenBuf,
								l_puc_Buf
								);
						}
						else
						{
							l_i_Ret = o_px_TemplateList.PutX984(
								l_x_UserData.m_px_pk_X984->m_us_Size,
								l_x_UserData.m_px_pk_X984->m_puc_Buffer
								);
						}
						

						if (l_i_Ret != MORPHO_OK)
						{
							ShowAnimation("image_Ko");
							ShowErrorBox("C_MORPHO_TemplateList::PutX984() failed", l_i_Ret);
						}
					}
					else
					{
						for (i = 0; i<l_x_UserData.m_uc_NbFingers; ++i)
						{
							o_cs_listfile[l_i_Counter] = GetfileNameFromPath(l_cs_OpenFile);
							o_x_ListinfoUsers[l_i_Counter].m_cs_ID = l_x_UserData.m_cs_ID;
							o_x_ListinfoUsers[l_i_Counter].m_cs_Firstname = l_x_UserData.m_cs_Firstname;
							o_x_ListinfoUsers[l_i_Counter].m_cs_Lastname = l_x_UserData.m_cs_Lastname;

							l_i_Counter++;
						}

						if (l_x_UserData.m_uc_NbFingers > 1)
						{
							if (l_x_UserData.m_uc_FileType == MORPHO_PK_ANSI_378)
							{
								l_x_UserData.m_uc_NbFingers = 1;
							}
						}

						for (i = 0; i<l_x_UserData.m_uc_NbFingers; ++i)
						{
							if (!l_cs_FileExt.CompareNoCase("fvp") || !l_cs_FileExt.CompareNoCase("fvp-m"))
							{
								if (i_b_isPrivacy)
								{
									_PreparePrivacyData((PC)l_x_UserData.m_px_Buffer[i].m_puc_Buffer,
										l_x_UserData.m_px_Buffer[i].m_us_Size,
										i_s_privacy_key, i_i_random, FALSE, l_puc_Buf, &l_ul_lenBuf);

									l_i_Ret = o_px_TemplateList.PutFVPTemplate(
										l_x_UserData.m_uc_FVPFiletype,				// T_MORPHO_FVP_TYPE_TEMPLATE  i_uc_FVPFiletype;,
										l_ul_lenBuf,		// UL			i_ul_lenTemplate,
										l_puc_Buf,	// PUC			i_puc_dataTemplate,
										l_uc_indexTemplate							// UC &			o_uc_indexTemplate);
										);
								}
								else
								{
									l_i_Ret = o_px_TemplateList.PutFVPTemplate(
										l_x_UserData.m_uc_FVPFiletype,				// T_MORPHO_FVP_TYPE_TEMPLATE  i_uc_FVPFiletype;,
										l_x_UserData.m_px_Buffer[i].m_us_Size,		// UL			i_ul_lenTemplate,
										l_x_UserData.m_px_Buffer[i].m_puc_Buffer,	// PUC			i_puc_dataTemplate,
										l_uc_indexTemplate							// UC &			o_uc_indexTemplate);
										);
								}
							}
							else
							{
								if (i_b_isPrivacy)
								{
									_PreparePrivacyData((PC)l_x_UserData.m_px_Buffer[i].m_puc_Buffer,
										l_x_UserData.m_px_Buffer[i].m_us_Size,
										i_s_privacy_key, i_i_random, FALSE, l_puc_Buf, &l_ul_lenBuf);

									l_i_Ret = o_px_TemplateList.PutTemplate(
										l_x_UserData.m_uc_FileType,					// T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
										l_ul_lenBuf,								// UL			i_ul_lenTemplate,
										l_puc_Buf,									// PUC			i_puc_dataTemplate,
										l_uc_indexTemplate							// UC &			o_uc_indexTemplate);
										);
								}
								else
								{
									l_i_Ret = o_px_TemplateList.PutTemplate(
										l_x_UserData.m_uc_FileType,					// T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
										l_x_UserData.m_px_Buffer[i].m_us_Size,		// UL			i_ul_lenTemplate,
										l_x_UserData.m_px_Buffer[i].m_puc_Buffer,	// PUC			i_puc_dataTemplate,
										l_uc_indexTemplate							// UC &			o_uc_indexTemplate);
										);
								}
							}

							if (l_i_Ret != MORPHO_OK)
							{
								ShowAnimation("image_Ko");
								ShowErrorBox("An error occured while calling C_MORPHO_TemplateList::PutTemplate() function", l_i_Ret);
								break;
							}
						}
					}
				}

				FreeOpenFileAndExtractData(&l_x_UserData);
			}
		}
		else
		{
			l_i_Ret = -1;
		}

	// Free temporary OpenFileDialog file path buffer
	if (l_pc_SelectedFilePathBuffer != NULL)
	{
		delete[] l_pc_SelectedFilePathBuffer;
		l_pc_SelectedFilePathBuffer = NULL;
	}

	if (l_i_Ret == MORPHO_OK && l_i_Counter > 20)
	{
		l_cs_Message.Format("The number of templates must be less or equal to 20.");
		AfxMessageBox(l_cs_Message);
		l_i_Ret = -1;
	}

	if (l_i_Ret == MORPHO_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::GetFileAndExtractData(
	CString		i_cs_DefaultFileName,
	CString		i_cs_FilesType,
	CString		&o_cs_FileToOpen,
	CString		&o_cs_FileExt,
	T_USER_DATA *o_px_UserData)
{
	if (!GetFilesNameToOpen(
		i_cs_DefaultFileName,			//default file name
		o_cs_FileToOpen,				//file name
		o_cs_FileExt,					//file name extension
		i_cs_FilesType
		))
	{
		AfxMessageBox("Get file aborted. Press OK to continue...", MB_ICONINFORMATION);
		return FALSE;
	}


	if (!OpenFileAndExtractData(
		o_cs_FileToOpen,	//file name
		o_cs_FileExt,		//file name extension
		o_px_UserData		//information
		))
	{
		FreeOpenFileAndExtractData(o_px_UserData);
		AfxMessageBox("Error while extracting data.\nFile may be corrupted.", MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnVerifSignX984()
{
	CString					l_cs_OpenFile;
	CString					l_cs_FileExt;
	T_USER_DATA				l_x_UserData;
	PUC						l_puc_Certif;
	UL						l_ul_CertifSize;
	C_MORPHO_TemplateList	l_CMorphoTemplateList;
	I						l_i_Result;
	I						l_i_Ret;

	if (!GetFileAndExtractData(
		"",									//default file name		CString		i_cs_DefaultFileName, 										
		"Token Bio Files (*.tkb)|*.tkb||",	//						CString		i_cs_FilesType,
		l_cs_OpenFile,						//file name				CString		&o_cs_FileToOpen,
		l_cs_FileExt,						//file name extension	CString		&o_cs_FileExt,
		&l_x_UserData						//information			T_USER_DATA *o_px_UserData)
		))
	{
		return;
	}

	if (!ExtractCertif(
		l_cs_OpenFile,		//CString	i_cs_FileToOpen,
		&l_ul_CertifSize,	//PUL		o_pul_CertifSize,
		&l_puc_Certif		//PUC*		o_ppuc_Certif
		))
	{
		return;
	}

	l_i_Ret = l_CMorphoTemplateList.VerifSignX984(
		l_x_UserData.m_px_pk_X984->m_us_Size,		//UL								i_ul_X984Size,
		l_x_UserData.m_px_pk_X984->m_puc_Buffer,	//PUC								i_puc_X984,
		l_ul_CertifSize,						//UL								i_ul_MsoCertificateSize,
		l_puc_Certif,							//PUC								i_puc_MsoCertificate,
		MSOSECU_VerifSignRSA,					//T_MORPHO_CALLBACK_VERIF_SIGN		i_pf_VerifSignRSA,
		MSOSECU_VerifSignDSA,					//T_MORPHO_CALLBACK_VERIF_SIGN		i_pf_VerifSignDSA,
		&l_i_Result								//PI								o_pi_Result);
		);

	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("C_MORPHO_TemplateList::VerifSignX984() failed", l_i_Ret);
		return;
	}

	if (l_i_Result == FALSE)
	{
		AfxMessageBox("The X984 is not valid");
	}
	else
	{
		AfxMessageBox("The X984 is valid");
	}
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnExtractSignerId()
{
	CString					l_cs_OpenFile;
	CString					l_cs_FileExt;
	T_USER_DATA				l_x_UserData;
	PUC						l_puc_Certif;
	UL						l_ul_CertifSize;
	C_MORPHO_TemplateList	l_CMorphoTemplateList;
	I						l_i_Ret;
	UL						l_ul_lenSignerId;
	PUC						l_puc_SignerId;
	CString					l_cs_SignerId;
	CString					l_cs_Temp;
	UL						i;

	if (!GetFileAndExtractData(
		"",										//default file name		CString		i_cs_DefaultFileName, 										
		"Token Bio Files (*.tkb)|*.tkb||",		//						CString		i_cs_FilesType,
		l_cs_OpenFile,							//file name				CString		&o_cs_FileToOpen,
		l_cs_FileExt,							//file name extension	CString		&o_cs_FileExt,
		&l_x_UserData							//information			T_USER_DATA *o_px_UserData)
		))
	{
		return;
	}

	if (!ExtractCertif(
		l_cs_OpenFile,		//CString	i_cs_FileToOpen,
		&l_ul_CertifSize,	//PUL		o_pul_CertifSize,
		&l_puc_Certif		//PUC*		o_ppuc_Certif
		))
	{
		return;
	}

	l_i_Ret = l_CMorphoTemplateList.ExtractSignerIdX984(
		l_x_UserData.m_px_pk_X984->m_us_Size,		//UL							i_ul_X984Size,
		l_x_UserData.m_px_pk_X984->m_puc_Buffer,	//PUC							i_puc_X984,
		l_ul_lenSignerId,						//UL						  &	o_ul_lenSignerId,
		l_puc_SignerId							//PUC						  & o_puc_SignerId);
		);


	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("C_MORPHO_TemplateList::ExtractSignerIdX984() failed", l_i_Ret);
		return;
	}


	l_cs_SignerId = "Certificate identifier :";

	for (i = 0; i<l_ul_lenSignerId; i++)
	{
		l_cs_Temp.Format(" %02X", l_puc_SignerId[i]);
		l_cs_SignerId += l_cs_Temp;
	}

	AfxMessageBox(l_cs_SignerId);
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::_VerifyWithSmartCardExternal(UL i_ul_CallbackMask)
{
	C_MORPHO_TemplateList l_x_TemplateList;
	CString		l_cs_Message;
	UC			l_uc_indexTemplate;
	I			i;
	I			l_i_Ret;
	T_USER_DATA l_x_UserData;
	CString		l_cs_OpenFile;
	CResultDlg	l_x_ResultDlg;
	T_MORPHO_TYPE_TEMPLATE				l_x_TypeTemplate;

	CBioDemoCard	*l_px_BioDemoCard;

	//------------------------------------------------------------
	// Extract Data From Smart card
	//------------------------------------------------------------

	l_i_Ret = MORPHO_OK;

	if (m_BioOpPSheet.m_VerifyPPage.m_cs_WorkflowType == STR_WORKFLOWTYPE_SAGEM_OPUCE)
	{
		l_px_BioDemoCard = new CSagemBioDemoCard;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_WorkflowType == STR_WORKFLOWTYPE_CLIENT_GB)
	{
		l_px_BioDemoCard = new CGB_BioDemoCard;
	}
	else
	{
		AfxMessageBox("Choose a Card Workflow Type valid");
		l_i_Ret = MORPHOERR_BADPARAMETER;
	}

	if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PKCOMP)
	{
		l_x_TypeTemplate = MORPHO_PK_COMP;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PKCOMPNORM)
	{
		l_x_TypeTemplate = MORPHO_PK_COMP_NORM;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PKMAT)
	{
		l_x_TypeTemplate = MORPHO_PK_MAT;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PKMATNORM)
	{
		l_x_TypeTemplate = MORPHO_PK_MAT_NORM;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PKLITE)
	{
		l_x_TypeTemplate = MORPHO_PK_PKLITE;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PKS)
	{
		l_x_TypeTemplate = MORPHO_PK_COMP;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ANSI378)
	{
		l_x_TypeTemplate = MORPHO_PK_ANSI_378;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ANSI378_2009)
	{
		l_x_TypeTemplate = MORPHO_PK_ANSI_378_2009;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ISO_FMC_CS)
	{
		l_x_TypeTemplate = MORPHO_PK_ISO_FMC_CS;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ISO_FMC_NS)
	{
		l_x_TypeTemplate = MORPHO_PK_ISO_FMC_NS;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ISO_FMR)
	{
		l_x_TypeTemplate = MORPHO_PK_ISO_FMR;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ISO_FMR_2011)
	{
		l_x_TypeTemplate = MORPHO_PK_ISO_FMR_2011;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_MINEX_A)
	{
		l_x_TypeTemplate = MORPHO_PK_MINEX_A;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PK_MOC)
	{
		l_x_TypeTemplate = MORPHO_PK_MOC;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PK_DIN_V66400_CS)
	{
		l_x_TypeTemplate = MORPHO_PK_DIN_V66400_CS;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PK_DIN_V66400_CS_AA)
	{
		l_x_TypeTemplate = MORPHO_PK_DIN_V66400_CS_AA;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ISO_FMC_CS_AA)
	{
		l_x_TypeTemplate = MORPHO_PK_ISO_FMC_CS_AA;
	}
	else
	{
		l_x_TypeTemplate = MORPHO_PK_COMP;
	}

	if (l_i_Ret == SMARTCARD_OK)
	{
		DisableButtons();
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

		l_i_Ret = OpenSmartCardAndExtractData(&l_x_UserData, l_px_BioDemoCard);
		delete l_px_BioDemoCard;
	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

	if (l_i_Ret != SMARTCARD_OK)
	{
		l_x_ResultDlg.m_cs_Msg1.Format("Extract data Failed");
		l_x_ResultDlg.m_cs_Msg2.Format("Error while reading data from smart card");
		l_x_ResultDlg.m_cs_Msg3.Format("Error 0x%08X", l_i_Ret);
		switch (l_i_Ret)
		{
		case  SCARD_W_REMOVED_CARD:
			l_x_ResultDlg.m_cs_Msg4.Format("Smart card withdranw !");
			break;
		case SCARD_W_UNRESPONSIVE_CARD:
			l_x_ResultDlg.m_cs_Msg4.Format("Smart card not responding to a reset !");
			break;
		case 0x8010002F: //SCARD_E_COMM_DATA_LOST:in scarerr.h
			l_x_ResultDlg.m_cs_Msg4.Format("COM error with smart card detected !");
			break;
		case 0x8010002e://SCARD_E_NO_READERS_AVAILABLE
			l_x_ResultDlg.m_cs_Msg4.Format("Cannot find a smart card reader !");
			break;
		default:
			l_x_ResultDlg.m_cs_Msg3.Format("Error 0x%08X. Check that", l_i_Ret);
			l_x_ResultDlg.m_cs_Msg4.Format("the card is compatible with the demo !", l_i_Ret);
			break;
		}
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_x_ResultDlg.DoModal();
		CleanAsyncMessages2();

		EnableButtons();
		return FALSE;
	}

	else // BUG CORRIGE PAR C++TEST [BD-PB-CC-2]
	{

		//------------------------------------------------------------
		// PutTemplate
		//------------------------------------------------------------

		for (i = 0; i <= l_x_UserData.m_uc_NbFingers; i++)
		{

			l_i_Ret = l_x_TemplateList.PutTemplate(
				l_x_TypeTemplate,								// T_MORPHO_TYPE_TEMPLATE	i_uc_typTemplate,
				l_x_UserData.m_px_Buffer[i].m_us_Size,	// UL			i_ul_lenTemplate,
				l_x_UserData.m_px_Buffer[i].m_puc_Buffer,	// PUC			i_puc_dataTemplate,
				l_uc_indexTemplate							// UC &			o_uc_indexTemplate);
				);

			if (l_i_Ret != MORPHO_OK)
			{
				l_cs_Message.Format("An error occured while calling C_MORPHO_TemplateList::PutTemplate() function (Error %d).", l_i_Ret);
				AfxMessageBox(l_cs_Message);
				EnableButtons();
				FreeOpenFileAndExtractData(&l_x_UserData);
				return FALSE;
			}
		}

		//------------------------------------------------------------
		// Verify
		//------------------------------------------------------------
		m_b_BiometricOperationProcessing = TRUE;

		if (m_tbCtrl.m_TabOptionsDlg->m_b_Export_Num_Pk == FALSE)
		{
			m_puc_ExportNumPk = NULL;
			m_uc_ExportNumPk = 0xFF;
		}
		else
		{
			m_puc_ExportNumPk = &m_uc_ExportNumPk;
		}

		l_i_Ret = m_x_MorphoSmart.Verify(
			m_tbCtrl.m_TabBioSettingsDlg->m_i_Timeout,								//US							i_us_Timeout, 
			(T_MORPHO_FAR)m_tbCtrl.m_TabBioSettingsDlg->m_i_Threshold,				//T_MORPHO_FAR					i_us_FAR,
			l_x_TemplateList,							//C_MORPHO_TemplateList &		i_x_TemplateList,
			i_ul_CallbackMask,							//UL							i_ul_CallbackCmd,
			BiometricCallback,							//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
			this,										//PVOID							i_pv_CallbackArgument
			NULL,										//PUL							o_pul_MatchingScore,
			m_puc_ExportNumPk,							//PUC							o_puc_ExportNumPk,
			m_tbCtrl.m_TabBioSettingsDlg->m_i_MatchingStrategy,							//I								i_i_CoderChoice,
			MORPHO_VERIF_DETECT_MODE | m_ul_WakeUpMode,	//UL							i_ul_DetecModeChoice,
			m_tbCtrl.m_TabBioSettingsDlg->m_i_MatchingStrategy									//UL							i_ul_ModeChoice
			);

		// Get FFD logs and write in log file
		GetAndWriteFFDLogs(&m_x_MorphoSmart);

		m_b_BiometricOperationProcessing = FALSE;
		CleanAsyncMessages();
	}

	if (l_i_Ret == MORPHOERR_DEVICE_LOCKED)
	{
		I l_i_Ret2 = UnlockDevice();
		if (l_i_Ret2 != MORPHO_OK)
		{
			ShowErrorBox("An error occured during Device Unlocking", l_i_Ret2);
		}
		return FALSE;
	}
	else if (l_i_Ret != MORPHO_OK)
	{
		ShowAnimation("image_Ko");
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_x_ResultDlg.m_cs_Msg1.Format("Verification Failed");
		switch (l_i_Ret)
		{
		case MORPHOERR_CMDE_ABORTED:
			l_x_ResultDlg.m_cs_Msg2.Format("Command Aborted.");
			break;
		case MORPHOERR_NO_HIT:
		case MORPHOERR_FFD:
		case MORPHOERR_MOIST_FINGER:
		case MORPHOERR_MOVED_FINGER:
		case MORPHOERR_SATURATED_FINGER:
		case MORPHOERR_INVALID_FINGER:
			l_x_ResultDlg.m_cs_Msg2.Format("Person not Authenticated.");
			break;
		case MORPHOERR_TIMEOUT:
			l_x_ResultDlg.m_cs_Msg2.Format("Timeout has expired.");
			l_x_ResultDlg.m_cs_Msg3.Format("Command Aborted.");
			break;
		case MORPHOERR_BADPARAMETER:
			l_x_ResultDlg.m_cs_Msg2.Format("If your MSO accepts signed Pks only,");
			l_x_ResultDlg.m_cs_Msg3.Format("your templates must be encapsulated");
			l_x_ResultDlg.m_cs_Msg4.Format("in a biometric token.");
			break;
		case MORPHOERR_NOT_IMPLEMENTED:
			l_x_ResultDlg.m_cs_Msg2.Format("Not Implemented !!!");
			break;
		default:
			l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
			l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Database::Verify()");
			l_x_ResultDlg.m_cs_Msg4.Format("function (Error %d).", l_i_Ret);
		}

		l_x_ResultDlg.DoModal();
		FreeOpenFileAndExtractData(&l_x_UserData);
		return FALSE;
	}

	else //BUG CORRIGE PAR C++TEST [BD-PB-CC-2] if(l_i_Ret == MORPHO_OK)
	{
		Beep(BEEP_FREQUENCY, BEEP_DURATION);
		ShowAnimation("image_Ok");
		l_x_ResultDlg.m_cs_Msg1.Format("User authenticated");
		l_x_ResultDlg.m_cs_Msg2.Format("Firstname : %s", l_x_UserData.m_cs_Firstname);
		l_x_ResultDlg.m_cs_Msg3.Format("Lastname : %s", l_x_UserData.m_cs_Lastname);
		l_x_ResultDlg.m_cs_Msg4.Format("ID : %s", l_x_UserData.m_cs_ID);

		if (m_puc_ExportNumPk != NULL)
		{
			l_x_ResultDlg.m_cs_Msg5.Format("Matching Pk Number : %d", m_uc_ExportNumPk);
		}

		l_x_ResultDlg.DoModal();
		CleanAsyncMessages2();


		FreeOpenFileAndExtractData(&l_x_UserData);
		return TRUE;
	}
	//BUG CORRIGE PAR C++TEST [BD-PB-CC-2]if(l_i_Ret == MORPHO_OK)
	/*if ( l_i_Ret == MORPHO_OK )
	{
	return TRUE;
	}
	else
	{
	return FALSE;
	}*/
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::_VerifyWithSmartCardInternal(UL i_ul_CallbackMask)
{
	C_MORPHO_TemplateList l_x_TemplateList;
	CString		l_cs_Message;
	I			l_i_Ret;
	CString		l_cs_OpenFile;
	CResultDlg	l_x_ResultDlg;
	T_MORPHO_TYPE_TEMPLATE				l_x_TypeTemplate;
	T_MORPHO_SMARTCARD_WORKFLOW_TYPE	l_x_WorkFlowType;

	memset(&l_x_TypeTemplate, 0, sizeof(T_MORPHO_TYPE_TEMPLATE));
	CBioDemoCard	*l_px_BioDemoCard;

	l_i_Ret = MORPHO_OK;

	if (m_BioOpPSheet.m_VerifyPPage.m_cs_WorkflowType == STR_WORKFLOWTYPE_SAGEM_OPUCE)
	{
		l_px_BioDemoCard = new CSagemBioDemoCard;
	}
	else if (m_BioOpPSheet.m_VerifyPPage.m_cs_WorkflowType == STR_WORKFLOWTYPE_CLIENT_GB)
	{
		l_px_BioDemoCard = new CGB_BioDemoCard;
	}
	else
	{
		AfxMessageBox("Choose a Card Workflow Type valid");
		l_i_Ret = MORPHOERR_BADPARAMETER;
	}

	//------------------------------------------------------------
	// Extract Data From Smart card
	//------------------------------------------------------------

	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = OpenSmartCard(l_px_BioDemoCard);

		DisableButtons();
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	}

	if (l_i_Ret == MORPHO_OK)
	{
		UC		l_auc_Buff[256];
		T__BUF	l_x_Buff;

		l_x_Buff.m_puc_Buf = l_auc_Buff;
		l_x_Buff.m_ul_Size = sizeof(l_auc_Buff);
		l_px_BioDemoCard->Select(0x3F00, &l_x_Buff);
	}

	if (l_i_Ret != SMARTCARD_OK)
	{
		l_x_ResultDlg.m_cs_Msg1.Format("Open Smart Card Failed");
		l_x_ResultDlg.m_cs_Msg2.Format("Error while reading data from smart card");
		l_x_ResultDlg.m_cs_Msg3.Format("Error 0x%08X", l_i_Ret);
		switch (l_i_Ret)
		{
		case  SCARD_W_REMOVED_CARD:
			l_x_ResultDlg.m_cs_Msg4.Format("Smart card withdrawn !");
			break;
		case SCARD_W_UNRESPONSIVE_CARD:
			l_x_ResultDlg.m_cs_Msg4.Format("Smart card not responding to a reset !");
			break;
		case 0x8010002F: //SCARD_E_COMM_DATA_LOST:in scarerr.h
			l_x_ResultDlg.m_cs_Msg4.Format("COM error with smart card detected !");
			break;
		case 0x8010002e://SCARD_E_NO_READERS_AVAILABLE
			l_x_ResultDlg.m_cs_Msg4.Format("Cannot find a smart card reader !");
			break;
		default:
			l_x_ResultDlg.m_cs_Msg3.Format("Error 0x%08X. Check that", l_i_Ret);
			l_x_ResultDlg.m_cs_Msg4.Format("the card is compatible with the demo !", l_i_Ret);
			break;
		}
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
		l_x_ResultDlg.DoModal();
		CleanAsyncMessages2();

		EnableButtons();
	}

	if (l_i_Ret == MORPHO_OK)
	{

		if ((m_BioOpPSheet.m_VerifyPPage.m_i_NbFingers != 0) && (m_BioOpPSheet.m_VerifyPPage.m_i_NbFingers != 1))
		{
			AfxMessageBox("Choose 1 or 2 fingers");
			l_i_Ret = MORPHOERR_BADPARAMETER;
		}

		if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PKCOMP)
		{
			l_x_TypeTemplate = MORPHO_PK_COMP;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PKCOMPNORM)
		{
			l_x_TypeTemplate = MORPHO_PK_COMP_NORM;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PKMAT)
		{
			l_x_TypeTemplate = MORPHO_PK_MAT;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PKMATNORM)
		{
			l_x_TypeTemplate = MORPHO_PK_MAT_NORM;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PKLITE)
		{
			l_x_TypeTemplate = MORPHO_PK_PKLITE;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PKS)
		{
			l_x_TypeTemplate = MORPHO_PK_COMP;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ANSI378)
		{
			l_x_TypeTemplate = MORPHO_PK_ANSI_378;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ANSI378_2009)
		{
			l_x_TypeTemplate = MORPHO_PK_ANSI_378_2009;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ISO_FMC_CS)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMC_CS;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ISO_FMC_NS)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMC_NS;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ISO_FMR)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMR;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ISO_FMR_2011)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMR_2011;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_MINEX_A)
		{
			l_x_TypeTemplate = MORPHO_PK_MINEX_A;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PK_MOC)
		{
			l_x_TypeTemplate = MORPHO_PK_MOC;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PK_DIN_V66400_CS)
		{
			l_x_TypeTemplate = MORPHO_PK_DIN_V66400_CS;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_PK_DIN_V66400_CS_AA)
		{
			l_x_TypeTemplate = MORPHO_PK_DIN_V66400_CS_AA;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_TemplateType == STR_ISO_FMC_CS_AA)
		{
			l_x_TypeTemplate = MORPHO_PK_ISO_FMC_CS_AA;
		}
		else
		{
			l_x_TypeTemplate = MORPHO_PK_COMP;
		}

		if (m_BioOpPSheet.m_VerifyPPage.m_cs_WorkflowType == STR_WORKFLOWTYPE_SAGEM_OPUCE)
		{
			l_x_WorkFlowType = MORPHO_WORFLOW_SMARTCARD_SAGEM_DEMO;
		}
		else if (m_BioOpPSheet.m_VerifyPPage.m_cs_WorkflowType == STR_WORKFLOWTYPE_CLIENT_GB)
		{
			l_x_WorkFlowType = MORPHO_WORFLOW_SMARTCARD_CLIENT_GB;
		}
		else
		{
			l_x_WorkFlowType = MORPHO_WORFLOW_SMARTCARD_SAGEM_DEMO;
		}

	}

	if (l_i_Ret == MORPHO_OK)
	{

		//------------------------------------------------------------
		// PutTemplate
		//------------------------------------------------------------
		l_i_Ret = l_x_TemplateList.SetTemplateIsFromSmartCard(l_x_WorkFlowType, l_x_TypeTemplate, m_BioOpPSheet.m_VerifyPPage.m_i_NbFingers + 1);
		if (l_i_Ret != MORPHO_OK)
		{
			l_cs_Message.Format("An error occured while calling C_MORPHO_TemplateList::PutTemplate() function (Error %d).", l_i_Ret);
			AfxMessageBox(l_cs_Message);
			EnableButtons();
		}
	}

	if (l_i_Ret == MORPHO_OK)
	{
		//------------------------------------------------------------
		// Verify
		//------------------------------------------------------------
		m_b_BiometricOperationProcessing = TRUE;

		if (m_tbCtrl.m_TabOptionsDlg->m_b_Export_Num_Pk == FALSE)
		{
			m_puc_ExportNumPk = NULL;
			m_uc_ExportNumPk = 0xFF;
		}
		else
		{
			m_puc_ExportNumPk = &m_uc_ExportNumPk;
		}

		l_i_Ret = m_x_MorphoSmart.Verify(
			m_tbCtrl.m_TabBioSettingsDlg->m_i_Timeout,								//US							i_us_Timeout, 
			(T_MORPHO_FAR)m_tbCtrl.m_TabBioSettingsDlg->m_i_Threshold,				//T_MORPHO_FAR					i_us_FAR,
			l_x_TemplateList,							//C_MORPHO_TemplateList &		i_x_TemplateList,
			i_ul_CallbackMask,							//UL							i_ul_CallbackStatus,
			BiometricCallback,							//T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
			this,										//PVOID							i_pv_CallbackArgument
			NULL,										//PUL							o_pul_MatchingScore,
			m_puc_ExportNumPk,							//PUC							o_puc_ExportNumPk,
			m_tbCtrl.m_TabBioSettingsDlg->m_i_MatchingStrategy,							//I								i_i_CoderChoice,
			MORPHO_VERIF_DETECT_MODE | m_ul_WakeUpMode,	//UL							i_ul_DetecModeChoice,
			m_tbCtrl.m_TabBioSettingsDlg->m_i_MatchingStrategy									//UL							i_ul_ModeChoice
			);

		// Get FFD logs and write in log file
		GetAndWriteFFDLogs(&m_x_MorphoSmart);

		m_b_BiometricOperationProcessing = FALSE;
		CleanAsyncMessages();

		if (l_i_Ret == MORPHOERR_DEVICE_LOCKED)
		{
			I l_i_Ret2 = UnlockDevice();
			if (l_i_Ret2 != MORPHO_OK)
			{
				ShowErrorBox("An error occured during Device Unlocking", l_i_Ret2);
			}
		}
		else if (l_i_Ret != MORPHO_OK)
		{
			ShowAnimation("image_Ko");
			//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
			l_x_ResultDlg.m_cs_Msg1.Format("Verification Failed");
			switch (l_i_Ret)
			{
			case MORPHOERR_CMDE_ABORTED:
				l_x_ResultDlg.m_cs_Msg2.Format("Command Aborted.");
				break;
			case MORPHOERR_NO_HIT:
			case MORPHOERR_FFD:
			case MORPHOERR_MOIST_FINGER:
			case MORPHOERR_MOVED_FINGER:
			case MORPHOERR_SATURATED_FINGER:
			case MORPHOERR_INVALID_FINGER:
				l_x_ResultDlg.m_cs_Msg2.Format("Person not Authenticated.");
				break;
			case MORPHOERR_TIMEOUT:
				l_x_ResultDlg.m_cs_Msg2.Format("Timeout has expired.");
				l_x_ResultDlg.m_cs_Msg3.Format("Command Aborted.");
				break;
			case MORPHOERR_BADPARAMETER:
				l_x_ResultDlg.m_cs_Msg2.Format("Verify selected parameters :");
				l_x_ResultDlg.m_cs_Msg3.Format("the Smart Card workflow type");
				l_x_ResultDlg.m_cs_Msg4.Format("the template type in the smart card");
				break;
			case MORPHOERR_NOT_IMPLEMENTED:
				l_x_ResultDlg.m_cs_Msg2.Format("Not Implemented !!!");
				break;
			case MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN:
				l_x_ResultDlg.m_cs_Msg2.Format("Finger is misplaced or has been");
				l_x_ResultDlg.m_cs_Msg3.Format("withdrawn from sensor during acquisition.");
				break;
			default:
				l_x_ResultDlg.m_cs_Msg2.Format("An error occured while calling");
				l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Database::Verify()");
				l_x_ResultDlg.m_cs_Msg4.Format("function (Error %d).", l_i_Ret);
			}

			l_x_ResultDlg.DoModal();
		}
	}

	if ((l_i_Ret == MORPHO_OK) && (m_BioOpPSheet.m_VerifyPPage.m_cs_WorkflowType == STR_WORKFLOWTYPE_CLIENT_GB))
	{
		UC			l_auc_Buff[2048];
		T__BUF		l_x_Buff;

		//select master file
		l_x_Buff.m_puc_Buf = l_auc_Buff;
		l_x_Buff.m_ul_Size = sizeof(l_auc_Buff);

		l_i_Ret = l_px_BioDemoCard->Select(0x3000, &l_x_Buff);

		l_i_Ret = l_px_BioDemoCard->ReadBinary(0, l_auc_Buff, 2048);

		l_i_Ret = CloseSmartCard(l_px_BioDemoCard);
	}

	if (l_i_Ret == MORPHO_OK)
	{
		Beep(BEEP_FREQUENCY, BEEP_DURATION);
		ShowAnimation("image_Ok");
		l_x_ResultDlg.m_cs_Msg1.Format("User authenticated");
		l_x_ResultDlg.m_cs_Msg2.Format("Firstname : %s", "");
		l_x_ResultDlg.m_cs_Msg3.Format("Lastname : %s", "");
		l_x_ResultDlg.m_cs_Msg4.Format("ID : %s", "");

		if (m_puc_ExportNumPk != NULL)
		{
			l_x_ResultDlg.m_cs_Msg5.Format("Matching Pk Number : %d", m_uc_ExportNumPk);
		}

		l_x_ResultDlg.DoModal();
		CleanAsyncMessages2();
	}

	delete l_px_BioDemoCard;

	if (l_i_Ret == MORPHO_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////
//i_uc_fct = 0 to set range
//i_uc_fct = 0 to set step
void SmartCartIHMCallback(UC	i_uc_fct, US i_us_value, PVOID i_pv_context)
{
	CMSO_DemoDlg* MSO_DemoDlg = (CMSO_DemoDlg *)i_pv_context;

	switch (i_uc_fct)
	{
	case 0:
		MSO_DemoDlg->m_ctl_ProgressCtrl.SetRange(0, i_us_value);
		break;
	case 1:
		MSO_DemoDlg->m_ctl_ProgressCtrl.SetStep(i_us_value);
		MSO_DemoDlg->m_ctl_ProgressCtrl.StepIt();

		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////
I CMSO_DemoDlg::OpenSmartCardAndInsertData
(
T_USER_DATA		*i_x_UserData,
CBioDemoCard	*i_px_SagemBioDemoCard,
BOOL			i_b_AddLength
)
{
	CString l_str_ReaderName;
	I	l_i_NbReader;
	T_CBioDemoCard_USER_DATA	l_x_UserData;
	I	l_i_Ret;
	I	l_i_ToAdd;

	if (i_b_AddLength == TRUE)
	{
		l_i_ToAdd = 2;
	}
	else
	{
		l_i_ToAdd = 0;
	}

	//Enumerate connected smart card reader
	l_i_Ret = i_px_SagemBioDemoCard->EnumReader(&l_i_NbReader);

	//select first smart card reader connected
	if (l_i_Ret == MORPHO_OK)
	{
		l_str_ReaderName = i_px_SagemBioDemoCard->GetReaderName(0);

		//Power on the card
		l_i_Ret = i_px_SagemBioDemoCard->PowerOn(l_str_ReaderName);

		if (l_i_Ret == MORPHO_OK)
		{
			//update data
			memcpy(l_x_UserData.m_puc_Name, (LPCTSTR)i_x_UserData->m_cs_Firstname, MAX_NAME_SIZE);
			memcpy(l_x_UserData.m_puc_Surname, (LPCTSTR)i_x_UserData->m_cs_Lastname, MAX_SURNAME_SIZE);
			memcpy(l_x_UserData.m_puc_Identifier, (LPCTSTR)i_x_UserData->m_cs_ID, MAX_IDENTIFIER_SIZE);

			l_x_UserData.m_us_PK1_Size = i_x_UserData->m_px_Buffer[0].m_us_Size + l_i_ToAdd;
			l_x_UserData.m_puc_PK1[0] = (UC)((l_x_UserData.m_us_PK1_Size - l_i_ToAdd) >> 8);
			l_x_UserData.m_puc_PK1[1] = (UC)((l_x_UserData.m_us_PK1_Size - l_i_ToAdd));
			memcpy(l_x_UserData.m_puc_PK1 + l_i_ToAdd, i_x_UserData->m_px_Buffer[0].m_puc_Buffer, l_x_UserData.m_us_PK1_Size - l_i_ToAdd);

			if (i_x_UserData->m_uc_NbFingers == 1)	//0=1 finger /1=2 finger 
			{
				l_x_UserData.m_us_PK2_Size = i_x_UserData->m_px_Buffer[1].m_us_Size + l_i_ToAdd;
				l_x_UserData.m_puc_PK2[0] = (UC)((l_x_UserData.m_us_PK2_Size - l_i_ToAdd) >> 8);
				l_x_UserData.m_puc_PK2[1] = (UC)((l_x_UserData.m_us_PK2_Size - l_i_ToAdd));
				memcpy(l_x_UserData.m_puc_PK2 + l_i_ToAdd, i_x_UserData->m_px_Buffer[1].m_puc_Buffer, l_x_UserData.m_us_PK2_Size - l_i_ToAdd);
			}
			else
			{
				l_x_UserData.m_us_PK2_Size = 0;
			}

			l_i_Ret = i_px_SagemBioDemoCard->Update(&l_x_UserData, (T_SMART_CARD_IHM_CALLBACK)&SmartCartIHMCallback, this);
		}

		if (l_i_Ret == MORPHO_OK)
		{
			//Power off the card
			l_i_Ret = i_px_SagemBioDemoCard->PowerOff();
		}

	}

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I CMSO_DemoDlg::OpenSmartCardAndExtractData
(
T_USER_DATA *o_x_UserData,
CBioDemoCard	*i_px_SagemBioDemoCard
)
{
	T_CBioDemoCard_USER_DATA	l_x_UserData;
	I	l_i_Ret;

	//Init
	o_x_UserData->m_cs_ID = "";
	o_x_UserData->m_cs_Firstname = "";
	o_x_UserData->m_cs_Lastname = "";
	o_x_UserData->m_uc_NbFingers = 0;
	o_x_UserData->m_px_Buffer = NULL;
	o_x_UserData->m_px_pk_X984 = NULL;

	l_i_Ret = OpenSmartCard(i_px_SagemBioDemoCard);

	if (l_i_Ret == MORPHO_OK)
	{
		//read data
		l_i_Ret = i_px_SagemBioDemoCard->Read(&l_x_UserData, (T_SMART_CARD_IHM_CALLBACK)&SmartCartIHMCallback, this, m_BioOpPSheet.m_VerifyPPage.m_i_NbFingers + 1);
	}

	if (l_i_Ret == MORPHO_OK)
	{
		//Power off the card
		l_i_Ret = CloseSmartCard(i_px_SagemBioDemoCard);
	}

	if (l_i_Ret == MORPHO_OK)
	{
		o_x_UserData->m_cs_Firstname = l_x_UserData.m_puc_Name;
		o_x_UserData->m_cs_Lastname = l_x_UserData.m_puc_Surname;
		o_x_UserData->m_cs_ID = l_x_UserData.m_puc_Identifier;

		if (l_x_UserData.m_us_PK2_Size != 0)
			o_x_UserData->m_uc_NbFingers = 1;	//0 means 1 finger, 1 means 2 finger

		o_x_UserData->m_px_Buffer = (PT_BUFFER)malloc((o_x_UserData->m_uc_NbFingers + 1)*sizeof(T_BUFFER));

		if (l_x_UserData.m_us_PK1_Size != 0)
		{
			o_x_UserData->m_px_Buffer[0].m_us_Size = l_x_UserData.m_us_PK1_Size,
				o_x_UserData->m_px_Buffer[0].m_puc_Buffer = (PUC)malloc(o_x_UserData->m_px_Buffer[0].m_us_Size);
			memcpy(o_x_UserData->m_px_Buffer[0].m_puc_Buffer, l_x_UserData.m_puc_PK1, l_x_UserData.m_us_PK1_Size);
		}
		if (l_x_UserData.m_us_PK2_Size != 0)
		{
			o_x_UserData->m_px_Buffer[1].m_us_Size = l_x_UserData.m_us_PK2_Size,
				o_x_UserData->m_px_Buffer[1].m_puc_Buffer = (PUC)malloc(o_x_UserData->m_px_Buffer[1].m_us_Size);
			memcpy(o_x_UserData->m_px_Buffer[1].m_puc_Buffer, l_x_UserData.m_puc_PK2, l_x_UserData.m_us_PK2_Size);
		}
	}

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I CMSO_DemoDlg::OpenSmartCard
(
CBioDemoCard	*i_px_SagemBioDemoCard
)
{
	CString l_str_ReaderName;
	I	l_i_NbReader;
	I	l_i_Ret;

	//Enumerate connected smart card reader
	l_i_Ret = i_px_SagemBioDemoCard->EnumReader(&l_i_NbReader);

	//select first smart card reader connected
	if (l_i_Ret == MORPHO_OK)
	{
		l_str_ReaderName = i_px_SagemBioDemoCard->GetReaderName(0);

		//Power on the card
		l_i_Ret = i_px_SagemBioDemoCard->PowerOn(l_str_ReaderName);
	}

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I CMSO_DemoDlg::CloseSmartCard
(
CBioDemoCard	*i_px_SagemBioDemoCard
)
{
	CString l_str_ReaderName;
	I	l_i_NbReader;
	I	l_i_Ret;

	DisableButtons();
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);

	//Enumerate connected smart card reader
	l_i_Ret = i_px_SagemBioDemoCard->EnumReader(&l_i_NbReader);

	//select first smart card reader connected
	if (l_i_Ret == MORPHO_OK)
	{
		l_str_ReaderName = i_px_SagemBioDemoCard->GetReaderName(0);
		//Power off the card
		l_i_Ret = i_px_SagemBioDemoCard->PowerOff();

	}

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::EnableDisableSmartCardControls()
{
	I	l_i_NbReader;
	CSCardReader	*l_px_CardReader;

	l_px_CardReader = new CSCardReader(SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1);

	BOOL		l_b_EnableControls = FALSE;

	l_px_CardReader->EnumReader(&l_i_NbReader);
	if (l_i_NbReader != 0)
	{
		l_b_EnableControls = TRUE;
	}

	delete l_px_CardReader;

	m_BioOpPSheet.EnableDisableSmartCardControls(l_b_EnableControls);

	//bool l_b_cardReadMso = IsMSOCardReader();

	if (!l_b_EnableControls)//|| !l_b_cardReadMso)
	{
		m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_MocPPage));
	}
}

//////////////////////////////////////////////////////////////////
bool CMSO_DemoDlg::IsMSOCardReader()
{
	CString l_cs_ProductDescriptor(m_x_MorphoSmart.m_puc_ProductDescriptor);

	int l_i_ret = 0;
	l_i_ret = l_cs_ProductDescriptor.Find("MSO35");
	if (l_i_ret != -1)
	{
		return true;
	}
	l_i_ret = l_cs_ProductDescriptor.Find("MSO135");
	if (l_i_ret != -1)
	{
		return true;
	}
	return false;

}
/////////////////////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::OnCheckNoCheck()
{
	if (m_b_NoCheck == FALSE)
	{
		m_b_NoCheck = TRUE;
	}
	else
	{
		m_b_NoCheck = FALSE;
	}
}

I OpenAndReadFile
(
PUC		i_puc_FileName,
PUC		o_puc_Buffread,
PUL		i_pul_BuffSizeToRead
)
{
	CFile	l_f_SecretFile;
	CString	l_cs_Title;

	memset(o_puc_Buffread, 0, *i_pul_BuffSizeToRead);

	CFileDialog l_x_OpenFileDlg(TRUE,
		"*.*",
		(PC)i_puc_FileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
		"All Files (*.*)|*.*||",
		NULL);

	SAV_PATH

		l_cs_Title.Format("Open a file containing the %s", i_puc_FileName);

	l_x_OpenFileDlg.m_ofn.lpstrTitle = (LPCTSTR)l_cs_Title;

	if (l_x_OpenFileDlg.DoModal() != IDOK)
	{
		RESTORE_PATH
			return MORPHOERR_DEVICE_LOCKED;
	}

	RESTORE_PATH

		l_f_SecretFile.Open(l_x_OpenFileDlg.GetPathName(), CFile::modeRead | CFile::typeBinary);

	*i_pul_BuffSizeToRead = l_f_SecretFile.Read(o_puc_Buffread, *i_pul_BuffSizeToRead);

	l_f_SecretFile.Close();

	return MORPHO_OK;
}

I CMSO_DemoDlg::UnlockDevice()
{
	I		l_i_Ret;
	PUC		i_puc_Seed;
	UL		i_ul_SeedSize;
	CString l_cs_BoxMessage;

	UC		l_auc_SecretID[LOCK_ID_KEY_SIZE];
	UL		l_ul_SecretIDLen;
	UC		l_auc_Secret[LOCK_KEY_SIZE];
	UL		l_ul_SecretLen;

	UC		l_auc_Ksecret_KCV[KCV_LEN];

	l_i_Ret = MORPHO_OK;

	l_cs_BoxMessage = "The Device is locked, you must unlock it loading the\ntwo binary Secret files (the SecretID and the Secret)";
	l_i_Ret = AfxMessageBox(l_cs_BoxMessage, MB_OKCANCEL);
	if (l_i_Ret != IDOK)
	{
		return MORPHOERR_DEVICE_LOCKED;
	}

	l_cs_BoxMessage = "Do you firstly want to check the value of the Secret ?\n(you will only need to load the Secret ID binary)";
	l_i_Ret = AfxMessageBox(l_cs_BoxMessage, MB_YESNO);
	if (l_i_Ret == IDYES)
	{
		// Load the Secret ID of the Secret whose value is to be checked
		l_ul_SecretIDLen = sizeof(l_auc_SecretID);
		l_i_Ret = OpenAndReadFile((PUC)"Secret ID.bin", l_auc_SecretID, &l_ul_SecretIDLen);

		if (l_i_Ret != MORPHO_OK)
		{
			return l_i_Ret;
		}

		// Get & dispalay Secret KCV
		l_i_Ret = m_x_MorphoSmart.GetKCV(ID_KSECRET, l_auc_SecretID, l_auc_Ksecret_KCV);
		if (l_i_Ret == MORPHO_OK)
		{
			l_cs_BoxMessage.Format("Secret KCV :  %02X %02X %02X\n\n" \
				"Do you want to load the binary Secret files now ?\n" \
				"(you will be asked to reload the Secret ID binary)", \
				l_auc_Ksecret_KCV[0], l_auc_Ksecret_KCV[1], l_auc_Ksecret_KCV[2]);
			l_i_Ret = AfxMessageBox(l_cs_BoxMessage, MB_YESNO);

			if (l_i_Ret == IDNO)
			{
				return MORPHOERR_DEVICE_LOCKED;
			}
		}
		else
		{
			AfxMessageBox("Error while trying to retrieve Secret KCV : wrong Key/Secret ID!");
			return MORPHOERR_DEVICE_LOCKED;
		}
	}

	l_ul_SecretIDLen = sizeof(l_auc_SecretID);
	l_i_Ret = OpenAndReadFile((PUC)"Secret ID.bin", l_auc_SecretID, &l_ul_SecretIDLen);
	if (l_i_Ret != MORPHO_OK)
	{
		return l_i_Ret;
	}

	l_ul_SecretLen = sizeof(l_auc_Secret);
	l_i_Ret = OpenAndReadFile((PUC)"Secret.bin", l_auc_Secret, &l_ul_SecretLen);
	if (l_i_Ret != MORPHO_OK)
	{
		return l_i_Ret;
	}

	l_i_Ret = m_x_MorphoSmart.GetUnlockSeed(i_puc_Seed, &i_ul_SeedSize);
	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling C_MORPHO_Device::GetUnlockSeed() function", l_i_Ret);
		return l_i_Ret;
	}

	l_i_Ret = MSOSECU_TripleDesCryptByKey(l_auc_Secret, i_ul_SeedSize, i_puc_Seed, i_puc_Seed);
	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling MSOSECU_TripleDesCryptByKey() function", l_i_Ret);
		return l_i_Ret;
	}

	l_i_Ret = m_x_MorphoSmart.Unlock(l_auc_SecretID, LOCK_ID_KEY_SIZE, i_puc_Seed, i_ul_SeedSize);
	if (l_i_Ret != MORPHO_OK)
	{
		ShowErrorBox("An error occured while calling C_MORPHO_Device::Unlock() function", l_i_Ret);
		return l_i_Ret;
	}

	return l_i_Ret;
}
void CMSO_DemoDlg::OnLoadMasterKey()
{
	UC		l_auc_MasterKeys[2 * DES_TRIPLE_KEY_LEN];
	UL		l_u_MasterKeysLen;
	I		l_i_Ret;
	CResultDlg	l_x_ResultDlg;

	l_x_ResultDlg.m_cs_Msg1.Format("Load KEnc");

	l_u_MasterKeysLen = sizeof(l_auc_MasterKeys);
	l_i_Ret = OpenAndReadFile((PUC)"MasterKeys.bin", l_auc_MasterKeys, &l_u_MasterKeysLen);
	if (l_i_Ret != MORPHO_OK)
	{
		return;
	}

	l_i_Ret = m_x_MorphoSmart.LoadMasterKeys(l_auc_MasterKeys, l_auc_MasterKeys + DES_TRIPLE_KEY_LEN);

	if (l_i_Ret == MORPHO_OK)
	{
		l_x_ResultDlg.m_cs_Msg2.Format("Successful");
		Beep(BEEP_FREQUENCY, BEEP_DURATION);
	}
	else
	{
		l_x_ResultDlg.m_cs_Msg2.Format("Failed");
		ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);

		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
	}

	l_x_ResultDlg.DoModal();
}

void CMSO_DemoDlg::OnLoadMocKEnc()
{
	I		l_i_Ret;

	UC			l_auc_MocKEnc[DES_DOUBLE_KEY_LEN];
	UL			l_ul_MocKencLen;

	// Use the compatible Solution
	UC			l_auc_MocKEnc_Trans[DES_TRIPLE_KEY_LEN];
	UL			l_ul_MocKenc_TransLen;
	CResultDlg	l_x_ResultDlg;

	// Use the Secured Solution
	PUC		l_puc_MocKEncCiphered;
	UL		l_ul_MocKencLenCiphered;
	UC		l_auc_CertifHost[10 * 1024];
	UL		l_ul_CertifHostSize;
	UC		l_auc_KeyHost[10 * 1024];
	UL		l_ul_KeyHostSize;
	PUC		l_puc_Signature;
	UL		l_ul_SignatureLen;

	l_i_Ret = MORPHO_OK;

	l_x_ResultDlg.m_cs_Msg1.Format("Load KEnc");
	l_ul_MocKencLen = sizeof(l_auc_MocKEnc);
	l_i_Ret = OpenAndReadFile((PUC)"MocKEnc.bin", l_auc_MocKEnc, &l_ul_MocKencLen);
	if (l_i_Ret != MORPHO_OK)
	{
		return;
	}


	//BUG CORRIGE PAR C++TEST [BD-PB-CC-2]
	if (m_puc_MsoCertif == NULL)
	{
		//read the MorphoSmartTM certification 
		//the MorphoSmartTM certification is stored in file .tkb
		l_i_Ret = m_x_MorphoSmart.SecuReadCertificate(
			0,						//MorphoSmartTM level	//UC			i_uc_index,			
			&m_ul_MsoCertifSize,	//PUL			o_pul_CertifSize,
			&m_puc_MsoCertif		//PUC*			o_ppuc_Certif
			);
		l_i_Ret = MORPHO_OK;
	}


	if (l_i_Ret == MORPHO_OK)
	{
		if (m_puc_MsoCertif == NULL)
		{
			// Use the Compatible Solution
			l_ul_MocKenc_TransLen = sizeof(l_auc_MocKEnc_Trans);
			l_i_Ret = OpenAndReadFile((PUC)"MocKEnc_Trans.bin", l_auc_MocKEnc_Trans, &l_ul_MocKenc_TransLen);
			if (l_i_Ret != MORPHO_OK)
			{
				l_x_ResultDlg.m_cs_Msg3.Format("OpenAndReadFile() error");
				return;
			}

			else //BUG CORRIGE PAR C++TEST [BD-PB-CC-2]
			{
				l_i_Ret = MSOSECU_TripleDesCryptByKey(l_auc_MocKEnc_Trans, l_ul_MocKencLen, l_auc_MocKEnc, l_auc_MocKEnc);
				if (l_i_Ret != MORPHO_OK)
				{
					l_x_ResultDlg.m_cs_Msg3.Format("MSOSECU_TripleDesCryptByKey() error");
				}
			}

			if (l_i_Ret == MORPHO_OK)
			{
				l_i_Ret = m_x_MorphoSmart.LoadMocKey(l_auc_MocKEnc, l_ul_MocKencLen);
				if (l_i_Ret != MORPHO_OK)
				{
					l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Device::LoadMocKey() error");
				}
			}
		}
		else
		{
			// Use the Secured Solution

			//BUG CORRIGE PAR C++TEST [BD-PB-CC-2]
			l_ul_KeyHostSize = sizeof(l_auc_KeyHost);
			l_i_Ret = OpenAndReadFile((PUC)"Host.key", l_auc_KeyHost, &l_ul_KeyHostSize);
			if (l_i_Ret != MORPHO_OK)
			{
				l_x_ResultDlg.m_cs_Msg3.Format("OpenAndReadFile() error");
				return;
			}

			//BUG CORRIGE PAR C++TEST [BD-PB-CC-2]

			l_ul_CertifHostSize = sizeof(l_auc_CertifHost);
			l_i_Ret = OpenAndReadFile((PUC)"Host.der", l_auc_CertifHost, &l_ul_CertifHostSize);
			if (l_i_Ret != MORPHO_OK)
			{
				l_x_ResultDlg.m_cs_Msg3.Format("OpenAndReadFile() error");
				return;
			}


			l_i_Ret = MSOSECU_RSA_Encrypt(m_ul_MsoCertifSize, m_puc_MsoCertif, sizeof(l_auc_MocKEnc), l_auc_MocKEnc, &l_ul_MocKencLenCiphered, &l_puc_MocKEncCiphered);
			if (l_i_Ret != MORPHO_OK)
			{
				l_x_ResultDlg.m_cs_Msg3.Format("MSOSECU_RSA_Encrypt() error");
			}

			if (l_i_Ret == MORPHO_OK)
			{
				l_i_Ret = MSOSECU_SignRSA(
					l_ul_KeyHostSize, l_auc_KeyHost,
					l_ul_MocKencLenCiphered, l_puc_MocKEncCiphered,
					&l_ul_SignatureLen, &l_puc_Signature);
			}


			if (l_i_Ret == MORPHO_OK)
			{
				l_i_Ret = m_x_MorphoSmart.LoadMocKey(
					l_puc_MocKEncCiphered, l_ul_MocKencLenCiphered,
					l_puc_Signature, l_ul_SignatureLen,
					l_auc_CertifHost, l_ul_CertifHostSize
					);
			}

			if (l_i_Ret != MORPHO_OK)
			{
				l_x_ResultDlg.m_cs_Msg3.Format("C_MORPHO_Device::LoadMocKey() error");
			}
		}
	}

	if (m_puc_MsoCertif != NULL)
	{
		MSOSECU_Free(&l_puc_MocKEncCiphered);
	}

	if (l_i_Ret == MORPHO_OK)
	{
		l_x_ResultDlg.m_cs_Msg2.Format("Successful");
		Beep(BEEP_FREQUENCY, BEEP_DURATION);
	}
	else
	{
		CString l_cs_Message;

		l_x_ResultDlg.m_cs_Msg2.Format("Failed");
		ConvertSDKError(l_i_Ret, l_x_ResultDlg.m_cs_Msg4);
		if (l_i_Ret == MORPHOERR_STATUS)
		{
			l_cs_Message.Format(" Internal Err =%d", m_x_MorphoSmart.GetInternalError());
			l_x_ResultDlg.m_cs_Msg4 += l_cs_Message;
		}
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
	}

	l_x_ResultDlg.DoModal();
}

void CMSO_DemoDlg::OnDisplayMocKencKcv()
{
	I			l_i_Ret;
	CString		l_cs_Message;
	UC			l_auc_Kenc_KCV[KCV_LEN];

	// Get Kenc KCV from MSO
	l_i_Ret = m_x_MorphoSmart.GetKCV(ID_KENC, NULL, l_auc_Kenc_KCV);

	// Display Kenc KCV
	if (l_i_Ret)
	{
		l_cs_Message.Format("Error while trying to retrieve Kenc KCV !");
	}
	else
	{
		l_cs_Message.Format("Kenc KCV :  %02X %02X %02X", l_auc_Kenc_KCV[0], l_auc_Kenc_KCV[1], l_auc_Kenc_KCV[2]);
	}

	AfxMessageBox(l_cs_Message);
}

void CMSO_DemoDlg::OnUncompressImage()
{
	I		l_i_Ret;

	T_MORPHO_IMAGE l_x_Image;
	PUC		l_puc_ImageCompressed = NULL;
	UL		l_ul_ImageCompressedLen;

	PUC		l_puc_ImageUnCompressed = NULL;
	UL		l_ul_ImageUnCompressedLen;

	CString		l_cs_CompressedFileName;
	CString		l_cs_ImageFileName;
	CFile		l_f_CompressedFile;
	CFile		l_f_RawFile;
	CResultDlg	l_x_ResultDlg;

	l_i_Ret = MORPHO_OK;


	CFileDialog l_x_OpenFileDlg(TRUE,
		"*.bin",
		(PC)"CompressedImage.bin",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
		"Binaries Files (*.bin)|*.bin;*.bin | WSQ Files (*.wsq) |*.wsq;*.wsq | All Files (*.*)|*.*||",
		NULL);


	SAV_PATH

		if (l_x_OpenFileDlg.DoModal() != IDOK)
		{
			l_i_Ret = MORPHOERR_MEMORY_PC;
			l_x_ResultDlg.m_cs_Msg1.Format("Uncompress Error");
			l_x_ResultDlg.m_cs_Msg2.Format("aborted by user.");
			l_x_ResultDlg.m_cs_Msg3.Format("");
			l_x_ResultDlg.m_cs_Msg4.Format("");
		}

	RESTORE_PATH

		CString l_cs_CompressedFileExt = l_x_OpenFileDlg.GetFileExt();
	if (l_cs_CompressedFileExt == "wsq")
	{
		AfxMessageBox("For a free WSQ viewer please visit \n http://www.cognaxon.com");
		ShellExecute(NULL, "open", "http://www.cognaxon.com", NULL, NULL, SW_SHOWNORMAL);
		return;
	}


	if (l_i_Ret == MORPHO_OK)
	{
		l_cs_CompressedFileName = l_x_OpenFileDlg.GetPathName();

		if (!l_f_CompressedFile.Open(l_cs_CompressedFileName, CFile::modeRead | CFile::typeBinary))
		{
			l_i_Ret = MORPHOERR_MEMORY_PC;
			l_x_ResultDlg.m_cs_Msg1.Format("Uncompress Error");
			l_x_ResultDlg.m_cs_Msg2.Format("Unable to open");
			l_x_ResultDlg.m_cs_Msg3.Format("File :");
			l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_CompressedFileName);
		}
	}


	if (l_i_Ret == MORPHO_OK)
	{
		l_ul_ImageCompressedLen = (UL)l_f_CompressedFile.GetLength();

		l_puc_ImageCompressed = (PUC)malloc(l_ul_ImageCompressedLen);
		if (l_puc_ImageCompressed == NULL)
		{
			l_i_Ret = MORPHOERR_MEMORY_PC;
			l_x_ResultDlg.m_cs_Msg1.Format("Uncompress Error");
			l_x_ResultDlg.m_cs_Msg2.Format("Allocation Error ");
			l_x_ResultDlg.m_cs_Msg3.Format("");
			l_x_ResultDlg.m_cs_Msg4.Format("");
		}
	}

	if (l_i_Ret == MORPHO_OK)
	{
		if (l_f_CompressedFile.Read(l_puc_ImageCompressed, l_ul_ImageCompressedLen) != l_ul_ImageCompressedLen)
		{
			l_i_Ret = MORPHOERR_MEMORY_PC;
			l_x_ResultDlg.m_cs_Msg1.Format("Uncompress Error");
			l_x_ResultDlg.m_cs_Msg2.Format("Unable to open read");
			l_x_ResultDlg.m_cs_Msg3.Format("File :");
			l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_CompressedFileName);
		}

		l_f_CompressedFile.Close();
	}

	if (l_i_Ret == MORPHO_OK)
	{
		l_i_Ret = COMPRESS_GetRawImageSize(l_puc_ImageCompressed, l_ul_ImageCompressedLen, &l_ul_ImageUnCompressedLen);
		if (l_i_Ret != MORPHO_OK)
		{
			l_i_Ret = MORPHOERR_MEMORY_PC;
			l_x_ResultDlg.m_cs_Msg1.Format("Uncompress Error");
			l_x_ResultDlg.m_cs_Msg2.Format("COMPRESS_GetRawImageSize Error ");
			l_x_ResultDlg.m_cs_Msg3.Format("");
			l_x_ResultDlg.m_cs_Msg4.Format("");
		}
	}

	if (l_i_Ret == MORPHO_OK)
	{
		l_puc_ImageUnCompressed = (PUC)malloc(l_ul_ImageUnCompressedLen);
		if (l_puc_ImageUnCompressed == NULL)
		{
			l_i_Ret = MORPHOERR_MEMORY_PC;
			l_x_ResultDlg.m_cs_Msg1.Format("Uncompress Error");
			l_x_ResultDlg.m_cs_Msg2.Format("Allocation Error ");
			l_x_ResultDlg.m_cs_Msg3.Format("");
			l_x_ResultDlg.m_cs_Msg4.Format("");
		}
	}

	if (l_i_Ret == MORPHO_OK)
	{
		l_i_Ret = COMPRESS_UnCompressImage(
			l_puc_ImageCompressed,
			l_ul_ImageCompressedLen,
			l_puc_ImageUnCompressed,
			&l_ul_ImageUnCompressedLen
			);
		if (l_i_Ret != MORPHO_OK)
		{
			l_i_Ret = MORPHOERR_MEMORY_PC;
			l_x_ResultDlg.m_cs_Msg1.Format("Uncompress Error");
			l_x_ResultDlg.m_cs_Msg2.Format("COMPRESS_UnCompressImage Error ");
			l_x_ResultDlg.m_cs_Msg3.Format("");
			l_x_ResultDlg.m_cs_Msg4.Format("");
		}
	}

	if (l_i_Ret == MORPHO_OK)
	{
		l_x_ResultDlg.m_cs_Msg1.Format("Uncompress Successful");

		memcpy(&l_x_Image.m_x_ImageHeader, (T_MORPHO_IMAGE_HEADER*)l_puc_ImageUnCompressed, sizeof(T_MORPHO_IMAGE_HEADER));
		l_x_Image.m_puc_Image = (PUC)l_puc_ImageUnCompressed + sizeof(T_MORPHO_IMAGE_HEADER);

		//------------------------------------------------------------
		// Display Data
		//------------------------------------------------------------

		CDisplayImageDlg l_x_DisplayImageDlg(&l_x_Image);
		l_x_DisplayImageDlg.DoModal();

		//------------------------------------------------------------
		// Save Data
		//------------------------------------------------------------

		l_cs_ImageFileName.Format("Image[%04dx%04dx00].raw", l_x_Image.m_x_ImageHeader.m_us_NbRow, l_x_Image.m_x_ImageHeader.m_us_NbCol);
		CFileDialog l_x_SaveFileDlg(FALSE,
			"*.raw",
			l_cs_ImageFileName,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
			"Raw Files (*.raw)|*.raw|All Files (*.*)|*.*||",
			this);

		SAV_PATH

			if (l_x_SaveFileDlg.DoModal() == IDOK)
			{
				RESTORE_PATH
					l_cs_ImageFileName = l_x_SaveFileDlg.GetPathName();

				//- - - - - - - - - - - 
				// Write data in File
				//- - - - - - - - - - -
				if (!l_f_RawFile.Open(l_cs_ImageFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
				{
					l_i_Ret = MORPHOERR_MEMORY_PC;
					l_x_ResultDlg.m_cs_Msg1.Format("Uncompress Error");
					l_x_ResultDlg.m_cs_Msg2.Format("Unable to open");
					l_x_ResultDlg.m_cs_Msg3.Format("File :");
					l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_CompressedFileName);
				}

				if (l_i_Ret == MORPHO_OK)
				{
					l_f_RawFile.Write(l_x_Image.m_puc_Image, l_x_Image.m_x_ImageHeader.m_us_NbRow*l_x_Image.m_x_ImageHeader.m_us_NbCol);

					l_x_ResultDlg.m_cs_Msg2.Format("Image successfully ");
					l_x_ResultDlg.m_cs_Msg3.Format("exported in file");
					l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_ImageFileName);
				}

				l_f_RawFile.Close();
			}
	}

	if (l_i_Ret == MORPHO_OK)
	{
		Beep(BEEP_FREQUENCY, BEEP_DURATION);
	}
	else
	{
		//Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
	}

	l_x_ResultDlg.DoModal();

	if (l_puc_ImageCompressed != NULL)
	{
		free(l_puc_ImageCompressed);
	}

	if (l_puc_ImageUnCompressed != NULL)
	{
		free(l_puc_ImageUnCompressed);
	}

}

void CMSO_DemoDlg::OnConvertISO19794_FIR()
{
	PC						l_pc_SelectedFilePathBuffer;
	DWORD					l_ul_SelectedFilePathBufferLenght;
	I						l_i_Ret, l_i_NumberFiles, l_i_CurImage;
	I						l_i_ImageWidth, l_i_ImageHeight;
	PUC						l_puc_ImageCompressed;
	UL						l_ul_ImageCompressedLen;
	CString					l_cs_CompressedFileName;
	CString					l_cs_ISO19794_FIR_OutputFileName;
	CFile					l_f_CompressedFile;
	CResultDlg				l_x_ResultDlg;
	PT_MORPHO_ISO19794_FIR	l_px_ISO19794_FIR;
	CString					l_cs_CompressedFileExt;
	T_MORPHO_COMPRESS_ALGO	l_x_DesiredImageCompression;
	T_MORPHO_COMPRESS_ALGO	l_x_ImageCompression;
	ISO19794_FIR_InfoDlg	l_x_ISO19794_FIR_InfoDlg;
	PT_MORPHO_IMAGE_INFO*	l_ppx_ImagesInfo;
	T_MORPHO_IMAGE**		l_ppx_Image;
	POSITION				l_pos_FilePosition;


	l_i_Ret = MORPHO_OK;
	l_puc_ImageCompressed = NULL;
	l_px_ISO19794_FIR = NULL;
	l_i_NumberFiles = 0;
	l_i_CurImage = 0;
	l_ppx_Image = NULL;
	l_ppx_ImagesInfo = NULL;
	l_i_ImageWidth = 0;
	l_i_ImageHeight = 0;
	l_pc_SelectedFilePathBuffer = NULL;
	l_ul_SelectedFilePathBufferLenght = 8192;

	CFileDialog l_x_OpenFileDlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT | OFN_ALLOWMULTISELECT,
		"RAW Images (*.raw)|*.raw|BMP Images (*.bmp)|*.bmp|WSQ Images (*.wsq) |*.wsq|JPEG Images (*.jpg)|*.jpg|JPEG 2000 Images (*.jp2)|*.jp2|PNG Images (*.png)|*.png|All Files (*.*)|*.*||",
		NULL);

	// Allocate temporary OpenFileDialog file path buffer
	l_pc_SelectedFilePathBuffer = new C[l_ul_SelectedFilePathBufferLenght];

	if (l_pc_SelectedFilePathBuffer == NULL)
		l_i_Ret = MORPHOERR_MEMORY_PC;

	if (l_i_Ret == MORPHO_OK)
	{
		// Prepare OpenFileDialog
		memset(l_pc_SelectedFilePathBuffer, 0, l_ul_SelectedFilePathBufferLenght * sizeof(C));
		l_x_OpenFileDlg.m_ofn.nMaxFile = l_ul_SelectedFilePathBufferLenght;
		l_x_OpenFileDlg.m_ofn.lpstrFile = l_pc_SelectedFilePathBuffer;

		SAV_PATH

			// Display OpenFileDialog
			if (l_x_OpenFileDlg.DoModal() != IDOK)
			{
				l_i_Ret = MORPHOERR_MEMORY_PC;
				l_x_ResultDlg.m_cs_Msg1.Format("Convert ISO-19794-4 Error");
				l_x_ResultDlg.m_cs_Msg2.Format("aborted by user.");
				//l_x_ResultDlg.m_cs_Msg3.Format("");
				//l_x_ResultDlg.m_cs_Msg4.Format("");
			}

		RESTORE_PATH
	}

	// To retrieve selected files number
	if (l_i_Ret == MORPHO_OK)
	{
		l_pos_FilePosition = l_x_OpenFileDlg.GetStartPosition();

		while (l_pos_FilePosition)
		{
			l_x_OpenFileDlg.GetNextPathName(l_pos_FilePosition);
			l_i_NumberFiles++;
		}
	}

	// traitment of selected files and fill l_ppx_ImagesInfo and l_ppx_Image
	if (l_i_Ret == MORPHO_OK)
	{
		l_ppx_ImagesInfo = new PT_MORPHO_IMAGE_INFO[l_i_NumberFiles];
		l_ppx_Image = new T_MORPHO_IMAGE*[l_i_NumberFiles];

		l_pos_FilePosition = l_x_OpenFileDlg.GetStartPosition();

		while (l_pos_FilePosition && (l_i_Ret == MORPHO_OK))
		{
			l_cs_CompressedFileName = l_x_OpenFileDlg.GetNextPathName(l_pos_FilePosition);

			// vérification ouverture fichier
			if (!l_f_CompressedFile.Open(l_cs_CompressedFileName, CFile::modeRead | CFile::typeBinary))
			{
				l_i_Ret = MORPHOERR_MEMORY_PC;
				l_x_ResultDlg.m_cs_Msg1.Format("Convert ISO-19794-4 Error");
				l_x_ResultDlg.m_cs_Msg5.Format("Unable to open file");
				l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_CompressedFileName);
			}

			// Obtention longueur fichier
			if (l_i_Ret == MORPHO_OK)
			{
				l_ul_ImageCompressedLen = (UL)l_f_CompressedFile.GetLength();

				l_puc_ImageCompressed = (PUC)malloc(l_ul_ImageCompressedLen);
				if (l_puc_ImageCompressed == NULL)
				{
					l_i_Ret = MORPHOERR_MEMORY_PC;
					l_x_ResultDlg.m_cs_Msg1.Format("Convert ISO-19794-4 Error");
					l_x_ResultDlg.m_cs_Msg2.Format("Allocation Error ");
					//l_x_ResultDlg.m_cs_Msg5.Format("");
					//l_x_ResultDlg.m_cs_Msg4.Format("");
				}
			}

			// Lecture fichier
			if (l_i_Ret == MORPHO_OK)
			{
				if (l_f_CompressedFile.Read(l_puc_ImageCompressed, l_ul_ImageCompressedLen) != l_ul_ImageCompressedLen)
				{
					l_i_Ret = MORPHOERR_MEMORY_PC;
					l_x_ResultDlg.m_cs_Msg1.Format("Convert ISO-19794-4 Error");
					l_x_ResultDlg.m_cs_Msg5.Format("Unable to open read file");
					l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_CompressedFileName);
				}
				// Fermeture fichier
				l_f_CompressedFile.Close();
			}

			// vérification extension
			if (l_i_Ret == MORPHO_OK)
			{
				l_cs_CompressedFileExt = l_cs_CompressedFileName.Right(3).MakeUpper();

				if (l_cs_CompressedFileExt == "JPG")
				{
					l_x_DesiredImageCompression = MORPHO_COMPRESS_UNCHANGED;
					l_x_ImageCompression = MORPHO_COMPRESS_JPEG;
				}
				else if (l_cs_CompressedFileExt == "JP2")
				{
					l_x_DesiredImageCompression = MORPHO_COMPRESS_UNCHANGED;
					l_x_ImageCompression = MORPHO_COMPRESS_JPEG2000;
				}
				else if (l_cs_CompressedFileExt == "BMP")
				{
					l_x_DesiredImageCompression = MORPHO_COMPRESS_UNCHANGED;
					l_x_ImageCompression = MORPHO_COMPRESS_BMP;
				}
				else if (l_cs_CompressedFileExt == "WSQ")
				{
					l_x_DesiredImageCompression = MORPHO_COMPRESS_UNCHANGED;
					l_x_ImageCompression = MORPHO_COMPRESS_WSQ;
				}
				else if (l_cs_CompressedFileExt == "PNG")
				{
					l_x_DesiredImageCompression = MORPHO_COMPRESS_UNCHANGED;
					l_x_ImageCompression = MORPHO_COMPRESS_PNG;
				}
				else if (l_cs_CompressedFileExt == "RAW")
				{
					l_x_ISO19794_FIR_InfoDlg.m_b_ShowDesiredCompressComoBox = true;
					l_x_ImageCompression = MORPHO_NO_COMPRESS;
				}
				else // Extension non supportée
				{
					l_i_Ret = MORPHOERR_MEMORY_PC;
					l_x_ResultDlg.m_cs_Msg1.Format("Convert ISO-19794-4 Error");
					l_x_ResultDlg.m_cs_Msg5.Format("Unknown extension");
					l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_CompressedFileExt);
				}

				// Popup champs additionnels ISO-19794-4 FIR
				if ((l_i_Ret == MORPHO_OK))
				{
					// Set image buffer size only in RAW mode in order to predict image dimensions
					if (l_cs_CompressedFileExt == "RAW")
						l_x_ISO19794_FIR_InfoDlg.m_ul_ImageBufferSize = l_ul_ImageCompressedLen;

					// Display popup
					if ((l_i_CurImage == 0) && (l_x_ISO19794_FIR_InfoDlg.DoModal() == IDOK))
					{
						// Get RAW image dimension given by user
						l_i_ImageWidth = atoi(l_x_ISO19794_FIR_InfoDlg.m_cs_NBCols);
						l_i_ImageHeight = atoi(l_x_ISO19794_FIR_InfoDlg.m_cs_NBRows);

						if (l_cs_CompressedFileExt == "RAW")
						{
							// If RAW image dimension given by user doesn't match the real image size, then return an error
							if ((l_i_ImageWidth * l_i_ImageHeight) != l_ul_ImageCompressedLen)
							{
								l_i_Ret = MORPHOERR_MEMORY_PC;
								l_x_ResultDlg.m_cs_Msg1.Format("Convert ISO-19794-4 Error");
								l_x_ResultDlg.m_cs_Msg2.Format("Invalid image dimension");
								l_x_ResultDlg.m_cs_Msg5.Format("Given buffer is too small");
								l_x_ResultDlg.m_cs_Msg4.Format("%d x %d > %d", l_i_ImageWidth, l_i_ImageHeight, l_ul_ImageCompressedLen);
							}
							else
							{
								l_x_DesiredImageCompression = l_x_ISO19794_FIR_InfoDlg.m_x_DesiredCompression;

								// l_cs_CompressedFileExt is used now in the output file name (ex: ISO19794-JPG)
								if (l_x_DesiredImageCompression == MORPHO_COMPRESS_JPEG)
								{
									l_cs_CompressedFileExt = "JPG";
								}
								else if (l_x_DesiredImageCompression == MORPHO_COMPRESS_JPEG2000)
								{
									l_cs_CompressedFileExt = "JP2";
								}
								else if (l_x_DesiredImageCompression == MORPHO_COMPRESS_PNG)
								{
									l_cs_CompressedFileExt = "PNG";
								}
								else if (l_x_DesiredImageCompression == MORPHO_COMPRESS_WSQ)
								{
									l_cs_CompressedFileExt = "WSQ";
								}
								else if (l_x_DesiredImageCompression == MORPHO_COMPRESS_BMP)
								{
									l_cs_CompressedFileExt = "BMP";
								}
							}
						}

						// Set ISO-19794-4 FIR Output default file name
						l_cs_ISO19794_FIR_OutputFileName = "ISO19794_FIR_" + l_cs_CompressedFileExt;
					}
					else if ((l_cs_CompressedFileExt == "RAW") && ((l_i_ImageWidth * l_i_ImageHeight) != l_ul_ImageCompressedLen))
					{
						l_i_Ret = MORPHOERR_MEMORY_PC;
						l_x_ResultDlg.m_cs_Msg1.Format("Convert ISO-19794-4 Error");
						l_x_ResultDlg.m_cs_Msg2.Format("Invalid image dimension");
						l_x_ResultDlg.m_cs_Msg5.Format("Given buffer is too small");
						l_x_ResultDlg.m_cs_Msg4.Format("%d x %d > %d", l_i_ImageWidth, l_i_ImageHeight, l_ul_ImageCompressedLen);
					}
				}
			}

			// Create ISO19794_FIR structure
			if (l_i_Ret == MORPHO_OK)
			{
				l_ppx_ImagesInfo[l_i_CurImage] = new T_MORPHO_IMAGE_INFO;

				l_ppx_ImagesInfo[l_i_CurImage]->m_uc_FingerImageQuality = 0;
				l_ppx_ImagesInfo[l_i_CurImage]->m_uc_FingerPosition = 0;
				l_ppx_ImagesInfo[l_i_CurImage]->m_uc_ImpressionType = 0;

				l_ppx_Image[l_i_CurImage] = new T_MORPHO_IMAGE;
				l_ppx_Image[l_i_CurImage]->m_x_ImageHeader.m_uc_CompressionType = l_x_ImageCompression;
				l_ppx_Image[l_i_CurImage]->m_x_ImageHeader.m_us_ResX = atoi(l_x_ISO19794_FIR_InfoDlg.m_cs_ResX);
				l_ppx_Image[l_i_CurImage]->m_x_ImageHeader.m_us_ResY = atoi(l_x_ISO19794_FIR_InfoDlg.m_cs_ResY);
				l_ppx_Image[l_i_CurImage]->m_x_ImageHeader.m_uc_NbBitsPerPixel = atoi(l_x_ISO19794_FIR_InfoDlg.m_cs_NbBitsPerPixel);
				l_ppx_Image[l_i_CurImage]->m_x_ImageHeader.m_us_NbRow = l_i_ImageHeight;
				l_ppx_Image[l_i_CurImage]->m_x_ImageHeader.m_us_NbCol = l_i_ImageWidth;
				l_ppx_Image[l_i_CurImage]->m_ul_imageSize = l_ul_ImageCompressedLen;
				l_ppx_Image[l_i_CurImage]->m_puc_Image = new UC[l_ul_ImageCompressedLen];
				memcpy(l_ppx_Image[l_i_CurImage]->m_puc_Image, l_puc_ImageCompressed, l_ul_ImageCompressedLen);

				l_i_CurImage++;
			}

			//free memory
			if (l_puc_ImageCompressed)
				free(l_puc_ImageCompressed);

		}
	}

	// Free temporary OpenFileDialog file path buffer
	if (l_pc_SelectedFilePathBuffer != NULL)
	{
		delete[] l_pc_SelectedFilePathBuffer;
		l_pc_SelectedFilePathBuffer = NULL;
	}

	// Create ISO19794_FIR structure
	if (l_i_Ret == MORPHO_OK)
	{
		l_px_ISO19794_FIR = new T_MORPHO_ISO19794_FIR;

		// Call CreateISO19794_FIR()
		l_i_Ret = C_MORPHO_Utils::CreateISO19794_FIR(l_ppx_Image, l_ppx_ImagesInfo, l_i_NumberFiles, l_px_ISO19794_FIR);

		if (l_i_Ret != MORPHO_OK)
		{
			l_x_ResultDlg.m_cs_Msg1.Format("Convert ISO-19794-4 Failed");
			l_x_ResultDlg.m_cs_Msg2.Format("Create ISO19794_FIR structure");
			l_x_ResultDlg.m_cs_Msg3.Format("Failed.");
		}
	}


	// Save ISO-19794-4 FIR
	if (l_i_Ret == MORPHO_OK)
	{
		CFileDialog l_x_SaveFileDlg(FALSE,
			"*.iso-fir",
			l_cs_ISO19794_FIR_OutputFileName,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
			"ISO-19794-4 FIR Files (*.iso-fir)|*.iso-fir||",
			this);
		SAV_PATH
			if (l_x_SaveFileDlg.DoModal() != IDOK)
			{
				RESTORE_PATH
					l_x_ResultDlg.m_cs_Msg1.Format("Convert ISO-19794-4 Failed");
				l_x_ResultDlg.m_cs_Msg2.Format("Saving file");
				l_x_ResultDlg.m_cs_Msg3.Format("Aborted by user.");

				l_i_Ret = MORPHOERR_MEMORY_PC;
			}
			else
			{
				RESTORE_PATH

					CString l_cs_File;
				l_cs_File.Format("%s", l_x_SaveFileDlg.GetPathName());

				// Call WriteISO19794_FIR
				l_i_Ret = C_MORPHO_Utils::WriteISO19794_FIR(l_px_ISO19794_FIR, l_x_DesiredImageCompression,
					l_cs_File.GetBuffer(l_cs_File.GetLength()));

				if (l_i_Ret == MORPHO_OK || l_i_Ret == MORPHOWARNING_WSQ_COMPRESSION_RATIO)
				{
					if (l_i_Ret == MORPHOWARNING_WSQ_COMPRESSION_RATIO)
					{
						l_x_ResultDlg.m_cs_Msg2.Format("MORPHOWARNING_WSQ_COMPRESSION_RATIO");
						l_i_Ret = MORPHO_OK;
					}

					l_x_ResultDlg.m_cs_Msg1.Format("Convert ISO19794-4 Success");
					l_x_ResultDlg.m_cs_Msg5.Format("Image successfully converted in file");
					l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_File);
				}
				else
				{
					l_x_ResultDlg.m_cs_Msg1.Format("Convert ISO-19794-4 Failed");
					l_x_ResultDlg.m_cs_Msg5.Format("Write ISO-19794-4 FIR failed in file");
					l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_File);
				}
			}
	}

	if (l_i_Ret == MORPHO_OK)
	{
		Beep(BEEP_FREQUENCY, BEEP_DURATION);
	}

	l_x_ResultDlg.DoModal();

	// free memory
	for (int i = 0; i<l_i_CurImage; i++)
	{
		delete[] l_ppx_Image[i]->m_puc_Image;
		l_ppx_Image[i]->m_puc_Image = NULL;

		delete l_ppx_Image[i];
		l_ppx_Image[i] = NULL;

		delete l_ppx_ImagesInfo[i];
		l_ppx_ImagesInfo[i] = NULL;
	}
	if (l_ppx_Image)
	{
		delete[]l_ppx_Image;
		l_ppx_Image = NULL;
	}
	if (l_ppx_ImagesInfo)
	{
		delete[]l_ppx_ImagesInfo;
		l_ppx_ImagesInfo = NULL;
	}

	// free l_px_ISO19794_FIR
	C_MORPHO_Utils::ReleaseISO19794_FIR(l_px_ISO19794_FIR);
	if (l_px_ISO19794_FIR)
	{
		delete l_px_ISO19794_FIR;
		l_px_ISO19794_FIR = NULL;
	}
}


/****************************************************************************//**
																			  * \brief MMIFileLoad
																			  *
																			  * Load a CSV File to FingerVP to modify the User Interface Configuration.
																			  *
																			  * @since v1.0
																			  ********************************************************************************/
void CMSO_DemoDlg::MMIFileLoad()
{
	I				l_i_Ret = MORPHO_OK;

	CString			l_cs_CSVFilePath;					// Path of CSV File
	CString			l_cs_CSVFileName;					// Name of CSV File
	UL				l_ul_CSVFileLen;					// Length of CSV File
	CFile			l_f_CSVFile;						// CSV File

	PC				l_pc_CSVFile;						// CSV File (CString format)

	CResultDlg	l_x_ResultDlg;

	// ILV
	//T_ILV	l_x_SentILV;
	//T_ILV	l_x_ReceivedILV;

	//UC		l_uc_ILV_Status;


	// Open a Load Dialog
	CFileDialog l_x_OpenFileDlg(TRUE,
		"*.csv",
		(PC)"UIConfig.csv",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE,
		"Comma-Separated Values (*.csv)|*.csv;*.csv | All Files (*.*)|*.*||",
		NULL);


	SAV_PATH
		// Check the OpenFileDialog opening
		if (IDOK != l_x_OpenFileDlg.DoModal())
		{
			l_i_Ret = MORPHOERR_MEMORY_PC;
			l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File Error");
			l_x_ResultDlg.m_cs_Msg2.Format("aborted by user.");
			l_x_ResultDlg.m_cs_Msg3.Format("");
			l_x_ResultDlg.m_cs_Msg4.Format("");
		}

	RESTORE_PATH

		// Check the File opening
		if (MORPHO_OK == l_i_Ret)
		{
			l_cs_CSVFilePath = l_x_OpenFileDlg.GetPathName();
			l_cs_CSVFileName = l_x_OpenFileDlg.GetFileName();

			if (!l_f_CSVFile.Open(l_cs_CSVFilePath, CFile::modeRead))
			{
				l_i_Ret = MORPHOERR_MEMORY_PC;
				l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File Error");
				l_x_ResultDlg.m_cs_Msg2.Format("Unable to open");
				l_x_ResultDlg.m_cs_Msg3.Format("File :");
				l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_CSVFileName);
			}
		}

	// Check the length of file
	if (MORPHO_OK == l_i_Ret)
	{
		l_ul_CSVFileLen = (UL)l_f_CSVFile.GetLength();
		if (l_ul_CSVFileLen >= 4 * 1024)
		{
			l_i_Ret = MORPHOERR_MEMORY_PC;
			l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File Error");
			l_x_ResultDlg.m_cs_Msg2.Format("The file size is too high");
			l_x_ResultDlg.m_cs_Msg3.Format("File size:");
			l_x_ResultDlg.m_cs_Msg4.Format("%d Bytes (Max : 4 KB)", l_ul_CSVFileLen);
		}
	}

	// Check the memory allocation
	if (MORPHO_OK == l_i_Ret)
	{
		l_pc_CSVFile = (PC)malloc(l_ul_CSVFileLen * sizeof(C));
		if (NULL == l_pc_CSVFile)
		{
			l_i_Ret = MORPHOERR_MEMORY_PC;
			l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File Error");
			l_x_ResultDlg.m_cs_Msg2.Format("Allocation Error ");

		}
	}

	// Check the reading
	if (MORPHO_OK == l_i_Ret)
	{
		if (l_f_CSVFile.Read(l_pc_CSVFile, l_ul_CSVFileLen) != l_ul_CSVFileLen)
		{
			l_i_Ret = MORPHOERR_MEMORY_PC;
			l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File Error");
			l_x_ResultDlg.m_cs_Msg2.Format("Unable to open read");
			l_x_ResultDlg.m_cs_Msg3.Format("File :");
			l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_CSVFileName);
		}

		l_f_CSVFile.Close();
	}

	// TODO : Verify the file

	// CSV File -> ILV
	if (MORPHO_OK == l_i_Ret)
	{
		// Send the configuration
		l_i_Ret = this->m_x_MorphoSmart.SetConfigParam((US)CONFIG_UI_CONFIG_TAG, l_ul_CSVFileLen, (PUC)l_pc_CSVFile);

		// Reply OK
		if (MORPHO_OK == l_i_Ret)
		{
			l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File ");
			l_x_ResultDlg.m_cs_Msg2.Format("Success");
			l_x_ResultDlg.m_cs_Msg3.Format("File %s is loaded.", l_cs_CSVFileName);
			l_x_ResultDlg.m_cs_Msg4.Format("To apply the update, restart FVP, please.");
		}
		else
		{
			l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File ");
			l_x_ResultDlg.m_cs_Msg2.Format("Failure");
			l_x_ResultDlg.m_cs_Msg3.Format("File %s is not loaded.", l_cs_CSVFileName);

		}
	}

	// Display the result message
	l_x_ResultDlg.DoModal();

}

/****************************************************************************//**
																			  * \brief MMIFileSaveAs
																			  *
																			  * Save a CSV File from FingerVP to get the User Interface Configuration.
																			  *
																			  * @since v1.0
																			  ********************************************************************************/
void CMSO_DemoDlg::MMIFileSaveAs()
{
	I				l_i_Ret = MORPHO_OK;

	CString			l_cs_CSVFilePath;					// Path of CSV File
	CString			l_cs_CSVFileName;					// Name of CSV File
	UL				l_ul_CSVFileLen;					// Length of CSV File
	CFile			l_f_CSVFile;						// CSV File

	PC				l_pc_CSVFile;						// CSV File (CString format)

	CResultDlg	l_x_ResultDlg;

	// Open a Load Dialog
	CFileDialog l_x_SaveFileDlg(FALSE,
		"*.csv",
		(PC)"UIConfig.csv",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE,
		"Comma-Separated Values (*.csv)|*.csv;*.csv | All Files (*.*)|*.*||",
		NULL);


	SAV_PATH
		// Check the OpenFileDialog opening
		if (IDOK != l_x_SaveFileDlg.DoModal())
		{
			l_i_Ret = MORPHOERR_MEMORY_PC;
			l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File Error");
			l_x_ResultDlg.m_cs_Msg2.Format("aborted by user.");

		}

	RESTORE_PATH

		// Check the File opening
		if (MORPHO_OK == l_i_Ret)
		{
			l_cs_CSVFilePath = l_x_SaveFileDlg.GetPathName();
			l_cs_CSVFileName = l_x_SaveFileDlg.GetFileName();

			if (!l_f_CSVFile.Open(l_cs_CSVFilePath, CFile::modeCreate | CFile::modeReadWrite))
			{
				l_i_Ret = MORPHOERR_MEMORY_PC;
				l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File Error");
				l_x_ResultDlg.m_cs_Msg2.Format("Unable to create");
				l_x_ResultDlg.m_cs_Msg3.Format("File :");
				l_x_ResultDlg.m_cs_Msg4.Format("%s", l_cs_CSVFileName);
			}
		}

	// ILV -> CSV File
	if (MORPHO_OK == l_i_Ret)
	{
		// Get the configuration
		l_ul_CSVFileLen = 0;
		l_i_Ret = this->m_x_MorphoSmart.GetConfigParam((US)CONFIG_UI_CONFIG_TAG, &l_ul_CSVFileLen, (PUC*)&l_pc_CSVFile);


		if (l_i_Ret)
		{
			l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File ");
			l_x_ResultDlg.m_cs_Msg2.Format("Failure");
			l_x_ResultDlg.m_cs_Msg3.Format("Unable to save MMI file.");

		}
		else
		{
			// Save File
			l_f_CSVFile.Write(l_pc_CSVFile, l_ul_CSVFileLen);
			l_f_CSVFile.Close();
		}

	}

	if (MORPHO_OK == l_i_Ret)
	{
		// Display Success
		l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File ");
		l_x_ResultDlg.m_cs_Msg2.Format("Success");
		l_x_ResultDlg.m_cs_Msg3.Format("File %s is saved.", l_cs_CSVFileName);

	}


	// Display the result message
	l_x_ResultDlg.DoModal();
}

/****************************************************************************//*
																			  * \brief MMIFileReset
																			  *
																			  * Reset the IHM configuration of FingerVP.
																			  *
																			  * @since v1.0
																			  ********************************************************************************/
void CMSO_DemoDlg::MMIFileReset()
{
	// Return value
	I			l_i_Ret = MORPHO_OK;

	// Dialog
	CResultDlg	l_x_ResultDlg;

	UC			l_uc_Reset = 0x01;

	// Send the configuration
	l_i_Ret = this->m_x_MorphoSmart.SetConfigParam((US)CONFIG_UI_CONFIG_TAG, 1, (PUC)&l_uc_Reset);

	// Reply OK
	if (MORPHO_OK == l_i_Ret)
	{
		l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File ");
		l_x_ResultDlg.m_cs_Msg2.Format("Reset Success");
		l_x_ResultDlg.m_cs_Msg4.Format("To apply the reset, restart FVP, please.");
	}
	else
	{
		l_x_ResultDlg.m_cs_Msg1.Format("FVP MMI File ");
		l_x_ResultDlg.m_cs_Msg2.Format("Reset Error");

	}

	// Display the result message
	l_x_ResultDlg.DoModal();
}

void CMSO_DemoDlg::GetProcduct_Type()
{
	CString l_cs_ProductDescriptor(m_x_MorphoSmart.m_puc_ProductDescriptor);
	//CString l_cs_ProductDescriptor = "Product Name : MSO FVP F"; //pour le test
	CString l_cs_resToken;
	int p = 0;
	l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n", p);
	while (l_cs_resToken != "")
	{
		l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n", p);
		if ((l_cs_resToken == "FVP") || (l_cs_resToken == "FINGER")) // since FVP 1.5 firmware release, the product name is "FINGER VP"
		{
			break;
		}
		//printf("l_cs_resToken %s ",l_cs_resToken);
	}
	if (l_cs_resToken != "")
	{
		l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n", p);
		l_cs_resToken.Trim(" \t \n \r");

		if (l_cs_resToken.CompareNoCase("F") == 0)
		{
			m_bl_Product_Type = true;
		}
		else
		{
			m_bl_Product_Type = false;
		}
	}
	else
	{
		m_bl_Product_Type = false;
	}

	return;
}

void CMSO_DemoDlg::CheckIfCBMV()
{
	CString l_cs_ProductDescriptor(m_x_MorphoSmart.m_puc_ProductDescriptor);
	CString l_cs_resToken;
	int p = 0;

	m_bl_isCBMV = false;

	while (p != -1)
	{
		l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n", p);
		l_cs_resToken.Trim(" \t \n \r");
		if (l_cs_resToken == "CBM-V")
		{
			m_tbCtrl.m_TabBioSettingsDlg->m_cmd_ForceFingerPlacement = FALSE; //disable this option in the case of CBM-V
			m_tbCtrl.m_TabBioSettingsDlg->GetDlgItem(IDC_CHECK_FFPLACEMENT_ON_TOP)->EnableWindow(FALSE);
			m_bl_isCBMV = true;
			break;
		}
	}

	m_tbCtrl.m_TabBioSettingsDlg->m_bl_isCBMV = m_bl_isCBMV;
}

void CMSO_DemoDlg::GetFVP_Status()
{
	CString l_cs_ProductDescriptor(m_x_MorphoSmart.m_puc_ProductDescriptor);
	CString l_cs_resToken;
	int p = 0;

	m_bl_FVP = false;

	while (p != -1)
	{
		l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n", p);
		l_cs_resToken.Trim(" \t \n \r");
		if ((l_cs_resToken == "FVP") || (l_cs_resToken == "FINGER")) // since FVP 1.5 firmware release, the product name is "FINGER VP"
		{
			m_bl_FVP = true;
			break;
		}
	}

	m_tbCtrl.m_TabBioSettingsDlg->m_bl_FVP = m_bl_FVP;

	return;
}

//////////////////////////////////////////////////////////////////
void CMSO_DemoDlg::GetBaseBioValue()
{
	CString l_cs_ProductDescriptor(m_x_MorphoSmart.m_puc_ProductDescriptor);
	CString l_cs_resToken;
	int p = 0;
	l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n", p);
	while (l_cs_resToken != "")
	{
		l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n", p);
		if (l_cs_resToken == "BaseBio")
		{
			break;
		}
		printf("l_cs_resToken %s ", l_cs_resToken);
	}
	if (l_cs_resToken != "")
	{
		l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n", p);
		l_cs_resToken.Trim(" \t \n \r");

		if (l_cs_resToken.CompareNoCase("Enable") == 0)
		{
			m_bl_BaseBio = true;
		}
		else
		{
			m_bl_BaseBio = false;
		}
	}
	else
	{
		m_bl_BaseBio = true;
	}

	return;
}

// if i_i_PageId = 1 ==> enroll,capture and getImage 
// if i_i_PageId = 0 ==> Identify ,verify

void CMSO_DemoDlg::GetSecurityLevel(int i_i_PageId)
{
	CString l_str_FFDSecuLevel;
	m_tbCtrl.m_TabBioSettingsDlg->m_cmd_FFDSecuLevel.GetLBText(m_tbCtrl.m_TabBioSettingsDlg->m_cmd_FFDSecuLevel.GetCurSel(), l_str_FFDSecuLevel);

	if (m_bl_FVP == true)
	{
		// FVP STD 
		if (i_i_PageId == 1)
		{
			// case enroll , getImage and Capture
			m_i_SecurityLevel = FFD_SECURITY_LEVEL_DEFAULT_HOST;
		}
		else
		{
			// case Verify and identify
			if (strcmp(l_str_FFDSecuLevel, "Standard") == 0)
			{
				m_i_SecurityLevel = MULTIMODAL_SECURITY_STANDARD;
			}
			else if (strcmp(l_str_FFDSecuLevel, "Medium") == 0)
			{
				m_i_SecurityLevel = MULTIMODAL_SECURITY_MEDIUM;
			}
			else if (strcmp(l_str_FFDSecuLevel, "High") == 0)
			{
				m_i_SecurityLevel = MULTIMODAL_SECURITY_HIGH;
			}
			//else if (strcmp(l_str_FFDSecuLevel, "Critical") == 0)
			//{
			//	m_i_SecurityLevel = MULTIMODAL_SECURITY_CRITICAL;
			//}
		}
	}
	else
	{
		// MSO ITO Or STD
		if (strcmp(l_str_FFDSecuLevel, "Low") == 0)
		{
			m_i_SecurityLevel = FFD_SECURITY_LEVEL_LOW_HOST;
		}
		else if (strcmp(l_str_FFDSecuLevel, "Medium") == 0)
		{
			m_i_SecurityLevel = FFD_SECURITY_LEVEL_MEDIUM_HOST;
		}
		else if (strcmp(l_str_FFDSecuLevel, "High") == 0)
		{
			m_i_SecurityLevel = FFD_SECURITY_LEVEL_HIGH_HOST;
		}
		else if (strcmp(l_str_FFDSecuLevel, "Critical") == 0)
		{
			m_i_SecurityLevel = FFD_SECURITY_LEVEL_CRITICAL_HOST;
		}
		else if (strcmp(l_str_FFDSecuLevel, "Default") == 0)
		{
			m_i_SecurityLevel = FFD_SECURITY_LEVEL_DEFAULT_HOST;
		}
	}

}
void CMSO_DemoDlg::OnBnClickedRebootsoft()
{
	m_ChangeCursor = TRUE;
	BeginWaitCursor();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_REBOOTSOFT)->EnableWindow(FALSE);

	m_x_MorphoSmart.RebootSoft();

	Sleep(2000); // sleep 2 seconds

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_REBOOTSOFT)->EnableWindow(TRUE);

	m_ChangeCursor = FALSE;
	EndWaitCursor();

}

void CMSO_DemoDlg::InsertText(CComboBox * i_px_ComboBox, CString i_x_String)
{
	// Local variables
	CDC *	l_px_DC;
	CFont *	l_px_Font;
	CSize	l_x_TextSize;
	CString	l_x_String;
	int		l_i_CurrentWidth;
	int		l_i_CalculatedWidth;

	// Add the given string to the ComboBox
	i_px_ComboBox->AddString(i_x_String);

	// Update data
	UpdateData();

	// Initialize local variables
	l_px_DC = NULL;
	l_px_Font = NULL;
	l_i_CurrentWidth = 0;
	l_i_CalculatedWidth = 0;

	// Retrieve ComboBox DeviceContext
	l_px_DC = i_px_ComboBox->GetDC();

	// Set ComboBox DeviceContext map mode to pixels (1 logical unit equal 1 device pixel)
	l_px_DC->SetMapMode(MM_TEXT);

	// Set ComboBox DeviceContext font with the current font in order to get real size values
	l_px_Font = this->GetFont();
	l_px_DC->SelectObject(l_px_Font);

	// Retrieve current ComboBox dropdown width
	l_i_CurrentWidth = i_px_ComboBox->GetDroppedWidth();

	// For-each string in the ComboBox
	for (int l_i_Cpt = 0; l_i_Cpt < i_px_ComboBox->GetCount(); l_i_Cpt++)
	{
		// Retrieve the ComboBox string
		i_px_ComboBox->GetLBText(l_i_Cpt, l_x_String);

		// Calculate ComboBox string size (in pixels)
		l_x_TextSize = l_px_DC->GetTextExtent(l_x_String);
		l_px_DC->LPtoDP(&l_x_TextSize);

		// If current string size is greater than the ComboBox dropdown calculated width, then update it
		if (l_x_TextSize.cx > l_i_CalculatedWidth)
			l_i_CalculatedWidth = l_x_TextSize.cx;
	}

	// Add left & right edges width plus vertical scrollbar width to the ComboBox dropdown calculated width
	l_i_CalculatedWidth += GetSystemMetrics(SM_CXVSCROLL) + (2 * GetSystemMetrics(SM_CXEDGE));

	// Select the greater value between the ComboBox dropdown current width and the ComboBox dropdown calculated width
	l_i_CurrentWidth = max(l_i_CurrentWidth, l_i_CalculatedWidth);

	// Update ComboBox dropdown width
	i_px_ComboBox->SetDroppedWidth(l_i_CurrentWidth);

	// Release ComboBox DeviceContext
	i_px_ComboBox->ReleaseDC(l_px_DC);
}

void CMSO_DemoDlg::OnLoadKs()
{
	int							l_i_ret = 0;
	CResultDlg					l_x_result_dlg;
	std::string					l_vuc_ks;
	CString						l_cs_fname, l_cs_fext;

	l_x_result_dlg.m_cs_Msg1.Format("Load Ks");

	// read new key from file
	if (GetFilesNameToOpen("ks.bin", l_cs_fname, l_cs_fext, "All Files (*.*)|*.*||"))
	{
		std::ifstream l_x_fs((LPCTSTR)l_cs_fname, std::ios::in | std::ios::binary);
		l_vuc_ks.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
		l_x_fs.close();
	}
	else
	{
		l_x_result_dlg.m_cs_Msg3.Format("GetFilesNameToOpen() error");
		l_i_ret = -1;
	}

	// load new key in device: key is sent in plaintext
	if (l_i_ret == 0)
	{
		l_i_ret = m_x_MorphoSmart.LoadKs((const unsigned char *)l_vuc_ks.c_str(), l_vuc_ks.size());
		if (l_i_ret)
		{
			l_x_result_dlg.m_cs_Msg3.Format("C_MORPHO_Device::LoadKs() error");
			l_i_ret = -1;
		}
	}

	if (l_i_ret == 0)
	{
		l_x_result_dlg.m_cs_Msg2.Format("Successful");
		Beep(BEEP_FREQUENCY, BEEP_DURATION);
	}
	else
	{
		CString l_cs_msg;
		l_x_result_dlg.m_cs_Msg2.Format("Failed");
		ConvertSDKError(l_i_ret, l_x_result_dlg.m_cs_Msg4);
		if (l_i_ret == MORPHOERR_STATUS)
		{
			l_cs_msg.Format("internal error %d", m_x_MorphoSmart.GetInternalError());
			l_x_result_dlg.m_cs_Msg4 += l_cs_msg;
		}
		//Beep(BEEP_FREQUENCY-300, BEEP_DURATION+400);
	}

	l_x_result_dlg.DoModal();

	return;
}

void CMSO_DemoDlg::OnLoadKsSymmSec()
{
	int							l_i_ret = 0;
	std::string					l_vuc_cur_ks;
	std::string					l_vuc_new_ks;
	unsigned char				l_auc_iv[DES_BLOCK_LENGTH];
	unsigned char *				l_puc_crypt = NULL;
	int							l_i_cryptlen = 0;
	CString						l_cs_fname, l_cs_fext;

	CResultDlg l_x_result_dlg;
	l_x_result_dlg.m_cs_Msg1.Format("Load Ks");

	// read current key from file
	if (GetFilesNameToOpen("ks.cur.bin", l_cs_fname, l_cs_fext, "All Files (*.*)|*.*||"))
	{
		std::ifstream l_x_fs((LPCTSTR)l_cs_fname, std::ios::in | std::ios::binary);
		l_vuc_cur_ks.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
		l_x_fs.close();

		if (l_vuc_cur_ks.size() != TRIPLE_DES_KEY_LENGTH)
		{
			l_x_result_dlg.m_cs_Msg3.Format("error: key is not 192 bits long");
			l_i_ret = -1;
		}
	}
	else
	{
		l_x_result_dlg.m_cs_Msg3.Format("GetFilesNameToOpen() error");
		l_i_ret = -1;
	}

	// read new key from file
	if (l_i_ret == 0)
	{
		if (GetFilesNameToOpen("ks.new.bin", l_cs_fname, l_cs_fext, "All Files (*.*)|*.*||"))
		{
			std::ifstream l_x_fs((LPCTSTR)l_cs_fname, std::ios::in | std::ios::binary);
			l_vuc_new_ks.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
			l_x_fs.close();

			if (l_vuc_new_ks.size() != TRIPLE_DES_KEY_LENGTH)
			{
				l_x_result_dlg.m_cs_Msg3.Format("error: key is not 192 bits long");
				l_i_ret = -1;
			}
		}
		else
		{
			l_x_result_dlg.m_cs_Msg3.Format("GetFilesNameToOpen() error");
			l_i_ret = -1;
		}
	}

	// format data for encryption
	if (l_i_ret == 0)
	{
		// we actually have to encrypt [current-ks||new-ks] with current ks
		l_vuc_new_ks.insert(l_vuc_new_ks.begin(), l_vuc_cur_ks.begin(), l_vuc_cur_ks.end());

		l_i_cryptlen = l_vuc_new_ks.size();
		l_puc_crypt = (unsigned char *)malloc(l_i_cryptlen);
		if (l_puc_crypt == NULL)
		{
			l_x_result_dlg.m_cs_Msg3.Format("Allocation error");
			l_i_ret = -1;
		}
	}

	// encrypt new key with current key
	if (l_i_ret == 0)
	{
		l_i_ret = MSOSECU_encrypt_3des_cbc_nopad((const UC*)l_vuc_cur_ks.c_str(),	//const unsigned char	i_puc_key[TRIPLE_DES_KEY_LENGTH],
			(const UC*)l_vuc_new_ks.c_str(),	//const unsigned char *	i_puc_in,
			l_i_cryptlen,			//int					i_i_len,
			l_auc_iv,				//unsigned char			io_puc_iv[DES_BLOCK_LENGTH],
			l_puc_crypt);			//unsigned char *		io_puc_out
		if (l_i_ret)
		{
			l_x_result_dlg.m_cs_Msg3.Format("MSOSECU_encrypt_3des_cbc_nopad() error");
			l_i_ret = -1;
		}
		else
		{
			// prepare buffer to send: [iv][3DES-CBC(current-ks,[current-ks||new-ks])]
			l_vuc_new_ks.assign(l_puc_crypt, l_puc_crypt + l_i_cryptlen);
			l_vuc_new_ks.insert(l_vuc_new_ks.begin(), l_auc_iv, l_auc_iv + sizeof(l_auc_iv));
		}
	}

	if (l_puc_crypt != NULL)
	{
		free(l_puc_crypt);
	}

	// load new key in device: key is sent in ciphertext
	if (l_i_ret == 0)
	{
		l_i_ret = m_x_MorphoSmart.LoadKsSecurely((const UC*)l_vuc_new_ks.c_str(), l_vuc_new_ks.size());
		if (l_i_ret)
		{
			l_x_result_dlg.m_cs_Msg3.Format("C_MORPHO_Device::LoadKs() error");
			l_i_ret = -1;
		}
	}

	if (l_i_ret == 0)
	{
		l_x_result_dlg.m_cs_Msg2.Format("Successful");
		Beep(BEEP_FREQUENCY, BEEP_DURATION);
	}
	else
	{
		CString l_cs_msg;
		l_x_result_dlg.m_cs_Msg2.Format("Failed");
		ConvertSDKError(l_i_ret, l_x_result_dlg.m_cs_Msg4);
		if (l_i_ret == MORPHOERR_STATUS)
		{
			l_cs_msg.Format("internal error %d", m_x_MorphoSmart.GetInternalError());
			l_x_result_dlg.m_cs_Msg4 += l_cs_msg;
		}
		//Beep(BEEP_FREQUENCY-300, BEEP_DURATION+400);
	}

	l_x_result_dlg.DoModal();

	return;
}

void CMSO_DemoDlg::OnLoadKsAsymmSec()
{
	int							l_i_ret = 0;
	CResultDlg					l_x_result_dlg;
	unsigned char				l_auc_host_key[10 * 1024] = { 0 };
	unsigned long				l_ul_host_key_size = sizeof(l_auc_host_key);
	unsigned char *				l_puc_kscrypt = NULL;
	unsigned long				l_ul_ks_size = 0;
	unsigned char *				l_puc_signature = NULL;
	unsigned long				l_ul_signature_size = 0;
	std::string					l_vuc_ks;
	std::string					l_vuc_host_certificate;
	std::string					l_vuc_signature;
	CString						l_cs_fname, l_cs_fext;

	l_x_result_dlg.m_cs_Msg1.Format("Load Ks");

	// read new key from file
	if (GetFilesNameToOpen("ks.bin", l_cs_fname, l_cs_fext, "All Files (*.*)|*.*||"))
	{
		std::ifstream l_x_fs((LPCTSTR)l_cs_fname, std::ios::in | std::ios::binary);
		l_vuc_ks.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
		l_x_fs.close();
	}
	else
	{
		l_x_result_dlg.m_cs_Msg3.Format("GetFilesNameToOpen() error");
		l_i_ret = -1;
	}

	// retrieve device certificate
	if (l_i_ret == 0)
	{
		if (m_puc_MsoCertif == NULL)
		{
			l_i_ret = m_x_MorphoSmart.SecuReadCertificate(0, &m_ul_MsoCertifSize, &m_puc_MsoCertif); // index 0 means device certificate
		}
	}

	// load new key in device: key is sent in ciphertext
	if (l_i_ret == 0)
	{
		// read host private key from file (PEM format)
		l_i_ret = OpenAndReadFile((PUC)"host.key", l_auc_host_key, &l_ul_host_key_size);
		if (l_i_ret)
		{
			l_x_result_dlg.m_cs_Msg3.Format("OpenAndReadFile() error");
			l_i_ret = -1;
		}

		// read host certificate from file (DER format)
		if (l_i_ret == 0)
		{
			if (GetFilesNameToOpen("host.der", l_cs_fname, l_cs_fext, "All Files (*.*)|*.*||"))
			{
				std::ifstream l_x_fs((LPCTSTR)l_cs_fname, std::ios::binary);
				l_vuc_host_certificate.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
				l_x_fs.close();
			}
			else
			{
				l_x_result_dlg.m_cs_Msg3.Format("GetFilesNameToOpen() error");
				l_i_ret = -1;
			}
		}

		// encrypt ks with device certificate
		if (l_i_ret == 0)
		{
			l_i_ret = MSOSECU_RSA_Encrypt(m_ul_MsoCertifSize,	//UL		i_ul_CertifSize,
				m_puc_MsoCertif,	//PUC		i_puc_Certif,
				l_vuc_ks.size(),		//UL		i_ul_DataSize,
				(UC*)l_vuc_ks.c_str(),	//PUC		i_puc_Data,
				&l_ul_ks_size,		//PUL		o_pul_DataSize,
				&l_puc_kscrypt);	//PUC*		o_ppuc_Data
			if (l_i_ret)
			{
				l_x_result_dlg.m_cs_Msg3.Format("MSOSECU_RSA_Encrypt() error");
				l_i_ret = -1;
			}
			else
			{
				l_vuc_ks.assign(l_puc_kscrypt, l_puc_kscrypt + l_ul_ks_size);
			}
		}

		// sign encrypted ks with host private key (only RSA public key algorithm is supported here)
		if (l_i_ret == 0)
		{
			l_i_ret = MSOSECU_SignRSA(l_ul_host_key_size,		//UL		i_ul_HostKey,
				l_auc_host_key,			//PUC		i_puc_HostKey,
				l_ul_ks_size,			//UL		i_ul_DataSize,
				l_puc_kscrypt,			//PUC		i_puc_Data,
				&l_ul_signature_size,	//PUL		o_pul_SignatureSize,
				&l_puc_signature);		//PUC*		o_ppuc_Signature
			if (l_i_ret)
			{
				l_x_result_dlg.m_cs_Msg3.Format("MSOSECU_SignRSA() error");
				l_i_ret = -1;
			}
			else
			{
				l_vuc_signature.assign(l_puc_signature, l_puc_signature + l_ul_signature_size);
			}
		}

		// securely load ks
		if (l_i_ret == 0)
		{
			l_i_ret = m_x_MorphoSmart.LoadKsSecurely((const UC*)l_vuc_ks.c_str()
				, l_vuc_ks.size()
				, (const UC*)l_vuc_signature.c_str()
				, l_vuc_signature.size()
				, (const UC*)l_vuc_host_certificate.c_str()
				, l_vuc_host_certificate.size());
			if (l_i_ret)
			{
				l_x_result_dlg.m_cs_Msg3.Format("C_MORPHO_Device::LoadKs() error");
				l_i_ret = -1;
			}
		}
	}

	if (l_puc_kscrypt != NULL)
	{
		MSOSECU_Free(&l_puc_kscrypt);
	}

	if (l_puc_signature != NULL)
	{
		MSOSECU_Free(&l_puc_signature);
	}

	if (l_i_ret == 0)
	{
		l_x_result_dlg.m_cs_Msg2.Format("Successful");
		Beep(BEEP_FREQUENCY, BEEP_DURATION);
	}
	else
	{
		CString l_cs_msg;
		l_x_result_dlg.m_cs_Msg2.Format("Failed");
		ConvertSDKError(l_i_ret, l_x_result_dlg.m_cs_Msg4);
		if (l_i_ret == MORPHOERR_STATUS)
		{
			l_cs_msg.Format("internal error %d", m_x_MorphoSmart.GetInternalError());
			l_x_result_dlg.m_cs_Msg4 += l_cs_msg;
		}
		//Beep(BEEP_FREQUENCY-300, BEEP_DURATION+400);
	}

	l_x_result_dlg.DoModal();

	return;
}

void CMSO_DemoDlg::OnDisplayKsKcv()
{
	int				l_i_ret = MORPHO_OK;
	CString			l_cs_msg, l_cs_error;
	unsigned char	l_auc_kcv[KCV_LEN] = { 0 };

	// get Ks KCV from device
	l_i_ret = m_x_MorphoSmart.GetKCV(ID_KS, NULL, l_auc_kcv);

	// display Ks KCV
	if (l_i_ret != MORPHO_OK)
	{
		ConvertSDKError(l_i_ret, l_cs_error);
		l_cs_msg.Format("C_MORPHO_Device::GetKCV returned error %d (%s)", l_i_ret, l_cs_error);
	}
	else
	{
		l_cs_msg.Format("Ks KCV :  0x%02X 0x%02X 0x%02X", l_auc_kcv[0], l_auc_kcv[1], l_auc_kcv[2]);
	}

	AfxMessageBox(l_cs_msg);

	return;
}

void CMSO_DemoDlg::OnDisplayKprivacyKcv()
{
	int				l_i_ret = MORPHO_OK;
	CString			l_cs_msg, l_cs_error;
	unsigned char	l_auc_kcv[KCV_LEN] = { 0 };

	// get Ks KCV from device
	l_i_ret = m_x_MorphoSmart.GetKCV(ID_KPRIVACY, NULL, l_auc_kcv);

	// display Ks KCV
	if (l_i_ret != MORPHO_OK)
	{
		ConvertSDKError(l_i_ret, l_cs_error);
		l_cs_msg.Format("C_MORPHO_Device::GetKCV returned error %d (%s)", l_i_ret, l_cs_error);
	}
	else
	{
		l_cs_msg.Format("Kprivacy KCV :  0x%02X 0x%02X 0x%02X", l_auc_kcv[0], l_auc_kcv[1], l_auc_kcv[2]);
	}

	AfxMessageBox(l_cs_msg);

	return;
}

void CMSO_DemoDlg::OnEnableBioDataEncryption()
{
	if (m_IsDataEncryption)
	{
		m_IsDataEncryption = FALSE;
		GetMenu()->CheckMenuItem(ID_SECURITY_ENABLE_BIO_DATA_ENCRYPTION, MF_UNCHECKED | MF_BYCOMMAND);
		m_x_MorphoSmart.EnableDataEncryption(FALSE, 0, NULL);
	}
	else
	{
		CDiversificationData l_x_DiversificationData;
		if (l_x_DiversificationData.DoModal() == 1)
		{
			m_IsDataEncryption = TRUE;
			GetMenu()->CheckMenuItem(ID_SECURITY_ENABLE_BIO_DATA_ENCRYPTION, MF_CHECKED | MF_BYCOMMAND);

			UL l_ul_DiversificationDataSize = l_x_DiversificationData.m_cs_DiversificationData.GetLength();
			m_x_MorphoSmart.EnableDataEncryption(TRUE, l_ul_DiversificationDataSize, (PUC)l_x_DiversificationData.m_cs_DiversificationData.GetBuffer());
		}
	}
}

void CMSO_DemoDlg::OnDecryptBiometricData()
{
	CResultDlg					l_x_result_dlg;
	int							l_i_ret = 0;
	CString						l_cs_folder, l_cs_fname1, l_cs_fname2, l_cs_fext;
	std::string					l_vuc_crypted_data;
	std::string					l_vuc_ks;

	CDiversificationData l_x_DiversificationData;

	l_x_result_dlg.m_cs_Msg1.Format("Decrypt Biometric Data");

	// read crypted data from file
	if (GetFilesNameToOpen("", l_cs_folder, l_cs_fname1, l_cs_fext, "Crypt Files (*.crypt)|*.crypt|All Files (*.*)|*.*||", false))
	{
		CString file_name;
		file_name.Format("%s\\%s.%s", l_cs_folder, l_cs_fname1, l_cs_fext);
		std::ifstream l_x_fs(file_name.GetBuffer(0), std::ios::in | std::ios::binary);
		l_vuc_crypted_data.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
		l_x_fs.close();
	}
	else
	{
		l_x_result_dlg.m_cs_Msg3.Format("GetFilesNameToOpen() error");
		l_i_ret = -1;
	}

	if (l_i_ret == 0)
	{
		// read key from file
		if (GetFilesNameToOpen("ks.bin", l_cs_fname2, l_cs_fext, "Bin Files (*.bin)|*.bin|All Files (*.*)|*.*||"))
		{
			std::ifstream l_x_fs((LPCTSTR)l_cs_fname2, std::ios::in | std::ios::binary);
			l_vuc_ks.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
			l_x_fs.close();
		}
		else
		{
			l_x_result_dlg.m_cs_Msg3.Format("GetFilesNameToOpen() error");
			l_i_ret = -1;
		}
	}

	if (l_i_ret == 0)
	{
		// get diversification data
		if (l_x_DiversificationData.DoModal() != 1)
			l_i_ret = -1;
	}

	if (l_i_ret == 0)
	{
		std::string l_vuc_crypted_buffer;
		unsigned char l_auc_iv[DES_BLOCK_LENGTH] = { 0 };
		unsigned char* l_puc_clear_data = (unsigned char *)malloc(l_vuc_crypted_data.size() - l_vuc_crypted_data.size() % DES_BLOCK_LENGTH);
		if (l_puc_clear_data == NULL)
		{
			l_x_result_dlg.m_cs_Msg3.Format("Allocation error");
			l_i_ret = -1;
		}
		else
		{
			// decrypt data
			l_vuc_crypted_buffer = l_vuc_crypted_data.substr(0, (l_vuc_crypted_data.size() - l_vuc_crypted_data.size() % DES_BLOCK_LENGTH));
			l_i_ret = MSOSECU_decrypt_3des_cbc_nopad((const UC*)l_vuc_ks.c_str(),
				(const UC*)l_vuc_crypted_buffer.c_str(),
				l_vuc_crypted_buffer.size(),
				l_auc_iv,
				l_puc_clear_data);
		}

		if (l_i_ret == 0)
		{
			std::string consistent_data;
			consistent_data.assign(l_puc_clear_data, l_puc_clear_data + l_vuc_crypted_buffer.size());
			consistent_data.append(l_vuc_crypted_data, l_vuc_crypted_buffer.size(), l_vuc_crypted_data.size() % DES_BLOCK_LENGTH);

			std::string decrypted_diversification_data;
			decrypted_diversification_data.append(consistent_data, DES_BLOCK_LENGTH, l_x_DiversificationData.m_cs_DiversificationData.GetLength());

			std::string decrypted_bio_data;
			decrypted_bio_data.append(consistent_data, DES_BLOCK_LENGTH + decrypted_diversification_data.size(), consistent_data.size() - DES_BLOCK_LENGTH - decrypted_diversification_data.size());

			// verify diversification data
			if (decrypted_diversification_data.compare(l_x_DiversificationData.m_cs_DiversificationData.GetBuffer(0)))
			{
				l_x_result_dlg.m_cs_Msg1.Format("Decrypt Failed");
				l_x_result_dlg.m_cs_Msg4.Format("Selected key or diversification data do not match");
			}
			else
			{
				// write file on disk	
				CFileDialog l_x_SaveFileDlg(FALSE,
					"",
					l_cs_fname1,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
					NULL,
					this);
				SAV_PATH
					if (l_x_SaveFileDlg.DoModal() != IDOK)
					{
						RESTORE_PATH
							l_x_result_dlg.m_cs_Msg1.Format("Decrypt Successful");
						l_x_result_dlg.m_cs_Msg2.Format("Saving decrypted file");
						l_x_result_dlg.m_cs_Msg3.Format("aborted by user.");
					}
					else
					{
						RESTORE_PATH
							CString l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();
						CFile l_f_DecryptedFile(l_cs_SaveFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
						l_f_DecryptedFile.Write(decrypted_bio_data.c_str(), decrypted_bio_data.size());
						l_f_DecryptedFile.Close();
						l_x_result_dlg.m_cs_Msg1.Format("Capture Successful");
						l_x_result_dlg.m_cs_Msg3.Format("Decrypted data ");
						l_x_result_dlg.m_cs_Msg5.Format("successfully exported in file");
						l_x_result_dlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
						Beep(BEEP_FREQUENCY, BEEP_DURATION);
					}
			}
		}
		else
		{
			l_x_result_dlg.m_cs_Msg3.Format("MSOSECU_decrypt_3des_cbc_nopad() error");
		}

		if (NULL != l_puc_clear_data)
			free(l_puc_clear_data);

		l_x_result_dlg.DoModal();
	}
}

void CMSO_DemoDlg::OnEncryptAESData()
{
	std::string		l_encrypt_Key;
	std::string		l_data_to_encrypt;
	UC		i_puc_Aes128CbcInputVector[AES128_BLOCK_LENGTH];
	UL		o_pul_Aes128CbcCryptedDataLen = 0;
	PUC		o_ppuc_Aes128CbcCryptedData = NULL;
	CString						l_cs_fname, l_cs_fname1, l_cs_fext, l_cs_fext1;
	int							l_i_ret = 0;
	int							l_i_cryptlen = 0;
	FILE * pFile;
	CResultDlg l_x_result_dlg;
	l_x_result_dlg.m_cs_Msg1.Format("Encrypt AES Data");
	memset(i_puc_Aes128CbcInputVector, 0, AES128_BLOCK_LENGTH);

	// read encrypt key from file
	if (GetFilesNameToOpen("key.bin", l_cs_fname, l_cs_fext, "AES 128 Key Files (*.*)|*.*||"))
	{
		std::ifstream l_x_fs((LPCTSTR)l_cs_fname, std::ios::in | std::ios::binary);
		l_encrypt_Key.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
		l_x_fs.close();
		if (l_encrypt_Key.size() != AES128_BLOCK_LENGTH)
		{
			l_x_result_dlg.m_cs_Msg3.Format("error: key is not 128 bits long");
			l_i_ret = -1;
		}
	}
	else
	{
		l_x_result_dlg.m_cs_Msg3.Format("Error while getting KPRIVACY file");
		l_i_ret = -1;
	}

	// read data to encrypt from file
	if (l_i_ret == 0)
	{
		if (GetFilesNameToOpen("data.bin", l_cs_fname1, l_cs_fext1, "Data to crypt (*.*)|*.*||"))
		{
			std::ifstream l_x_fs((LPCTSTR)l_cs_fname1, std::ios::in | std::ios::binary);
			l_data_to_encrypt.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
			l_x_fs.close();

			if (l_data_to_encrypt.size() == 0)
			{
				l_x_result_dlg.m_cs_Msg3.Format("error: data lenght is not valid");
				l_i_ret = -1;
			}
		}
		else
		{
			l_x_result_dlg.m_cs_Msg3.Format("Error while getting data");
			l_i_ret = -1;
		}
	}
	if (l_i_ret == 0)
	{
		l_i_ret = MSOSECU_encrypt_aes128_cbc(l_encrypt_Key.length(), (PUC)l_encrypt_Key.c_str(), (UL)l_data_to_encrypt.length(), (PUC)l_data_to_encrypt.c_str(), i_puc_Aes128CbcInputVector, TRUE, &o_pul_Aes128CbcCryptedDataLen, &o_ppuc_Aes128CbcCryptedData);
	}


	if (l_i_ret == 0)
	{
		l_cs_fname1 += ".crypt";
		CFileDialog l_x_SaveFileDlg(FALSE,
			"",
			l_cs_fname1,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
			NULL,
			this);
		if (l_x_SaveFileDlg.DoModal() != IDOK)
		{
			RESTORE_PATH
				l_x_result_dlg.m_cs_Msg1.Format("Encrypt Successful");
			l_x_result_dlg.m_cs_Msg2.Format("Saving decrypted file");
			l_x_result_dlg.m_cs_Msg3.Format("aborted by user.");
		}
		else
		{
			RESTORE_PATH
				CString l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();
			CFile l_f_DecryptedFile(l_cs_SaveFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
			l_f_DecryptedFile.Write(o_ppuc_Aes128CbcCryptedData, o_pul_Aes128CbcCryptedDataLen);
			l_f_DecryptedFile.Close();
			l_x_result_dlg.m_cs_Msg1.Format("Encrypt Successful");
			l_x_result_dlg.m_cs_Msg3.Format("Encrypted data ");
			l_x_result_dlg.m_cs_Msg5.Format("successfully exported in file");
			l_x_result_dlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
			Beep(BEEP_FREQUENCY, BEEP_DURATION);
		}
	}
	else
	{
		l_x_result_dlg.m_cs_Msg1.Format("Encrypt Failed");
	}
	if (o_ppuc_Aes128CbcCryptedData)
		MSOSECU_Free(&o_ppuc_Aes128CbcCryptedData);

	l_x_result_dlg.DoModal();

}

void CMSO_DemoDlg::OnDecryptAESData()
{
	std::string		l_encrypt_Key;
	std::string		l_data_to_decrypt;
	UC		i_puc_Aes128CbcInputVector[AES128_BLOCK_LENGTH];
	UL		o_pul_Aes128CbcClearDataLen = 0;
	PUC		o_ppuc_Aes128CbcClearData = NULL;
	CString						l_cs_fname, l_cs_fname1, l_cs_fext, l_cs_fext1;
	int							l_i_ret = 0;
	int							l_i_cryptlen = 0;

	CResultDlg l_x_result_dlg;
	l_x_result_dlg.m_cs_Msg1.Format("Decrypt AES Data");

	memset(i_puc_Aes128CbcInputVector, 0, AES128_BLOCK_LENGTH);

	// read encrypt key from file
	if (GetFilesNameToOpen("key.bin", l_cs_fname, l_cs_fext, "AES 128 Key Files (*.*)|*.*||"))
	{
		std::ifstream l_x_fs((LPCTSTR)l_cs_fname, std::ios::in | std::ios::binary);
		l_encrypt_Key.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
		l_x_fs.close();
		if (l_encrypt_Key.size() != AES128_BLOCK_LENGTH)
		{
			l_x_result_dlg.m_cs_Msg3.Format("error: key is not 128 bits long");
			l_i_ret = -1;
		}
	}
	else
	{
		l_x_result_dlg.m_cs_Msg3.Format("GetFilesNameToOpen() error");
		l_i_ret = -1;
	}

	// read data to encrypt from file
	if (l_i_ret == 0)
	{
		if (GetFilesNameToOpen("data.bin.crypt", l_cs_fname1, l_cs_fext1, "data to decrypt (*.*)|*.*||"))
		{
			std::ifstream l_x_fs((LPCTSTR)l_cs_fname1, std::ios::in | std::ios::binary);
			l_data_to_decrypt.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
			l_x_fs.close();

			if (l_data_to_decrypt.size() == 0)
			{
				l_x_result_dlg.m_cs_Msg3.Format("error: lenght of data to decrypt is not valid");
				l_i_ret = -1;
			}
		}
		else
		{
			l_x_result_dlg.m_cs_Msg3.Format("GetFilesNameToOpen() error");
			l_i_ret = -1;
		}
	}
	if (l_i_ret == 0)
	{
		l_i_ret = MSOSECU_decrypt_aes128_cbc(l_encrypt_Key.length(), (PUC)l_encrypt_Key.c_str(), (UL)l_data_to_decrypt.length(), (PUC)l_data_to_decrypt.c_str(), i_puc_Aes128CbcInputVector, TRUE, &o_pul_Aes128CbcClearDataLen, &o_ppuc_Aes128CbcClearData);
	}

	if (l_i_ret == 0)
	{
		l_cs_fname1 += ".decrypt";
		CFileDialog l_x_SaveFileDlg(FALSE,
			"",
			l_cs_fname1,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE | OFN_CREATEPROMPT,
			NULL,
			this);
		if (l_x_SaveFileDlg.DoModal() != IDOK)
		{
			RESTORE_PATH
				l_x_result_dlg.m_cs_Msg1.Format("Encrypt Successful");
			l_x_result_dlg.m_cs_Msg2.Format("Saving decrypted file");
			l_x_result_dlg.m_cs_Msg3.Format("aborted by user.");
		}
		else
		{
			RESTORE_PATH
				CString l_cs_SaveFile = l_x_SaveFileDlg.GetPathName();
			CFile l_f_DecryptedFile(l_cs_SaveFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
			l_f_DecryptedFile.Write(o_ppuc_Aes128CbcClearData, o_pul_Aes128CbcClearDataLen);
			l_f_DecryptedFile.Close();
			l_x_result_dlg.m_cs_Msg1.Format("Decrypt Successful");
			l_x_result_dlg.m_cs_Msg3.Format("Decrypted data ");
			l_x_result_dlg.m_cs_Msg5.Format("successfully exported in file");
			l_x_result_dlg.m_cs_Msg4.Format("%s", l_cs_SaveFile);
			Beep(BEEP_FREQUENCY, BEEP_DURATION);
		}
	}
	else
	{
		l_x_result_dlg.m_cs_Msg1.Format("Decrypt Failed");
	}

	if (NULL != o_ppuc_Aes128CbcClearData)
		MSOSECU_Free(&o_ppuc_Aes128CbcClearData);

	l_x_result_dlg.DoModal();



}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// PRIVACY MODE ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

void CMSO_DemoDlg::OnPrivacyMode_Enable_NoOperationDB()
{
	I	l_i_ret = m_x_MorphoSmart.SetPrivacyModeStatus(C_MORPHO_Device::PRIVACY_MODE_ENABLED, C_MORPHO_Device::PRIVACY_MODE_DB_PROCESSING_NOTHING);
	if (MORPHO_OK != l_i_ret)
	{
		if (MORPHOERR_SECU == l_i_ret)
		{
			if (m_b_BaseAvailable)
			{
				unsigned char	l_auc_kcv[KCV_LEN] = { 0 };
				l_i_ret = m_x_MorphoSmart.GetKCV(ID_KPRIVACY, NULL, l_auc_kcv);
				if (l_i_ret != MORPHO_OK)
				{
					AfxMessageBox("The activation of the Privacy Mode is not allowed when a database exists but not the key Kprivacy and database erase is not specified", MB_ICONSTOP);
				}
				else
				{
					AfxMessageBox("The activation of the Privacy Mode is not allowed when an unsupported database exists and database erase or ciphering/transciphering is not specified", MB_ICONSTOP);
				}
			}
			else
				AfxMessageBox("The activation of the Privacy Mode is not allowed on some old MorphoSmart™ devices", MB_ICONSTOP);
		}
		else
		{
			AfxMessageBox("Enable Privacy Mode operation failed", MB_ICONSTOP);
		}
		return;
	}

	m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_ENABLED;

	CMenu *cMainMenu = GetMenu()->GetSubMenu(0);
	int SubMenuPosition = FindMenuItem(cMainMenu, _T("Privacy Mode"));
	CMenu *CMenuPrivacyMode = cMainMenu->GetSubMenu(SubMenuPosition);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Disable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_ENABLED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Partial Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	DrawMenuBar();
}

void CMSO_DemoDlg::OnPrivacyMode_Enable_EraseDB()
{
	I	l_i_ret = m_x_MorphoSmart.SetPrivacyModeStatus(C_MORPHO_Device::PRIVACY_MODE_ENABLED, C_MORPHO_Device::PRIVACY_MODE_DB_PROCESSING_ERASE);
	BOOL l_b_BaseAvailable = m_b_BaseAvailable;
	if (MORPHO_OK != l_i_ret)
	{
		if (MORPHOERR_SECU == l_i_ret)
		{
			AfxMessageBox("The activation of the Privacy Mode is not allowed on some old MorphoSmart™ devices", MB_ICONSTOP);
		}
		else
		{
			AfxMessageBox("Enable Privacy Mode operation failed", MB_ICONSTOP);
		}

		return;
	}
	
	// synchronisation
	DestroyDatabase();

	m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_ENABLED;

	CMenu *cMainMenu = GetMenu()->GetSubMenu(0);
	int SubMenuPosition = FindMenuItem(cMainMenu, _T("Privacy Mode"));
	CMenu *CMenuPrivacyMode = cMainMenu->GetSubMenu(SubMenuPosition);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Disable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_ENABLED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Partial Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	// update database status
	m_cs_MaxNbRecords.Format("%d", 0);
	m_tbCtrl.m_TabDbInfoDlg->SetParam("MaxNbRecords", m_cs_MaxNbRecords);

	m_NbFingers.Format("%d", 0);
	m_tbCtrl.m_TabDbInfoDlg->SetParam("NbFingers", m_NbFingers);

	m_DbEncryption.Format("%s", "No");
	m_tbCtrl.m_TabDbInfoDlg->SetParam("EncryptedDB", m_DbEncryption);

	GetDlgItem(IDC_BUTTON_CREATE_BASE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DESTROY_BASE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_NO_CHECK)->EnableWindow(FALSE);
	m_listCtrl.DeleteAllItems();
	GetDlgItem(IDC_LIST_USER)->EnableWindow(FALSE);
	UpdateNumberOfRecords();
	if (l_b_BaseAvailable == TRUE)
	{
		m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_EnrollPPage));
		m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_IdentifyPPage));
	}

	DrawMenuBar();
}

void CMSO_DemoDlg::OnPrivacyMode_Enable_CipherDB()
{
	I	l_i_ret = m_x_MorphoSmart.SetPrivacyModeStatus(C_MORPHO_Device::PRIVACY_MODE_ENABLED, C_MORPHO_Device::PRIVACY_MODE_DB_PROCESSING_CIPHER_TRANCIPHER);
	if (MORPHO_OK != l_i_ret)
	{
		if (MORPHOERR_SECU == l_i_ret)
		{
			AfxMessageBox("The activation of the Privacy Mode is not allowed on some old MorphoSmart™ devices", MB_ICONSTOP);
		}
		else
		{
			AfxMessageBox("Enable Privacy Mode operation failed", MB_ICONSTOP);
		}

		return;
	}

	m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_ENABLED;

	CMenu *cMainMenu = GetMenu()->GetSubMenu(0);
	int SubMenuPosition = FindMenuItem(cMainMenu, _T("Privacy Mode"));
	CMenu *CMenuPrivacyMode = cMainMenu->GetSubMenu(SubMenuPosition);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Disable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_ENABLED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Partial Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	DrawMenuBar();
}

void CMSO_DemoDlg::OnPrivacyMode_PartialEnable_NoOperationDB()
{
	I	l_i_ret = m_x_MorphoSmart.SetPrivacyModeStatus(C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED, C_MORPHO_Device::PRIVACY_MODE_DB_PROCESSING_NOTHING);
	if (MORPHO_OK != l_i_ret)
	{
		if (MORPHOERR_SECU == l_i_ret)
		{
			if (m_b_BaseAvailable)
			{
				unsigned char	l_auc_kcv[KCV_LEN] = { 0 };
				l_i_ret = m_x_MorphoSmart.GetKCV(ID_KPRIVACY, NULL, l_auc_kcv);
				if (l_i_ret != MORPHO_OK)
				{
					AfxMessageBox("The activation of the Partial Privacy Mode is not allowed when a database exists but not the key Kprivacy and database erase is not specified", MB_ICONSTOP);
				}
				else
				{
					AfxMessageBox("The activation of the Partial Privacy Mode is not allowed when an unsupported database exists and database erase or ciphering/transciphering is not specified", MB_ICONSTOP);
				}
			}
			else
				AfxMessageBox("The activation of the Partial Privacy Mode is not allowed on some old MorphoSmart™ devices", MB_ICONSTOP);
		}
		else
		{
			AfxMessageBox("Enable Partial Privacy Mode operation failed", MB_ICONSTOP);
		}
		return;
	}

	m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED;

	CMenu *cMainMenu = GetMenu()->GetSubMenu(0);
	int SubMenuPosition = FindMenuItem(cMainMenu, _T("Privacy Mode"));
	CMenu *CMenuPrivacyMode = cMainMenu->GetSubMenu(SubMenuPosition);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Disable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_ENABLED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Partial Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	DrawMenuBar();
}

void CMSO_DemoDlg::OnPrivacyMode_PartialEnable_EraseDB()
{
	I	l_i_ret = m_x_MorphoSmart.SetPrivacyModeStatus(C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED, C_MORPHO_Device::PRIVACY_MODE_DB_PROCESSING_ERASE);
	BOOL l_b_BaseAvailable = m_b_BaseAvailable;
	if (MORPHO_OK != l_i_ret)
	{
		if (MORPHOERR_SECU == l_i_ret)
		{
			AfxMessageBox("The activation of the Partial Privacy Mode is not allowed on some old MorphoSmart™ devices", MB_ICONSTOP);
		}
		else
		{
			AfxMessageBox("Enable Partial Privacy Mode operation failed", MB_ICONSTOP);
		}

		return;
	}
	// synchronisation
	DestroyDatabase();

	m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED;

	CMenu *cMainMenu = GetMenu()->GetSubMenu(0);
	int SubMenuPosition = FindMenuItem(cMainMenu, _T("Privacy Mode"));
	CMenu *CMenuPrivacyMode = cMainMenu->GetSubMenu(SubMenuPosition);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Disable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_ENABLED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Partial Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	// update database status
	m_cs_MaxNbRecords.Format("%d", 0);
	m_tbCtrl.m_TabDbInfoDlg->SetParam("MaxNbRecords", m_cs_MaxNbRecords);

	m_NbFingers.Format("%d", 0);
	m_tbCtrl.m_TabDbInfoDlg->SetParam("NbFingers", m_NbFingers);

	m_DbEncryption.Format("%s", "No");
	m_tbCtrl.m_TabDbInfoDlg->SetParam("EncryptedDB", m_DbEncryption);

	GetDlgItem(IDC_BUTTON_CREATE_BASE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DESTROY_BASE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_NO_CHECK)->EnableWindow(FALSE);
	m_listCtrl.DeleteAllItems();
	GetDlgItem(IDC_LIST_USER)->EnableWindow(FALSE);
	UpdateNumberOfRecords();

	if (l_b_BaseAvailable == TRUE)
	{
		m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_EnrollPPage));
		m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_IdentifyPPage));
	}

	DrawMenuBar();
}

void CMSO_DemoDlg::OnPrivacyMode_PartialEnable_CipherDB()
{
	I	l_i_ret = m_x_MorphoSmart.SetPrivacyModeStatus(C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED, C_MORPHO_Device::PRIVACY_MODE_DB_PROCESSING_CIPHER_TRANCIPHER);
	if (MORPHO_OK != l_i_ret)
	{
		if (MORPHOERR_SECU == l_i_ret)
		{
			AfxMessageBox("The activation of the Partial Privacy Mode is not allowed on some old MorphoSmart™ devices", MB_ICONSTOP);
		}
		else
		{
			AfxMessageBox("Enable Partial Privacy Mode operation failed", MB_ICONSTOP);
		}

		return;
	}

	m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_PARTIAL_ENABLED;

	CMenu *cMainMenu = GetMenu()->GetSubMenu(0);
	int SubMenuPosition = FindMenuItem(cMainMenu, _T("Privacy Mode"));
	CMenu *CMenuPrivacyMode = cMainMenu->GetSubMenu(SubMenuPosition);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Disable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_ENABLED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Partial Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	DrawMenuBar();
}

void CMSO_DemoDlg::OnPrivacyMode_Disable_NoOperationDB()
{
	I	l_i_ret = m_x_MorphoSmart.SetPrivacyModeStatus(C_MORPHO_Device::PRIVACY_MODE_DISABLED, C_MORPHO_Device::PRIVACY_MODE_DB_PROCESSING_NOTHING);
	if (MORPHO_OK != l_i_ret)
	{
		if (MORPHOERR_SECU == l_i_ret)
		{
			AfxMessageBox("Disable Privacy Mode is not allowed when a database exists and database erase is not specified", MB_ICONSTOP);
		}
		else
		{
			AfxMessageBox("Disable Privacy Mode operation failed", MB_ICONSTOP);
		}
		return;
	}

	m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_DISABLED;

	CMenu *cMainMenu = GetMenu()->GetSubMenu(0);
	int SubMenuPosition = FindMenuItem(cMainMenu, _T("Privacy Mode"));
	CMenu *CMenuPrivacyMode = cMainMenu->GetSubMenu(SubMenuPosition);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_ENABLED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Disable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Partial Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_ENABLED);

	DrawMenuBar();
}

void CMSO_DemoDlg::OnPrivacyMode_Disable_EraseDB()
{
	I	l_i_ret = m_x_MorphoSmart.SetPrivacyModeStatus(C_MORPHO_Device::PRIVACY_MODE_DISABLED, C_MORPHO_Device::PRIVACY_MODE_DB_PROCESSING_ERASE);
	BOOL l_b_BaseAvailable = m_b_BaseAvailable;

	if (MORPHO_OK != l_i_ret)
	{
		AfxMessageBox("Disable Privacy Mode operation failed", MB_ICONSTOP);
		return;
	}
	// synchronisation
	DestroyDatabase();

	m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_DISABLED;

	CMenu *cMainMenu = GetMenu()->GetSubMenu(0);
	int SubMenuPosition = FindMenuItem(cMainMenu, _T("Privacy Mode"));
	CMenu *CMenuPrivacyMode = cMainMenu->GetSubMenu(SubMenuPosition);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_ENABLED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Disable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Partial Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_ENABLED);

	// update database status
	m_cs_MaxNbRecords.Format("%d", 0);
	m_tbCtrl.m_TabDbInfoDlg->SetParam("MaxNbRecords", m_cs_MaxNbRecords);

	m_NbFingers.Format("%d", 0);
	m_tbCtrl.m_TabDbInfoDlg->SetParam("NbFingers", m_NbFingers);

	m_DbEncryption.Format("%s", "No");
	m_tbCtrl.m_TabDbInfoDlg->SetParam("EncryptedDB", m_DbEncryption);

	GetDlgItem(IDC_BUTTON_CREATE_BASE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DESTROY_BASE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_IDENT_MATCH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REMOVE_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REMOVE_ALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPDATE_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_NO_CHECK)->EnableWindow(FALSE);
	m_listCtrl.DeleteAllItems();
	GetDlgItem(IDC_LIST_USER)->EnableWindow(FALSE);
	UpdateNumberOfRecords();

	if (l_b_BaseAvailable == TRUE)
	{
		m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_EnrollPPage));
		m_BioOpPSheet.HidePage(&(m_BioOpPSheet.m_IdentifyPPage));
	}

	DrawMenuBar();
}

void CMSO_DemoDlg::OnPrivacyMode_Disable_CipherDB()
{
	I	l_i_ret = m_x_MorphoSmart.SetPrivacyModeStatus(C_MORPHO_Device::PRIVACY_MODE_DISABLED, C_MORPHO_Device::PRIVACY_MODE_DB_PROCESSING_CIPHER_TRANCIPHER);
	if (MORPHO_OK != l_i_ret)
	{
		AfxMessageBox("Disable Privacy Mode operation failed", MB_ICONSTOP);
		return;
	}

	m_PrivacyModeStatus = C_MORPHO_Device::PRIVACY_MODE_DISABLED;

	CMenu *cMainMenu = GetMenu()->GetSubMenu(0);
	int SubMenuPosition = FindMenuItem(cMainMenu, _T("Privacy Mode"));
	CMenu *CMenuPrivacyMode = cMainMenu->GetSubMenu(SubMenuPosition);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_ENABLED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Disable Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_GRAYED);

	SubMenuPosition = FindMenuItem(CMenuPrivacyMode, _T("Enable Partial Privacy Mode"));
	CMenuPrivacyMode->EnableMenuItem(SubMenuPosition, MF_BYPOSITION | MF_ENABLED);

	DrawMenuBar();
}

void CMSO_DemoDlg::OnLoadKprivacy()
{
	int							l_i_ret = 0;
	CResultDlg					l_x_result_dlg;
	std::string					l_vuc_Kprivacy;
	CString						l_cs_fname, l_cs_fext;

	l_x_result_dlg.m_cs_Msg1.Format("Load Kprivacy");

	// read new key from file
	if (GetFilesNameToOpen("Kprivacy.bin", l_cs_fname, l_cs_fext, "All Files (*.*)|*.*||"))
	{
		std::ifstream l_x_fs((LPCTSTR)l_cs_fname, std::ios::in | std::ios::binary);
		l_vuc_Kprivacy.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
		l_x_fs.close();
	}
	else
	{
		l_x_result_dlg.m_cs_Msg3.Format("GetFilesNameToOpen() error");
		l_i_ret = -1;
	}

	// load new key in device: key is sent in plaintext
	if (l_i_ret == 0)
	{
		l_i_ret = m_x_MorphoSmart.LoadKprivacy((const unsigned char *)l_vuc_Kprivacy.c_str(), l_vuc_Kprivacy.size());
		if (l_i_ret)
		{
			l_x_result_dlg.m_cs_Msg3.Format("C_MORPHO_Device::LoadKprivacy() error");
			l_i_ret = -1;
		}
	}

	if (l_i_ret == 0)
	{
		l_x_result_dlg.m_cs_Msg2.Format("Successful");
		Beep(BEEP_FREQUENCY, BEEP_DURATION);
	}
	else
	{
		CString l_cs_msg;
		l_x_result_dlg.m_cs_Msg2.Format("Failed");
		ConvertSDKError(l_i_ret, l_x_result_dlg.m_cs_Msg4);
		if (l_i_ret == MORPHOERR_STATUS)
		{
			l_cs_msg.Format("internal error %d", m_x_MorphoSmart.GetInternalError());
			l_x_result_dlg.m_cs_Msg4 += l_cs_msg;
		}
		//Beep(BEEP_FREQUENCY-300, BEEP_DURATION+400);
	}

	l_x_result_dlg.DoModal();

	return;
}

void CMSO_DemoDlg::OnLoadKprivacySymmSec()
{
	int							l_i_ret = 0;
	std::string					l_vuc_cur_Kprivacy;
	std::string					l_vuc_new_Kprivacy;
	unsigned char				l_auc_iv[DES_BLOCK_LENGTH];
	CString						l_cs_fname, l_cs_fext;
	UC							i_puc_Aes128CbcInputVector[AES128_BLOCK_LENGTH];
	UL							o_ul_KprivacyCryptedDataLen = 0;
	PUC							o_puc_KprivacyCryptedData = NULL;
	UL							o_ul_seedDataLen = 0;
	PUC							o_puc_seedData = NULL;
	UL							o_ul_seedCryptedDataLen = 0;
	PUC							o_puc_seedCryptedData = NULL;

	CResultDlg l_x_result_dlg;
	l_x_result_dlg.m_cs_Msg1.Format("Load Kprivacy");


	// read current key from file
	if (GetFilesNameToOpen("Kprivacy.cur.bin", l_cs_fname, l_cs_fext, "All Files (*.*)|*.*||"))
	{
		std::ifstream l_x_fs((LPCTSTR)l_cs_fname, std::ios::in | std::ios::binary);
		l_vuc_cur_Kprivacy.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
		l_x_fs.close();

		if (l_vuc_cur_Kprivacy.size() != AES128_BLOCK_LENGTH)
		{
			l_x_result_dlg.m_cs_Msg3.Format("error: key is not 128 bits long");
			l_i_ret = -1;
		}
	}
	else
	{
		l_x_result_dlg.m_cs_Msg3.Format("GetFilesNameToOpen() error");
		l_i_ret = -1;
	}

	// read new key from file
	if (l_i_ret == 0)
	{
		if (GetFilesNameToOpen("Kprivacy.new.bin", l_cs_fname, l_cs_fext, "All Files (*.*)|*.*||"))
		{
			std::ifstream l_x_fs((LPCTSTR)l_cs_fname, std::ios::in | std::ios::binary);
			l_vuc_new_Kprivacy.assign(std::istreambuf_iterator<char>(l_x_fs), std::istreambuf_iterator<char>());
			l_x_fs.close();

			if (l_vuc_new_Kprivacy.size() != AES128_BLOCK_LENGTH)
			{
				l_x_result_dlg.m_cs_Msg3.Format("error: key is not 128 bits long");
				l_i_ret = -1;
			}
		}
		else
		{
			l_x_result_dlg.m_cs_Msg3.Format("GetFilesNameToOpen() error");
			l_i_ret = -1;
		}
	}

	memset(i_puc_Aes128CbcInputVector, 0, AES128_BLOCK_LENGTH);
	if (i_puc_Aes128CbcInputVector == NULL)
	{
		l_x_result_dlg.m_cs_Msg3.Format("Allocation error");
		l_i_ret = -1;
	}

	// encrypt new key with current key
	if (l_i_ret == 0)
	{
		l_i_ret = MSOSECU_encrypt_aes128_cbc(l_vuc_cur_Kprivacy.size(),
			(PUC)l_vuc_cur_Kprivacy.c_str(),
			l_vuc_new_Kprivacy.size(),
			(PUC)l_vuc_new_Kprivacy.c_str(),
			i_puc_Aes128CbcInputVector, 
			FALSE,
			&o_ul_KprivacyCryptedDataLen,
			&o_puc_KprivacyCryptedData);

		if (l_i_ret)
		{
			l_x_result_dlg.m_cs_Msg3.Format("MSOSECU_encrypt_aes128_cbc() error");
			l_i_ret = -1;
		}
	}

	// encrypt seed with current key
	if (l_i_ret == 0)
	{
		//get the Privacy Seed from the device
		l_i_ret = m_x_MorphoSmart.GetPrivacySeed(o_puc_seedData, &o_ul_seedDataLen);
		if (l_i_ret == 0)
		{
			l_i_ret = MSOSECU_encrypt_aes128_cbc(l_vuc_cur_Kprivacy.size(),
				(PUC)l_vuc_cur_Kprivacy.c_str(),
				o_ul_seedDataLen,
				o_puc_seedData,
				o_puc_KprivacyCryptedData,
				TRUE,
				&o_ul_seedCryptedDataLen,
				&o_puc_seedCryptedData);

			if (l_i_ret)
			{
				l_x_result_dlg.m_cs_Msg3.Format("MSOSECU_encrypt_aes128_cbc() error");
				l_i_ret = -1;
			}
		}
		else
		{
			l_x_result_dlg.m_cs_Msg3.Format("C_MORPHO_Device::GetPrivacySeed() error");
			l_i_ret = -1;
		}
	}

	// load new key in device: key is sent in ciphertext
	if (l_i_ret == 0)
	{
		l_i_ret = m_x_MorphoSmart.LoadKprivacySecurely((PUC)o_puc_KprivacyCryptedData, o_ul_KprivacyCryptedDataLen, o_puc_seedCryptedData, o_ul_seedCryptedDataLen);
		if (l_i_ret)
		{
			l_x_result_dlg.m_cs_Msg3.Format("C_MORPHO_Device::LoadKprivacySecurely() error");
			l_i_ret = -1;
		}
	}

	if (o_puc_KprivacyCryptedData)
		MSOSECU_Free(&o_puc_KprivacyCryptedData);

	if (o_puc_seedData)
		m_x_MorphoSmart.Free((void**)&o_puc_seedData);

	if (o_puc_seedCryptedData)
		MSOSECU_Free(&o_puc_seedCryptedData);


	if (l_i_ret == 0)
	{
		l_x_result_dlg.m_cs_Msg2.Format("Successful");
		Beep(BEEP_FREQUENCY, BEEP_DURATION);
	}
	else
	{
		CString l_cs_msg;
		l_x_result_dlg.m_cs_Msg2.Format("Failed");
		ConvertSDKError(l_i_ret, l_x_result_dlg.m_cs_Msg4);
		if (l_i_ret == MORPHOERR_STATUS)
		{
			l_cs_msg.Format("internal error %d", m_x_MorphoSmart.GetInternalError());
			l_x_result_dlg.m_cs_Msg4 += l_cs_msg;
		}
		//Beep(BEEP_FREQUENCY-300, BEEP_DURATION+400);
	}

	l_x_result_dlg.DoModal();

	return;
}

BOOL CMSO_DemoDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_ChangeCursor == TRUE)
	{
		RestoreWaitCursor();
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
