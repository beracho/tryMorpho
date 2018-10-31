// TabCtrl.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "TabCtrl.h"
#include "TabDbInfo.h"
#include "TabBioSettings.h"
#include "TabOptions.h"
#include "MORPHO_Interface.h"

// Boîte de dialogue TabCtrl

IMPLEMENT_DYNAMIC(TabCtrl, CTabCtrl)

TabCtrl::TabCtrl(CWnd* pParent /*=NULL*/)
	: CTabCtrl()
{
   m_DialogID[0] =IDD_TAB_DB_INFO;
   m_DialogID[1] =IDD_TAB_BIO_SETTINGS;
   m_DialogID[2] =IDD_TAB_OPTIONS;

   m_TabDbInfoDlg = new TabDbInfo();
   m_Dialog[0] = m_TabDbInfoDlg;

   m_TabBioSettingsDlg = new TabBioSettings();
   m_Dialog[1] = m_TabBioSettingsDlg;

   m_TabOptionsDlg = new TabOptions();
   m_Dialog[2] = m_TabOptionsDlg;

   m_nPageCount = 3;
}

TabCtrl::~TabCtrl()
{
	if(m_TabDbInfoDlg)
	{
		delete m_TabDbInfoDlg;
	}

	if(m_TabBioSettingsDlg)
	{
		delete m_TabBioSettingsDlg;
	}

	if(m_TabOptionsDlg)
	{
		delete(m_TabOptionsDlg);
	}
}

//This function creates the Dialog boxes once
void TabCtrl::InitDialogs()
{
m_Dialog[0]->Create(m_DialogID[0],GetParent());
m_Dialog[1]->Create(m_DialogID[1],GetParent());
m_Dialog[2]->Create(m_DialogID[2],GetParent());
}

void TabCtrl::DoDataExchange(CDataExchange* pDX)
{
	CTabCtrl::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnTcnSelchangeTabctrl)
END_MESSAGE_MAP()


// Gestionnaires de messages de TabCtrl

void TabCtrl::OnTcnSelchangeTabctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	ActivateTabDialogs();
	*pResult = 0;
}

void TabCtrl::ActivateTabDialogs()
{
  int nSel = GetCurSel();
  if(m_Dialog[nSel]->m_hWnd)
     m_Dialog[nSel]->ShowWindow(SW_HIDE);

  CRect l_rectClient;
  CRect l_rectWnd;

  GetClientRect(l_rectClient);
  AdjustRect(FALSE,l_rectClient);
  GetWindowRect(l_rectWnd);
  GetParent()->ScreenToClient(l_rectWnd);
  l_rectClient.OffsetRect(l_rectWnd.left,l_rectWnd.top);
  for(int nCount=0; nCount < m_nPageCount; nCount++){
     m_Dialog[nCount]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_HIDEWINDOW);
  }
  m_Dialog[nSel]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_SHOWWINDOW);

  m_Dialog[nSel]->ShowWindow(SW_SHOW); 


  m_Dialog[nSel]->UpdateData(0);
  m_Dialog[nSel]->RedrawWindow();


}

