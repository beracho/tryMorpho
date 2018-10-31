#pragma once


// GetInfoDlg dialog

class GetInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(GetInfoDlg)

public:
	GetInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GetInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_GET_INFO };
	CString	m_cs_Msg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
