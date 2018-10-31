/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

// MSO_DemoDlg.h : header file
//

#if !defined(AFX_MSO_DEMODLG_H__BC60D23E_6ED5_4CDD_B8A0_7E94D9CE41C2__INCLUDED_)
#define AFX_MSO_DEMODLG_H__BC60D23E_6ED5_4CDD_B8A0_7E94D9CE41C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BioOperationPSheet.h"
#include "MORPHO_Interface.h"
#include "MORPHO_Types.h"
#include "MORPHO_Image.h"
#include "MORPHO_Errors.h"
#include "DisplayImageDlg.h"
#include "SCardReader.h"
#include "tabctrl.h"
#include "GUIMediaVideo.h"
#include "afxwin.h"
#include "afxcmn.h"

#define PORT_COM			1  // COM1 by default
#define	DB_FIELD_MAX_SIZE	15
#define	DB_NB_RECORDS		500
#define	DB_NB_FINGERS		2

#define WM_UPDATE_DATA_FALSE	(WM_USER + 100)
#define WM_UPDATE_DATA_TRUE		(WM_USER + 101)
#define WM_ADD_PROCESS_PAGE		(WM_USER + 102)
#define WM_REMOVE_PROCESS_PAGE	(WM_USER + 103)

#define BEEP_FREQUENCY	500
#define BEEP_DURATION	100

#define LIVE_QUALITY_MIN				0//20
#define LIVE_QUALITY_MAX				150//130

#define MAGIC_NB_DEMO		0x43464747
#define TKB_FILE_VERSION	0

#define	NB_THREADS			5

#define ID_THREAD_CAPTURE	0
#define ID_THREAD_VERIFY	1
#define ID_THREAD_GETIMAGE	2
#define ID_THREAD_ENROLL	3
#define ID_THREAD_IDENTIFY	4

#define UP  	0
#define RIGHT	1
#define DOWN	2
#define LEFT	3

#define	MAX_TEMPLATE	100

#define CMDID_BIOPAD_GET_FEATURE_INFOS			0x10000001
#define CMDID_BIOPAD_FEATURE_CAPTURE_TEMPLATE	0x10000011
#define CMDID_BIOPAD_FEATURE_TEMPLATE_TO_CARD	0x10000012

#define TAG_FORMAT_TEXT	0x2f
#define TAG_DESC_PRODUCT 0x29
#define TAG_DESC_SENSOR	0x2b
#define TAG_DESC_SOFTWARE 0x2a
class CSagemBioDemoCard;

/////////////////////////////////////////////////////////////////////////////
// CMSO_DemoDlg dialog

typedef struct _T_BUFFER
{
	US	m_us_Size;
	PUC	m_puc_Buffer;
}T_BUFFER, * PT_BUFFER;

typedef struct _T_USER_DATA
{
	CString m_cs_ID;
	CString m_cs_Firstname;
	CString m_cs_Lastname;

	UC			m_uc_NbFingers;
	T_MORPHO_TYPE_TEMPLATE		m_uc_FileType;
	T_MORPHO_FVP_TYPE_TEMPLATE  m_uc_FVPFiletype;
	PT_BUFFER	m_px_Buffer;
	PT_BUFFER	m_px_pk_X984;
	PT_BUFFER	m_px_fvp_X984;
}T_USER_DATA;

typedef struct _T_INFO_USER
{
	CString m_cs_ID;
	CString m_cs_Firstname;
	CString m_cs_Lastname;
}T_INFO_USER;

typedef struct
{
	UL			m_ul_MagicNb;
	UL			m_ul_Version;
	UL			m_ul_SizeTkb;
	UL			m_ul_SizeCertif;
}T_TKB_HDR, *PT_TKB_HDR;

class CMSO_DemoDlg : public CDialog
{
// Construction
public:

	CMSO_DemoDlg(CWnd* pParent = NULL);	// standard constructor
	CBioOperationPSheet m_BioOpPSheet;

	bool _EncryptAESData(PC l_data_to_encrypt, UL l_data_to_encryptLen, PUC *o_ppuc_Aes128CbcCryptedData, PUL	o_pul_Aes128CbcCryptedDataLen, std::string	l_encrypt_Key, UI i_random);
	bool _DecryptAESData(PUC _auc_FileBuffer, UL _ul_FileBufferSize, PUC	*o_ppuc_Aes128CbcClearData, UL	&o_pul_Aes128CbcClearDataLen, std::string	l_encrypt_Key);
	bool _PreparePrivacyData(PC l_pc_data_to_encrypt, UL l_ul_data_to_encryptLen, std::string	l_encrypt_Key, UI i_random, BOOL i_b_addStringTerminator, PUC l_encrypted_data, PUL l_encrypted_data_len);
	bool _CheckPrivacyData(PUC l_pc_data_to_check, UL l_ul_data_to_checkLen, std::string l_encrypt_Key);
	
