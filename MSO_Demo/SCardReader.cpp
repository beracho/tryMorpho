/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// SCardReader.cpp: implementation of the CSCardReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <scarderr.h>
#include <winerror.h>
#include <winscard.h>	//Win32 Scard header file
#include "SCardReader.h"
#include "SmartCard_Error.h"

#define OLD_OPUCE_CARD	1
#define	NEW_OPUCE_CARD	2

int		g_i_card_type;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSCardReader::CSCardReader( IN DWORD i_dw_PreferredProtocols )
{
	//Initialize the context
	Initialize(i_dw_PreferredProtocols);
}

CSCardReader::~CSCardReader()
{
	//Free context resource
	Terminate();
}

//////////////////////////////////////////////////////////////////////
// PowerON
//////////////////////////////////////////////////////////////////////
I	CSCardReader::PowerOn(CString i_str_ReaderName)
{
	I	l_i_Ret;
	
	//power the card on
	l_i_Ret = Connect(i_str_ReaderName);

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////////
// PowerOFF
//////////////////////////////////////////////////////////////////////
I	CSCardReader::PowerOff()
{
	I	l_i_Ret;

	//power the card off
	l_i_Ret = Disconnect();

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////////
void CSCardReader::ConvertSCardError(I i_i_Error, CString i_str_Commentaire, CString &o_cs_Message)
{
	LPVOID lpMsgBuf;

	switch(i_i_Error)
	{
		case SMARTCARD_BUSY	:
			o_cs_Message.Format("CSCardReader SMARTCARD_BUSY");
			break;

		case SMARTCARD_INITIALIZE_ERR	:
			o_cs_Message.Format("CSCardReader SMARTCARD_INITIALIZE_ERR");
			break;

		case SMARTCARD_NOT_FOUND	:
			o_cs_Message.Format("CSCardReader SMARTCARD_NOT_FOUND");
			break;

		case SMARTCARD_SCARD_ERROR	:
			{
				FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					m_i_SCardError,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
					);
				// Process any inserts in lpMsgBuf.
				// ...
				// Display the string.
				o_cs_Message.Format ( "CSCardReader Err %s Err=0x%08X : %s", i_str_Commentaire, m_i_SCardError, (LPCTSTR)lpMsgBuf );

				// Free the buffer.
				LocalFree( lpMsgBuf );
			}
			break;

		case SMARTCARD_NO_ATR	:
			o_cs_Message.Format("CSCardReader SMARTCARD_NO_ATR");
			break;

		case SMARTCARD_BADPARAMETER	:
			o_cs_Message.Format("CSCardReader SMARTCARD_BADPARAMETER");
			break;

		case SMARTCARD_ALLOC_FAILED	:
			o_cs_Message.Format("CSCardReader SMARTCARD_ALLOC_FAILED");
			break;

		case SMARTCARD_GET_RESPONSE_DATA_STILL_AVAILABLE	:
			o_cs_Message.Format("CSCardReader SMARTCARD_GET_RESPONSE_DATA_STILL_AVAILABLE");
			break;

		case SMARTCARD_STATUS_BYTES_ERROR	:
			o_cs_Message.Format("CSCardReader SW1=0x%02X SW2=0x%02X", m_uc_SW1, m_uc_SW2 );
			break;

		case SMARTCARD_READER_ERROR	:
			o_cs_Message.Format("CSCardReader SW1=0x%02X SW2=0x%02X", m_uc_SW1, m_uc_SW2);
			break;

		case SMARTCARD_TAG_COMPACT_FORMAT_NOT_FOUND	:
			o_cs_Message.Format("CSCardReader SMARTCARD_TAG_COMPACT_FORMAT_NOT_FOUND");
			break;

		case SMARTCARD_BAD_PK_BUFFER_SIZE	:
			o_cs_Message.Format("CSCardReader SMARTCARD_BAD_PK_BUFFER_SIZE");
			break;

		case SMARTCARD_INVALID_OFFSET	:
			o_cs_Message.Format("CSCardReader SMARTCARD_INVALID_OFFSET");
			break;

		case SMARTCARD_GET_RESPONSE_NOT_EXPECTED	:
			o_cs_Message.Format("CSCardReader SMARTCARD_GET_RESPONSE_NOT_EXPECTED");
			break;

		case SMARTCARD_BAD_IDENTIFIER_BUFFER_SIZE	:
			o_cs_Message.Format("CSCardReader SMARTCARD_BAD_IDENTIFIER_BUFFER_SIZE");
			break;
		
		default:
			o_cs_Message.Format("CSCardReader Error");
			break;
	}
}

//////////////////////////////////////////////////////////////////////
I CSCardReader::SetSCardError(CString i_str_FuncName,I i_i_RetValue)
{
	m_i_SCardError = i_i_RetValue;
	
	TraceSCardError ( i_str_FuncName, m_i_SCardError );

	return SMARTCARD_SCARD_ERROR;
}

//////////////////////////////////////////////////////////////////////
void CSCardReader::TraceSCardError(CString i_str_FuncName,I i_i_RetValue)
{
	CString	l_str_Message;

	ConvertSCardError ( i_i_RetValue, i_str_FuncName, l_str_Message );

	//AfxMessageBox((LPCTSTR)l_str_message,MB_ICONSTOP);
	TRACE0(l_str_Message);
	
}

//////////////////////////////////////////////////////////////////////
I	CSCardReader::Initialize(IN DWORD i_dw_PreferredProtocols)
{
	I	l_i_Ret = SCARD_S_SUCCESS;

	m_dw_PreferredProtocols = i_dw_PreferredProtocols;


	//The SCardEstablishContext function establishes the resource manager context 
	//(the scope) within which database operations are performed.
	//The resource manager context is the context used by the resource manager 
	//when accessing the smart card database. 
	//The resource manager context is primarily used by the query and management 
	//functions when accessing the database. The scope of the resource manager 
	//context can be the current user or the system. 
	l_i_Ret = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &m_h_SCardContext);
	if ( l_i_Ret != SCARD_S_SUCCESS)
	{
		l_i_Ret = SetSCardError ( "SCardEstablishContext", l_i_Ret );
		m_h_SCardContext = NULL;
	}

	//initialize
	m_h_SCardHandle=NULL;
	m_pc_BufferStringReaderList=NULL;
	m_dw_SizeOfBufferStringReaderList=0;
	m_ul_ATR_Size=ATR_MAX_SIZE;
	memset ( m_puc_ATR, 0, m_ul_ATR_Size );

	return l_i_Ret;
}


