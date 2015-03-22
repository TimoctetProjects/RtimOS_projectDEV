/**
  ******************************************************************************
  * @file      list.c
  * @author    duclos timothe
  * @version   V0.1
  * @date      08/03/2015
  * @brief     This file handle double linked list in circular/linear way
  ******************************************************************************
*/
/**
 * TODO: Gestion des const dans les prototypes
 * TODO: Test liste linear avec methodes actuel
 * TODO: Test methodes linear specifiques avec ajout a la fin et au depart
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
static inline void _List_add(list_head_s* new, list_head_s* prev, list_head_s* next);
static inline void _List_del(list_head_s* prev,list_head_s* next);


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
List_add_tail(	void* pNewEntry,
				void* pHead)
{
	_List_add(	((list_head_s*)pNewEntry),
				((list_head_s*)pHead)->prev,
				((list_head_s*)pHead)		);
}

/**
  * @brief  Insert a new entry after the specified head
  * @param  _NewEntry		Pointer to the new entry to add
  * 		_head			Pointer to the entry after which we'll add the new one
  */
inline void
List_add(	void* pNewEntry,
			void* pHead)
{
	_List_add(	((list_head_s*)pNewEntry),
				((list_head_s*)pHead),
				((list_head_s*)pHead)->next	);
}

/**
  * @brief  Delete an entry after the specified head
  * @param  _DoomedEntry	The entry to delete from the list
  */
inline void
List_del(void* pDoomedEntry)
{
	_List_del(	((list_head_s*)pDoomedEntry)->prev,
				((list_head_s*)pDoomedEntry)->next	);
}




/**
  * @brief  Add an entry at the end of a linear list
  * @param  NewEntry		The entry to add to the list
  * 		pHead			An entry of the list
  *
  * @note	DO NOT CALL THIS METHOD WITH A CIRCULAR LIST
  * 		OTHERWISE IT WILL NEVER END !
  * 		THINK BEFORE YOU CALL !
  */
inline void
ListLinear_add_end(	void* pNewEntry,
					void* _pHead)
{
	// Search for list end
	list_head_s* pLastEntry = (list_head_s *)_pHead;
	while( pLastEntry->next ) {
		pLastEntry = ((list_head_s *)pLastEntry)->next;
	}

	// Add the entry
	_List_add_end(	((list_head_s*)pNewEntry),
					((list_head_s*)pLastEntry)	);
	// pLastEntry->next = pNewEntry
	// pNewEntry->next = NULL
	// pNewEntry->prev = pLastEntry
	// pLastEntry->prev conserve ?
}

/**
  * @brief  Add an entry after the start of a linear list
  * @param  NewEntry		The entry to add to the list
  * 		pHead			An entry of the list
  *
  * @note	DO NOT CALL THIS METHOD WITH A CIRCULAR LIST
  * 		OTHERWISE IT WILL NEVER END !
  * 		THINK BEFORE YOU CALL !
  */
inline void
ListLinear_add_start(	void* pNewEntry,
						void* _pHead)
{
	// Search for list start
	list_head_s* pFirstEntry = (list_head_s *)_pHead;
	while( pFirstEntry->prev ) {
		pFirstEntry = ((list_head_s *)pFirstEntry)->prev;
	}

	// Add the entry
	_List_add_start(	((list_head_s*)pNewEntry),
						((list_head_s*)pFirstEntry)	);
	// pNewEntry->next = pFirstEntry->next
	// pFirstEntry->next = pNewEntry
	// pNewEntry->prev = pFirstEntry
	// pFirstEntry->prev conserve ?
}

/**
   ******************************************************************************
 * Private function definition
 *
 */
/**
 * @brief add an entry in a circular/linear list between two specified others
 */
static inline void
_List_add(	list_head_s* new,
			list_head_s* prev,
			list_head_s* next)
{
	 next->prev = new;
	 new->next  = next;
	 new->prev  = prev;
	 prev->next = new;
}

/**
 * @brief 	Delete a list entry in a circular/linear list
 * 			by making its prev/next point to each other
 */
static inline void
_List_del(	list_head_s* prev,
			list_head_s* next)
{
	next->prev = prev;
	prev->next = next;
}



/***********************************END OF FILE********************************/
