/**
  ******************************************************************************
  * @file      list.h
  * @author    duclos timothe
  * @version   V0.1
  * @date      09/03/2015
  * @brief     This file handle double linked list in circular/linear way
  ******************************************************************************
*/

#ifndef LIST_H
#define LIST_H

/**
 ******************************************************************************
 * Exported macros definition
 *
 */
/**
  * @brief  Define NULL for the linear lists
  */
#ifdef NULL
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
inline void List_add_tail	(void* pNewEntry, void* pHead);
inline void List_add		(void* pNewEntry, void* pHead);
inline void List_del		(void* pDoomedEntry);

inline void ListLinear_add_end	(void* pNewEntry, void* pHead);
inline void ListLinear_add_start(void* pNewEntry, void* pHead);

#endif /** LIST_H */

/***********************************END OF FILE********************************/
