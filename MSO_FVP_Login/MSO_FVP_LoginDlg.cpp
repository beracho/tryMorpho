// MSO_FVP_LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_FVP_Login.h"
#include "MSO_FVP_LoginDlg.h"
#include "ResultDlg.h"
#include "MSOSECU.h"
#include "SecurityLvlDlg.h"

#include "ConvertSDKError.h"
#include "InfoVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString		g_str_ProductDesc;
CString		g_str_SoftwareDesc;
CString		g_str_SensorDesc;

//////////////////////////////////////////////////////////////////
I BiometricCallback(	PVOID						i_pv_context,
						T_MORPHO_CALLBACK_COMMAND	i_i_command,
						PVOID						i_pv_param	
						)
{
	PUC		l_puc_EnrollmentCmd;	
	CDC		*l_p_Cdc;
	CRect	myRect;
	UC l_uc_quality;

	// Correction fait par C++Test [BD-PB-NOTINIT-1]
	T_MORPHO_CALLBACK_IMAGE_STATUS l_x_ImageStructure;
    memset ( &l_x_ImageStructure, NULL, sizeof(l_x_ImageStructure));
	CMSO_FVP_LoginDlg* MSO_FVP_LoginDlg = (CMSO_FVP_LoginDlg *)i_pv_context;

	//-----------------------------------------------------------
	// Asynchronous Position Messages
	//-----------------------------------------------------------

		if (i_i_command == MORPHO_CALLBACK_COMMAND_CMD)
		{	
			switch(*(PI)i_pv_param)
			{				
				case MORPHO_REMOVE_FINGER:
					if(MSO_FVP_LoginDlg->m_i_CurrentViewerIndex == 0)		
					{
						MSO_FVP_LoginDlg->m_ctl_CaptureIcon1.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadIcon(IDI_ICON_OK) );	
						MSO_FVP_LoginDlg->ShowAnimation("image_Ok");
					}
					else if(MSO_FVP_LoginDlg->m_i_CurrentViewerIndex == 1)	
					{
						MSO_FVP_LoginDlg->m_ctl_CaptureIcon2.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadIcon(IDI_ICON_OK) );
						MSO_FVP_LoginDlg->ShowAnimation("image_Ok");
					}
					else if(MSO_FVP_LoginDlg->m_i_CurrentViewerIndex == 2)	
					{
						MSO_FVP_LoginDlg->m_ctl_CaptureIcon3.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadIcon(IDI_ICON_OK) );
						MSO_FVP_LoginDlg->ShowAnimation("image_Ok");
					}
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("REMOVE FINGER");
					Beep(BEEP_FREQUENCY,BEEP_DURATION);
					break;
				case MORPHO_MOVE_NO_FINGER:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("PLACE YOUR FINGER");
					if(MSO_FVP_LoginDlg->m_bl_FVP)
					{
						MSO_FVP_LoginDlg->ShowAnimation("capture");
					}

					break;
				case MORPHO_LATENT:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("LATENT");
					break;
				case MORPHO_MOVE_FINGER_UP:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("MOVE UP");
					break;
				case MORPHO_MOVE_FINGER_DOWN:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("MOVE DOWN");
					break;
				case MORPHO_MOVE_FINGER_LEFT:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("MOVE LEFT");
					break;
				case MORPHO_MOVE_FINGER_RIGHT:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("MOVE RIGHT");
					break;
				case MORPHO_PRESS_FINGER_HARDER:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("PRESS HARDER");
					break;
				case MORPHO_FINGER_OK:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("ACQUISITION OK");
					//MSO_FVP_LoginDlg->ShowAnimation("image_Ok");
					break;
				case MORPHOERR_FFD:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("FALSE FINGER DETECTED !!!");	
					break;
				case MORPHOERR_MOIST_FINGER:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("FINGER TOO MOIST !!!");
					break;
				case MORPHOERR_MOVED_FINGER:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("FINGER MOVED !!!");
					break;
				case MORPHOERR_SATURATED_FINGER:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("FINGER SATURATED !!!");
					break;
				case MORPHOERR_INVALID_FINGER:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("INVALID FINGER !!!");
					break;
				case MORPHO_FINGER_DETECTED:
					
					if(!MSO_FVP_LoginDlg->m_bl_FVP)
					{
						MSO_FVP_LoginDlg->HideAnimation();
					}
					else
					{
						MSO_FVP_LoginDlg->ShowAnimation("maillage");	
					}

					break;
				case MORPHO_LIVE_OK:
					
					if(!MSO_FVP_LoginDlg->m_bl_FVP)
					{
						MSO_FVP_LoginDlg->HideAnimation();
					}else
					{
						MSO_FVP_LoginDlg->ShowAnimation("maillage");
					}
						break;
				case MORPHO_FINGER_MISPLACED:
					MSO_FVP_LoginDlg->m_cs_CommandMsg.Format("BAD FINGER PLACEMENT");
						break;
			}
			MSO_FVP_LoginDlg->PostMessage(WM_UPDATE_DATA_FALSE,0,0);

			if((*(PI)i_pv_param) != MORPHO_MOVE_NO_FINGER)
			{
				MSO_FVP_LoginDlg->PostMessage(WM_DISABLE_ERASE_BASE_BUTTON,0,0);
			}
			else
			{
				MSO_FVP_LoginDlg->PostMessage(WM_ENABLE_ERASE_BASE_BUTTON,0,0);
			}
		}

	//-----------------------------------------------------------
	// Asynchronous SendQualityCode Message
	//-----------------------------------------------------------
		if (i_i_command == MORPHO_CALLBACK_CODEQUALITY)
		{
			l_uc_quality = *(PUC)i_pv_param;
			MSO_FVP_LoginDlg->m_cs_CommandMsg2.Format("CODED QUALITY: %d",l_uc_quality);
			MSO_FVP_LoginDlg->PostMessage(WM_UPDATE_DATA_FALSE,0,0);
			/////////////////////////////////////
			if(((int)l_uc_quality) >20)
			{
				MSO_FVP_LoginDlg->m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(0,255,0));
			}
			else
			{
				MSO_FVP_LoginDlg->m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(0,0,255)); 
			}

			MSO_FVP_LoginDlg->m_qualiteProgress.ShowWindow(SWP_SHOWWINDOW);
			MSO_FVP_LoginDlg->m_qualiteProgress.SetPos((int)l_uc_quality);
			/////////////////////////////////////
		}

		if (i_i_command == MORPHO_CALLBACK_DETECTQUALITY)
		{
			l_uc_quality = *(PUC)i_pv_param;
			MSO_FVP_LoginDlg->m_cs_CommandMsg2.Format("DETECTED QUALITY: %d",l_uc_quality);
			MSO_FVP_LoginDlg->PostMessage(WM_UPDATE_DATA_FALSE,0,0);

			/////////////////////////////////////
			if(((int)l_uc_quality) >20)
			{
				MSO_FVP_LoginDlg->m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(0,255,0));
			}
			else
			{
				MSO_FVP_LoginDlg->m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(0,0,255)); 
			}

			MSO_FVP_LoginDlg->m_qualiteProgress.ShowWindow(SWP_SHOWWINDOW);
			MSO_FVP_LoginDlg->m_qualiteProgress.SetPos((int)l_uc_quality);
			/////////////////////////////////////
		}

	//-----------------------------------------------------------
	// Asynchronous Command Messages
	//-----------------------------------------------------------

		// l_puc_EnrollmentCmd[0] // Current Finger number enrolled(1 or 2) 
		// l_puc_EnrollmentCmd[1] // Number of fingers to enroll (1 or 2)
		// l_puc_EnrollmentCmd[2] // Current acquisition number (1 to 3) 
		// l_puc_EnrollmentCmd[3] // Total number of acquisition (3)		

		if (i_i_command == MORPHO_CALLBACK_ENROLLMENT_CMD)
		{
			l_puc_EnrollmentCmd = (PUC)i_pv_param;		
			switch (l_puc_EnrollmentCmd[0])
			{
				case 1:
					switch (l_puc_EnrollmentCmd[2])
					{
						case 1:	
							MSO_FVP_LoginDlg->m_cs_CaptureMsg1.Format("\nCAPTURE 1/3");
							MSO_FVP_LoginDlg->ShowAnimation("image_Empty");
							MSO_FVP_LoginDlg->m_ctl_CurrentMiniViewer = &(MSO_FVP_LoginDlg->m_ctl_MiniViewer1);	
							MSO_FVP_LoginDlg->m_i_CurrentViewerIndex = 0;
								MSO_FVP_LoginDlg->m_ctl_CaptureIcon1.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadStandardIcon(IDI_QUESTION ) );
							break;

						case 2:
							MSO_FVP_LoginDlg->m_cs_CaptureMsg2.Format("\nCAPTURE 2/3");
							MSO_FVP_LoginDlg->ShowAnimation("image_Empty");
							MSO_FVP_LoginDlg->m_ctl_CaptureIcon1.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadIcon(IDI_ICON_OK) );																								
						    MSO_FVP_LoginDlg->m_ctl_CurrentMiniViewer = &(MSO_FVP_LoginDlg->m_ctl_MiniViewer2);
							MSO_FVP_LoginDlg->m_i_CurrentViewerIndex = 1;	
							MSO_FVP_LoginDlg->m_ctl_CaptureIcon2.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadStandardIcon(IDI_QUESTION ) );	
							break;

						case 3:
							MSO_FVP_LoginDlg->m_cs_CaptureMsg3.Format("\nCAPTURE 3/3");
							MSO_FVP_LoginDlg->ShowAnimation("image_Empty");
							MSO_FVP_LoginDlg->m_ctl_CaptureIcon2.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadIcon(IDI_ICON_OK) );
							MSO_FVP_LoginDlg->m_ctl_CurrentMiniViewer = &(MSO_FVP_LoginDlg->m_ctl_MiniViewer3);
							MSO_FVP_LoginDlg->m_i_CurrentViewerIndex = 2;
							MSO_FVP_LoginDlg->m_ctl_CaptureIcon3.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadStandardIcon(IDI_QUESTION ) );	
							break;
					}
					break;

				case 2:
					// Enrollment 1 finger only
					Sleep(1);
					break;				
			}					
			MSO_FVP_LoginDlg->PostMessage(WM_UPDATE_DATA_FALSE,0,0);
		}

	//-----------------------------------------------------------
	// Asynchronous Images 
	//-----------------------------------------------------------

		if (i_i_command == MORPHO_CALLBACK_IMAGE_CMD)
		{
			memcpy(&l_x_ImageStructure.m_x_ImageHeader, (T_MORPHO_IMAGE_HEADER*)i_pv_param, sizeof(T_MORPHO_IMAGE_HEADER) );
			l_x_ImageStructure.m_puc_Image = (PUC)i_pv_param+sizeof(T_MORPHO_IMAGE_HEADER);


			if(MSO_FVP_LoginDlg->m_b_Expand) // Enroll Mode ?
			{
				l_p_Cdc = MSO_FVP_LoginDlg->m_ctl_CurrentMiniViewer->GetDC();
				MSO_FVP_LoginDlg->m_ctl_CurrentMiniViewer->GetClientRect(&myRect);	
				MSO_FVP_LoginDlg->m_px_Image->SetImage(	
					MSO_FVP_LoginDlg->m_i_CurrentViewerIndex,	// UL	i_ul_ImageIndex,
					l_x_ImageStructure,						// T_MORPHO_CALLBACK_IMAGE_STATUS	i_x_Image,
					0,										// UL	i_ul_imageFilter,
					l_p_Cdc,								// CDC	*i_p_Cdc,
					myRect									// CRect &i_p_Rect
					);
				MSO_FVP_LoginDlg->ReleaseDC(l_p_Cdc);

				MSO_FVP_LoginDlg->PostMessage(WM_PAINT,0,0);
			}
			if(!MSO_FVP_LoginDlg->m_bl_FVP)
			{
				l_p_Cdc = MSO_FVP_LoginDlg->m_ctl_Viewer.GetDC();
				MSO_FVP_LoginDlg->m_ctl_Viewer.GetClientRect(&myRect);	
				MSO_FVP_LoginDlg->m_px_Image->SetImage(	
						MSO_FVP_LoginDlg->m_i_CurrentViewerIndex,	// UL	i_ul_ImageIndex,
						l_x_ImageStructure,						// T_MORPHO_CALLBACK_IMAGE_STATUS	i_x_Image,
						MORPHO_LINEAR_INTERPOLATION,			// UL	i_ul_imageFilter,
						l_p_Cdc,								// CDC	*i_p_Cdc,
						myRect									// CRect &i_p_Rect
						);
				MSO_FVP_LoginDlg->ReleaseDC(l_p_Cdc);
			}
			
		}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_str_product_desc;
	CString	m_str_sensor_desc;
	CString	m_str_software_desc;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	virtual INT_PTR DoModal();
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
	m_str_product_desc = _T("");
	m_str_sensor_desc = _T("");
	m_str_software_desc = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_PRODUCT_DESC, m_str_product_desc);
	DDX_Text(pDX, IDC_SENSOR_DESC, m_str_sensor_desc);
	DDX_Text(pDX, IDC_SOFTWARE_DESC, m_str_software_desc);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

