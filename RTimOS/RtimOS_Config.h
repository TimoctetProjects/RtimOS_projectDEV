/**
  ******************************************************************************
  * @file      task.c
  * @author    duclos timothe
  * @version   V0.1
  * @date      08/03/2015
  * @brief     Configuration de l'OS
  ******************************************************************************
*/

#define NB_TASK_MAX			32

#define HEAPSIZE_bytes		4096


// Include GCC limits
#include <limits.h>

/** Types (unused as of yet) */
#if LONG_MAX == 0x7fffffff
	typedef unsigned long	Ru32_t;
	typedef unsigned short	Ru16_t;
	typedef unsigned char	Ru8_t;
	typedef signed long		Ri32_t;
	typedef signed short	Ri16_t;
	typedef signed char		Ri8_t;
#endif





/***********************************END OF FILE*******************************/
