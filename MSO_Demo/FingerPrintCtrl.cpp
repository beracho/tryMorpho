#include "StdAfx.h"
#include "MSO_Demo.h"
#include "FingerPrintCtrl.h"
#include "MSO_DemoDlg.h"



CFingerPrintCtrl::CFingerPrintCtrl(void):CStatic()
{
	m_px_ImageStructure = NULL;
	m_bIsFingerPrintLoaded = FALSE;
}

CFingerPrintCtrl::~CFingerPrintCtrl(void)
{
	if(m_px_ImageStructure != NULL)
	{
		if(m_px_ImageStructure->m_puc_Image != NULL)
		{
			free(m_px_ImageStructure->m_puc_Image);
			m_px_ImageStructure->m_puc_Image = NULL;
		}
		free(m_px_ImageStructure);
		m_px_ImageStructure = NULL;
	}
}

void CFingerPrintCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0, SS_OWNERDRAW);
}

void CFingerPrintCtrl::SetParent(CMSO_DemoDlg* pDemoDlg)
{
	m_pDemoDlg = pDemoDlg;
}

void CFingerPrintCtrl::SetIndex(int i_i_index)
{
	m_i_index = i_i_index;
}

void CFingerPrintCtrl::SetImage(T_MORPHO_CALLBACK_IMAGE_STATUS i_x_image)
{
	int imageSize = 0;
	if(m_px_ImageStructure == NULL)
	{
		m_px_ImageStructure = (T_MORPHO_CALLBACK_IMAGE_STATUS *  )malloc(sizeof(T_MORPHO_CALLBACK_IMAGE_STATUS));
		memset(m_px_ImageStructure,0x00,sizeof(T_MORPHO_CALLBACK_IMAGE_STATUS)); 
	}
	
	memcpy(&m_px_ImageStructure->m_x_ImageHeader, &i_x_image.m_x_ImageHeader, sizeof(T_MORPHO_IMAGE_HEADER));

	imageSize = (i_x_image.m_x_ImageHeader.m_us_NbRow * i_x_image.m_x_ImageHeader.m_us_NbCol)/(8/i_x_image.m_x_ImageHeader.m_uc_NbBitsPerPixel);

	if(m_px_ImageStructure->m_puc_Image != NULL)
	{
		free(m_px_ImageStructure->m_puc_Image);
		m_px_ImageStructure->m_puc_Image = NULL;
	}
		
	m_px_ImageStructure->m_puc_Image = (PUC)malloc(imageSize * sizeof(UC));	

	memcpy(m_px_ImageStructure->m_puc_Image,i_x_image.m_puc_Image,imageSize);	

	m_bIsFingerPrintLoaded = TRUE;
}

void CFingerPrintCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{	
	if(m_bIsFingerPrintLoaded && m_pDemoDlg->m_px_Image != NULL)
	{
		CRect rect;
		CDC	*l_p_Cdc;

		l_p_Cdc = this->GetDC();
		this->GetClientRect(&rect);

		m_pDemoDlg->m_px_Image->SetImage(	
			(UL)m_i_index,							// UL	i_ul_ImageIndex,
			*m_px_ImageStructure,					// T_MORPHO_CALLBACK_IMAGE_STATUS	i_x_Image,
			0,										// UL	i_ul_imageFilter,
			l_p_Cdc,								// CDC	*i_p_Cdc,
			rect									// CRect &i_p_Rect
			);

		m_pDemoDlg->ReleaseDC(l_p_Cdc);
	}
}

