#include "stdafx.h"
#include "SmartcardYpsID.h"
#include "Smartcard_Error.h"
#include "MSOSECU.h"



#define NUM_BUFFER_SIZE 2048

#define BIOREADER_DEFAULT_KDEK_BIO_DATA		((BYTE*) "\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF")
#define BIOREADER_DEFAULT_KDEK_BIO_SIZE		16

I	m_i_ApduMaxLen;

int Tool_MorphoParametersD2C_Free(	T_MORPHO_MOC_APDU_PARAMETERS* p_psDest)
{
	if (p_psDest == NULL)
		return MORPHOERR_BADPARAMETER;

	if (p_psDest->m_puc_ApduToTrack != NULL)
		free( p_psDest->m_puc_ApduToTrack);			
	p_psDest->m_puc_ApduToTrack    = NULL;
	p_psDest->m_i_ApduToTrackLen = 0;

	if (p_psDest->m_puc_ApduHeaderToSend_Unique != NULL)
		free( p_psDest->m_puc_ApduHeaderToSend_Unique);			
	p_psDest->m_puc_ApduHeaderToSend_Unique    = NULL;
	if (p_psDest->m_puc_ApduHeaderToSend_First != NULL)
		free( p_psDest->m_puc_ApduHeaderToSend_First);			
	p_psDest->m_puc_ApduHeaderToSend_First     = NULL;
	if (p_psDest->m_puc_ApduHeaderToSend_Middle != NULL)
		free( p_psDest->m_puc_ApduHeaderToSend_Middle);			
	p_psDest->m_puc_ApduHeaderToSend_Middle    = NULL;
	if (p_psDest->m_puc_ApduHeaderToSend_Last != NULL)
		free( p_psDest->m_puc_ApduHeaderToSend_Last);			
	p_psDest->m_puc_ApduHeaderToSend_Last      = NULL;

	if (p_psDest->m_puc_DataToWriteBeforeTemplate != NULL)
		free( p_psDest->m_puc_DataToWriteBeforeTemplate);			
	p_psDest->m_puc_DataToWriteBeforeTemplate    = NULL;
	p_psDest->m_i_DataToWriteBeforeTemplateLen = 0;		
	if (p_psDest->m_puc_DataToWriteAfterTemplate != NULL)
		free( p_psDest->m_puc_DataToWriteAfterTemplate);			
	p_psDest->m_puc_DataToWriteAfterTemplate    = NULL;
	p_psDest->m_i_DataToWriteAfterTemplateLen = 0;		

	if (p_psDest->m_puc_DiversificationData != NULL)
		free( p_psDest->m_puc_DiversificationData);			
	p_psDest->m_puc_DiversificationData    = NULL;
	p_psDest->m_i_DiversificationDataLen = 0;		

	memset( p_psDest, 0, sizeof(T_MORPHO_MOC_APDU_PARAMETERS));
	return MORPHO_OK;
}

int Tool_MorphoParametersD2C_Duplicate( T_MORPHO_MOC_APDU_PARAMETERS*			p_psDest,
										const T_MORPHO_MOC_APDU_PARAMETERS*		p_psSrc)
{
	int	l_nResult = MORPHO_OK;

	if (p_psDest != NULL)
		memset( p_psDest, 0, sizeof(T_MORPHO_MOC_APDU_PARAMETERS));
	if ((p_psDest == NULL) || (p_psSrc == NULL))
		return MORPHOERR_BADPARAMETER;

	p_psDest->m_x_DiversificationType = p_psSrc->m_x_DiversificationType;			
	p_psDest->m_x_CipheredType        = p_psSrc->m_x_CipheredType;
	p_psDest->m_x_PaddingType         = p_psSrc->m_x_PaddingType;					
	p_psDest->m_x_ChainType           = p_psSrc->m_x_ChainType;

	if (  (l_nResult == MORPHO_OK)
	   && (p_psSrc->m_puc_ApduToTrack != NULL)
	   && (p_psSrc->m_i_ApduToTrackLen != 0) )
	{
		p_psDest->m_puc_ApduToTrack = (BYTE*) malloc( p_psSrc->m_i_ApduToTrackLen + 1);
		if (p_psDest->m_puc_ApduToTrack != NULL)
		{
			memcpy( p_psDest->m_puc_ApduToTrack, p_psSrc->m_puc_ApduToTrack, p_psSrc->m_i_ApduToTrackLen);
			p_psDest->m_i_ApduToTrackLen = p_psSrc->m_i_ApduToTrackLen;
		}
		else
			l_nResult = MORPHOERR_MEMORY_PC;
	}

	if (  (l_nResult == MORPHO_OK)
	   && (p_psSrc->m_puc_ApduHeaderToSend_Unique != NULL) )
	{
		p_psDest->m_puc_ApduHeaderToSend_Unique = (BYTE*) malloc( 5 + 1);
		if (p_psDest->m_puc_ApduHeaderToSend_Unique != NULL)
		{
			memcpy( p_psDest->m_puc_ApduHeaderToSend_Unique, p_psSrc->m_puc_ApduHeaderToSend_Unique, 5);
		}
		else
			l_nResult = MORPHOERR_MEMORY_PC;
	}
	if (  (l_nResult == MORPHO_OK)
	   && (p_psSrc->m_puc_ApduHeaderToSend_First != NULL) )
	{
		p_psDest->m_puc_ApduHeaderToSend_First = (BYTE*) malloc( 5 + 1);
		if (p_psDest->m_puc_ApduHeaderToSend_First != NULL)
		{
			memcpy( p_psDest->m_puc_ApduHeaderToSend_First, p_psSrc->m_puc_ApduHeaderToSend_First, 5);
		}
		else
			l_nResult = MORPHOERR_MEMORY_PC;
	}
	if (  (l_nResult == MORPHO_OK)
	   && (p_psSrc->m_puc_ApduHeaderToSend_Middle != NULL) )
	{
		p_psDest->m_puc_ApduHeaderToSend_Middle = (BYTE*) malloc( 5 + 1);
		if (p_psDest->m_puc_ApduHeaderToSend_Middle != NULL)
		{
			memset( p_psDest->m_puc_ApduHeaderToSend_Middle, 0, 5 + 1);
			memcpy( p_psDest->m_puc_ApduHeaderToSend_Middle, p_psSrc->m_puc_ApduHeaderToSend_Middle, 5);
		}
		else
			l_nResult = MORPHOERR_MEMORY_PC;
	}
	if (  (l_nResult == MORPHO_OK)
	   && (p_psSrc->m_puc_ApduHeaderToSend_Last != NULL) )
	{
		p_psDest->m_puc_ApduHeaderToSend_Last = (BYTE*) malloc( 5 + 1);
		if (p_psDest->m_puc_ApduHeaderToSend_Last != NULL)
		{
			memset( p_psDest->m_puc_ApduHeaderToSend_Last, 0, 5 + 1);
			memcpy( p_psDest->m_puc_ApduHeaderToSend_Last, p_psSrc->m_puc_ApduHeaderToSend_Last, 5);
		}
		else
			l_nResult = MORPHOERR_MEMORY_PC;
	}

	if (  (l_nResult == MORPHO_OK)
	   && (p_psSrc->m_puc_DataToWriteBeforeTemplate != NULL)
	   && (p_psSrc->m_i_DataToWriteBeforeTemplateLen != 0) )
	{
		p_psDest->m_puc_DataToWriteBeforeTemplate = (BYTE*) malloc( p_psSrc->m_i_DataToWriteBeforeTemplateLen + 1);
		if (p_psDest->m_puc_DataToWriteBeforeTemplate != NULL)
		{
			memset( p_psDest->m_puc_DataToWriteBeforeTemplate, 0, p_psSrc->m_i_DataToWriteBeforeTemplateLen + 1);
			memcpy( p_psDest->m_puc_DataToWriteBeforeTemplate, p_psSrc->m_puc_DataToWriteBeforeTemplate, p_psSrc->m_i_DataToWriteBeforeTemplateLen);
			p_psDest->m_i_DataToWriteBeforeTemplateLen = p_psSrc->m_i_DataToWriteBeforeTemplateLen;
		}
		else
			l_nResult = MORPHOERR_MEMORY_PC;
	}
	if (  (l_nResult == MORPHO_OK)
	   && (p_psSrc->m_puc_DataToWriteAfterTemplate != NULL)
	   && (p_psSrc->m_i_DataToWriteAfterTemplateLen != 0) )
	{
		p_psDest->m_puc_DataToWriteAfterTemplate = (BYTE*) malloc( p_psSrc->m_i_DataToWriteAfterTemplateLen + 1);
		if (p_psDest->m_puc_DataToWriteAfterTemplate != NULL)
		{
			memset( p_psDest->m_puc_DataToWriteAfterTemplate, 0, p_psSrc->m_i_DataToWriteAfterTemplateLen + 1);
			memcpy( p_psDest->m_puc_DataToWriteAfterTemplate, p_psSrc->m_puc_DataToWriteAfterTemplate, p_psSrc->m_i_DataToWriteAfterTemplateLen);
			p_psDest->m_i_DataToWriteAfterTemplateLen = p_psSrc->m_i_DataToWriteAfterTemplateLen;
		}
		else
			l_nResult = MORPHOERR_MEMORY_PC;
	}

	if (  (l_nResult == MORPHO_OK)
	   && (p_psSrc->m_puc_DiversificationData != NULL)
	   && (p_psSrc->m_i_DiversificationDataLen != 0) )
	{
		p_psDest->m_puc_DiversificationData = (BYTE*) malloc( p_psSrc->m_i_DiversificationDataLen + 1);
		if (p_psDest->m_puc_DiversificationData != NULL)
		{
			memset( p_psDest->m_puc_DiversificationData, 0, p_psSrc->m_i_DiversificationDataLen + 1);
			memcpy( p_psDest->m_puc_DiversificationData, p_psSrc->m_puc_DiversificationData, p_psSrc->m_i_DiversificationDataLen);
			p_psDest->m_i_DiversificationDataLen = p_psSrc->m_i_DiversificationDataLen;
		}
		else
			l_nResult = MORPHOERR_MEMORY_PC;
	}

	if (l_nResult != MORPHO_OK)
	{
		Tool_MorphoParametersD2C_Free( p_psDest);
		memset( p_psDest, 0, sizeof(T_MORPHO_MOC_APDU_PARAMETERS));
	}
	return l_nResult;
}

