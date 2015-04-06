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

/** @brief 		Determine la longueur d'un tableau
 *  @arg pTAB	Pointeur sur la premiere case du tableau */
#define __Tab_GetLength(pTAB) \
	sizeof (pTAB) / sizeof *(pTAB)

typedef struct {
	unsigned long led_ID;
	unsigned char inverse;
	unsigned long SystickCount;
}Struct_Led_s;

static Task_s*  TestTask[4];
static Timer_s* TestTimer[4];

void LED_initialize(void); 	// Initialize LED
void TimerCallback_led(void* _Led);
void task(void* _Led);

Struct_Led_s Leds[4] = {
		{LED0, 0, 100},
		{LED1, 0, 500},
		{LED2, 0, 1000},
		{LED3, 0, 1500},
};



// -------------------------------------------------------------
// Start of main program
int
main()
{
	SystemInit();
	SystemCoreClockUpdate();

	LED_initialize();

	TestTask[0] = Task_Create(128, (unsigned long)task, &Leds[0]);
	TestTask[1] = Task_Create(128, (unsigned long)task, &Leds[1]);
	TestTask[2] = Task_Create(128, (unsigned long)task, &Leds[2]);
	TestTask[3] = Task_Create(128, (unsigned long)task, &Leds[3]);

//	TestTimer[0] = Timer_Create(100, (unsigned long)TimerCallback_led, &Leds[0], 1);
//	TestTimer[1] = Timer_Create(500, (unsigned long)TimerCallback_led, &Leds[1], 1);
//	TestTimer[2] = Timer_Create(1000,(unsigned long)TimerCallback_led, &Leds[2], 1);
//	TestTimer[3] = Timer_Create(2000,(unsigned long)TimerCallback_led, &Leds[3], 1);
//
//	Timer_Start(TestTimer[0]);
//	Timer_Start(TestTimer[1]);
//	Timer_Start(TestTimer[2]);
//	Timer_Start(TestTimer[3]);

	// Last main call, this function should never return
	// But it will if no task can be loaded
	initTimOS();

	// Should not be here
	//stop_cpu;
	while(1);
}


// ------------------------------------------------------------
void
TimerCallback_led(void* _Led)
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
task(void* _Led) // Toggle LED #0
{
	Struct_Led_s* Led = (Struct_Led_s*)_Led;

	while (1) {

		if (Led->inverse) {
			GPIOD->BSRRL = Led->led_ID;
			Led->inverse--;
		}

		else {
			GPIOD->BSRRH = Led->led_ID;
			Led->inverse++;
		}

		Task_Delay(Led->SystickCount);
	}
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