INT_PTR CAboutDlg::DoModal() 
{
	
	m_str_product_desc = g_str_ProductDesc;
	m_str_software_desc = g_str_SoftwareDesc;
	m_str_sensor_desc = g_str_SensorDesc;

	return CDialog::DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMSO_FVP_LoginDlg dialog

CMSO_FVP_LoginDlg::CMSO_FVP_LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMSO_FVP_LoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMSO_FVP_LoginDlg)
	m_cs_EditName = _T("");
	m_cs_CommandMsg = _T("");
	m_cs_CommandMsg2 = _T("");
	m_cs_CaptureMsg1 = _T("");
	m_cs_CaptureMsg2 = _T("");
	m_cs_CaptureMsg3 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_b_EnrollmentProcessing = FALSE;
	m_px_Image = NULL;
	m_b_ExitThread = FALSE;
	m_b_ReadyToExit = TRUE;
	m_b_AskedForBaseRemoving = FALSE;

	m_vidPlayer[0] = NULL;
	m_vidPlayer[1] = NULL;
}

void CMSO_FVP_LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMSO_FVP_LoginDlg)
	DDX_Control(pDX, IDC_STATIC_COMMAND_MSG2, m_ctl_CommandMsg2);
	DDX_Control(pDX, IDC_STATIC_MINI_VIEWER3, m_ctl_MiniViewer3);
	DDX_Control(pDX, IDC_STATIC_MINI_VIEWER2, m_ctl_MiniViewer2);
	DDX_Control(pDX, IDC_STATIC_MINI_VIEWER1, m_ctl_MiniViewer1);
	DDX_Control(pDX, IDC_STATIC_CAPTURE_ICON3, m_ctl_CaptureIcon3);
	DDX_Control(pDX, IDC_STATIC_CAPTURE_ICON2, m_ctl_CaptureIcon2);
	DDX_Control(pDX, IDC_STATIC_CAPTURE_ICON1, m_ctl_CaptureIcon1);
	DDX_Control(pDX, IDC_STATIC_CAPTURE_MSG3, m_ctl_CaptureMsg3);
	DDX_Control(pDX, IDC_STATIC_CAPTURE_MSG2, m_ctl_CaptureMsg2);
	DDX_Control(pDX, IDC_STATIC_CAPTURE_MSG1, m_ctl_CaptureMsg1);
	DDX_Control(pDX, IDC_STATIC_COMMAND_MSG, m_ctl_CommandMsg);
	DDX_Control(pDX, IDC_STATIC_VIEWER, m_ctl_Viewer);
	DDX_Control(pDX, IDC_EDIT_NAME, m_ctl_EditName);
	DDX_Control(pDX, IDC_STATIC_MSG1, m_ctl_StaticMsg1);
	DDX_Text(pDX, IDC_EDIT_NAME, m_cs_EditName);
	DDX_Text(pDX, IDC_STATIC_COMMAND_MSG, m_cs_CommandMsg);
	DDX_Text(pDX, IDC_STATIC_COMMAND_MSG2, m_cs_CommandMsg2);
	DDX_Text(pDX, IDC_STATIC_VIEWER, m_cs_Viewer);
	DDX_Text(pDX, IDC_STATIC_CAPTURE_MSG1, m_cs_CaptureMsg1);
	DDX_Text(pDX, IDC_STATIC_CAPTURE_MSG2, m_cs_CaptureMsg2);
	DDX_Text(pDX, IDC_STATIC_CAPTURE_MSG3, m_cs_CaptureMsg3);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_QUALITE_PROGRESS, m_qualiteProgress);
	DDX_Control(pDX, IDC_CLOSE_BTN, m_ctrl_Close);
	DDX_Control(pDX, IDC_ERDB_BTN, m_ctrl_destroyBd);
	DDX_Control(pDX, IDC_SECLVL_BTN, m_ctrl_SecurityLvl);
	DDX_Control(pDX, IDC_ENROLL_BTN, m_ctrl_Enroll);
	DDX_Control(pDX, IDC_NEWUSER_BTN, m_ctrl_addUser);
}

BEGIN_MESSAGE_MAP(CMSO_FVP_LoginDlg, CDialog)
	//{{AFX_MSG_MAP(CMSO_FVP_LoginDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	ON_WM_CLOSE()
	ON_WM_CANCELMODE()
	ON_MESSAGE(WM_UPDATE_DATA_FALSE, OnUpdateDataFalse)
	ON_MESSAGE(WM_UPDATE_DATA_TRUE, OnUpdateDataTrue)
	ON_MESSAGE(WM_ENABLE_CLOSE, OnEnableCloseButton)
	ON_MESSAGE(WM_DISABLE_CLOSE, OnDisableCloseButton)
	ON_MESSAGE(WM_DISABLE_ERASE_BASE_BUTTON, OnDisableEraseBaseButton)
	ON_MESSAGE(WM_ENABLE_ERASE_BASE_BUTTON, OnEnableEraseBaseButton)
	ON_MESSAGE(WM_DISABLE_SECURITY_LEVEL,OnDisableSecurityLevelButton)
	ON_MESSAGE(WM_ENABLE_SECURITY_LEVEL,OnEnableSecurityLevelButton)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_CLOSE_BTN, &CMSO_FVP_LoginDlg::OnStnClickedCloseBtn)
	ON_STN_CLICKED(IDC_ERDB_BTN, &CMSO_FVP_LoginDlg::OnStnClickedErdbBtn)
	ON_STN_CLICKED(IDC_NEWUSER_BTN, &CMSO_FVP_LoginDlg::OnStnClickedNewuserBtn)
	ON_STN_CLICKED(IDC_ENROLL_BTN, &CMSO_FVP_LoginDlg::OnStnClickedEnrollBtn)
	ON_STN_CLICKED(IDC_SECLVL_BTN, &CMSO_FVP_LoginDlg::OnStnClickedSeclvlBtn)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
///						CMSO_FVP_LoginDlg Methods							  ///
/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
LRESULT CMSO_FVP_LoginDlg::OnUpdateDataFalse(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);
	return	0L;

}

//////////////////////////////////////////////////////////////////
LRESULT CMSO_FVP_LoginDlg::OnUpdateDataTrue(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	return	0L;
}

//////////////////////////////////////////////////////////////////
LRESULT CMSO_FVP_LoginDlg::OnEnableCloseButton(WPARAM wParam, LPARAM lParam)
{
	m_x_btnState.m_b_closeBtnEnabled = TRUE;
	m_ctrl_Close.SetBitmap(m_hBitmap_Close);
	UpdateData(TRUE);
	return	0L;
}

//////////////////////////////////////////////////////////////////
LRESULT CMSO_FVP_LoginDlg::OnDisableCloseButton(WPARAM wParam, LPARAM lParam)
{
	m_x_btnState.m_b_closeBtnEnabled = FALSE;
    m_ctrl_Close.SetBitmap(m_hBitmap_DisabledClose);
	return	0L;
}

//////////////////////////////////////////////////////////////////
LRESULT CMSO_FVP_LoginDlg::OnDisableEraseBaseButton(WPARAM wParam, LPARAM lParam)
{
	m_x_btnState.m_b_destroyBtnEnabled = FALSE;
	m_ctrl_destroyBd.SetBitmap(m_hBitmap_DisabledEraseBd);
	return	0L;
}

//////////////////////////////////////////////////////////////////
LRESULT CMSO_FVP_LoginDlg::OnEnableEraseBaseButton(WPARAM wParam, LPARAM lParam)
{
	if(!m_b_Expand)
	{
		m_x_btnState.m_b_destroyBtnEnabled = TRUE;
		m_ctrl_destroyBd.SetBitmap(m_hBitmap_EraseBd);
		UpdateData(TRUE);
	}
	return	0L;
}
//////////////////////////////////////////////////////////////////
LRESULT CMSO_FVP_LoginDlg::OnDisableSecurityLevelButton(WPARAM wParam, LPARAM lParam)
{
	m_x_btnState.m_b_securityLvlBtnEnabled = FALSE;
	m_ctrl_SecurityLvl.SetBitmap(m_hBitmap_DisabledScLevel);

	UpdateData(TRUE);
	return	0L;
}
//////////////////////////////////////////////////////////////////
LRESULT CMSO_FVP_LoginDlg::OnEnableSecurityLevelButton(WPARAM wParam, LPARAM lParam)
{
	m_x_btnState.m_b_securityLvlBtnEnabled = TRUE;
	m_ctrl_SecurityLvl.SetBitmap(m_hBitmap_ScLevel);

	UpdateData(TRUE);

	return	0L;
}

