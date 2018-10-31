/***********************************************************
  Copyright (c) 2002, SAGEM SA
************************************************************

 PROJET      : MorphoSmart
 MODULE      : 

 FILE NAME   : $Workfile:   Smartcard_Interface.h  $

 REVISION    : $Revision:   1.3  $

 DATE        : $Date:   Jul 07 2004 11:22:40  $

************************************************************/

/*_____________________________BEGIN_CONSTANTS______________________________*/
#define CARD_STATUS		1
/*______________________________CONSTANTS_END_______________________________*/

/*_______________________________BEGIN_TYPES________________________________*/
typedef struct
{
	L	m_l_result;
	UL	m_ul_length;
	PUC	m_puc_data;

} T__CALLBACK_RESULT, * PT__CALLBACK_RESULT;

typedef void (* PT__CALLBACK) (PT__CALLBACK_RESULT);

typedef struct
{
	UC	m_uc_cla;
	UC	m_uc_ins;
	UC	m_uc_p1;
	UC	m_uc_p2;
	UC	m_uc_p3;
	UC	m_uc_length;
	UC	m_puc_data[256];

} T__CARD_APDU, *PT__CARD_APDU;


typedef struct
{
	UC	m_uc_length;
	UC	m_puc_data[256];
	UC	m_uc_sw1;
	UC	m_uc_sw2;

} T__CARD_APDU_RESULT, *PT__CARD_APDU_RESULT;

/*________________________________TYPES_END_________________________________*/

/*_______________________________BEGIN_MACROS_______________________________*/
/*________________________________MACROS_END________________________________*/

/*______________BEGIN_GLOBAL_VARIABLES_DECLARATIONS/DEFINITIONS_____________*/
/*_______________GLOBAL_VARIABLES_DECLARATIONS/DEFINITIONS_END______________*/

/*_______________________BEGIN_FUNCTIONS_DEFINITIONS________________________*/


L			CARD_Initialize (void);
L			CARD_Configure (PT__CALLBACK i_pfunc_callback);
L			CARD_Close ();

UINT		CARD_CardDetectionThread (LPVOID lpParam);
const PC	CARD_GetReaderInfo ();
L			CARD_GetAtr (PUC o_puc_buffer, PUL io_pul_size);
L			CARD_SendAPDU (PT__CARD_APDU i_pst_apdu, PT__CARD_APDU_RESULT p_pst_res);

/*________________________FUNCTIONS_DEFINITIONS_END_________________________*/


/***********************************************************************
							Smartcard MSO_Demo Errors
 2456 to 2479 reserved
***********************************************************************/
#define SMARTCARD_OK				0		// The smart card reader is already running
#define SMARTCARD_BUSY				-2456	// The smart card reader is already running
#define SMARTCARD_INITIALIZE_ERR	-2457	// Error while initializing the smart card reader
#define SMARTCARD_NOT_FOUND			-2458	// Couldn't find the device
#define SMARTCARD_ERROR				-2459
#define SMARTCARD_NO_ATR			-2460	// ATR not retrieved
#define SMARTCARD_BADPARAMETER		-2461
