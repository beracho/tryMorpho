// Mso_Demo_ServerPipe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include "Winsock2.h"
#include "MORPHO_Interface.h"
#include "Pipe_Tcp.h"
#include "Sp_ClientPipe.h"	// Service Protocol definitions
#include "Sp_Pipe_Functions.h"	// Service Protocol definitions

/* 
This file provides function to implement a Remote communication scheme as describe in the "MorphoSmart Programmer's Guide".

This part of the Remote communication scheme is the Server Pipe implement as a Service Protocol (Sp_ClientPipe) based on the TCP/IP 
protocol to access the MSO.
*/

/* For informations about the protocol between the "Client Pipe" and the "Slave Pipe", see Sp_ClientPipe.h */



/////////////////////////////////////////////////////////////////
int 
AConnection 
( 
	SOCKET	i_x_Sk 
)
{
	int				l_i_Ret;

	char			l_uc_Buff[10];
	char			l_c_Tag;
	int				l_i_BuffLen;

	char			*l_pc_Recv_Buff;
	unsigned long	l_ul_Recv_BuffLenToReceive;

	char			*l_pc_Send_Buff;
	int				l_i_Send_BuffLenToEmitt;

	C_MORPHO_Device		l_x_MorphoSmart;

	// Reception of the Connection
	while ( TRUE )
	{
		l_i_Ret = TCP_Recv ( i_x_Sk, &l_c_Tag, sizeof (char), &l_i_BuffLen, MAX_RECV_TIME );
		if ( l_i_Ret == 0 && l_i_BuffLen == sizeof (char) )
		{
			if ( l_c_Tag == SP_PIPE_TAG_MSO_LIST )
			{
				unsigned long		l_ul_NbUsbDevice;
				unsigned long		l_ul_Count;
				char 				*l_pc_UsbDeviceName;
				char 				*l_puc_UsbDeviceProperties;

				l_i_Ret = l_x_MorphoSmart.InitUsbDevicesNameEnum(&l_ul_NbUsbDevice);

				// "Write Status" transmission 
				Sp_Pipe_LogText ( "MSO.InitUsbDevicesNameEnum returns status %d, Responding it to client", l_i_Ret );
				l_i_Ret = Sp_Pipe_Send_TagAndStatus ( i_x_Sk, l_c_Tag, l_i_Ret );

				l_uc_Buff[0] = (char)l_ul_NbUsbDevice;
				l_i_Ret = TCP_Send ( i_x_Sk, l_uc_Buff, sizeof (char) );

				for (l_ul_Count=0; l_ul_Count<l_ul_NbUsbDevice; l_ul_Count++)
				{
					l_x_MorphoSmart.GetUsbDevicesNameEnum ( l_ul_Count, l_pc_UsbDeviceName, l_puc_UsbDeviceProperties );
					
					l_i_Ret = Sp_Pipe_Send_LenAndDatas ( i_x_Sk, l_pc_UsbDeviceName, (long)strlen ( l_pc_UsbDeviceName )+1 );
				}
			}
			else if ( l_c_Tag == SP_PIPE_TAG_CONNECT_MSO )
			{
				char	*		l_pc_SerialNumber;
				unsigned long	l_ul_SerialNumberLen;

				Sp_Pipe_LogText ( "Received Connect command" );

				l_i_Ret = Sp_Pipe_Receive_LenAndDatas ( i_x_Sk, &l_pc_SerialNumber, &l_ul_SerialNumberLen );
				if ( l_i_Ret != 0 )
				{
					break;
				}
				// MorphoSmart Connection
				if ( l_i_Ret == 0 )
				{
					if ( l_ul_SerialNumberLen == 0 )
					{
						Sp_Pipe_LogText ( "Connecting to any MorphoSmart" );
						l_i_Ret = l_x_MorphoSmart.OpenDevice ( -1, 0 );
						if ( l_i_Ret != 0 )
						{
							Sp_Pipe_LogText ( "l_x_MorphoSmart.OpenDevice Err: %d", l_i_Ret );
						}
					}
					else
					{
						Sp_Pipe_LogText ( "Connecting to MorphoSmart %s", l_pc_SerialNumber );
						l_i_Ret = l_x_MorphoSmart.OpenUsbDevice ( l_pc_SerialNumber, MAX_RECV_TIME, NULL );
						if ( l_i_Ret != 0 )
						{
							Sp_Pipe_LogText ( "l_x_MorphoSmart.OpenDevice Err: %d", l_i_Ret );
						}
					}

					// "Write Status" transmission 
					Sp_Pipe_LogText ( "MSO.OpenUsbDevice returns status %d, Responding it to client", l_i_Ret );
					l_i_Ret = Sp_Pipe_Send_TagAndStatus ( i_x_Sk, l_c_Tag, l_i_Ret );

					free ( l_pc_SerialNumber );

					// Break the loop to go to ComSend/ComReceive commands
					break;
				}
			}
			else
			{
				l_i_Ret = -1;
				// "Write Status" transmission 
				Sp_Pipe_LogText ( "Unknow Tag, responding %d", l_i_Ret );
				l_i_Ret = Sp_Pipe_Send_TagAndStatus ( i_x_Sk, l_c_Tag, l_i_Ret );
			}
		}

		if ( l_i_Ret != 0 )
		{
			break;
		}
	}


	if ( l_i_Ret == 0 )
	{
		Sp_Pipe_LogText ( "Waiting MSO Commands" );

		while ( TRUE )
		{
			// Recv
			{
				l_i_Ret = TCP_Recv ( i_x_Sk, &l_c_Tag, sizeof (char), &l_i_BuffLen, 0 );
				if ( l_i_Ret == 0 && l_c_Tag == SP_PIPE_TAG_COMSEND )
				{
					l_i_Ret = Sp_Pipe_Receive_LenAndDatas ( i_x_Sk, &l_pc_Recv_Buff, &l_ul_Recv_BuffLenToReceive );
					if ( l_i_Ret == 0 )
					{
						Sp_Pipe_LogText ( "Received Data, sending to MSO with MSO.ComSend" );

						// Write buffer to MSO and "Write Status" transmission
						l_i_Ret = l_x_MorphoSmart.ComSend ( 200, (unsigned char *)l_pc_Recv_Buff, l_ul_Recv_BuffLenToReceive );
						
						free ( l_pc_Recv_Buff );

						// "Write Status" transmission 
						Sp_Pipe_LogText ( "MSO.ComSend returns status %d, Responding it to client", l_i_Ret );
						Sp_Pipe_Send_TagAndStatus ( i_x_Sk, l_c_Tag, l_i_Ret );
					}
				}
				else if ( l_i_Ret == COMERR_TIMEOUT_RECEIVE )
				{
					l_i_Ret = 0;
				}
			}

			if ( l_i_Ret != 0 )
			{
				break;
			}

			l_i_Ret = l_x_MorphoSmart.ComReceive ( 10, (unsigned char **)&l_pc_Send_Buff, (PUL)&l_i_Send_BuffLenToEmitt );
			if ( l_i_Ret == 0 )
			{
				Sp_Pipe_LogText ( "MSO.ComReceive %d char, Responding it to client", l_i_Send_BuffLenToEmitt );
				// "Write Commande type 'Receive' " transmission 
				l_uc_Buff[0] = 'R';
				l_i_Ret = TCP_Send ( i_x_Sk, l_uc_Buff, sizeof (char) );

				l_i_Ret = Sp_Pipe_Send_LenAndDatas ( i_x_Sk, l_pc_Send_Buff, l_i_Send_BuffLenToEmitt );

				if ( l_i_Ret != 0 )
				{
					break;
				}
			}

		}

		Sp_Pipe_LogText ( "End of transmission" );
	}

	Sp_Pipe_LogText ( "Closing Connection to MSO" );

	l_i_Ret = l_x_MorphoSmart.CloseDevice ( );

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
void TaskAConnection( void *pParam )
{
	SOCKET		*l_px_SkAccepted;

	l_px_SkAccepted = (SOCKET *)pParam;

	AConnection ( *l_px_SkAccepted );
	TCP_Server_Close ( l_px_SkAccepted );
}


/////////////////////////////////////////////////////////////////
int ServeurStart ( char *i_pc_AppName, const DWORD i_dw_Port )
{
	SOCKET		l_x_Sk;
	SOCKET		l_x_SkAccepted;
	int			l_i_Ret;

	char		l_ac_HostName[100];
	char		l_ac_Title[100];
	char		l_ac_BaseName[_MAX_FNAME];
	struct hostent*		l_px_HostEnt;
	ULONG*				l_pul_Addr;
	in_addr				l_x_Addr;


	Sp_Pipe_LogText ( "Server starting");

	gethostname ( l_ac_HostName, sizeof ( l_ac_HostName ) );
	l_px_HostEnt = (struct hostent*) gethostbyname ( l_ac_HostName );

	if ( l_px_HostEnt != 0 )
	{
		l_pul_Addr                     = (ULONG*) l_px_HostEnt->h_addr_list[ 0];
		l_x_Addr.s_addr = *l_pul_Addr;
		Sp_Pipe_LogText ( "Host: %s", l_px_HostEnt->h_name );
		Sp_Pipe_LogText ( "Addr: %s", inet_ntoa( l_x_Addr) );
		Sp_Pipe_LogText ( "Port: %d", i_dw_Port );
	}

#if (_MSC_VER > 1300)
	_splitpath_s( i_pc_AppName, NULL,0,  NULL,0, l_ac_BaseName, _MAX_FNAME, NULL,0 );
	_snprintf_s ( l_ac_Title, sizeof ( l_ac_Title ), "%s on: %s (%s) port: %d", l_ac_BaseName, l_ac_HostName, inet_ntoa( l_x_Addr), i_dw_Port );
#else
	_splitpath( i_pc_AppName, NULL, NULL, l_ac_BaseName, NULL );
	_snprintf ( l_ac_Title, sizeof ( l_ac_Title ), "%s on: %s (%s) port: %d", l_ac_BaseName, l_ac_HostName, inet_ntoa( l_x_Addr), i_dw_Port );
#endif

	SetConsoleTitle ( l_ac_Title);

	l_i_Ret = TCP_Server_Create ( i_dw_Port, &l_x_Sk );
	if ( l_i_Ret != 0 )
		return l_i_Ret;

	if ( l_i_Ret == 0 )
	{
		while ( TRUE )
		{
			Sp_Pipe_LogText ( "Waiting Connection");
			l_i_Ret = TCP_Server_Accept ( l_x_Sk, &l_x_SkAccepted );
			if ( l_i_Ret == 0 )
			{ 
				_beginthread ( &TaskAConnection, 0, &l_x_SkAccepted );
			}
		}
	}

	TCP_Server_Close ( &l_x_Sk );

	return 0;
}

/////////////////////////////////////////////////////////////////
bool Usage (int argc, char* argv[])
{
	char l_ac_BaseName[_MAX_FNAME];

#if (_MSC_VER > 1300)
	_splitpath_s( argv[0], NULL,0, NULL,0, l_ac_BaseName,_MAX_FNAME, NULL,0 );
#else
	_splitpath( argv[0], NULL, NULL, l_ac_BaseName, NULL );
#endif

	fprintf (stderr, "%s\n", l_ac_BaseName );
	fprintf (stderr, "Copyright (c) 2007, Morpho\n");
	fprintf (stderr, "\n" );
	fprintf (stderr, "usage :\n" );
	fprintf (stderr, "%s (use Tcp Port 11011 by default)\n", l_ac_BaseName );
	fprintf (stderr, "%s -h\n", l_ac_BaseName );
	fprintf (stderr, "%s -p \"TcpPortNumber\"\n", l_ac_BaseName );
	fprintf (stderr, "\n" );
	fprintf (stderr, "\t-h :  This Help \n");
	fprintf (stderr, "\t-p TcpPortNumber: Tcp Port Number\n");
	fprintf (stderr, "\n" );

	return false;
}

/////////////////////////////////////////////////////////////////
bool ScanArgs (int argc, char* argv[], DWORD * o_pdw_Port )
{
	int nThisArg;

	for ( nThisArg = 1; nThisArg < argc; nThisArg++ )
	{
		if ( (strlen ( argv[nThisArg] )) != 2 )
				return Usage ( argc, argv );

		if ( argv[nThisArg][0] == '-' )
		{
			switch ( argv[nThisArg][1] )
			{
				case 'H':
				case 'h':
					return Usage ( argc, argv );
					break;
				case 'p':
				case 'P':
#if (_MSC_VER > 1300)
					sscanf_s ( argv[nThisArg+1], "%d", o_pdw_Port );
#else
					sscanf ( argv[nThisArg+1], "%d", o_pdw_Port );
#endif
					nThisArg++;
					break;
				default:
					return Usage ( argc, argv );
			}
		}
	}

	return true;
}


int main(int argc, char* argv[])
{
	DWORD l_dw_Port;

	l_dw_Port = 11011;

	if ( !ScanArgs ( argc, argv, &l_dw_Port ) )
		return 1;

	TCP_Start ( TCP_LOG_OFF );

	ServeurStart ( argv[0], l_dw_Port );

	TCP_Stop();

	return 0;
}

