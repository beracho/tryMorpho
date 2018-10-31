#pragma once
#include "afxwin.h"
#include "MORPHO_Interface.h"


// Boîte de dialogue TabOptions

class TabOptions : public CDialog
{
	DECLARE_DYNAMIC(TabOptions)

public:
	TabOptions(CWnd* pParent = NULL);   // constructeur standard

	virtual ~TabOptions();

// Données de boîte de dialogue
	enum { IDD = IDD_TAB_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
    CToolTipCtrl m_ToolTips;

	CButton m_cmd_MainViewerCheckBox;
	BOOL m_b_MainViewerActive;

	CButton m_cmd_MiniViewerCheckBox;
	BOOL m_b_MiniViewerActive;

	BOOL m_b_AsyncCommand;
	BOOL* m_pb_AsyncCommand;

	BOOL m_b_AsyncEnrollCommand;
	BOOL* m_pb_AsyncEnrollCommand;

	BOOL m_b_AsyncDetectQuality;
	BOOL* m_pb_AsyncDetectQuality;

	BOOL m_b_AsyncCodeQuality;
	BOOL* m_pb_AsyncCodeQuality;

	BOOL m_b_Export_Num_Pk;
	BOOL m_b_ConfigKeyUser;

    CButton m_cmd_WakeUpLedOff;
	BOOL m_b_WakeUpLedOff;
	BOOL* m_pb_WakeUpLedOff;

	CWnd* m_pcw_Mso_DemoDlg; 
	(unsigned long)* m_pul_CallbackMask;
	(unsigned long)* m_pul_WakeUpMode;
	
	
	afx_msg void OnCheckMainViewerActive();
	afx_msg void OnCheckMiniViewerActive();
	afx_msg void OnCheckAsyncCommand();
	afx_msg void OnCheckEnrollCommand();
	afx_msg void OnCheckAsyncDetectQuality();
	afx_msg void OnCheckAsyncCodeQuality();
	afx_msg void OnCheckExportNumPk();
	afx_msg void OnCheckConfigKeyUser();
	afx_msg void OnCheckWakeUpMode();
	void InitConfParams();

	C_MORPHO_Device*		m_x_pMorphoSmart;
	CComboBox m_ctrl_defaultBaudRate;
	CString m_cs_defaultBaudRate;
	CEdit m_ctrl_sleepTimeout;
	int m_i_sleepTimeout;
	CComboBox m_ctrl_sensorWinPosition;
	CString m_cs_sensorWinPosition;
	afx_msg void OnCbnSelchangeConfigDefaultBaudRate();
	afx_msg void OnCbnSelchangeConfigSensorWinPosition();
	afx_msg void OnEnKillfocusConfigSleepTimeout();
};
