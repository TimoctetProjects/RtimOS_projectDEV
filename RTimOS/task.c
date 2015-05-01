/**
  ******************************************************************************
  * @file      task.c
  * @author    duclos timothe
  * @version   V0.1
  * @date      07/03/2015
  * @brief     Gestion des taches de l'OS
  ******************************************************************************
*/

/**
   ******************************************************************************
 * Include
 *
 */
#include "task.h"
#include <stdlib.h>
#include "mem.h"
#include "RtimOS_Config.h"
#include "stm32f4xx.h"
#include "RtimOS_portCM4.h"


/**
 ******************************************************************************
 * Private type definition
 *
 */
/** @brief Hardware stacked registers on exception entry*/
typedef struct {
	unsigned int r0;
	unsigned int r1;
	unsigned int r2;
	unsigned int r3;
	unsigned int r12;
	unsigned int lr;
	unsigned int pc;
	unsigned int psr;
}Stack_Frame_HW_s;

/** @brief Registers that will need to be stacked manually */
typedef struct {
	unsigned int r4;
	unsigned int r5;
	unsigned int r6;
	unsigned int r7;
	unsigned int r8;
	unsigned int r9;
	unsigned int r10;
	unsigned int r11;
}Stack_Frame_SW_s;

/**
 ******************************************************************************
 * Private function prototypes
 *
 */
__attribute__ ( ( isr, naked ) )
void PendSV_Handler(void);

__attribute__ ( ( isr, naked ) )
void SVC_Handler(void);

void SVC_Handler_C(Rui32* svc_args);

static void Task_GetNextTask();
inline Rui8 Task_IsWaitOver(Task_s* task);
static void Task_CheckForWaitingTask();
static void CleanTOPofStack();
static void IDLETask_Handler(void* pParam);
static void Task_Exit();
static void Task_StackInit(	Task_s*	_Task,
							Rui32 	StackSize,
							Rui32 	_ptr_TaskFunction,
							void*	_TaskArg );
inline void _Task_Del_FromRunningList();
inline void _Task_Insert_InWaitingList();
inline void _Task_Insert_ToRunningList(Task_s* pTask);


/**
 ******************************************************************************
 * Private variables definition
 *
 */
/** @brief	Running List's current task running*/
static Task_s*	CurrentTaskRunning;
/** @brief	Running List's next task running*/
static Task_s*	NextTaskToRun;

/** @brief	Waiting lists's first element */
static Task_s*	pFirstTaskWaiting;

static Task_s	TaskIDLE;

#define RTIMOS_CHECK_FOR_CPU_USAGE	1
#if RTIMOS_CHECK_FOR_CPU_USAGE
	static Rui32	TickInIDLE;
	static Rui32	TickInTasks;
	static Rui32*	pSystickCount = &TickInTasks;
	static Rui32	CpuUsage_pr100;
#else
	static Rui32 	SystickCount;
#endif /** RTIMOS_CHECK_FOR_CPU_USAGE */

/**
 ******************************************************************************
 * Exported function definition
 *
 */
/**
  * @brief  OS initialisation
  * @retval Initialisation Status (should never return)
  */
unsigned char
initTimOS()
{
	#if IS_VALIDATION

		unsigned char resultat = 1;

		#if VALIDATION_LIST_LINEAIRE
			resultat &= VALIDATION_list_linear_add();
		#endif /** VALIDATION_LIST_LINEAIRE */

		#if VALIDATION_LIST_CIRULAIR
			resultat &= VALIDATION_list_circular_add();
			resultat &= VALIDATION_list_circular_del();
		#endif /** VALIDATION_LIST_LINEAIRE */

			asm("nop");

	#endif /** IS_VALIDATION */

	SysTick_Config(SystemCoreClock/1000); 		// 1ms SysTick interrupt on 168MHz

	// Return FAIL if nos task can be loaded
	if(!CurrentTaskRunning)
		CurrentTaskRunning = &TaskIDLE;

	// Creation de la tache IDLE
	TaskIDLE.pStack = (Rui32*)malloc(128 * sizeof(Rui32));
	Task_StackInit(&TaskIDLE, 128, (Rui32)IDLETask_Handler, NULL);
	LISTCIRCULAR_HEAD_INIT(&TaskIDLE);

	CleanTOPofStack();

	//__set_PSP(((Rui32)(CurrentTaskRunning->pStack) + 128*4));
	NVIC_SetPriority(PendSV_IRQn, 0xFF); 		// Set PendSV to lowest	possible priority
	__set_CONTROL(0x3); 						// Switch to use Process Stack, in unprivileged
												// state by setting Control Register

	__ISB(); 									// Flush processor cache

	//LoadFirstTaskContext();
	__asm volatile("svc 0 \n\r");
	//Lauch_SVC_Service(SVC_SERVICE_STARTFRISTTASK);

	// Sould not reach here
	//stop_cpu;
	return FAIL;
}


