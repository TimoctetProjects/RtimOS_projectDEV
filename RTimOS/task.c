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

/**
 ******************************************************************************
 * Private macros definition
 *
 */
#define OFFSETSTACK_XPCR				4
#define OFFSETSTACK_TASKFUNCTION		4

#define	TASK_LISTHEAD_OFFSET_byte	__GetOffset_byte(Task_s, list)

/**
 ******************************************************************************
 * Private type definition
 *
 */
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
void
PendSV_Handler(void)
__attribute__ ( ( isr, naked ) );

void
SVC_Handler(void)
__attribute__ ( ( isr, naked ) );

static void
Task_GetNextTask();

static unsigned long*
Task_StackInit(	Task_s*			_Task,
				unsigned long 	StackSize,
				unsigned long 	_ptr_TaskFunction,
				void*		  	_TaskArg );

static void CleanTOPofStack();
static void Task_Exit();


/**
 ******************************************************************************
 * Private variables definition
 *
 */
static Task_s*			CurrentTaskRunning;
static Task_s*			NextTaskToRun;
static unsigned long 	SystickCount;

typedef void (*pFunction)(void);

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
			resultat &= VALIDATION_list_linear();
		#endif /** VALIDATION_LIST_LINEAIRE */

		#if VALIDATION_LIST_CIRULAIR
			resultat &= VALIDATION_list_circular();
		#endif /** VALIDATION_LIST_LINEAIRE */

			asm("nop");

	#endif /** IS_VALIDATION */

	SysTick_Config(SystemCoreClock/1000); 		// 1ms SysTick interrupt on 168MHz

	// Return FAIL if nos task can be loaded
	if(!CurrentTaskRunning)
		return FAIL;

	CleanTOPofStack();

	//__set_PSP(((unsigned long)(CurrentTaskRunning->pStack) + 128*4));
	NVIC_SetPriority(PendSV_IRQn, 0xFF); 		// Set PendSV to lowest	possible priority
	__set_CONTROL(0x3); 						// Switch to use Process Stack, in unprivileged
												// state by setting Control Register

	__ISB(); 									// Flush processor cache

	//LoadFirstTaskContext();
	__asm volatile("svc 0 \n\r");
	//((pFunction)(((Stack_Frame_HW_s *)(CurrentTaskRunning->PSP_value + sizeof(Stack_Frame_SW_s)))->pc))();

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
Task_Create(	unsigned long 		StackSize,
				unsigned long		_ptr_TaskFunction,
				void*				_ptr_Param)
{
	static uint8_t FirstTask = 0;

	//----------------- Allocate memomry for Task
	Task_s* pNewTask = (Task_s*) malloc(sizeof(Task_s));
	if(pNewTask == NULL)
		return NULL;
	mem_ClearZone(pNewTask, sizeof(Task_s));

	//------------------ Allocate memory for stack
	pNewTask->pStack 	= Task_StackInit(pNewTask, StackSize, _ptr_TaskFunction, _ptr_Param);
	if(pNewTask->pStack == NULL) {
		free(pNewTask);
		return NULL;
	}

	LISTCIRCULAR_HEAD_INIT(pNewTask);

	//----------------- Prepare struct fields in task's handle
	pNewTask->StackSize = StackSize;
	pNewTask->State		= TaskReady;

	if(!FirstTask) {
		CurrentTaskRunning = pNewTask;
		FirstTask++;
	} else
		list_add(pNewTask, CurrentTaskRunning);

	return pNewTask;
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
static unsigned long*
Task_StackInit(	Task_s*			_Task,
				unsigned long	StackSize,
				unsigned long 	_ptr_TaskFunction,
				void*		  	_TaskArg )
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

	unsigned long* 		pStack_init  = NULL;
	Stack_Frame_HW_s* 	ProcessStack = NULL;

	//------------------ Allocate memory for stack
	unsigned long* pStack = (unsigned long*) malloc(StackSize * sizeof(unsigned long));
	if(pStack == NULL)
		return NULL;
	mem_ClearZone(pStack, StackSize * sizeof(unsigned long));

	//------------------ Initialize start of stack
	// In order to detect stack overflow
	for(pStack_init = pStack; pStack_init<pStack+4; pStack_init++)
		*pStack_init = 0x5A5A5A5A;

	//------------------ Get end of Stack pointer
	// Make it point to R4
	// just like if it had already been called
	// Because the first thing we'll do is to unstack R4-R11
	_Task->PSP_value = 		((unsigned long) pStack)
							+ 	StackSize*sizeof(unsigned long)
							- (sizeof(Stack_Frame_HW_s)
							+ sizeof(Stack_Frame_SW_s));


	//------------------ Align on PSP adress like if it were a Stack_Frame_HW_s struct
	// This is done in order to simply acess stack region
	ProcessStack = (Stack_Frame_HW_s *) (_Task->PSP_value + sizeof(Stack_Frame_SW_s));

	//------------------ Initialize task argument (not tested)
	ProcessStack->r0 = (unsigned int) _TaskArg;

	//------------------ Initialize task function adress
	// To the before last byte in task stack
	ProcessStack->pc = (unsigned long) _ptr_TaskFunction;

	//----------------- Initialize return adress
	// If the task returns it will enter an empy infinity loop
	ProcessStack->lr = (unsigned long) &Task_Exit;

	//------------------ Initialize xPCR
	// it's the last byte in task stack
	// Allow to enable Thumb mode (T bit to 1)
	ProcessStack->psr = 0x01000000;

	return pStack;
}

/**
  * @brief  Get the next task to run
  */
static void
Task_GetNextTask()
{
	// If there is more than one task, no need to switch context
	if(CurrentTaskRunning != (Task_s*)CurrentTaskRunning->list.next) {

		NextTaskToRun 	= List_GetNext(Task_s, CurrentTaskRunning);
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
		__ISB();
	}
}

/**
  * @brief  Read the current systick count
  * @retval Number of systicktick irq that occured
  */
inline unsigned long
getSystickCount()
{
	return SystickCount;
}

/**
  * @brief  Systick IRQ
  * 		This interruption will count time
  * 		& provoque a context switch
  */
void
SysTick_Handler()
{
	SystickCount++;
	Timer_Tick();
	Task_GetNextTask();
}


void
Task_Exit()
{
	for(;;);
}

void
SVC_Handler()
{
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
