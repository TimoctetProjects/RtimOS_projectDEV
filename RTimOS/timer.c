/**
  ******************************************************************************
  * @file      timer.h
  * @author    duclos timothe
  * @version   V0.1
  * @date      26/03/2015
  * @brief     Gestion des timers de l'OS
  ******************************************************************************
*/

/**
   ******************************************************************************
 * Include
 *
 */
#include "timer.h"
#include "mem.h"
#include <stdlib.h>

/**
 ******************************************************************************
 * Private macros definition
 *
 */
#define TAILLE_TSW_32_bits	0xFFFFFFFF

/**
 ******************************************************************************
 * Private type definition
 *
 */
/** @brief Pointeur de fonction pour les callback des timers */
typedef void (*pFunctionTimer)(void* pParam);

/**
 ******************************************************************************
 * Private function prototypes
 *
 */

/**
 ******************************************************************************
 * Private variables definition
 *
 */
static unsigned long msTicks;
static Timer_s*		 pFirstTimer;

/**
 ******************************************************************************
 * Exported function definition
 *
 */
/**
  * @brief  Create the timer
  * @param	Value_ms	Value to count, expressed in millisecond
  * 		pCallBack	Function's adress to call on timer's end
  * @retval	Newly created Timer's adress
  */
Timer_s*
Timer_Create(	unsigned long	Value_ms,
				unsigned long	CallBack,
				void*			pParam,
				unsigned char	AutoRestart	)
{
	// TODO: assert param

	Timer_s* pNewTimer = (Timer_s *)malloc(sizeof(Timer_s));
	if(!pNewTimer)
		return pNewTimer;
	mem_ClearZone(pNewTimer, sizeof(Timer_s));

	pNewTimer->CallBackFunction = CallBack;
	pNewTimer->CountValue_ms 	= Value_ms;
	pNewTimer->pParameter		= pParam;
	pNewTimer->NeverEnding		= AutoRestart;

	return pNewTimer;
}

/**
  * @brief  Demarrer le timer
  * @param	pTimer	Adresse du timer
  */
void
Timer_Start(	Timer_s* pTimer	)
{
	// TODO: assert param

	if(pTimer->Status != STATUS_ENCOURS)	{

		if(!pFirstTimer) {
			pFirstTimer = pTimer;
			LISTLINEAR_HEAD_INIT(pFirstTimer);
		} else {
			LISTLINEAR_HEAD_INIT(pTimer);
			list_add(pTimer, pFirstTimer);

		}
	}

	pTimer->Status 			= STATUS_ENCOURS;
	pTimer->Stop_Value_ms 	= msTicks + pTimer->CountValue_ms;
	pTimer->Start_Value_ms 	= msTicks;
}


/**
  * @brief  Lecture of the number of tick
  * @retval	Number of ticks that occured
  */
inline unsigned long
Timer_GetTickCount()
{
	return msTicks;
}

/**
  * @brief  Demarrer le timer
  * @param	pTimer	Adresse du timer
  */
inline void
Timer_Tick()
{
	Timer_s* pCurrentTimer;

	if(msTicks == TAILLE_TSW_32_bits) {
			msTicks=0;
	} else 	msTicks++;

	if(!pFirstTimer)
		return;

	// Look over the entire list
	for(pCurrentTimer = pFirstTimer;
		pCurrentTimer;
		pCurrentTimer = List_GetNext(Timer_s, pCurrentTimer))
	{
		if(pCurrentTimer->Stop_Value_ms <= msTicks && pCurrentTimer->Status) {


			if(!pCurrentTimer->NeverEnding) {

				// Set status to finish
				pCurrentTimer->Status = STATUS_FINIS;

				// Suppres Timer from list
				list_del(pCurrentTimer);
			}

			else {

				pCurrentTimer->Stop_Value_ms = msTicks + pCurrentTimer->CountValue_ms;
			}

			// Execute Timer's callback
			if(pCurrentTimer->CallBackFunction)
				((pFunctionTimer)(pCurrentTimer->CallBackFunction))(pCurrentTimer->pParameter);
		}
	}
}