int Tool_Crypto_Cipher(	const BYTE*									p_ddDataKey,
						const DWORD									p_dwDataKeyLength,
						const T_MORPHO_MOC_DIVERSIFICATION_TYPE		p_eKeyDiversificationType,
						const BYTE*									p_ddKeyDiversificationData,
						const DWORD									p_dwKeyDiversificationDataLength,
						const T_MORPHO_MOC_CIPHERED_TYPE			p_eCipherType,
						const T_MORPHO_MOC_APDU_PADDING_TYPE		p_ePaddingType,
						const BYTE*									p_ddInputData,
						const DWORD									p_dwInputDataLength,
						BYTE*										p_ddOutputData,
						DWORD*										p_pdwOutputDataLength)
{
	int					l_nResult		= MORPHO_OK;
	BYTE				l_ddOutputData[ 2048];
	BYTE				l_ddInputData[ 2048];
	BYTE				l_ddDataKey[ 64];
	DWORD				l_dwDataKeyLength       = 0;
	DWORD				l_dwInputDataLength     = 0;
	DWORD				l_dwOutputDataLength    = 0;
	DWORD				l_dwOutputDataLengthRef = 0;

	if (p_pdwOutputDataLength != NULL)
	{
		l_dwOutputDataLength = (*p_pdwOutputDataLength);
		(*p_pdwOutputDataLength) = 0;
	}
	if ((p_ddOutputData != NULL) && (l_dwOutputDataLength != 0))
		memset( p_ddOutputData, 0, l_dwOutputDataLength);

	if (  (p_eKeyDiversificationType != MORPHO_MOC_DIVERSIFICATION_NO) 
	   && (p_eKeyDiversificationType != MORPHO_MOC_DIVERSIFICATION_TYPE_1))
		return MORPHOERR_BADPARAMETER;
	if (  (p_eCipherType != MORPHO_MOC_CIPHERED_NO) 
	   && (p_eCipherType != MORPHO_MOC_CIPHERED_3DES_ECB)
	   && (p_eCipherType != MORPHO_MOC_CIPHERED_3DES_CBC))
		return MORPHOERR_BADPARAMETER;
	if (  (p_ePaddingType != MORPHO_MOC_APDU_PADDING_TYPE_NO) 
	   && (p_ePaddingType != MORPHO_MOC_APDU_PADDING_TYPE_1))
		return MORPHOERR_BADPARAMETER;
	if (p_pdwOutputDataLength == NULL)
		return MORPHOERR_BADPARAMETER;
	if ((p_ddInputData == NULL) || (p_dwInputDataLength == 0))
		return MORPHOERR_BADPARAMETER;

	if (  (p_eCipherType != MORPHO_MOC_CIPHERED_NO)
	   && ((p_ddDataKey == NULL) || (p_dwDataKeyLength == 0)) 
	   && (p_dwDataKeyLength != 16) )
		return MORPHOERR_BADPARAMETER;

	if ((p_ddDataKey == NULL) || (p_dwDataKeyLength == 0))
		return MORPHOERR_BADPARAMETER;

	if (   (p_eKeyDiversificationType != MORPHO_MOC_DIVERSIFICATION_NO)
		&& ((p_ddKeyDiversificationData == NULL) || (p_dwKeyDiversificationDataLength == 0)) 
		&& (p_dwKeyDiversificationDataLength != 16) )
		return MORPHOERR_BADPARAMETER;

	if (  (p_eCipherType != MORPHO_MOC_CIPHERED_NO)
	   && (p_ePaddingType == MORPHO_MOC_APDU_PADDING_TYPE_NO)
	   && ((p_dwInputDataLength % 8) != 0) )
		return MORPHOERR_BADPARAMETER;

	memset( l_ddOutputData, 0, 2048);
	memset( l_ddInputData, 0, 2048);
	memset( l_ddDataKey, 0, 64);

	// 1) diversification de la clé si nicessaire
	if (p_eCipherType != MORPHO_MOC_CIPHERED_NO)
	{
		// transfert de la clé à utiliser
		if (p_eKeyDiversificationType == MORPHO_MOC_DIVERSIFICATION_NO)
		{
			memcpy( l_ddDataKey,  p_ddDataKey, p_dwDataKeyLength);
			memcpy( &l_ddDataKey[ 16],  l_ddDataKey, 8);
			l_dwDataKeyLength = p_dwDataKeyLength;
		}
		// Calcul par diversification de la clé à utiliser
		else
		{
			memcpy( &l_ddDataKey[ 24],  p_ddDataKey, p_dwDataKeyLength);
			memcpy( &l_ddDataKey[ 24 + 16],  p_ddDataKey, 8);
			MSOSECU_TripleDesCryptByKey ( (UC*) &l_ddDataKey[ 24], p_dwKeyDiversificationDataLength, (UC*) p_ddKeyDiversificationData, l_ddDataKey );
			memcpy( &l_ddDataKey[ 16],  l_ddDataKey, 8);
			l_dwDataKeyLength = p_dwKeyDiversificationDataLength;
		}
	}

	// 2) gestion du padding
	if (l_nResult == MORPHO_OK)
	{
		memcpy( l_ddInputData,  p_ddInputData, p_dwInputDataLength);
		l_dwInputDataLength = p_dwInputDataLength;
		if (p_ePaddingType == MORPHO_MOC_APDU_PADDING_TYPE_1)
		{
			l_ddInputData[ l_dwInputDataLength] = 0x80;
			l_dwInputDataLength++;
			if ((l_dwInputDataLength % 8) != 0)
				l_dwInputDataLength += 8 - (l_dwInputDataLength % 8);
		}
	}

	// 3) gestion du Chiffrement
	if (l_nResult == MORPHO_OK)
	{
		switch(p_eCipherType)
		{
			case MORPHO_MOC_CIPHERED_NO:
				l_dwOutputDataLengthRef = l_dwInputDataLength;
				memcpy( l_ddOutputData, l_ddInputData, l_dwInputDataLength);
				break;

			case MORPHO_MOC_CIPHERED_3DES_ECB:
				MSOSECU_TripleDesCryptByKey ( (UC*) l_ddDataKey, l_dwInputDataLength, (UC*) l_ddInputData, l_ddOutputData );
				l_dwOutputDataLengthRef = l_dwInputDataLength;
				break;

			case MORPHO_MOC_CIPHERED_3DES_CBC:
				break;
		}
	}

	// 4) Effacement des données intermedaire
	memset( l_ddInputData, 0, 2048);
	memset( l_ddDataKey, 0, 64);

	// 5) Exportation des resultats
	if (l_nResult == MORPHO_OK)
	{
		if ((p_ddOutputData != NULL) && (l_dwOutputDataLengthRef <= l_dwOutputDataLength))
		{
			memcpy( p_ddOutputData, l_ddOutputData, l_dwOutputDataLengthRef);
			(*p_pdwOutputDataLength) = l_dwOutputDataLengthRef;
		}
		else if (p_ddOutputData != NULL) 
		{
			(*p_pdwOutputDataLength) = l_dwOutputDataLengthRef;
			l_nResult = MORPHOERR_BADPARAMETER;
		}
		else
			(*p_pdwOutputDataLength) = l_dwOutputDataLengthRef;
	}

	// Libération Mémoire
	memset( l_ddOutputData, 0, 2048);
	return l_nResult;
}



///////////////////////////////////////////////////////////////
// ASN.1 tools
//

BOOL Asn1_DecodeTLVLength( DWORD* p_pdwOffset, const BYTE* p_ddSDOencoded, const DWORD p_dwLenSDOencoded, DWORD* p_pdwLength)
{
	BOOL	l_bResult = TRUE;

    if (p_pdwLength != NULL)
        (*p_pdwLength) = 0;
    if ((p_pdwOffset == NULL) || (p_ddSDOencoded == NULL) || (p_dwLenSDOencoded == 0) || (p_pdwLength == NULL))
        return FALSE;
    if ((*p_pdwOffset) >= p_dwLenSDOencoded)
        return FALSE;

    if (p_ddSDOencoded[ (*p_pdwOffset)] < 0x80)
    {
        (*p_pdwLength) = p_ddSDOencoded[ (*p_pdwOffset)++];
    }
    else if (p_ddSDOencoded[ (*p_pdwOffset)] == 0x81)
    {
        if ((p_dwLenSDOencoded - (*p_pdwOffset)) > 0)
        {
            (*p_pdwLength) = p_ddSDOencoded[ (*p_pdwOffset)+1];
            (*p_pdwOffset)+=2;
        }
        else
	        l_bResult = FALSE;
    }
    else if (p_ddSDOencoded[ (*p_pdwOffset)] == 0x82)
    {
        if ((p_dwLenSDOencoded - (*p_pdwOffset)) > 1)
        {
            (*p_pdwLength)   = p_ddSDOencoded[ (*p_pdwOffset)+1];
            (*p_pdwLength) <<= 8;
            (*p_pdwLength)  += p_ddSDOencoded[ (*p_pdwOffset)+2];
            (*p_pdwOffset)+=3;
        }
        else
	        l_bResult = FALSE;
    }
    else if (p_ddSDOencoded[ (*p_pdwOffset)] == 0x83)
    {
        if ((p_dwLenSDOencoded - (*p_pdwOffset)) > 2)
        {
            (*p_pdwLength)   = p_ddSDOencoded[ (*p_pdwOffset)+1];
            (*p_pdwLength) <<= 8;
            (*p_pdwLength)  += p_ddSDOencoded[ (*p_pdwOffset)+2];
            (*p_pdwLength) <<= 8;
            (*p_pdwLength)  += p_ddSDOencoded[ (*p_pdwOffset)+3];
            (*p_pdwOffset)+=4;
        }
        else
	        l_bResult = FALSE;
    }
    else
        l_bResult = FALSE;

    return l_bResult;
}

