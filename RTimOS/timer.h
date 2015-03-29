/**
  ******************************************************************************
  * @file      timer.h
  * @author    duclos timothe
  * @version   V0.1
  * @date      26/03/2015
  * @brief     Gestion des timers de l'OS
  ******************************************************************************
*/

#ifndef TIMER_H
#define TIMER_H

/**
   ******************************************************************************
 * Include
 *
 */
#include "list.h"

// TODO: Choisir, tick ou MScount ou faire la macro

/**
 ******************************************************************************
 * Exported type definition
 *
 */
typedef struct {

	list_head_s		list;			/** Double linked list*/

	Status_e		Status;			/** Status du Timer */

	unsigned long	Stop_Value_ms;	/** Valeur de depart du timer */
	unsigned long	Pause_Value_ms;	/** Valeur de comptage du timer lors de l'arret */
	unsigned long	Start_Value_ms;	/** Valeur de fin du timer */

	unsigned long	CountValue_ms;	/** Value that the timer will count */

	unsigned long	CallBackFunction;	/** Function's adress to call at timer's end */
	void*			pParameter;			/** Timer callback's parameter*/

	unsigned char	NeverEnding;		/** Put its value to one if you want the timer to auto restart */

}Timer_s;

/**
 ******************************************************************************
 * Exported Macros
 *
 */
/** @brief Is the specified timer finished
 *  @param pTIMER	Timer's adress
 */
#define __Timer_IsFinished(pTIMER) \
	!pTimer->Status

/**
 ******************************************************************************
 * Exported Function prototype
 *
 */
Timer_s* Timer_Create(unsigned long	Value_ms, unsigned long pCallBack, void* pParam, unsigned char	AutoRestart);
void Timer_Start(Timer_s* pTimer);
unsigned long Timer_Stop(Timer_s* pTimer);

inline unsigned long Timer_GetElapsedTime_ms(Timer_s* pTimer);
inline unsigned long Timer_GetRemainingTime_ms(Timer_s* pTimer);


inline unsigned long Timer_GetTickCount();

inline void Timer_Tick();



#endif /** TIMER_H */

/***********************************END OF FILE*******************************/