/**
  * @brief  This is the task creator
  * @param  ptr_Stack			Pointeur to the task's stack
  * 		size_Stack			Taille de la stack
  * 		ptr_TaskFunction 	Pointer to the task's function
  * 		ptr_Param			Param to be passed to the task's function
  * @retval Pointer to task newly created (NULL si error)
  */
Task_s*
Task_Create(	Rui32 		StackSize,
				Rui32		_ptr_TaskFunction,
				void*		_ptr_Param)
{
	//----------------- Allocate memomry for Task
	Task_s* pNewTask = (Task_s*) malloc(sizeof(Task_s));
	if(!pNewTask)
		return pNewTask;
	mem_ClearZone(pNewTask, sizeof(Task_s));

	//------------------ Allocate memory for stack
	pNewTask->pStack = (typeof(pNewTask->pStack)) malloc(StackSize * sizeof(typeof(*pNewTask->pStack)));
	if(!pNewTask->pStack) {
		free(pNewTask);
		return pNewTask;
	}
	mem_ClearZone(pNewTask->pStack, StackSize * sizeof(Rui32));

	//--------------- Init Stack
	Task_StackInit(pNewTask, StackSize, _ptr_TaskFunction, _ptr_Param);


	//----------------- Prepare struct fields in task's handle
	LISTCIRCULAR_HEAD_INIT(pNewTask);
	pNewTask->StackSize = StackSize;
	pNewTask->State		= TaskReady;

	if(!CurrentTaskRunning) {
		CurrentTaskRunning = pNewTask;

	} else
		list_add(pNewTask, CurrentTaskRunning);

	return pNewTask;
}

/**
  * @brief  Suspend a task for a given number of tick
  * @param  nbTicksToDelay		Nomber of tick for the task to be suspended
  */
void
Task_Delay(Rui32 nbTicksToDelay)
{
	_Task_Del_FromRunningList();
	_Task_Insert_InWaitingList();

	#if	RTIMOS_CHECK_FOR_CPU_USAGE
		CurrentTaskRunning->ResartValue_ticks = TickInIDLE + TickInTasks + nbTicksToDelay;
	#else
		CurrentTaskRunning->ResartValue_ticks = SystickCount + nbTicksToDelay;
	#endif	/** RTIMOS_CHECK_FOR_CPU_USAGE */

	__asm volatile("svc 0x01 \n\r");
}

/**
  * @brief  Suspend a task for a given number of tick from a previous value
  * @param  PreviousValue_tick	Previous value in tick
  * 		nbTicksToDelay		Nomber of tick for the task to be suspended
  * @retval	New Previous value to keep
  */
Rui32
Task_DelayUntil(	Rui32 PreviousValue_tick,
					Rui32 nbTicksToDelay		)
{


	return;
}


Rui8
Task_Suspend()
{
	_Task_Del_FromRunningList();
	__asm volatile("svc 0x01 \n\r");
	return PASS;
}

Rui8
Task_Resume(Task_s* pTask)
{
	if(!pTask)	// Check for state
		return FAIL;

	_Task_Insert_ToRunningList(pTask);

	return PASS;
}

/**
 ******************************************************************************
 * Private function definition
 *
 */
/**
  * @brief  Init the stack of a task
  * @param  _StackSize			Size of the stack
  * @retval Pointer to the botom of the stack
  */