BOOL Asn1_EncodeTLVLength( const DWORD p_dwLength, DWORD* p_pdwOffset, BYTE* p_ddBuffer, const DWORD p_dwLenBuffer)
{
    if ((p_pdwOffset == NULL) || (p_ddBuffer == NULL) || (p_dwLenBuffer == 0))
		return FALSE;
    if (p_dwLength < 0x00000080)
    {
        p_ddBuffer[ (*p_pdwOffset)] = (BYTE) (p_dwLength & 0x7F);
        (*p_pdwOffset)++;
        return TRUE;
    }
    else if (p_dwLength < 0x00000100) 
    {
        p_ddBuffer[ (*p_pdwOffset)]   = 0x81;
        p_ddBuffer[ (*p_pdwOffset)+1] = (BYTE) (p_dwLength & 0xFF);
        (*p_pdwOffset)+=2;
        return TRUE;
    }
    else if (p_dwLength < 0x00010000) 
    {
        p_ddBuffer[ (*p_pdwOffset)]   = 0x82;
        p_ddBuffer[ (*p_pdwOffset)+1] = (BYTE) ((p_dwLength >> 8) & 0xFF);
        p_ddBuffer[ (*p_pdwOffset)+2] = (BYTE) (p_dwLength & 0xFF);
        (*p_pdwOffset)+=3;
        return TRUE;
    }
    else if (p_dwLength < 0x01000000) 
    {
        p_ddBuffer[ (*p_pdwOffset)]   = 0x83;
        p_ddBuffer[ (*p_pdwOffset)+1] = (BYTE) ((p_dwLength >> 16) & 0xFF);
        p_ddBuffer[ (*p_pdwOffset)+2] = (BYTE) ((p_dwLength >> 8) & 0xFF);
        p_ddBuffer[ (*p_pdwOffset)+2] = (BYTE) (p_dwLength & 0xFF);
        (*p_pdwOffset)+=4;
        return TRUE;
    }
    return FALSE;
}

DWORD Asn1_GetNecessaryByteCount( const DWORD p_dwLength)
{
    if (p_dwLength < 0x00000080)
        return 1;
    else if (p_dwLength < 0x00000100) 
        return 2;
    else if (p_dwLength < 0x00010000) 
        return 3;
    else if (p_dwLength < 0x01000000) 
        return 4;
    return 5;
}

///////////////////////////////////////////////////////////////
// Scard tools
//

#define NUM_BUFFER_INDEX_CLA    0
#define NUM_BUFFER_INDEX_INS    1
#define NUM_BUFFER_INDEX_P1     2
#define NUM_BUFFER_INDEX_P2     3
#define NUM_BUFFER_INDEX_P3     4
#define NUM_BUFFER_INDEX_Lc     4
#define NUM_BUFFER_INDEX_DATA   5

BOOL SCard_BeginTransaction( CSCardReader* p_poCardReader)
{
	if (p_poCardReader == NULL)
		return FALSE;

	return TRUE;
}

BOOL SCard_EndTransaction( CSCardReader* p_poCardReader)
{
	if (p_poCardReader == NULL)
		return FALSE;

	return TRUE;
}

BOOL SCard_Transmit( CSCardReader* p_poCardReader, const BOOL p_bModeTransactionned, const BYTE* p_ddCmd, const DWORD p_dwCmdLength, BYTE* p_ddAnswer, DWORD* p_pdwAnswerLength, BYTE* p_pbySW1, BYTE* p_pbySW2)
{
	BYTE				l_ddAnswer[ 512 + 2];
	BOOL				l_bResult           = FALSE;
	LONG				l_lCardError		= SMARTCARD_OK;
	DWORD				l_dwIoAnswerLen     = 0;
	DWORD				l_dwAnswerLength    = 0;
	BYTE				l_bySW1             = 0x00;
	BYTE				l_bySW2             = 0x00;

	if (p_poCardReader == NULL)
		return FALSE;

	if (p_pdwAnswerLength != NULL)
	{
		l_dwAnswerLength = (*p_pdwAnswerLength);
		(*p_pdwAnswerLength) = 0;
	}
	if ((p_ddAnswer != NULL) && (l_dwAnswerLength != 0))
		memset( p_ddAnswer, 0, l_dwAnswerLength);
	if (p_pbySW1 != NULL)
		(*p_pbySW1) = 0x00;
	if (p_pbySW2 != NULL)
		(*p_pbySW2) = 0x00;
	if ((p_ddCmd == NULL) || (p_dwCmdLength < 4))
		return FALSE;

	l_bResult = TRUE;
	if (l_lCardError == SMARTCARD_OK)
	{
		T__APDU_IN			l_x_APDU;
		UL					l_ul_AnswerLenght = 0;

		memset( &l_x_APDU, 0, sizeof(T__APDU_IN));
		l_x_APDU.m_uc_Cla = p_ddCmd[0];
		l_x_APDU.m_uc_Ins = p_ddCmd[1];
		l_x_APDU.m_uc_P1  = p_ddCmd[2];
		l_x_APDU.m_uc_P2  = p_ddCmd[3];
		l_x_APDU.m_us_Le  = 0; 

		memset( l_ddAnswer, 0, (512 + 2));
		if ((p_ddAnswer != NULL) && (l_dwAnswerLength != 0))
		{
			l_ul_AnswerLenght = 2 + ((l_dwAnswerLength < 512) ? l_dwAnswerLength : 512);
		}

		if (p_dwCmdLength > 5)
		{
			l_x_APDU.m_uc_Lc  = p_ddCmd[4];

			l_lCardError = p_poCardReader->SendAPDU ( &l_x_APDU, (UC*) &p_ddCmd[ 5], p_dwCmdLength - 5, l_ddAnswer, &l_ul_AnswerLenght );
		}
		else if (p_dwCmdLength == 5)
		{
			l_x_APDU.m_us_Le  = p_ddCmd[4];

			l_lCardError = p_poCardReader->SendAPDU ( &l_x_APDU, NULL, 0, l_ddAnswer, &l_ul_AnswerLenght );
		}
		else
		{
			l_lCardError = p_poCardReader->SendAPDU ( &l_x_APDU, NULL, 0, l_ddAnswer, &l_ul_AnswerLenght );
		}

		l_bResult = (l_lCardError != SMARTCARD_OK) ? FALSE : TRUE;
		p_poCardReader->GetLastStatusBytesError( &l_bySW1, &l_bySW2);
		l_dwIoAnswerLen = l_ul_AnswerLenght;
	}
	if (l_bResult == TRUE)
	{
		if (p_ddAnswer != NULL)
			memcpy( p_ddAnswer, l_ddAnswer, l_dwIoAnswerLen);
		if (p_pdwAnswerLength != NULL)
			(*p_pdwAnswerLength) = l_dwIoAnswerLen;
	}
	if (p_pbySW1 != NULL)
		(*p_pbySW1) = l_bySW1;
	if (p_pbySW2 != NULL)
		(*p_pbySW2) = l_bySW2;
	return l_bResult;
}


///////////////////////////////////////////////////////////////
// ypsID S2 tools
//

#define YPSIDS2_DEFAULT_KDEK_DATA_DATA		((BYTE*) "\x53\x6F\x30\x30\x31\x5F\x4B\x6D\x5F\x64\x65\x6B\x5F\x44\x61\x74")
#define YPSIDS2_DEFAULT_KDEK_DATA_SIZE		16


# define YPSIDS2_BIOREADER_VERIFY_CMD_TO_TRACK     "\x80\x81\x82V\x08ypsIDS20"
# define YPSIDS2_BIOREADER_VERIFY_LEN_CMD_TO_TRACK (sizeof(YPSIDS2_BIOREADER_VERIFY_CMD_TO_TRACK)-1)

# define YPSIDS2_BIOREADER_ENROLL_CMD_TO_TRACK      "\x80\x81\x82W\x07ypsIDS2"
# define YPSIDS2_BIOREADER_ENROLL_LEN_CMD_TO_TRACK (sizeof(YPSIDS2_BIOREADER_ENROLL_CMD_TO_TRACK)-1)


# define YPSIDS2_BIT_TEMPLATE_VALUE_OFFSET_REFJMARKOBJECT  2
# define YPSIDS2_BIT_TEMPLATE_VALUE_OFFSET_REFFINGER       18
# define YPSIDS2_BIT_TEMPLATE_VALUE_OFFSET_PKFORMAT        21

# define YPSIDS2_BIT_TEMPLATE_VALUE_PKFORMAT_ISO           "\x00\x1D"
# define YPSIDS2_BIT_TEMPLATE_VALUE_PKFORMAT_ISO_LEN        (sizeof(YPSIDS2_BIT_TEMPLATE_VALUE_PKFORMAT_ISO) - 1)

# define YPSIDS2_BIT_TEMPLATE_VALUE				"\x83\x01\x00" \
												"\x06\x06\x60\x86\x48\x01\x65\x03" \
												"\xA1\x17" \
                                                         "\x81\x01\x08" \
                                                         "\x82\x01\x00" \
                                                         "\x87\x02\x00\x1A" \
                                                         "\x88\x02\x08\x04" \
                                                         "\xB1\x07" \
                                                                  "\x81\x02\x0C\x38" \
                                                                  "\x82\x01\x0D"

# define YPSIDS2_BIT_TEMPLATE_VALUE_LEN        (sizeof(YPSIDS2_BIT_TEMPLATE_VALUE) - 1)

