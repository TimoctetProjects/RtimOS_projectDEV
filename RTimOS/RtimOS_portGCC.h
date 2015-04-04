/**
  ******************************************************************************
  * @file      RtimOS_portGCC.h
  * @author    duclos timothe
  * @version   V0.1
  * @date      03/04/2015
  * @brief     Portage pour GCC
  ******************************************************************************
*/

#ifndef RTIMOS_PORTGCC_H
#define RTIMOS_PORTGCC_h




#if defined ( __GNUC__ )

	#define _GiveAttribute(a, args...) 	__attribute__((a, ## args))
	#define _NAKED 						__attribute__((naked))

#endif	/** __GNUC__ */





#endif/** RTIMOS_PORTGCC_h */

/***********************************END OF FILE*******************************/
