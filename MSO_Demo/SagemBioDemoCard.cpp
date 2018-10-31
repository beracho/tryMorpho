/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// SagemBioDemoCard.cpp: implementation of the CSagemBioDemoCard class.
//
//This class manage the Morpho OPUCE card personalized for biometric demo card, and
//structured as described below :
//
//				MF
//		 _______|________
//		|				 |
//	   EFpk			  EFalpha
//
//The data used for this demo are :
// - PK1 and PK2 saved in EFpks
// - Identifier, Surname and Name saved in EFalpha
//
//There is 2 masks for the card :
//	- the first does not have access conditions restricitions for the EF (Update/Read/Write always)
//	- the second is Read Always / Write Never and an external authentication is required 
//	for update binary
// This demo will manage both cards.

//!!!!! Restrictions !!!!!!!
//This source code does not manage extended APDU length


//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MORPHO_Interface.h" //SDK types definition
#include "SagemBioDemoCard.h"
#include "SmartCard_Error.h"
#include "MSOSECU.h"

// Smart card mapping
//--------------------
#define ID_MF					0x3F00
#define ID_EF_PKS				0x0100
#define ID_EF_ALPHA				0x0200

#define EF_PKS_OFFSET_FIELD_SIZE_PK1			6
#define EF_PKS_OFFSET_FIELD_SIZE_PK2			8
#define EF_PKS_FIELDLEN_SIZE_PK					2

#define EF_PKS_OFFSET_BUFFER_PK1				10
#define EF_PKS_OFFSET_BUFFER_PK2				522
#define EF_PKS_FIELDLEN_BUFFER_PK				MAX_PK_SIZE

#define EF_ALPHA_OFFSET_IDENTIFIER				0
#define EF_ALPHA_FIELDLEN_IDENTIFIER			20
#define EF_ALPHA_OFFSET_NAME					30
#define EF_ALPHA_FIELDLEN_NAME					60
#define EF_ALPHA_OFFSET_SURNAME					90
#define EF_ALPHA_FIELDLEN_SURNAME				30

#define NUMBER_OK_KEY	5
PC	g_c_key_value[NUMBER_OK_KEY]={
	"","admkey_1","admkey_2","admkey_3","admkey_4"};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSagemBioDemoCard::CSagemBioDemoCard() : CBioDemoCard(SCARD_PROTOCOL_T0)
{
}

CSagemBioDemoCard::~CSagemBioDemoCard()
{
}

//////////////////////////////////////////////////////////////////
I	CSagemBioDemoCard::Read(PT_CBioDemoCard_USER_DATA o_px_user_data,
							T_SMART_CARD_IHM_CALLBACK i_x_IHM_Callback,
							PVOID	i_pv_context,
							UC		i_uc_TemplateNumber )
{
	I	l_i_Ret;
	UC		l_auc_Buff[256];
	T__BUF	l_x_Buff;

	l_x_Buff.m_puc_Buf = l_auc_Buff;
	l_x_Buff.m_ul_Size = sizeof ( l_auc_Buff );

//init output data
	memset(o_px_user_data->m_puc_PK1,0,MAX_PK_SIZE);
	o_px_user_data->m_us_PK1_Size = 0;
	memset(o_px_user_data->m_puc_PK2,0,MAX_PK_SIZE);
	o_px_user_data->m_us_PK2_Size = 0;
	memset(o_px_user_data->m_puc_Identifier,0,MAX_IDENTIFIER_SIZE);
	memset(o_px_user_data->m_puc_Surname,0,MAX_SURNAME_SIZE);
	memset(o_px_user_data->m_puc_Name,0,MAX_NAME_SIZE);

//init ihm range
	i_x_IHM_Callback(0,8,i_pv_context);

	l_i_Ret = Select(ID_MF,&l_x_Buff);
//set step
	i_x_IHM_Callback(1,1,i_pv_context);

	if ( i_uc_TemplateNumber >= 1 )
	{
	//Read PKs
	//select EF Pks	
		if (l_i_Ret == SMARTCARD_OK)
		{
	//select master file
			l_x_Buff.m_ul_Size = sizeof ( l_auc_Buff );

			l_i_Ret = Select(ID_EF_PKS,&l_x_Buff);
	//set step
			i_x_IHM_Callback(1,1,i_pv_context);
		}

	//Read PK1
		if (l_i_Ret == SMARTCARD_OK)
		{
			l_i_Ret = ReadPK(0,
							o_px_user_data->m_puc_PK1,
							&o_px_user_data->m_us_PK1_Size,
							MAX_PK_SIZE);
	//set step
			i_x_IHM_Callback(1,1,i_pv_context);
		}
	}

	if ( i_uc_TemplateNumber == 2 )
	{
	//Read PK2
		if (l_i_Ret == SMARTCARD_OK)
		{
			l_i_Ret = ReadPK(1,
							o_px_user_data->m_puc_PK2,
							&o_px_user_data->m_us_PK2_Size,
							MAX_PK_SIZE);
	//set step
			i_x_IHM_Callback(1,1,i_pv_context);
		}
	}

//Read Data
//select EF Alpha	
	if (l_i_Ret == SMARTCARD_OK)
	{
		l_x_Buff.m_ul_Size = sizeof ( l_auc_Buff );

		l_i_Ret = Select(ID_EF_ALPHA,&l_x_Buff);
//set step
		i_x_IHM_Callback(1,1,i_pv_context);
	}
	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = ReadBinary(EF_ALPHA_OFFSET_IDENTIFIER,o_px_user_data->m_puc_Identifier,MAX_IDENTIFIER_SIZE);
//set step
		i_x_IHM_Callback(1,1,i_pv_context);
	}
	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = ReadBinary(EF_ALPHA_OFFSET_NAME,o_px_user_data->m_puc_Name,MAX_NAME_SIZE);
