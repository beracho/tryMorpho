#pragma once


// CDiversificationData dialog

class CDiversificationData : public CDialog
{
	DECLARE_DYNAMIC(CDiversificationData)

public:
	CDiversificationData(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDiversificationData();

// Dialog Data
	enum { IDD = IDD_DIALOG_BIO_DATA_ENCRYPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_cs_DiversificationData;
};
