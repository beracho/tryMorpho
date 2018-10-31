/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// GB_BioDemoCard.cpp: implementation of the CGB_BioDemoCard class.
//
//This class manage the SAGEM OPUCE card personalized for biometric demo card, and
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
#include "GB_BioDemoCard.h"
#include "SmartCard_Error.h"
#include "MSOSECU.h"

// Smart card mapping
//--------------------
#define ID_MF					0x3F00
#define ID_DF2					0x1200
#define ID_DFBIO				0x1F07

#define ID_CHIP_ID				0x0181

#define ID_MK1_ID				0x05
#define ID_MK2_ID				0x06
#define ID_MK3_ID				0x07

#define ID_EF_BIO_TEMPLATE1		0x1000
#define ID_EF_BIO_TEMPLATE2		0x1001

#define EF_PKS_FIELDLEN_SIZE_PK			2
#define EF_PKS_FIELDLEN_BUFFER_PK		(2*PKMAT_MAXSIZE)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UC	g_uc_MK1[DES_TRIPLE_KEY_LEN]={ 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
UC	g_uc_MK2[DES_TRIPLE_KEY_LEN]={ 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33 };
UC	g_uc_MK3[DES_TRIPLE_KEY_LEN]={ 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55 };


CGB_BioDemoCard::CGB_BioDemoCard() : CBioDemoCard ( SCARD_PROTOCOL_T1 )
{
}

CGB_BioDemoCard::~CGB_BioDemoCard()
{
}

I	CGB_BioDemoCard::CalcAMasterKey ( PUC i_puc_ChipId, PUC i_puc_MK, UC i_uc_KeyID, PT_MASTERKEY o_px_MasterKey )
{
	I			l_i_Ret;
	UC			l_auc_A[DES_BLOCKSIZE];
	UC			l_auc_B[DES_BLOCKSIZE];

	l_i_Ret = SMARTCARD_OK;

	
		l_i_Ret = MSOSECU_TripleDesCryptByKey ( i_puc_MK, DES_BLOCKSIZE, i_puc_ChipId, l_auc_A );
	

	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = MSOSECU_TripleDesCryptByKey ( i_puc_MK, DES_BLOCKSIZE, i_puc_ChipId+DES_BLOCKSIZE, l_auc_B );
	}

	if (l_i_Ret == SMARTCARD_OK)
	{
		memcpy ( o_px_MasterKey->m_auc_Key, l_auc_A, DES_BLOCKSIZE );
		memcpy ( o_px_MasterKey->m_auc_Key+DES_BLOCKSIZE, l_auc_B, DES_BLOCKSIZE );
		memcpy ( o_px_MasterKey->m_auc_Key+DES_BLOCKSIZE+DES_BLOCKSIZE, l_auc_A, DES_BLOCKSIZE );
		o_px_MasterKey->m_uc_ID = i_uc_KeyID;
	}

	return l_i_Ret;
}

I	CGB_BioDemoCard::DoExternalAuthenticate ( PT_MASTERKEY i_px_MasterKey )
{
	I			l_i_Ret;
	T__RANDOM	l_x_rand;
	T__RANDOM	l_x_encrypted_rand;

	l_i_Ret = SMARTCARD_OK;


		l_i_Ret = GetChallenge ( &l_x_rand );


	if (l_i_Ret == SMARTCARD_OK)
	{
		//crypt returned random 
		l_i_Ret = MSOSECU_TripleDesCryptByKey( i_px_MasterKey->m_auc_Key, 
							sizeof(l_x_rand.m_puc_data),
							l_x_rand.m_puc_data,
							l_x_encrypted_rand.m_puc_data);
	}

	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = ExternalAuthenticate ( &l_x_encrypted_rand, i_px_MasterKey->m_uc_ID );
	}

	return l_i_Ret;
}

