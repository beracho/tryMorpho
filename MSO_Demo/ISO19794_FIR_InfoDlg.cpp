// ISO19794_FIR_InfoDlg.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "ISO19794_FIR_InfoDlg.h"


// Boîte de dialogue ISO19794_FIR_InfoDlg

IMPLEMENT_DYNAMIC(ISO19794_FIR_InfoDlg, CDialog)

ISO19794_FIR_InfoDlg::ISO19794_FIR_InfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ISO19794_FIR_InfoDlg::IDD, pParent)
	, m_cs_DesiredCompression(_T(""))
	, m_b_UpdateCtr(true)
	, m_b_ShowDesiredCompressComoBox(false)
	, m_x_DesiredCompression(MORPHO_COMPRESS_UNCHANGED)
	, m_ul_ImageBufferSize(0)
{
}

ISO19794_FIR_InfoDlg::~ISO19794_FIR_InfoDlg()
{
}

void ISO19794_FIR_InfoDlg::FitComboBox(CComboBox & i_px_ComboBox)
{
	// Local variables
	CDC *	l_px_DC;
	CFont *	l_px_Font;
	CSize	l_x_TextSize;
	CString	l_x_String;
	int		l_i_CurrentWidth;
	int		l_i_CalculatedWidth;

	// Update data
	//UpdateData();

	// Initialize local variables
	l_px_DC				= NULL;
	l_px_Font			= NULL;
	l_i_CurrentWidth	= 0;
	l_i_CalculatedWidth	= 0;

	// Retrieve ComboBox DeviceContext
	l_px_DC = i_px_ComboBox.GetDC();

	// Set ComboBox DeviceContext map mode to pixels (1 logical unit equal 1 device pixel)
	l_px_DC->SetMapMode(MM_TEXT);

	// Set ComboBox DeviceContext font with the current font in order to get real size values
	l_px_Font = this->GetFont();
	l_px_DC->SelectObject(l_px_Font);

	// Retrieve current ComboBox dropdown width
	l_i_CurrentWidth = i_px_ComboBox.GetDroppedWidth();

	// For-each string in the ComboBox
	for(int l_i_Cpt = 0; l_i_Cpt < i_px_ComboBox.GetCount(); l_i_Cpt++)
	{
		// Retrieve the ComboBox string
		i_px_ComboBox.GetLBText(l_i_Cpt, l_x_String);

		// Calculate ComboBox string size (in pixels)
		l_x_TextSize = l_px_DC->GetTextExtent(l_x_String);
		l_px_DC->LPtoDP(&l_x_TextSize);

		// If current string size is greater than the ComboBox dropdown calculated width, then update it
		if(l_x_TextSize.cx > l_i_CalculatedWidth)
			l_i_CalculatedWidth = l_x_TextSize.cx;
	}

	// Add left & right edges width plus vertical scrollbar width to the ComboBox dropdown calculated width
	l_i_CalculatedWidth += GetSystemMetrics(SM_CXVSCROLL) + (2 * GetSystemMetrics(SM_CXEDGE));

	// Select the greater value between the ComboBox dropdown current width and the ComboBox dropdown calculated width
	l_i_CurrentWidth = max(l_i_CurrentWidth, l_i_CalculatedWidth);

	// Update ComboBox dropdown width
	i_px_ComboBox.SetDroppedWidth(l_i_CurrentWidth);

	// Release ComboBox DeviceContext
	i_px_ComboBox.ReleaseDC(l_px_DC);
}

BOOL ISO19794_FIR_InfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(m_b_ShowDesiredCompressComoBox == true)
	{
		GetDlgItem(IDC_COMBO_DESIRED_COMPRESS)->EnableWindow(TRUE);
	}
	else 
	{
		GetDlgItem(IDC_COMBO_DESIRED_COMPRESS)->EnableWindow(FALSE);
		RECT rc;		
		m_cmd_DesiredCompression.GetWindowRect(&rc);
		ScreenToClient(&rc);
		m_ToolTipCtrl.Create(this);
		m_ToolTipCtrl.AddTool(this,IDS_TT_INFO_USE_RAW,&rc,1);
		m_ToolTipCtrl.Activate(TRUE);
	}

	switch(m_ul_ImageBufferSize)
	{
		// Image seems to be a CBM image
		case 102400:
			this->SetDlgItemTextA(IDC_EDIT_NBCOLS,"256");
			this->SetDlgItemTextA(IDC_EDIT_NBROWS,"400");
			break;
		// Image seems to be a FVP image
		case 160000:
			this->SetDlgItemTextA(IDC_EDIT_NBCOLS,"400");
			this->SetDlgItemTextA(IDC_EDIT_NBROWS,"400");
			break;
		// Image seems to be a MSO image
		case 173056:
			this->SetDlgItemTextA(IDC_EDIT_NBCOLS,"416");
			this->SetDlgItemTextA(IDC_EDIT_NBROWS,"416");
			break;
		// Image seems to be a MorphoTop image
		case 2400000:
			this->SetDlgItemTextA(IDC_EDIT_NBCOLS,"1600");
			this->SetDlgItemTextA(IDC_EDIT_NBROWS,"1500");
			break;
		// Default image size (MSO image size)
		default:
			this->SetDlgItemTextA(IDC_EDIT_NBCOLS,"416");
			this->SetDlgItemTextA(IDC_EDIT_NBROWS,"416");
			break;
	}

	this->SetDlgItemTextA(IDC_EDIT_RESX,"500");
	this->SetDlgItemTextA(IDC_EDIT_RESY,"500");
	this->SetDlgItemTextA(IDC_EDIT_NB_BIT_PPIXEL,"8");

	return TRUE;
}

