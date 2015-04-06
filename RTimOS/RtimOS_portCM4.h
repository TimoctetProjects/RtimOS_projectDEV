/**
  ******************************************************************************
  * @file      RtimOS_portCM4.h
  * @author    duclos timothe
  * @version   V0.1
  * @date      03/04/2015
  * @brief     Portage pour cortex M4
  ******************************************************************************
*/

#ifndef RTIMOS_PORTCM4_H
#define RTIMOS_PORTCM4_H

#include "RtimOS_portGCC.h"


#define Rui32		unsigned long
#define Rui16		unsigned short
#define Rui8		unsigned char

#define Ri32		signed long
#define Ri16		signed short
#define Ri8			signed char


__attribute__ ( (always_inline) )
static inline void
Port_Disable_irq()
{
	asm volatile ("cpsid i");
}

__attribute__ ( (always_inline) )
static inline void
Port_Enable_irq()
{
	asm volatile ("cpsie i");
}

#define Port_YIELD		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk

#define Port_Systick_IRQ 		SysTick_Handler
#define Port_PendSV				PendSV_Handler
#define Port_SVC				SVC_Handler



#endif /** RTIMOS_PORTCM4_H */

/***********************************END OF FILE*******************************/
