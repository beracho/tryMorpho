#pragma once
#include "afxwin.h"

#ifdef _WIN32_WCE
#error "CDHtmlDialog n'est pas pris en charge pour Windows CE."
#endif 

// Boîte de dialogue CSecurityLvlDlg

#define SECURITY_LEVEL_CRITICAL		0x400
#define SECURITY_LEVEL_HIGH			0x100
#define SECURITY_LEVEL_MEDIUM 		0x200
#define SECURITY_LEVEL_STANDARD		0x0000
#define SECURITY_LEVEL_DEFAULT		SECURITY_LEVEL_STANDARD

// MSO
/** @brief Low False Finger Detection security level */
#define FFD_SECURITY_LEVEL_LOW_HOST                       0x0000
/** @brief Medium False Finger Detection security level */
#define FFD_SECURITY_LEVEL_MEDIUM_HOST					  0x0001
/** @brief High False Finger Detection security level */
#define FFD_SECURITY_LEVEL_HIGH_HOST                      0x0002
/** @brief This level allows disabling FFD functionality */
#define FFD_SECURITY_LEVEL_NONE_HOST                      0x0003
/** @brief Critical False Finger Detection security level */
#define FFD_SECURITY_LEVEL_CRITICAL_HOST                  0x0004

/** @brief Default False Finger Detection security level */
#define FFD_SECURITY_LEVEL_DEFAULT_HOST					  FFD_SECURITY_LEVEL_LOW_HOST



class CSecurityLvlDlg : public CDialog
{
//	DECLARE_DYNCREATE(CSecurityLvlDlg)

public:
	CSecurityLvlDlg(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CSecurityLvlDlg();
// Substitutions


// Données de boîte de dialogue
	enum { IDD = IDD_SECURITY_LEVEL_DIALOG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	virtual BOOL OnInitDialog();
	BOOL m_bl_FvpStatus;

	DECLARE_MESSAGE_MAP()	
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush	m_brush;
	afx_msg void OnBnClickedOk();

	CComboBox m_ctrl_SecurityLevel;
	int m_i_SecurityLevel;
	int m_i_CancelSecurityLevel;
	afx_msg void OnCbnSelchangeComboSecurityLevel();
	afx_msg void OnBnClickedCancel();

public:
	void SetFVPStatus(BOOL i_bl_FVP);

};
