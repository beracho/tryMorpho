/***********************************************************

 PROJET      : Morpho Smart
 MODULE      : 

 FILE NAME   : $Workfile:   MORPHO_Image.cpp  $
 DESCRIPTION :

 REVISION    : $Revision:   1.0  $

 DATE        : $Date:   Apr 18 2016 14:30:06  $

************************************************************
  Copyright (c) 2016, Morpho
************************************************************

************************************************************/

#include "stdafx.h"
#include <math.h>
#include "MORPHO_Types.h"
#include "MORPHO_Image.h"
#include "MORPHO_Errors.h"

#define MAX_IMAGE_SIZE	(4*416*416)

C_MORPHO_Image::C_MORPHO_Image(UL	i_ul_ImageIndex)
{
	m_px_MorphoImage = NULL;
	m_px_MorphoImage = (PT_MORPHO_IMAGE)malloc(sizeof(T_MORPHO_IMAGE));
	if (m_px_MorphoImage != NULL)
	{
		m_px_MorphoImage->m_pui_image8bits = (PUI)malloc(MAX_IMAGE_SIZE);
		m_px_MorphoImage->m_pBitmap = (HBITMAP*)calloc(i_ul_ImageIndex, sizeof(HBITMAP));
		m_px_MorphoImage->m_ul_ImageNbIndex = i_ul_ImageIndex;
	}
}

C_MORPHO_Image::C_MORPHO_Image(const C_MORPHO_Image& i_px_Image)
{
	m_px_MorphoImage = NULL;
	m_px_MorphoImage = (PT_MORPHO_IMAGE)malloc(sizeof(T_MORPHO_IMAGE));
	if (m_px_MorphoImage != NULL)
	{
		m_px_MorphoImage->m_ul_ImageNbIndex = i_px_Image.m_px_MorphoImage->m_ul_ImageNbIndex;
		m_px_MorphoImage->m_pui_image8bits = (PUI)malloc(MAX_IMAGE_SIZE);
		if (m_px_MorphoImage->m_pui_image8bits != NULL)
		{
			memcpy(m_px_MorphoImage->m_pui_image8bits, i_px_Image.m_px_MorphoImage->m_pui_image8bits, MAX_IMAGE_SIZE);
		}
		m_px_MorphoImage->m_pBitmap = (HBITMAP*)calloc(m_px_MorphoImage->m_ul_ImageNbIndex, sizeof(HBITMAP));
	}
}

C_MORPHO_Image& C_MORPHO_Image::operator= (const C_MORPHO_Image& i_px_Image)
{
	if(this != &i_px_Image)
	{
		m_px_MorphoImage = NULL;
		m_px_MorphoImage = (PT_MORPHO_IMAGE)malloc(sizeof(T_MORPHO_IMAGE));
		if (m_px_MorphoImage != NULL)
		{
			m_px_MorphoImage->m_ul_ImageNbIndex = i_px_Image.m_px_MorphoImage->m_ul_ImageNbIndex;
			m_px_MorphoImage->m_pui_image8bits = (PUI)malloc(MAX_IMAGE_SIZE);
			if (m_px_MorphoImage->m_pui_image8bits != NULL)
			{
				memcpy(m_px_MorphoImage->m_pui_image8bits, i_px_Image.m_px_MorphoImage->m_pui_image8bits, MAX_IMAGE_SIZE);
				m_px_MorphoImage->m_pBitmap = (HBITMAP*)calloc(m_px_MorphoImage->m_ul_ImageNbIndex, sizeof(HBITMAP));
			}
		}
	}
	return *this;
}

C_MORPHO_Image::~C_MORPHO_Image()
{
	UL	l_ul_Cnt;

	if (m_px_MorphoImage->m_pui_image8bits!=NULL)
	{
		free(m_px_MorphoImage->m_pui_image8bits);
		m_px_MorphoImage->m_pui_image8bits = NULL;
	}

	for (l_ul_Cnt=0; l_ul_Cnt<m_px_MorphoImage->m_ul_ImageNbIndex; l_ul_Cnt++)
		if (m_px_MorphoImage->m_pBitmap != NULL)
		{
			DeleteObject(m_px_MorphoImage->m_pBitmap[l_ul_Cnt]);
			m_px_MorphoImage->m_pBitmap[l_ul_Cnt] = NULL;
		}

	if (m_px_MorphoImage->m_pBitmap != NULL)
	{
		free(m_px_MorphoImage->m_pBitmap);
		m_px_MorphoImage->m_pBitmap = NULL;
	}

	free(m_px_MorphoImage);
	m_px_MorphoImage = NULL;
}

