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
//#include <stdint.h>
/** Types */

//#if __STDINT_EXP(LONG_MAX) == 0x7fffffff && !defined(__SPU__)
//	typedef unsigned int	Ru32_t;
//#endif


typedef unsigned short	Ru16_t;
typedef unsigned char	Ru8_t;
typedef signed int		Ri32_t;
typedef signed short	Ri16_t;
typedef signed char		Ri8_t;


/***********************************END OF FILE*******************************/
