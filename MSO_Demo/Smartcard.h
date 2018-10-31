/************************************************************
  Copyright (c) 2004, Morpho 
************************************************************/

#if !defined(AFX_SMARTCARD_H__C8A0ADE3_6AD2_11D5_ACA5_00E0293DB8F0__INCLUDED_)
#define AFX_SMARTCARD_H__C8A0ADE3_6AD2_11D5_ACA5_00E0293DB8F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MORE_DATA		0x9F

typedef struct
{
	UC	m_uc_cla;
	UC	m_uc_ins;
	UC	m_uc_p1;
	UC	m_uc_p2;
	UC	m_uc_p3;
	UC	m_uc_length;
	UC	m_puc_data[256];

} T__APDU, *PT__APDU;


typedef struct
{
	UC	m_uc_length;
	UC	m_puc_data[256];
	UC	m_uc_sw1;
	UC	m_uc_sw2;

} T__APDU_RESULT, *PT__APDU_RESULT;



class CSmartcard 
{
public:
	CSmartcard	();
	~CSmartcard();

	BOOL	GetReader (PC o_pc_readerName);	

	BOOL	IsCardAbsent(PC readerName);
	BOOL	IsCardPresent(PC readerName);

	BOOL	Connect(PC readerNameToUse = NULL);
	BOOL	Disconnect(UL disposition = SCARD_LEAVE_CARD);
	L		GetATR (PUC o_puc_buffer, PUL io_pul_size);

	L		SendCommand (PT__APDU i_pst_apdu, PT__APDU_RESULT o_pst_result);

	BOOL	GetAndVerifyPINCode(UC typeCHV = 0x01);
	L		Transmit(UC *input, UL inputLen, PC funcName = NULL);
		
	BOOL	m_isConnected;

protected:
	BOOL		GetListReaders(CPtrList *list);
	BOOL		SendData(UC *inputData, short inputDataLen, UC *outputData, L *outputDataLen, PC funcname = NULL);	
	L			UpdateBinary(UC *offset, UC Lg, UC *data);
	L			ReadBinary(UC *offset, UC Lg, UC *bufferOut, L *bufferOutLen);
	L			Select(UC *fid);
	
	UC			m_outputBuffer [1024];
	UC			m_inputBuffer [1024];
	UL			m_outputBufferLen;
	UL			m_inputBufferLen;
	UL			m_responseLen;
	
	UC				m_atr[20];
	UL				m_atrLen;
	UC				m_cla;
	UC				m_lifePhase;	
	UL				m_protocol;
	SCARDHANDLE		m_hCard;
	SCARDCONTEXT	m_hContext;
};

#endif // !defined(AFX_SMARTCARD_H__C8A0ADE3_6AD2_11D5_ACA5_00E0293DB8F0__INCLUDED_)
