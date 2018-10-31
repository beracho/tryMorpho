/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

// Pipe_Tcp.cpp : implementation file
//

#define MAX_RECV_TIME		10000

#define MAX_LOG_LEN		2048

#define MAX_TRANSMIT	8192

#define TCP_LOG_OFF	0
#define TCP_LOG_ON	1

/////////////////////////////////////////////////////////////////
void LogText
(
	LPTSTR pszFmt
);

/////////////////////////////////////////////////////////////////
void TCP_LogText
(
	LPTSTR pszFmt,
    ...
);

/////////////////////////////////////////////////////////////////
//
// This function is used to initiates the use of the functionality of this file
//
// used by :
//		Server, Client
//
/////////////////////////////////////////////////////////////////
BOOL TCP_Start
(
	int i_i_Log = TCP_LOG_OFF	// In : Use to activate Log if TCP_LOG_ON
);

/////////////////////////////////////////////////////////////////
//
// This function is used to initiates the use of the functionality of this file
//
// used by :
//		Server, Client
//
/////////////////////////////////////////////////////////////////
BOOL TCP_Stop
(
	void
);

/////////////////////////////////////////////////////////////////
//
// This function is used to initiates a Server socket, attached to a Port Number specified, ready to accept a Client connection
//
// used by :
//		Server
//
/////////////////////////////////////////////////////////////////
int TCP_Server_Create
(
	DWORD		i_dw_Port,	// In : Port Number that identifies the application
	SOCKET		*o_px_Sk	// Out: Socket initiated and returned
);

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
);

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
);

/////////////////////////////////////////////////////////////////
//
// This function is used to establishes a connection to a Server, specified by its Address, and it's Port Number
//
// used by :
//		Client
//
/////////////////////////////////////////////////////////////////
int TCP_Client_Connect
(
	char *			i_pc_Adress,	// In : Server Address IP Format ("127.0.0.1") or URL Format
	unsigned long	i_ul_Port,		// In : Server Port Number
	SOCKET			*o_px_Sk,		// Out: Socket created to the Server
	sockaddr_in		*o_px_Adress	// Out: Saved address 
);

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
);

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
);

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
);