//////////////////////////////////////////////////////////////////
void CMSO_FVP_LoginDlg::ExpandDialog()
{
	// This function is used to switch between :
	// - Enrollment mode
	//       The window is enlarged in order to show the controls 
	//       used for enrolment. The width of the window is given
	//       by the position of the IDC_LARGE control.
	//       m_b_Expand is then set to TRUE.
	// - Acquisition mode (m_b_Expand == FALSE)
	//       The width of the window is reduced to hide the 
	//       controls used for enrollment. The width of the window 
	//       is given by the position of the IDC_SMALL control.
	//       m_b_Expand is then set to FALSE.
	
	CRect rcDialog;
	GetWindowRect( &rcDialog );
	bool l_b_Temp;
	int nNewWidth = -1;

	l_b_Temp = !m_b_Expand;

	if( l_b_Temp )
	{
		CWnd* pWndLarge = GetDlgItem( IDC_LARGE );
		ASSERT_VALID( pWndLarge );

		CRect rcLarge;
		pWndLarge->GetWindowRect( &rcLarge );

		nNewWidth = rcLarge.left-rcDialog.left;			
	}
	else
	{
		CWnd* pWndSmall = GetDlgItem( IDC_SMALL );
		ASSERT_VALID( pWndSmall );

		CRect rcSmall;
		pWndSmall->GetWindowRect( &rcSmall );

		nNewWidth = rcSmall.left-rcDialog.left;
	}

	ASSERT( nNewWidth > 0 );
	SetWindowPos( NULL, 0, 0,
						 nNewWidth,rcDialog.Height(),
						SWP_NOMOVE | SWP_NOZORDER );

	CWnd* pWndControl = GetWindow( GW_CHILD );
	while( pWndControl != NULL )
	{
		CRect rcControl;
		pWndControl->GetWindowRect( &rcControl );
		pWndControl->EnableWindow( rcControl.left <= rcDialog.left + nNewWidth );
		pWndControl = pWndControl->GetWindow( GW_HWNDNEXT );
	}

	CWnd* pWndActiveControl = CWnd::GetFocus();
	if( pWndActiveControl == NULL )
	{
		CWnd* pWndFirstControl = GetNextDlgTabItem( NULL );
		ASSERT_VALID( pWndFirstControl );
		ASSERT( pWndFirstControl->IsWindowEnabled() );
		pWndFirstControl->SetFocus();
	}

	CWnd* pWndEdit = GetDlgItem( IDC_EDIT_NAME );
	pWndEdit->SetFocus();
	CenterWindow();
	m_b_Expand = l_b_Temp;
}

/////////////////////////////////////////////////////////////////////////////
UINT TaskIdentify( LPVOID pParam )
{
	bool l_b_Ret;
	CMSO_FVP_LoginDlg *MSO_FVP_LoginDlg = (CMSO_FVP_LoginDlg*) pParam;
	l_b_Ret = MSO_FVP_LoginDlg->T_Identify();

	MSO_FVP_LoginDlg->InitQualityBar();
	return l_b_Ret ;
}

/////////////////////////////////////////////////////////////////////////////
UINT TaskEnroll( LPVOID pParam )
{
	bool l_b_Ret;
	CMSO_FVP_LoginDlg *MSO_FVP_LoginDlg = (CMSO_FVP_LoginDlg*) pParam;
	l_b_Ret = MSO_FVP_LoginDlg->T_Enroll();

	MSO_FVP_LoginDlg->InitQualityBar();
	return l_b_Ret;
}

/////////////////////////////////////////////////////////////////////////////
// CMSO_FVP_LoginDlg message handlers

