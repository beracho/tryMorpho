// TabDbInfo.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "TabDbInfo.h"


// Boîte de dialogue TabDbInfo

IMPLEMENT_DYNAMIC(TabDbInfo, CDialog)

TabDbInfo::TabDbInfo(CWnd* pParent /*=NULL*/)
	: CDialog(TabDbInfo::IDD, pParent)
	, m_cs_MaxNbRecords(_T(""))
	, m_CurrentNbRecords(_T(""))
	, m_NbFingers(_T(""))
	, m_cs_encryptDB(_T(""))
{

}

TabDbInfo::~TabDbInfo()
{
}

void TabDbInfo::SetParam(CString p_cs_paramName, CString p_cs_paramValue)
{


	if( p_cs_paramName.Compare( "MaxNbRecords" ) == 0 )  
	{
		m_cs_MaxNbRecords = p_cs_paramValue;
	}

	if( p_cs_paramName.Compare( "CurrentNbRecords" ) == 0 ) 
	{
		m_CurrentNbRecords = p_cs_paramValue;
	}

    if( p_cs_paramName.Compare( "NbFingers" ) == 0 ) 
	{
		m_NbFingers = p_cs_paramValue;
	}

	if( p_cs_paramName.Compare( "EncryptedDB" ) == 0 ) 
	{
		m_cs_encryptDB = p_cs_paramValue;
	}

    if(this->m_hWnd && this->IsWindowEnabled())
	{
		UpdateData(0);
		RedrawWindow();
	}
	return;
}

void TabDbInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAX_NB_RECORDS, m_cs_MaxNbRecords);
	DDX_Text(pDX, IDC_CURRENT_NB_RECORDS, m_CurrentNbRecords);
	DDX_Text(pDX, IDC_NB_FINGERS, m_NbFingers);
	DDX_Text(pDX, IDC_VAL_ECRYP_DB, m_cs_encryptDB);
}


