/************************************************************
  Copyright (c) 2002, Morpho 
************************************************************/

#include "afxwin.h"
#if !defined(AFX_RESULTDLG_H__6C640BC4_7097_47DA_B3FF_D7ABFEE60F01__INCLUDED_)
#define AFX_RESULTDLG_H__6C640BC4_7097_47DA_B3FF_D7ABFEE60F01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResultDlg.h : header file
//

#define	COUNTER 10 // 10 seconds
#define	WM_UPDATE_COUNTER (WM_USER + 200)

/////////////////////////////////////////////////////////////////////////////
// CResultDlg dialog

class CResultDlg : public CDialog
{
// Construction
public:
	CResultDlg(CWnd* pParent = NULL);   // standard constructor
	void ActivateYesNoButtons();
	void DeactivateYesNoButtons();
	LRESULT OnUpdateCounter(WPARAM wParam, LPARAM lParam);
	void Exit();
	void ChangeIcon(LPCTSTR lpszIconName); 
	int	 m_i_Counter;

// Dialog Data
	//{{AFX_DATA(CResultDlg)
	enum { IDD = IDD_RESULT_DIALOG };
	CStatic	m_ctl_StaticCounter;
	CStatic	m_ctl_ResDlgIcon;
	CStatic	m_ctl_MainMsg;
	CStatic	m_ctl_DetailedMsg;
	CString	m_cs_DetailedMsg;
	CString	m_cs_MainMsg;
	CString m_cs_QualityScore;
	CString	m_cs_StaticCounter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResultDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CFont m_fFontBig, m_fFontSmall;
	LPCTSTR m_lpctstr_DefaultIcon;
	UINT m_nEventTimer;

private:
	bool m_b_YesNoButton;
	CBrush	m_brush;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_ctl_QualityScore;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTDLG_H__6C640BC4_7097_47DA_B3FF_D7ABFEE60F01__INCLUDED_)
