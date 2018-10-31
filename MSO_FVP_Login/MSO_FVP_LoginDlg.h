// MSO_FVP_LoginDlg.h : header file
//

#if !defined(AFX_MSO_FVP_LoginDLG_H__FFB48421_5ADA_417A_8B72_40693ACF9A48__INCLUDED_)
#define AFX_MSO_FVP_LoginDLG_H__FFB48421_5ADA_417A_8B72_40693ACF9A48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Morpho_Interface.h"
#include "MORPHO_Types.h"
#include "MORPHO_Image.h"
#include "MORPHO_Errors.h"
#include "GUIMediaVideo.h"
#include "afxcmn.h"
#include "afxwin.h"

#define BIOMETRIC_TIMEOUT		0
#define BIOMETRIC_THRESHOLD		5

#define WM_UPDATE_DATA_FALSE			(WM_USER + 100)
#define WM_UPDATE_DATA_TRUE				(WM_USER + 101)
#define WM_ENABLE_CLOSE					(WM_USER + 102)
#define WM_DISABLE_CLOSE				(WM_USER + 103)
#define WM_ENABLE_ERASE_BASE_BUTTON		(WM_USER + 104)
#define WM_DISABLE_ERASE_BASE_BUTTON	(WM_USER + 105)
#define WM_DISABLE_SECURITY_LEVEL       (WM_USER + 106)
#define WM_ENABLE_SECURITY_LEVEL        (WM_USER + 107)

#define BEEP_FREQUENCY	400
#define BEEP_DURATION	60

#define LIVE_QUALITY_MIN				0//20
#define LIVE_QUALITY_MAX				150//130

#define	NB_THREADS			2
#define ID_THREAD_IDENTIFY	0

typedef struct _T_BUFFER
{
	US	m_us_Size;
	PUC	m_puc_Buffer;
}T_BUFFER, * PT_BUFFER;

typedef struct _T_BUTTON_STATE
{
	BOOL			m_b_closeBtnEnabled;
	BOOL			m_b_destroyBtnEnabled;
	BOOL			m_b_securityLvlBtnEnabled;
	BOOL			m_b_newUserBtnEnabled;
	BOOL			m_b_enrollBtnEnabled;
}T_BUTTON_STATE;

typedef struct _T_USER_DATA
{
	CString		m_cs_Name;	// Actually, this is the ID
	T_BUFFER	m_x_Buffer;	// One buffer for one finger
}T_USER_DATA;

/////////////////////////////////////////////////////////////////////////////
// CMSO_FVP_LoginDlg dialog

class CMSO_FVP_LoginDlg : public CDialog
{
// Construction
public:
	CMSO_FVP_LoginDlg(CWnd* pParent = NULL);	// standard constructor
	bool T_Identify();
	bool T_Enroll();
	C_MORPHO_Image*	m_px_Image;
	I	m_i_CurrentViewerIndex;
	CStatic* m_ctl_CurrentMiniViewer;
	bool m_b_Expand;
	bool StartEnrollment();
	bool  m_bl_WSQ;
	bool m_bl_FVP;

	/** @bbrief boolean that indicate if we use CBM-V device   
	 */
	bool m_bl_isCBMV;