BOOL Tool_ypsIDS2_FillParametersD2C ( T_MORPHO_MOC_APDU_PARAMETERS*	p_psD2Cparams, 
									  const BYTE*					p_ddBioTemplate,		
									  const DWORD					p_dwBioTemplateLength, 
									  const int						p_nIndexBIO_ToSet,	
									  const int						p_byPkRefFingerInfos,
									  const BYTE*					p_ddCardSerialNumber, 
									  const DWORD					p_dwCardSerialNumberLength, 
									  const BOOL					p_bEnroll)
{
    BYTE	l_ddDataTmp[NUM_BUFFER_SIZE];      /* Buffer dans lequel on encode 7F2E L2 90 L1 PK */
    BYTE	l_ddBitTemplate[NUM_BUFFER_SIZE];  /* Buffer dans lequel on encode le Biometric Information Template */
    BOOL	l_bResult       = TRUE;
    DWORD	l_dwOffsetTmp   = 0;                /* Offset courant dans le buffer = taille des données dans le buffer */
    DWORD	l_dwOffsetBit   = 0;                /* Offset courant dans le buffer = taille des données dans le buffer */
    DWORD	l_dwL2          = 0;
    DWORD	l_dwLenPkData   = 0;                /* Longueur des données Pk (pour le cas ou les données Pks sont déjà encapsulées dans un 7F2E ) */
    DWORD	l_dwLenBefore   = 0;
    DWORD	l_dwLenAfter    = 0;
    
	if (p_psD2Cparams != NULL)
		memset( p_psD2Cparams, 0, sizeof(p_psD2Cparams));
	else
		return FALSE;
	if (p_dwBioTemplateLength == 0)
		return FALSE;
	if ((p_ddCardSerialNumber == NULL) || (p_dwCardSerialNumberLength == 0))
		return FALSE;

    /* 3 etapes : 
     * 1) On encapsule les Pks si nécessaire (PKISO dans un 7F2E ...)
     * 2) On met le B.I.T. 7F60 si nécessaire
     * 3) On met le tout dans un 5F2E .......
     */

    /* 1) Encapsulation des Pks */
	memset( l_ddDataTmp, 0, NUM_BUFFER_SIZE );
	memset( l_ddBitTemplate, 0, NUM_BUFFER_SIZE );
    /* | (B.I.T.=7F60 L4 B.I.T.) | 5F2E | L3 | 7F2E | L2 | 90 | L1 | Pk | */
    /* Création de 7F2E L2 90 L1 Pk si nécessaire */
    if (p_ddBioTemplate == NULL)
	{
        l_dwL2 = 1 + Asn1_GetNecessaryByteCount(p_dwBioTemplateLength) + p_dwBioTemplateLength;			/* sizeof(90) + sizeof(L1) + sizeof(Pk) */
        l_ddDataTmp[l_dwOffsetTmp++] = 0x7F;
        l_ddDataTmp[l_dwOffsetTmp++] = 0x2E;                                                            /* 7F2E */
        Asn1_EncodeTLVLength ( l_dwL2, &l_dwOffsetTmp, l_ddDataTmp, NUM_BUFFER_SIZE );					/* 7F2E L2 */
        l_ddDataTmp[l_dwOffsetTmp++] = 0x90;                                                            /* 7F2E L2 90 */
        Asn1_EncodeTLVLength ( p_dwBioTemplateLength, &l_dwOffsetTmp, l_ddDataTmp, NUM_BUFFER_SIZE );	/* 7F2E L2 90 L1 */
        memset ( &l_ddDataTmp[ l_dwOffsetTmp], 0x22, p_dwBioTemplateLength );								/* 7F2E L2 90 L1 PkData */                
        l_dwLenBefore = l_dwOffsetTmp;
        l_dwOffsetTmp += p_dwBioTemplateLength;
        l_dwLenPkData  = p_dwBioTemplateLength;
	}
    else if ((p_ddBioTemplate[ 0] != '\x7F') || (p_ddBioTemplate[ 1] != '\x2E')) 
    {
        l_dwL2 = 1 + Asn1_GetNecessaryByteCount(p_dwBioTemplateLength) + p_dwBioTemplateLength;			/* sizeof(90) + sizeof(L1) + sizeof(Pk) */
        l_ddDataTmp[l_dwOffsetTmp++] = 0x7F;
        l_ddDataTmp[l_dwOffsetTmp++] = 0x2E;                                                            /* 7F2E */
        Asn1_EncodeTLVLength ( l_dwL2, &l_dwOffsetTmp, l_ddDataTmp, NUM_BUFFER_SIZE );					/* 7F2E L2 */
        l_ddDataTmp[l_dwOffsetTmp++] = 0x90;                                                            /* 7F2E L2 90 */
        Asn1_EncodeTLVLength ( p_dwBioTemplateLength, &l_dwOffsetTmp, l_ddDataTmp, NUM_BUFFER_SIZE );	/* 7F2E L2 90 L1 */
        memcpy ( &l_ddDataTmp[ l_dwOffsetTmp], p_ddBioTemplate, p_dwBioTemplateLength );                /* 7F2E L2 90 L1 PkData */                
        l_dwLenBefore = l_dwOffsetTmp;
        l_dwOffsetTmp += p_dwBioTemplateLength;
        l_dwLenPkData  = p_dwBioTemplateLength;
    }
    else
    {
        DWORD ll_dwOffset = 2;

        memcpy ( l_ddDataTmp, p_ddBioTemplate, p_dwBioTemplateLength );
        l_dwOffsetTmp = p_dwBioTemplateLength;
        /* On récupère la vraie taille des données Pks */
        Asn1_DecodeTLVLength ( &ll_dwOffset, l_ddDataTmp, l_dwOffsetTmp, &l_dwLenPkData );				/* Decode la longueur L2 */
        ll_dwOffset++;																					/* Se positionne sur L1 */
        Asn1_DecodeTLVLength ( &ll_dwOffset, l_ddDataTmp, l_dwOffsetTmp, &l_dwLenPkData );				/* Decode la longueur L1 */
        l_dwLenBefore = ll_dwOffset;
    }

    /* 2) On encode le B.I.T. si nécessaire */
    if (l_bResult == TRUE)
    {
        /* On ajoute le BIT le cas échéant : i.e. juste lorsqu'on enrolle */
        /* Création de 7F60 ... */
        if ( p_bEnroll == TRUE ) 
        {
            l_dwL2  = 2 + Asn1_GetNecessaryByteCount( YPSIDS2_BIT_TEMPLATE_VALUE_LEN) + YPSIDS2_BIT_TEMPLATE_VALUE_LEN; /* sizeof (7F60) + sizeof(Lbit) + sizeof(bit) */
            l_dwL2 += 2 + Asn1_GetNecessaryByteCount( l_dwOffsetTmp) + l_dwOffsetTmp;									/* + sizeof(ce qu'il y avait avant) */
            l_dwLenBefore += 2 + Asn1_GetNecessaryByteCount( YPSIDS2_BIT_TEMPLATE_VALUE_LEN) + YPSIDS2_BIT_TEMPLATE_VALUE_LEN;
            l_dwLenBefore += 2 + Asn1_GetNecessaryByteCount( l_dwOffsetTmp);
            l_ddBitTemplate[l_dwOffsetBit++] = 0x7F;                              /*      */
            l_ddBitTemplate[l_dwOffsetBit++] = 0x60;                               /* 7F60 */
            Asn1_EncodeTLVLength( YPSIDS2_BIT_TEMPLATE_VALUE_LEN, &l_dwOffsetBit, l_ddBitTemplate, NUM_BUFFER_SIZE); /* 7F60 L2 */
            memcpy( &l_ddBitTemplate[ l_dwOffsetBit], YPSIDS2_BIT_TEMPLATE_VALUE , YPSIDS2_BIT_TEMPLATE_VALUE_LEN);    /* 7F60 L2 | Biometric Information Template | */

            /* On remplit le template */
            l_ddBitTemplate[l_dwOffsetBit + YPSIDS2_BIT_TEMPLATE_VALUE_OFFSET_REFJMARKOBJECT] = (BYTE) (p_nIndexBIO_ToSet & 0xFF); /* On met l'indice du SDO */
            l_ddBitTemplate[l_dwOffsetBit + YPSIDS2_BIT_TEMPLATE_VALUE_OFFSET_REFFINGER]      = (BYTE) (p_byPkRefFingerInfos);     /* On met la ref du doigt */
            memcpy( &l_ddBitTemplate[ l_dwOffsetBit + YPSIDS2_BIT_TEMPLATE_VALUE_OFFSET_PKFORMAT], YPSIDS2_BIT_TEMPLATE_VALUE_PKFORMAT_ISO, YPSIDS2_BIT_TEMPLATE_VALUE_PKFORMAT_ISO_LEN );

            /* Taille du B.I.T. encodé */
            l_dwOffsetBit += YPSIDS2_BIT_TEMPLATE_VALUE_LEN;
        }
        else
        {
            l_dwL2 = 2 + Asn1_GetNecessaryByteCount(l_dwOffsetTmp) + l_dwOffsetTmp; /* 2 + sizeof(Longueur(ddDataTmp) + sizeof(ddDataTmp) = 5F2E L Data */
            l_dwLenBefore += 2 + Asn1_GetNecessaryByteCount(l_dwOffsetTmp);
        }
    }

    /* 3) On encapsule le tout dans un 5F2E .... */
    if (l_bResult == TRUE)
    {
        l_ddBitTemplate[ l_dwOffsetBit++] = 0x5F;
        l_ddBitTemplate[ l_dwOffsetBit++] = 0x2E;
        Asn1_EncodeTLVLength( l_dwOffsetTmp, &l_dwOffsetBit, l_ddBitTemplate, NUM_BUFFER_SIZE);
        memcpy( &l_ddBitTemplate[ l_dwOffsetBit], l_ddDataTmp, l_dwOffsetTmp);
        l_dwOffsetBit += l_dwOffsetTmp;

        if ( l_dwL2 != l_dwOffsetBit )
        {
            // probléme
            memset( l_ddDataTmp, 0, NUM_BUFFER_SIZE);
            memset( l_ddBitTemplate, 0, NUM_BUFFER_SIZE);
            l_bResult = FALSE;
        }
    }

    /* Everything went fine */
    if (l_bResult == TRUE)
    {
		T_MORPHO_MOC_APDU_PARAMETERS	ll_sD2Cparams;
		BYTE							ll_ddTrack[ 64];	
		BYTE							ll_ddCmd[ 10];	
		BYTE							ll_ddDiversificationData[ 16];

		memset( &ll_sD2Cparams, 0, sizeof(T_MORPHO_MOC_APDU_PARAMETERS));
		memset( ll_ddDiversificationData, 0, 16);
		memset( ll_ddTrack, 0, 64);
		memset( ll_ddCmd, 0, 10);

        if ( p_bEnroll == TRUE ) 
        {
			memcpy( ll_ddTrack, YPSIDS2_BIOREADER_ENROLL_CMD_TO_TRACK, YPSIDS2_BIOREADER_ENROLL_LEN_CMD_TO_TRACK);
			ll_sD2Cparams.m_puc_ApduToTrack    = ll_ddTrack;
			ll_sD2Cparams.m_i_ApduToTrackLen = YPSIDS2_BIOREADER_ENROLL_LEN_CMD_TO_TRACK;

            ll_ddCmd[ NUM_BUFFER_INDEX_INS] = 0x24;
            ll_ddCmd[ NUM_BUFFER_INDEX_P1]  = 0x01;
			ll_ddCmd[ NUM_BUFFER_INDEX_P2]  = (BYTE)(p_nIndexBIO_ToSet & 0xFF);
			memcpy( &ll_ddCmd[ 5], ll_ddCmd, 5);
			ll_ddCmd[ 5 + NUM_BUFFER_INDEX_CLA] |= 0x10;
		}
		else
        {
			memcpy( ll_ddTrack, YPSIDS2_BIOREADER_VERIFY_CMD_TO_TRACK, YPSIDS2_BIOREADER_VERIFY_LEN_CMD_TO_TRACK);
			ll_ddTrack[ YPSIDS2_BIOREADER_VERIFY_LEN_CMD_TO_TRACK - 1] = (BYTE)(p_nIndexBIO_ToSet & 0xFF);
			ll_sD2Cparams.m_puc_ApduToTrack    = ll_ddTrack;
			ll_sD2Cparams.m_i_ApduToTrackLen = YPSIDS2_BIOREADER_VERIFY_LEN_CMD_TO_TRACK;

            ll_ddCmd[ NUM_BUFFER_INDEX_INS] = 0x20;
            ll_ddCmd[ NUM_BUFFER_INDEX_P1]  = 0x01;
			ll_ddCmd[ NUM_BUFFER_INDEX_P2]  = (BYTE)(p_nIndexBIO_ToSet & 0xFF);
			memcpy( &ll_ddCmd[ 5], ll_ddCmd, 5);
			ll_ddCmd[ 5 + NUM_BUFFER_INDEX_CLA] |= 0x10;
		}

		if (l_dwLenBefore != 0)
		{
			ll_sD2Cparams.m_puc_DataToWriteBeforeTemplate    = &l_ddBitTemplate[ 0];
			ll_sD2Cparams.m_i_DataToWriteBeforeTemplateLen = l_dwLenBefore;
		}
		if (l_dwLenAfter != 0)
		{
			ll_sD2Cparams.m_puc_DataToWriteBeforeTemplate    = &l_ddBitTemplate[ l_dwOffsetBit - l_dwLenAfter];
			ll_sD2Cparams.m_i_DataToWriteBeforeTemplateLen = l_dwLenAfter;
		}

		ll_sD2Cparams.m_puc_ApduHeaderToSend_Unique = ll_ddCmd;
		ll_sD2Cparams.m_puc_ApduHeaderToSend_First  = &ll_ddCmd[5];
		ll_sD2Cparams.m_puc_ApduHeaderToSend_Middle = &ll_ddCmd[5];
		ll_sD2Cparams.m_puc_ApduHeaderToSend_Last   = ll_ddCmd;

        ll_sD2Cparams.m_x_CipheredType = MORPHO_MOC_CIPHERED_3DES_ECB;
        ll_sD2Cparams.m_x_PaddingType  = MORPHO_MOC_APDU_PADDING_TYPE_1;
		ll_sD2Cparams.m_x_ChainType    = MORPHO_MOC_APDU_CHAIN_TYPE_1;

		if ((p_ddCardSerialNumber != NULL) && (p_dwCardSerialNumberLength != 0) && (p_dwCardSerialNumberLength == 8))
		{
			memcpy( &ll_ddDiversificationData[ 0], &p_ddCardSerialNumber[2], 6);
			memset( &ll_ddDiversificationData[ 6], 0, 2);
			memcpy( &ll_ddDiversificationData[ 8], ll_ddDiversificationData, 8);

			ll_ddDiversificationData[ 6]  = 0xF0;
			ll_ddDiversificationData[ 7]  = 0x03;
			ll_ddDiversificationData[ 14] = 0x0F;
			ll_ddDiversificationData[ 15] = 0x03;

			ll_sD2Cparams.m_x_DiversificationType    = MORPHO_MOC_DIVERSIFICATION_TYPE_1;
			ll_sD2Cparams.m_puc_DiversificationData  = ll_ddDiversificationData;
			ll_sD2Cparams.m_i_DiversificationDataLen = 16;
		}

		if (Tool_MorphoParametersD2C_Duplicate( p_psD2Cparams, &ll_sD2Cparams) != MORPHO_OK)
			l_bResult = FALSE;
    }

    return l_bResult;
}