BOOL CMSO_FVP_LoginDlg::OnInitDialog()
{
	CString l_cs_Interface, l_cs_Message;
	CString l_cs_MsoName;
	S		l_s_PortCom;
	I		l_i_Ret;
	I		l_i_BaudRate;

	UC							l_auc_SecuSerialNumber[MSO_SERIAL_NUMBER_LEN+1];
	UC							l_uc_SecuConfig;
	T_MORPHO_FAR				l_x_SecuFar;
	UL							l_ul_CertifHostSize;
	PUC							l_puc_CertifHost;
	UL							l_ul_TimeOut;

	CDialog::OnInitDialog();
	
	SetTitleBar ( m_hWnd );

	m_b_Expand=TRUE;
	m_b_securityLevelChanged = FALSE;

	//*************************************************************
	
	SetClassLongPtr(GetDlgItem(IDC_ENROLL_BTN)->m_hWnd,GCLP_HCURSOR,(LONG)LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
	
	ExpandDialog();

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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	#ifdef APP_MODE_1
//		GetDlgItem(IDC_BUTTON_EXPAND)->ShowWindow(SW_HIDE);
	#endif

	//---------------------------------------------
	// Create a big font 
	//---------------------------------------------

		CRect	rRect;
		int	iHeight;
		
		m_ctl_StaticMsg1.GetWindowRect(&rRect);

		iHeight = rRect.top - rRect.bottom;

		if(iHeight < 0) iHeight = 0 - iHeight;

		m_fFont.Detach();
		m_fFont.CreateFont((iHeight - 5),0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"");

		m_ctl_StaticMsg1.SetFont(&m_fFont);
		m_ctl_EditName.SetFont(&m_fFont);
		m_ctl_CommandMsg.SetFont(&m_fFont);
		m_ctl_Viewer.SetFont(&m_fFont);
		m_ctl_CaptureMsg1.SetFont(&m_fFont);
		m_ctl_CaptureMsg2.SetFont(&m_fFont);
		m_ctl_CaptureMsg3.SetFont(&m_fFont);

		m_ctl_EditName.SetLimitText(23);
		UpdateData(FALSE);

	//---------------------------------------------
	// Open Com
	//---------------------------------------------
		
		l_cs_Interface = APP->GetProfileString(COM_PARAMETERS, COM_INTERFACE, DEFAULT_COM_INTERFACE); 
	
		if(l_cs_Interface.Compare("USB") == 0)
		{
			l_cs_MsoName = APP->GetProfileString(COM_PARAMETERS, SERIALNUMBER, DEFAULT_SERIALNUMBER); 
			l_ul_TimeOut = APP->GetProfileInt( COM_PARAMETERS, TIMEOUT,DEFAULT_TIMEOUT );

			l_i_Ret = m_x_MorphoSmart.OpenUsbDevice ( 
					(PC)(LPCTSTR)l_cs_MsoName,	//PC								i_pc_MsoName,
					l_ul_TimeOut,				//UL								i_ul_TimeOut,
					NULL						//PVOID								io_pv_param
					);

			if(l_i_Ret == MORPHOERR_NO_SERVER)
			{
				ShowErrorBox("Server stopped or not installed\nPlease start or install the Morpho m_x_MorphoSmart Service Provider Usb Server", l_i_Ret);
				CDialog::OnCancel();
				return FALSE;
			}

			if( (l_i_Ret != MORPHO_OK) && (l_i_Ret != MORPHOERR_NO_SERVER) )
			{
				ShowErrorBox("The connection with the MSO failed. Please\ncheck the device is correctly plugged", l_i_Ret);
				CDialog::OnCancel();
				return FALSE;
			}
		}
		else
		{
			l_i_BaudRate = APP->GetProfileInt( COM_PARAMETERS, BAUD_RATE,DEFAULT_BAUD_RATE );
			l_s_PortCom = APP->GetProfileInt( COM_PARAMETERS, PORT_NUMBER, DEFAULT_COM );

			l_i_Ret = m_x_MorphoSmart.OpenDevice(l_s_PortCom,l_i_BaudRate);

			if(l_i_Ret == MORPHOERR_NO_SERVER)
			{
				ShowErrorBox("Server stopped or not installed\nPlease start or install the Morpho m_x_MorphoSmart Service Provider Usb Server", l_i_Ret);
				CDialog::OnCancel();
				return FALSE;
			}

			if( (l_i_Ret != MORPHO_OK) && (l_i_Ret != MORPHOERR_NO_SERVER) )
			{
				l_cs_Message.Format("The connection with the MSO failed. Please\ncheck the device is correctly plugged on COM%d",l_s_PortCom);
				ShowErrorBox(l_cs_Message, l_i_Ret);
				CDialog::OnCancel();
				return FALSE;
			}
		}

	//---------------------------------------------
	// Setup security
	//---------------------------------------------
	
		memset(l_auc_SecuSerialNumber,0,sizeof(l_auc_SecuSerialNumber));

		l_i_Ret = m_x_MorphoSmart.GetSecuConfig(
								l_auc_SecuSerialNumber,		//UC	o_auc_SecuSerialNumber[MSO_SERIAL_NUMBER_LEN],
								&l_uc_SecuConfig,			//PUC	o_puc_SecuConfig
								&l_x_SecuFar				//PUS	o_pus_SecuFar
								);
	
		if(l_i_Ret)
		{
			ShowErrorBox("An error occured while calling C_MORPHO_Device::GetSecuConfig()", l_i_Ret);
			CDialog::OnCancel();
			return FALSE;
		}

		// GetSecuConfig is not implemented in some old MSO
		// In this case, l_i_Ret != MORPHO_OK. This implies
		// that the MSO doesn't include security features.

		if(l_i_Ret == MORPHO_OK)
		{
			if(l_uc_SecuConfig !=0) 
			{
				if(l_uc_SecuConfig&SECU_TUNNELING)
				{
					l_i_Ret = MSOSECU_GetHostCertif(
										&l_ul_CertifHostSize,	//PUL			o_pul_CertifHostSize,
										&l_puc_CertifHost		//PUC*		o_ppuc_CertifHost
										);

					if(l_i_Ret)
					{
						ShowErrorBox("Cannot read Host certificate\nPlease check that:\n1/ You have generate the HOST keys (with the MKMS or other)\n2/ You have copied host.der (HOST public certificate), host.key (HOST private key), ca.crt (CA public key that can be found in the MSKM station) in the directory .\\openssl\\keys.", l_i_Ret);
						CDialog::OnCancel();
						return FALSE;
					}

					l_i_Ret = m_x_MorphoSmart.TunnelingOpen(
									MSOSECU_TripleDesSign,		//T_MORPHO_CALLBACK_TRIPLE_DES_SIGN		i_pf_CallbackTripleDesSign,
									MSOSECU_TripleDesCrypt,		//T_MORPHO_CALLBACK_TRIPLE_DES_CRYPT		i_pf_CallbackTripleDesCrypt,
									MSOSECU_TripleDesVerifSign,	//T_MORPHO_CALLBACK_TRIPLE_DES_VERIF_SIGN	i_pf_CallbackTripleDesVerifSign,
									MSOSECU_TripleDesDecrypt,	//T_MORPHO_CALLBACK_TRIPLE_DES_DECRYPT	i_pf_CallbackTripleDesDecrypt,
									MSOSECU_TunnelingInit1,		//T_MORPHO_CALLBACK_TUNNELING_INIT1		i_pf_CallbackTunnelingInit1,
									MSOSECU_TunnelingInit2,		//T_MORPHO_CALLBACK_TUNNELING_INIT2		i_pf_CallbackTunnelingInit2,
									l_ul_CertifHostSize,		//UL										i_ul_HostCertificateSize,
									l_puc_CertifHost			//PUC										i_puc_HostCertificate
									);
					if(l_i_Ret)
					{
						ShowErrorBox("Security Tunneling Initialization failed\nPlease check that:\n1/ You have generate the HOST keys (with the MKMS or other)\n2/ You have copied host.der (HOST public certificate), host.key (HOST private key), ca.crt (CA public key that can be found in the MSKM station) in the directory .\\openssl\\keys.", l_i_Ret);
						CDialog::OnCancel();
						return FALSE;
					}
				}
				else if(l_uc_SecuConfig&SECU_OFFERED_SECURITY)
				{
					l_i_Ret = m_x_MorphoSmart.OfferedSecuOpen(
						MSOSECU_GenRandom,						//T_CallbackGenRandom						i_pf_CallbackGenRandom,
						MSOSECU_VerifOfferedSecuritySignature,	//T_CallbackVerifOfferedSecuritySignature	i_pf_CallbackVerifOfferedSecuritySignature,
						MSOSECU_VerifCertif						//T_MORPHO_CALLBACK_VERIF_CERTIF
						);

					if (l_i_Ret)
					{
						ShowErrorBox("Offered Security Initialization failed", l_i_Ret);
						if (AfxMessageBox("The m_x_MorphoSmart Certificate has not been authenticated.\nPlease check that you have copied ca.crt (CA public key that can be found in the MKMS station) in the directory .\\openssl\\keys.\nContinue anyway ?",MB_YESNO) == IDYES)
						{
							l_i_Ret = MORPHO_OK;
						}
						else
						{		
							CDialog::OnCancel();
							return FALSE;
						}
					}
				}
			}
		}
		g_str_ProductDesc = m_x_MorphoSmart.m_puc_ProductDescriptor;
		g_str_SoftwareDesc = m_x_MorphoSmart.m_puc_SoftwareDescriptor;
		g_str_SensorDesc = m_x_MorphoSmart.m_puc_SensorDescriptor;

	//---------------------------------------------
	// Get Base if the soft is standard 
	// close this application if the soft support WSQ Image format
	//---------------------------------------------
        CheckIfCBMV(); // check if CBM-V and disable MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE in ENROLL
		GetFVP_Status();
		GetWsqValue();
		if ( m_bl_WSQ == true )
		{
			GetBaseBioValue();
		}
		else
		{
			m_bl_BaseBio = true;
		}
		
		if(m_bl_WSQ == true && m_bl_BaseBio == false)
		{
			CString l_cs_BoxMessage;
			l_cs_BoxMessage.Format("This application does not support %s !\n ",m_x_MorphoSmart.m_puc_SoftwareDescriptor);
			AfxMessageBox(l_cs_BoxMessage,MB_ICONERROR);
			CDialog::OnCancel();
			return FALSE;
		}
		else
		{
			l_i_Ret = m_x_MorphoSmart.GetDatabase(0, m_x_Database);
			
			if (l_i_Ret == -13)
				{
					l_i_Ret = MORPHO_OK;
				}

			if(l_i_Ret != MORPHO_OK)
			{
				ShowErrorBox("An error occured while calling C_MORPHO_Device::GetDatabase()", l_i_Ret);
				CDialog::OnCancel();
				return FALSE;
			}
		}

	//On regle les couleurs
	m_brush.CreateSolidBrush(RGB(5, 5, 5));

	//Quality progress bar
	m_qualiteProgress.SetRange(LIVE_QUALITY_MIN, LIVE_QUALITY_MAX);
    m_qualiteProgress.ShowWindow(SWP_HIDEWINDOW);

	/*Video player***********************************/
	CRect rect;
	m_ctl_Viewer.GetWindowRect(&rect);  
	ScreenToClient(&rect);

	RECT Recto = { 0, 0, (long)rect.Width()-5, (long)rect.Height() -5}; //height and width of m_ctl_Viewer

	m_vidPlayer[0] = new GUIMediaVideo(&m_ctl_Viewer, &Recto, ".\\Media\\capture.avi");
	m_vidPlayer[1] = new GUIMediaVideo(&m_ctl_Viewer, &Recto, ".\\Media\\maillage.avi");
    /************************************************/

	//Result + button icons////////////////////////////////////////////
	m_hBitmap_Ok = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP_OK));
	m_hBitmap_Ko = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP_KO));
	((CStatic*)(GetDlgItem(IDC_STATIC_VIEWER)))->ModifyStyle(0,SS_CENTERIMAGE| SS_BITMAP);

	m_hBitmap_Close = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_CLOSE_BMP));
	m_hBitmap_DisabledClose = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_DISABLED_CLOSE));

	m_hBitmap_EraseBd = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_ERDB_BMP));
	m_hBitmap_DisabledEraseBd = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_DESABLED_ERDB));

	m_hBitmap_NewUser = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_NEWUSER_BMP));
	m_hBitmap_DisabledNewUser = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_DISABLED_NEWUSER));
	m_hBitmap_IdentifyMode = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_IDENTIFY_MODE_BMP));

	m_hBitmap_ScLevel = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_SecLvl));
	m_hBitmap_DisabledScLevel = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_DESABLED_SCLEVEL));

	m_hBitmap_Enroll = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_ENROLL_BMP));;
	m_hBitmap_StopEnroll = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_STOP_ENROLL_BMP));;

	m_hBitmap_EmptyImg = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_EMPTY_BMP));;
	m_hBitmap_Config = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_CONFIG_BMP));;
	
	 //////////////////////////////////////////////////////////

	//Init of button state
	m_x_btnState.m_b_enrollBtnEnabled= FALSE;
	m_x_btnState.m_b_closeBtnEnabled= TRUE;
	m_x_btnState.m_b_destroyBtnEnabled= TRUE;
	m_x_btnState.m_b_securityLvlBtnEnabled= TRUE;
	m_x_btnState.m_b_newUserBtnEnabled= TRUE;

	//---------------------------------------------
	// Start Identify Task
	//---------------------------------------------

	m_ppx_Threads[ID_THREAD_IDENTIFY] = AfxBeginThread( 
				TaskIdentify,	//AFX_THREADPROC pfnThreadProc, 
				(LPVOID)this	//LPVOID pParam, 
			);	

	return TRUE;  // return TRUE  unless you set the focus to a control

}

/////////////////////////////////////////////////////////////////////////////
void CMSO_FVP_LoginDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

/////////////////////////////////////////////////////////////////////////////
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMSO_FVP_LoginDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
			if (m_b_Expand)
			{	
				l_i_LastImageIndex = 3;
			}
			else
			{
				l_i_LastImageIndex = 0;
			}

			//-------------------------------
			// Refresh Async Images Viewer 
			//-------------------------------	
				
				/*l_p_Cdc = m_ctl_Viewer.GetDC();
				m_ctl_Viewer.GetClientRect(&myRect);
				m_px_Image->RefreshViewer(l_i_LastImageIndex,myRect,l_p_Cdc); 
				ReleaseDC(l_p_Cdc);	*/
				
				if (m_b_Expand)
				{
					l_p_Cdc = m_ctl_MiniViewer1.GetDC();
					m_ctl_MiniViewer1.GetClientRect(&myRect);
					m_px_Image->RefreshViewer(0,myRect,l_p_Cdc);  
					ReleaseDC(l_p_Cdc);	
					
					l_p_Cdc = m_ctl_MiniViewer2.GetDC();
					m_ctl_MiniViewer2.GetClientRect(&myRect);
					m_px_Image->RefreshViewer(1,myRect,l_p_Cdc); 
					ReleaseDC(l_p_Cdc);	

					l_p_Cdc = m_ctl_MiniViewer3.GetDC();
					m_ctl_MiniViewer3.GetClientRect(&myRect);
					m_px_Image->RefreshViewer(2,myRect,l_p_Cdc); 
					ReleaseDC(l_p_Cdc);	
				}
		}
		CDialog::OnPaint();

		m_vidPlayer[0]->RepaintVideo();
		m_vidPlayer[1]->RepaintVideo();
	}
}

