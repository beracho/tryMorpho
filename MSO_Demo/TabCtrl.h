#pragma once
#include "TabDbInfo.h"
#include "TabBioSettings.h"
#include "TabOptions.h"

// Boîte de dialogue TabCtrl

class TabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(TabCtrl)

public:
	TabCtrl(CWnd* pParent = NULL);   // constructeur standard

	virtual ~TabCtrl();


	//Array to hold the list of dialog boxes/tab pages for CTabCtr
	int m_DialogID[3];

	//CDialog Array Variable to hold the dialogs 
	CDialog *m_Dialog[3];
	TabDbInfo *m_TabDbInfoDlg;
	TabBioSettings *m_TabBioSettingsDlg;
	TabOptions *m_TabOptionsDlg;

	//Function to Create the dialog boxes during startup
	void InitDialogs();

	//Function to activate the tab dialog boxes 
	void ActivateTabDialogs();

	int m_nPageCount;
 






// Données de boîte de dialogue
	enum { IDD = IDD_MSO_DEMO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTabctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonStart();
};
