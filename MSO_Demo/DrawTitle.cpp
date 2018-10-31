/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// DrawTitle.cpp: implementation of the CDrawTitle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mso_demo.h"
#include "DrawTitle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawTitle::CDrawTitle()
{

}

CDrawTitle::~CDrawTitle()
{

}

BOOL CDrawTitle::VFillGradientRect(CDC* pDC, CRect rect, COLORREF ColorBas, COLORREF ColorHaut)
{
	int			i;
	int			nFrame;
	int			vRR, vRRh, vRRtt;
	int			vGG, vGGh, vGGtt;
	int			vBB, vBBh, vBBtt;
	CRect		PaintRect;
	CBrush		PaintBrush;
	CBrush*		pOldBrush;

	if (rect.Height() < 64)		
		nFrame = 32;
	else
		nFrame = 64;

	PaintRect.CopyRect( &rect);
	vRRh = GetRValue( ColorHaut);
	vGGh = GetGValue( ColorHaut);
	vBBh = GetBValue( ColorHaut);
	vRRtt = GetRValue( ColorBas) - vRRh;
	vGGtt = GetGValue( ColorBas) - vGGh;
	vBBtt = GetBValue( ColorBas) - vBBh;
	// Premiere Etape
	PaintRect.bottom = rect.top + MulDiv( rect.Height(), 1, nFrame);
	PaintBrush.CreateSolidBrush( ColorHaut);
	pOldBrush = pDC->SelectObject( &PaintBrush);
	pDC->FillRect( &PaintRect, &PaintBrush);
	pDC->SelectObject( pOldBrush);
	PaintBrush.DeleteObject();
	// Etape
	for (i = 1;i < nFrame - 1; i++)
	{
		PaintRect.top = rect.top + MulDiv( rect.Height(), i, nFrame);
		PaintRect.bottom = rect.top + MulDiv( rect.Height(), i + 1, nFrame);
		vRR = vRRh + MulDiv( vRRtt, i, nFrame);
		vGG = vGGh + MulDiv( vGGtt, i, nFrame);
		vBB = vBBh + MulDiv( vBBtt, i, nFrame);
		PaintBrush.CreateSolidBrush( RGB( vRR, vGG, vBB));
		pOldBrush = pDC->SelectObject( &PaintBrush);
		pDC->FillRect( &PaintRect, &PaintBrush);
		pDC->SelectObject( pOldBrush);
		PaintBrush.DeleteObject();
	}
	// Derniere Etape
	PaintRect.top = rect.top + MulDiv( rect.Height(), nFrame - 1, nFrame);
	PaintRect.bottom = rect.bottom;
	PaintBrush.CreateSolidBrush( ColorBas);
	pOldBrush = pDC->SelectObject( &PaintBrush);
	pDC->FillRect( &PaintRect, &PaintBrush);
	pDC->SelectObject( pOldBrush);
	PaintBrush.DeleteObject();
	return TRUE;
}

void CDrawTitle::DrawVerticalTitle( CWnd* pWnd, CDC* pDC, CString titre, BOOL bDevMode) 
{
	CFont		font, *OldFont;
	CBrush		brush, *OldBrush;
	CPen		pen, *OldPen;
	CRect		rect,Rect2;
	int			OldBkMode;
	COLORREF	gris;

	pWnd->GetClientRect( &rect);
	// Creation des outils de dessin
	gris = RGB(128, 128, 128);
	font.CreateFont(20, 0, 900, 0, FW_EXTRABOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, "Arial");
	brush.CreateSolidBrush( gris);
	pen.CreatePen( PS_SOLID, 0, gris);

	OldBrush = (CBrush*) pDC->SelectObject( &brush);
	OldPen = (CPen*) pDC->SelectObject( &pen);
	OldFont = (CFont*) pDC->SelectObject( &font);

	if (bDevMode)
	{
		Rect2 = CRect::CRect( 0, 1, 26, rect.bottom);
		VFillGradientRect( pDC, Rect2, RGB( 0, 128, 128),  RGB( 128, 0, 128));
	}
	else
	{
		Rect2 = CRect::CRect( 0, 1, 26, rect.bottom);
		VFillGradientRect( pDC, Rect2, RGB( 0, 0, 255),  RGB( 48, 48, 48));
	}

	OldBkMode = pDC->SetBkMode( TRANSPARENT);
	pDC->SetBkColor(gris);
	pDC->SetTextColor(RGB(255, 255, 255)); // blanc
	pDC->TextOut(2, rect.bottom-7, titre);

	// RAZ du contexte
	pDC->SetBkMode( OldBkMode);
	pDC->SelectObject(OldFont);
	pDC->SelectObject(OldBrush);
	pDC->SelectObject(OldPen);
}