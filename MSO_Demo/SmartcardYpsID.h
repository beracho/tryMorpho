#ifndef __SmartCardYpsID_h__
#define __SmartCardYpsID_h__

#if _MSC_VER > 1000
#   pragma once
#endif // _MSC_VER > 1000

#include "MORPHO_Types.h"
#include "MORPHO_Errors.h"
#include "SCardReader.h"

//Messa-
#define RESULT_OK		0
#define RESULT_NOK		1

#define ACTION_SELECT_APPLET_PKI		0x01
#define ACTION_GET_SERIALNUMBER			0x02
#define ACTION_VERIFY_PIN_ADMIN			0x04
//#define ACTION_SEND_REAL_BIO			0x10
//#define ACTION_SEND_D2C_BIO				0x20
//#define ACTION_END_BIO_ENROLL			0x40

typedef struct tagSmartCardCtx
{
	BYTE			ddScardSerialNumber[ 32];
	DWORD			dwScardSerialNumberLength;
}SmartCardCtx;

void Update_ApduMaxLength(I i_i_ApduMaxLength);

int Tool_MorphoParametersD2C_Free(	T_MORPHO_MOC_APDU_PARAMETERS* p_psDest);

int Tool_MorphoParametersD2C_Duplicate( T_MORPHO_MOC_APDU_PARAMETERS*			p_psDest,
										const T_MORPHO_MOC_APDU_PARAMETERS*		p_psSrc);

///////////////////////////////////////////////////////////////
// ypsID S2 tools
//

BOOL Tool_ypsIDS2_FillParametersD2C ( T_MORPHO_MOC_APDU_PARAMETERS*	p_psD2Cparams, 
									  const BYTE*					p_ddBioTemplate,		
									  const DWORD					p_dwBioTemplateLength, 
									  const int						p_nIndexBIO_ToSet,	
									  const int						p_byPkRefFingerInfos,
									  const BYTE*					p_ddCardSerialNumber, 
									  const DWORD					p_dwCardSerialNumberLength, 
									  const BOOL					p_bEnroll);

BOOL Tool_ypsIDS2_SCard_Action( CSCardReader* p_poCardReader, SmartCardCtx* p_psCardCtx, const DWORD p_dwListActions, const T_MORPHO_MOC_APDU_PARAMETERS* p_psD2Cparams, const BYTE* p_ddBioTemplate, const DWORD p_dwBioTemplateLength, BYTE* p_pbySW1, BYTE* p_pbySW2);

///////////////////////////////////////////////////////////////
// ypsID S3 tools
//

BOOL Tool_ypsIDS3_FillParametersD2C ( T_MORPHO_MOC_APDU_PARAMETERS*	p_psD2Cparams, 
									  const BYTE*					p_ddBioTemplate,		
									  const DWORD					p_dwBioTemplateLength, 
									  const int						p_nIndexBIO_ToSet,	
									  const int						p_byPkRefFingerInfos,
									  const BOOL					p_bEnroll);

BOOL Tool_ypsIDS3_SCard_Action( CSCardReader* p_poCardReader, SmartCardCtx* p_psCardCtx, const DWORD p_dwListActions, const T_MORPHO_MOC_APDU_PARAMETERS* p_psD2Cparams, const BYTE* p_ddBioTemplate, const DWORD p_dwBioTemplateLength, BYTE* p_pbySW1, BYTE* p_pbySW2);

BOOL Tool_Send_ChainedApdu_ToCard(CSCardReader* p_poCardReader, SmartCardCtx* p_psCardCtx, const T_MORPHO_MOC_APDU_PARAMETERS* p_psD2Cparams, const BYTE* p_ddBioTemplate, const DWORD p_dwBioTemplateLength, BYTE* p_pbySW1, BYTE* p_pbySW2);

BOOL Tool_Length_ChainedApdu(T_MORPHO_MOC_APDU_PARAMETERS* p_psD2Cparams, const DWORD p_dwBioTemplateLength);

#endif // __SmartCardYpsID_h__