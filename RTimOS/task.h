/**
  ******************************************************************************
  * @file      task.h
  * @author    duclos timothe
  * @version   V0.1
  * @date      07/03/2015
  * @brief     Gestion des taches de l'OS
  ******************************************************************************
*/

#ifndef TASK_H
#define TASK_H

/**
   ******************************************************************************
 * Include
 *
 */
#include "list.h"

/**
 ******************************************************************************
 * Exported macros definition
 *
 */
/* @brief GCC Way to halt a cpu */
#define stop_cpu \
	__builtin_trap()

/**
 ******************************************************************************
 * Exported type definition
 *
 */
typedef enum {
	FAIL = 0,
	PASS = !FAIL
}Status_e;

/** @brief Etat d'une tache */
typedef enum {
	TaskReady = 0,
	TaskRunning,
	TaskWaiting
}TaskState_s;

/** @brief Task structure */
typedef struct _Task_s{

	//----------- list
	list_head_s	list;

	//----------- Stack of task
	unsigned long* 		pStack;
	unsigned long 		StackSize;

	//----------- State of task
	TaskState_s			State;

	//----------- Process Stack pointer adress value
	unsigned int		PSP_value;

	//---------- Timer Task
	unsigned long		ResartValue_ticks;

}Task_s;

/**
 ******************************************************************************
 * Exported function prototypes
 *
 */
inline unsigned long getSystickCount();
unsigned char 	initTimOS	();
Task_s* 		Task_Create	(	unsigned long StackSize,
								unsigned long _ptr_TaskFunction,
								void* _ptr_Param	);
void Task_suspend(unsigned long TickToWait_ticks);



#endif /** TASK_H */

/***********************************END OF FILE*******************************/