	bool	GetEncryptedKey(std::string		&l_encrypt_Key);
	bool _Capture();
	bool _Verify();
	bool _VerifyWithFile(UL	i_ul_CallbackMask);
	bool _VerifyInBase(UL	i_ul_CallbackMask);
	I Verify(
				   C_MORPHO_TemplateList &		i_x_TemplateList,
				   UL							i_ul_CallbackMask
				   );
	bool _Enroll();
	bool _Identify();
	bool _GetImage();
	bool _MatchOnCard();
	bool _VerifMatchWithFile(); 

	bool MatchAndStoreOnCard();
	bool GetInfoCCID();

	void            InitQualityBar();
	void            InitGuidingAnimations();
	void            ShowAnimation(CString p_st_typeAnim);
	void            HideAnimation();
	void            ShowDirection(int p_i_direction);
	void            HideDirection();

	bool			EnableButtons();
	bool			DisableButtons();
	void            UpdateNumberOfRecords();

	CFingerPrintCtrl*	m_ctl_CurrentMiniViewer;
	C_MORPHO_Image*		m_px_Image;
	I					m_i_CurrentViewerIndex;
	CBitmap				m_btm_BitmapBaseStatus;
	CStatic				m_Static;
	CWinThread*			m_ppx_Threads[NB_THREADS];

// Dialog Data
	//{{AFX_DATA(CMSO_DemoDlg)
	enum { IDD = IDD_MSO_DEMO_DIALOG };
	CComboBox	m_cmd_BioMode;
	CButton	m_cmd_WakeUpLedOff;
	CComboBox	m_cmd_CoderChoice;
	CStatic	m_ctl_AsyncCmdMsg2;
	CComboBox	m_cmd_FFDSecuLevel;
	CStatic	m_ctrl_Image;
	CButton	m_cmd_DestroyBase;
	CButton	m_cmd_MiniViewerCheckBox;
	CButton	m_cmd_MainViewerCheckBox;
	CButton	m_cmd_VerifMatch;
	CButton	m_cmd_Start;
	CButton	m_cmd_RemoveUser;
	CButton	m_cmd_RemoveAll;
	CButton	m_cmd_IdentMatch;
	CButton	m_cmd_CreateBase;
	CButton	m_cmd_Close;
	CButton	m_cmd_AddUser;
	CStatic	m_ctl_BmpBaseStatus;
	CProgressCtrl	m_ctl_ProgressCtrl;
	CStatic	m_ctl_AsyncCmdMsg;
	CStatic	m_ctl_AsyncEnrollMsg;	
	CStatic	m_ctl_BitmapPosition;
	CListCtrl	m_listCtrl;
	CString	m_cs_MaxNbRecords;
	CString	m_NbFingers;
	CString	m_DbEncryption;
	CString	m_CurrentNbRecords;
	CString	m_cs_AsyncEnrollMsg;
	CString m_cs_QualityMsg;
	CString	m_cs_AsyncCmdMsg;
	BOOL	m_b_NoCheck;
	CString	m_cs_AsyncCmdMsg2;
	BOOL	m_b_WakeUpLedOff;
	BOOL	m_b_ConfigKeyUser;
	BOOL	m_b_AsyncCodeQuality;
	BOOL	m_b_AsyncCommand;
	BOOL	m_b_AsyncDetectQuality;
	BOOL	m_b_AsyncEnrollCommand;
	CString	m_cs_ImageInfo;

	/** @brief boolean that indicate if we use WSQ compress  
	 */
	bool  m_bl_WSQ;
	/** @brief boolean that indicate if we use FVP device  
	 */
	bool  m_bl_FVP;
	/** @brief boolean that indicate if we the FVP acquesition is running   
	*/
	bool  m_bl_FVP_Live_isRunning;
	/** @brief boolean that indicate if  the product MSO or FVP is STD (false) or ITO (true) 
	*/
    bool  m_bl_Product_Type;
	/** @brief boolean that indicate if we use base bio  
	 */
	bool  m_bl_BaseBio;
	/** @bbrief boolean that indicate if we use CBM-V device   
	 */
	bool m_bl_isCBMV;
	//}}AFX_DATA

	

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSO_DemoDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CToolTipCtrl m_ToolTips;
	CMenu	m_menu;
	////////////////////////////:::
	

	C_MORPHO_Device		m_x_MorphoSmart;
	C_MORPHO_Database	m_x_Database;
	CFont m_fFont;
	LRESULT OnUpdateDataFalse(WPARAM wParam, LPARAM lParam);
	LRESULT OnUpdateDataTrue(WPARAM wParam, LPARAM lParam);
	LRESULT OnAddProcessPage(WPARAM wParam, LPARAM lParam);
	LRESULT OnRemoveProcessPage(WPARAM wParam, LPARAM lParam);
	void EnableDisableSmartCardControls();
	bool IsMSOCardReader();