I	CGB_BioDemoCard::InitSmcAndClacMasterKeys ( )
{
	I			l_i_Ret;
	UC			l_auc_ChipID[256];
	UC			l_auc_Buff[2*DES_BLOCKSIZE];
	T__BUF		l_x_Buff;

//select master file
	l_x_Buff.m_puc_Buf = l_auc_Buff;
	l_x_Buff.m_ul_Size = sizeof ( l_auc_Buff );

	l_i_Ret = Select(ID_MF,&l_x_Buff);

	if (l_i_Ret == SMARTCARD_OK)
	{
	//Get ChipID
		l_x_Buff.m_puc_Buf = l_auc_ChipID;
		l_x_Buff.m_ul_Size = sizeof ( l_auc_ChipID );
		l_i_Ret = GetData ( ID_CHIP_ID, &l_x_Buff );
	}

	if (l_i_Ret == SMARTCARD_OK)
	{
	//select ID_DF2
		l_x_Buff.m_puc_Buf = l_auc_Buff;
		l_x_Buff.m_ul_Size = sizeof ( l_auc_Buff );

		l_i_Ret = Select(ID_DF2,&l_x_Buff);
	}

	if (l_i_Ret == SMARTCARD_OK)
	{
	//select ID_DFBIO
		l_x_Buff.m_puc_Buf = l_auc_Buff;
		l_x_Buff.m_ul_Size = sizeof ( l_auc_Buff );

		l_i_Ret = Select(ID_DFBIO,&l_x_Buff);
	}

	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = CalcAMasterKey ( l_auc_ChipID, g_uc_MK1, ID_MK1_ID, &m_x_MasterK1 ); 
	}

	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = CalcAMasterKey ( l_auc_ChipID, g_uc_MK2, ID_MK2_ID, &m_x_MasterK2 ); 
	}

	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = CalcAMasterKey ( l_auc_ChipID, g_uc_MK3, ID_MK3_ID, &m_x_MasterK3 ); 
	}

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CGB_BioDemoCard::Read(PT_CBioDemoCard_USER_DATA o_px_user_data,
							T_SMART_CARD_IHM_CALLBACK i_x_IHM_Callback,
							PVOID	i_pv_context,
							UC		i_uc_TemplateNumber )
{
	I			l_i_Ret;

//init output data
	memset(o_px_user_data->m_puc_PK1,0,MAX_PK_SIZE);
	o_px_user_data->m_us_PK1_Size = 0;
	memset(o_px_user_data->m_puc_PK2,0,MAX_PK_SIZE);
	o_px_user_data->m_us_PK2_Size = 0;
	memset(o_px_user_data->m_puc_Identifier,0,MAX_IDENTIFIER_SIZE);
	memset(o_px_user_data->m_puc_Surname,0,MAX_SURNAME_SIZE);
	memset(o_px_user_data->m_puc_Name,0,MAX_NAME_SIZE);

//init ihm range
	if ( i_x_IHM_Callback != NULL )
		i_x_IHM_Callback(0,8,i_pv_context);

	l_i_Ret = InitSmcAndClacMasterKeys ();
//set step
	if ( i_x_IHM_Callback != NULL )
		i_x_IHM_Callback(1,1,i_pv_context);

	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = DoExternalAuthenticate ( &m_x_MasterK3 );
	}

//set step
	if ( i_x_IHM_Callback != NULL )
		i_x_IHM_Callback(1,1,i_pv_context);

	if ( i_uc_TemplateNumber >= 1 )
	{
		if (l_i_Ret == SMARTCARD_OK)
		{
			l_i_Ret = ReadPK ( ID_EF_BIO_TEMPLATE1,
							o_px_user_data->m_puc_PK1,
							&o_px_user_data->m_us_PK1_Size,
							MAX_PK_SIZE);
		}
	}

//set step
	if ( i_x_IHM_Callback != NULL )
		i_x_IHM_Callback(1,1,i_pv_context);

	if ( i_uc_TemplateNumber == 2 )
	{
		if (l_i_Ret == SMARTCARD_OK)
		{
			l_i_Ret = ReadPK ( ID_EF_BIO_TEMPLATE2,
							o_px_user_data->m_puc_PK2,
							&o_px_user_data->m_us_PK2_Size,
							MAX_PK_SIZE);
		}
	}

//set step
	if ( i_x_IHM_Callback != NULL )
		i_x_IHM_Callback(1,1,i_pv_context);

