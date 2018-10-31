// PPageMoc.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "PPageMoc.h"


// CPPageMoc dialog

IMPLEMENT_DYNAMIC(CPPageMoc, CPropertyPage)

CPPageMoc::CPPageMoc()
	: CPropertyPage(CPPageMoc::IDD)
	, m_b_Diversification(FALSE)
{
	m_cs_TemplateType = _T("");
	m_cs_CardType = _T("");
	m_i_CardType = 0;
	m_i_CaptureType = 0;
	m_i_TemplateActionType = 0;
	m_i_FingerUsed = 6;
	m_b_LatentDetect = FALSE;
	m_i_TemplateMaxSize = 255;
	m_i_ApduMaxLength = 255;
	// Match On Card Page
}

CPPageMoc::~CPPageMoc()
{
}


void CPPageMoc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEMPLATE_TYPE, m_ctrl_TemplateType);
	DDX_Text(pDX, IDC_TEMPLATE_TYPE, m_cs_TemplateType);
	DDX_Control(pDX, IDC_CARD_TYPE, m_ctrl_CardType);
	DDX_CBString(pDX, IDC_CARD_TYPE, m_cs_CardType);
	DDX_Radio(pDX, IDC_RADIO_MOC_ENROLL, m_i_TemplateActionType);
	DDX_Check(pDX, IDC_LATENT_DETECT, m_b_LatentDetect);
	DDX_Radio(pDX, IDC_FINGERPRINT_LEFT_LITTLE, m_i_FingerUsed);
	DDX_Check(pDX, IDC_FULL_CCID, m_b_FullCCID);
	DDX_Text(pDX, IDC_TEMPLATE_MAX_SIZE, m_i_TemplateMaxSize);
	DDX_Text(pDX, IDC_APDU_MAX_LENGTH, m_i_ApduMaxLength);

	DDX_Text(pDX, IDC_D0, m_cs_D0);
	DDX_Text(pDX, IDC_D1, m_cs_D1);
	DDX_Text(pDX, IDC_D2, m_cs_D2);
	DDX_Text(pDX, IDC_D3, m_cs_D3);
}	


BEGIN_MESSAGE_MAP(CPPageMoc, CPropertyPage)
	ON_BN_CLICKED(IDC_RADIO_MOC_ENROLL, OnBnClickedRadioStoreOnCard)
	ON_BN_CLICKED(IDC_RADIO_MOC_VERIF, OnBnClickedRadioMatchOnCard)
	ON_CBN_SELCHANGE(IDC_CARD_TYPE, OnSelChangeCardType)
	ON_BN_CLICKED(IDC_FULL_CCID, &CPPageMoc::OnBnClickedFullCcid)
	ON_BN_CLICKED(IDC_GET_INFO, &CPPageMoc::OnBnClickedGetInfo)
END_MESSAGE_MAP()