BOOL Tool_ypsIDS2_SCard_Action( CSCardReader* p_poCardReader, SmartCardCtx* p_psCardCtx, const DWORD p_dwListActions, const T_MORPHO_MOC_APDU_PARAMETERS* p_psD2Cparams, const BYTE* p_ddBioTemplate, const DWORD p_dwBioTemplateLength, BYTE* p_pbySW1, BYTE* p_pbySW2)
{
	BYTE	l_ddCmd[ 512];
	BYTE	l_ddAnswer[ 512];
	DWORD	l_dwCountCmd          = 0;
	DWORD	l_dwCmdLength         = 0;
	DWORD	l_dwAnswerLength      = 0;
	BYTE	l_bySW1               = 0x00;
	BYTE	l_bySW2               = 0x00;
	BOOL    l_bResult             = FALSE;
	BOOL	l_bUniqueCmd          = FALSE;
	BOOL	l_bGoBeginTransaction = FALSE;
	BOOL	l_bGoEndTransaction   = FALSE;

	if (p_pbySW1 != NULL)
		(*p_pbySW1) = 0x00;
	if (p_pbySW2 != NULL)
		(*p_pbySW2) = 0x00;
	if (p_poCardReader == NULL)
		return FALSE;
	if ((p_dwListActions & ACTION_SELECT_APPLET_PKI) != 0)
		l_dwCountCmd++;
	if ((p_dwListActions & ACTION_GET_SERIALNUMBER) != 0)
		l_dwCountCmd++;
	if ((p_dwListActions & ACTION_VERIFY_PIN_ADMIN) != 0)
		l_dwCountCmd++;
/*	if ((p_dwListActions & ACTION_SEND_REAL_BIO) != 0)
		l_dwCountCmd+=2;
	else if ((p_dwListActions & ACTION_SEND_D2C_BIO) != 0)
		l_dwCountCmd++;
*/

	if (l_dwCountCmd == 1)
	{
		l_bResult = TRUE;
	    l_bUniqueCmd = TRUE;
	}
	else if (l_dwCountCmd > 0)
	{
		l_bResult = TRUE;
		l_bGoBeginTransaction = TRUE;
	}
	else
		return FALSE;

	if ((l_bResult == TRUE) && (l_bGoBeginTransaction == TRUE))
	{
		l_bResult = SCard_BeginTransaction( p_poCardReader);
		l_bGoEndTransaction = l_bResult;
	}

	if ((l_bResult == TRUE) && ((p_dwListActions & ACTION_SELECT_APPLET_PKI) != 0))
	{
		// Select Appli PKI (ypsID)
		memset( l_ddCmd, 0, 512);
		memset( l_ddAnswer, 0, 512);
		l_bySW1 = 0x00;
		l_bySW2 = 0x00;
		// prepare CMD
		memcpy( &l_ddCmd[0], "\x00\xA4\x04\x00\x00", 5);
		memcpy( &l_ddCmd[5], "\xA0\x00\x00\x02\x43\x00\x15\x00\x03\x01\x00\x01", 12);
		l_ddCmd[ NUM_BUFFER_INDEX_Lc] = 12;
		l_dwCmdLength = 5 + 12;
		l_dwAnswerLength = 256;
		// transmit CMD
		l_bResult = SCard_Transmit( p_poCardReader, l_bUniqueCmd, l_ddCmd, l_dwCmdLength, l_ddAnswer, &l_dwAnswerLength, &l_bySW1, &l_bySW2);
		if (p_pbySW1 != NULL)
			(*p_pbySW1) = l_bySW1;
		if (p_pbySW2 != NULL)
			(*p_pbySW2) = l_bySW2;
	}

	if ((l_bResult == TRUE) && ((p_dwListActions & ACTION_GET_SERIALNUMBER) != 0))
	{
		// Get TAG Serial Number (ypsID)
		memset( l_ddCmd, 0, 512);
		memset( l_ddAnswer, 0, 512);
		l_bySW1 = 0x00;
		l_bySW2 = 0x00;
		// prepare CMD
		memcpy( &l_ddCmd[0], "\x00\xCA\x00\x00\x00", 5);
		memcpy( &l_ddCmd[2], "\x01\x21", 2); // 		0x0121
		l_dwCmdLength = 5;
		l_ddCmd[ NUM_BUFFER_INDEX_Lc] = 0x00 + 8;
		l_dwAnswerLength = 8;
		// transmit CMD
		l_bResult = SCard_Transmit( p_poCardReader, l_bUniqueCmd, l_ddCmd, l_dwCmdLength, l_ddAnswer, &l_dwAnswerLength, &l_bySW1, &l_bySW2);
		if (p_pbySW1 != NULL)
			(*p_pbySW1) = l_bySW1;
		if (p_pbySW2 != NULL)
			(*p_pbySW2) = l_bySW2;

		if ((l_bResult == TRUE) && (l_dwAnswerLength != 0) && (p_psCardCtx != NULL))
		{
			memset( p_psCardCtx->ddScardSerialNumber, 0, 32);
			p_psCardCtx->dwScardSerialNumberLength = l_dwAnswerLength;
			memcpy( p_psCardCtx->ddScardSerialNumber, l_ddAnswer, l_dwAnswerLength);
		}
	}

	if ((l_bResult == TRUE) && ((p_dwListActions & ACTION_VERIFY_PIN_ADMIN) != 0))
	{
		BYTE	ll_ddAuthData[ 512];
		BYTE	ll_ddDiversificationData[ 16];
		int		ll_nResult          = 0;
		DWORD	ll_dwAuthDataLength = 0;
		DWORD	ll_dwLength         = 0;

		memset( ll_ddAuthData, 0, 512);
		memset( ll_ddDiversificationData, 0, 16);
		// Fill
		strcpy( (char*) ll_ddAuthData, "12345678");
		ll_dwAuthDataLength = (DWORD) strlen( (char*) ll_ddAuthData);
		if ((p_psCardCtx != NULL) && (p_psCardCtx->ddScardSerialNumber != NULL) && (p_psCardCtx->dwScardSerialNumberLength != 0) && (p_psCardCtx->dwScardSerialNumberLength == 8))
		{
			memcpy( &ll_ddDiversificationData[ 0], &p_psCardCtx->ddScardSerialNumber[2], 6);
			memset( &ll_ddDiversificationData[ 6], 0, 2);
			memcpy( &ll_ddDiversificationData[ 8], ll_ddDiversificationData, 8);
		}
		ll_ddDiversificationData[ 6]  = 0xF0;
		ll_ddDiversificationData[ 7]  = 0x03;
		ll_ddDiversificationData[ 14] = 0x0F;
		ll_ddDiversificationData[ 15] = 0x03;

		// Verify PIN (PIN_ADMIN) (ypsID)
		memset( l_ddCmd, 0, 512);
		memset( l_ddAnswer, 0, 512);
		l_bySW1 = 0x00;
		l_bySW2 = 0x00;
		// prepare CMD
		memcpy( &l_ddCmd[0], "\x00\x20\x01\x00\x00", 5);
		l_ddCmd[ NUM_BUFFER_INDEX_P2] = 0x19; // ref SDO PIN
		l_dwAnswerLength = 0;
		// cipher DATA
		ll_dwLength = 512 - 5;
		ll_nResult = Tool_Crypto_Cipher( YPSIDS2_DEFAULT_KDEK_DATA_DATA, YPSIDS2_DEFAULT_KDEK_DATA_SIZE,
										 MORPHO_MOC_DIVERSIFICATION_TYPE_1,
										 ll_ddDiversificationData, 16,
										 MORPHO_MOC_CIPHERED_3DES_ECB,
										 MORPHO_MOC_APDU_PADDING_TYPE_1,
										 ll_ddAuthData, ll_dwAuthDataLength,
										 &l_ddCmd[5], &ll_dwLength);

		l_dwCmdLength = 5 + ll_dwLength;
		l_ddCmd[ NUM_BUFFER_INDEX_Lc] = (BYTE) ll_dwLength;
		// transmit CMD

		l_bResult = SCard_Transmit( p_poCardReader, l_bUniqueCmd, l_ddCmd, l_dwCmdLength, l_ddAnswer, &l_dwAnswerLength, &l_bySW1, &l_bySW2);
		if (p_pbySW1 != NULL)
			(*p_pbySW1) = l_bySW1;
		if (p_pbySW2 != NULL)
			(*p_pbySW2) = l_bySW2;
	}

	if (l_bGoEndTransaction == TRUE)
	{
		SCard_EndTransaction( p_poCardReader);
	}

	return l_bResult;
}

