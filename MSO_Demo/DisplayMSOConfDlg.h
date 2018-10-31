/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

#if !defined(AFX_DISPLAYMSOCONFDLG_H__B2B8F681_C982_4A93_A137_EEDAC489B962__INCLUDED_)
#define AFX_DISPLAYMSOCONFDLG_H__B2B8F681_C982_4A93_A137_EEDAC489B962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisplayMSOConfDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisplayMSOConfDlg dialog

class CDisplayMSOConfDlg : public CDialog
{
// Construction
public:
	CDisplayMSOConfDlg(PC i_str_serialNumber, UC i_uc_SecuOption, US i_us_FAR,  UL i_uL_MSL, bool i_b_isFVP, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDisplayMSOConfDlg)
	enum { IDD = IDD_MSO_CONF };
	CListCtrl	m_list;
	CString	m_FAR;
	CString	m_serialNum;
	CString m_cs_Min_MSL;
	bool	m_isFVP;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplayMSOConfDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList	m_imageList;
	I			m_i_index;
	UC			m_uc_secuConf;
	// Generated message map functions
	//{{AFX_MSG(CDisplayMSOConfDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYMSOCONFDLG_H__B2B8F681_C982_4A93_A137_EEDAC489B962__INCLUDED_)
