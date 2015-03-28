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



/**
 ******************************************************************************
 * Private macros definition
 *
 */

/**
 ******************************************************************************
 * Private type definition
 *
 */


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
static Timer_s*		 FirstTimer;

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
				unsigned long	CallBack	)
{
	Timer_s* pNewTimer = (Timer_s)malloc(sizeof(Timer_s));
	if(!pNewTimer)
		return pNewTimer;
	mem_ClearZone(pNewTimer, sizeof(Timer_s));

	pNewTimer->CallBackFunction = CallBack;
	pNewTimer->CountValue_ms 	= Value_ms;
}

/**
  * @brief  Demarrer le timer
  * @param	pTimer	Adresse du timer
  */
void
TSW_Start(Timer_s*	 	pTimer)
{

	if(Timer->Status != STATUS_ENCOURS)	{

		if(!FirstTimer) {
			FirstTimer = pTimer;
		} else {
			LISTLINEAR_HEAD_INIT(pTimer);
			list_add(pTimer, FirstTimer, 0);
		}
	}

	pTimer->Status 			= STATUS_ENCOURS;
	pTimer->Stop_Value_ms 	= msTicks + pTimer->CountValue_ms;
	pTimer->Start_Value_ms 	= msTicks;
}
