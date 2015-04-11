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
#include "timer.h"
#include "RtimOS_portCM4.h"

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
/** @brief Etat d'une tache */
typedef enum {
	TaskReady = 0,
	TaskRunning,
	TaskWaiting
}TaskState_e;

/** @brief Function pointer to a task's handler */
typedef void (*pFuncTask_f)(void* pArg);

/** @brief Task structure */
typedef struct _Task_s{

	//----------- list
	list_head_s	list;

	//----------- Stack of task
	unsigned long* 		pStack;
	unsigned long 		StackSize;

	//----------- State of task
	TaskState_e			State;

	//----------- Process Stack pointer adress value
	unsigned int		PSP_value;

	//---------- Timer Task
	unsigned long		ResartValue_ticks;

}Task_s;

/** @brief	Structure d'une semaphore */
typedef struct {

	Task_s* Task;

}Semaphore_s;

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

void Task_Delay(Rui32 TickToWait_ticks);
Rui32 Task_DelayUntil(Rui32 PreviousValue_tick, Rui32 nbTicksToDelay);

Rui8 Semaphore_Give(Semaphore_s* Sem);
Rui8 Semaphore_Take(Semaphore_s* Sem);

#endif /** TASK_H */

/***********************************END OF FILE*******************************/