BOOL CPPageMoc::OnInitDialog()
{
	CString l_cs_string;
	CString l_cs_str;

	CPropertyPage::OnInitDialog();

	m_ctrl_CardType.ResetContent();
	l_cs_string.Format("%s", S1);
	m_ctrl_CardType.AddString(l_cs_string);
	l_cs_string.Format("%s", morphoS2);
	m_ctrl_CardType.AddString(l_cs_string);
	l_cs_string.Format("%s", morphoS3);
	m_ctrl_CardType.AddString(l_cs_string);
	m_ctrl_CardType.SetCurSel(1);

	m_ctrl_TemplateType.ResetContent();
	l_cs_str.Format("%s", STR_PKCOMP);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_PKCOMPNORM);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_PKMAT);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_PKMATNORM);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_PKLITE);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_PKS);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_ANSI378);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_ANSI378_2009);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_ISO_FMR);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_ISO_FMR_2011);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_ISO_FMC_NS);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_ISO_FMC_CS);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_MINEX_A);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_PK_MOC);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_PK_DIN_V66400_CS);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_PK_DIN_V66400_CS_AA);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_ISO_FMC_CS_AA);
	m_ctrl_TemplateType.AddString(l_cs_str);

	l_cs_str.Format("%s", STR_NO_PK_FP);
	m_ctrl_TemplateType.AddString(l_cs_str);

	m_ctrl_TemplateType.SelectString(0, STR_ISO_FMC_CS_AA);
	OnBnClickedRadioMatchOnCard();
	OnSelChangeCardType();

	// Diversification data editboxes disabled by default
	GetDlgItem(IDC_D0)->EnableWindow(FALSE);
	GetDlgItem(IDC_D1)->EnableWindow(FALSE);
	GetDlgItem(IDC_D2)->EnableWindow(FALSE);
	GetDlgItem(IDC_D3)->EnableWindow(FALSE);
	
	UpdateData(TRUE);
	GetDlgItem(IDC_APDU_MAX_LENGTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_MAX_LENGTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_GET_INFO)->EnableWindow(FALSE);
	GetDlgItem(IDC_TEMPLATE_MAX_SIZE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_MAX_SIZE)->EnableWindow(TRUE);
	GetDlgItem(IDC_TEMPLATE_TYPE)->EnableWindow(TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
// CPPageMoc message handlers
void CPPageMoc::OnBnClickedRadioStoreOnCard()
{
	CString l_cs_str;

	UpdateData(TRUE);
	m_i_TemplateActionType = 0;
	m_i_CaptureType = 0;
	GetDlgItem(IDC_LATENT_DETECT)->EnableWindow(FALSE);
	m_b_LatentDetect = FALSE;
	m_b_GetInfo = FALSE;
	GetDlgItem(IDC_TEMPLATE_MAX_SIZE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_MAX_SIZE)->EnableWindow(TRUE);
	GetDlgItem(IDC_TEMPLATE_TYPE)->EnableWindow(TRUE);
	UpdateData(FALSE);

  if (m_cs_CardType == S1)
	{
		// the default template type is PkMoc for a store on S1 card
		m_ctrl_TemplateType.SelectString(0, STR_PK_MOC);
	}
	else if ((m_cs_CardType == morphoS2) || (m_cs_CardType == morphoS3))
	{
		// the default template type is PkIsoFmcCsAa for a store on S3 card
		m_ctrl_TemplateType.SelectString(0, STR_ISO_FMC_CS_AA);
	}

	SelectHand();
}

void CPPageMoc::OnBnClickedRadioMatchOnCard()
{
	CString l_cs_str;

	UpdateData(TRUE);
	m_i_TemplateActionType = 1;
	m_i_CaptureType = 1;
	GetDlgItem(IDC_LATENT_DETECT)->EnableWindow(TRUE);
	m_b_LatentDetect = TRUE;
	m_b_GetInfo = FALSE;
	GetDlgItem(IDC_TEMPLATE_MAX_SIZE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_MAX_SIZE)->EnableWindow(TRUE);
	GetDlgItem(IDC_TEMPLATE_TYPE)->EnableWindow(TRUE);
	UpdateData(FALSE);

	if (m_cs_CardType == S1)
	{
		// the default template type is PkDinCsAa for a match on S1 card
		m_ctrl_TemplateType.SelectString(0, STR_PK_DIN_V66400_CS_AA);
	}
	else if ((m_cs_CardType == morphoS2) || (m_cs_CardType == morphoS3))
	{
		// the default template type is PkIsoFmcCsAa for a match on S3 card
		m_ctrl_TemplateType.SelectString(0, STR_ISO_FMC_CS_AA);
	}

	SelectHand();
}

void CPPageMoc::OnSelChangeCardType() 
{
	UpdateData( TRUE );

	m_ctrl_CardType.GetLBText ( m_ctrl_CardType.GetCurSel(), m_cs_CardType );

	if ( strcmp ( m_cs_CardType, S1 ) == 0 )
	{
		m_i_CardType = 0;
		m_b_Diversification = FALSE;
		
		// Update TemplateType list
		if (m_b_GetInfo)
			OnBnClickedGetInfo();
		else if (m_i_TemplateActionType == 0) 
			OnBnClickedRadioStoreOnCard(); // Store On S1 Card
		else 
			OnBnClickedRadioMatchOnCard(); // Match on S1 Card

		// Disable diversification data editboxes
		GetDlgItem(IDC_D0)->EnableWindow(FALSE);
		GetDlgItem(IDC_D1)->EnableWindow(FALSE);
		GetDlgItem(IDC_D2)->EnableWindow(FALSE);
		GetDlgItem(IDC_D3)->EnableWindow(FALSE);
	}
	else if ( strcmp ( m_cs_CardType, morphoS2 ) == 0 )
	{
		m_i_CardType = 2;
		m_b_Diversification = TRUE;

		// Update TemplateType list
		if (m_b_GetInfo)
			OnBnClickedGetInfo();
		else if(m_i_TemplateActionType == 0)
			OnBnClickedRadioStoreOnCard(); // Store On S2_DIV card
		else
			OnBnClickedRadioMatchOnCard(); // Match on S2_DIV card

		// Enable diversification data editboxes
		GetDlgItem(IDC_D0)->EnableWindow(FALSE);
		GetDlgItem(IDC_D1)->EnableWindow(FALSE);
		GetDlgItem(IDC_D2)->EnableWindow(FALSE);
		GetDlgItem(IDC_D3)->EnableWindow(FALSE);

		m_i_ApduMaxLength = 240;
		UpdateData(FALSE);
	}
	else if ( strcmp ( m_cs_CardType, morphoS3 ) == 0 )
	{
		m_i_CardType = 3;
		m_b_Diversification = FALSE;

		// Update TemplateType list
		if (m_b_GetInfo)
			OnBnClickedGetInfo();
		else if (m_i_TemplateActionType == 0)
			OnBnClickedRadioStoreOnCard ();	// Store On S3 card
		else			
			OnBnClickedRadioMatchOnCard ();	// Match On S3 card

		// Disable diversification data editboxes
		GetDlgItem(IDC_D0)->EnableWindow(FALSE);
		GetDlgItem(IDC_D1)->EnableWindow(FALSE);
		GetDlgItem(IDC_D2)->EnableWindow(FALSE);
		GetDlgItem(IDC_D3)->EnableWindow(FALSE);

		m_i_ApduMaxLength = 255;
		UpdateData(FALSE);
	}

}

void CPPageMoc::SelectHand()
{
	BOOL l_b_Display;

	l_b_Display = TRUE;

	GetDlgItem ( IDC_STATIC_CHOOSE_FINGER )->SetWindowText ( "Choose a finger :");

	if ( m_i_CardType == 0 )
	{
		// S1
		if ( m_i_TemplateActionType == 0 )
		{
			l_b_Display = FALSE;
		}
		else
		{
			GetDlgItem ( IDC_STATIC_CHOOSE_FINGER )->SetWindowText ( "Match");
			l_b_Display = FALSE;
		}
	}
	else
	{
		// S2
		if ( m_i_TemplateActionType == 0 )
		{
			l_b_Display = FALSE;
		}
		else
		{
			GetDlgItem ( IDC_STATIC_CHOOSE_FINGER )->SetWindowText ( "Match");
			l_b_Display = FALSE;
		}
	}

	GetDlgItem ( IDC_HAND_LEFT )->EnableWindow ( l_b_Display );
	GetDlgItem ( IDC_HAND_RIGHT )->EnableWindow ( l_b_Display );
	GetDlgItem ( IDC_STATIC_CHOOSE_FINGER )->EnableWindow ( l_b_Display );

	GetDlgItem ( IDC_FINGERPRINT_LEFT_LITTLE )->EnableWindow ( l_b_Display );
	GetDlgItem ( IDC_FINGERPRINT_LEFT_RING )->EnableWindow ( l_b_Display );
	GetDlgItem ( IDC_FINGERPRINT_LEFT_MIDDLE )->EnableWindow ( l_b_Display );
	GetDlgItem ( IDC_FINGERPRINT_LEFT_INDEX )->EnableWindow ( l_b_Display );
	GetDlgItem ( IDC_FINGERPRINT_LEFT_THUMB )->EnableWindow ( l_b_Display );

	GetDlgItem ( IDC_FINGERPRINT_RIGHT_LITTLE )->EnableWindow ( l_b_Display );
	GetDlgItem ( IDC_FINGERPRINT_RIGHT_RING )->EnableWindow ( l_b_Display );
	GetDlgItem ( IDC_FINGERPRINT_RIGHT_MIDDLE )->EnableWindow ( l_b_Display );
	GetDlgItem ( IDC_FINGERPRINT_RIGHT_INDEX )->EnableWindow ( l_b_Display );
	GetDlgItem ( IDC_FINGERPRINT_RIGHT_THUMB )->EnableWindow ( l_b_Display );
}

void CPPageMoc::OnBnClickedFullCcid()
{
	UpdateData(TRUE);
	if (m_b_FullCCID)
	{
		GetDlgItem(IDC_APDU_MAX_LENGTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MAX_LENGTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_GET_INFO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_APDU_MAX_LENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MAX_LENGTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_GET_INFO)->EnableWindow(FALSE);
		if (m_b_GetInfo)
			OnBnClickedRadioMatchOnCard();
	}
}


void CPPageMoc::OnBnClickedGetInfo()
{
	m_b_GetInfo = TRUE;
	GetDlgItem(IDC_LATENT_DETECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_TEMPLATE_MAX_SIZE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_MAX_SIZE)->EnableWindow(FALSE);
	GetDlgItem(IDC_TEMPLATE_TYPE)->EnableWindow(FALSE);
}