	bool m_b_BiometricOperationProcessing;

	// Generated message map functions
	//{{AFX_MSG(CMSO_DemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg void OnDblclkListUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRemoveUser();
	afx_msg void OnButtonRemoveAllUsers();
	afx_msg void OnButtonCreateBase();
	afx_msg void OnButtonDestroyBase();
	afx_msg void OnClickOnPsheetArea();
	afx_msg void OnButtonAddUser();
	afx_msg void OnButtonClose();
	afx_msg void OnClose();
	afx_msg void OnCancelMode();
	afx_msg void OnCheckMainViewerActive();
	afx_msg void OnCheckMiniViewerActive();
	afx_msg void OnButtonIdentMatch();
	afx_msg void OnButtonVerifMatch();
	afx_msg void OnColumnclickListUser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLoadPkcs12();
	afx_msg void OnReadCertificat();
	afx_msg void OnReadConfiguration();
	afx_msg void OnStoreCertificat();
	afx_msg void OnVerifSignX984();
	afx_msg void OnExtractSignerId();
	afx_msg void OnSelchangeFfdSecuLevel();
	afx_msg void OnCheckExportNumPk();
	afx_msg void OnCheckConfigKeyUser();
	afx_msg void OnCheckWakeUpMode();
	afx_msg void OnCheckAsyncCommand();
	afx_msg void OnCheckEnrollCommand();
	afx_msg void OnCheckAsyncDetectQuality();
	afx_msg void OnCheckAsyncCodeQuality();
	afx_msg void OnLoadMasterKey();
	afx_msg void OnButtonUpdateUser();
	afx_msg void OnSelchangeBioMode();
	afx_msg void OnUncompressImage();
	afx_msg void OnConvertISO19794_FIR();
	afx_msg void MMIFileLoad();
	afx_msg void MMIFileSaveAs();
	afx_msg void MMIFileReset();
	afx_msg void OnLoadMocKEnc();
	afx_msg void OnDisplayMocKencKcv();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool _VerifyWithSmartCardExternal(UL	i_ul_CallbackMask);
	bool _VerifyWithSmartCardInternal(UL i_ul_CallbackMask);

	I OpenSmartCardAndExtractData( T_USER_DATA *o_x_UserData, CBioDemoCard *i_px_SagemBioDemoCard );
	I OpenSmartCardAndInsertData ( T_USER_DATA *i_x_UserData, CBioDemoCard *i_px_SagemBioDemoCard, BOOL i_i_AddLength );
	I OpenSmartCard (	CBioDemoCard	*i_px_BioDemoCard );
	I CloseSmartCard (	CBioDemoCard	*i_px_BioDemoCard );

	bool CreateDatabase();
	bool DestroyDatabase();

	
	bool OpenFileAndExtractData(
								CString i_cs_FileToOpen,
								CString i_cs_FileExt, 
								T_USER_DATA *o_x_UserData);
	VOID FreeOpenFileAndExtractData(
					T_USER_DATA *o_px_UserData);
	bool GetUserList(C_MORPHO_UserList & o_x_UserList);
	bool GetIndexDescriptor(UC io_auc_FieldIndexDescriptor[MORPHO_NB_FIELD_MAX/8]);
	void InitializeListCtrl();
	bool RefreshListCtrl(std::string	_sl_encrypt_Key ="");
	bool CheckIfOneUserSelectedInList(CString &o_cs_ID);
	bool GetFilesNameToOpen(
							CString i_cs_DefaultFileName, 
							CString &o_cs_FileToOpen,
							CString &o_cs_FileExt,
							CString i_cs_FilesType,
							bool	i_b_allowMultiSelect); 
	bool GetFilesNameToOpen(
							CString i_cs_DefaultFileName,
							CString &o_cs_Folder,
							CString &o_cs_FileToOpen,
							CString &o_cs_FileExt,
							CString i_cs_FilesType,
							bool i_b_allowMultiSelect);
	bool ReadFile(CString &i_cs_FileToOpen,PUC *o_ppuc_buffer,PI o_pi_len) ;
	bool FillTemplateListFromUserStruct(T_USER_DATA & i_x_UserData, C_MORPHO_TemplateList & o_x_TemplateList,UC i_uc_Index = 0xFF, BOOL i_b_isPrivacy = FALSE,std::string i_s_privacy_key = "", I i_i_random = 0);
	void CleanAsyncMessages();
	void CleanAsyncMessages2();
	void RedrawWnd();
	bool ExtractAndStoreCertif(CString i_cs_FileToOpen);

