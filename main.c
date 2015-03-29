/*********************************************************************
 * *******************************************************************
 *
 * @author	Duclos Timothe
 *
 * @date	09/12/2014
 *
 * @brief	Main
 *
 *********************************************************************/

/********************************************************************
 * Includes
 */
#include "stm32f4xx.h"
#include "task.h"
#include "timer.h"

#define LED0 (1<<12)
#define LED1 (1<<13)
#define LED2 (1<<14)
#define LED3 (1<<15)

void LED_initialize(void); 	// Initialize LED
void task0(void); 			// Toggle LED0
void task1(void); 			// Toggle LED1
void task2(void); 			// Toggle LED2
void task3(void); 			// Toggle LED3

/** @brief 		Determine la longueur d'un tableau
 *  @arg pTAB	Pointeur sur la premiere case du tableau */
#define __Tab_GetLength(pTAB) \
	sizeof (pTAB) / sizeof *(pTAB)

Task_s* test_Task0;
Task_s* test_Task1;
Task_s* test_Task2;
Task_s* test_Task3;

Timer_s* Timer0;
Timer_s* Timer1;
Timer_s* Timer2;
Timer_s* Timer3;


typedef struct {
	unsigned long led_ID;
	unsigned char inverse;
}Struct_Led_s;

void task_led(void* _Led);

Struct_Led_s Led0 = {
		LED0, 0
};

Struct_Led_s Led1 = {
		LED1, 0
};

Struct_Led_s Led2 = {
		LED2, 0
};

Struct_Led_s Led3 = {
		LED3, 0
};

// -------------------------------------------------------------
// Start of main program
int
main()
{
	SystemInit();
	SystemCoreClockUpdate();

	LED_initialize();

//	test_Task0 = Task_Create(128, (unsigned long)task0, NULL);
//	test_Task1 = Task_Create(128, (unsigned long)task1, NULL);
//	test_Task2 = Task_Create(128, (unsigned long)task2, NULL);
//	test_Task3 = Task_Create(128, (unsigned long)task3, NULL);

	Timer0 = Timer_Create(100, (unsigned long)task_led, &Led0, 1);
	Timer1 = Timer_Create(500, (unsigned long)task_led, &Led1, 1);
	Timer2 = Timer_Create(1000, (unsigned long)task_led, &Led2, 1);
	Timer3 = Timer_Create(2000, (unsigned long)task_led, &Led3, 1);

	Timer_Start(Timer0);
	Timer_Start(Timer1);
	Timer_Start(Timer2);
	Timer_Start(Timer3);

	// Last main call, this function should never return
	// But it will if no task can be loaded
	initTimOS();

	// Should not be here
	//stop_cpu;
	while(1);
}

void
task_led(void* _Led)
{
	Struct_Led_s* Led = (Struct_Led_s*)_Led;

	if(Led->inverse) {
		GPIOD->BSRRL = Led->led_ID;
		Led->inverse--;
		return;
	}

	Led->inverse++;
	GPIOD->BSRRH =  Led->led_ID;
}


// ------------------------------------------------------------
void
task0() // Toggle LED #0
{
	while (1) {
		if (getSystickCount() & 0x80) {
			GPIOD->BSRRL = LED0;
		}

		else {
			GPIOD->BSRRH = LED0;
		}
	}
}

// ------------------------------------------------------------
void
task1() // Toggle LED #1
{
	while (1) {
		if (getSystickCount() & 0x100){
			GPIOD->BSRRL = LED1;
		}

		else {
			GPIOD->BSRRH = LED1;
		}
	}
}
// ------------------------------------------------------------
void
task2() // Toggle LED #2
{
	while (1) {
		if (getSystickCount() & 0x200){
			GPIOD->BSRRL = LED2;
		}

		else {
			GPIOD->BSRRH = LED2;
		}
	}
}

// ------------------------------------------------------------
void
task3() // Toggle LED #3
{
	while (1) {
		if (getSystickCount() & 0x400){
			GPIOD->BSRRL = LED3;
		}

		else {
			GPIOD->BSRRH = LED3;
		}
	};
}


// ------------------------------------------------------------
void
LED_initialize()
{
	// Configure LED outputs without any calls
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // Enable Port D clock

	// Set pin 12, 13, 14, 15 as general purpose output mode (pull-push)
	GPIOD->MODER |= (	GPIO_MODER_MODER12_0 |
						GPIO_MODER_MODER13_0 |
						GPIO_MODER_MODER14_0 |
						GPIO_MODER_MODER15_0 ) ;

	// GPIOD->OTYPER j= 0; // No need to change - use pull-push output
	GPIOD->OSPEEDR |= (	GPIO_OSPEEDER_OSPEEDR12 | // 100MHz operations
						GPIO_OSPEEDER_OSPEEDR13 |
						GPIO_OSPEEDER_OSPEEDR14 |
						GPIO_OSPEEDER_OSPEEDR15 );

	GPIOD->PUPDR = 0; // No pull up , no pull down

	return;
}
// ------------------------------------------------------------