//set step
		i_x_IHM_Callback(1,1,i_pv_context);
	}
	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = ReadBinary(EF_ALPHA_OFFSET_SURNAME,o_px_user_data->m_puc_Surname,MAX_SURNAME_SIZE);
//set step
		i_x_IHM_Callback(1,1,i_pv_context);
	}

//reset range
	i_x_IHM_Callback(0,0,i_pv_context);

	return l_i_Ret;
}


//////////////////////////////////////////////////////////////////
I	CSagemBioDemoCard::ReadPK(UC i_uc_PKNum, PUC o_puc_PK, PUS o_pus_PKSize, US i_us_MaxPKSize)
{
	I	l_i_Ret;
	US	l_us_PKSize;
	US	l_us_EFPK_Buffer_Offset;
	US	l_us_EFPK_Size_Offset;

	*o_pus_PKSize = 0;
	memset(o_puc_PK,0,i_us_MaxPKSize);

	switch (i_uc_PKNum)
	{
	case 0:
		l_us_EFPK_Buffer_Offset = EF_PKS_OFFSET_BUFFER_PK1;
		l_us_EFPK_Size_Offset = EF_PKS_OFFSET_FIELD_SIZE_PK1;
		break;
	case 1:
		l_us_EFPK_Buffer_Offset = EF_PKS_OFFSET_BUFFER_PK2;
		l_us_EFPK_Size_Offset = EF_PKS_OFFSET_FIELD_SIZE_PK2;
		break;
	default:
		return SMARTCARD_BADPARAMETER;
		break;
	}

//Get PK size
	l_i_Ret = ReadBinary(l_us_EFPK_Size_Offset,(PUC)&l_us_PKSize,EF_PKS_FIELDLEN_SIZE_PK);

	if (l_i_Ret == SMARTCARD_OK)
	{	
//PK size has been written in big endian (Morphotouch format) => convert to litel endina
		*o_pus_PKSize = ((l_us_PKSize&0xFF00)>>8) + ((l_us_PKSize&0x00FF)<<8);
		if (*o_pus_PKSize > i_us_MaxPKSize)
			return SMARTCARD_BAD_PK_BUFFER_SIZE;

//Get PK
		if (*o_pus_PKSize!=0)
			l_i_Ret = ReadBinary(l_us_EFPK_Buffer_Offset,(PUC)o_puc_PK,*o_pus_PKSize);
	}

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSagemBioDemoCard::UpdatePK(UC i_uc_PKNum, PUC i_puc_PK, US i_us_PKSize)
{
	I	l_i_Ret;
	US	l_us_PKSize;
	US	l_us_EFPK_Buffer_Offset;
	US	l_us_EFPK_Size_Offset;

	switch (i_uc_PKNum)
	{
	case 0:
		l_us_EFPK_Buffer_Offset = EF_PKS_OFFSET_BUFFER_PK1;
		l_us_EFPK_Size_Offset = EF_PKS_OFFSET_FIELD_SIZE_PK1;
		break;
	case 1:
		l_us_EFPK_Buffer_Offset = EF_PKS_OFFSET_BUFFER_PK2;
		l_us_EFPK_Size_Offset = EF_PKS_OFFSET_FIELD_SIZE_PK2;
		break;
	default:
		return SMARTCARD_BADPARAMETER;
		break;
	}

//PK size has been written in big endian (Morphotouch format) => convert to big endian
	if (i_us_PKSize > EF_PKS_FIELDLEN_BUFFER_PK)
		return SMARTCARD_BAD_PK_BUFFER_SIZE;
	l_us_PKSize = ((i_us_PKSize&0xFF00)>>8) + ((i_us_PKSize&0x00FF)<<8);

//Get PK size
	l_i_Ret = UpdateBinary(l_us_EFPK_Size_Offset,(PUC)&l_us_PKSize,EF_PKS_FIELDLEN_SIZE_PK);

//Get PK
	l_i_Ret = UpdateBinary(l_us_EFPK_Buffer_Offset,(PUC)i_puc_PK,i_us_PKSize);

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSagemBioDemoCard::Update(PT_CBioDemoCard_USER_DATA i_px_user_data,
							T_SMART_CARD_IHM_CALLBACK i_x_IHM_Callback,
							PVOID	i_pv_context)
{
	I	l_i_Ret;
	UC	l_uACByte;
	T__RANDOM	l_x_rand;
	T__RANDOM	l_x_encrypted_rand;
	UC		l_auc_Buff[256];
	T__BUF	l_x_Buff;

	l_x_Buff.m_puc_Buf = l_auc_Buff;
	l_x_Buff.m_ul_Size = sizeof ( l_auc_Buff );

	i_x_IHM_Callback(0,8,i_pv_context);

//select master file
	l_x_Buff.m_ul_Size = sizeof ( l_auc_Buff );

	l_i_Ret = Select(ID_MF,&l_x_Buff);
	i_x_IHM_Callback(1,1,i_pv_context);

//Read PKs
//select EF Pks	
	if (l_i_Ret == SMARTCARD_OK)
	{
	l_x_Buff.m_ul_Size = sizeof ( l_auc_Buff );
		l_i_Ret = Select(ID_EF_PKS,&l_x_Buff);
	}
	i_x_IHM_Callback(1,1,i_pv_context);

//Check security access condition
	//check 12th bytes of FCI file as described in opuce documentation
	l_uACByte = l_x_Buff.m_puc_Buf[12];
	if (l_uACByte==0x02)
	{
		//Update is protected by external authentication
		l_i_Ret = GetChallenge(&l_x_rand);
		if (l_i_Ret == SMARTCARD_OK)
		{
			//crypt returned random 
			l_i_Ret = MSOSECU_DesCrypt(g_c_key_value[2], 
								sizeof(l_x_rand.m_puc_data),
								l_x_rand.m_puc_data,
								l_x_encrypted_rand.m_puc_data);
		}
		if (l_i_Ret == SMARTCARD_OK)
		{
			l_i_Ret = ExternalAuthenticate(&l_x_encrypted_rand, 2);
		}	
	}

//Update PK1
	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = UpdatePK(0,
						i_px_user_data->m_puc_PK1,
						i_px_user_data->m_us_PK1_Size);
		i_x_IHM_Callback(1,1,i_pv_context);
	}
//Update PK2
	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = UpdatePK(1,
						i_px_user_data->m_puc_PK2,
						i_px_user_data->m_us_PK2_Size);
		i_x_IHM_Callback(1,1,i_pv_context);
	}

//Update Data
//select EF Alpha	
	if (l_i_Ret == SMARTCARD_OK)
	{
		l_x_Buff.m_ul_Size = sizeof ( l_auc_Buff );
		l_i_Ret = Select(ID_EF_ALPHA,&l_x_Buff);
		i_x_IHM_Callback(1,1,i_pv_context);
	}

	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = UpdateBinary(EF_ALPHA_OFFSET_IDENTIFIER,i_px_user_data->m_puc_Identifier,MAX_IDENTIFIER_SIZE);
		i_x_IHM_Callback(1,1,i_pv_context);
	}
	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = UpdateBinary(EF_ALPHA_OFFSET_NAME,i_px_user_data->m_puc_Name,MAX_NAME_SIZE);
		i_x_IHM_Callback(1,1,i_pv_context);
	}
	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = UpdateBinary(EF_ALPHA_OFFSET_SURNAME,i_px_user_data->m_puc_Surname,MAX_SURNAME_SIZE);
		i_x_IHM_Callback(1,1,i_pv_context);
	}

//reset range
		i_x_IHM_Callback(0,0,i_pv_context);

	return l_i_Ret;
}


