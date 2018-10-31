/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

// Sp_Pipe.h : implementation file
//

/*
This Service Protocol must implement a simple protocol to forward data between the "Client Pipe" and the "Slave Pipe".

First, to connect to a particular MSO, the Client must send a connect with the Mso's Serial Number. 
	Mso's Serial Number can be Zero Length Size, and the server connect to the first MSO it finds.

 * the Connect function:
	sends 
		SP_PIPE_TAG_CONNECT_MSO (1 byte)
		Len (4 bytes)	(can be 0)
		Mso's serial Number (Len bytes) (can not exists if Len is 0)
	receives
		SP_PIPE_TAG_CONNECT_MSO (1 byte)
		Status (4 Bytes)

 * the MsoList function:
	sends 
		SP_PIPE_TAG_MSO_LIST (1 byte)
	receives
		SP_PIPE_TAG_MSO_LIST (1 byte)
		Status (4 Bytes)
		MsoNumber (1 Byte)
		Mso_1_SerialNumberLen (4 Bytes)
		Mso_1_SerialNumber	 (Mso_1_SerialNumberLen Bytes)
		...
  		Mso_N_SerialNumberLen (4 Bytes)
		Mso_N_SerialNumber	 (Mso_N_SerialNumberLen Bytes)


Next, the connection is established and the Client 'converse' directly to the MSO through 2 commands :

 * The ComSend function:
	sends 
		SP_PIPE_TAG_COMSEND (1 byte)
		Len (4 bytes) 
		Datas (Len bytes)
	receives
		SP_PIPE_TAG_COMSEND (1 byte)
		Status (4 Bytes)

 * The ComReceive function:
  	receives
		SP_PIPE_TAG_COMRECV (1 byte)
		Len (4 Bytes)
		Datas (Len bytes)

When connection closed, the MSO is automatically released.
*/

// Client and Server Service Protocol definitions
#define SP_PIPE_TAG_MSO_LIST	'L'
#define SP_PIPE_TAG_CONNECT_MSO	'C'
#define SP_PIPE_TAG_COMSEND		'S'
#define SP_PIPE_TAG_COMRECV		'R'


// Client definitions
F_MORPHO_CALLBACK_COM_OPEN			ClientPipe_CallbackComOpen;
F_MORPHO_CALLBACK_COM_SEND			ClientPipe_CallbackComSend;	
F_MORPHO_CALLBACK_COM_RECEIVE		ClientPipe_CallbackComReceive;
F_MORPHO_CALLBACK_COM_RECEIVE_FREE	ClientPipe_CallbackComReceiveFree;
F_MORPHO_CALLBACK_COM_CLOSE			ClientPipe_CallbackComClose;


typedef struct
{
	TCHAR 	m_auc_MSOSerialNumber[50];
	unsigned long	m_ul_MSOSerialNumberLen;
	unsigned long	m_ul_TimeOut;

} T_CLIENTPIPE_CALLBACKCOMOPEN_PARAMETERS, *PT_CLIENTPIPE_CALLBACKCOMOPEN_PARAMETERS;

