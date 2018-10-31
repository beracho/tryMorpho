/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// MSO_DemoDlg.cpp : implementation file
//
/* 
This file provides function to implement a Remote communication scheme as describe in the "MorphoSmart Programmer's Guide".
This part of the Remote communication scheme is the Client Pipe implement as a Service Protocol (Sp_ClientPipe) based on the TCP/IP 
protocol to access a MSO on a remote PC through the MSO_ServerPipe.exe application.
*/

/* For informations about the protocol between the "Client Pipe" and the "Slave Pipe", see Sp_ClientPipe.h */

#include "stdafx.h"

#include "Winsock2.h"

#include "MORPHO_Types.h"
#include "Sp_ClientPipe.h"
#include "Sp_Pipe_Functions.h"
#include "Pipe_Tcp.h"


typedef struct
{
	SOCKET			m_x_Sk;
	sockaddr_in		m_x_Adress;
	HANDLE			m_h_Mutex;

} T_CLIENT_PIPE_VARS, *PT_CLIENT_PIPE_VARS;


/////////////////////////////////////////////////////////////////
//
// F_MORPHO_CALLBACK_COM_OPEN			ClientPipe_CallbackComOpen;
//
// The function initializes/opens a specific customer communication link.
//
/////////////////////////////////////////////////////////////////
I 
ClientPipe_CallbackComOpen
(
	MORPHO_HANDLE	*io_ph_MsoHandle,
	PC				i_pc_String,
	UL				i_ul_Param,
	PVOID			io_pv_Param	 // Time Out for Pipe connection
)
{
	PT_CLIENT_PIPE_VARS l_px_Vars;
	PT_CLIENTPIPE_CALLBACKCOMOPEN_PARAMETERS l_px_OpenParameters;
	T_CLIENTPIPE_CALLBACKCOMOPEN_PARAMETERS l_x_OpenParameters = {"", 0, 10000 };
	I		l_i_Ret;
	C		l_c_Tag;

	l_i_Ret = 0;

	// Allocates local variables structure attached to this communication link
	l_px_Vars = (PT_CLIENT_PIPE_VARS)malloc ( sizeof(T_CLIENT_PIPE_VARS) );

	if ( io_pv_Param == NULL )
	{
		l_px_OpenParameters	= &l_x_OpenParameters;
	}
	else
	{
		l_px_OpenParameters = (PT_CLIENTPIPE_CALLBACKCOMOPEN_PARAMETERS)io_pv_Param;
	}
	// Correction fait par C++Test [BD-PB-NP-1] 
	if (l_px_Vars != NULL)
	{
		memset ( l_px_Vars, 0, sizeof(T_CLIENT_PIPE_VARS) );
		
		//	Enable TCP functionnalities
		TCP_Start ( TCP_LOG_ON );

		// Establishes a connection to a Server, specified by its Address, and it's Port Number
		l_i_Ret = TCP_Client_Connect ( i_pc_String, i_ul_Param, &l_px_Vars->m_x_Sk, &l_px_Vars->m_x_Adress );

		// Transmit a Connection
		// Correction fait par C++Test [BD-PB-NP-1] 
		
		if ( l_i_Ret == 0 )
		{
			l_c_Tag = SP_PIPE_TAG_CONNECT_MSO;
			l_i_Ret = TCP_Send ( l_px_Vars->m_x_Sk, &l_c_Tag, sizeof (char) );
		}

		if ( l_i_Ret == 0 )
		{
			l_i_Ret = Sp_Pipe_Send_LenAndDatas ( l_px_Vars->m_x_Sk, l_px_OpenParameters->m_auc_MSOSerialNumber, l_px_OpenParameters->m_ul_MSOSerialNumberLen );
		}

		if ( l_i_Ret == 0 )
		{
			l_i_Ret = Sp_Pipe_Receive_Responses ( l_px_Vars->m_x_Sk, l_px_OpenParameters->m_ul_TimeOut, NULL, NULL, SP_PIPE_TAG_CONNECT_MSO );
		}
	
		// Creates a Mutex to protect Emission and Reception cross problems
		l_px_Vars->m_h_Mutex = CreateMutex ( NULL, FALSE, NULL );
	}

	// returns local variables structure as a MORPHO_HANDLE
	*io_ph_MsoHandle = l_px_Vars;

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// F_MORPHO_CALLBACK_COM_CLOSE			ClientPipe_CallbackComClose;
//
// The function closes a specific customer communication link.
//
/////////////////////////////////////////////////////////////////
I ClientPipe_CallbackComClose
( 
	MORPHO_HANDLE	*io_ph_MsoHandle
)
{
	int					l_i_Ret;
	PT_CLIENT_PIPE_VARS l_px_Vars;

	// Points to the local variables structure attached to this communication link
	l_px_Vars = (PT_CLIENT_PIPE_VARS)*io_ph_MsoHandle;

	// Closes the socket
	l_i_Ret = TCP_Client_Close ( &l_px_Vars->m_x_Sk );

	// Close the TCP functionnalities
	TCP_Stop();

	// Close the Mutex
	CloseHandle ( l_px_Vars->m_h_Mutex );
	l_px_Vars->m_h_Mutex = NULL;

	// Free the local variables structure attached to this communication link
		free ( *io_ph_MsoHandle );
		*io_ph_MsoHandle = NULL;

	return l_i_Ret;
}


/////////////////////////////////////////////////////////////////
//
// F_MORPHO_CALLBACK_COM_SEND			ClientPipe_CallbackComSend;
//
// The function sends an ILV frame, using a specific customer communication link.
//
/////////////////////////////////////////////////////////////////
I
ClientPipe_CallbackComSend
( 
	MORPHO_HANDLE	i_h_MsoHandle,
	UL				i_ul_Timeout,
	PUC				i_puc_Data,
	UL				i_ul_DataSize
)
{
	int				l_i_Ret;
	char	l_ac_Buff[10];
	char	l_c_Tag;

	PT_CLIENT_PIPE_VARS l_px_Vars;

	// Points to the local variables structure attached to this communication link
	l_px_Vars = (PT_CLIENT_PIPE_VARS)i_h_MsoHandle;

	l_i_Ret = 0;

	WaitForSingleObject ( l_px_Vars->m_h_Mutex, INFINITE );

	TCP_LogText ( "ClientPipe_CallbackComSend sending: %d to server", i_ul_DataSize );

	// Tag transmission (1 bytes)
	l_c_Tag = SP_PIPE_TAG_COMSEND;

	l_i_Ret = TCP_Send ( l_px_Vars->m_x_Sk, &l_c_Tag, sizeof (l_c_Tag) );
	if ( l_i_Ret != 0 )
	{
		TCP_LogText ( "ClientPipe_CallbackComSend TCP_Send Err: %d", l_i_Ret );
	}

	// Len transmission (4 bytes)
	if ( l_i_Ret == 0 )
	{
		memcpy ( l_ac_Buff, (PUC)&i_ul_DataSize, sizeof (i_ul_DataSize) );

		l_i_Ret = TCP_Send ( l_px_Vars->m_x_Sk, l_ac_Buff, sizeof (i_ul_DataSize) );
		if ( l_i_Ret != 0 )
		{
			TCP_LogText ( "ClientPipe_CallbackComSend TCP_Send Err: %d", l_i_Ret );
		}
	}

	if ( l_i_Ret == 0 )
	{
		// Data transmission (Len bytes)
		l_i_Ret = TCP_Send ( l_px_Vars->m_x_Sk, (char*)i_puc_Data, i_ul_DataSize );
		if ( l_i_Ret != 0 )
		{
			TCP_LogText ( "ClientPipe_CallbackComSend TCP_Send Err: %d", l_i_Ret );
		}
	}

	if ( l_i_Ret == 0 )
	{
		l_i_Ret = Sp_Pipe_Receive_Responses ( l_px_Vars->m_x_Sk, i_ul_Timeout, NULL, NULL, SP_PIPE_TAG_COMSEND );
	}

	ReleaseMutex ( l_px_Vars->m_h_Mutex );

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// F_MORPHO_CALLBACK_COM_RECEIVE			ClientPipe_CallbackComReceive;
//
// The function sends an ILV frame, using a specific customer communication link.
//
/////////////////////////////////////////////////////////////////
I
ClientPipe_CallbackComReceive
( 
	MORPHO_HANDLE	i_h_MsoHandle,
	UL				i_ul_Timeout,
	PUC*			io_ppuc_Data,
	PUL				o_pul_DataSize
)
{
	int				l_i_Ret;

	PT_CLIENT_PIPE_VARS l_px_Vars;

	// Points to the local variables structure attached to this communication link
	l_px_Vars = (PT_CLIENT_PIPE_VARS)i_h_MsoHandle;

	WaitForSingleObject ( l_px_Vars->m_h_Mutex, INFINITE );

	l_i_Ret = Sp_Pipe_Receive_Responses ( l_px_Vars->m_x_Sk, i_ul_Timeout, (char **)io_ppuc_Data, o_pul_DataSize, SP_PIPE_TAG_COMRECV );

	ReleaseMutex ( l_px_Vars->m_h_Mutex );

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// F_MORPHO_CALLBACK_COM_RECEIVE_FREE			ClientPipe_CallbackComReceiveFree;
//
// The function releases memory allocated by T_MORPHO_CALLBACK_COM_RECEIVE.
//
/////////////////////////////////////////////////////////////////
I ClientPipe_CallbackComReceiveFree
( 
	MORPHO_HANDLE	i_h_MsoHandle,
	PUC*			io_ppuc_Data
)
{
	PT_CLIENT_PIPE_VARS l_px_Vars;

	// Points to the local variables structure attached to this communication link
	l_px_Vars = (PT_CLIENT_PIPE_VARS)i_h_MsoHandle;

	// Releases memory allocated 
	if ( *io_ppuc_Data != NULL )
	{
		free ( *io_ppuc_Data );
		*io_ppuc_Data = NULL;
	}

	return 0;
}

