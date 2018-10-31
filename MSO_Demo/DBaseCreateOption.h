#pragma once


// DBaseCreateOption dialog

class DBaseCreateOption : public CDialog
{
	DECLARE_DYNAMIC(DBaseCreateOption)

public:
	DBaseCreateOption(CWnd* pParent = NULL);   // standard constructor
	virtual ~DBaseCreateOption();

// Dialog Data
	enum { IDD = IDD_BASE_OPTION };
    int m_i_max_record ;
	int m_i_finger_number;
	// NEMO
	int m_b_dataEncryption;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
					   

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadioNo();
	afx_msg void OnBnClickedRadioYes();

};
