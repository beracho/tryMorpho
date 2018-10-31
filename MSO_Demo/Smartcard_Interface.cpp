/***********************************************************
  Copyright (c) 2002, SAGEM SA
************************************************************

 PROJET      : MorphoSmart
 MODULE      : 

 FILE NAME   : $Workfile:   Smartcard_Interface.cpp  $

 REVISION    : $Revision:   1.3  $

 DATE        : $Date:   Jul 07 2004 14:35:36  $

************************************************************/

#include "stdafx.h"
#include "mso_demo.h"
#include "Morpho_Interface.h"
#include <winscard.h>
#include "Smartcard.h"
#include "Smartcard_Interface.h"

CSmartcard	* g_pcl_card = NULL;
C			g_pc_readername [128];
B			g_b_polling;
B			g_b_async_events;
HANDLE		g_h_synchro_mutex;

/*______________________________FUNCTION HEADER_____________________________*/
/*																			*/
/* Name:		CARD_Initialize												*/
/*																			*/
/* Arguments:	None														*/
/*																			*/
/* Description:	Initialization of the Smart Card layer						*/
/*																			*/
/* Return		Error Code													*/
/*					- MORPHO_OK : The cmd has been successfully executed	*/
/*					- MORPHO_BUSY : Already running							*/
/*					- MORPHO_CANT_OPEN_FILE : Couldn't open COM port		*/
/*					- MORPHO_NOT_INITIALIZED : Error while initializing		*/
/*__________________________________________________________________________*/
L	CARD_Initialize (void)
{
	L	l_l_res;
	
	
	if (g_pcl_card != NULL)
	{
		B	l_b_res = g_pcl_card->GetReader (g_pc_readername);
		
		if (l_b_res && (g_pc_readername != NULL) && (g_pc_readername[0] != 0))
		{
			l_l_res = SMARTCARD_BUSY;
		}
		else
		{
			l_l_res = SMARTCARD_INITIALIZE_ERR;
		}
	}
	else
	{
		CWinThread * l_h_thread;
		
		g_pcl_card = new CSmartcard;
		g_pcl_card->GetReader (g_pc_readername);

		if ((g_pc_readername != NULL) &&
			(g_pc_readername[0] != 0))
		{
			g_h_synchro_mutex = CreateMutex (NULL, FALSE, NULL);
			g_b_async_events = FALSE;
	
			// Launch the polling thread to get the changed status
			g_b_polling = TRUE;
			l_h_thread = AfxBeginThread (CARD_CardDetectionThread, NULL);

			l_l_res = MORPHO_OK;
		}
		else
		{
			delete g_pcl_card;
			g_pcl_card = NULL;
			l_l_res = SMARTCARD_NOT_FOUND;
		}
	}

	return l_l_res;
}

/*______________________________FUNCTION HEADER_____________________________*/
/*																			*/
/* Name:		CARD_CardDetectionThread									*/
/*																			*/
/* Arguments:	LPVOID lpParam : Not used									*/
/*																			*/
/* Description:	Polls the reader to check if a card is inserted				*/
/*																			*/
/* Return		UINT : 0 : Normal exit										*/
/*					  -1 : Problem detected									*/
/*__________________________________________________________________________*/
UINT	CARD_CardDetectionThread (LPVOID lpParam)
{

	B	l_b_card_inserted = FALSE;

	if (g_pcl_card == NULL)
	{
		return -1;
	}

	while (g_b_polling)
	{		
		// Reserve Mutex		
		WaitForSingleObject (g_h_synchro_mutex, INFINITE);

		if (g_pcl_card->IsCardPresent(g_pc_readername))
		{															
			// Card detected : Connect !
			if (!g_pcl_card->m_isConnected)
			{
				g_pcl_card->Connect (g_pc_readername);
			}

			// Notify if state changed
			if (!l_b_card_inserted)
			{		
				UC					l_puc_buffer [40];
				UL					l_ul_length = 39;

				if (g_pcl_card->GetATR (l_puc_buffer+1, &l_ul_length) == MORPHO_OK)
				{
					// Increment data length (status + ATR)
					l_ul_length ++;
				}
				else
				{
					// Data length = 1 (Status only)
					l_ul_length = 1;
				}		
			}

			l_b_card_inserted = TRUE;
		}
		else
		{		
			// Card retrieved : Disconnect !
			if (g_pcl_card->m_isConnected)
			{
				g_pcl_card->Disconnect ();				
			}

			l_b_card_inserted = FALSE;
		}
	
		ReleaseMutex (g_h_synchro_mutex);
		Sleep (100);
	}

	return 0;
}
