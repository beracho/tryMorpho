/***********************************************************
  Copyright (c) 2002, SAGEM SA
************************************************************

 PROJET      : MorphoSmart
 MODULE      : 

 FILE NAME   : $Workfile:   Smartcard.cpp  $

 REVISION    : $Revision:   1.6  $

 DATE        : $Date:   Sep 15 2004 16:51:46  $

************************************************************/

#include "stdafx.h"
#include "MORPHO_Types.h"
#include <Winscard.h>
#include "Smartcard.h"
#include "Smartcard_Interface.h"

#ifdef _DEBUG
#undef THIS_FILE
static C THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ATR: 3b6f0000805453494d44620531711283039000

//////////////////////////////////////////////////////////////////
CSmartcard::CSmartcard() 
{
	L	rc;

	m_outputBufferLen = m_responseLen = 0;
	m_hCard = NULL;
	m_hContext = NULL;
	m_isConnected = FALSE;
	m_cla = 0x00;
	m_lifePhase = 0xFF; // unknown life phase
	if ((rc = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &m_hContext)) != SCARD_S_SUCCESS)
	{
		m_hContext = NULL;		
	}
}

//////////////////////////////////////////////////////////////////
CSmartcard::~CSmartcard()
{
	Disconnect(SCARD_UNPOWER_CARD);
	SCardReleaseContext(m_hContext);
	m_hContext = NULL;
}

