/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

// DrawTitle.h: interface for the CDrawTitle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWTITLE_H__565914E8_165A_4E81_A2E5_BDE601508E69__INCLUDED_)
#define AFX_DRAWTITLE_H__565914E8_165A_4E81_A2E5_BDE601508E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDrawTitle  
{
public:
	CDrawTitle();
	virtual ~CDrawTitle();
	static BOOL VFillGradientRect(CDC* pDC, CRect rect, COLORREF ColorBas, COLORREF ColorHaut );
	static void DrawVerticalTitle( CWnd* pWnd, CDC* pDC, CString titre, BOOL bDevMode); 

};

#endif // !defined(AFX_DRAWTITLE_H__565914E8_165A_4E81_A2E5_BDE601508E69__INCLUDED_)