	bool ShowUserData(
								 C_MORPHO_User & i_x_User
								 );
	bool SaveFile(PUC l_auc_FileBuffer,UL l_ul_FileBufferSize,CString i_cs_default_name,CString i_cs_filter);

	void ShowErrorBox(CString i_cs_Message, I i_i_Error);
	
	
	bool GetFilesAndExtarctTemplates(
									CString					i_cs_FilesType,
									CString					o_cs_listfile[MAX_TEMPLATE],
									T_INFO_USER				o_x_ListinfoUsers[MAX_TEMPLATE],
									I						&o_i_nb_file,
									C_MORPHO_TemplateList	&o_px_TemplateList,
									BOOL i_b_isPrivacy = FALSE, 
									std::string i_s_privacy_key = "", 
									I i_i_random = 0
									);

	bool GetFileAndExtractData(
										CString		i_cs_DefaultFileName, 										
										CString		i_cs_FilesType,
										CString		&o_cs_FileToOpen,
										CString		&o_cs_FileExt,
										T_USER_DATA *o_px_UserData);

	bool ExtractCertif(
										  CString	i_cs_FileToOpen,
										  PUL		o_pul_CertifSize,
										  PUC*		o_ppuc_Certif
										  );

	bool CMSO_DemoDlg::SortListCtrl
	( 
		int nCol, 
		bool bAscending, 
		int low = 0,
		int high = -1
	);


	void OnCheckNoCheck( void );

	PUC		m_puc_MsoCertif;
	UL		m_ul_MsoCertifSize;
	UC		m_uc_SecuConfig;
	UL		m_ul_CertifHostSize;
	PUC		m_puc_CertifHost;
	UC		m_uc_ExportNumPk;
	PUC		m_puc_ExportNumPk;	
	UL		m_ul_WakeUpMode;
	UL		m_ul_CallbackMask;
	bool	m_b_BaseAvailable;

	// Mso Interface parameters
	CString		m_cs_Interface;
	CString		m_cs_MsoName;
	UL			m_ul_TimeOut;
	S			m_s_PortCom;
	I			m_i_BaudRate;
	CString		m_cs_IpAdress;
	UL			m_ul_PortNumber;

	// Threads Stop
	B			m_b_EndThread;

	void GetFVP_Status();
	void CheckIfCBMV();
	void GetBaseBioValue();
	void GetProcduct_Type();
	void GetSecurityLevel(int i_i_PageId);
	I UnlockDevice();
public:
	TabCtrl m_tbCtrl;
	CStatic m_ctl_AnimationPic;
	HBITMAP m_hBitmap_Ok;
	HBITMAP m_hBitmap_Ko;
	HBITMAP m_hBitmap_Up;
	HBITMAP m_hBitmap_Right;
	HBITMAP m_hBitmap_Down;
	HBITMAP m_hBitmap_Left;

	GUIMediaVideo * m_vidPlayer[2];

    int     m_i_SecurityLevel;
	afx_msg void OnCbnSelchangeFfdSecuLevel();
	CProgressCtrl m_qualiteProgress;

	//CStatic m_cs_AsyncEnrollDirectionPic;
	CStatic m_ctl_AsyncEnrollDirectionPic;
	afx_msg void OnBnClickedRebootsoft();
	void InsertText(CComboBox *pCmb,CString str);
	CString m_cs_animationPic;
	void OnLoadKs();
	void OnLoadKsAsymmSec();
	void OnLoadKsSymmSec();
	void OnDisplayKsKcv();
	void OnDisplayKprivacyKcv();
	void OnEnableBioDataEncryption();
	void OnDecryptBiometricData();
	void OnEncryptAESData();
	void OnDecryptAESData();
	void OnPrivacyMode_Enable_NoOperationDB();
	void OnPrivacyMode_Enable_EraseDB();
	void OnPrivacyMode_Enable_CipherDB();
	void OnPrivacyMode_Disable_NoOperationDB();
	void OnPrivacyMode_Disable_EraseDB();
	void OnPrivacyMode_Disable_CipherDB();
	void OnPrivacyMode_PartialEnable_NoOperationDB();
	void OnPrivacyMode_PartialEnable_EraseDB();
	void OnPrivacyMode_PartialEnable_CipherDB();
	void OnLoadKprivacy();
	void OnLoadKprivacySymmSec();
	B	m_IsDataEncryption;
	B	m_IsPrivacyListClear;
	C_MORPHO_Device::T_DEVICE_PRIVACY_MODE_STATUS	m_PrivacyModeStatus;
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	B	m_ChangeCursor;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSO_DEMODLG_H__BC60D23E_6ED5_4CDD_B8A0_7E94D9CE41C2__INCLUDED_)
