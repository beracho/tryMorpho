/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

// Pipe_Tcp.cpp : implementation file
//
// This file provides function to implement a simple Server and a simple Client based on the TCP/IP protocol.

#include "stdafx.h"

#include "Winsock2.h"

#include "MORPHO_Types.h"
#include "Pipe_Tcp.h"

int g_i_Log;

/////////////////////////////////////////////////////////////////
void LogText
(
	LPTSTR pszFmt
)
{
static	TCHAR l_ac_Str[MAX_LOG_LEN];

#if (_MSC_VER > 1300)
	_snprintf_s ( l_ac_Str, MAX_LOG_LEN, TEXT("%09ld %08X %s\n"), GetTickCount(), GetCurrentThreadId(), pszFmt );
#else
	_snprintf ( l_ac_Str, MAX_LOG_LEN, TEXT("%09ld %08X %s\n"), GetTickCount(), GetCurrentThreadId(), pszFmt );
#endif

	// Output the newly formated message string to the debugger display.
	OutputDebugString(l_ac_Str);
	printf("%s", l_ac_Str);

	return;
}

/////////////////////////////////////////////////////////////////
void TCP_LogText
(
	LPTSTR pszFmt,
	...
)
{
static	TCHAR szMsg[MAX_LOG_LEN];
	if ( g_i_Log != TCP_LOG_ON )
		return;

	va_list arglist;
	va_start(arglist, pszFmt);

	// Use the format string and arguments to format the content text.
#if (_MSC_VER > 1300)
	vsprintf_s(szMsg, pszFmt, arglist);
#else
	vsprintf(szMsg, pszFmt, arglist);
#endif

	LogText ( szMsg );

	return;
}

/////////////////////////////////////////////////////////////////
//
// This function is used to initiates the use of the functionality of this file
//
// used by :
//		Server, Client
//
/////////////////////////////////////////////////////////////////
BOOL	
TCP_Start
( 
	int i_i_Log /* = TCP_LOG_OFF */ // In : Use to activate Log if TCP_LOG_ON
)
{
	WSADATA				m_WSDllInfo;		// Socket Dll Info	
	long				l_WSAResult;

#ifdef _WIN32

	g_i_Log = i_i_Log;

	TCP_LogText ( "TCP_Start()" );

	// The WSAStartup function initiates use of Ws2_32.dll by a process.
	l_WSAResult = WSAStartup(0x0202, &m_WSDllInfo); 
	if(	l_WSAResult != 0) //success
	{
		TCP_LogText ( "TCP_Start WSAStartup() Err: %d", WSAGetLastError() );
		// The WSACleanup function terminates use of the Ws2_32.dll.
		WSACleanup();
		return FALSE;
	}	
	else
#endif // _WIN32
	return TRUE;
}

