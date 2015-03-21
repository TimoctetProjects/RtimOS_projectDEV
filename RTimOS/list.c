/**
  ******************************************************************************
  * @file      list.c
  * @author    duclos timothe
  * @version   V0.1
  * @date      08/03/2015
  * @brief     Gestion de liste doublement chainee
  ******************************************************************************
*/

/**
   ******************************************************************************
 * Include
 *
 */
#include "list.h"

/**
   ******************************************************************************
 * Private function prototype
 *
 */
static inline void _list_add(list_head_s* new, list_head_s* prev, list_head_s* next);
static inline void _list_del(list_head_s* prev,list_head_s* next);


/**
   ******************************************************************************
 * Exported function prototype
 *
 */
/**
  * @brief  Insert a new entry before the specified head
  * @param  NewEntry		Pointer to the new entry to add
  * 		head			Pointer to the entry before which we'll add the new one
  */
inline void
list_add_tail(	void* NewEntry,
				void* head)
{
	_list_add(	((list_head_s*)NewEntry),
				((list_head_s*)head)->prev,
				((list_head_s*)head)		);
}

/**
  * @brief  Insert a new entry after the specified head
  * @param  _NewEntry		Pointer to the new entry to add
  * 		_head			Pointer to the entry after which we'll add the new one
  */
inline void
list_add(	void* NewEntry,
			void* head)
{
	_list_add(	((list_head_s*)NewEntry),
				((list_head_s*)head),
				((list_head_s*)head)->next	);
}

/**
  * @brief  Delete an entry after the specified head
  * @param  _DoomedEntry		The entry to delete from the list
  */
inline void
list_del(void* DoomedEntry)
{
	_list_del(	((list_head_s*)DoomedEntry)->prev,
				((list_head_s*)DoomedEntry)->next	);
}

/**
   ******************************************************************************
 * Private function definition
 *
 */
/**
 * @brief add an entry between two others
 */
static inline void
_list_add(	list_head_s* new,
			list_head_s* prev,
			list_head_s* next)
{
	 next->prev = new;
	 new->next = next;
	 new->prev = prev;
	 prev->next = new;
}

/**
 * @brief Delete a list entry by making its prev/next point to each other
 */
static inline void
_list_del(	list_head_s* prev,
			list_head_s* next)
{
	next->prev = prev;
	prev->next = next;
}

/***********************************END OF FILE********************************/
