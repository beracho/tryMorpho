/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

// GB_BioDemoCard.h: interface for the CGB_BioDemoCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAGEMBIODEMOCARD_H__0A608CAF_869E_46B5_956C_D5D4DA13CE43__INCLUDED_)
#define AFX_SAGEMBIODEMOCARD_H__0A608CAF_869E_46B5_956C_D5D4DA13CE43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SCardReader.h"

#define DES_DOUBLE_KEY_LEN					16
#define DES_TRIPLE_KEY_LEN					24
#define DES_BLOCKSIZE						8

class CGB_BioDemoCard : public CBioDemoCard
{
public:
	CGB_BioDemoCard();
	virtual ~CGB_BioDemoCard();

	I	Read(PT_CBioDemoCard_USER_DATA o_px_user_data,T_SMART_CARD_IHM_CALLBACK i_x_IHM_Callback,PVOID i_x_Context, UC i_uc_TemplateNumber);
	I	Update(PT_CBioDemoCard_USER_DATA i_px_user_data,T_SMART_CARD_IHM_CALLBACK i_x_IHM_Callback,PVOID i_x_Context);
	
private:
	typedef struct
	{
		UC	m_auc_Key[DES_TRIPLE_KEY_LEN];
		UC	m_uc_ID;
	} T_MASTERKEY, *PT_MASTERKEY;

	I	ReadPK ( US i_us_EF_TemplateNum, PUC o_puc_PK, PUS o_pus_PKSize, US i_us_MaxPKSize);
	I	UpdatePK( US i_us_EF_TemplateNum, PUC i_puc_PK, US i_us_PKSize);

	I	DoExternalAuthenticate ( PT_MASTERKEY i_px_MasterKey );
	I	CalcAMasterKey ( PUC i_puc_ChipId, PUC i_puc_MK, UC i_uc_KeyID, PT_MASTERKEY o_px_MasterKey );
	I	InitSmcAndClacMasterKeys ( );

	T_MASTERKEY	m_x_MasterK1;
	T_MASTERKEY	m_x_MasterK2;
	T_MASTERKEY	m_x_MasterK3;

};

#endif // !defined(AFX_SAGEMBIODEMOCARD_H__0A608CAF_869E_46B5_956C_D5D4DA13CE43__INCLUDED_)
