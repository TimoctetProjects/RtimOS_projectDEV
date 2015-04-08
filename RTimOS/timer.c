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

// TODO: Modif config timer

/**
 ******************************************************************************
 * Private macros definition
 *
 */
#define TAILLE_TSW_32_bits	0xFFFFFFFF

// TODO: Deplacer dans config
#define TIMER_CHECK_FOR_LIST_INTEGRITY	1

#if TIMER_CHECK_FOR_LIST_INTEGRITY
	unsigned char IsTimerList_OK(Timer_s* pFirst);
#endif /** TIMER_CHECK_FOR_LIST_INTEGRITY */

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
static inline void _timer_restart	(Timer_s* pTimer);
static inline void _timer_start		(Timer_s* pTimer);

/**
 ******************************************************************************
 * Private variables definition
 *
 */
static unsigned long msTicks;
static Timer_s*	pFirstTimer;

/**
 ******************************************************************************
 * Exported function definition
 *
 */
/**
  * @brief  Create the timer
  * @param	Value_ms	Value to count, expressed in millisecond
  * 		pCallBack	Function's adress to call on timer's end
  * 		pParam		Parameter to pass to the timer's callback
  * 					(usefull if several timer share the  same
  * 					function)
  * 		AutoRestart	Allow the timer to auto-restart at it's
  * 					end
  * @retval	Newly created Timer's adress
  */
Timer_s*
Timer_Create(	unsigned long	 Value_ms,
				pFunctionTimer_t CallBack,
				void*			 pParam,
				unsigned char	 AutoRestart	)
{
	// TODO: assert param

	Timer_s* pNewTimer = (Timer_s *)malloc(sizeof(Timer_s));
	if(!pNewTimer)
		return pNewTimer;
	mem_ClearZone(pNewTimer, sizeof(Timer_s));

	pNewTimer->CallBackFunction = (unsigned long)CallBack;
	pNewTimer->CountValue_ms 	= Value_ms;
	pNewTimer->pParameter		= pParam;
	pNewTimer->NeverEnding		= AutoRestart;

	return pNewTimer;
}

/**
  * @brief  Start the specified timer
  * @param	pTimer	Timer's adress
  */
void
Timer_Start(Timer_s* pTimer)
{
	// TODO: assert param
	if(!pTimer)
		return;

	//-------- Start or Restart the timer
	if(!pTimer->Status)		_timer_start	(pTimer);
	else					_timer_restart	(pTimer);

}

/**
  * @brief  Get the specified timer's elapsed time
  * @param	pTimer	Timer's adress
  * @retval	Elapsed time
  */
unsigned long
Timer_Stop(Timer_s* pTimer)
{
	if(!pTimer)
		return 0;

	if(pTimer->Status) {

		unsigned long rtrn = Timer_GetElapsedTime_ms(pTimer);

		list_del(pTimer);
		LISTLINEAR_HEAD_INIT(pTimer);

		pTimer->Status 			= STATUS_FINIS;
		pTimer->Start_Value_ms 	= 0;
		pTimer->Stop_Value_ms 	= 0;

		return rtrn;
	}

	return 0;
}

/**
  * @brief  Reset the specified timer
  * @param	pTimer	Timer's adress
  */
void
Timer_Reset(Timer_s* pTimer)
{
	if(!pTimer)
		return;

	if(pTimer->Status) {
		list_del(pTimer);
		LISTLINEAR_HEAD_INIT(pTimer);
	}

	pTimer->Status 			= STATUS_FINIS;
	pTimer->Start_Value_ms 	= 0;
	pTimer->Stop_Value_ms 	= 0;
}

/**
  * @brief  Get the specified timer's elapsed time
  * @param	pTimer	Timer's adress
  * @retval	Elapsed time
  */
inline unsigned long
Timer_GetElapsedTime_ms(Timer_s* pTimer)
{
	if(pTimer->Status && pTimer)
		return (msTicks - pTimer->Start_Value_ms);

	return 0;
}

/**
  * @brief  Get the specified timer's remaining time
  * @param	pTimer	Timer's adress
  * @retval	Remaining time
  */
