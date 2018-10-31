/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// DisplayMSOConfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MORPHO_Interface.h"
#include "mso_demo.h"
#include "DisplayMSOConfDlg.h"
#include "DrawTitle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplayMSOConfDlg dialog

#define FAR0		"All FAR are allowed"
#define FAR1		"1 (1 %)"
#define FAR2		"2 (0.3 %)"
#define FAR3		"3 (0.1 %)"
#define FAR4		"4 (0.03 %)"
#define FAR5		"5 (0.01 %) : recommended"
#define FAR6		"6 (0.001 %)"
#define FAR7		"7 (0.0001 %)"
#define FAR8		"8 (0.00001 %)"
#define FAR9		"9 (0.0000001 %)"
#define FAR_OVER	"No FAR are allowed"

#define SECURITY_LEVEL_STANDARD	"Standard"
#define SECURITY_LEVEL_MEDIUM	"Medium"
#define SECURITY_LEVEL_HIGH		"High"
#define SECURITY_LEVEL_CRITICAL	"Critical"

CDisplayMSOConfDlg::CDisplayMSOConfDlg(PC i_str_serialNumber, UC i_uc_SecuOption, US i_us_FAR, UL i_uL_MSL, bool i_b_isFVP, CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayMSOConfDlg::IDD, pParent)
	, m_cs_Min_MSL(_T(""))
{
	//{{AFX_DATA_INIT(CDisplayMSOConfDlg)
	m_serialNum = _T(i_str_serialNumber);
	//}}AFX_DATA_INIT

	switch(i_us_FAR)
	{
		case 0:
			m_FAR = _T(FAR0);
			break;
		case 1:
			m_FAR = _T(FAR1);
			break;
		case 2:
			m_FAR = _T(FAR2);
			break;
		case 3:
			m_FAR = _T(FAR3);
			break;
		case 4:
			m_FAR = _T(FAR4);
			break;
		case 5:
			m_FAR = _T(FAR5);
			break;
		case 6:
			m_FAR = _T(FAR6);
			break;
		case 7:
			m_FAR = _T(FAR7);
			break;
		case 8:
			m_FAR = _T(FAR8);
			break;
		case 9:
			m_FAR = _T(FAR9);
			break;
		default:
			m_FAR = _T(FAR_OVER);
			break;
	}

	m_uc_secuConf = i_uc_SecuOption;

	m_isFVP = i_b_isFVP;

	switch(i_uL_MSL)
	{
		case MULTIMODAL_SECURITY_CRITICAL:
			m_cs_Min_MSL = _T(SECURITY_LEVEL_CRITICAL);
			break;
		case MULTIMODAL_SECURITY_HIGH:
			m_cs_Min_MSL = _T(SECURITY_LEVEL_HIGH);
			break;
		case MULTIMODAL_SECURITY_MEDIUM:
			m_cs_Min_MSL = _T(SECURITY_LEVEL_MEDIUM);
			break;
		case MULTIMODAL_SECURITY_STANDARD:
			m_cs_Min_MSL = _T(SECURITY_LEVEL_STANDARD);
			break;
	}
}


void CDisplayMSOConfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplayMSOConfDlg)
	DDX_Control(pDX, IDC_SECU_OPTIONS, m_list);
	DDX_Text(pDX, IDC_EDIT_FAR, m_FAR);
	DDV_MaxChars(pDX, m_FAR, 30);
	DDX_Text(pDX, IDC_EDIT_SERIAL_NUMBER, m_serialNum);
	DDV_MaxChars(pDX, m_serialNum, 25);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_MINMSL, m_cs_Min_MSL);
}


BEGIN_MESSAGE_MAP(CDisplayMSOConfDlg, CDialog)
	//{{AFX_MSG_MAP(CDisplayMSOConfDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplayMSOConfDlg message handlers

BOOL CDisplayMSOConfDlg::OnInitDialog() 
{
	HICON	hIcon;
	CString	str;

	CDialog::OnInitDialog();
	m_list.InsertColumn(0, "description", LVCFMT_LEFT, 230);
	m_imageList.Create(16, 16, ILC_COLOR, 4, 2);
	hIcon = AfxGetApp()->LoadIcon(IDI_YES);
	m_imageList.Add(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_NO);
	m_imageList.Add(hIcon);
	m_list.SetImageList(&m_imageList, LVSIL_SMALL);

	m_i_index = 0;

	str.LoadString(IDS_SECU_SIGNED_DOWNLOAD);
	if (m_uc_secuConf & SECU_SIGNED_DOWNLOAD)
	{
		m_list.InsertItem(m_i_index++, str ,0);
	}
	else
	{
		m_list.InsertItem(m_i_index++, str ,1);
	}

	str.LoadString(IDS_SECU_TUNNELING);
	if (m_uc_secuConf & SECU_TUNNELING)
	{
		m_list.InsertItem(m_i_index++, str ,0);
	}
	else
	{
		m_list.InsertItem(m_i_index++, str ,1);
	}

	str.LoadString(IDS_SECU_OFFERED_SECU);
	if (m_uc_secuConf & SECU_OFFERED_SECURITY)
	{
		m_list.InsertItem(m_i_index++, str ,0);
	}
	else
	{
		m_list.InsertItem(m_i_index++, str ,1);
	}

	str.LoadString(IDS_SECU_PK_ONLY_SIGNED);
	if (m_uc_secuConf & SECU_PK_ONLY_SIGNED)
	{
		m_list.InsertItem(m_i_index++, str ,0);
	}
	else
	{
		m_list.InsertItem(m_i_index++, str ,1);
	}

	str.LoadString(IDS_SECU_EXPORT_SCORE);
	if (m_uc_secuConf & SECU_NEVER_EXPORT_SCORE)
	{
		m_list.InsertItem(m_i_index++, str ,1);
	}
	else
	{
		m_list.InsertItem(m_i_index++, str ,0);
	}

	if(!m_isFVP)
	{
		GetDlgItem(IDC_MINMSL_TXT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_MINMSL)->ShowWindow(SW_HIDE);
		RECT l_x_idok_rect;
		l_x_idok_rect.left = 240;
		l_x_idok_rect.top = 235;
		l_x_idok_rect.right = 300;
		l_x_idok_rect.bottom = 255;

		GetDlgItem(IDOK)->MoveWindow(&l_x_idok_rect,0);		
		RECT r;
		GetWindowRect(&r);
		SetWindowPos(NULL,r.left,r.top,r.right-r.left,r.bottom-r.top-65,0);
		UpdateWindow();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDisplayMSOConfDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CString		msg;

	msg.LoadString(IDS_MSO_CONFIGURATION);

	CDrawTitle::DrawVerticalTitle(this, &dc, msg, FALSE);
	// Do not call CDialog::OnPaint() for painting messages
}

void CDisplayMSOConfDlg::OnOK() 
{
	m_imageList.DeleteImageList();

	m_list.DeleteItem(0);
	CDialog::OnOK();
}