//////////////////////////////////////////////////////////////////
BOOL CSmartcard::Connect(PC readerNameToUse)
{
	CString	readerName;
	L		rc;
	UL		readerNameLen=0, dwState, dwProtocol;

	if (readerNameToUse == NULL)
	{
		return FALSE;
	}
	else
		readerName = readerNameToUse;
	if ((rc = SCardConnect(m_hContext, readerName, SCARD_SHARE_EXCLUSIVE, 
		SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &m_hCard, &m_protocol)) != SCARD_S_SUCCESS)
	{
		return FALSE;
	}
	memset (m_atr, 0, 20);
	m_atrLen = 20;

	SCardStatus(m_hCard, NULL, &readerNameLen, &dwState, &dwProtocol, m_atr, &m_atrLen);
	m_isConnected = TRUE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////
BOOL CSmartcard::Disconnect(UL disposition)
{
	L			rc;

	if (m_isConnected != TRUE)
		return TRUE;
	if ((rc = SCardDisconnect(m_hCard, disposition)) != SCARD_S_SUCCESS)
	{
		return FALSE;
	}
	m_isConnected = FALSE;
	m_atrLen = 0;
	return TRUE;
}

//////////////////////////////////////////////////////////////////
BOOL CSmartcard::GetListReaders(CPtrList *list)
{
	UL	protocol;
	LONG lReturn2;
	LONG	rc;

	SCARDHANDLE	l_hCard = NULL;
	DWORD readerLen=SCARD_AUTOALLOCATE;

	if (list == NULL) 
	{
		return FALSE;
	}
	readerLen = SCARD_AUTOALLOCATE;

	LPTSTR          pmszReaders = NULL;
	LPTSTR          pReader = NULL;

	rc = SCardListReaders(m_hContext, NULL, (LPTSTR)&pmszReaders, &readerLen);
	
	
	switch(rc)
	{
		case SCARD_S_SUCCESS :
			pReader=pmszReaders;
			while('\0'!=*pReader)
			{
				rc = SCardConnect(m_hContext, pReader, SCARD_SHARE_EXCLUSIVE, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &l_hCard, &protocol);
				if (rc != SCARD_E_UNKNOWN_READER)
				{
					list->AddTail(strdup(pReader));
					SCardDisconnect(l_hCard, SCARD_LEAVE_CARD);
				}
				pReader += strlen(pReader)+1;
			}
			lReturn2=SCardFreeMemory(m_hContext,pmszReaders);
			if(lReturn2!=SCARD_S_SUCCESS)
				printf("FailedScardFreeMemory");
			break;
		default:
			printf("FailedScardListReaders");
			break;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////
BOOL CSmartcard::GetReader (PC o_pc_readerName)
{
	CPtrList	listReader;	
	C			*reader;
	BOOL		rc = TRUE;

	if (GetListReaders(&listReader) == FALSE)
		return FALSE;
	
	if (listReader.IsEmpty() == TRUE)
	{
		return FALSE;
	}	
	
	strcpy (o_pc_readerName, (PC)listReader.GetHead());

	while (listReader.IsEmpty() == FALSE)
	{
		reader = (C *)listReader.RemoveHead();
		free(reader);
	}
	return rc;
}

//////////////////////////////////////////////////////////////////
BOOL CSmartcard::IsCardPresent(PC readerName)
{
	L					rc;
	SCARD_READERSTATE	state;

	state.szReader = readerName;
	state.dwCurrentState = SCARD_STATE_EMPTY;
	rc = SCardGetStatusChange(m_hContext, 100, &state, 1);
	if (rc == SCARD_S_SUCCESS && (state.dwEventState & SCARD_STATE_PRESENT) )
	{
		if(state.dwEventState & SCARD_STATE_MUTE )
			return FALSE; // The card may be upside down

		return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////
L	CSmartcard::SendCommand (PT__APDU i_pst_apdu, PT__APDU_RESULT o_pst_result)
{
	L	l_l_res;
	
	if (SCardBeginTransaction(m_hCard) != SCARD_S_SUCCESS)
	{
		l_l_res = SMARTCARD_ERROR;
	}
	else
	{
		UC	l_puc_transmit [300];
		UC	l_puc_receive  [300];
		UL	l_ul_transmit_size;
		UL	l_ul_receive_size;
		
		// Build Transmit buffer
		l_puc_transmit[0] = i_pst_apdu->m_uc_cla;	// CLA
		l_puc_transmit[1] = i_pst_apdu->m_uc_ins;	// INS
		l_puc_transmit[2] = i_pst_apdu->m_uc_p1;	// P1
		l_puc_transmit[3] = i_pst_apdu->m_uc_p2;	// P2
		l_puc_transmit[4] = i_pst_apdu->m_uc_p3;	// P3
		memcpy (l_puc_transmit+5, i_pst_apdu->m_puc_data, i_pst_apdu->m_uc_length);
		
		// Compute total length of data to send : (CLA + INS + PA + P2 + P3 + data)
		l_ul_transmit_size = 5 + i_pst_apdu->m_uc_length; 
		l_ul_receive_size = 300;

		l_l_res = SCardTransmit (m_hCard,  
								 SCARD_PCI_T0, l_puc_transmit, l_ul_transmit_size,
								 NULL,		   l_puc_receive,  &l_ul_receive_size );

		if (l_l_res == SCARD_S_SUCCESS)
		{
			o_pst_result->m_uc_sw1 = l_puc_receive[l_ul_receive_size-2];
			o_pst_result->m_uc_sw2 = l_puc_receive[l_ul_receive_size-1];
			o_pst_result->m_uc_length = (UC)(l_ul_receive_size-2);
			memcpy (o_pst_result->m_puc_data, l_puc_receive, o_pst_result->m_uc_length);
			
			
			switch (o_pst_result->m_uc_sw1)
			{
			case 0x90:
				l_l_res = SMARTCARD_OK; 
				break;
			case 0x61:
				//send get response : INS = 0xC0
				l_puc_transmit[0] = i_pst_apdu->m_uc_cla;	// CLA
				l_puc_transmit[1] = 0xC0;					// INS
				l_puc_transmit[2] = 0;	// P1
				l_puc_transmit[3] = 0;	// P2
				l_puc_transmit[4] = o_pst_result->m_uc_sw2;	// Le Filed

				l_ul_transmit_size = 5; 
				l_ul_receive_size = 300;
				
				l_l_res = SCardTransmit (m_hCard,  
					SCARD_PCI_T0, l_puc_transmit, l_ul_transmit_size,
					NULL,		   l_puc_receive,  &l_ul_receive_size );

				o_pst_result->m_uc_sw1 = l_puc_receive[l_ul_receive_size-2];
				o_pst_result->m_uc_sw2 = l_puc_receive[l_ul_receive_size-1];
				o_pst_result->m_uc_length = (UC)(l_ul_receive_size-2);
				memcpy (o_pst_result->m_puc_data, l_puc_receive, o_pst_result->m_uc_length);
				
				break;
			default:
				l_l_res = SMARTCARD_ERROR;;
				break;
			}

			if (SCardEndTransaction (m_hCard, SCARD_LEAVE_CARD) != SCARD_S_SUCCESS)
			{
				// Just Trace
				// TRACE_WARNING ("Error While ending SmartCard Transaction !");
			}

		}
		else
		{
			// Command failed !
			l_l_res = SMARTCARD_ERROR;
		}
	}
	return l_l_res;
}


//////////////////////////////////////////////////////////////////
L	CSmartcard::Transmit(UC *input, UL inputLen, PC funcName)
{
	L		rc;
	CString	str;

	if ((rc = SCardBeginTransaction(m_hCard)) != SCARD_S_SUCCESS)
	{
		// return with an explicit error message if necessary
	}


	m_outputBufferLen = 260;
	if (rc == SCARD_S_SUCCESS &&
		(rc = SCardTransmit(m_hCard, SCARD_PCI_T0, (UC *)input, inputLen, 
		NULL, m_outputBuffer, &m_outputBufferLen)) != SCARD_S_SUCCESS)
	{
		// return with an explicit error message if necessary
	}
	if (rc == SCARD_S_SUCCESS &&
		SCardEndTransaction(m_hCard, SCARD_LEAVE_CARD) != SCARD_S_SUCCESS)
	{
		// return with an explicit error message if necessary
	}
	if (rc != SCARD_S_SUCCESS) 
		return rc;
	rc = (m_outputBuffer[m_outputBufferLen-2] << 8) + m_outputBuffer[m_outputBufferLen-1];
	if (m_outputBuffer[m_outputBufferLen-2] == MORE_DATA)
		m_responseLen = m_outputBuffer[m_outputBufferLen-1];

	return rc;
}

//////////////////////////////////////////////////////////////////
BOOL CSmartcard::GetAndVerifyPINCode(UC typeCHV)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////
L	CSmartcard::GetATR (PUC o_puc_buffer, PUL io_pul_size)
{
	// Check we have an ATR available
	if (m_atrLen == 0)
	{
		return SMARTCARD_NO_ATR;
	}
	
	// Check arguments
	if ((o_puc_buffer == NULL) || (*io_pul_size < m_atrLen))
	{
		return SMARTCARD_BADPARAMETER;
	}

	// Copy ATR to output buffer;
	memcpy (o_puc_buffer, m_atr, m_atrLen);
	*io_pul_size = m_atrLen;

	return SMARTCARD_OK;
}

//////////////////////////////////////////////////////////////////
BOOL CSmartcard::IsCardAbsent(PC readerName)
{
	L					rc;
	SCARD_READERSTATE	state;

	state.szReader = readerName;
	state.dwCurrentState = SCARD_STATE_PRESENT;
	rc = SCardGetStatusChange(m_hContext, 100, &state, 1);
	if (rc == SCARD_S_SUCCESS && (state.dwEventState & SCARD_STATE_EMPTY))
		return TRUE;
	return FALSE;
}

//////////////////////////////////////////////////////////////////
BOOL CSmartcard::SendData(UC *inputData, short inputDataLen, UC *outputData, L *outputDataLen, PC funcname)
{
	Transmit(inputData, inputDataLen, funcname);
	if (m_outputBuffer[m_outputBufferLen-2] == MORE_DATA)
		return TRUE;
	if (m_outputBuffer[m_outputBufferLen-2] == 0x90 && 
		m_outputBuffer[m_outputBufferLen-1] == 0x00)
	{
		if (outputDataLen != NULL)
			*outputDataLen = m_outputBufferLen-2;
		if (m_outputBufferLen > 2 && outputData != NULL && outputDataLen != NULL)
			memcpy(outputData, m_outputBuffer, m_outputBufferLen-2);
		return TRUE;
	}
	return FALSE;
}
