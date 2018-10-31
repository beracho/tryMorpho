// DiversificationData.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "DiversificationData.h"


// CDiversificationData dialog

IMPLEMENT_DYNAMIC(CDiversificationData, CDialog)

CDiversificationData::CDiversificationData(CWnd* pParent /*=NULL*/)
	: CDialog(CDiversificationData::IDD, pParent)
	, m_cs_DiversificationData(_T(""))
{

}

CDiversificationData::~CDiversificationData()
{
}

void CDiversificationData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DIVERSIFICATION_DATA, m_cs_DiversificationData);
}


BEGIN_MESSAGE_MAP(CDiversificationData, CDialog)
	ON_BN_CLICKED(IDOK, &CDiversificationData::OnBnClickedOk)
END_MESSAGE_MAP()


// CDiversificationData message handlers

void CDiversificationData::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
