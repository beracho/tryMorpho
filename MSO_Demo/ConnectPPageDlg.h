#if !defined(AFX_CONNECTPPAGEDLG_H__4063661F_8F46_4D69_82A6_62F8E7092C1F__INCLUDED_)
#define AFX_CONNECTPPAGEDLG_H__4063661F_8F46_4D69_82A6_62F8E7092C1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnectPPageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConnectPPageDlg dialog

class CConnectPPageDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CConnectPPageDlg)

// Construction
public:
	CConnectPPageDlg();
	~CConnectPPageDlg();

// Dialog Data
	//{{AFX_DATA(CConnectPPageDlg)
	enum { IDD = IDD_CONNECTION_PSHEET_PIPE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConnectPPageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConnectPPageDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTPPAGEDLG_H__4063661F_8F46_4D69_82A6_62F8E7092C1F__INCLUDED_)
