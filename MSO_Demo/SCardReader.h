/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

// SCardReader.h: interface for the CSCardReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCARDREADER_H__FC840AE1_4754_4ECB_98E3_BDF7DBB483F5__INCLUDED_)
#define AFX_SCARDREADER_H__FC840AE1_4754_4ECB_98E3_BDF7DBB483F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winscard.h>	//Win32 Scard header file
#define ATR_MAX_SIZE	40

/* 7816-3 definitions */
#define APDU_OFFSET_CLA			0
#define APDU_OFFSET_INS			1
#define APDU_OFFSET_P1			2
#define APDU_OFFSET_P2			3
#define APDU_OFFSET_P3			4
#define APDU_SIZEOF_APDU_HEADER	(APDU_OFFSET_P3+1)
#define APDU_HEADER_LEN	5

#define IOCTL_CCID_ESCAPE SCARD_CTL_CODE(3500)

#ifndef PVOID

#define B			BOOLEAN
#define UC			unsigned char
#define C			char
#define US			unsigned short
#define S			short
#define UL			unsigned long
#define L			long
#define I			int
#define UI			unsigned int
#define PUI			unsigned int*
#define PUC			unsigned char*
#define PC			char*
#define PUS			unsigned short*
#define PS			short*
#define PUL			unsigned long*
#define PL			long*
#define PI			int*

#endif


//the card does not manage extended APDU length => L are encoded on one byte
//and buffer are limited to 256 bytes
typedef struct
{
	US	m_us_Le;
	UC	m_uc_Cla;
	UC	m_uc_Ins;
	UC	m_uc_P1;
	UC	m_uc_P2;
	UC	m_uc_Lc;
	UC	m_auc_Pad[1];
} T__APDU_IN, *PT__APDU_IN;


typedef struct
{
	UL			m_ul_Size;
	PUC			m_puc_Buf;
}T__BUF, *PT__BUF;

typedef struct
{
	UC	m_puc_data[8];	
} T__RANDOM, *PT__RANDOM;


class CSCardReader  
{
public:
	DWORD	m_dw_ActiveProtocol;

	CSCardReader
		( 
			IN DWORD i_dw_PreferredProtocols
		);

	virtual ~CSCardReader
		(
			void
		);

	I	PowerOn
		(
			CString i_str_ReaderName
		);
	I	PowerOff
		(
			void
		);

	I	EnumReader
		(
			I*	o_pi_NbConnectedSCReader
		);

	CString	GetReaderName
		(
			I i_i_ReaderNumber
		);

	void	TraceSCardError
		(
			CString i_str_FuncName,
			I i_i_RetValue
		);

	void ConvertSCardError(I i_i_Error, CString i_str_Commentaire, CString &o_cs_Message);

	I	GetLastStatusBytesError
		(
			PUC o_puc_SW1, 
			PUC o_puc_SW2
		);

	// Select by file Identifier
	I	Select
		(
			US	i_us_FileId, 
			PT__BUF io_px_Buff	// Can be null if no response data is required
		);

	// Selcet by DF Name
	I	Select
	(
		PUC			i_puc_data, 
		UL			i_ul_data_size,
		PT__BUF		io_px_Buff	// Can be null if no response data is required
	);

	I	GetData 
		( 
			US i_us_P1P2, 
			PT__BUF io_px_Buff
		);

	I	ReadBinary
		(
			UL	i_ul_offset, 
			PUC o_puc_data, 
			UL i_ul_data_size
		);

	I	ReadBinary256
		(
			US					i_us_offset , 
			PUC					o_puc_data, 
			PUL					io_pul_data_size
		);

	I	UpdateBinary
		(
			UL	i_ul_offset, 
			PUC o_puc_data, 
			UL i_ul_data_size
		);

	I	UpdateBinary255
		(
			US	i_us_offset , 
			PUC i_puc_data, 
			UL i_ul_data_size
		);

	I	GetChallenge
		(
			PT__RANDOM			o_px_rand
		);