//////////////////////////////////////////////////////////////////////
I	CSCardReader::Terminate()
{
	I	l_i_Ret = SCARD_S_SUCCESS;
	
	if (m_pc_BufferStringReaderList!=NULL)
	{
		l_i_Ret = SCardFreeMemory(m_h_SCardContext,m_pc_BufferStringReaderList);
		if (l_i_Ret != SCARD_S_SUCCESS)
		{
			l_i_Ret = SetSCardError ( "SCardFreeMemory",l_i_Ret);
		}
		else
		{
			m_pc_BufferStringReaderList=NULL;
			m_dw_SizeOfBufferStringReaderList=0;
		}
	}

	
	//The SCardReleaseContext function closes an established resource manager context,
	//freeing any resources allocated under that context, including SCARDHANDLE objects
	//and memory allocated using the SCARD_AUTOALLOCATE length designator.
	if (m_h_SCardContext!=NULL)
	{
		l_i_Ret = SCardReleaseContext(m_h_SCardContext);
		if (l_i_Ret != SCARD_S_SUCCESS)
		{
			l_i_Ret = SetSCardError ( "SCardReleaseContext", l_i_Ret );
		}
		else
		{
			m_h_SCardContext = NULL;
		}
	}

	m_ul_ATR_Size=ATR_MAX_SIZE;
	memset(m_puc_ATR,0,m_ul_ATR_Size);

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////////
I	CSCardReader::Connect(CString i_str_ReaderName)
{
	I	l_i_Ret = SCARD_S_SUCCESS;
	DWORD	l_dw_State;
	
	//power the card on
	l_i_Ret = SCardConnect(m_h_SCardContext, i_str_ReaderName, SCARD_SHARE_EXCLUSIVE, 
		m_dw_PreferredProtocols, &m_h_SCardHandle, &m_dw_ActiveProtocol);
	if (l_i_Ret != SCARD_S_SUCCESS)
	{
		l_i_Ret = SetSCardError ( "SCardConnect", l_i_Ret );
		m_h_SCardHandle = NULL;
	}

	if (m_dw_ActiveProtocol == SCARD_PROTOCOL_T1)
	{
		m_pio_SendPci = SCARD_PCI_T1;
	}
	else
	{
		m_pio_SendPci = SCARD_PCI_T0;
	}

	//Get ATR
	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = SCardStatus(m_h_SCardHandle, NULL, 0, 
			&l_dw_State, &m_dw_ActiveProtocol, (PUC)&m_puc_ATR, &m_ul_ATR_Size);
		if (l_i_Ret != SCARD_S_SUCCESS)
		{
			l_i_Ret = SetSCardError ( "SCardStatus", l_i_Ret );
		}

		if(m_ul_ATR_Size>5 )
			g_i_card_type = OLD_OPUCE_CARD;
		else
			g_i_card_type = NEW_OPUCE_CARD;
	}
	
	return l_i_Ret;
}
 