///////////////////////////////////////////////////////////////
// ypsID S3 tools
//

# define YPSIDS3_BIOREADER_VERIFY_CMD_TO_TRACK     "\x80\x81\x82V\x08ypsIDS30"
# define YPSIDS3_BIOREADER_VERIFY_LEN_CMD_TO_TRACK (sizeof(YPSIDS3_BIOREADER_VERIFY_CMD_TO_TRACK)-1)

# define YPSIDS3_BIOREADER_ENROLL_CMD_TO_TRACK      "\x80\x81\x82W\x07ypsIDS3"
# define YPSIDS3_BIOREADER_ENROLL_LEN_CMD_TO_TRACK (sizeof(YPSIDS3_BIOREADER_ENROLL_CMD_TO_TRACK)-1)


BOOL Tool_ypsIDS3_FillParametersD2C ( T_MORPHO_MOC_APDU_PARAMETERS*	p_psD2Cparams, 
									  const BYTE*					p_ddBioTemplate,		
									  const DWORD					p_dwBioTemplateLength, 
									  const int						p_nIndexBIO_ToSet,	
									  const int						p_byPkRefFingerInfos,
									  const BOOL					p_bEnroll)
{
    BYTE	l_ddDataTmp[NUM_BUFFER_SIZE];      /* Buffer dans lequel on encode 7F2E L2 90 L1 PK */
    BYTE	l_ddTemplate[NUM_BUFFER_SIZE];  /* Buffer dans lequel on encode le Biometric Information Template */
    BOOL	l_bResult       = TRUE;
    DWORD	l_dwOffsetTmp   = 0;                /* Offset courant dans le buffer = taille des données dans le buffer */
    DWORD	l_dwOffsetBit   = 0;                /* Offset courant dans le buffer = taille des données dans le buffer */
    DWORD	l_dwLenPkData   = 0;                /* Longueur des données Pk (pour le cas ou les données Pks sont déjà encapsulées dans un 7F2E ) */
    DWORD	l_dwLenBefore   = 0;
    DWORD	l_dwLenAfter    = 0;
    
	if (p_psD2Cparams != NULL)
		memset( p_psD2Cparams, 0, sizeof(p_psD2Cparams));
	else
		return FALSE;
	if (p_dwBioTemplateLength == 0)
		return FALSE;

    /* 1) Encapsulation des Pks */
	memset( l_ddDataTmp, 0, NUM_BUFFER_SIZE );
	memset( l_ddTemplate, 0, NUM_BUFFER_SIZE );

	if (p_bEnroll == TRUE)
	{
		DWORD ll_dwLengthTemplate = 0;
		DWORD ll_dwLengthDOCP     = 0;
		DWORD ll_dwLengthData     = 0;
		DWORD ll_dwLengthObject   = 0;
		DWORD ll_dwTotalLength    = 0;
			
		// Taille apres tag 7F 2E
		ll_dwLengthTemplate =   1 + 2 + p_dwBioTemplateLength;
		//Taille après tag 7F 50
		ll_dwLengthDOCP = 2 + 2 + ll_dwLengthTemplate ;
		// Taille apres tag FF 1D 0x
		ll_dwLengthData = 2 + 2 + ll_dwLengthDOCP;
		//Taille après tag 70
		ll_dwLengthObject = 3 + 2 + ll_dwLengthData;
		// Taille totale des donnees envoyees a la carte
		ll_dwTotalLength = 1 + 2 + ll_dwLengthObject;

		l_ddDataTmp[ 0]	= 0x70;
		l_ddDataTmp[ 1]	= 0x81;
		l_ddDataTmp[ 2]  = (BYTE) (ll_dwLengthObject & 0xFF);	
		l_ddDataTmp[ 3]  = 0xBF; // indique que le tag formé est de type propriétaire
		l_ddDataTmp[ 4]  = 0x82; // classe de l'objet + portée de l'objet 																					         
		l_ddDataTmp[ 5]  = (BYTE)((p_nIndexBIO_ToSet) & 0xFF); // référence de l'objet
		l_ddDataTmp[ 6]  = 0x81; 
		l_ddDataTmp[ 7]  = (BYTE) (ll_dwLengthData & 0xFF);

		l_ddDataTmp[ 8]  = 0x7F; 
		l_ddDataTmp[ 9]  = 0x50; 			
		l_ddDataTmp[ 10] = 0x81; 
		l_ddDataTmp[ 11] = (BYTE) (ll_dwLengthDOCP & 0xFF);

		l_ddDataTmp[ 12] = 0x7F; 
		l_ddDataTmp[ 13] = 0x2E; 			
		l_ddDataTmp[ 14] = 0x81; 
		l_ddDataTmp[ 15] = (BYTE) (ll_dwLengthTemplate & 0xFF);

		l_ddDataTmp[ 16] = 0x81; // indique que le tag formé est de type propriétaire			
		l_ddDataTmp[ 17] = 0x81; 
		l_ddDataTmp[ 18] = (BYTE) (p_dwBioTemplateLength & 0xFF);

		l_dwLenBefore = 19;

		if (p_ddBioTemplate != NULL)
		{
			memcpy( &l_ddDataTmp[ l_dwLenBefore], p_ddBioTemplate, p_dwBioTemplateLength);
		}
		memcpy( l_ddTemplate, l_ddDataTmp, ll_dwTotalLength);
	}
	else
	{
		l_dwLenPkData  = (BYTE) ((p_dwBioTemplateLength + 7) & 0xFF);
		l_ddDataTmp[0] = 0x7F;
		l_ddDataTmp[1] = 0x2E;
		l_ddDataTmp[2] = 0x81;
		l_ddDataTmp[3] = (BYTE)((p_dwBioTemplateLength + 3) & 0xFF);
		l_ddDataTmp[4] = 0x81;
		l_ddDataTmp[5] = 0x81;
		l_ddDataTmp[6] = (BYTE)(p_dwBioTemplateLength & 0xFF);
		l_dwLenBefore = 7;

		if (p_ddBioTemplate != NULL)
		{
			memcpy( &l_ddDataTmp[ l_dwLenBefore], p_ddBioTemplate, p_dwBioTemplateLength);
		}
		memcpy( l_ddTemplate, l_ddDataTmp, l_dwLenPkData);
	}


    /* Everything went fine */
    if (l_bResult == TRUE)
    {
		T_MORPHO_MOC_APDU_PARAMETERS	ll_sD2Cparams;
		BYTE							ll_ddTrack[ 64];	
		BYTE							ll_ddCmd[ 10];	

		memset( &ll_sD2Cparams, 0, sizeof(T_MORPHO_MOC_APDU_PARAMETERS));
		memset( ll_ddTrack, 0, 64);
		memset( ll_ddCmd, 0, 10);

        if ( p_bEnroll == TRUE ) 
        {
			memcpy( ll_ddTrack, YPSIDS3_BIOREADER_ENROLL_CMD_TO_TRACK, YPSIDS3_BIOREADER_ENROLL_LEN_CMD_TO_TRACK);
			ll_sD2Cparams.m_puc_ApduToTrack    = ll_ddTrack;
			ll_sD2Cparams.m_i_ApduToTrackLen = YPSIDS3_BIOREADER_ENROLL_LEN_CMD_TO_TRACK;

			// Préparation de la commande PUT DATA
            ll_ddCmd[ NUM_BUFFER_INDEX_INS] = 0xDB;
            ll_ddCmd[ NUM_BUFFER_INDEX_P1]  = 0x3F;
			ll_ddCmd[ NUM_BUFFER_INDEX_P2]  = 0xFF;
			memcpy( &ll_ddCmd[ 5], ll_ddCmd, 5);
			ll_ddCmd[ 5 + NUM_BUFFER_INDEX_CLA] |= 0x10;
		}
		else
        {
			memcpy( ll_ddTrack, YPSIDS3_BIOREADER_VERIFY_CMD_TO_TRACK, YPSIDS3_BIOREADER_VERIFY_LEN_CMD_TO_TRACK);
			ll_ddTrack[ YPSIDS3_BIOREADER_VERIFY_LEN_CMD_TO_TRACK - 1] = (BYTE)(p_nIndexBIO_ToSet & 0xFF);
			ll_sD2Cparams.m_puc_ApduToTrack    = ll_ddTrack;
			ll_sD2Cparams.m_i_ApduToTrackLen = YPSIDS3_BIOREADER_VERIFY_LEN_CMD_TO_TRACK;

			// Préparation de la commande VERIFY
            ll_ddCmd[ NUM_BUFFER_INDEX_INS] = 0x21;
            ll_ddCmd[ NUM_BUFFER_INDEX_P1]  = 0x00;
			ll_ddCmd[ NUM_BUFFER_INDEX_P2]  = ((BYTE)(p_nIndexBIO_ToSet & 0xFF))  | 0x80;
			memcpy( &ll_ddCmd[ 5], ll_ddCmd, 5);
			ll_ddCmd[ 5 + NUM_BUFFER_INDEX_CLA] |= 0x10;
		}

		if (l_dwLenBefore != 0)
		{
			ll_sD2Cparams.m_puc_DataToWriteBeforeTemplate    = &l_ddTemplate[ 0];
			ll_sD2Cparams.m_i_DataToWriteBeforeTemplateLen = l_dwLenBefore;
		}
		if (l_dwLenAfter != 0)
		{
			ll_sD2Cparams.m_puc_DataToWriteBeforeTemplate  = &l_ddTemplate[ l_dwOffsetBit - l_dwLenAfter];
			ll_sD2Cparams.m_i_DataToWriteBeforeTemplateLen = l_dwLenAfter;
		}

		ll_sD2Cparams.m_puc_ApduHeaderToSend_Unique = ll_ddCmd;
		ll_sD2Cparams.m_puc_ApduHeaderToSend_First  = &ll_ddCmd[5];
		ll_sD2Cparams.m_puc_ApduHeaderToSend_Middle = &ll_ddCmd[5];
		ll_sD2Cparams.m_puc_ApduHeaderToSend_Last   = ll_ddCmd;

        ll_sD2Cparams.m_x_CipheredType = MORPHO_MOC_CIPHERED_NO;
        ll_sD2Cparams.m_x_PaddingType  = MORPHO_MOC_APDU_PADDING_TYPE_NO;
		ll_sD2Cparams.m_x_ChainType    = MORPHO_MOC_APDU_CHAIN_TYPE_1;

		if (Tool_MorphoParametersD2C_Duplicate( p_psD2Cparams, &ll_sD2Cparams) != MORPHO_OK)
			l_bResult = FALSE;
    }

    return l_bResult;
}