/////////////////////////////////////////////////////////////////////////////
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMSO_FVP_LoginDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
/////////////////////////////////////////////////////////////////////////////
bool CMSO_FVP_LoginDlg::T_Identify()
{
	bool	l_b_Ret = FALSE;
	I		l_i_Ret;
	UL		l_ul_CallbackMask;
	CString	l_cs_Message;
	CString l_cs_Interface;
	CString l_cs_MsoName;
	UL		l_ul_TimeOut;
	S		l_s_PortCom;
	I		l_i_BaudRate;

	C_MORPHO_User	User;
	C_MORPHO_Device		l_x_MsoDevice;

	m_b_ReadyToExit = FALSE;
	
	//----------------------------------------------------------
	// Check if there is at least one record before starting
	//----------------------------------------------------------

	PostMessage(WM_DISABLE_ERASE_BASE_BUTTON,0,0);
	
	if(GetNumberOfRecords()==-1)
	{
		// m_x_Database does not exist
		CreateDatabase();
	}

	if(GetNumberOfRecords()==0)
	{
		CResultDlg		ResultDlg;
		ResultDlg.m_cs_MainMsg.Format("Database Empty");
		ResultDlg.m_cs_DetailedMsg.Format("You must enroll at least\none person before\nstarting.");
		ResultDlg.DoModal();
		StartEnrollment(); // Switch to enroll mode
	}

	l_ul_CallbackMask = MORPHO_CALLBACK_IMAGE_CMD|MORPHO_CALLBACK_COMMAND_CMD|MORPHO_CALLBACK_CODEQUALITY|MORPHO_CALLBACK_DETECTQUALITY;

	if(m_px_Image != NULL)
	{
		delete(m_px_Image);
	}	
	m_px_Image = new C_MORPHO_Image(1);
	m_i_CurrentViewerIndex = 0;

	while(1)
	{
		while(m_b_Expand)
		{
			Sleep(1); // Enroll Mode ... -> Standby
		}

		if(m_b_ExitThread) 
		{
			m_b_ReadyToExit = TRUE;
			return FALSE;
		}
		ShowAnimation("image_Empty");
		l_i_Ret = m_x_Database.Identify (	
								BIOMETRIC_TIMEOUT,					//	US							i_us_Timeout, 
								(T_MORPHO_FAR) BIOMETRIC_THRESHOLD,	//	T_MORPHO_FAR				i_us_FAR,
								l_ul_CallbackMask,					//	UL							i_ul_CallbackStatus,
								BiometricCallback,					//	T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
								this, 								//	PVOID						i_pv_CallbackArgument,
								User								//	C_MORPHO_User &				o_x_User
								);
		 
		PostMessage(WM_DISABLE_ERASE_BASE_BUTTON,0,0);

		if(m_b_ExitThread) 
		{
			m_b_ReadyToExit = TRUE;
			return FALSE;
		}

		if(l_i_Ret != MORPHO_OK)
		{	
			CResultDlg		ResultDlg;
			switch(l_i_Ret)
			{			
				case MORPHOERR_CMDE_ABORTED:
					if(m_b_AskedForBaseRemoving)
					{
						m_b_ReadyToRemove = TRUE;
						do
						{
							Sleep(1);
						} while(m_b_AskedForBaseRemoving);	
					}
					else if (m_b_ExitThread)
					{
						m_b_ReadyToExit = TRUE;
						return FALSE;
					}
					else if (m_b_securityLevelChanged)
					{
						m_b_securityLevelChanged = FALSE;
					}
					else
					{
						StartEnrollment();
					}
					break;
				
					#ifndef	APP_MODE_1
				case MORPHOERR_NO_HIT :
					    ShowAnimation("image_Ko");
						Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400); // "error" beep
						m_cs_CommandMsg.Format("NOT IDENTIFIED !!!");
						PostMessage(WM_UPDATE_DATA_FALSE,0,0);
						m_b_ReadyToExit = TRUE;
						Sleep(2500);
						m_b_ReadyToExit = FALSE;
						m_cs_CommandMsg.Format("PLACE YOUR FINGER");
						PostMessage(WM_UPDATE_DATA_FALSE,0,0);
						break;
					#endif

				case MORPHOERR_DB_EMPTY :
					m_b_ReadyToExit = TRUE;
					ResultDlg.m_cs_MainMsg.Format("Database Empty");
					ResultDlg.m_cs_DetailedMsg.Format("You must enroll at least\none person.");
					ResultDlg.DoModal();
					m_b_ReadyToExit = FALSE;
					StartEnrollment();
					break;

				case MORPHOERR_FFD :
						Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400); // "error" beep
						m_cs_CommandMsg.Format("FALSE FINGER DETECTED !!!");
						PostMessage(WM_UPDATE_DATA_FALSE,0,0);
						Sleep(2500);
						m_cs_CommandMsg.Format("PLACE YOUR FINGER");
						PostMessage(WM_UPDATE_DATA_FALSE,0,0);
						break;
				case MORPHOERR_MOIST_FINGER :
						Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400); // "error" beep
						m_cs_CommandMsg.Format("FINGER TOO MOIST !!!");
						PostMessage(WM_UPDATE_DATA_FALSE,0,0);
						Sleep(2500);
						m_cs_CommandMsg.Format("PLACE YOUR FINGER");
						PostMessage(WM_UPDATE_DATA_FALSE,0,0);
						break;
				case MORPHOERR_MOVED_FINGER:
					Beep(BEEP_FREQUENCY - 300, BEEP_DURATION + 400); // "error" beep
					m_cs_CommandMsg.Format("FINGER MOVED !!!");
					PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
					Sleep(2500);
					m_cs_CommandMsg.Format("PLACE YOUR FINGER");
					PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
					break;
				case MORPHOERR_SATURATED_FINGER:
					Beep(BEEP_FREQUENCY - 300, BEEP_DURATION + 400); // "error" beep
					m_cs_CommandMsg.Format("FINGER SATURATED !!!");
					PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
					Sleep(2500);
					m_cs_CommandMsg.Format("PLACE YOUR FINGER");
					PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
					break;
				case MORPHOERR_INVALID_FINGER:
					Beep(BEEP_FREQUENCY - 300, BEEP_DURATION + 400); // "error" beep
					m_cs_CommandMsg.Format("INVALID FINGER !!!");
					PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
					Sleep(2500);
					m_cs_CommandMsg.Format("PLACE YOUR FINGER");
					PostMessage(WM_UPDATE_DATA_FALSE, 0, 0);
					break;
				case MORPHOERR_PROTOCOLE :
						Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400); // "error" beep
						m_cs_CommandMsg.Format("PROTOCOL ERROR !!!");
						m_cs_CommandMsg2.Format("Please check the MSO is correctly plugged");
						PostMessage(WM_UPDATE_DATA_FALSE,0,0);

						// Exit application is possible
						m_b_ReadyToExit = TRUE;

						l_cs_Interface = APP->GetProfileString(COM_PARAMETERS, COM_INTERFACE, DEFAULT_COM_INTERFACE);
						
						if(l_cs_Interface.Compare("USB") == 0)
						{
							l_cs_MsoName = APP->GetProfileString(COM_PARAMETERS, SERIALNUMBER, DEFAULT_SERIALNUMBER); 
							l_ul_TimeOut = APP->GetProfileInt( COM_PARAMETERS, TIMEOUT,DEFAULT_TIMEOUT );
							
							// Wait for plugging MSO USB
							while(l_i_Ret != MORPHO_OK)
							{
								l_i_Ret = m_x_MorphoSmart.OpenUsbDevice ( 
										(PC)(LPCTSTR)l_cs_MsoName,	//PC								i_pc_MsoName,
										l_ul_TimeOut,				//UL								i_ul_TimeOut,
										NULL						//PVOID								io_pv_param
										);
							}
						}
						else
						{
							l_i_BaudRate = APP->GetProfileInt( COM_PARAMETERS, BAUD_RATE,DEFAULT_BAUD_RATE );
							l_s_PortCom = APP->GetProfileInt( COM_PARAMETERS, PORT_NUMBER, DEFAULT_COM );
							
							// Wait for plugging MSO USB
							while(l_i_Ret != MORPHO_OK)
							{
								l_i_Ret = m_x_MorphoSmart.OpenDevice(l_s_PortCom,l_i_BaudRate);
							}
						}
						PostMessage(WM_UPDATE_DATA_FALSE,0,0); 
						break;
				case MORPHOERR_FVP_FINGER_MISPLACED_OR_WITHDRAWN :
						Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400);
						m_b_ReadyToExit = TRUE;
						ResultDlg.m_cs_MainMsg.Format("Identification Failed");
						ResultDlg.m_cs_DetailedMsg.Format("Finger is misplaced or has been withdrawn form sensor during acquisition.");
						ResultDlg.DoModal();
						m_b_ReadyToExit = FALSE;
						break;
				default:
					ShowAnimation("image_Ko");
					Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400); // "error" beep
					ResultDlg.m_cs_MainMsg.Format("Identification Failed");	
					if(l_i_Ret != MORPHOERR_NO_HIT)
					{
						ResultDlg.m_cs_DetailedMsg.Format(ConvertError(l_i_Ret));
						ResultDlg.DoModal();
					//	ResultDlg.ContinueModal
						
						m_b_ReadyToExit = TRUE;
					}
					else
					{
						ResultDlg.m_cs_DetailedMsg.Format("You are not Enrolled.\nWould you like\nto enroll now ?");
						ResultDlg.ActivateYesNoButtons();
						int l_i_tmp;
						l_i_tmp = ResultDlg.DoModal();
						if(l_i_tmp == IDOK)//(ResultDlg.DoModal()==IDOK)
						{
							StartEnrollment();
						}						
					}
			}
		}
		else
		{
			UL	l_ul_LenField = 25;
			PUC	l_puc_DataField;
			UC	l_puc_GenericBuf[25];

			ShowAnimation("image_Ok");

			l_i_Ret = User.GetField(	
							0,					//I			i_i_FieldIndex,
							l_ul_LenField,		//UL			o_ul_LenField,
							l_puc_DataField		//PUC			o_puc_DataField	
							);

			memcpy( l_puc_GenericBuf, l_puc_DataField, l_ul_LenField);
			l_puc_GenericBuf[l_ul_LenField] = 0;
			
			Beep(BEEP_FREQUENCY,BEEP_DURATION+200);
			m_cs_CommandMsg.Format("WELCOME %s !", l_puc_GenericBuf);
			PostMessage(WM_UPDATE_DATA_FALSE,0,0);
			m_b_ReadyToExit = TRUE;
			Sleep(2500);
			m_b_ReadyToExit = FALSE;
			m_cs_CommandMsg.Format("PLACE YOUR FINGER");
			PostMessage(WM_UPDATE_DATA_FALSE,0,0);
		}
		ShowAnimation("image_Empty");
		PostMessage(WM_ENABLE_ERASE_BASE_BUTTON,0,0);
	}

	return l_b_Ret;
}

