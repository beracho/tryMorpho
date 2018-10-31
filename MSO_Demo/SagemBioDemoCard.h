/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

// SagemBioDemoCard.h: interface for the CSagemBioDemoCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAGEMBIODEMOCARD_H__0A608CAF_869E_46B5_955C_D5D4DA13CE43__INCLUDED_)
#define AFX_SAGEMBIODEMOCARD_H__0A608CAF_869E_46B5_955C_D5D4DA13CE43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SCardReader.h"

class CSagemBioDemoCard : public CBioDemoCard
{
public:
	CSagemBioDemoCard();
	virtual ~CSagemBioDemoCard();


	I	Read(PT_CBioDemoCard_USER_DATA o_px_user_data,T_SMART_CARD_IHM_CALLBACK i_x_IHM_Callback,PVOID i_x_Context,UC i_uc_TemplateNumber);
	I	Update(PT_CBioDemoCard_USER_DATA i_px_user_data,T_SMART_CARD_IHM_CALLBACK i_x_IHM_Callback,PVOID i_x_Context);

private:

	I	ReadPK(UC i_uc_PKNum, PUC o_puc_PK, PUS o_pus_PKSize, US i_us_MaxPKSize);
	I	UpdatePK(UC i_uc_PKNum, PUC i_puc_PK, US i_us_PKSize);

	
};

#endif // !defined(AFX_SAGEMBIODEMOCARD_H__0A608CAF_869E_46B5_955C_D5D4DA13CE43__INCLUDED_)
