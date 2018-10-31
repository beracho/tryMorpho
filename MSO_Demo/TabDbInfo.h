#pragma once


// Boîte de dialogue TabDbInfo

class TabDbInfo : public CDialog
{
	DECLARE_DYNAMIC(TabDbInfo)

public:
	TabDbInfo(CWnd* pParent = NULL);   // constructeur standard
	virtual ~TabDbInfo();
	void SetParam(CString p_cs_paramName, CString p_cs_paramValue); 

// Données de boîte de dialogue
	enum { IDD = IDD_TAB_DB_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV


public:
	CString m_cs_MaxNbRecords;
	CString m_CurrentNbRecords;
	CString m_NbFingers;
	CString m_cs_encryptDB;
};
