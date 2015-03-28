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

}Timer_s;

/**
 ******************************************************************************
 * Exported Function prototype
 *
 */
Timer_s* Timer_Create(unsigned long	Value_ms, unsigned long pCallBack);



#endif /** TIMER_H */

/***********************************END OF FILE*******************************/
