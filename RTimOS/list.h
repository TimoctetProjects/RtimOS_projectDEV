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
	#undef 	NULL
#endif /** NULL */
#ifndef NULL
	#define NULL		((void *)0)
#endif /** NULL */

/**
  * @brief  Initialize the list_head struct of a object in a circular way
  * @param  lIST	Object's adress with the list head to initialize
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
  * @brief  Initialize the list_head struct of a object in a linear way
  * @param  lIST	Object's adress with the list head to initialize
  */
#define LISTLINEAR_HEAD_INIT(lIST) \
	*((list_head_s*)(lIST)) = (list_head_s) { NULL, NULL }

/**
 ******************************************************************************
 * Exported type definition
 *
 */
/**
  * @brief  Structure of a list head, to put first in a object's struct using it
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
/** For both Double circular & linear linked lists*/
inline void list_add_tail	(void* pNewEntry, void* pHead);
inline void list_add		(void* pNewEntry, void* pHead);
inline void list_del		(void* pDoomedEntry);

/** For both Double circular & linear linked lists*/
void ListLinear_add_end		(void* pNewEntry, void* _pHead);
void ListLinear_add_start	(void* const pNewEntry, void* const _pHead);

inline void ListLinear_add_end	(void* pNewEntry, void* pHead);
inline void ListLinear_add_start(void* pNewEntry, void* pHead);

#endif /** LIST_H */

/***********************************END OF FILE********************************/
