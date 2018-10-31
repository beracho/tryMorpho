/************************************************************
  Copyright (c) 2002-2005, Morpho 
************************************************************/

#include <scarderr.h>


/***********************************************************************
							Smartcard Demo Errors
 2456 to 2479 reserved
***********************************************************************/
#define SMARTCARD_OK								0		// The smart card reader is already running
#define SMARTCARD_BUSY								-2456	// 0xF668 The smart card reader is already running
#define SMARTCARD_INITIALIZE_ERR					-2457	// 0xF667 Error while initializing the smart card reader
#define SMARTCARD_NOT_FOUND							-2458	// 0xF666 Couldn't find the device
#define SMARTCARD_SCARD_ERROR						-2459	// 0xF665
#define SMARTCARD_NO_ATR							-2460	// 0xF664 ATR not retrieved
#define SMARTCARD_BADPARAMETER						-2461	// 0xF663
#define SMARTCARD_ALLOC_FAILED						-2462	// 0xF662
#define SMARTCARD_GET_RESPONSE_DATA_STILL_AVAILABLE -2463	// 0xF661
#define SMARTCARD_STATUS_BYTES_ERROR				-2464	// 0xF660
#define SMARTCARD_READER_ERROR						-2465	// 0xF65F
#define SMARTCARD_TAG_COMPACT_FORMAT_NOT_FOUND		-2466	// 0xF65E
#define SMARTCARD_BAD_PK_BUFFER_SIZE				-2467	// 0xF65D
#define SMARTCARD_INVALID_OFFSET					-2468	// 0xF65C
#define SMARTCARD_GET_RESPONSE_NOT_EXPECTED			-2469	// 0xF65B
#define SMARTCARD_BAD_IDENTIFIER_BUFFER_SIZE		-2470	// 0xF65A

