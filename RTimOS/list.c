/**
  ******************************************************************************
  * @file      list.c
  * @author    duclos timothe
  * @version   V0.1
  * @date      08/03/2015
  * @brief     This software handles double linked list (linear & cirular)
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
  * @brief  Insert a new entry after the specified head
  * @param  _NewEntry		Pointer to the new entry to add
  * 		_head			Pointer to the entry after which we'll add the new one
  */
inline void
list_add(	void* NewEntry,
			void* head,
			unsigned long offset)
{
	_list_add(	((list_head_s*)NewEntry),
				((list_head_s*)head),
				((list_head_s*)head)->next	);
}

/**
  * @brief  Insert a new entry before the specified head
  * @param  NewEntry		Pointer to the new entry to add
  * 		head			Pointer to the entry before which we'll add the new one
  */
inline void
list_add_tail(	void* pNewEntry,
				void* pHead,
				unsigned long offset)
{
	_list_add(	((list_head_s*)pNewEntry),
				((list_head_s*)pHead)->prev,
				((list_head_s*)pHead)		);
}

/**
  * @brief  Delete an entry after the specified head
  * @param  _DoomedEntry		The entry to delete from the list
  */
inline void
list_del(	void* pDoomedEntry,
		 	unsigned long offset)
{
	_list_del(	((list_head_s*)pDoomedEntry)->prev,
				((list_head_s*)pDoomedEntry)->next	);
}

/**
	* @brief 	Add an entry at the end of a linear list
	* @param 	NewEntry 	The entry to add to the list
	*			pHead An entry of the list
	*
	* @note 	DO NOT CALL THIS METHOD WITH A CIRCULAR LIST
	*		 	OTHERWISE IT WILL NEVER END !
*/
void
ListLinear_add_end(	void* pNewEntry,
					void* _pHead,
					unsigned long offset)
{
	// Search for list end
	list_head_s* pLastEntry = (list_head_s *)_pHead;
	while( pLastEntry->next ) {
		pLastEntry = ((list_head_s *)pLastEntry)->next;
	}

	// Add the entry
	_list_add( 	((list_head_s*)pNewEntry),
				((list_head_s*)pLastEntry),
				((list_head_s*)pLastEntry)->next );
}

/**
	* @brief	Add an entry to the start of a linear list
	* @param	NewEntry 	The entry to add to the list
	* 			pHead 		An entry of the list
	*
	* @note 	DO NOT CALL THIS METHOD WITH A CIRCULAR LIST
	* 			OTHERWISE IT WILL NEVER END !
*/
void
ListLinear_add_start( 	void* const pNewEntry,
						void* const _pHead,
						unsigned long offset	)
{
	// Search for list start
	list_head_s* pFirstEntry = (list_head_s *)_pHead;
	while( pFirstEntry->prev ) {
		pFirstEntry = ((list_head_s *)pFirstEntry)->prev;
	}

	// Add the entry
	_list_add( 	((list_head_s*)pNewEntry),
				((list_head_s*)pFirstEntry)->prev,
				((list_head_s*)pFirstEntry)	);
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




/**
   ******************************************************************************
 * Validation
 *
 */
#if VALIDATION_LIST_LINEAIRE

	unsigned char
	VALIDATION_list_lineaire()
	{
		list_head_s list1, list2, list3, list4, list5, list0;

		LISTLINEAR_HEAD_INIT(&list1);
		LISTLINEAR_HEAD_INIT(&list2);
		LISTLINEAR_HEAD_INIT(&list3);
		LISTLINEAR_HEAD_INIT(&list4);
		LISTLINEAR_HEAD_INIT(&list0);
		LISTLINEAR_HEAD_INIT(&list5);

		list_add			(&list3, &list1, 0);
		list_add_tail		(&list2, &list3, 0);
		list_add			(&list4, &list3, 0);
		ListLinear_add_start(&list0, &list4, 0);
		ListLinear_add_end	(&list5, &list0, 0);

		return(		list0.prev == NULL
				&&	list0.next == &list1
				&&	list1.prev == &list0
				&&	list1.next == &list2
				&&	list2.prev == &list1
				&&	list2.next == &list3

				&&	list3.next == &list4
				&&	list3.prev == &list2
				&&	list4.next == &list5
				&&	list4.prev == &list3
				&&	list5.next == NULL
				&&	list5.prev == &list4	);
	}

#endif /** VALIDATION_LIST_LINEAIRE */

/***********************************END OF FILE********************************/
