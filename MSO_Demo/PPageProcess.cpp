/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// PPageProcess.cpp : implementation file
//

#include "stdafx.h"
#include "MSO_Demo.h"
#include "PPageProcess.h"
#include "MSO_DemoDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPageProcess property page

IMPLEMENT_DYNCREATE(CPPageProcess, CPropertyPage)

CPPageProcess::CPPageProcess() : CPropertyPage(CPPageProcess::IDD)
, m_cs_AsyncCmdMsg(_T(""))
{

}

CPPageProcess::CPPageProcess(CMSO_DemoDlg* pDemoDlg) : CPropertyPage(CPPageProcess::IDD)
{
	//{{AFX_DATA_INIT(CPPageProcess)
	//}}AFX_DATA_INIT

	m_pDemoDlg = pDemoDlg;
	m_i_ViewerToColorIdx = 0;
	m_cs_AsyncCmdMsg = _T("");
	
	m_i_tabColor[0] = 0;
	m_i_tabColor[1] = 0;
	m_i_tabColor[2] = 0;
	m_i_tabColor[3] = 0;
	m_i_tabColor[4] = 0;
	m_i_tabColor[5] = 0;

	m_ctl_MiniViewer_2_3.SetParent(pDemoDlg);
	m_ctl_MiniViewer_2_2.SetParent(pDemoDlg);
	m_ctl_MiniViewer_2_1.SetParent(pDemoDlg);
	m_ctl_MiniViewer_1_3.SetParent(pDemoDlg);
	m_ctl_MiniViewer_1_2.SetParent(pDemoDlg);
	m_ctl_MiniViewer_1_1.SetParent(pDemoDlg);

	m_ctl_MiniViewer_1_1.SetIndex(0);
	m_ctl_MiniViewer_1_2.SetIndex(1);
	m_ctl_MiniViewer_1_3.SetIndex(2);
	m_ctl_MiniViewer_2_1.SetIndex(3);
	m_ctl_MiniViewer_2_2.SetIndex(4);
	m_ctl_MiniViewer_2_3.SetIndex(5);
}

CPPageProcess::~CPPageProcess()
{
}

void CPPageProcess::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPageProcess)
	DDX_Control(pDX, IDC_STATIC_MINI_VIEWER_2_3, m_ctl_MiniViewer_2_3);
	DDX_Control(pDX, IDC_STATIC_MINI_VIEWER_2_2, m_ctl_MiniViewer_2_2);
	DDX_Control(pDX, IDC_STATIC_MINI_VIEWER_2_1, m_ctl_MiniViewer_2_1);
	DDX_Control(pDX, IDC_STATIC_MINI_VIEWER_1_3, m_ctl_MiniViewer_1_3);
	DDX_Control(pDX, IDC_STATIC_MINI_VIEWER_1_2, m_ctl_MiniViewer_1_2);
	DDX_Control(pDX, IDC_STATIC_MINI_VIEWER_1_1, m_ctl_MiniViewer_1_1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SYNC_QUALITY, m_ctl_AsyncCmdMsg);
	DDX_Text(pDX, IDC_SYNC_QUALITY, m_cs_AsyncCmdMsg);
}


BEGIN_MESSAGE_MAP(CPPageProcess, CPropertyPage)
	//{{AFX_MSG_MAP(CPPageProcess)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPageProcess message handlers

void CPPageProcess::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Do not call CPropertyPage::OnPaint() for painting messages
    // Draw a  rectangle around the viewer

	drawRectangle();
}

void CPPageProcess::setColor(int i_i_Color)
{
	if(m_i_tabColor[m_i_ViewerToColorIdx] != 1)
	{
		m_i_tabColor[m_i_ViewerToColorIdx] = i_i_Color;
	}
}

void CPPageProcess::drawRectangle()
{
	int i=0;
	CDC* pDC = GetDC();
	CRect l_rect;

	CBrush l_greenBrush( RGB( 0, 255, 0));
	CBrush l_orangeBrush( RGB(255,165,0));
	
	for(i=0;i<6;i++)
	{
		if(m_i_tabColor[i] != 0)
		{
			switch (i)
			{
				case 0:
					m_ctl_MiniViewer_1_1.GetWindowRect(&l_rect);
					break;
				case 1:
					m_ctl_MiniViewer_1_2.GetWindowRect(&l_rect);
					break;
				case 2:
					m_ctl_MiniViewer_1_3.GetWindowRect(&l_rect);
					break;
				case 3:
					m_ctl_MiniViewer_2_1.GetWindowRect(&l_rect);
					break;
				case 4:
					m_ctl_MiniViewer_2_2.GetWindowRect(&l_rect);
					break;
				case 5:
					m_ctl_MiniViewer_2_3.GetWindowRect(&l_rect);
					break;
			}

			ScreenToClient(&l_rect);

			//Draw a single line around the outside	
			if(m_i_tabColor[i] == 1)
			{
				pDC->FrameRect(&l_rect, &l_greenBrush);	
			}
			else if(m_i_tabColor[i] == 2)
			{
				pDC->FrameRect(&l_rect, &l_orangeBrush);
			}
		}
	}
    
	ReleaseDC( pDC);
}