//reset range
	if ( i_x_IHM_Callback != NULL )
		i_x_IHM_Callback(0,0,i_pv_context);

	return l_i_Ret;
}


//////////////////////////////////////////////////////////////////
I	CGB_BioDemoCard::ReadPK ( US i_us_EF_TemplateNum, PUC o_puc_PK, PUS o_pus_PKSize, US i_us_MaxPKSize)
{

	I			l_i_Ret;
	UC			l_auc_Buff[MAX_PK_SIZE];
	T__BUF		l_x_Buff;
	US			l_us_PKSize;

	l_i_Ret = SMARTCARD_OK;

	
	//select EF Pks	
		l_x_Buff.m_puc_Buf = l_auc_Buff;
		l_x_Buff.m_ul_Size = sizeof ( l_auc_Buff );

		l_i_Ret = Select ( i_us_EF_TemplateNum, &l_x_Buff );
	

//Get PK size
	l_i_Ret = ReadBinary ( 0, (PUC)&l_us_PKSize, EF_PKS_FIELDLEN_SIZE_PK );

	if (l_i_Ret == SMARTCARD_OK)
	{	
//PK size has been written in big endian (Morphotouch format) => convert to litel endina
		*o_pus_PKSize = ((l_us_PKSize&0xFF00)>>8) + ((l_us_PKSize&0x00FF)<<8);
		if ( *o_pus_PKSize > i_us_MaxPKSize )
		{
			return SMARTCARD_BAD_PK_BUFFER_SIZE;
		}

//Get PK
		if ( *o_pus_PKSize != 0 )
		{
			l_i_Ret = ReadBinary ( EF_PKS_FIELDLEN_SIZE_PK, (PUC)o_puc_PK, *o_pus_PKSize );
		}
	}
	

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CGB_BioDemoCard::Update ( PT_CBioDemoCard_USER_DATA i_px_user_data,
							T_SMART_CARD_IHM_CALLBACK i_x_IHM_Callback,
							PVOID	i_pv_context)
{
	I			l_i_Ret;

//init ihm range
	if ( i_x_IHM_Callback != NULL )
		i_x_IHM_Callback(0,8,i_pv_context);

	l_i_Ret = InitSmcAndClacMasterKeys ();
//set step
	if ( i_x_IHM_Callback != NULL )
		i_x_IHM_Callback(1,1,i_pv_context);

	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = DoExternalAuthenticate ( &m_x_MasterK2 );
	}

//set step
	if ( i_x_IHM_Callback != NULL )
		i_x_IHM_Callback(1,1,i_pv_context);

	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = UpdatePK ( ID_EF_BIO_TEMPLATE1,
						i_px_user_data->m_puc_PK1,
						i_px_user_data->m_us_PK1_Size);
	}

//set step
	if ( i_x_IHM_Callback != NULL )
		i_x_IHM_Callback(1,1,i_pv_context);

	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = UpdatePK ( ID_EF_BIO_TEMPLATE2,
						i_px_user_data->m_puc_PK2,
						i_px_user_data->m_us_PK2_Size);
	}

//set step
	if ( i_x_IHM_Callback != NULL )
		i_x_IHM_Callback(1,1,i_pv_context);

//reset range
	if ( i_x_IHM_Callback != NULL )
		i_x_IHM_Callback(0,0,i_pv_context);

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CGB_BioDemoCard::UpdatePK( US i_us_EF_TemplateNum, PUC i_puc_PK, US i_us_PKSize)
{
	I			l_i_Ret;
	UC			l_auc_Buff[MAX_PK_SIZE];
	T__BUF		l_x_Buff;

	l_i_Ret = SMARTCARD_OK;

	
	//select EF Pks	
		l_x_Buff.m_puc_Buf = l_auc_Buff;
		l_x_Buff.m_ul_Size = sizeof ( l_auc_Buff );
		l_i_Ret = Select ( i_us_EF_TemplateNum, &l_x_Buff );
	

//Get PK
	l_i_Ret = UpdateBinary ( 0, (PUC)i_puc_PK, i_us_PKSize) ;
	
	return l_i_Ret;
}

