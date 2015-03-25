/**
  ******************************************************************************
  * @file      RtimOS.h
  * @author    duclos timothe
  * @version   V0.1
  * @date      07/03/2015
  * @brief     Gestion de l'OS
  ******************************************************************************
*/

#ifndef RTIMOS_H
#define RTIMOS_H

/**
   ******************************************************************************
 * Include
 *
 */


/**
   ******************************************************************************
 * Exported Maccro
 *
 */
/**
* @brief This section define NULL
*/
#ifdef NULL
	#undef NULL
#endif /** NULL */
#ifndef NULL
	#define NULL	((void *)0)
#endif /** NULL */

/**
   ******************************************************************************
 * This section handles validations
 *
 */
/** @brief Validating ALL */
#define VALDIATION					1


/** @brief Linked list */
#define VALIDATION_LIST				1 | VALDIATION
#define VALIDATION_LIST_LINEAIRE	0 | VALIDATION_LIST
#define VALIDATION_LIST_CIRULAIR	0 | VALIDATION_LIST


#endif /** RTIMOS_H */
/***********************************END OF FILE*******************************/
