/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// ppagegetimage.cpp : implementation file
//

#include "stdafx.h"
#include "mso_demo.h"
#include "PPageGetImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPageGetImage property page

IMPLEMENT_DYNCREATE(CPPageGetImage, CPropertyPage)

CPPageGetImage::CPPageGetImage() : CPropertyPage(CPPageGetImage::IDD)
{
	//{{AFX_DATA_INIT(CPPageGetImage)
	m_i_DetectModeChoice = 1;
	m_b_GetImageLatentDetect = FALSE;
	m_str_CompressionType = _T("RAW");
	m_i_CompressionType = 0;
	m_i_CompressionRate = 10;
	//}}AFX_DATA_INIT
}

CPPageGetImage::~CPPageGetImage()
{
}

void CPPageGetImage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPageGetImage)
	DDX_Radio(pDX, IDC_RADIO_GET_IMAGE_VERIF_DETECT_MODE_CHOICE, m_i_DetectModeChoice);
	DDX_Check(pDX, IDC_GET_IMAGE_LATENT, m_b_GetImageLatentDetect);
	DDX_Control(pDX, IDC_COMPRESSION_TYPE, m_cmd_CompressionType);
	DDX_CBString(pDX, IDC_COMPRESSION_TYPE, m_str_CompressionType);
	DDX_Text(pDX, IDC_COMPRESSION_RATE, m_i_CompressionRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPageGetImage, CPropertyPage)
	//{{AFX_MSG_MAP(CPPageGetImage)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMPRESSION_TYPE, &CPPageGetImage::OnCbnSelchangeCompressionType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPageGetImage message handlers

void CPPageGetImage::OnCbnSelchangeCompressionType()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	UpdateData(TRUE);
    GetDlgItem(IDC_COMPRESSION_RATE)->EnableWindow( FALSE );
	GetDlgItem(IDC_TEXT_COMPRESSION_RATE)->EnableWindow( FALSE );
	m_cmd_CompressionType.GetLBText(m_cmd_CompressionType.GetCurSel(), m_str_CompressionType);

	if(strcmp(m_str_CompressionType, "RAW") == 0)//The default compression is choice by the MorphoSmart™ (the choice is No compression)
	{
		m_i_CompressionType = 0 ;
		m_i_CompressionRate = 0;
		
	}
	else if(strcmp(m_str_CompressionType, "SAGEM_V1") == 0)
	{
		m_i_CompressionType = 1;
		m_i_CompressionRate = 0;
	}
	else if(strcmp(m_str_CompressionType, "WSQ") == 0)
	{
		m_i_CompressionType = 2 ;
		GetDlgItem(IDC_COMPRESSION_RATE)->EnableWindow( TRUE );
		GetDlgItem(IDC_TEXT_COMPRESSION_RATE)->EnableWindow( TRUE );
	}
	else if(strcmp(m_str_CompressionType, STR_ISO19794_FIR_RAW) == 0)
	{
		m_i_CompressionType = 3;
		m_i_CompressionRate = 0;
	}
	else if(strcmp(m_str_CompressionType, STR_ISO19794_FIR_BMP) == 0)
	{
		m_i_CompressionType = 4;
		m_i_CompressionRate = 0;
	}
	else if(strcmp(m_str_CompressionType, STR_ISO19794_FIR_WSQ) == 0)
	{
		m_i_CompressionType = 8;
		m_i_CompressionRate = 0;
	}
	else if(strcmp(m_str_CompressionType, STR_ISO19794_FIR_JPEG) == 0)
	{
		m_i_CompressionType = 6;
		m_i_CompressionRate = 0;
	}
	else if(strcmp(m_str_CompressionType, STR_ISO19794_FIR_JPEG2000) == 0)
	{
		m_i_CompressionType = 5;
		m_i_CompressionRate = 0;
	}
	else if(strcmp(m_str_CompressionType, STR_ISO19794_FIR_PNG) == 0)
	{
		m_i_CompressionType = 7;
		m_i_CompressionRate = 0;
	}
	else
	{
		m_i_CompressionType = 0;
	}
}