//////////////////////////////////////////////////////////////////////
I	CSCardReader::Disconnect()
{
	I	l_i_Ret = SCARD_S_SUCCESS;
	
	//power the card off
	l_i_Ret = SCardDisconnect(m_h_SCardHandle, SCARD_UNPOWER_CARD);
	if (l_i_Ret != SCARD_S_SUCCESS)
	{
		l_i_Ret = SetSCardError ( "SCardDisconnect", l_i_Ret );
	}
	else
	{
		m_h_SCardHandle = NULL;
		m_ul_ATR_Size=ATR_MAX_SIZE;
		memset(m_puc_ATR,0,m_ul_ATR_Size);
	}

	return l_i_Ret;
}
  
//////////////////////////////////////////////////////////////////////
I	CSCardReader::EnumReader(I*	o_pi_NbConnectedSCReader)
{
	I	l_i_Ret = SCARD_S_SUCCESS;
	DWORD l_dw_count;
	
	*o_pi_NbConnectedSCReader = 0;
	
	if (m_pc_BufferStringReaderList!=NULL)
	{
		l_i_Ret = SCardFreeMemory(m_h_SCardContext,m_pc_BufferStringReaderList);
		if (l_i_Ret != SCARD_S_SUCCESS)
		{
			l_i_Ret = SetSCardError ( "SCardFreeMemory", l_i_Ret );
		}
	}
	
	if (l_i_Ret == SMARTCARD_OK)
	{
		m_pc_BufferStringReaderList = NULL;
		m_dw_SizeOfBufferStringReaderList = SCARD_AUTOALLOCATE;
		
		//enumerate list of reader connected
		l_i_Ret = SCardListReaders(m_h_SCardContext, NULL, 
			(PC)&m_pc_BufferStringReaderList, &m_dw_SizeOfBufferStringReaderList);
		if (l_i_Ret != SCARD_S_SUCCESS)
		{
			l_i_Ret = SetSCardError ( "SCardListReaders", l_i_Ret );
		}
		else
		{
			l_dw_count = 0;
			while (l_dw_count<m_dw_SizeOfBufferStringReaderList-1 /*exlude null terminator of string list*/)
			{
				*o_pi_NbConnectedSCReader += 1;
				l_dw_count+=(DWORD)strlen(m_pc_BufferStringReaderList+l_dw_count) + 1;
			}
		}
	}

	return l_i_Ret;
} 
 
//////////////////////////////////////////////////////////////////////
CString	CSCardReader::GetReaderName(I i_i_ReaderNumber)
{
	CString	l_str_ReaderName;
	PC	l_pc_ptrBufferStringReaderList;
	PC	l_pc_ptrEndBufferStringReaderList;
	I	l_i_ReaderNumber;

	l_str_ReaderName="";
	l_i_ReaderNumber=0;

	l_pc_ptrBufferStringReaderList = m_pc_BufferStringReaderList;
	l_pc_ptrEndBufferStringReaderList = m_pc_BufferStringReaderList + m_dw_SizeOfBufferStringReaderList - 1 /*exclude null terminatro of list string*/;

	while (l_pc_ptrBufferStringReaderList<l_pc_ptrEndBufferStringReaderList)
	{
		if (l_i_ReaderNumber==i_i_ReaderNumber)
		{
			l_str_ReaderName = l_pc_ptrBufferStringReaderList;
			break;
		}
		l_pc_ptrBufferStringReaderList+=strlen(l_pc_ptrBufferStringReaderList) + 1 /*null terminator*/; 
		l_i_ReaderNumber++;
	}

	return l_str_ReaderName;
}
 