#define CORRECT_LEVEL2(a)	if (a == 0x00)		\
								a = 0x02;		\
							else if (a == 0x40)	\
								a = 0x48;		\
							else if (a == 0x80)	\
								a = 0xB0;		\
							else if (a == 0xC0)	\
								a = 0xFE;		\

#define CORRECT_LEVEL1(a)	if (a == 0x00)		\
								a = 0x00;		\
							else if (a == 0x80)	\
								a = 0xFF;		\

I C_MORPHO_Image::SetImage(	UL								i_ul_ImageIndex,
							T_MORPHO_CALLBACK_IMAGE_STATUS	i_x_Image,
							UL								i_ul_imageFilter,
							CDC								*i_p_Cdc,
							CRect &							i_p_Rect)
{
	I		l_i_ImageSize, l_i_Cnt;
	I		l_i_Ret;
	PUC		l_puc_image8bits;
	PUC		l_puc_image = NULL;

	if (!m_px_MorphoImage)
		return MORPHOERR_INVALID_CLASS;

	if  (i_x_Image.m_ul_imageSize == 1)
		i_x_Image.m_ul_imageSize = 1;
	if (i_x_Image.m_x_ImageHeader.m_uc_NbBitsPerPixel == 1)
		i_x_Image.m_ul_imageSize = i_x_Image.m_ul_imageSize;

	if (m_px_MorphoImage->m_ul_ImageNbIndex <= i_ul_ImageIndex)
		return MORPHOERR_BADPARAMETER;

	if (m_px_MorphoImage->m_pui_image8bits == NULL)
		return MORPHOERR_CORRUPTED_CLASS;

	if (i_ul_imageFilter & MORPHO_LINEAR_INTERPOLATION)
	{
		l_puc_image = (PUC)malloc(MAX_IMAGE_SIZE);
		if (l_puc_image == NULL)
			l_i_Ret = MORPHOERR_MEMORY_PC;
		l_puc_image8bits = l_puc_image;
	}
	else
		l_puc_image8bits = (PUC)m_px_MorphoImage->m_pui_image8bits;

	l_i_ImageSize = i_x_Image.m_x_ImageHeader.m_us_NbCol * i_x_Image.m_x_ImageHeader.m_us_NbRow;

	switch(i_x_Image.m_x_ImageHeader.m_uc_NbBitsPerPixel)
	{
		case 8:
			if (l_puc_image8bits!= NULL)
			{
				memcpy(l_puc_image8bits, i_x_Image.m_puc_Image, l_i_ImageSize);
			}
			break;
		case 4:
			for (l_i_Cnt = 0; l_i_Cnt < l_i_ImageSize/2; l_i_Cnt++)
			{
				if (l_puc_image8bits!= NULL)
				{
					l_puc_image8bits[2*l_i_Cnt]   = (i_x_Image.m_puc_Image[l_i_Cnt] & 0xF0) + 0x08;
					l_puc_image8bits[2*l_i_Cnt+1] = ((i_x_Image.m_puc_Image[l_i_Cnt] & 0x0F)<<4) + 0x08;
				}
			}
			break;
		case 2:
			for (l_i_Cnt = 0; l_i_Cnt < l_i_ImageSize/4; l_i_Cnt++)
			{
				if (l_puc_image8bits!= NULL)
				{
					l_puc_image8bits[4*l_i_Cnt]   = (i_x_Image.m_puc_Image[l_i_Cnt] & 0xC0);
					l_puc_image8bits[4*l_i_Cnt+1] = ((i_x_Image.m_puc_Image[l_i_Cnt] & 0x30)<<2);
					l_puc_image8bits[4*l_i_Cnt+2] = ((i_x_Image.m_puc_Image[l_i_Cnt] & 0x0C)<<4);
					l_puc_image8bits[4*l_i_Cnt+3] = ((i_x_Image.m_puc_Image[l_i_Cnt] & 0x03)<<6);
					CORRECT_LEVEL2(l_puc_image8bits[4*l_i_Cnt])
					CORRECT_LEVEL2(l_puc_image8bits[4*l_i_Cnt+1])
					CORRECT_LEVEL2(l_puc_image8bits[4*l_i_Cnt+2])
					CORRECT_LEVEL2(l_puc_image8bits[4*l_i_Cnt+3])
				}
			}
			break;
		case 1:
			for (l_i_Cnt = 0; l_i_Cnt < l_i_ImageSize/8; l_i_Cnt++)
			{
				if (l_puc_image8bits!= NULL)
				{
					if ((l_puc_image8bits != NULL) && (i_x_Image.m_puc_Image != NULL))
					{
						l_puc_image8bits[8*l_i_Cnt]   = (UC)(i_x_Image.m_puc_Image[l_i_Cnt] & 0x80);
						l_puc_image8bits[8*l_i_Cnt+1] = (UC)((i_x_Image.m_puc_Image[l_i_Cnt] & 0x40)<<1);
						l_puc_image8bits[8*l_i_Cnt+2] = (UC)((i_x_Image.m_puc_Image[l_i_Cnt] & 0x20)<<2);
						l_puc_image8bits[8*l_i_Cnt+3] = (UC)((i_x_Image.m_puc_Image[l_i_Cnt] & 0x10)<<3);
						l_puc_image8bits[8*l_i_Cnt+4] = (UC)((i_x_Image.m_puc_Image[l_i_Cnt] & 0x08)<<4);
						l_puc_image8bits[8*l_i_Cnt+5] = (UC)((i_x_Image.m_puc_Image[l_i_Cnt] & 0x04)<<5);
						l_puc_image8bits[8*l_i_Cnt+6] = (UC)((i_x_Image.m_puc_Image[l_i_Cnt] & 0x02)<<6);
						l_puc_image8bits[8*l_i_Cnt+7] = (UC)((i_x_Image.m_puc_Image[l_i_Cnt] & 0x01)<<7);
						CORRECT_LEVEL1(l_puc_image8bits[8*l_i_Cnt])
						CORRECT_LEVEL1(l_puc_image8bits[8*l_i_Cnt+1])
						CORRECT_LEVEL1(l_puc_image8bits[8*l_i_Cnt+2])
						CORRECT_LEVEL1(l_puc_image8bits[8*l_i_Cnt+3])
						CORRECT_LEVEL1(l_puc_image8bits[8*l_i_Cnt+4])
						CORRECT_LEVEL1(l_puc_image8bits[8*l_i_Cnt+5])
						CORRECT_LEVEL1(l_puc_image8bits[8*l_i_Cnt+6])
						CORRECT_LEVEL1(l_puc_image8bits[8*l_i_Cnt+7])
					}
				}
			}
			break;
	}

	if (i_ul_imageFilter & MORPHO_LINEAR_INTERPOLATION)
	{
		ZoomImageBilineaire(l_puc_image8bits, (PUC)m_px_MorphoImage->m_pui_image8bits, 0,  
							i_x_Image.m_x_ImageHeader.m_us_NbCol,   i_x_Image.m_x_ImageHeader.m_us_NbRow, 1, 
							i_x_Image.m_x_ImageHeader.m_us_NbCol*2, i_x_Image.m_x_ImageHeader.m_us_NbRow*2, 2);
#ifdef _WIN32_WCE
		l_i_Ret = MonochromeArrayToDIB(i_ul_ImageIndex, i_x_Image.m_x_ImageHeader.m_us_NbRow*2-2, i_x_Image.m_x_ImageHeader.m_us_NbCol*2-2, (PUC)m_px_MorphoImage->m_pui_image8bits, i_p_Cdc, i_p_Rect);
#else
		l_i_Ret = MonochromeArrayToDIB(i_ul_ImageIndex, i_x_Image.m_x_ImageHeader.m_us_NbRow*2-2, i_x_Image.m_x_ImageHeader.m_us_NbCol*2-2, (PUC)m_px_MorphoImage->m_pui_image8bits, i_p_Cdc);
#endif
	}
	else
	{
#ifdef _WIN32_WCE
		l_i_Ret = MonochromeArrayToDIB(i_ul_ImageIndex, i_x_Image.m_x_ImageHeader.m_us_NbRow, i_x_Image.m_x_ImageHeader.m_us_NbCol, (PUC)m_px_MorphoImage->m_pui_image8bits, i_p_Cdc,i_p_Rect);
#else
		l_i_Ret = MonochromeArrayToDIB(i_ul_ImageIndex, i_x_Image.m_x_ImageHeader.m_us_NbRow, i_x_Image.m_x_ImageHeader.m_us_NbCol, (PUC)m_px_MorphoImage->m_pui_image8bits, i_p_Cdc);
#endif
	}

#ifndef _WIN32_WCE
	if (l_i_Ret == MORPHO_OK)
		l_i_Ret = RefreshViewer(i_ul_ImageIndex, i_p_Rect, i_p_Cdc);
#endif

	if (l_puc_image != NULL)
		free(l_puc_image);

	return l_i_Ret;
}

