#pragma once
#include "afxwin.h"

// CPPageMoc dialog

class CPPageMoc : public CPropertyPage
{
	DECLARE_DYNAMIC(CPPageMoc)

public:
	CPPageMoc();
	virtual ~CPPageMoc();

// Dialog Data
	enum { IDD = IDD_PPAGE_MOC };
	CComboBox	m_ctrl_TemplateType;
	CString		m_cs_TemplateType;
	CComboBox	m_ctrl_CardType;
	CString		m_cs_CardType;
	CString		m_cs_D0, m_cs_D1, m_cs_D2, m_cs_D3;
	int			m_i_CardType;
	int			m_i_CaptureType;
	int			m_i_TemplateActionType;
	int			m_i_FingerUsed;
	int			m_i_TemplateMaxSize;
	int			m_i_ApduMaxLength;
	BOOL		m_b_LatentDetect;
	BOOL		m_b_Diversification;
	BOOL		m_b_FullCCID;
	BOOL		m_b_GetInfo;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	afx_msg void OnBnClickedRadioStoreOnCard();
	afx_msg void OnBnClickedRadioMatchOnCard();
	afx_msg void OnSelChangeCardType();
	DECLARE_MESSAGE_MAP()

public:
	void SelectHand();

	afx_msg void OnBnClickedFullCcid();
	afx_msg void OnBnClickedGetInfo();

};