BOOL ISO19794_FIR_InfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// transate the message based on TTM_WINDOWFROMPOINT
	MSG msg = *pMsg;
	msg.hwnd = (HWND)m_ToolTipCtrl.SendMessage(TTM_WINDOWFROMPOINT, 0, (LPARAM)&msg.pt);
	CPoint pt = pMsg->pt;
	if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
		::ScreenToClient(msg.hwnd, &pt);
	msg.lParam = MAKELONG(pt.x, pt.y);

	// Let the ToolTip process this message.
	m_ToolTipCtrl.RelayEvent(&msg);

	return CDialog::PreTranslateMessage(pMsg);
}

void ISO19794_FIR_InfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_DESIRED_COMPRESS, m_cmd_DesiredCompression);
	DDX_CBString(pDX, IDC_COMBO_DESIRED_COMPRESS, m_cs_DesiredCompression);

	if(m_b_UpdateCtr)
	{
		// Image compression
		m_cmd_DesiredCompression.ResetContent();

		CString l_cs_string;

		l_cs_string.Format("%s", STR_ISO19794_FIR_RAW);
		m_cmd_DesiredCompression.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO19794_FIR_BMP);
		m_cmd_DesiredCompression.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO19794_FIR_WSQ);
		m_cmd_DesiredCompression.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO19794_FIR_JPEG);
		m_cmd_DesiredCompression.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO19794_FIR_JPEG2000);
		m_cmd_DesiredCompression.AddString(l_cs_string);

		l_cs_string.Format("%s", STR_ISO19794_FIR_PNG);
		m_cmd_DesiredCompression.AddString(l_cs_string);

		FitComboBox(m_cmd_DesiredCompression);
		m_cmd_DesiredCompression.SelectString(0, STR_ISO19794_FIR_RAW);
	}
}


BEGIN_MESSAGE_MAP(ISO19794_FIR_InfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &ISO19794_FIR_InfoDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_DESIRED_COMPRESS, &ISO19794_FIR_InfoDlg::OnCbnSelchangeComboDesiredCompress)
END_MESSAGE_MAP()


// Gestionnaires de messages de ISO19794_FIR_InfoDlg

void ISO19794_FIR_InfoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	this->GetDlgItemTextA(IDC_EDIT_RESX,m_cs_ResX);
	this->GetDlgItemTextA(IDC_EDIT_RESY,m_cs_ResY);
	this->GetDlgItemTextA(IDC_EDIT_NBROWS,m_cs_NBRows);
	this->GetDlgItemTextA(IDC_EDIT_NBCOLS,m_cs_NBCols);
	this->GetDlgItemTextA(IDC_EDIT_NB_BIT_PPIXEL,m_cs_NbBitsPerPixel);

	OnOK();
}

void ISO19794_FIR_InfoDlg::OnCbnSelchangeComboDesiredCompress()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	m_b_UpdateCtr = false;
	UpdateData(TRUE);

	m_cmd_DesiredCompression.GetLBText(m_cmd_DesiredCompression.GetCurSel(), m_cs_DesiredCompression);

	if(strcmp(m_cs_DesiredCompression, STR_ISO19794_FIR_RAW) == 0)
		m_x_DesiredCompression = MORPHO_COMPRESS_UNCHANGED;
	else if(strcmp(m_cs_DesiredCompression, STR_ISO19794_FIR_BMP) == 0)
		m_x_DesiredCompression = MORPHO_COMPRESS_BMP;
	else if(strcmp(m_cs_DesiredCompression, STR_ISO19794_FIR_WSQ) == 0)
		m_x_DesiredCompression = MORPHO_COMPRESS_WSQ;
	else if(strcmp(m_cs_DesiredCompression, STR_ISO19794_FIR_JPEG) == 0)
		m_x_DesiredCompression = MORPHO_COMPRESS_JPEG;
	else if(strcmp(m_cs_DesiredCompression, STR_ISO19794_FIR_JPEG2000) == 0)
		m_x_DesiredCompression = MORPHO_COMPRESS_JPEG2000;
	else if(strcmp(m_cs_DesiredCompression, STR_ISO19794_FIR_PNG) == 0)
		m_x_DesiredCompression = MORPHO_COMPRESS_PNG;
}