I C_MORPHO_Image::RefreshViewer(UL	i_ul_ImageIndex,
							  CRect &	i_p_Rect, 
							  CDC*	i_p_Cdc)
{
    HDC             l_phMemDC	=	NULL;
    BITMAP			l_Bitmap;
	UL				l_ul_ImageViewerRow;
	UL				l_ul_ImageViewerCol;
	double			l_d_temp;

	if (!m_px_MorphoImage)
		return MORPHOERR_INVALID_CLASS;

	if (m_px_MorphoImage->m_ul_ImageNbIndex <= i_ul_ImageIndex)
		return MORPHOERR_BADPARAMETER;

	if (m_px_MorphoImage->m_pBitmap!= NULL)
	{
		 GetObject(m_px_MorphoImage->m_pBitmap[i_ul_ImageIndex], sizeof(BITMAP), &l_Bitmap);

		 if (*i_p_Cdc != NULL)
		 {
			l_phMemDC = CreateCompatibleDC(i_p_Cdc->m_hDC);
		 }

		SelectObject(l_phMemDC, m_px_MorphoImage->m_pBitmap[i_ul_ImageIndex]);

		if(
			(i_p_Rect.Height() >= l_Bitmap.bmHeight) &&
			(i_p_Rect.Width() >= l_Bitmap.bmWidth)
			)
		{
			l_d_temp = __min((double)i_p_Rect.Height()/(double)l_Bitmap.bmHeight,(double)i_p_Rect.Width()/(double)l_Bitmap.bmWidth);
		}
		else
		{
			if(
				(i_p_Rect.Height() < l_Bitmap.bmHeight) &&
				(i_p_Rect.Width() < l_Bitmap.bmWidth)
			)
			{
				l_d_temp = __min((double)i_p_Rect.Height()/(double)l_Bitmap.bmHeight,(double)i_p_Rect.Width()/(double)l_Bitmap.bmWidth);
			}
			else if(i_p_Rect.Height() < l_Bitmap.bmHeight)
			{
				l_d_temp = (double)i_p_Rect.Height()/(double)l_Bitmap.bmHeight;				
			}
			else
			{
				l_d_temp = (double)i_p_Rect.Width()/(double)l_Bitmap.bmWidth;			
			}			
		}

		l_ul_ImageViewerRow = l_d_temp*l_Bitmap.bmHeight;
		l_ul_ImageViewerCol = l_d_temp*l_Bitmap.bmWidth;
			
#ifndef _WIN32_WCE
		if (i_p_Cdc != NULL)
		{
			i_p_Cdc->SetStretchBltMode(COLORONCOLOR);
		}
#endif

		StretchBlt(i_p_Cdc->m_hDC, (i_p_Rect.Width()-l_ul_ImageViewerCol)/2, (i_p_Rect.Height()-l_ul_ImageViewerRow)/2, l_ul_ImageViewerCol, l_ul_ImageViewerRow, 
				   l_phMemDC, 0, 0, l_Bitmap.bmWidth, l_Bitmap.bmHeight, SRCCOPY);

		if(l_phMemDC != NULL)
		{
			DeleteDC(l_phMemDC);
			l_phMemDC = NULL;
		}
	}

	return MORPHO_OK;
}

