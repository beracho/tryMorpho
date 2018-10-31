#pragma once
#include "afxwin.h"
#include "MORPHO_Interface.h"
// Boîte de dialogue TabBioSettings

class TabBioSettings : public CDialog
{
	DECLARE_DYNAMIC(TabBioSettings)

public:
	TabBioSettings(CWnd* pParent = NULL);   // constructeur standard
	
	virtual ~TabBioSettings();	


// Données de boîte de dialogue
	enum { IDD = IDD_TAB_BIO_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	int m_i_Threshold;
	int m_i_Timeout;
	int	m_i_CoderChoice;
	int m_i_MatchingStrategy;
	CEdit m_i_EditFingerPrintQuality;
	bool m_bl_FVP;
	bool m_bl_isCBMV;
	unsigned char m_uc_StrategyAcquisitionMode;
	
	virtual BOOL OnInitDialog();
	BOOL OnToolTipNotify(UINT i_ui_id, NMHDR *i_PNMHDR_Nmhdr, LRESULT *o_PLRESULT_Result);


	C_MORPHO_Device*		m_px_pMorphoSmart;
	CString m_str_CoderChoice;
	CComboBox m_cmd_CoderChoice;
	afx_msg void OnSelchangeCoderChoice();
	CComboBox m_cmd_FFDSecuLevel;
	afx_msg void OnSelchangeFfdSecuLevel();
	CString m_str_FFDSecuLevel;
	afx_msg void OnSelchangeBioMode();
	CString m_str_BioMode;
	CComboBox m_cmd_BioMode;

	afx_msg void OnEnChangeEditTimeout();
	afx_msg void OnEnChangeEditThreshold();
	BOOL m_cmd_ForceFingerPlacement;
	BOOL m_cmd_ASLC;
	BOOL m_cmd_FingerPrint_Quality;
	afx_msg void OnBnClickedCheckFingerprintQuality();
	CString m_str_StrategyAcquisitionMode;
	afx_msg void OnCbnSelchangeStrategyAcquisitionMode();
	afx_msg void OnBnClickedCheckFfplacementOnTop();
	// // FVP 1.10
	CComboBox m_cmd_StrategyAcquisitionMode;
};
