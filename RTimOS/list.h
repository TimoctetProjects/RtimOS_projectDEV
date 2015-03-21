/**
  ******************************************************************************
  * @file      list.h
  * @author    duclos timothe
  * @version   V0.1
  * @date      09/03/2015
  * @brief     Gestion de listes doublement chainees
  ******************************************************************************
*/

#ifndef LIST_H
#define LIST_H

/**
 ******************************************************************************
 * Exported macros definition
 *
 */
#define LIST_HEAD_INIT(lIST) \
	*((list_head_s*)(lIST)) = (list_head_s) { (list_head_s*)(lIST), (list_head_s*)(lIST) }

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
inline void list_add_tail	(void* NewEntry, void* head);
inline void list_add		(void* NewEntry, void* head);
inline void list_del		(void* DoomedEntry);


#endif /** LIST_H */

/***********************************END OF FILE********************************/
