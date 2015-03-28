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
 * Exported Types
 *
 */
typedef enum {
	FAIL = 0,
	PASS = !FAIL
}ResultTest_e;

typedef enum {
	STATUS_FINIS = 0,
	STATUS_ENCOURS = !STATUS_FINIS
}Status_e;

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
* @brief Read the offset in byte of a struct's field
*/
#define __GetOffset_byte(type, field)	\
	((unsigned long)&(((type*)0)->field))

/**
   ******************************************************************************
 * This section handles validations
 *
 */
/** @brief Validating ALL */
#define VALDIATION					0


/** @brief Linked list */
#define VALIDATION_LIST				0 | VALDIATION
#define VALIDATION_LIST_LINEAIRE	0 | VALIDATION_LIST
#define VALIDATION_LIST_CIRULAIR	0 | VALIDATION_LIST



#define IS_VALIDATION	\
	VALIDATION_LIST_CIRULAIR | VALIDATION_LIST_LINEAIRE



#endif /** RTIMOS_H */

/***********************************END OF FILE*******************************/
