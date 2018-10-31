/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// DisplayImage.cpp : implementation file
//

#include "stdafx.h"
#include "mso_demo.h"
#include "MORPHO_Types.h"
#include "MORPHO_Image.h"
#include "DisplayImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplayImageDlg dialog


CDisplayImageDlg::CDisplayImageDlg(T_MORPHO_IMAGE	*i_px_Image, CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayImageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDisplayImageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_px_CImage = new C_MORPHO_Image(1);

	m_px_TImage = i_px_Image;

}

CDisplayImageDlg::~CDisplayImageDlg()
{
	delete m_px_CImage;
}

void CDisplayImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplayImageDlg)
	DDX_Control(pDX, IDOK, m_ctrl_Ok);
	DDX_Control(pDX, IDC_IMAGE, m_ctrl_Image);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDisplayImageDlg, CDialog)
	//{{AFX_MSG_MAP(CDisplayImageDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplayImageDlg message handlers

void CDisplayImageDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDC				*l_p_Cdc;
	CRect			myRect;
	int				l_i_HORZRES;
	int				l_i_VERTRES;
	RECT RectWindowCWnd;
	RECT RectWindowOK;
	CString l_cs_Title;

	CDialog::OnShowWindow(bShow, nStatus);
	
	
	l_cs_Title.Format ( "Size:%d*%d pix, Res:%d*%d dpi, %d bits/pixels", 
		m_px_TImage->m_x_ImageHeader.m_us_NbCol, 
		m_px_TImage->m_x_ImageHeader.m_us_NbRow, 
		m_px_TImage->m_x_ImageHeader.m_us_ResX, 
		m_px_TImage->m_x_ImageHeader.m_us_ResY, 
		m_px_TImage->m_x_ImageHeader.m_uc_NbBitsPerPixel );

	SetWindowText ( l_cs_Title );

	l_p_Cdc = m_ctrl_Image.GetDC();
	m_ctrl_Image.GetClientRect(&myRect);

	m_px_CImage->SetImage(	
		0,							// UL	i_ul_ImageIndex,
		*m_px_TImage,				// T_MORPHO_CALLBACK_IMAGE_STATUS	i_x_Image,
		0,							// UL	i_ul_imageFilter,
		l_p_Cdc,					// CDC	*i_p_Cdc,
		myRect						// CRect &i_p_Rect
		);
	
	l_i_HORZRES = l_p_Cdc->GetDeviceCaps ( HORZRES );
	l_i_VERTRES = l_p_Cdc->GetDeviceCaps ( VERTRES );

	ReleaseDC(l_p_Cdc);

	GetWindowRect ( &RectWindowCWnd );
	m_ctrl_Ok.GetWindowRect ( &RectWindowOK );
	m_l_OKPos = RectWindowCWnd.bottom-RectWindowOK.bottom;

	MoveWindow (0, 0, l_i_HORZRES*9/10, l_i_VERTRES*9/10);

}

HBRUSH CDisplayImageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
// To correctly refresh
	PostMessage(WM_PAINT,0,0);

	return hbr;
}

void CDisplayImageDlg::OnPaint() 
{
	CDC		*l_p_Cdc;
	CRect	myRect;

	if (m_px_CImage != NULL)
	{
		//-------------------------------
		// Refresh Async Images Viewer 
		//-------------------------------	
			l_p_Cdc = m_ctrl_Image.GetDC();
			m_ctrl_Image.GetClientRect(&myRect);
			m_px_CImage->RefreshViewer(0,myRect,l_p_Cdc);  
			ReleaseDC(l_p_Cdc);
	}

	CDialog::OnPaint();
}

void CDisplayImageDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	RECT RectClientCWnd;
	RECT RectClientOK;
	int l_i_cx;
	int l_i_cy;

	l_i_cx = cx;
	l_i_cy = cy-(4*m_l_OKPos);

	if (l_i_cy > l_i_cx)
		l_i_cy = l_i_cx;

	if ( l_i_cx > l_i_cy )
		l_i_cx = l_i_cy;

	if ( m_ctrl_Image )
		m_ctrl_Image.MoveWindow ( /*to center*/(cx-l_i_cx)/2, 0, l_i_cx, l_i_cy);

	if ( m_ctrl_Image )
	{
		GetClientRect ( &RectClientCWnd );
		m_ctrl_Ok.GetClientRect ( &RectClientOK );

		m_ctrl_Ok.SetWindowPos ( 
			NULL, 
			RectClientCWnd.right/2 - RectClientOK.right/2, 
			cy - m_l_OKPos - RectClientOK.bottom , 
			RectClientOK.right, 
			RectClientOK.bottom, 
			0 );
	}

}
