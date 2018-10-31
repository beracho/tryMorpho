// DBaseCreateOption.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "DBaseCreateOption.h"


// DBaseCreateOption dialog

IMPLEMENT_DYNAMIC(DBaseCreateOption, CDialog)

DBaseCreateOption::DBaseCreateOption(CWnd* pParent /*=NULL*/)
	: CDialog(DBaseCreateOption::IDD, pParent)
	, m_b_dataEncryption(0)
{


}

DBaseCreateOption::~DBaseCreateOption()
{
}

BOOL DBaseCreateOption::OnInitDialog()
{
	m_i_max_record = 500;
	m_i_finger_number = 2;

	CEdit* l_px_edit = reinterpret_cast<CEdit *>(GetDlgItem(IDC_MAX_RECORD));
	l_px_edit->SetWindowText("500");
	l_px_edit = reinterpret_cast<CEdit *>(GetDlgItem(IDC_NUM_FINGER));
	l_px_edit->SetWindowText("2");
	m_b_dataEncryption = 0;
	CButton* l_px_Button = reinterpret_cast<CButton *>(GetDlgItem(IDC_RADIO_NO));
	l_px_Button->SetCheck(true);
	return TRUE;
}

void DBaseCreateOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_MAX_RECORD, m_i_max_record);
	DDX_Text(pDX,IDC_NUM_FINGER, m_i_finger_number);
	DDX_Radio(pDX, IDC_RADIO_NO, m_b_dataEncryption);
}


BEGIN_MESSAGE_MAP(DBaseCreateOption, CDialog)
	ON_BN_CLICKED(IDOK, &DBaseCreateOption::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DBaseCreateOption::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_NO, &DBaseCreateOption::OnBnClickedRadioNo)
	ON_BN_CLICKED(IDC_RADIO_YES, &DBaseCreateOption::OnBnClickedRadioYes)
END_MESSAGE_MAP()


// DBaseCreateOption message handlers

void DBaseCreateOption::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//
	CString l_cs_text;
	CEdit* l_px_edit = reinterpret_cast<CEdit *>(GetDlgItem(IDC_MAX_RECORD));
	l_px_edit->GetWindowText(l_cs_text);
	sscanf_s((char*)l_cs_text.GetBuffer(l_cs_text.GetLength()),"%d",&m_i_max_record);	
	l_px_edit = reinterpret_cast<CEdit *>(GetDlgItem(IDC_NUM_FINGER));
	l_px_edit->GetWindowText(l_cs_text);
	sscanf_s((char*)l_cs_text.GetBuffer(l_cs_text.GetLength()),"%d",&m_i_finger_number);
	
	OnOK();
}

void DBaseCreateOption::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


void DBaseCreateOption::OnBnClickedRadioNo()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle	
}

void DBaseCreateOption::OnBnClickedRadioYes()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}