static void
Task_StackInit(	Task_s*	_Task,
				Rui32	StackSize,
				Rui32 	_ptr_TaskFunction,
				void*	_TaskArg )
{

	//    [ XPSR ] <-- 	Adresse la plus haute du contexte
	//    [ PC   ] <-- 	Program counter. Contiendra la valeur du point d'entrée
	//					initial à laquelle on veut que le thread demarre
	//    [ LR   ] <-- 	Adresse de retour de la fonction appelante
	//    [ R12  ] <-- 	SP
	//    [ R3   ] <--
	//    [ R2   ]
	//    [ R1   ]
	//    [ R0   ] <-- 	Arguments passe a la fonction

	//    [ R11   ]
	//    ...
	//    [ R4    ] <-- Adresse la plus basse du cotexte

	//Rui32* 				pStack_init  = _Task->pStack;
	Stack_Frame_HW_s* 	ProcessStack = NULL;

	//------------------ Initialize start of stack
	// In order to detect stack overflow
	/*for(; pStack_init<_Task->pStack+4; pStack_init++)
		*pStack_init = 0x5A5A5A5A;*/

	//------------------ Get end of Stack pointer
	// Make it point to R4
	// just like if it had already been called
	// Because the first thing we'll do is to unstack R4-R11
	_Task->PSP_value = 			((Rui32) _Task->pStack)
							+ 	StackSize*sizeof(Rui32)
							- 	(		sizeof(Stack_Frame_HW_s)
									+ 	sizeof(Stack_Frame_SW_s)	)
							- 	4;


	//------------------ Align on PSP adress like if it were a Stack_Frame_HW_s struct
	// This is done in order to simply acess stack region
	ProcessStack = (Stack_Frame_HW_s *) (_Task->PSP_value + sizeof(Stack_Frame_SW_s));

	//------------------ Initialize task argument (not tested)
	ProcessStack->r0 = (Rui32) _TaskArg;

	//------------------ Initialize task function adress
	// To the before last byte in task stack
	ProcessStack->pc = (Rui32) _ptr_TaskFunction;

	//----------------- Initialize return adress
	// If the task returns it will enter an empy infinity loop
	ProcessStack->lr = (Rui32) &Task_Exit;

	//------------------ Initialize xPCR
	// it's the last byte in task stack
	// Allow to enable Thumb mode (T bit to 1)
	ProcessStack->psr = 0x01000000;
}

/**
  * @brief  Get the next task to run
  */
static void
Task_GetNextTask()
{
	// If there is more than one task, no need to switch context
	if( 	!CurrentTaskRunning
		&& 	(		CurrentTaskRunning != (Task_s*)CurrentTaskRunning->list.next
				|| 	NextTaskToRun != CurrentTaskRunning	)	)
	{

		if(NextTaskToRun == CurrentTaskRunning)
			NextTaskToRun = List_GetNext(Task_s, CurrentTaskRunning);

		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
		__ISB();
	}
}

/**
  * @brief  Insert pTask in Running list
  * @param	pTask	Task's adress to add
  */
inline void
_Task_Insert_ToRunningList(Task_s* pTask)
{
	LISTCIRCULAR_HEAD_INIT(pTask);

	// Handle IDLE Task
	if(CurrentTaskRunning == &TaskIDLE)
	{
			NextTaskToRun = pTask;
			LISTCIRCULAR_HEAD_INIT(&TaskIDLE);

			#if RTIMOS_CHECK_FOR_CPU_USAGE

				pSystickCount = &TickInTasks;

				if(!TickInTasks)
					CpuUsage_pr100 = 0;
				else
					CpuUsage_pr100 = TickInIDLE*100 / TickInTasks;

			#endif /** RTIMOS_CHECK_FOR_CPU_USAGE */
	}

	else	list_add(pTask, CurrentTaskRunning);
}

/**
  * @brief  Suppress Current List from Running list
  */
inline void
_Task_Del_FromRunningList()
{
	// Si une seule tache tourne
	if(List_GetNext(Task_s, CurrentTaskRunning) == CurrentTaskRunning)
	{
		NextTaskToRun = &TaskIDLE;

		#if RTIMOS_CHECK_FOR_CPU_USAGE
			pSystickCount = &TickInIDLE;
		#endif /** RTIMOS_CHECK_FOR_CPU_USAGE */
	}

	// Sinon (au moins une autre tache active)
	else
	{
		NextTaskToRun = List_GetNext(Task_s, CurrentTaskRunning);
		list_del(CurrentTaskRunning);
	}
}