BOOL Tool_ypsIDS3_SCard_Action( CSCardReader* p_poCardReader, SmartCardCtx* p_psCardCtx, const DWORD p_dwListActions, const T_MORPHO_MOC_APDU_PARAMETERS* p_psD2Cparams, const BYTE* p_ddBioTemplate, const DWORD p_dwBioTemplateLength, BYTE* p_pbySW1, BYTE* p_pbySW2)
{
	BYTE	l_ddCmd[ 512];
	BYTE	l_ddAnswer[ 512];
	DWORD	l_dwCountCmd          = 0;
	DWORD	l_dwCmdLength         = 0;
	DWORD	l_dwAnswerLength      = 0;
	BYTE	l_bySW1               = 0x00;
	BYTE	l_bySW2               = 0x00;
	BOOL    l_bResult             = FALSE;
	BOOL	l_bUniqueCmd          = FALSE;
	BOOL	l_bGoBeginTransaction = FALSE;
	BOOL	l_bGoEndTransaction   = FALSE;

	if (p_pbySW1 != NULL)
		(*p_pbySW1) = 0x00;
	if (p_pbySW2 != NULL)
		(*p_pbySW2) = 0x00;
	if (p_poCardReader == NULL)
		return FALSE;
	if ((p_dwListActions & ACTION_SELECT_APPLET_PKI) != 0)
		l_dwCountCmd++;
	if ((p_dwListActions & ACTION_GET_SERIALNUMBER) != 0)
		l_dwCountCmd++;
	if ((p_dwListActions & ACTION_VERIFY_PIN_ADMIN) != 0)
		l_dwCountCmd++;

	if (l_dwCountCmd == 1)
	{
		l_bResult = TRUE;
	    l_bUniqueCmd = TRUE;
	}
	else if (l_dwCountCmd > 0)
	{
		l_bResult = TRUE;
		l_bGoBeginTransaction = TRUE;
	}
	else
		return FALSE;

	if ((l_bResult == TRUE) && (l_bGoBeginTransaction == TRUE))
	{
		l_bResult = SCard_BeginTransaction( p_poCardReader);
		l_bGoEndTransaction = l_bResult;
	}
	if ((l_bResult == TRUE) && ((p_dwListActions & ACTION_SELECT_APPLET_PKI) != 0))
	{
		// Select Appli PKI (ypsID)
		memset( l_ddCmd, 0, 512);
		memset( l_ddAnswer, 0, 512);
		l_bySW1 = 0x00;
		l_bySW2 = 0x00;
		// prepare CMD
		memcpy( &l_ddCmd[0], "\x00\xA4\x04\x0C\x00", 5);
		memcpy( &l_ddCmd[5], "\xA0\x00\x00\x00\x00\x79\x70\x73\x49\x44", 10);
		l_ddCmd[ NUM_BUFFER_INDEX_Lc] = 10;
		l_dwCmdLength = 5 + 10;
		l_dwAnswerLength = 256;
		// transmit CMD
		l_bResult = SCard_Transmit( p_poCardReader, l_bUniqueCmd, l_ddCmd, l_dwCmdLength, l_ddAnswer, &l_dwAnswerLength, &l_bySW1, &l_bySW2);
		if (p_pbySW1 != NULL)
			(*p_pbySW1) = l_bySW1;
		if (p_pbySW2 != NULL)
			(*p_pbySW2) = l_bySW2;
	}
	if ((l_bResult == TRUE) && ((p_dwListActions & ACTION_GET_SERIALNUMBER) != 0))
	{
		// Get TAG Serial Number (ypsID)
		memset( l_ddCmd, 0, 512);
		memset( l_ddAnswer, 0, 512);
		l_bySW1 = 0x00;
		l_bySW2 = 0x00;
		// prepare CMD
		memcpy( &l_ddCmd[0], "\x00\xCA\x00\x00\x00", 5);
		memcpy( &l_ddCmd[2], "\xDF\x5B", 2); // 		0xDF5B
		l_dwCmdLength = 5;
		l_ddCmd[ NUM_BUFFER_INDEX_Lc] = 0x00 + 8;
		l_dwAnswerLength = 8;
		// transmit CMD
		l_bResult = SCard_Transmit( p_poCardReader, l_bUniqueCmd, l_ddCmd, l_dwCmdLength, l_ddAnswer, &l_dwAnswerLength, &l_bySW1, &l_bySW2);
		if (p_pbySW1 != NULL)
			(*p_pbySW1) = l_bySW1;
		if (p_pbySW2 != NULL)
			(*p_pbySW2) = l_bySW2;

		if ((l_bResult == TRUE) && (l_dwAnswerLength != 0) && (p_psCardCtx != NULL))
		{
			memset( p_psCardCtx->ddScardSerialNumber, 0, 32);
			p_psCardCtx->dwScardSerialNumberLength = l_dwAnswerLength;
			memcpy( p_psCardCtx->ddScardSerialNumber, l_ddAnswer, l_dwAnswerLength);
		}
	}
	if ((l_bResult == TRUE) && ((p_dwListActions & ACTION_VERIFY_PIN_ADMIN) != 0))
	{
		BYTE	ll_ddAuthData[ 512];
		DWORD	ll_dwAuthDataLength = 0;
		DWORD	ll_dwLength         = 0;

		memset( ll_ddAuthData, 0, 512);
		// Fill
		strcpy( (char*) ll_ddAuthData, "12345678");
		ll_dwAuthDataLength = (DWORD) strlen( (char*) ll_ddAuthData);

		// Verify PIN (PIN_ADMIN) (ypsID)
		memset( l_ddCmd, 0, 512);
		memset( l_ddAnswer, 0, 512);
		l_bySW1 = 0x00;
		l_bySW2 = 0x00;
		// prepare CMD
		memcpy( &l_ddCmd[0], "\x00\x20\x00\x00\x00", 5);
		l_ddCmd[ NUM_BUFFER_INDEX_P2] = 0x02 | 0x80; // ref SDO PIN
		l_dwAnswerLength = 0;
		// set DATA
		memcpy( &l_ddCmd[5], ll_ddAuthData, ll_dwAuthDataLength);
		ll_dwLength = ll_dwAuthDataLength;

		l_dwCmdLength = 5 + ll_dwLength;
		l_ddCmd[ NUM_BUFFER_INDEX_Lc] = (BYTE) ll_dwLength;
		// transmit CMD

		l_bResult = SCard_Transmit( p_poCardReader, l_bUniqueCmd, l_ddCmd, l_dwCmdLength, l_ddAnswer, &l_dwAnswerLength, &l_bySW1, &l_bySW2);
		if (p_pbySW1 != NULL)
			(*p_pbySW1) = l_bySW1;
		if (p_pbySW2 != NULL)
			(*p_pbySW2) = l_bySW2;
	}

	if (l_bGoEndTransaction == TRUE)
	{
		SCard_EndTransaction( p_poCardReader);
	}

	return l_bResult;
}