/////////////////////////////////////////////////////////////////
//
// This function is used to initiates the use of the functionality of this file
//
// used by :
//		Server, Client
//
/////////////////////////////////////////////////////////////////
BOOL
TCP_Stop
(
	void
)
{
	TCP_LogText ( "TCP_Stop()" );

	// The WSACleanup function terminates use of the Ws2_32.dll.
	//----------------------------
	if (WSACleanup() == SOCKET_ERROR) 
	{
		TCP_LogText ( "TCP_Stop WSACleanup() Err: %d", WSAGetLastError() );
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////
//
// This function is used to initiates a Server socket, attached to a specified Port Number, ready to accept a Client connection
//
// used by :
//		Server
//
/////////////////////////////////////////////////////////////////
int 
TCP_Server_Create
(
	DWORD		i_dw_Port,	// In : Port Number that identifies the application
	SOCKET		*o_px_Sk	// Out: Socket initiated and returned
)
{
	sockaddr_in		l_x_Adress;
	int				l_i_Ret;
	BOOL			l_bTcpNoDelay;

	l_bTcpNoDelay    = TRUE;

	l_i_Ret = 0;

	// Create connexion socket
	//------------------------
	// The socket function creates a socket that is bound to a specific service provider.
	*o_px_Sk = socket(AF_INET, SOCK_STREAM, 0);

	if ( *o_px_Sk == INVALID_SOCKET )
	{
		TCP_LogText ( "ERROR: socket() Err: %d", WSAGetLastError() );
		l_i_Ret = SOCKET_ERROR;
	}
	else
	{
		TCP_LogText("socket successfull");
	}

	if ( l_i_Ret == 0 )
	{
		l_x_Adress.sin_family      = AF_INET; 
		l_x_Adress.sin_port        = htons( (unsigned short) i_dw_Port); 
		l_x_Adress.sin_addr.s_addr = htonl( INADDR_ANY); 

		setsockopt( *o_px_Sk, IPPROTO_IP, TCP_NODELAY, (char*)&l_bTcpNoDelay, sizeof(BOOL));

		// The bind function associates a local address with a socket.
		l_i_Ret = bind ( *o_px_Sk, (sockaddr*) &l_x_Adress, sizeof(l_x_Adress));
		if ( l_i_Ret != 0 )
		{
			TCP_LogText ( "ERROR: bind() Err: %d", WSAGetLastError() );
		}
		else
		{
			TCP_LogText("bind successfull");
		}
	}


	if ( l_i_Ret == 0 )
	{
		// The listen function places a socket a state where it is listening for an incoming connection.
		l_i_Ret = listen ( *o_px_Sk, 1 );
		if ( l_i_Ret == SOCKET_ERROR )
		{
			TCP_LogText ( "ERROR: listen() Err: %d", WSAGetLastError() );
		}
		else
		{
			TCP_LogText("listen successfull");
		}
	}

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// This function is used to wait a Client connection and returns a new connection for this Client
//
// used by :
//		Server
//
/////////////////////////////////////////////////////////////////
int 
TCP_Server_Accept
(
	SOCKET			i_x_Sk,				// In : Socket previously initiated for the Server 
	SOCKET			*o_px_SkAccepted	// Out: New Client connection
)
{
	int		l_i_Ret;
	ULONG	l_lCmd;

	l_i_Ret = 0;

	TCP_LogText("Waiting for a Client connection");

	// The accept function permits an incoming connection attempt on a socket.
	*o_px_SkAccepted = accept( i_x_Sk, NULL, 0);
	if ( *o_px_SkAccepted == INVALID_SOCKET )
	{
		l_i_Ret = INVALID_SOCKET;
		TCP_LogText ( "TCP_Server_Accept accept() Err: %d", WSAGetLastError() );
	}
	else
	{
		TCP_LogText("accept(ed) successfull");
	}

	// To enable the nonblocking mode of the socket
	l_lCmd = 1;			// != 0.
	l_i_Ret = ioctlsocket ( *o_px_SkAccepted, FIONBIO, &l_lCmd );
	if ( l_i_Ret == SOCKET_ERROR )
	{
		l_i_Ret = SOCKET_ERROR;
		TCP_LogText ( "TCP_Server_Accept ioctlsocket(FIONBIO) Err: %d", WSAGetLastError() );
	}

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// This function is used to close server socket
//
// used by :
//		Server
//
/////////////////////////////////////////////////////////////////
int 
TCP_Server_Close
(
	SOCKET			*o_px_Sk	// Socket to close
)
{
	int		l_i_Ret;

	// The shutdown function disables sends or receives on a socket.
	l_i_Ret = shutdown( *o_px_Sk, SD_BOTH);
	if ( l_i_Ret == SOCKET_ERROR )
	{
		l_i_Ret = SOCKET_ERROR;
		TCP_LogText ( "TCP_Server_Close shutdown() Err: %d", WSAGetLastError() );
	}

	// The closesocket function closes an existing socket.
	l_i_Ret = closesocket ( *o_px_Sk );
	if ( l_i_Ret == SOCKET_ERROR )
	{
		l_i_Ret = SOCKET_ERROR;
		TCP_LogText ( "TCP_Server_Close closesocket() Err: %d", WSAGetLastError() );
	}

	*o_px_Sk = INVALID_SOCKET;

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// This function is used to establishes a connection to a Server, specified by its Address, and it's Port Number
//
// used by :
//		Client
//
/////////////////////////////////////////////////////////////////
int 
TCP_Client_Connect
(
	char *			i_pc_Adress,	// In : Server Address IP Format ("127.0.0.1") or URL Format
	unsigned long	i_ul_Port,		// In : Server Port Number
	SOCKET			*o_px_Sk,		// Out: Socket created to the Server
	sockaddr_in		*o_px_Adress	// Out: Saved address 
)
{
	int		l_i_Ret;
	ULONG	l_lCmd;
	struct hostent*		l_px_HostEnt;
	ULONG*				l_pul_Addr;

	l_i_Ret = 0;
	l_px_HostEnt   = NULL;
	l_pul_Addr     = NULL;

	o_px_Adress->sin_family = AF_INET;

	// The inet_addr function converts a string containing an (Ipv4) Internet Protocol dotted address into a proper address for the IN_ADDR structure.
	o_px_Adress->sin_addr.s_addr = inet_addr ( i_pc_Adress );
	if ( o_px_Adress->sin_addr.s_addr == INADDR_NONE )
	{
		// The gethostbyname function retrieves host information corresponding to a host name from a host database.
		l_px_HostEnt = (struct hostent*) gethostbyname ( i_pc_Adress );
		if ( l_px_HostEnt != NULL )
		{
			l_i_Ret = 0;
		}

		if ( l_i_Ret == 0 )
		{
			l_pul_Addr                     = (ULONG*) l_px_HostEnt->h_addr_list[ 0];
			o_px_Adress->sin_addr.s_addr = *l_pul_Addr; // address is already in network byte order
		}
	}

	// Port MUST be in Network Byte Order
	//-----------------------------------
	// The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
	o_px_Adress->sin_port = htons ( (unsigned short)i_ul_Port );

	// Create connexion socket
	//------------------------
	// The socket function creates a socket that is bound to a specific service provider.
	*o_px_Sk = socket(AF_INET, SOCK_STREAM, 0);

	if ( *o_px_Sk == INVALID_SOCKET )
	{
		TCP_LogText ( "TCP_Client_Connect socket() Err: %d", WSAGetLastError() );
		l_i_Ret = INVALID_SOCKET;
	}
	else
	{
		l_i_Ret = 0;
	}

	if ( l_i_Ret == 0 )
	{
		// The connect function establishes a connection to a specified socket.
		l_i_Ret = connect ( *o_px_Sk, (struct sockaddr *)o_px_Adress, sizeof(sockaddr_in) );

		if(	l_i_Ret == SOCKET_ERROR )
		{
			TCP_LogText ( "TCP_Client_Connect connect() Err: %d", WSAGetLastError() );
		}			
	}

	if ( l_i_Ret == 0 )
	{
	// To enable the nonblocking mode of the socket
		l_lCmd = 1;			// != 0.
		l_i_Ret = ioctlsocket ( *o_px_Sk, FIONBIO, &l_lCmd );
		if ( l_i_Ret == SOCKET_ERROR )
		{
			l_i_Ret = SOCKET_ERROR;
			TCP_LogText ( "TCP_Client_Connect ioctlsocket(FIONBIO) Err: %d", WSAGetLastError() );
		}
	}

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// This function is used to close Client socket
//
// used by :
//		Client
//
/////////////////////////////////////////////////////////////////
int 
TCP_Client_Close
(
	SOCKET			*o_px_Sk	// Socket to close
)
{
	int		l_i_Ret;

	// The shutdown function disables sends or receives on a socket.
	l_i_Ret = shutdown( *o_px_Sk, SD_BOTH);
	if ( l_i_Ret == SOCKET_ERROR )
	{
		l_i_Ret = SOCKET_ERROR;
		TCP_LogText ( "TCP_Client_Close shutdown() Err: %d", WSAGetLastError() );
	}

	// The closesocket function closes an existing socket.
	l_i_Ret = closesocket ( *o_px_Sk );
	if ( l_i_Ret == SOCKET_ERROR )
	{
		l_i_Ret = SOCKET_ERROR;
		TCP_LogText ( "TCP_Client_Close closesocket() Err: %d", WSAGetLastError() );
	}

	*o_px_Sk = INVALID_SOCKET;

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// This function is used to receive data in a certain time, from the remote connection, can be Server (if caller is Client) or Client (if caller is Server).
//
// used by :
//		Server, Client
//
/////////////////////////////////////////////////////////////////
int TCP_Recv
(
	SOCKET			i_x_Sk,				// In : Socket to remote connection
	char			*o_pc_Buffer,		// Out: Buffer for the incoming data 
	int				i_i_LenToRecive,	// In : Len of data to be receive
	int				*o_pi_LenReceived,	// Out: Number of bytes received
	unsigned long	i_ul_TimeOut		// In : Maximum time for this reception
)
{
	int		l_i_Ret;
	int		l_i_ReceivedSize;
	int		l_i_YetReceivedSize;
	int		l_i_LeftToReceive;
	int		l_i_LastErr;
	int		l_i_BlockSize;

	
	DWORD	l_dw_Now;
	DWORD	l_dw_MaxWait;

	*o_pi_LenReceived = 0;

//	TCP_LogText ( "TCP_Recv waiting %d cars in %d ms", i_i_LenToRecive, i_ul_TimeOut );

	l_i_Ret = 0;

// calculates the term of the time
	if ( i_ul_TimeOut == INFINITE )
	{
		l_dw_Now = 0;
		l_dw_MaxWait = INFINITE;
	}
	else
	{
		l_dw_Now = GetTickCount();
		l_dw_MaxWait = l_dw_Now + i_ul_TimeOut;
	}

	l_i_YetReceivedSize = 0;
	l_i_LeftToReceive = i_i_LenToRecive;

	l_i_BlockSize	=	MAX_TRANSMIT;
		// A loop is need, all data can be received in several times
		do
		{
			if ( l_i_BlockSize > l_i_LeftToReceive ) 
			{
				l_i_BlockSize = l_i_LeftToReceive;
			}

			// The recv function receives data from a connected or bound socket
			l_i_ReceivedSize = recv ( i_x_Sk, o_pc_Buffer + l_i_YetReceivedSize, l_i_BlockSize, 0);

			l_i_LastErr = WSAGetLastError();

			// If the return size is SOCKET_ERROR, let's know the error
			if ( l_i_ReceivedSize == SOCKET_ERROR ) 
			{
				if ( l_i_LastErr != WSAEWOULDBLOCK )
				{
					// Error
					TCP_LogText ( "TCP_Recv Sk %d recv Err: %d", i_x_Sk, l_i_LastErr );
					l_i_Ret = SOCKET_ERROR;
					break;
				}
				else
				{
					// The error is WSAEWOULDBLOCK
					// This error is returned from operations on nonblocking sockets that cannot be completed immediately, for example recv when no data is queued to be read from the socket. It is a nonfatal error, and the operation should be retried later.
				}
			}
			// If the return size is 0, the connection has been gracefully closed by the remote
			else if ( l_i_ReceivedSize == 0 ) 
			{
				TCP_LogText ( "TCP_Recv Sk %d gracefully closed", i_x_Sk );
				l_i_Ret = SOCKET_ERROR;
				break;
			}
			// If the return size is > 0, data has been received
			else if ( l_i_ReceivedSize > 0 ) 
			{
//				TCP_LogText ( "TCP_Recv Sk %d recv: %d", i_x_Sk, l_i_ReceivedSize );

				l_i_YetReceivedSize += l_i_ReceivedSize;	
				l_i_LeftToReceive -= l_i_ReceivedSize;

				if ( l_i_YetReceivedSize >= i_i_LenToRecive )
				{
					l_i_Ret = 0;
					break;
				}
			}
			// Else it's an error
			else
			{
				TCP_LogText ( "TCP_Recv Sk %d recv Err: %d %d", i_x_Sk, l_i_ReceivedSize, l_i_LastErr );
				l_i_Ret = SOCKET_ERROR;
				break;
			}

			// Check if the term time occured
			l_dw_Now = GetTickCount();
			if ( l_dw_Now >= l_dw_MaxWait )
			{
				l_i_Ret = COMERR_TIMEOUT_RECEIVE;
				if ( i_ul_TimeOut != 0 )
				{
					TCP_LogText ( "TCP_Recv Sk %d recv TimeOut", i_x_Sk );
				}
				// To give system hand
				Sleep ( 1 );
				break;
			}
		}
		while ( TRUE );

	if ( l_i_Ret == 0 )
	{
		*o_pi_LenReceived = l_i_YetReceivedSize;

		TCP_LogText ( "TCP_Recv received %d cars ", l_i_YetReceivedSize );
	}

	return l_i_Ret;
}

/////////////////////////////////////////////////////////////////
//
// This function is used to send data, to the remote connection, can be Server (if caller is Client) or Client (if caller is Server).
//
// used by :
//		Server, Client
//
/////////////////////////////////////////////////////////////////
int TCP_Send
(
	SOCKET			i_x_Sk,			// In : Socket to remote connection
	char			*i_pc_Buffer,	// In : Buffer containing the data to be transmitted
	int				i_i_BufferSize	// In : Length of the data to be transmitted
)
{
	int		l_i_Ret;
	int		l_i_SentSize;
	int		l_i_LeftToSend;
	int		l_i_YetSentSize;
	int		l_i_BlockSize;
	int		l_i_LastErr;

	l_i_LeftToSend	= i_i_BufferSize;
	l_i_YetSentSize	= 0;
	l_i_BlockSize	=	MAX_TRANSMIT;

	l_i_Ret = 0;

	TCP_LogText ( "TCP_Send Sk %d send: len %d", i_x_Sk, i_i_BufferSize );

	// A loop is need, data should be send in several times (MAX_TRANSMIT = 8192)
	while ( l_i_YetSentSize < i_i_BufferSize )
	{
		if ( l_i_BlockSize > l_i_LeftToSend ) 
		{
			l_i_BlockSize = l_i_LeftToSend;
		}
 
		// The send function sends data on a connected socket.
		l_i_SentSize = send ( i_x_Sk, i_pc_Buffer + l_i_YetSentSize, l_i_BlockSize, 0);

		// If the return size is > 0, data has been send
		if ( l_i_SentSize > 0 ) 
		{
			// Carreful, for nonblocking sockets:
			// If no error occurs, send returns the total number of bytes sent, which can be less than the number indicated by len for nonblocking sockets. 

//			TCP_LogText ( "TCP_Send Sk %d sended: len %d", i_x_Sk, l_i_SentSize );
			l_i_LeftToSend	-=	l_i_SentSize;
			l_i_YetSentSize	+=	l_i_SentSize;
		}
		else
		{
			// If the return size is SOCKET_ERROR, let's know the error
			l_i_LastErr = WSAGetLastError();
			if ( l_i_LastErr != WSAEWOULDBLOCK )
			{
				// Error
				TCP_LogText ( "TCP_Send Sk %d send Err: %d", i_x_Sk, l_i_LastErr );
				l_i_Ret = SOCKET_ERROR;
				break;
			}
			else
			{
				// The error is WSAEWOULDBLOCK
				// This error is returned from operations on nonblocking sockets that cannot be completed immediately, for example recv when no data is queued to be read from the socket. It is a nonfatal error, and the operation should be retried later.
			}
		}
	}

	return l_i_Ret;
}