/**
  * @brief Insert Current task running in waiting list
  */
inline void
_Task_Insert_InWaitingList()
{
	LISTLINEAR_HEAD_INIT(CurrentTaskRunning);

	if(!pFirstTaskWaiting)
	{
		pFirstTaskWaiting = CurrentTaskRunning;
	}

	else
	{
		list_add(CurrentTaskRunning, pFirstTaskWaiting);
	}
}

/**
  * @brief  Read the current systick count
  * @retval Number of systicktick irq that occured
  */
inline Rui32
getSystickCount()
{
	#if RTIMOS_CHECK_FOR_CPU_USAGE
		return TickInIDLE + TickInTasks;
	#else
		return SystickCount;
	#endif /** RTIMOS_CHECK_FOR_CPU_USAGE */

}

/**
  * @brief  Return 1 if the task has finished waiting
  * @retval 0 (not finished) 1 (finished)
  */
inline Rui8
Task_IsWaitOver(Task_s* task)
{
	#if RTIMOS_CHECK_FOR_CPU_USAGE
		return (task->ResartValue_ticks <= (TickInIDLE + TickInTasks));
	#else
		return (task->ResartValue_ticks <= SystickCount);
	#endif /** RTIMOS_CHECK_FOR_CPU_USAGE */
}

/**
  * @brief  Check if a task has finished waiting
  */
static void
Task_CheckForWaitingTask()
{
	Task_s* _pCurrentTask;

	if(!pFirstTaskWaiting)
		return;

	for(	_pCurrentTask = pFirstTaskWaiting;
			_pCurrentTask;
			_pCurrentTask = List_GetNext(Task_s, _pCurrentTask)	)
	{
		if(Task_IsWaitOver(_pCurrentTask))
		{
			_pCurrentTask->ResartValue_ticks = 0;

			//-------------- Suppresion a la liste waiting
			// Si c'est le seul de la liste
			if(!List_GetNext(Task_s, _pCurrentTask) && !List_GetPrev(Task_s, _pCurrentTask))
			{
				pFirstTaskWaiting = NULL;
			}

			// Sinon (s'il y a d'autres elements)
			else
			{
				if(_pCurrentTask == pFirstTaskWaiting)
				{
					pFirstTaskWaiting = List_GetNext(Task_s, _pCurrentTask);
				}

				list_del(_pCurrentTask);
			}

			//-------------- Ajout a la liste running
			// Si c'est la tache IDLE qui tourne
			_Task_Insert_ToRunningList(_pCurrentTask);

			return;
		}
	}
}

/**
  * @brief  IDLE task's function
  */
void IDLETask_Handler(void* pParam)
{
	for(;;);
}

/**
  * @brief  Function called on task's exit
  */
void
Task_Exit()
{
	for(;;);
}

/**
 ******************************************************************************
 * Interruption & Exceptions Section
 *
 */

/**
  * @brief  Systick IRQ
  * 		This interruption will count time
  * 		& provoque a context switch
  */
void
Port_Systick_IRQ()
{
	//SystickCount++;

	#if RTIMOS_CHECK_FOR_CPU_USAGE
		(*pSystickCount)++;
	#else
		SystickCount++;
	#endif /** RTIMOS_CHECK_FOR_CPU_USAGE */

	Timer_Tick();
	Task_CheckForWaitingTask();
	Task_GetNextTask();
}

void SVC_Handler(void)
{
	asm volatile(	"TST LR, #4 \n\r"		// ; Test bit 2 of EXC_RETURN
					"ITE EQ \n\r"
					"MRSEQ R0, MSP \n\r"	// ; if 0, stacking used MSP, copy to R0
					"MRSNE R0, PSP \n\r"	// ; if 1, stacking used PSP, copy to R0
					"B %[SVC_Handler_C] \n\r"
					".ALIGN 4 \n\r"
					:
					: [SVC_Handler_C] "i" (SVC_Handler_C)
					: "r0"									);
}

