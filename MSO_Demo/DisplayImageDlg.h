/************************************************************
  Copyright (c) 2002-2005, SAGEM SA
************************************************************/

// DisplayImageDlg.h : header file
//
#if !defined(AFX_DISPLAYIMAGE_H__BF86DD1B_1776_4971_9910_E158814B30CD__INCLUDED_)
#define AFX_DISPLAYIMAGE_H__BF86DD1B_1776_4971_9910_E158814B30CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDisplayImageDlg dialog

class CDisplayImageDlg : public CDialog
{
// Construction
public:
	CDisplayImageDlg(T_MORPHO_IMAGE	*o_px_Image, CWnd* pParent = NULL);   // standard constructor
	~CDisplayImageDlg();


// Dialog Data
	//{{AFX_DATA(CDisplayImageDlg)
	enum { IDD = IDD_IMAGE };
	CButton	m_ctrl_Ok;
	CStatic	m_ctrl_Image;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplayImageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	C_MORPHO_Image	*m_px_CImage;

	LONG m_l_OKPos;
	
	T_MORPHO_IMAGE	*m_px_TImage;

	// Generated message map functions
	//{{AFX_MSG(CDisplayImageDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYIMAGE_H__BF86DD1B_1776_4971_9910_E158814B30CD__INCLUDED_)