/////////////////////////////////////////////////////////////////////////////
void CMSO_FVP_LoginDlg::OnOK() 
{
	if( m_x_btnState.m_b_enrollBtnEnabled)
		OnStnClickedEnrollBtn(); 
	else if (!m_b_Expand)
		CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
void CMSO_FVP_LoginDlg::OnChangeEditName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	UpdateData();
	m_b_userNameIsEmpty = TRUE;

	if(m_cs_EditName.Compare("") == 0)
	{
		m_b_userNameIsEmpty = TRUE;
	}
	else
	{
		m_b_userNameIsEmpty = FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
bool CMSO_FVP_LoginDlg::T_Enroll()
{
	//------------------------------------------------------------
	// This function executed in a new thread, started when the
	// user clicks on the "enroll" button (see : OnButtonEnroll())
	//------------------------------------------------------------
	bool	l_b_Ret = FALSE;
	I		l_i_Ret;
	UL		l_ul_CallbackMask;
	CString	l_cs_Message;
	CString l_cs_TempString;


	C_MORPHO_User	User;
	CResultDlg		ResultDlg;
	UL				l_ul_lenBuf;
	UC				l_uc_lenBuf;
	UC				l_puc_Buf[25];

	PostMessage(WM_DISABLE_CLOSE,0,0);
	PostMessage(WM_DISABLE_SECURITY_LEVEL,0,0);
	C_MORPHO_TemplateList  		l_x_Template;
	T_MORPHO_IMAGE				l_px_Image;
	PT_MORPHO_MOC_PARAMETERS	l_px_MocParameters=NULL;
	UC							l_uc_PkFpQuality;
	UL							l_ul_DetectModeChoice;

	//------------------------------------------------------------
	// Associate the User Object with the m_x_Database Object
	//------------------------------------------------------------

		//- - - - - - - - - -
		// m_cs_EditName is a CString object associated with the 
		// Edit control
		//- - - - - - - - - -
		l_uc_lenBuf = m_cs_EditName.GetLength();		
		memcpy(l_puc_Buf, m_cs_EditName.GetBuffer(l_uc_lenBuf),l_uc_lenBuf);
		l_puc_Buf[l_uc_lenBuf++] = 0;

		l_i_Ret = m_x_Database.GetUser(	
								l_uc_lenBuf,	// UC			i_uc_lenUserID,
								l_puc_Buf,		// PUC			i_puc_UserID,
								User			// C_MORPHO_User & o_x_User 
								);

		if(l_i_Ret != MORPHO_OK)
		{
			if (!m_b_ExitThread)
				ShowErrorBox("An error occured while calling C_MORPHO_Database::GetUser() function", l_i_Ret);
			l_b_Ret = FALSE;
		}
	
	//------------------------------------------------------------
	// Additionnal Fields : To be compatible with other demos
	//------------------------------------------------------------

			l_cs_TempString.Format("<None>");
			l_ul_lenBuf = l_cs_TempString.GetLength();
			memcpy(l_puc_Buf, l_cs_TempString.GetBuffer(l_ul_lenBuf), l_ul_lenBuf);
			l_puc_Buf[l_ul_lenBuf++] = 0;
		
		//- - - - - - - - - -
		// Firstname
		//- - - - - - - - - -
		
			l_i_Ret = User.PutField(	
								1,						// UL		i_ul_indexField,
								l_ul_lenBuf,			// UL		i_ul_lenField,
								l_puc_Buf				// PUC		i_puc_dataField);
								);

		//- - - - - - - - - -
		// Lastname
		//- - - - - - - - - -

			if(l_i_Ret == MORPHO_OK)
			{	
				l_i_Ret = User.PutField(	
								2,						// UL		i_ul_indexField,
								l_ul_lenBuf,			// UL		i_ul_lenField,
								l_puc_Buf				// PUC		i_puc_dataField);
								);						// I		i_i_indexField,
			}

		//- - - - - - - - - -
		// Error cases
		//- - - - - - - - - -

			if(l_i_Ret != MORPHO_OK)
			{
				if (!m_b_ExitThread)
					ShowErrorBox("An error occured while calling C_MORPHO_User::PutField() function", l_i_Ret);
				l_b_Ret = FALSE;
			}

	//------------------------------------------------------------
	// Enroll
	//------------------------------------------------------------

		l_ul_CallbackMask = MORPHO_CALLBACK_ENROLLMENT_CMD|MORPHO_CALLBACK_IMAGE_CMD|MORPHO_CALLBACK_COMMAND_CMD|MORPHO_CALLBACK_CODEQUALITY|MORPHO_CALLBACK_DETECTQUALITY;
		if(m_px_Image != NULL)
		{
			delete(m_px_Image);
		}
		
		m_px_Image = new C_MORPHO_Image(3);

		m_b_EnrollmentProcessing = TRUE;


		l_ul_DetectModeChoice = MORPHO_ENROLL_DETECT_MODE;

		if(!m_bl_isCBMV)
		{
			l_ul_DetectModeChoice|= MORPHO_FORCE_FINGER_ON_TOP_DETECT_MODE;
		}


		l_i_Ret = User.Enroll(
					BIOMETRIC_TIMEOUT,						// US							i_us_Timeout, 
					0,										//UC							i_uc_AcquisitionThreshold
					0,										//UC							i_uc_AdvancedSecurityLevelsRequired
					MORPHO_NO_COMPRESS,                     // T_MORPHO_COMPRESS_ALGO		i_x_CompressAlgo,
					0,
					0,										// UC                           i_uc_CompressRate,
					0,										//UC							i_uc_ExportMinutiae
					1,										// UC							i_uc_FingerNumber,
					MORPHO_PK_COMP,							// T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType,
					MORPHO_NO_PK_FVP,                       //T_MORPHO_FVP_TYPE_TEMPLATE	i_x_FVPTemplateType,
					1,										//UC							i_uc_SaveRecord,
					l_ul_CallbackMask,						// UL							i_ul_CallbackCmd,
					BiometricCallback,						// T_MORPHO_CALLBACK_FUNCTION	i_pf_Callback,
					this,									// PVOID						i_pv_CallbackArgument
					MORPHO_MSO_V9_CODER,					//I							    i_i_CoderChoice
					l_ul_DetectModeChoice,				//UL         					i_ul_DetectModeChoice,
					l_x_Template,                           //C_MORPHO_TemplateList &		o_x_Template,
					&l_px_Image,                            // T_MORPHO_IMAGE				*o_px_Image,
					l_px_MocParameters                      //PT_MORPHO_MOC_PARAMETERS	i_px_MocParameters
				    );


		if (l_i_Ret == MORPHO_OK)
		{
				UC l_uc_dataIndex;
				T_MORPHO_TYPE_TEMPLATE      l_x_TypeTemplate;	
				l_x_TypeTemplate = MORPHO_PK_COMP;
				PUC							l_puc_DataTemplate;
				UL							l_ul_LenTemplate;

				l_i_Ret = l_x_Template.GetTemplate(	
												0,					// UC	i_uc_indexTemplate,
												l_x_TypeTemplate,	// T_MORPHO_TYPE_TEMPLATE &	o_uc_typTemplate,
												l_ul_LenTemplate,	// UL	&o_ul_lenTemplate,
												l_puc_DataTemplate,  // PUC	&o_puc_dataTemplate
												l_uc_PkFpQuality,
												l_uc_dataIndex
												);

				if(l_i_Ret != MORPHO_OK)
				{
					ShowErrorBox("An error occured while calling C_MORPHO_TemplateList::GetTemplate() function", l_i_Ret);
					//PostMessage(WM_REMOVE_PROCESS_PAGE,0,(LPARAM)&(m_BioOpPSheet.m_CodePPage));
					return FALSE;
				}
					
				
		}


		m_b_EnrollmentProcessing = FALSE;

		if( l_i_Ret != MORPHO_OK)
		{	
			switch(l_i_Ret)
			{			
				case MORPHOERR_CMDE_ABORTED:

					if(m_b_ExitThread)
					{
						ExpandDialog();
						PostMessage(WM_ENABLE_CLOSE,0,0);
						l_b_Ret = FALSE;
					}
					else
					{
						m_cs_CommandMsg.Format("");
						//m_cs_Viewer.Format("Acquisition Stopped");

					//-------------------------------------
					// "Identify mode" view
					//-------------------------------------
						//ExpandDialog();	
						//GetDlgItem(IDC_BUTTON_EXPAND)->SetWindowText("Enroll New User ... >>");
						//PostMessage(WM_ENABLE_CLOSE,0,0);
						l_b_Ret = FALSE;
					}
					break;

				case MORPHOERR_FFD :
						Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400); // "error" beep
						ResultDlg.m_cs_MainMsg.Format("False finger detected !!!");
						ResultDlg.m_cs_DetailedMsg.Format(ConvertError(l_i_Ret));
						ResultDlg.DoModal();
						break;
				case MORPHOERR_MOIST_FINGER :
						Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400); // "error" beep
						ResultDlg.m_cs_MainMsg.Format("Finger too moist !!!");
						ResultDlg.m_cs_DetailedMsg.Format(ConvertError(l_i_Ret));
						ResultDlg.DoModal();
						break;
				case MORPHOERR_MOVED_FINGER:
					Beep(BEEP_FREQUENCY - 300, BEEP_DURATION + 400); // "error" beep
					ResultDlg.m_cs_MainMsg.Format("Finger moved !!!");
					ResultDlg.m_cs_DetailedMsg.Format(ConvertError(l_i_Ret));
					ResultDlg.DoModal();
					break;
				case MORPHOERR_SATURATED_FINGER:
					Beep(BEEP_FREQUENCY - 300, BEEP_DURATION + 400); // "error" beep
					ResultDlg.m_cs_MainMsg.Format("Finger saturated !!!");
					ResultDlg.m_cs_DetailedMsg.Format(ConvertError(l_i_Ret));
					ResultDlg.DoModal();
					break;
				case MORPHOERR_INVALID_FINGER:
					Beep(BEEP_FREQUENCY - 300, BEEP_DURATION + 400); // "error" beep
					ResultDlg.m_cs_MainMsg.Format("Invalid finger !!!");
					ResultDlg.m_cs_DetailedMsg.Format(ConvertError(l_i_Ret));
					ResultDlg.DoModal();
					break;
				default:								
					Beep(BEEP_FREQUENCY-300,BEEP_DURATION+400); // "error" beep
					ShowAnimation("image_Ko");
					ResultDlg.m_cs_MainMsg.Format("Enrollment Failed");
					ResultDlg.m_cs_DetailedMsg.Format(ConvertError(l_i_Ret));
					ResultDlg.DoModal();
			}
		}
		else
		{
			m_cs_CaptureMsg3.Format("\nCapture 3/3");
			m_ctl_CaptureIcon3.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadIcon(IDI_ICON_OK) );
			ShowAnimation("image_Ok");
			PostMessage(WM_UPDATE_DATA_FALSE,0,0);

			ResultDlg.m_cs_MainMsg.Format("Enrollment Done");
			ResultDlg.m_cs_DetailedMsg.Format("You're now enrolled !");
			ResultDlg.m_cs_QualityScore.Format("Quality Score: %u  ",l_uc_PkFpQuality);
			ResultDlg.ChangeIcon(IDI_ASTERISK);
			ResultDlg.DoModal();
			l_b_Ret = TRUE;
		}
		
		m_cs_EditName.Format("");
		m_b_userNameIsEmpty = TRUE;
		m_cs_CaptureMsg1.Format("");
		m_cs_CaptureMsg2.Format("");
		m_cs_CaptureMsg3.Format("");
		
		m_ctl_CaptureIcon1.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadIcon(IDI_ICON_NONE) );						
		m_ctl_CaptureIcon2.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadIcon(IDI_ICON_NONE) );	
		m_ctl_CaptureIcon3.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadIcon(IDI_ICON_NONE) );	

		HideAnimation();
		//initialiser les minis viewers en cas d'erreur
		if( l_i_Ret != MORPHO_OK)
		{	
			InitMiniViewers();
		}

		m_ctrl_Enroll.SetBitmap(m_hBitmap_Enroll);
		
		PostMessage(WM_UPDATE_DATA_FALSE,0,0);
		PostMessage(WM_PAINT,0,0);
		
		#ifdef APP_MODE_1
			ExpandDialog();
		#endif

	/*if(m_px_Image != NULL)
	{
		delete(m_px_Image);
	}	
	m_px_Image = new C_MORPHO_Image(1);*/
	m_i_CurrentViewerIndex = 0;

	PostMessage(WM_ENABLE_CLOSE,0,0);
	PostMessage(WM_ENABLE_SECURITY_LEVEL,0,0);

	return l_b_Ret;
}