//////////////////////////////////////////////////////////////////
I	CSCardReader::Transmit
(
	PUC i_puc_Input, 
	UL i_ul_InputLen,
	PUC o_puc_Output, 
	PUL io_pul_OutputLen
)
{
	I	l_i_Ret;
	UC	l_auc_BufferOut [256+2]; // + 2 pour SW1 SW2
	UC	l_auc_BufferGetResponse [5];
	PUC	l_puc_BufferIn;
	UL	l_ul_LenBuffer;
	UL	l_ul_Offset;

	l_puc_BufferIn = i_puc_Input;
	l_ul_Offset = 0;
	
	l_i_Ret = SCARD_S_SUCCESS;
	
	l_i_Ret = SCardBeginTransaction(m_h_SCardHandle);
	if (l_i_Ret != SCARD_S_SUCCESS)
	{
			l_i_Ret = SetSCardError ( "SCardBeginTransaction", l_i_Ret );
	}

	do
	{
		l_ul_LenBuffer = sizeof(l_auc_BufferOut);

		if (l_i_Ret == SMARTCARD_OK)
		{
			l_i_Ret = SCardTransmit(m_h_SCardHandle, m_pio_SendPci, l_puc_BufferIn, i_ul_InputLen, NULL, l_auc_BufferOut, &l_ul_LenBuffer );
			if (l_i_Ret != SCARD_S_SUCCESS)
			{
				l_i_Ret = SetSCardError ( "SCardTransmit", l_i_Ret );
			}	
		}

		if ( o_puc_Output != NULL )
		{
			memcpy ( o_puc_Output + l_ul_Offset, l_auc_BufferOut, (I)(l_ul_LenBuffer - 2) );
		}

		l_ul_Offset = l_ul_Offset + l_ul_LenBuffer - 2;

		m_uc_SW1 = l_auc_BufferOut[l_ul_LenBuffer - 2];
		m_uc_SW2 = l_auc_BufferOut[l_ul_LenBuffer - 1];

		if ( m_uc_SW1 == 0x61 )
		{
			l_puc_BufferIn = l_auc_BufferGetResponse;
			i_ul_InputLen = 5;

			l_auc_BufferGetResponse[0] = 0x00;	
			l_auc_BufferGetResponse[1] = 0xC0;	
			l_auc_BufferGetResponse[2] = 0x00;
			l_auc_BufferGetResponse[3] = 0x00;
			l_auc_BufferGetResponse[4] = m_uc_SW2;
		}
	} while ( m_uc_SW1 == 0x61 );

	if ( l_i_Ret == SCARD_S_SUCCESS )
	{
		if ( m_uc_SW1 != 0x90 )
		{
			l_i_Ret = SMARTCARD_STATUS_BYTES_ERROR;;
			if ( io_pul_OutputLen != NULL )
			{
				*io_pul_OutputLen = 0;
			}
		}
		else
		{
			if ( io_pul_OutputLen != NULL )
			{
				*io_pul_OutputLen = l_ul_Offset;
			}
		}
	}

	SCardEndTransaction(m_h_SCardHandle, SCARD_LEAVE_CARD);

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::GetLastStatusBytesError(PUC o_puc_SW1, PUC o_puc_SW2)
{
	*o_puc_SW1 = m_uc_SW1;
	*o_puc_SW2 = m_uc_SW2;

	return SMARTCARD_OK;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::SendAPDU
(
	PT__APDU_IN		i_px_APDU, 
	PUC				i_puc_data,
	UL				i_ul_data_size,
	PUC				o_puc_data, 
	PUL				io_pul_data_size
)
{
	I	l_i_Ret;
	PUC	l_puc_buffer_in;
	UL	l_ul_sizeof_buffer_in;

//init out
	if ( o_puc_data != NULL )
	{
		memset ( o_puc_data, 0, (I)(*io_pul_data_size) );
	}

//build transmit buffers (in and out)
	l_i_Ret = BuildCmd ( i_px_APDU, i_puc_data, &l_puc_buffer_in, &l_ul_sizeof_buffer_in );

//transmit
	if (l_i_Ret == SMARTCARD_OK)
	{
		l_i_Ret = Transmit ( l_puc_buffer_in, l_ul_sizeof_buffer_in, o_puc_data, io_pul_data_size );
	}

	FreeCmd ( &l_puc_buffer_in );

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::SendEscapeCmd
(
PUC				i_puc_data,
UL				i_ul_data_size,
PUC				o_puc_data,
PUL				o_pul_data_size
)
{
	I	l_i_Ret = SCARD_S_SUCCESS;
	UL  l_ul_length;
	UL	code = IOCTL_CCID_ESCAPE;

	if (o_puc_data != NULL)
	{
		memset(o_puc_data, 0, (I)(*o_pul_data_size));
	}

	l_i_Ret = SCardBeginTransaction(m_h_SCardHandle);
	if (l_i_Ret != SCARD_S_SUCCESS)
	{
		l_i_Ret = SetSCardError("SCardBeginTransaction", l_i_Ret);
	}

	do
	{
		if (l_i_Ret == SMARTCARD_OK)
		{
			l_i_Ret = SCardControl(m_h_SCardHandle, code, i_puc_data, i_ul_data_size, o_puc_data, *o_pul_data_size, &l_ul_length);
			if (l_i_Ret != SCARD_S_SUCCESS)
			{
				l_i_Ret = SetSCardError("SCardcontrol", l_i_Ret);
			}
			m_uc_SW1 = o_puc_data[l_ul_length - 2];
			m_uc_SW2 = o_puc_data[l_ul_length - 1];
			*o_pul_data_size = l_ul_length-2;
		}

	} while (m_uc_SW1 == 0x61);

	if (l_i_Ret == SCARD_S_SUCCESS)
	{
		if (m_uc_SW1 != 0x90)
		{
			l_i_Ret = SMARTCARD_READER_ERROR;
		}
	}

	SCardEndTransaction(m_h_SCardHandle, SCARD_LEAVE_CARD);
	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::Select
(
	US					i_us_ID, 
	PT__BUF				io_px_Buff
)
{
	I	l_i_Ret;
	
	T__APDU_IN	l_x_apdu_in;

	UC			l_auc_ApduInData[2];

	PUC	l_puc_buffer_in;
	UL	l_ul_sizeof_buffer_in;

	l_i_Ret = SMARTCARD_OK;

//init out
	if(io_px_Buff != NULL)
	{
		memset ( io_px_Buff->m_puc_Buf, 0, (I)io_px_Buff->m_ul_Size );
	}

//build apdu to transmit
	l_x_apdu_in.m_uc_Cla = 0x00;	
	l_x_apdu_in.m_uc_Ins = 0xA4;	

	
	if(g_i_card_type == NEW_OPUCE_CARD)
	{
		// New type of OPUCE Morpho CARD

		if(i_us_ID == 0x3F00)	// MF
			l_x_apdu_in.m_uc_P1 = 0x00;
		else
			l_x_apdu_in.m_uc_P1 = 0x02;
	}
	else
	{
		// Old type of OPUCE Morpho CARD

		l_x_apdu_in.m_uc_P1 = 0x00;
	}

	l_x_apdu_in.m_uc_P2 = 0x00;
	if ( io_px_Buff == NULL )
	{
		l_x_apdu_in.m_uc_P2 |= 0x0C;
	}

	l_x_apdu_in.m_uc_Lc = 0x02; 
	l_auc_ApduInData[0]	= (UC)(i_us_ID>>8); 
	l_auc_ApduInData[1]	= (UC)i_us_ID; 
	//
	l_x_apdu_in.m_us_Le = 0;

//build transmit buffers (in and out)
	l_i_Ret = BuildCmd ( &l_x_apdu_in, l_auc_ApduInData, &l_puc_buffer_in, &l_ul_sizeof_buffer_in );
//transmit
	if ( l_i_Ret == SMARTCARD_OK )
	{
		if ( io_px_Buff == NULL )
		{
			l_i_Ret = Transmit ( l_puc_buffer_in, l_ul_sizeof_buffer_in, NULL, 0 );
		}
		else
		{
			l_i_Ret = Transmit ( l_puc_buffer_in, l_ul_sizeof_buffer_in, io_px_Buff->m_puc_Buf, &io_px_Buff->m_ul_Size );
		}
	}

	FreeCmd ( &l_puc_buffer_in );

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::Select
(
	PUC			i_puc_data, 
	UL			i_ul_data_size,
	PT__BUF		io_px_Buff
)
{
	I	l_i_Ret;
	
	T__APDU_IN	l_x_apdu_in;

	PUC	l_puc_buffer_in;
	UL	l_ul_sizeof_buffer_in;

	l_i_Ret = SMARTCARD_OK;

//init out
	if ( io_px_Buff != NULL )
	{
		memset ( io_px_Buff->m_puc_Buf, 0, (I)io_px_Buff->m_ul_Size );
	}

//build apdu to transmit
	l_x_apdu_in.m_uc_Cla = 0x00;	
	l_x_apdu_in.m_uc_Ins = 0xA4;	
	l_x_apdu_in.m_uc_P1 = 0x04;
	l_x_apdu_in.m_uc_P2 = 0x00;

	if ( io_px_Buff == NULL )
	{
		l_x_apdu_in.m_uc_P2 |= 0x0C;
	}

	l_x_apdu_in.m_uc_Lc = (UC)i_ul_data_size; 
	//
	l_x_apdu_in.m_us_Le = 0;

//build transmit buffers (in and out)
	l_i_Ret = BuildCmd ( &l_x_apdu_in, i_puc_data, &l_puc_buffer_in, &l_ul_sizeof_buffer_in );
//transmit
	if ( l_i_Ret == SMARTCARD_OK )
	{
		if ( io_px_Buff == NULL )
		{
			l_i_Ret = Transmit ( l_puc_buffer_in, l_ul_sizeof_buffer_in, NULL, 0 );
		}
		else
		{
			l_i_Ret = Transmit ( l_puc_buffer_in, l_ul_sizeof_buffer_in, io_px_Buff->m_puc_Buf, &io_px_Buff->m_ul_Size );
		}
	}

	FreeCmd ( &l_puc_buffer_in );

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::GetData 
( 
	US i_us_P1P2, 
	PT__BUF io_px_Buff
)
{
	I	l_i_Ret = SMARTCARD_OK;
	
	T__APDU_IN	l_x_apdu_in;
	PUC			l_puc_buffer_in;
	UL			l_ul_sizeof_buffer_in;


//init out
	memset ( io_px_Buff->m_puc_Buf, 0, (I)(io_px_Buff->m_ul_Size) );

	if ( io_px_Buff->m_ul_Size > 256 )
		return SMARTCARD_BADPARAMETER;

//build apdu to transmit
	l_x_apdu_in.m_uc_Cla = 0x00;	
	l_x_apdu_in.m_uc_Ins = 0xCA;
	l_x_apdu_in.m_uc_P1 = (UC)(i_us_P1P2>>8);
	l_x_apdu_in.m_uc_P2 = (UC)i_us_P1P2;
	l_x_apdu_in.m_uc_Lc = 0; 
//
	l_x_apdu_in.m_us_Le = (US)io_px_Buff->m_ul_Size;

//build transmit buffers (in and out)
	l_i_Ret = BuildCmd ( &l_x_apdu_in, NULL, &l_puc_buffer_in, &l_ul_sizeof_buffer_in );

	//transmit
	if ( l_i_Ret == SMARTCARD_OK )
	{
		l_i_Ret = Transmit ( l_puc_buffer_in, l_ul_sizeof_buffer_in, io_px_Buff->m_puc_Buf, &io_px_Buff->m_ul_Size );
	}
	
	FreeCmd ( &l_puc_buffer_in );

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::ReadBinary(UL	i_ul_offset, PUC o_puc_data, UL i_ul_data_size)
{
	// Correction fait par C++Test [BD-PB-NOTINIT-1]
	I	l_i_Ret = 0;
	UL	l_ul_Offset;
	UL	l_ul_SizeToRead;

	l_ul_Offset = 0;
	while (l_ul_Offset<i_ul_data_size)
	{
		l_ul_SizeToRead = (US)__min( (i_ul_data_size - l_ul_Offset),256);
		l_i_Ret = ReadBinary256((US)(i_ul_offset+l_ul_Offset),o_puc_data+l_ul_Offset,&l_ul_SizeToRead);
		if (l_i_Ret)
			break;
		l_ul_Offset+=l_ul_SizeToRead;
	};

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::ReadBinary256
(
	US					i_us_offset , 
	PUC					o_puc_data, 
	PUL					io_pul_data_size
)
{
	I	l_i_Ret = SMARTCARD_OK;
	
	T__APDU_IN	l_x_apdu_in;

	PUC	l_puc_buffer_in;
	UL	l_ul_sizeof_buffer_in;

//init out
	memset ( o_puc_data, 0, (I)(*io_pul_data_size) );

	if ( i_us_offset>0xEFFF )
		return SMARTCARD_INVALID_OFFSET;

	if ( *io_pul_data_size > 256)
		return SMARTCARD_BADPARAMETER;

//build apdu to transmit
	l_x_apdu_in.m_uc_Cla = 0x00;	
	l_x_apdu_in.m_uc_Ins = 0xB0;
	l_x_apdu_in.m_uc_P1 = (UC) ( (i_us_offset>>8)&0xEF );
	l_x_apdu_in.m_uc_P2 = (UC)i_us_offset;
	l_x_apdu_in.m_uc_Lc = 0; 
//
	l_x_apdu_in.m_us_Le = (US)(*io_pul_data_size);

//build transmit buffers (in and out)
	l_i_Ret = BuildCmd ( &l_x_apdu_in, NULL, &l_puc_buffer_in, &l_ul_sizeof_buffer_in );

	//transmit
	if ( l_i_Ret == SMARTCARD_OK )
	{
		l_i_Ret = Transmit ( l_puc_buffer_in, l_ul_sizeof_buffer_in, o_puc_data, io_pul_data_size );
	}
	
	FreeCmd ( &l_puc_buffer_in );

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::ExternalAuthenticate
(
	PT__RANDOM			i_px_rand, 
	UC					i_uc_KeyNum
)
{
	I	l_i_Ret;
	
	T__APDU_IN	l_x_apdu_in;
	PUC			l_puc_buffer_in;
	UL			l_ul_sizeof_buffer_in;

	UL			l_ul_data_size;

	l_i_Ret = SMARTCARD_OK;

//build apdu to transmit
	l_x_apdu_in.m_uc_Cla = 0x00;	
	l_x_apdu_in.m_uc_Ins = 0x82;
	l_x_apdu_in.m_uc_P1 = 0x00;
	l_x_apdu_in.m_uc_P2 = (UC)(0x80|i_uc_KeyNum);
	l_x_apdu_in.m_uc_Lc = sizeof(i_px_rand->m_puc_data); 

	l_x_apdu_in.m_us_Le = 0; 

//build transmit buffers (in and out)
	l_i_Ret = BuildCmd ( &l_x_apdu_in, i_px_rand->m_puc_data, &l_puc_buffer_in, &l_ul_sizeof_buffer_in );

	//transmit
	if ( l_i_Ret == SMARTCARD_OK )
	{
		l_ul_data_size = 0;

		l_i_Ret = Transmit ( l_puc_buffer_in, l_ul_sizeof_buffer_in, NULL, &l_ul_data_size );
	}
	
	FreeCmd ( &l_puc_buffer_in );

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::GetChallenge
(
	PT__RANDOM			o_px_rand
)
{
	I	l_i_Ret = SMARTCARD_OK;
	T__APDU_IN	l_x_apdu_in;
	PUC			l_puc_buffer_in;
	UL			l_ul_sizeof_buffer_in;
	UL			l_ul_data_size;

//build apdu to transmit
	l_x_apdu_in.m_uc_Cla = 0x00;	
	l_x_apdu_in.m_uc_Ins = 0x84;
	l_x_apdu_in.m_uc_P1 = 0x00;
	l_x_apdu_in.m_uc_P2 = 0x00;
	l_x_apdu_in.m_uc_Lc = 0; 
	l_x_apdu_in.m_us_Le = sizeof(o_px_rand->m_puc_data); 

//build transmit buffers (in and out)
	l_i_Ret = BuildCmd ( &l_x_apdu_in, NULL, &l_puc_buffer_in, &l_ul_sizeof_buffer_in );

	//transmit
	if ( l_i_Ret == SMARTCARD_OK )
	{
		l_ul_data_size = sizeof(o_px_rand->m_puc_data);
		l_i_Ret = Transmit ( l_puc_buffer_in, l_ul_sizeof_buffer_in, o_px_rand->m_puc_data, &l_ul_data_size );
	}
	
	FreeCmd ( &l_puc_buffer_in );

	return l_i_Ret;
	
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::UpdateBinary(UL	i_ul_offset, PUC i_puc_data, UL i_ul_data_size)
{
	I	l_i_Ret;
	UL	l_ul_Offset;
	UL	l_ul_SizeToUpdate;

	if (i_ul_data_size==0)
		return SMARTCARD_OK;
	
	l_ul_Offset = 0;
	while (l_ul_Offset<i_ul_data_size)
	{
		l_ul_SizeToUpdate = (US)__min(i_ul_data_size-l_ul_Offset,255);
		l_i_Ret = UpdateBinary255((US)(i_ul_offset+l_ul_Offset),i_puc_data+l_ul_Offset,l_ul_SizeToUpdate);
		if (l_i_Ret)
			break;
		l_ul_Offset+=l_ul_SizeToUpdate;
	};

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::UpdateBinary255
(
	US		i_us_offset, 
	PUC		i_puc_data, 
	UL		i_ul_data_size
)
{
	I	l_i_Ret = SMARTCARD_OK;

	T__APDU_IN	l_x_apdu_in;
	PUC	l_puc_buffer_in;
	UL	l_ul_sizeof_buffer_in;
	UL			l_ul_data_size;

	if (i_us_offset>0xEFFF)
		return SMARTCARD_INVALID_OFFSET;

	if (i_ul_data_size > 255)
		return SMARTCARD_BADPARAMETER;

//build apdu to transmit
	l_x_apdu_in.m_uc_Cla = 0x00;	
	l_x_apdu_in.m_uc_Ins = 0xD6;
	l_x_apdu_in.m_uc_P1 = (UC)(i_us_offset>>8)&0xEF;
	l_x_apdu_in.m_uc_P2 = (UC)i_us_offset;
	l_x_apdu_in.m_us_Le = 0; 
//
	l_x_apdu_in.m_uc_Lc = (UC)i_ul_data_size;

//build transmit buffers (in and out)
	l_i_Ret = BuildCmd ( &l_x_apdu_in, i_puc_data, &l_puc_buffer_in, &l_ul_sizeof_buffer_in );

//transmit
	if (l_i_Ret == SMARTCARD_OK)
	{
		l_ul_data_size = i_ul_data_size;
		l_i_Ret = Transmit ( l_puc_buffer_in, l_ul_sizeof_buffer_in, NULL, &l_ul_data_size );
	}

	FreeCmd ( &l_puc_buffer_in );

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::Verify
(
	PUC i_puc_data, 
	UL	i_ul_data_size,
	UC	i_uc_KeyNum
)
{
	I	l_i_Ret;
	
	T__APDU_IN	l_x_apdu_in;
	PUC			l_puc_buffer_in;
	UL			l_ul_sizeof_buffer_in;

	UL			l_ul_data_size;

	l_i_Ret = SMARTCARD_OK;

//build apdu to transmit
	l_x_apdu_in.m_uc_Cla = 0x00;	
	l_x_apdu_in.m_uc_Ins = 0x20;
	l_x_apdu_in.m_uc_P1 = 0x00;
	l_x_apdu_in.m_uc_P2 = (UC)(0x80|i_uc_KeyNum);
	l_x_apdu_in.m_uc_Lc = (UC)i_ul_data_size; 

	l_x_apdu_in.m_us_Le = 0; 

//build transmit buffers (in and out)
	l_i_Ret = BuildCmd ( &l_x_apdu_in, i_puc_data, &l_puc_buffer_in, &l_ul_sizeof_buffer_in );

	//transmit
	if ( l_i_Ret == SMARTCARD_OK )
	{
		l_ul_data_size = 0;

		l_i_Ret = Transmit ( l_puc_buffer_in, l_ul_sizeof_buffer_in, NULL, &l_ul_data_size );
	}
	
	FreeCmd ( &l_puc_buffer_in );

	return l_i_Ret;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::Verify
(
	PUC i_puc_data, 
	UL	i_ul_data_size,
	UC	i_uc_Role,
	UC	i_uc_KeyNum
)
{
	I	l_i_Ret;
	
	T__APDU_IN	l_x_apdu_in;
	PUC			l_puc_buffer_in;
	UL			l_ul_sizeof_buffer_in;

	UL			l_ul_data_size;

	l_i_Ret = SMARTCARD_OK;

//build apdu to transmit
	l_x_apdu_in.m_uc_Cla = 0x00;	
	l_x_apdu_in.m_uc_Ins = 0x20;
	l_x_apdu_in.m_uc_P1 = i_uc_Role;
	l_x_apdu_in.m_uc_P2 = i_uc_KeyNum;
	l_x_apdu_in.m_uc_Lc = (UC)i_ul_data_size; 

	l_x_apdu_in.m_us_Le = 0; 

//build transmit buffers (in and out)
	l_i_Ret = BuildCmd ( &l_x_apdu_in, i_puc_data, &l_puc_buffer_in, &l_ul_sizeof_buffer_in );

	//transmit
	if ( l_i_Ret == SMARTCARD_OK )
	{
		l_ul_data_size = 0;

		l_i_Ret = Transmit ( l_puc_buffer_in, l_ul_sizeof_buffer_in, NULL, &l_ul_data_size );
	}
	
	FreeCmd ( &l_puc_buffer_in );

	return l_i_Ret;
}



//////////////////////////////////////////////////////////////////
I	CSCardReader::BuildCmd
(
	PT__APDU_IN i_px_ApduIn, 
	PUC			i_puc_ApduInData,
	PUC			* o_ppuc_buffer_in, 
	PUL			o_pul_sizeof_buffer_in
)
{
	UL	l_ul_i;
	
	//buffer buffer in
	*o_pul_sizeof_buffer_in = 4;	//CLA+INS+P1+P2
	if	(i_px_ApduIn->m_uc_Lc != 0)
	{
		*o_pul_sizeof_buffer_in = *o_pul_sizeof_buffer_in  + 1 + i_px_ApduIn->m_uc_Lc;
	}
	if	(i_px_ApduIn->m_us_Le != 0)
	{
		*o_pul_sizeof_buffer_in = *o_pul_sizeof_buffer_in + 1;
	}

	*o_ppuc_buffer_in = (PUC)malloc ( (I)(*o_pul_sizeof_buffer_in) );
	if (*o_ppuc_buffer_in == NULL)
	{
		return SMARTCARD_ALLOC_FAILED;
	}

	//buffer buffer out
	l_ul_i=0;
	(*o_ppuc_buffer_in)[l_ul_i++]=i_px_ApduIn->m_uc_Cla;
	(*o_ppuc_buffer_in)[l_ul_i++]=i_px_ApduIn->m_uc_Ins;
	(*o_ppuc_buffer_in)[l_ul_i++]=i_px_ApduIn->m_uc_P1;
	(*o_ppuc_buffer_in)[l_ul_i++]=i_px_ApduIn->m_uc_P2;
	if	(i_px_ApduIn->m_uc_Lc != 0)
	{
		(*o_ppuc_buffer_in)[l_ul_i++]=i_px_ApduIn->m_uc_Lc;
		memcpy((*o_ppuc_buffer_in) + l_ul_i, i_puc_ApduInData, i_px_ApduIn->m_uc_Lc);
		l_ul_i+=i_px_ApduIn->m_uc_Lc;
	}
	if	(i_px_ApduIn->m_us_Le != 0)
	{
		(*o_ppuc_buffer_in)[l_ul_i++]=(UC)i_px_ApduIn->m_us_Le;
	}

	return SMARTCARD_OK;
}

//////////////////////////////////////////////////////////////////
I	CSCardReader::FreeCmd 
(
	PUC * o_ppuc_buffer_in
)
{
	if (*o_ppuc_buffer_in!=NULL)
	{
		free ( *o_ppuc_buffer_in );
		*o_ppuc_buffer_in = NULL;
	}
	
	return SMARTCARD_OK;
}