inline unsigned long
Timer_GetRemainingTime_ms(Timer_s* pTimer)
{
	if(pTimer->Status && pTimer)
		return (pTimer->Stop_Value_ms - msTicks);

	return 0;
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
	unsigned char done = 0;

	if(msTicks == TAILLE_TSW_32_bits) {
			msTicks=0;
	} else 	msTicks++;

	if(!pFirstTimer)
		return;

	if(msTicks == 450) {
		asm volatile("nop \n\r");
	}

	if(msTicks == 500) {
		asm volatile("nop \n\r");
	}

	while(!done)
	{
		if(pFirstTimer->Stop_Value_ms <= msTicks && pFirstTimer->Status) {

			// Execute Timer's callback
			if(pFirstTimer->CallBackFunction)
				((pFunctionTimer_t)(pFirstTimer->CallBackFunction))(pFirstTimer->pParameter);

			if(!pFirstTimer->NeverEnding)
			{
				Timer_s* pOldFirstTimer = pFirstTimer;

				// Set status to finish
				pOldFirstTimer->Status = STATUS_FINIS;

				if(List_GetNext(Timer_s, pOldFirstTimer) != NULL)
					pFirstTimer = List_GetNext(Timer_s, pOldFirstTimer);
				else
					pFirstTimer = NULL;

				// Suppres Timer from list
				list_del(pOldFirstTimer);
				LISTLINEAR_HEAD_INIT(pOldFirstTimer);

			}

			else
				Timer_Start(pFirstTimer);
		}

		else 	done = 1;
	}

	#if TIMER_CHECK_FOR_LIST_INTEGRITY
		if(!IsTimerList_OK(pFirstTimer))
			asm volatile("nop \n\r");	// TODO: Launche a high priority task related to the define TIMER_CHECK_FOR_LIST_INTEGRITY handling this error
										// Maybe re-arrange the list, that would be awesome ! (just for the algorythm)
										// And maybe put two level, on where we just assert, another where we re-arrange the list
										// And if we can't, we while(1)
	#endif /** TIMER_CHECK_FOR_LIST_INTEGRITY */
}

/**
 ******************************************************************************
 * Private function definition
 *
 */
/**
  * @brief  Demarrer un timer eteint
  * @param	pTimer	Adresse du timer
  */
static inline void
_timer_start(Timer_s* pTimer)
{
	Timer_s* _pCurrentTimer;

	pTimer->Status 			= STATUS_ENCOURS;
	pTimer->Stop_Value_ms 	= msTicks + pTimer->CountValue_ms;
	pTimer->Start_Value_ms 	= msTicks;

	// No other timer, so this is our new pFirstTimer
	if(!pFirstTimer)
	{
		pFirstTimer = pTimer;
		LISTLINEAR_HEAD_INIT(pFirstTimer);
	}

	// We place the Timer in the right position if ther are others
	else
	{
		LISTLINEAR_HEAD_INIT(pTimer);

		// Get to the right position
		for(_pCurrentTimer = pFirstTimer;
				List_GetNext(Timer_s, _pCurrentTimer)
			&& 	pTimer->Stop_Value_ms > _pCurrentTimer->Stop_Value_ms;
			_pCurrentTimer = List_GetNext(Timer_s, _pCurrentTimer)		);

		// Add the timer
		list_add(pTimer, _pCurrentTimer);
	}
}

/**
  * @brief  Demarrer un timer deja en cours
  * @param	pTimer	Adresse du timer
  */
static inline void
_timer_restart(Timer_s* pTimer)
{
	Timer_s* _pCurrentTimer;

	pTimer->Stop_Value_ms 	= msTicks + pTimer->CountValue_ms;
	pTimer->Start_Value_ms 	= msTicks;

	// Get to the right position
	for(_pCurrentTimer = pTimer;
			List_GetNext(Timer_s, _pCurrentTimer)->Stop_Value_ms <= pTimer->Stop_Value_ms
		&&	_pCurrentTimer != NULL;
		_pCurrentTimer = List_GetNext(Timer_s, _pCurrentTimer)		);

	if(_pCurrentTimer == pTimer)
		return;

	// If it's the first timer ans there is a next
	if(pTimer == pFirstTimer && (List_GetNext(Timer_s, pTimer) != NULL))
	{
		pFirstTimer = List_GetNext(Timer_s, pTimer);

		// Delete the timer
		list_del(pTimer);

		// Add the timer
		list_add(pTimer, _pCurrentTimer);

	}

	else if(pTimer != pFirstTimer)
	{
		// Delete the timer
		list_del(pTimer);

		// Add the timer
		list_add(pTimer, _pCurrentTimer);
	}
}



/**
 ******************************************************************************
 * Validation
 *
 */
#if TIMER_CHECK_FOR_LIST_INTEGRITY

	unsigned char
	IsTimerList_OK(Timer_s* pFirst)
	{
		while(List_GetNext(Timer_s, pFirst) != NULL)
		{
			if(pFirst->Stop_Value_ms > List_GetNext(Timer_s, pFirst)->Stop_Value_ms)
			{
				return 0;
			}

			pFirst = List_GetNext(Timer_s, pFirst);
		}

		return 1;
	}

#endif /** TIMER_CHECK_FOR_LIST_INTEGRITY */
