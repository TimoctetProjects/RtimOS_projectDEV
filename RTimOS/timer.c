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
  * 					(usefull if several timer share the same function)
  * 		AutoRestart	Allow the timer to auto-restart at it's end
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
	Timer_s* _pCurrentTimer;

	// TODO: assert param
	if(!pTimer)
		return;

//	pTimer->Stop_Value_ms 	= msTicks + pTimer->CountValue_ms;
//	pTimer->Start_Value_ms 	= msTicks;
//
//	//--------- If the Timer isn't running
//	if(!pTimer->Status)
//	{
//
//		pTimer->Status = STATUS_ENCOURS;
//
//		// No other timer, so this is our new pFirstTimer
//		if(!pFirstTimer)
//		{
//			pFirstTimer = pTimer;
//			LISTCIRCULAR_HEAD_INIT(pFirstTimer);
//		}
//
//		// We place the Timer in the right position if ther are others
//		else
//		{
//			LISTCIRCULAR_HEAD_INIT(pTimer);
//
//			// Get to the right position
//			for(_pCurrentTimer = pFirstTimer;
//				List_GetNext(Timer_s, _pCurrentTimer)->Stop_Value_ms > _pCurrentTimer->Stop_Value_ms
//				&& 	pTimer->Stop_Value_ms > _pCurrentTimer->Stop_Value_ms;
//				_pCurrentTimer = List_GetNext(Timer_s, _pCurrentTimer)		);
//
//			// Add the timer
//			list_add(pTimer, _pCurrentTimer);
//
//		}
//	}
//
//	//--------- We adjust the timer's position in the list if it is running
//	else
//	{
//		// Get to the right position
//		for(_pCurrentTimer = pTimer;
//			List_GetNext(Timer_s, _pCurrentTimer)->Stop_Value_ms < _pCurrentTimer->Stop_Value_ms;
//			//&& 	pTimer->Stop_Value_ms >= _pCurrentTimer->Stop_Value_ms;
//			_pCurrentTimer = List_GetNext(Timer_s, _pCurrentTimer)		);
//
//		// If they're the same, then we already are at a good position
//		if(_pCurrentTimer == pTimer)
//			return;
//
//
//		//------------------ Gestion des pointeurs HAL Timer
//		// Si on doit changer la position du timer
//		// Mais qu'un pointeur etait dessus
//		if(pTimer == pFirstTimer)
//		{
//			Timer_s* ASUP = pFirstTimer;
//
//			// TODO: A factoriser
//			// UpdatepFirstTimer
//			//----------- Gestion du premier pointeur de la liste pFirstTimer
//			// Si c'est le seul element
//			//if(List_GetNext(Timer_s, pTimer) == pTimer)
//			if(List_GetNext(Timer_s, pTimer) == List_GetPrev(Timer_s, pTimer))
//				pFirstTimer = NULL;
//
//			// Sinon on fait pointer sur l'element suivant
//			else
//				pFirstTimer = List_GetNext(Timer_s, pTimer);
//
//			if(pCurrentTimer == ASUP)
//				pCurrentTimer = pFirstTimer;
//
//			// A ce stade, on l'ajoutera forcement apres le premiere timer
//			// Tant que pFirstTimer pointe bien sur le premier element
//		}
//
//		// Si c'est le timer courrement pointe (prochain traite dans l'IRQ)
//		else if(pTimer == pCurrentTimer)
//		{
//			// Si c'est le dernier element
//			// TODO: A factoriser dans une foncton
//			// Timer_liste_IsLastElement
//
//			// Si c'est le dernier element
//			if(pTimer->Stop_Value_ms < List_GetNext(Timer_s, pTimer)->Stop_Value_ms)
//				pCurrentTimer = List_GetPrev(Timer_s, pTimer);
//
//			// Si c'est le premier element c'est deja gere
//
//			// Donc sinon
//			else
//				pCurrentTimer = List_GetNext(Timer_s, pTimer);
//
//		}
//
//		// Si c'est le dernier element
////		else if(pTimer->Stop_Value_ms > List_GetNext(Timer_s, pTimer)->Stop_Value_ms)
////						pFirstTimer = List_GetPrev(Timer_s, pTimer);
//
//		//----------- Replacement dans la liste
//		// Delete the timer from list
//		list_del(pTimer);
//
//		// Add the timer to the new position in list
//		list_add(pTimer, _pCurrentTimer);
//	}

	pTimer->Stop_Value_ms 	= msTicks + pTimer->CountValue_ms;
	pTimer->Start_Value_ms 	= msTicks;

	//--------- If the Timer isn't running
	if(!pTimer->Status)
	{

		pTimer->Status = STATUS_ENCOURS;

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

	// TODO: Factoriser : créer TSW_RestartTimer (appelée dans TSW_satart s'il est en cours
	// Pareil avec le TSW_start, en faire une prive qui le demarre from scratch
	//--------- We adjust the timer's position in the list if it is running
	else
	{
		// Get to the right position
		for(_pCurrentTimer = pTimer;
				List_GetNext(Timer_s, _pCurrentTimer)->Stop_Value_ms <= pTimer->Stop_Value_ms
			&&	_pCurrentTimer != NULL;
			_pCurrentTimer = List_GetNext(Timer_s, _pCurrentTimer)		);

	/*	for(_pCurrentTimer = pTimer;
				List_GetNext(Timer_s, _pCurrentTimer)
			&& 	pTimer->Stop_Value_ms > _pCurrentTimer->Stop_Value_ms;
			_pCurrentTimer = List_GetNext(Timer_s, _pCurrentTimer)		);*/

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

		// If it's the first timer and ther is no next
//		else if(pTimer == pFirstTimer && (List_GetNext(Timer_s, pTimer) == NULL))
//		{
//			// Pcurrent position would be the same as pTimer as there would be no other one
//			// So unreachable
//		}

		else if(pTimer != pFirstTimer)
		{
			// Delete the timer
			list_del(pTimer);

			// Add the timer
			list_add(pTimer, _pCurrentTimer);
		}
	}
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


// TODO: Deplacer dans config
#define TIMER_CHECK_FOR_LIST_INTEGRITY	1

// TODO: Deplacer la haut dans PRIVATE FUNCTION PROTOTYPE
#if TIMER_CHECK_FOR_LIST_INTEGRITY
	unsigned char IsTimerList_OK(Timer_s* pFirst);
#endif /** TIMER_CHECK_FOR_LIST_INTEGRITY */

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

//	if(!pCurrentTimer)
//		pCurrentTimer = pFirstTimer;

//	while( !done )
//
//	{
//		if(pCurrentTimer->Stop_Value_ms <= msTicks && pCurrentTimer->Status)
//		{
//			if(!pCurrentTimer->NeverEnding)
//			{
//				Timer_s* _pCurrentTimer = pCurrentTimer;
//
//				_pCurrentTimer->Status = STATUS_FINIS;
//
//				// Si le timer est fini et doit etre supprime
//				if(List_GetNext(Timer_s, pCurrentTimer) != List_GetPrev(Timer_s, pCurrentTimer))
//				{
//					if(pCurrentTimer->Stop_Value_ms > List_GetNext(Timer_s, pCurrentTimer)->Stop_Value_ms)
//						pCurrentTimer = List_GetPrev(Timer_s, pCurrentTimer);
//					else
//						pCurrentTimer = List_GetNext(Timer_s, pCurrentTimer);
//
//					list_del(_pCurrentTimer);
//					LISTLINEAR_HEAD_INIT(_pCurrentTimer);
//				}
//
//				// Si c'est le seul timer
//				else
//				{
//					pCurrentTimer = NULL;
//					LISTLINEAR_HEAD_INIT(_pCurrentTimer);
//					done = 1;
//				}
//			}
//
//			else
//				Timer_Start(pCurrentTimer);
//
//			if(pCurrentTimer->CallBackFunction)
//				((pFunctionTimer_t)(pCurrentTimer->CallBackFunction))(pCurrentTimer->pParameter);
//
//			//pCurrentTimer = List_GetNext(Timer_s, pCurrentTimer);
//		}
//
//		else	done = 1;
//	}

	//Timer_s* pOldFirstTimer = pFirstTimer;

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

			/*if(pOldPosition != pFirstTimer)
				pFirstTimer = List_GetNext(Timer_s, pFirstTimer);*/
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