	bool  m_bl_BaseBio;
	void  GetWsqValue();
	void  GetFVP_Status();
	void  CheckIfCBMV();
	void  GetBaseBioValue();
	LRESULT OnEnableCloseButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnDisableCloseButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnDisableEraseBaseButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnEnableEraseBaseButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnDisableSecurityLevelButton(WPARAM wParam, LPARAM lParam);
	LRESULT OnEnableSecurityLevelButton(WPARAM wParam, LPARAM lParam);
	CWinThread*		m_ppx_Threads[NB_THREADS];

// Dialog Data
	//{{AFX_DATA(CMSO_FVP_LoginDlg)
	enum { IDD = IDD_MSO_FVP_Login_DIALOG };
	CStatic	m_ctl_CommandMsg2;
	CStatic	m_ctl_MiniViewer3;
	CStatic	m_ctl_MiniViewer2;
	CStatic	m_ctl_MiniViewer1;
	CStatic	m_ctl_CaptureIcon3;
	CStatic	m_ctl_CaptureIcon2;
	CStatic	m_ctl_CaptureIcon1;
	CStatic	m_ctl_CaptureMsg3;
	CStatic	m_ctl_CaptureMsg2;
	CStatic	m_ctl_CaptureMsg1;
	CStatic	m_ctl_CommandMsg;
	CStatic	m_ctl_Viewer;
	CEdit	m_ctl_EditName;
	CStatic	m_ctl_StaticMsg1;
	CString	m_cs_EditName;
	CString	m_cs_CommandMsg;
	CString	m_cs_CommandMsg2;
	CString	m_cs_Viewer;
	CString	m_cs_CaptureMsg1;
	CString	m_cs_CaptureMsg2;
	CString	m_cs_CaptureMsg3;
	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSO_FVP_LoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
		CBrush	m_brush;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMSO_FVP_LoginDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeEditName();
	afx_msg void OnClose();
	afx_msg void OnCancelMode();
	virtual void OnOK();
	virtual void OnCancel();	
	//}}AFX_MSG
	//afx_msg HBRUSH OnCtlColor(CDC *pDC,CWnd *pWnd,UINT ref);
	DECLARE_MESSAGE_MAP()
	

	void ExpandDialog();
	LRESULT OnUpdateDataFalse(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateDataTrue(WPARAM wParam, LPARAM lParam);
	bool ConvertError(int i_i_Error, CString &o_cs_Message);
	CString ConvertError(int i_i_Error);
	int GetNumberOfRecords();
	void ShowErrorBox(CString i_cs_Message, I i_i_Error);
	
	C_MORPHO_Device		m_x_MorphoSmart;
	C_MORPHO_Database	m_x_Database;
	bool	m_b_EnrollmentProcessing;
	CFont	m_fFont;
	bool	m_b_ExitThread;
	bool CreateDatabase();
	bool m_b_ReadyToExit;
	bool m_b_AskedForBaseRemoving;
	bool m_b_ReadyToRemove;
	bool m_b_userNameIsEmpty;
	bool m_b_securityLevelChanged;

	GUIMediaVideo * m_vidPlayer[2];
	HBITMAP m_hBitmap_Ok;
	HBITMAP m_hBitmap_Ko;

	HBITMAP m_hBitmap_Close;
	HBITMAP m_hBitmap_DisabledClose;

	HBITMAP m_hBitmap_EraseBd;
	HBITMAP m_hBitmap_DisabledEraseBd;

	HBITMAP m_hBitmap_NewUser;
	HBITMAP m_hBitmap_DisabledNewUser;
	HBITMAP m_hBitmap_IdentifyMode;

	HBITMAP m_hBitmap_ScLevel;
	HBITMAP m_hBitmap_DisabledScLevel;

	HBITMAP m_hBitmap_Enroll;
	HBITMAP m_hBitmap_StopEnroll;

	HBITMAP m_hBitmap_EmptyImg;
	HBITMAP m_hBitmap_Config;




private:
	T_BUTTON_STATE m_x_btnState;// = {TRUE, TRUE, TRUE, TRUE, TRUE};


public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedCloseBtn();
	afx_msg void OnStnClickedErdbBtn();
	afx_msg void OnStnClickedNewuserBtn();
	afx_msg void OnStnClickedEnrollBtn();
	CProgressCtrl m_qualiteProgress;
	void InitQualityBar();
	void ShowAnimation(CString p_st_typeAnim);
	void HideAnimation();
	void InitMiniViewers();
	void InitCurrentMiniViewer();
	CStatic m_ctrl_Close;
	afx_msg void OnStnClickedSeclvlBtn();
	CStatic m_ctrl_destroyBd;
	CStatic m_ctrl_SecurityLvl;
	static int m_i_value;
	CStatic m_ctrl_Enroll;
	CStatic m_ctrl_addUser;
	afx_msg LRESULT OnNcHitTest(CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSO_FVP_LoginDLG_H__FFB48421_5ADA_417A_8B72_40693ACF9A48__INCLUDED_)
