// GetInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "GetInfoDlg.h"
#include "afxdialogex.h"


// GetInfoDlg dialog

IMPLEMENT_DYNAMIC(GetInfoDlg, CDialog)

GetInfoDlg::GetInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GetInfoDlg::IDD, pParent)
{
	m_cs_Msg = _T("-");
}

GetInfoDlg::~GetInfoDlg()
{
}

void GetInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MSG, m_cs_Msg);
}


BEGIN_MESSAGE_MAP(GetInfoDlg, CDialog)
END_MESSAGE_MAP()


// GetInfoDlg message handlers