/////////////////////////////////////////////////////////////////////////////
void CMSO_FVP_LoginDlg::OnClose() 
{		
	if(m_b_EnrollmentProcessing)
	{
		OnStnClickedEnrollBtn();
		return;
	}

	m_b_ExitThread = TRUE;

	m_cs_CommandMsg.Format("Exiting ...");
	UpdateData(FALSE);	

	if(m_b_Expand) 
	{	
		if (!m_b_EnrollmentProcessing)
		{
			ExpandDialog();
		}		
	}
	else
	{		
			m_x_MorphoSmart.CancelLiveAcquisition();
	}

	while(!m_b_ReadyToExit) 
	{
		Sleep(1);
	}

	if(m_px_Image != NULL)
	{
		delete(m_px_Image);
	}
	OnOK();
 	CDialog::OnClose();
}

//////////////////////////////////////////////////////////////////
void CMSO_FVP_LoginDlg::OnCancel() 
{
	if(m_b_EnrollmentProcessing) 
	{
		OnStnClickedEnrollBtn();
		return;
	}

	OnClose();
	CDialog::OnCancel();
}
/////////////////////////////////////////////////////////////////////////////
void CMSO_FVP_LoginDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	// TODO: Add your message handler code here	
}

/////////////////////////////////////////////////////////////////////////////
bool CMSO_FVP_LoginDlg::ConvertError(int i_i_Error, CString &o_cs_Message)
{
	return ConvertSDKError(i_i_Error, o_cs_Message);
}

/////////////////////////////////////////////////////////////////////////////
bool CMSO_FVP_LoginDlg::StartEnrollment()
{
		m_cs_CommandMsg.Format("");
		//m_cs_Viewer.Format("Acquisition Stopped");

	//-------------------------------------
	// "Enroll mode" view
	//-------------------------------------
		ExpandDialog();	
		m_ctrl_addUser.SetBitmap(m_hBitmap_IdentifyMode);


		//GetDlgItem(IDC_BUTTON_ERASE_BASE)->EnableWindow( FALSE );
		PostMessage(WM_DISABLE_ERASE_BASE_BUTTON,0,0);

	//-------------------------------------
	// Free Edit name area
	//-------------------------------------

		m_cs_EditName.Format("");
		m_b_userNameIsEmpty = TRUE;


		//m_x_btnState.m_b_enrollBtnEnabled = FALSE;
		//GetDlgItem(IDC_BUTTON_ENROLL)->EnableWindow( FALSE );

		//GetDlgItem(IDC_BUTTON_ENROLL)->SetWindowText("Enroll");
		m_ctrl_Enroll.SetBitmap(m_hBitmap_Enroll);

		m_ctl_CaptureIcon1.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadIcon(IDI_ICON_NONE) );						
		m_ctl_CaptureIcon2.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadIcon(IDI_ICON_NONE) );	
		m_ctl_CaptureIcon3.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadIcon(IDI_ICON_NONE) );	
		
		RedrawWindow();
		PostMessage(WM_UPDATE_DATA_FALSE,0,0);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
CString CMSO_FVP_LoginDlg::ConvertError(int i_i_Error)
{
	CString l_cs_Message;
	
	if (!ConvertError(i_i_Error,l_cs_Message))
		l_cs_Message.Format("");
	
	return l_cs_Message;
}

//////////////////////////////////////////////////////////////////
int CMSO_FVP_LoginDlg::GetNumberOfRecords() 
{
	UL		l_ul_nbUsedRecord;
	I		l_i_Ret;
	CString	l_cs_Message;
	
	l_i_Ret = m_x_Database.GetNbUsedRecord(l_ul_nbUsedRecord);

	if(l_i_Ret != MORPHO_OK)
	{
		if(l_i_Ret == MORPHOERR_BASE_NOT_FOUND) 
		{
			l_ul_nbUsedRecord = -1;
		}
		else
		{
			if (!m_b_ExitThread)
				ShowErrorBox("An error occured while calling C_MORPHO_Database::GetNbUsedRecord() function", l_i_Ret);
			return -2;
		}
	}

	return l_ul_nbUsedRecord;
}

//////////////////////////////////////////////////////////////////
bool CMSO_FVP_LoginDlg::CreateDatabase()
{
	UL		l_ul_fieldIndex;
	C		l_ac_fieldName[MORPHO_FIELD_NAME_LEN];
	I		l_i_Ret;
	CString	l_cs_Message;
	
	// Create a default m_x_Database with 2000 records, 2 fingers, 2 fields,
	// to be compatible with all demos.

	//--------------------------------------------------//
	// Create Fields 									//
	//--------------------------------------------------//
		//- - - - - - - - - -
		// Firstname
		//- - - - - - - - - - 
	
			strcpy_s( l_ac_fieldName, sizeof(l_ac_fieldName), "First" );
			l_i_Ret = m_x_Database.PutField(	
				MORPHO_PUBLIC_FIELD,	// T_MORPHO_FIELD_ATTRIBUTE	i_uc_fieldAttribute, 
				DB_FIELD_MAX_SIZE,		// US						i_us_fieldMaxSize,
				(PUC)l_ac_fieldName,		// UC						i_puc_fieldName[MORPHO_FIELD_NAME_LEN],
				l_ul_fieldIndex			// UL &						o_ul_fieldIndex
				);

		//- - - - - - - - - -
		// Lastname
		//- - - - - - - - - - 
			if(l_i_Ret == MORPHO_OK)
			{	
				strcpy_s( l_ac_fieldName, sizeof(l_ac_fieldName), "Last" );
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
			
			if(l_i_Ret != MORPHO_OK)
			{	
				if (!m_b_ExitThread)
					ShowErrorBox("An error occured while calling C_MORPHO_Database::PutField() function", l_i_Ret);
			}

	//--------------------------------------------------//
	// Create Base 										//
	//--------------------------------------------------//
		
			l_i_Ret = m_x_Database.DbCreate(	
					DB_NB_RECORDS,		// UL							i_ul_NbRecord,
					DB_NB_FINGERS,		// UC							i_uc_NbFinger	
					MORPHO_PK_COMP		// T_MORPHO_TYPE_TEMPLATE		i_x_TemplateType
					);

			if(l_i_Ret != MORPHO_OK)
			{					
				if (!m_b_ExitThread)
					ShowErrorBox("An error occured while calling C_MORPHO_Database::CreateDataBase() function", l_i_Ret);
				return FALSE;
			}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
void CMSO_FVP_LoginDlg::ShowErrorBox(CString i_cs_Message, I i_i_Error)
{
	CString l_cs_ErrorText;
	CString l_cs_BoxMessage;

	//------------------------------------------------------------
	// Get string associated with the error code
	// Construct the message
	//------------------------------------------------------------
	ConvertSDKError(i_i_Error, l_cs_ErrorText);

	l_cs_BoxMessage.Format("%s\n%d\t: %s\n",i_cs_Message,i_i_Error, l_cs_ErrorText);

	l_cs_ErrorText.Format("%d\t: Internal error",  m_x_MorphoSmart.GetInternalError());
	l_cs_BoxMessage += l_cs_ErrorText;

	AfxMessageBox(l_cs_BoxMessage,MB_ICONERROR);
}

//////////////////////////////////////////////////////////////////
void CMSO_FVP_LoginDlg::GetWsqValue()
{
	CString l_cs_ProductDescriptor(m_x_MorphoSmart.m_puc_ProductDescriptor);
	CString l_cs_resToken;
	int p=0;
	l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n",p);
	while(l_cs_resToken !="")
	{
		l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n",p);
		if(l_cs_resToken== "WSQ")
		{
			break;
		}
		printf("l_cs_resToken %s ",l_cs_resToken);
	}
	if(l_cs_resToken !="")
	{
		l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n",p);
		l_cs_resToken.Trim(" \t \n \r");
    
		if(l_cs_resToken.CompareNoCase("Enable")==0)
		{
			m_bl_WSQ = true;
		}
		else
		{
			m_bl_WSQ = false;
		}
	}
	else
	{
		m_bl_WSQ = false;
	}

	return;
}

void CMSO_FVP_LoginDlg::CheckIfCBMV()
{
	CString l_cs_ProductDescriptor(m_x_MorphoSmart.m_puc_ProductDescriptor);
	CString l_cs_resToken;
	int p = 0;

	m_bl_isCBMV = false;

	while (p != -1)
	{
		l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n",p);
		l_cs_resToken.Trim(" \t \n \r");
		if (l_cs_resToken == "CBM-V") 
		{		
			m_bl_isCBMV = true;
			break;
		}
	}	
}

void CMSO_FVP_LoginDlg::GetFVP_Status()
{
	CString l_cs_ProductDescriptor(m_x_MorphoSmart.m_puc_ProductDescriptor);
	CString l_cs_resToken;
	int p = 0;
 
	m_bl_FVP = false;

	while (p != -1)
	{
		l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n",p);
		l_cs_resToken.Trim(" \t \n \r");
		if ((l_cs_resToken == "FVP") || (l_cs_resToken == "FINGER")) // since FVP 1.5 firmware release, the product name is "FINGER VP"
		{
			m_bl_FVP = true;
			break;
		}
	}

	return ;
}

//////////////////////////////////////////////////////////////////
void CMSO_FVP_LoginDlg::GetBaseBioValue()
{
	CString l_cs_ProductDescriptor(m_x_MorphoSmart.m_puc_ProductDescriptor);
	CString l_cs_resToken;
	int p=0;
	l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n",p);
	while(l_cs_resToken !="")
	{
		l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n",p);
		if(l_cs_resToken== "BaseBio")
		{
			break;
		}
		printf("l_cs_resToken %s ",l_cs_resToken);
	}
	if(l_cs_resToken !="")
	{
		l_cs_resToken = l_cs_ProductDescriptor.Tokenize(" : \n",p);
		l_cs_resToken.Trim(" \t \n \r");
    
		if(l_cs_resToken.CompareNoCase("Enable")==0)
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
		m_bl_BaseBio = false;
	}

	return;
}
// dialog background color
BOOL CMSO_FVP_LoginDlg::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);
    CBrush myBrush(RGB(0, 0, 0));    // dialog background color
    CBrush *pOld = pDC->SelectObject(&myBrush);
    BOOL bRes = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOld);    // restore old brush
    return bRes;                       
}


