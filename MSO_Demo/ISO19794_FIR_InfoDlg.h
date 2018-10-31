#pragma once
#include "MORPHO_Types.h"


// Boîte de dialogue ISO19794_FIR_InfoDlg

class ISO19794_FIR_InfoDlg : public CDialog
{
	DECLARE_DYNAMIC(ISO19794_FIR_InfoDlg)

public:
	ISO19794_FIR_InfoDlg(CWnd* pParent = NULL);   // constructeur standard
	virtual ~ISO19794_FIR_InfoDlg();

// Données de boîte de dialogue
	enum { IDD = IDD_MSO_ISO19794_FIR_INFO };
	CComboBox				m_cmd_DesiredCompression;
	CString					m_cs_DesiredCompression;
	bool					m_b_UpdateCtr;
	T_MORPHO_COMPRESS_ALGO	m_x_DesiredCompression;
	CString					m_cs_ResX;
	CString					m_cs_ResY;
	CString					m_cs_NBCols;
	CString					m_cs_NBRows;
	CString					m_cs_NbBitsPerPixel;
	UL						m_ul_ImageBufferSize;
	
protected:
	void FitComboBox(CComboBox & i_px_ComboBox);
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboDesiredCompress();
	bool m_b_ShowDesiredCompressComoBox;
	CToolTipCtrl m_ToolTipCtrl;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
