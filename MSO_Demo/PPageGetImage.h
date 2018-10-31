/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

// ppagegetimage.h : header file
//

#if !defined(AFX_PPAGEGETIMAGE_H__57B9D910_39AF_4E68_BDBD_FAD595CAFD11__INCLUDED_)
#define AFX_PPAGEGETIMAGE_H__57B9D910_39AF_4E68_BDBD_FAD595CAFD11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CPPageGetImage dialog

class CPPageGetImage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPageGetImage)

// Construction
public:
	CPPageGetImage();
	~CPPageGetImage();

// Dialog Data
	//{{AFX_DATA(CPPageGetImage)
	enum { IDD = IDD_PPAGE_GET_IMAGE };
	int		m_i_DetectModeChoice;
	BOOL	m_b_GetImageLatentDetect;
	CComboBox	m_cmd_CompressionType;
	CString	    m_str_CompressionType;
	int		    m_i_CompressionType;
	int         m_i_CompressionRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPageGetImage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPageGetImage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCbnSelchangeCompressionType();
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPAGEGETIMAGE_H__57B9D910_39AF_4E68_BDBD_FAD595CAFD11__INCLUDED_)