#ifdef _WIN32_WCE
	I C_MORPHO_Image::MonochromeArrayToDIB(	UL	   i_ul_ImageIndex,
											UL	   i_ul_ImageRow,
											UL	   i_ul_ImageCol,
											PUC    i_puc_Image,
											CDC	  *i_p_Cdc,
											CRect &i_p_Rect)
#else
	I C_MORPHO_Image::MonochromeArrayToDIB(	UL	i_ul_ImageIndex,
											UL	i_ul_ImageRow,
											UL	i_ul_ImageCol,
											PUC i_puc_Image,
											CDC	*i_p_Cdc)
#endif
{
   	INT		i;	
	PVOID	l_pv_Bits;

	if (!m_px_MorphoImage)
		return MORPHOERR_INVALID_CLASS;

	if (m_px_MorphoImage->m_ul_ImageNbIndex <= i_ul_ImageIndex)
		return MORPHOERR_BADPARAMETER;

    if (m_px_MorphoImage->m_pBitmap[i_ul_ImageIndex] != NULL)
	{
		DeleteObject(m_px_MorphoImage->m_pBitmap[i_ul_ImageIndex]);
		m_px_MorphoImage->m_pBitmap[i_ul_ImageIndex] = NULL;
	}

	//Info image (format bitmap)
    struct  
	{
         BITMAPINFOHEADER    bmiHeader;
         RGBQUAD             bmiColors[256];
	} l_InfoBitmap;

    //Init RGBQUAD Palette monochrome de 256 niveaux
    for(i = 0; i < 256; i++)
    {
		l_InfoBitmap.bmiColors[i].rgbBlue = i; 
        l_InfoBitmap.bmiColors[i].rgbGreen = i; 
        l_InfoBitmap.bmiColors[i].rgbRed = i; 
        l_InfoBitmap.bmiColors[i].rgbReserved = 0; 
    }

	//Init BITMAPINFOHEADER    
    l_InfoBitmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    l_InfoBitmap.bmiHeader.biWidth = i_ul_ImageCol;
    l_InfoBitmap.bmiHeader.biHeight = - (long)i_ul_ImageRow; /*the minus sign imply that */
												/*the bitmap is a top-down DIB and its origin is the upper left corner*/

    l_InfoBitmap.bmiHeader.biPlanes = 1;
    l_InfoBitmap.bmiHeader.biBitCount = 8;
    l_InfoBitmap.bmiHeader.biCompression = BI_RGB;
    l_InfoBitmap.bmiHeader.biSizeImage = 0;

    l_InfoBitmap.bmiHeader.biXPelsPerMeter = 0;
    l_InfoBitmap.bmiHeader.biYPelsPerMeter = 0;
    l_InfoBitmap.bmiHeader.biClrUsed = 0;
    l_InfoBitmap.bmiHeader.biClrImportant = 0;

	if (*	i_p_Cdc!= NULL)
	{
		m_px_MorphoImage->m_pBitmap[i_ul_ImageIndex] = CreateDIBSection(i_p_Cdc->m_hDC, (BITMAPINFO*)&l_InfoBitmap, DIB_RGB_COLORS, &l_pv_Bits, NULL, 0);
	}
#ifdef _WIN32_WCE
	StretchDIBits
		(
		i_p_Cdc->m_hDC,
		0,
		0,
		i_p_Rect.Width(),
		i_p_Rect.Height(),
		0,
		0,
		i_ul_ImageCol,
		i_ul_ImageRow,
		i_puc_Image,
		(BITMAPINFO*)&l_InfoBitmap,
		DIB_RGB_COLORS,
		SRCCOPY
		);
#else
	SetDIBits
		(
		i_p_Cdc->m_hDC, 
		m_px_MorphoImage->m_pBitmap[i_ul_ImageIndex], 
		0, 
		i_ul_ImageRow, 
		i_puc_Image, 
		(BITMAPINFO*)&l_InfoBitmap, 
		DIB_RGB_COLORS
		);
#endif

	return MORPHO_OK;
}