HBRUSH CMSO_FVP_LoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	pDC->SetTextColor(RGB(255,255,255));
	pDC->SetBkColor(RGB(4,4,4));	

	return m_brush;
}


void CMSO_FVP_LoginDlg::OnStnClickedCloseBtn()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	if(!m_x_btnState.m_b_closeBtnEnabled)
	{
		return;
	}

	OnClose();
}

void CMSO_FVP_LoginDlg::OnStnClickedErdbBtn()
{
	I		l_i_Ret=0;
	CString l_cs_Message;
	


	if(!m_x_btnState.m_b_destroyBtnEnabled)
	{
		return;
	}

	m_b_ReadyToRemove = FALSE;
	m_b_AskedForBaseRemoving = TRUE;
	m_cs_CommandMsg.Format("Please Wait...");
	UpdateData(FALSE);

	m_x_MorphoSmart.CancelLiveAcquisition();

	while(!m_b_ReadyToRemove)
	{
		Sleep(1);
	}

	m_cs_CommandMsg.Format("");
	UpdateData(FALSE);

    CResultDlg ResultDlg; 
	ResultDlg.m_cs_MainMsg.Format("Erase Base");
	ResultDlg.m_cs_DetailedMsg.Format("This operation is going\nto erase the whole Database.\nConfirm ?");
	ResultDlg.ActivateYesNoButtons();
	if(ResultDlg.DoModal()==IDOK)
	{
		CResultDlg ResultDlg1; 
		//--------------------------------------------------//
		// Remove all users from the m_x_Database			//
		//--------------------------------------------------//
		
		l_i_Ret	= m_x_Database.DbDelete(
				C_MORPHO_Database::MORPHO_ERASE_BASE // T_MORPHO_TYPE_DELETION		i_uc_DeletionType 
								); 

			if(l_i_Ret != MORPHO_OK)
			{
				if (!m_b_ExitThread)
				{
					ShowErrorBox("An error occured while calling C_MORPHO_Database::DbDelete() function", l_i_Ret);
				}
			}
			
		//--------------------------------------------------//
			
			ResultDlg1.m_cs_MainMsg.Format("Erase Base");
			ResultDlg1.m_cs_DetailedMsg.Format("Database Empty");
			ResultDlg1.ChangeIcon(IDI_EXCLAMATION);
			ResultDlg1.DeactivateYesNoButtons();
			ResultDlg1.DoModal();
	}
	
	m_cs_CommandMsg.Format("PLACE YOUR FINGER");
	HideAnimation();

	UpdateData(FALSE);
	m_b_AskedForBaseRemoving = FALSE;
}

void CMSO_FVP_LoginDlg::OnStnClickedNewuserBtn()
{	
	if(!m_x_btnState.m_b_newUserBtnEnabled)
	{
		return;
	}

	Beep(BEEP_FREQUENCY,BEEP_DURATION);

	//-------------------------------------
	// - If we are in the "identify mode"  (=> m_b_Expand = FALSE)
	// view, stop acquisition. The Abort
	// command in the Identify thread will
	// call ExpandDialog() itself.
	// - If we are in the "enroll mode" 
	// view, and if the enrollment is processing, (=> m_b_EnrollmentProcessing = TRUE)
	//	stop acquisition. The Abort
	// command in the Enroll thread will
	// call ExpandDialog() itself.
	//-------------------------------------		
		if((!m_b_Expand)||(m_b_EnrollmentProcessing))
		{
			if(m_b_EnrollmentProcessing)
			{
				switch(m_i_CurrentViewerIndex)
				{
					case 0:
						m_cs_CaptureMsg1.Format("Command aborted\nby user");
						m_ctl_CaptureIcon1.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadStandardIcon(IDI_EXCLAMATION ) );	
						break;
					case 1: 
						m_cs_CaptureMsg2.Format("Command aborted\nby user");
						m_ctl_CaptureIcon2.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadStandardIcon(IDI_EXCLAMATION ) );	
						break;
					case 2:
						m_cs_CaptureMsg3.Format("Command aborted\nby user");
						m_ctl_CaptureIcon3.SetIcon( ((CMSO_FVP_LoginApp*)AfxGetApp())->LoadStandardIcon(IDI_EXCLAMATION ) );	
						break;
				}
			}

			m_cs_CommandMsg.Format("Please Wait ...");
			UpdateData(FALSE);
			m_x_MorphoSmart.CancelLiveAcquisition();
			HideAnimation();
		}

	//-------------------------------------
	// Otherwise, call ExpandDialog( ) now
	// to go back into the Identify mode
	//-------------------------------------	
		else
		{		
			UpdateData(FALSE);
			ExpandDialog();	
			m_ctrl_addUser.SetBitmap(m_hBitmap_NewUser);
		}
}

void CMSO_FVP_LoginDlg::OnStnClickedEnrollBtn()
{

	/*if(!m_x_btnState.m_b_enrollBtnEnabled)
		return;*/

	if(m_b_EnrollmentProcessing)
	{
		OnStnClickedNewuserBtn();
		m_ctrl_Enroll.SetBitmap(m_hBitmap_Enroll);
	}
	else
	{
		if(m_b_userNameIsEmpty)
		{
			return;
		}

		m_ctrl_Enroll.SetBitmap(m_hBitmap_StopEnroll);
		RedrawWindow();
		AfxBeginThread( 
					TaskEnroll,		//AFX_THREADPROC pfnThreadProc, 
					(LPVOID)this	//LPVOID pParam, 
					);	
	}
}
void CMSO_FVP_LoginDlg::InitQualityBar()
{
    m_qualiteProgress.SetPos(0);
    m_qualiteProgress.SendMessage(PBM_SETBARCOLOR, 0, RGB(255,255,0)); 
}
void CMSO_FVP_LoginDlg::ShowAnimation(CString p_st_typeAnim)
{
	//CDC	*l_p_Cdc;
	CRect	myRect;
	if (p_st_typeAnim== "maillage")		
	{
		/*GetDlgItem(IDC_STATIC_VIEWER)->ShowWindow(SW_HIDE);
	    if (m_AnimationPic.Load(_T("maillage.gif")))
                m_AnimationPic.Draw();
		m_AnimationPic.SetFont(&m_fFont);*/
		GetDlgItem(IDC_STATIC_VIEWER)->ShowWindow(SW_SHOW);
		m_vidPlayer[0]->Stop();
		m_vidPlayer[1]->Play(true);
	}
	else if (p_st_typeAnim== "capture")
	{
		GetDlgItem(IDC_STATIC_VIEWER)->ShowWindow(SW_SHOW);
		m_vidPlayer[0]->Play(true);
		m_vidPlayer[1]->Stop();
	}
	else if (p_st_typeAnim== "image_Ok")
	{
		GetDlgItem(IDC_STATIC_VIEWER)->ShowWindow(SW_SHOW);
		m_vidPlayer[0]->Stop();
		m_vidPlayer[1]->Stop();
		m_ctl_Viewer.SetBitmap( m_hBitmap_Ok );
		GetDlgItem(IDC_STATIC_VIEWER)->ShowWindow(SW_SHOW);			
	}
	else if (p_st_typeAnim== "image_Ko")
	{
		GetDlgItem(IDC_STATIC_VIEWER)->ShowWindow(SW_SHOW);
		m_vidPlayer[0]->Stop();
		m_vidPlayer[1]->Stop();
		m_ctl_Viewer.SetBitmap( m_hBitmap_Ko );
	}	
	else if (p_st_typeAnim== "image_Config")
	{
		GetDlgItem(IDC_STATIC_VIEWER)->ShowWindow(SW_SHOW);
		m_vidPlayer[0]->Stop();
		m_vidPlayer[1]->Stop();
		m_ctl_Viewer.SetBitmap( m_hBitmap_Config );
	}
	else if (p_st_typeAnim== "image_Empty")
	{
		GetDlgItem(IDC_STATIC_VIEWER)->ShowWindow(SW_SHOW);
		m_ctl_Viewer.SetBitmap( m_hBitmap_EmptyImg );
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

void CMSO_FVP_LoginDlg::HideAnimation()
{
	GetDlgItem(IDC_STATIC_VIEWER)->ShowWindow(SW_SHOW);
	m_vidPlayer[0]->Stop();
	m_vidPlayer[1]->Stop();
	RedrawWindow();
}

void CMSO_FVP_LoginDlg::InitMiniViewers()
{
	m_ctl_MiniViewer1.ShowWindow(SW_HIDE);
	m_ctl_MiniViewer2.ShowWindow(SW_HIDE);
	m_ctl_MiniViewer3.ShowWindow(SW_HIDE);

	m_ctl_MiniViewer1.ShowWindow(SW_SHOW);
	m_ctl_MiniViewer2.ShowWindow(SW_SHOW);
	m_ctl_MiniViewer3.ShowWindow(SW_SHOW);
}



void CMSO_FVP_LoginDlg::OnStnClickedSeclvlBtn()
{
	//if button desabled
	if(!m_x_btnState.m_b_securityLvlBtnEnabled)
	{
		return;
	}
	
	m_ppx_Threads[ID_THREAD_IDENTIFY]->SuspendThread();
	
	ShowAnimation("image_Config");
	CSecurityLvlDlg secLvlDlg;	

	//Retreive current value
	m_x_MorphoSmart.Get_FFD_SecurityLevel(&secLvlDlg.m_i_SecurityLevel);


	secLvlDlg.SetFVPStatus(m_bl_FVP);
	

	//User may change or not the securityLevel 
	secLvlDlg.DoModal();	
	m_ppx_Threads[ID_THREAD_IDENTIFY]->ResumeThread();
	 
	//Setting the new value(old value if secLvlDlg canceled)
    m_x_MorphoSmart.Set_FFD_SecurityLevel(secLvlDlg.m_i_SecurityLevel);

	//to init the main viewer in enrollement mode
	HideAnimation();

	//In verification mode :Identify is canceled ,it'll be restarted automatically and the main viewer will show the animation
	m_b_securityLevelChanged = TRUE;
	m_x_MorphoSmart.CancelLiveAcquisition();
}


LRESULT CMSO_FVP_LoginDlg::OnNcHitTest(CPoint point)
{
	int xPos = point.x;
	int yPos = point.y;
	UINT nHitTest = CDialog::OnNcHitTest(CSize(xPos, yPos));
	return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;
}
