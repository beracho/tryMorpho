/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// ConvertSDKError.cpp : implementation file
//

#include "stdafx.h"

#include "Morpho_Interface.h"
#include "MORPHO_Types.h"
#include "MORPHO_Image.h"
#include "MORPHO_Errors.h"
#include "ConvertSDKError.h"

bool ConvertSDKError(I i_i_Error, CString &o_cs_Message)
{
	switch(i_i_Error)
	{
		case MORPHO_OK	:
			o_cs_Message.Format("MORPHO_OK");
			break;
		case MORPHOERR_INTERNAL	:	
			o_cs_Message.Format("MORPHOERR_INTERNAL");
			break;
		case MORPHOERR_PROTOCOLE	:	
			o_cs_Message.Format("MORPHOERR_PROTOCOLE");
			break;		
		case MORPHOERR_CONNECT		:
			o_cs_Message.Format("MORPHOERR_CONNECT");
			break;		
		case MORPHOERR_CLOSE_COM	:
			o_cs_Message.Format("MORPHOERR_CLOSE_COM");
			break;			
		case MORPHOERR_BADPARAMETER	:
			o_cs_Message.Format("MORPHOERR_BADPARAMETER");
			break;		
		case MORPHOERR_MEMORY_PC	:
			o_cs_Message.Format("MORPHOERR_MEMORY_PC");
			break;			
		case MORPHOERR_MEMORY_DEVICE:
			o_cs_Message.Format("MORPHOERR_MEMORY_DEVICE");
			break;			
		case MORPHOERR_NO_HIT		:
			o_cs_Message.Format("Bad Capture Sequence.");
			break;		
		case MORPHOERR_STATUS		:
			o_cs_Message.Format("MORPHOERR_STATUS");
			break;		
		case MORPHOERR_DB_FULL		:
			o_cs_Message.Format("Database is full !");
			break;		
		case MORPHOERR_DB_EMPTY		:
			o_cs_Message.Format("Base is empty !");
			break;		
		case MORPHOERR_ALREADY_ENROLLED	:
			o_cs_Message.Format("User already enrolled.");
			break;	
		case MORPHOERR_BASE_NOT_FOUND	:
			o_cs_Message.Format("Base does not exist.");
			break;	
		case MORPHOERR_BASE_ALREADY_EXISTS	:
			o_cs_Message.Format("Base already exists.");
			break;
		case MORPHOERR_NO_ASSOCIATED_DB	:	
			o_cs_Message.Format("MORPHOERR_NO_ASSOCIATED_DB");
			break;
		case MORPHOERR_NO_ASSOCIATED_DEVICE	:
			o_cs_Message.Format("MORPHOERR_NO_ASSOCIATED_DEVICE");
			break;
		case MORPHOERR_INVALID_TEMPLATE	:	
			o_cs_Message.Format("MORPHOERR_INVALID_TEMPLATE");
			break;
		case MORPHOERR_NOT_IMPLEMENTED	:	
			o_cs_Message.Format("MORPHOERR_NOT_IMPLEMENTED");
			break;
		case MORPHOERR_TIMEOUT		:	
			o_cs_Message.Format("Timeout has expired.\nCommand aborted.");
			break;	
		case MORPHOERR_NO_REGISTERED_TEMPLATE:
			o_cs_Message.Format("MORPHOERR_NO_REGISTERED_TEMPLATE");
			break;
		case MORPHOERR_FIELD_NOT_FOUND	:
			o_cs_Message.Format("MORPHOERR_FIELD_NOT_FOUND");
			break;	
		case MORPHOERR_CORRUPTED_CLASS	:
			o_cs_Message.Format("MORPHOERR_CORRUPTED_CLASS");
			break;	
		case MORPHOERR_TO_MANY_TEMPLATE	:
			o_cs_Message.Format("MORPHOERR_TO_MANY_TEMPLATE");
			break;	
		case MORPHOERR_TO_MANY_FIELD	:
			o_cs_Message.Format("MORPHOERR_TO_MANY_FIELD");
			break;		
		case MORPHOERR_MIXED_TEMPLATE	:
			o_cs_Message.Format("MORPHOERR_MIXED_TEMPLATE");
			break;	
		case MORPHOERR_CMDE_ABORTED		:
			o_cs_Message.Format("Command aborted by user.");
			break;	
		case MORPHOERR_INVALID_PK_FORMAT:
			o_cs_Message.Format("Invalid PK format");
			break;	
		case MORPHOERR_SAME_FINGER:
			o_cs_Message.Format("User gave twice the same finger");
			break;
		case MORPHOERR_OUT_OF_FIELD:
			o_cs_Message.Format("The number of the additional field is more than 128");
			break;
		case MORPHOERR_INVALID_USER_ID	:
			o_cs_Message.Format("A person with the same\nuser ID is already enrolled\nin the database.");
			break;	
		case MORPHOERR_INVALID_USER_DATA:
			o_cs_Message.Format("MORPHOERR_INVALID_USER_DATA");
			break;		
		case MORPHOERR_FIELD_INVALID	:
			o_cs_Message.Format("MORPHOERR_FIELD_INVALID");
			break;
		case MORPHOERR_USER_NOT_FOUND	:
			o_cs_Message.Format("MORPHOERR_USER_NOT_FOUND");
			break;
		case MORPHOERR_COM_NOT_OPEN	:
			o_cs_Message.Format("MORPHOERR_COM_NOT_OPEN");
			break;
		case MORPHOERR_ELT_ALREADY_PRESENT	:
			o_cs_Message.Format("MORPHOERR_ELT_ALREADY_PRESENT");
			break;
		case MORPHOERR_NOCALLTO_DBQUERRYFIRST	:
			o_cs_Message.Format("MORPHOERR_NOCALLTO_DBQUERRYFIRST");
			break;
		case MORPHOERR_USER	:
			o_cs_Message.Format("MORPHOERR_USER");
			break;
		case MORPHOERR_BAD_COMPRESSION	:
			o_cs_Message.Format("MORPHOERR_BAD_COMPRESSION");
			break;
		case MORPHOERR_SECU	:
			o_cs_Message.Format("MORPHOERR_SECU");
			break;
		case MORPHOERR_CERTIF_UNKNOW	:
			o_cs_Message.Format("MORPHOERR_CERTIF_UNKNOW");
			break;
		case MORPHOERR_INVALID_CLASS	:
			o_cs_Message.Format("MORPHOERR_INVALID_CLASS");
			break;
		case MORPHOERR_USB_DEVICE_NAME_UNKNOWN	:
			o_cs_Message.Format("MORPHOERR_USB_DEVICE_NAME_UNKNOWN");
			break;
		case MORPHOERR_CERTIF_INVALID	:
			o_cs_Message.Format("MORPHOERR_CERTIF_INVALID");
			break;
		case MORPHOERR_SIGNER_ID	:
			o_cs_Message.Format("MORPHOERR_SIGNER_ID");
			break;
		case MORPHOERR_SIGNER_ID_INVALID	:
			o_cs_Message.Format("MORPHOERR_SIGNER_ID_INVALID");
			break;
		case MORPHOERR_FFD	:
			o_cs_Message.Format("MORPHOERR_FFD");
			break;
		case MORPHOERR_MOIST_FINGER	:
			o_cs_Message.Format("MORPHOERR_MOIST_FINGER");
			break;
		case MORPHOERR_NO_SERVER	:
			o_cs_Message.Format("MORPHOERR_NO_SERVER");
			break;
		case MORPHOERR_OTP_NOT_INITIALIZED	:
			o_cs_Message.Format("MORPHOERR_OTP_NOT_INITIALIZED");
			break;
		case MORPHOERR_OTP_PIN_NEEDED	:
			o_cs_Message.Format("MORPHOERR_OTP_PIN_NEEDED");
			break;
		case MORPHOERR_OTP_REENROLL_NOT_ALLOWED	:
			o_cs_Message.Format("MORPHOERR_OTP_REENROLL_NOT_ALLOWED");
			break;
		case MORPHOERR_OTP_ENROLL_FAILED	:
			o_cs_Message.Format("MORPHOERR_OTP_ENROLL_FAILED");
			break;
		case MORPHOERR_OTP_IDENT_FAILED	:
			o_cs_Message.Format("MORPHOERR_OTP_IDENT_FAILED");
			break;
		case MORPHOERR_NO_MORE_OTP	:
			o_cs_Message.Format("MORPHOERR_NO_MORE_OTP");
			break;
		case MORPHOERR_OTP_NO_HIT	:
			o_cs_Message.Format("MORPHOERR_OTP_NO_HIT");
			break;
		case MORPHOERR_OTP_ENROLL_NEEDED	:
			o_cs_Message.Format("MORPHOERR_OTP_ENROLL_NEEDED");
			break;
		case MORPHOERR_DEVICE_LOCKED	:
			o_cs_Message.Format("MORPHOERR_DEVICE_LOCKED");
			break;
		case MORPHOERR_DEVICE_NOT_LOCK	:
			o_cs_Message.Format("MORPHOERR_DEVICE_NOT_LOCK");
			break;
		default:
			o_cs_Message.Format("MORPHOERR");
			return FALSE;
	}		
	return TRUE;
}