VOID C_MORPHO_Image::ZoomImageBilineaire(PUC entree, PUC sortie, UC background, 
									L larg_entree, L haut_entree, L reso_entree, 
									L larg_sortie, L haut_sortie, L reso_sortie)
{
	long            i, j, k, l;
	double          u0, u1, v0, v1, r, r0, r1, x, y, dx, dy;
	double          zz;
	if (reso_sortie != 0)
	{
		zz = (double) reso_entree / (double) reso_sortie;
	}
	double          x0 = (-(double) larg_sortie / (double) 2) * zz + (double) larg_entree / (double) 2;
	double          y0 = (-(double) haut_sortie / (double) 2) * zz + (double) haut_entree / (double) 2;
	unsigned char  *e = NULL;

	for (i = 0, y = y0; i < haut_sortie; i++, y += zz) {
		k = (long) floor(y);
		if (k < 0 || k > haut_entree - 2) {
			(void) memset(sortie, background, larg_sortie);
			sortie += larg_sortie;
		} else {
			dy = y - k;
			v0 = 1 - dy;
			v1 = dy;
			for (j = 0, x = x0; j < larg_sortie; j++, x += zz) {
				l = (long) floor(x);
				if (l < 0 || l > larg_entree - 2) {
					*sortie++ = background;
				} else {
					dx = x - l;
					u0 = 1 - dx;
					u1 = dx;
					e = entree + k * larg_entree + l;
					r0 = u0 * e[0] + u1 * e[1];
					e = e + larg_entree;
					r1 = u0 * e[0] + u1 * e[1];
					r = (long) (v0 * r0 + v1 * r1 + 0.5);
					r = (r < 0 ? 0 : r > 255 ? 255 : r);
					*sortie++ = (UC)r;
				}
			}
		}
	}
}