BOOL Tool_Send_ChainedApdu_ToCard(CSCardReader* p_poCardReader, SmartCardCtx* p_psCardCtx, const T_MORPHO_MOC_APDU_PARAMETERS* p_psD2Cparams, const BYTE* p_ddBioTemplate, const DWORD p_dwBioTemplateLength, BYTE* p_pbySW1, BYTE* p_pbySW2)
{
	BOOL    l_bResult = TRUE;
	BYTE	l_ddCmd[512];
	BYTE	l_ddAnswer[512];
	BYTE	l_bySW1 = 0x00;
	BYTE	l_bySW2 = 0x00;
	BOOL	l_bUniqueCmd = FALSE;
	DWORD	l_dwAnswerLength = 0;
	DWORD	ll_dwDataLentgh = 0;
	DWORD	ll_dwOffset = 0;

	if (p_pbySW1 != NULL)
		(*p_pbySW1) = 0x00;
	if (p_pbySW2 != NULL)
		(*p_pbySW2) = 0x00;
	if (p_poCardReader == NULL)
		return FALSE;

	l_bResult = SCard_BeginTransaction(p_poCardReader);

	// Buid APDU to Send
	memset(l_ddCmd, 0, 512);
	memset(l_ddAnswer, 0, 512);
	l_bySW1 = 0x00;
	l_bySW2 = 0x00;

	// prepare CMD APDU
	memcpy(&l_ddCmd[0], p_psD2Cparams->m_puc_ApduHeaderToSend_Unique, 5);
	ll_dwOffset = 5;
	if ((p_psD2Cparams->m_puc_DataToWriteBeforeTemplate != NULL) && (p_psD2Cparams->m_i_DataToWriteBeforeTemplateLen != 0))
	{
		memcpy(&l_ddCmd[ll_dwOffset], p_psD2Cparams->m_puc_DataToWriteBeforeTemplate, p_psD2Cparams->m_i_DataToWriteBeforeTemplateLen);
		ll_dwOffset += p_psD2Cparams->m_i_DataToWriteBeforeTemplateLen;
	}
	if ((p_ddBioTemplate != NULL) && (p_dwBioTemplateLength != 0))
	{
		memcpy(&l_ddCmd[ll_dwOffset], p_ddBioTemplate, p_dwBioTemplateLength);
		ll_dwOffset += p_dwBioTemplateLength;
	}
	if ((p_psD2Cparams->m_puc_DataToWriteAfterTemplate != NULL) && (p_psD2Cparams->m_i_DataToWriteAfterTemplateLen != 0))
	{
		memcpy(&l_ddCmd[ll_dwOffset], p_psD2Cparams->m_puc_DataToWriteAfterTemplate, p_psD2Cparams->m_i_DataToWriteAfterTemplateLen);
		ll_dwOffset += p_psD2Cparams->m_i_DataToWriteAfterTemplateLen;
	}
	ll_dwDataLentgh = ll_dwOffset - 5;

	// Case of unique CMD APDU
	if (ll_dwDataLentgh <= m_i_ApduMaxLen)
	{
		l_dwAnswerLength = 0;
		// transmit CMD
		l_bResult = SCard_Transmit(p_poCardReader, l_bUniqueCmd, l_ddCmd, ll_dwOffset, l_ddAnswer, &l_dwAnswerLength, &l_bySW1, &l_bySW2);
		if (p_pbySW1 != NULL)
			(*p_pbySW1) = l_bySW1;
		if (p_pbySW2 != NULL)
			(*p_pbySW2) = l_bySW2;
	}
	// Case of chained CMD APDU
	else
	{
		UC	l_puc_ApduComplet[512];	// pointeur sur l'APDU complet a envoyer (entete et datas)
		I	l_i_ApduCompletLen;	// Taille de l'APDU complet a envoyer (entete et datas)
		I	l_i_ApduDataLen;	// Taille des Datas de l'APDU
		I	l_i_ApduDataEmitted = 0;
		I	l_i_counter = 0;

		while (l_i_ApduDataEmitted < ll_dwDataLentgh)
		{
			l_i_counter++;
			if ((ll_dwDataLentgh - l_i_ApduDataEmitted) > m_i_ApduMaxLen)
			{
				// Il faut chainer
				l_i_ApduDataLen = m_i_ApduMaxLen;
			}
			else
			{
				l_i_ApduDataLen = (I)(ll_dwDataLentgh - l_i_ApduDataEmitted);
			}

			// Copie de l'entete de l'APDU,
			if (l_i_counter == 1)
			{
				memcpy(l_puc_ApduComplet, p_psD2Cparams->m_puc_ApduHeaderToSend_First, APDU_HEADER_LEN);
			}
			else if ((ll_dwDataLentgh - l_i_ApduDataEmitted) > m_i_ApduMaxLen)
			{
				memcpy(l_puc_ApduComplet, p_psD2Cparams->m_puc_ApduHeaderToSend_Middle, APDU_HEADER_LEN);
			}
			else
			{
				memcpy(l_puc_ApduComplet, p_psD2Cparams->m_puc_ApduHeaderToSend_Last, APDU_HEADER_LEN);
			}

			// Copie des donnees de l'APDU a envoyer pour ce chainage
			memcpy(l_puc_ApduComplet + APDU_HEADER_LEN, &l_ddCmd[5] + l_i_ApduDataEmitted, l_i_ApduDataLen);

			l_i_ApduCompletLen = l_i_ApduDataLen + APDU_HEADER_LEN;

			if (l_bResult == TRUE)
			{
				// decrementation du nombre emis
				l_i_ApduDataEmitted += l_i_ApduDataLen;

				l_dwAnswerLength = 0;
				// transmit CMD
				l_bResult = SCard_Transmit(p_poCardReader, l_bUniqueCmd, l_puc_ApduComplet, l_i_ApduCompletLen, l_ddAnswer, &l_dwAnswerLength, &l_bySW1, &l_bySW2);
				if (p_pbySW1 != NULL)
					(*p_pbySW1) = l_bySW1;
				if (p_pbySW2 != NULL)
					(*p_pbySW2) = l_bySW2;
			}
		}
	}

	SCard_EndTransaction(p_poCardReader);

	return l_bResult;
}

BOOL Tool_Length_ChainedApdu(T_MORPHO_MOC_APDU_PARAMETERS* p_psD2Cparams, const DWORD p_dwBioTemplateLength)
{
	BOOL	l_bResult = TRUE;

	DWORD	ll_dwDataLentgh = p_dwBioTemplateLength + p_psD2Cparams->m_i_DataToWriteBeforeTemplateLen + p_psD2Cparams->m_i_DataToWriteAfterTemplateLen;

		// Case of unique CMD APDU
		if (ll_dwDataLentgh <= m_i_ApduMaxLen)
		{
			p_psD2Cparams->m_puc_ApduHeaderToSend_Unique[NUM_BUFFER_INDEX_Lc] = (BYTE)(ll_dwDataLentgh);
		}
		// Case of chained CMD APDU
		else
		{
			I	l_i_ApduDataLen;	// Taille des Datas de l'APDU
			I	l_i_ApduDataEmitted = 0;
			I	l_i_counter = 0;

			while (l_i_ApduDataEmitted < ll_dwDataLentgh)
			{
				if ((ll_dwDataLentgh - l_i_ApduDataEmitted) > m_i_ApduMaxLen)
				{
					// Il faut chainer
					l_i_ApduDataLen = m_i_ApduMaxLen;
				}
				else
				{
					l_i_ApduDataLen = (I)(ll_dwDataLentgh - l_i_ApduDataEmitted);
				}

				// decrementation du nombre emis
				l_i_counter++;
				l_i_ApduDataEmitted += l_i_ApduDataLen;

				switch (l_i_counter)
				{
				case 1:
					p_psD2Cparams->m_puc_ApduHeaderToSend_First[NUM_BUFFER_INDEX_Lc] = (BYTE)(l_i_ApduDataLen);
					break;
				case 2:
					if (l_i_ApduDataEmitted < ll_dwDataLentgh)
						p_psD2Cparams->m_puc_ApduHeaderToSend_Middle[NUM_BUFFER_INDEX_Lc] = (BYTE)(l_i_ApduDataLen);
					else
						p_psD2Cparams->m_puc_ApduHeaderToSend_Last[NUM_BUFFER_INDEX_Lc] = (BYTE)(l_i_ApduDataLen);
					break;
				case 3:
					p_psD2Cparams->m_puc_ApduHeaderToSend_Last[NUM_BUFFER_INDEX_Lc] = (BYTE)(l_i_ApduDataLen);
					break;
				default:
					l_bResult = FALSE;
					break;
				}	
			}
		}
	return l_bResult;
}

void Update_ApduMaxLength(I i_i_ApduMaxLength)
{
	m_i_ApduMaxLen = i_i_ApduMaxLength;
}