void
SVC_Handler_C(Rui32* svc_args)
{
	unsigned long svc_serviceID = ((char*)svc_args[6])[-2]; // *((uint8_t *)svc_args);		// WTF ?

	switch(svc_serviceID)
	{
		case SVC_SERVICE_STARTFRISTTASK:
			__asm volatile(

					"LDR R0, FIRSTTASK_	\n\r"
					"LDR R0, [R0]		\n\r"
					"LDR R0, [R0, #20]	\n\r"

					//----------

					"LDMIA R0!,{R4-R11} \n\r"

					"MSR PSP, R0		\n\r"

					"isb			\n\r"
					"BX LR			\n\r"
					".ALIGN 4		\n\r"

					" FIRSTTASK_		: .word CurrentTaskRunning \n\r"
			);
			break;

		case 1:
			Port_YIELD;
			asm("nop");
			break;

		default:
			break;
	}
}

/**
  * @brief  This exception handling the contexts switching
  */
static void
CleanTOPofStack()
{
	__asm volatile(

			// Lecture de l'adresse de fin de RAM dans R1 soit ENDOFRAMVALUE_
			// Adresse situee dans les 4 premiers octets (premier mot) du tableau de vecteur d'interruption
			"LDR R0, =0x08000000	\n\r"
			"LDR R0, [R0]			\n\r"

			// On fait pointer MSP sur la fin de la stack afin de detruire l'impact qu'il y avait sur le main
			"MSR	MSP, R0		\n\r"

			// Activation des IRQ et vidage du cache
			"cpsie		i		\n\r"
			"cpsie		f		\n\r"
			"dsb				\n\r"
			"isb				\n\r"

	);
}

/**
  * @brief  This exception handling the contexts switching
  */
__attribute__ ( ( isr, naked ) )
void
PendSV_Handler()
{
	__asm volatile (
					// 	LDR R0, <what to load>
					//  STR R0, <where to store>

					//-------------------------
					// Disable core IRQ
					"cpsid I \n\r"

					//"isb \b\r"

					// -------------------------
					// Save current context
					// Get current process stack pointer value
					"MRS R0, PSP 		\n\r"		// Lecture du PSP actuellement utilise
													// Apres le stacking, PSP pointe sur l'adresse la plus basse

					// Manually save R4 to R11 registers in task stack
					"STMDB R0!,{R4-R11} \n\r"

					// Get current task pointer in R1
					"LDR R1, CURRTASK_ 	\n\r"	// R1 pointe sur CurrentTaskRunning

					"LDR R3, [R1]	\n\r"		// R3 prend comme valeur l'adresse pointe par R1

					"ADD R3, R3, #20	\n\r"	// R3 contient sous forme de valeur, l'adresse de task->PSP

					// Save Current Task's Stack pointer value
					// into the corresponding field of Current Task's structure
					"STR R0, [R3]	\n\r"		// On charge a l'adresse contenu sous forme de valeur dans R3 la valeur contenue dans R0


					// Les deux dernieres lignes sont equivalente a
					// "STR R0, [R3, #20]	\n\r"



					// Load Next Task to run
					"LDR R2, NEXTTASK_		\n\r"
					// Get the value pointed by Next Task
					"LDR R2, [R2] 			\n\r"


					// Write CurrentTask = NextTask
					"STR R2, [R1]			\n\r"


					// -------------------------
					// Load next context
					// Get next task ID

					// Set curr_task = next_task

					// Load PSP value from PSP_array
					"LDR R0, [R2, #20] \n\r"


					// Load R4 to R11 from task stack (8 regs)
					"LDMIA R0!,{R4-R11} \n\r"

					// Set PSP to next task
					"MSR PSP, R0  \n\r"

					//	"isb 	\n\r"

					//-------------------------
					// Enable core IRQ
					"cpsie I \n\r"

					// Resume Task execution
					"BX LR  	\n\r"
					".ALIGN 4 	\n\r"

					" NEXTTASK_		: .word NextTaskToRun \n"
					" CURRTASK_		: .word CurrentTaskRunning \n"

					".ALIGN 4 	\n\r"
		);
}

/***********************************END OF FILE********************************/
