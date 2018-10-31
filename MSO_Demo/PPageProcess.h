/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

#include "afxwin.h"

#if !defined(AFX_PPAGEPROCESS_H__54C6A7C5_91C3_4710_AD6E_3988D02B038B__INCLUDED_)
#define AFX_PPAGEPROCESS_H__54C6A7C5_91C3_4710_AD6E_3988D02B038B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPageProcess.h : header file
//
#include "FingerPrintCtrl.h"

class CMSO_DemoDlg;



/////////////////////////////////////////////////////////////////////////////
// CPPageProcess dialog

class CPPageProcess : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPageProcess)

// Construction
public:
	CPPageProcess();
	CPPageProcess(CMSO_DemoDlg* pDemoDlg);
	~CPPageProcess();

// Dialog Data
	//{{AFX_DATA(CPPageProcess)
	enum { IDD = IDD_PPAGE_PROCESS };
	CFingerPrintCtrl	m_ctl_MiniViewer_2_3;
	CFingerPrintCtrl	m_ctl_MiniViewer_2_2;
	CFingerPrintCtrl	m_ctl_MiniViewer_2_1;
	CFingerPrintCtrl	m_ctl_MiniViewer_1_3;
	CFingerPrintCtrl	m_ctl_MiniViewer_1_2;
	CFingerPrintCtrl	m_ctl_MiniViewer_1_1;
	//}}AFX_DATA
	int m_i_ViewerToColorIdx;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPageProcess)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPageProcess)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CMSO_DemoDlg* m_pDemoDlg;

public:
	afx_msg void OnStnClickedStaticMiniViewer22();
	afx_msg void OnStnClickedStaticMiniViewer12();
public:
	afx_msg void OnStnClickedStaticMiniViewer13();

public:
	void drawRectangle();
	void setColor(int i_i_color);
public:
	CStatic m_ctl_AsyncCmdMsg;
public:
	CString m_cs_AsyncCmdMsg;

	int m_i_tabColor[6]; //0:red, 1:green, 2:orange
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPAGEPROCESS_H__54C6A7C5_91C3_4710_AD6E_3988D02B038B__INCLUDED_)