	I	ExternalAuthenticate
		(
			PT__RANDOM			i_px_rand, 
			UC					i_uc_KeyNum
		);

	I	CSCardReader::Verify
		(
			PUC i_puc_data, 
			UL i_ul_data_size,
			UC	i_uc_Role,
			UC					i_uc_KeyNum
		);

	I	CSCardReader::Verify
		(
			PUC i_puc_data, 
			UL i_ul_data_size,
			UC					i_uc_KeyNum
		);

	I	SendAPDU
		(
			PT__APDU_IN		i_px_APDU, 
			PUC				i_puc_data,
			UL				i_ul_data_size,
			PUC				o_puc_data, 
			PUL				io_pul_data_size
		);

	I	SendEscapeCmd
		(
		PUC				i_puc_data,
		UL				i_ul_data_size,
		PUC				o_puc_data,
		PUL				o_pul_data_size
		);

private:

	I	Transmit
		(
			PUC i_puc_Input, 
			UL i_ul_InputLen,
			PUC o_puc_Output, 
			PUL io_pul_OutputLen
		);

	I	Initialize
		(
			IN DWORD i_dw_PreferredProtocols
		);

	I	Terminate
		(
			void
		);

	I	Connect
		(
			CString i_str_ReaderName
		);

	I	Disconnect
		(
			void
		);

	I	BuildCmd
		(
			PT__APDU_IN i_px_ApduIn, 
			PUC			i_puc_ApduInData,
			PUC			* o_ppuc_buffer_in, 
			PUL			o_pul_sizeof_buffer_in
		);

	I	FreeCmd
		(
			PUC * o_ppuc_buffer_in
		);

	I SetSCardError
		(
			CString i_str_FuncName,
			I i_i_RetValue
		);

	SCARDCONTEXT			m_h_SCardContext;
	SCARDHANDLE				m_h_SCardHandle;
	PC						m_pc_BufferStringReaderList;
	DWORD					m_dw_SizeOfBufferStringReaderList;
	UC						m_puc_ATR[ATR_MAX_SIZE];
	UL						m_ul_ATR_Size;

	I						m_i_SCardError;
	UC						m_uc_SW1;
	UC						m_uc_SW2;

	DWORD					m_dw_PreferredProtocols;
	LPCSCARD_IO_REQUEST		m_pio_SendPci;

};

typedef I	(*T_SMART_CARD_IHM_CALLBACK)( 
					UC							i_uc_ID_Func,
					US							i_us_value,
					PVOID						i_pv_context);

#define MAX_PK_SIZE			1024
#define MAX_IDENTIFIER_SIZE	20
#define MAX_SURNAME_SIZE	30
#define MAX_NAME_SIZE		60

typedef struct
{
	UC	m_puc_PK1[MAX_PK_SIZE];
	US	m_us_PK1_Size;
	UC	m_puc_PK2[MAX_PK_SIZE];
	US	m_us_PK2_Size;
	UC	m_puc_Identifier[MAX_IDENTIFIER_SIZE];
	UC	m_puc_Surname[MAX_SURNAME_SIZE];
	UC	m_puc_Name[MAX_NAME_SIZE];
} T_CBioDemoCard_USER_DATA, *PT_CBioDemoCard_USER_DATA;

class CBioDemoCard : public CSCardReader
{
public:

	CBioDemoCard( IN DWORD i_dw_PreferredProtocols ) :CSCardReader ( i_dw_PreferredProtocols ){};

	virtual ~CBioDemoCard(){};

	virtual I	Read(PT_CBioDemoCard_USER_DATA o_px_user_data,T_SMART_CARD_IHM_CALLBACK i_x_IHM_Callback,PVOID i_x_Context,UC i_uc_TemplateNumber) = 0;
	virtual I	Update(PT_CBioDemoCard_USER_DATA i_px_user_data,T_SMART_CARD_IHM_CALLBACK i_x_IHM_Callback,PVOID i_x_Context) = 0;
};

#endif // !defined(AFX_SCARDREADER_H__FC840AE1_4754_4ECB_98E3_BDF7DBB483F5__INCLUDED_)

