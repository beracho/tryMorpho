/************************************************************
  Copyright (c) 2002-2005, Morpho
************************************************************/

// Sp_Pipe_Functions.cpp : implementation file
//

#include "stdafx.h"
#include <stdlib.h>

#include "Winsock2.h"

#include "MORPHO_Types.h"
#include "Sp_ClientPipe.h"
#include "Sp_Pipe_Functions.h"
#include "Pipe_Tcp.h"

/////////////////////////////////////////////////////////////////
void Sp_Pipe_LogText
(
	LPTSTR pszFmt,
	...
)
{
static	TCHAR szMsg[MAX_LOG_LEN];

	va_list arglist;
	va_start(arglist, pszFmt);

	// Use the format string and arguments to format the content text.
#if (_MSC_VER > 1300)
	vsprintf_s(szMsg, pszFmt, arglist);
#else
	vsprintf(szMsg, pszFmt, arglist);
#endif

	LogText ( szMsg );
}

/////////////////////////////////////////////////////////////////
//
// This function Receive a Len (4 bytes) and Datas (Len bytes)
// 
/////////////////////////////////////////////////////////////////
I 
Sp_Pipe_Receive_LenAndDatas
(
	SOCKET	i_x_Sk,
	char	**io_ppc_Data,
	unsigned long	*o_pul_DataSize
)
{
	I		l_i_Ret;
	PC		l_pc_Data;
	char	l_ac_BuffLen[10];
	int		l_i_Recv_BuffLen =0; //BUG CORRIGE PAR C++TEST [BD-PB-NOTINIT-1]
	int		l_i_LenReceived;
	int		l_i_Status;

	// Len Reception (4 bytes)
	l_i_Ret = TCP_Recv ( i_x_Sk, l_ac_BuffLen, sizeof (l_i_Recv_BuffLen), &l_i_LenReceived, MAX_RECV_TIME );
	if ( l_i_Ret != 0 )
	{
		l_i_Status = l_i_Ret;
	}
	else
	{
		memcpy ( (PUC)&l_i_Recv_BuffLen, l_ac_BuffLen, sizeof (l_i_Recv_BuffLen) );
		Sp_Pipe_LogText ( "Receive_LenAndDatas Received len: %d", l_i_Recv_BuffLen );
		l_pc_Data = (char *)malloc ( l_i_Recv_BuffLen );
		l_i_Ret = 0;
	}

	if ( l_i_Recv_BuffLen == 0 )
	{
		Sp_Pipe_LogText ( "Receive_LenAndDatas Received Buffer" );
		*io_ppc_Data = NULL;
		*o_pul_DataSize = 0;
	}
	else
	{
		if ( l_i_Ret == 0 )
		{
			// Buffer Reception (Len bytes)
			l_i_Ret = TCP_Recv ( i_x_Sk, l_pc_Data, l_i_Recv_BuffLen, &l_i_LenReceived, MAX_RECV_TIME );
			if ( l_i_Ret != 0 )
			{
			}
			else
			{
				if ( ( io_ppc_Data != NULL ) && ( o_pul_DataSize != NULL ))
				{
					Sp_Pipe_LogText ( "Receive_LenAndDatas Received Buffer" );
					*io_ppc_Data = l_pc_Data;
					*o_pul_DataSize = l_i_LenReceived;
				}
				else
				{
					Sp_Pipe_LogText ( "Receive_LenAndDatas Received Buffer => trash" );
					free ( l_pc_Data );
				}
			}
		}
	}

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// This function receives a Response tagged as a "ComSend Response" after the reception of the ResponseTag (1 byte) = TAG_COMSEND ('S')
// 
/////////////////////////////////////////////////////////////////
I 
Sp_Pipe_Receive_Status
(
	SOCKET	i_x_Sk
)
{
	int		l_i_Ret;
	char	l_ac_Buff[10];
	int				l_i_LenReceived;

	// Receive the Status (4 bytes) of the distant Write to the Mso
	l_i_Ret = TCP_Recv ( i_x_Sk, l_ac_Buff, sizeof (l_i_Ret), &l_i_LenReceived, MAX_RECV_TIME );
	if ( l_i_Ret != 0 )
	{
		Sp_Pipe_LogText ( "Receive_ComSendResponse TCP_Recv() Err: %d", l_i_Ret );
	}
	else
	{
		memcpy ( (PUC)&l_i_Ret, l_ac_Buff, sizeof (l_i_Ret) );
		Sp_Pipe_LogText ( "Receive_ComSendResponse Received status: %d", l_i_Ret );
	}

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// This function receives all Response, it loops for the desired Tag response
// 
/////////////////////////////////////////////////////////////////
I 
Sp_Pipe_Receive_Responses
(
	SOCKET	i_x_Sk,
	UL		i_ul_Timeout,
	PC*		io_ppc_Data,
	PUL		o_pul_DataSize,
	UC		i_uc_TagResponseDesired
)
{
	int				l_i_Ret;
	char			l_c_Tag;
	int				l_i_LenReceived;

	// We need a loop to discard the not desired response.
	while ( TRUE )
	{
		l_i_Ret = TCP_Recv ( i_x_Sk, &l_c_Tag, sizeof (l_c_Tag), &l_i_LenReceived, i_ul_Timeout );
		if ( l_i_Ret != 0 )
		{
			Sp_Pipe_LogText ( "ClientPipe_CallbackComSend TCP_Recv Err: %d", l_i_Ret );
		}
		else
		{
			Sp_Pipe_LogText ( "ClientPipe_CallbackComSend received type %c", l_c_Tag );
		}

		if ( l_i_Ret != 0 )
		{
			break;
		}

		if ( l_c_Tag == SP_PIPE_TAG_COMRECV )
		{
			Sp_Pipe_Receive_LenAndDatas ( i_x_Sk, io_ppc_Data, o_pul_DataSize );
		}

		if ( l_c_Tag == SP_PIPE_TAG_COMSEND )
		{
			// can discard the ComSend response, this arrives in case of asynchrone ILV reception during waiting a ILV_Cancel response.
			l_i_Ret = Sp_Pipe_Receive_Status ( i_x_Sk );
		}

		if ( l_c_Tag == SP_PIPE_TAG_CONNECT_MSO )
		{
			// can discard the ComSend response, this arrives in case of asynchrone ILV reception during waiting a ILV_Cancel response.
			l_i_Ret = Sp_Pipe_Receive_Status ( i_x_Sk );
		}


		if ( i_uc_TagResponseDesired == l_c_Tag )
			break;
	}

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// This function 
// 
/////////////////////////////////////////////////////////////////
I 
Sp_Pipe_Send_TagAndStatus
(
	SOCKET	i_x_Sk,
	unsigned char	i_uc_Tag,
	int				i_i_Status
)
{
	char	l_ac_Buff[10];
	int				l_i_Ret;

	// Write Commande type 'TAG' transmission 
	l_ac_Buff[0] = i_uc_Tag;

	// Write Status transmission 
	memcpy ( l_ac_Buff+sizeof (i_uc_Tag), (PUC)&i_i_Status, sizeof (i_i_Status) );

	l_i_Ret = TCP_Send ( i_x_Sk, l_ac_Buff, sizeof (i_uc_Tag)+sizeof (i_i_Status) );

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// This function Sends a Len (4 bytes) and Datas (Len bytes)
// 
/////////////////////////////////////////////////////////////////
I 
Sp_Pipe_Send_LenAndDatas
(
	SOCKET	i_x_Sk,
	PC		i_pc_Data,
	UL		i_ul_DataSize
)
{
	I	l_i_Ret;
	char	l_ac_Buff[10];

	l_i_Ret = 0;

	// Len transmission (4 bytes)
	if ( l_i_Ret == 0 )
	{
		memcpy ( l_ac_Buff, (PUC)&i_ul_DataSize, sizeof (i_ul_DataSize) );

		l_i_Ret = TCP_Send ( i_x_Sk, l_ac_Buff, sizeof (i_ul_DataSize) );
		if ( l_i_Ret != 0 )
		{
			Sp_Pipe_LogText ( "Sends_LenAndDatas TCP_Send Err: %d", l_i_Ret );
		}
	}

	if ( l_i_Ret == 0 )
	{
		// Data transmission (Len bytes)
		l_i_Ret = TCP_Send ( i_x_Sk, i_pc_Data, i_ul_DataSize );
		if ( l_i_Ret != 0 )
		{
			Sp_Pipe_LogText ( "Sends_LenAndDatas TCP_Send Err: %d", l_i_Ret );
		}
	}

	return l_i_Ret;

}
