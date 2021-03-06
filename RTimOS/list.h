/**
  ******************************************************************************
  * @file      list.h
  * @author    duclos timothe
  * @version   V0.1
  * @date      09/03/2015
  * @brief     This software handles double linked list (linear & cirular)
  ******************************************************************************
*/

#ifndef LIST_H
#define LIST_H

/**
 ******************************************************************************
 * Include
 *
 */
#include "RtimOS.h"

/**
 ******************************************************************************
 * Exported macros definition
 *
 */
/**
	* @brief 	Initialize the list_head struct of a object in a circular way
	* @param 	lIST 	Object's adress with the list head to initialize
*/
#define LISTCIRCULAR_HEAD_INIT(lIST) \
	*((list_head_s*)(lIST)) = (list_head_s) { (list_head_s*)(lIST), (list_head_s*)(lIST) }


/**
	* @brief 	Initialize the list_head struct of a object in a linear way
	* @param 	lIST 	Object's adress with the list head to initialize
*/
#define LISTLINEAR_HEAD_INIT(lIST) \
		*((list_head_s*)(lIST)) = (list_head_s) { NULL, NULL }


/**
	* @brief 	Get the nex element
	* @param 	tYPE	Type that contains the list struct head
	* 			dATA	Variable's adress declared as tYPE
*/
#define List_GetNext(tYPE, dATA) \
		((tYPE *)(((list_head_s*)dATA)->next))

#define List_GetPrev(tYPE, dATA) \
		((tYPE *)(((list_head_s*)dATA)->prev))

// other version
/*#define List_GetNext(tYPE, lIST, dATA) \
		((tYPE *)(dATA)->lIST.next)*/



/**
 ******************************************************************************
 * Exported type definition
 *
 */
typedef struct _list_head_s {
	struct _list_head_s* next;
	struct _list_head_s* prev;
}list_head_s;

/**
 ******************************************************************************
 * Exported function prototype
 *
 */
/** For both circular & linear linked lists */
inline unsigned char List_IsEmpty	(void* pHead_Circular);
inline void list_add_tail	(void* pNewEntry, void* pHead);
inline void list_add		(void* pNewEntry, void* pHead);
inline void list_del		(void* pDoomedEntry);


/** For linear list */
inline unsigned char ListLinear_IsEmpty		(void* pHead_Linear);
void ListLinear_add_end		(void* pNewEntry, void* _pHead);
void ListLinear_add_start	(void* const pNewEntry, void* const _pHead);



/** Validation */
#if VALIDATION_LIST_LINEAIRE
	unsigned char 	VALIDATION_list_linear_add();
	unsigned char	VALIDATION_list_linear_del();
#endif /** VALIDATION_LIST_LINEAIRE */
#if VALIDATION_LIST_CIRULAIR
	unsigned char 	VALIDATION_list_circular_add();
	unsigned char 	VALIDATION_list_circular_del();
#endif /** VALIDATION_LIST_LINEAIRE */

#endif /** LIST_H */

/***********************************END OF FILE********************************/
