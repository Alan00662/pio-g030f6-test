#ifndef __LIST_H__
#define __LIST_H__

/**
 * 0: 双向单链表
 * 1: 双向循环链表
 */
#define USE_Double_Circular_Linked_List   1

#define bool_t _Bool
#define TRUE 1
#define FALSE 0
/** 
    @ingroup UTILS
    @defgroup LINKED_LIST list.h : Linked List

    A double (forward and backward) linked list using pointers.
    
    File(s):
    - utils/list.h
    - utils/list.c
    
    @see http://en.wikipedia.org/wiki/Linked_list

    Example:
 
        @code
        No items in list:
    
        [first = NULL] [last = NULL]
    
        One item in list:
    
        [first] -> [prev = NULL][next = NULL] <- [last]
    
        Two items in list:
    
        [first] -> [prev = NULL][next] <-> [prev][next = NULL]  <- [last]
    
        Three items in list:
    
        [first] -> [prev = NULL][next] <-> [prev][next] <-> [prev][next = NULL]  <- [last]
        @endcode
 */
/// @{

/* _____STANDARD INCLUDES____________________________________________________ */
#include <stddef.h>
#include "main.h"
/* _____PROJECT INCLUDES_____________________________________________________ */

#ifdef __cplusplus
extern "C" {
#endif
/* _____DEFINITIONS _________________________________________________________ */

/* _____TYPE DEFINITIONS_____________________________________________________ */
/// Link structure that must be at the head of each item in the list
typedef struct list_item_s
{
    struct list_item_s *next_item;
    struct list_item_s *previous_item;
    uint8_t sensor_id; 
    uint8_t *sensor_buf;
    uint8_t buf_len;
    uint16_t exit_cnt;
} list_item_t;

/// Linked list structure
typedef struct
{
    struct list_item_s *first_item;        ///< Pointer to first item in the list
    struct list_item_s *last_item;         ///< Pointer to last item in the list
    size_t              nr_of_items;       ///< Counter for number of items in the list
    size_t              max_nr_of_items;   ///< Maximum number of items allowed in list; 0 means no limit
} list_t;

/* _____GLOBAL VARIABLES_____________________________________________________ */

/* _____GLOBAL FUNCTION DECLARATIONS_________________________________________ */
/** 
   Initialises a linked list structure

   @param list              Pointer to the linked list
   @param max_nr_of_items   Maximum number of items allowed in list; 0 means no limit
 */
void list_init(list_t *list,
               size_t max_nr_of_items);

/** 
   Initialises a list item.

   Initialises the item structure to indicate that it is not in the list.
   @sa list_item_in_list()

   @param list      Pointer to the linked list
   @param item      Pointer to specified item
 */
void list_item_init(list_t      *list,
                    list_item_t *item);

/** 
   See if the list is empty

   @param list      Pointer to the linked list

   @return TRUE     List is empty 
   @return FALSE    List contains one or more items
 */
bool_t list_is_empty(list_t *list);

/** 
   See if the list is full

   @param list      Pointer to the linked list

   @retval TRUE     The list is full
   @retval FALSE    The list is not full, or there is no limit (max_nr_of_items = 0)
 */
bool_t list_is_full(list_t *list);

/** 
   Get the number of items in the list

    @param list         Pointer to the linked list

    @returns size_t     The number of items in the list 
 */
size_t list_nr_of_items(list_t *list);

/** 
   Get a pointer to the first item in the list

   @param list              Pointer to the linked list

   @returns list_item_t*    Pointer to the first item in the list;
                            NULL will be returned if the list is empty.
 */
list_item_t *list_first_item(list_t *list);

/** 
   Get a pointer to the last item in the list

   @param list              Pointer to the linked list

   @returns list_item_t*    Pointer to the last item in the list;
                            NULL will be returned if the list is empty.
 */
list_item_t *list_last_item(list_t *list);

/** 
   Get a pointer to the next item in the list (after the specified item).

   @param list              Pointer to the linked list 
   @param item              Current item

   @returns list_item_t*    Pointer to the next item in the list;
                            NULL will be returned if the specified item is the
                            last one in the list.
 */
list_item_t *list_next_item(list_t      *list,
                            list_item_t *item);

/** 
   Get a pointer to the previous item in the list (before the specified item).

   @param list              Pointer to the linked list 
   @param item              Current item 

   @returns list_item_t*    Pointer to the next item in the list;
                            NULL will be returned if the specified item is the
                            first one in the list.
 */
list_item_t *list_previous_item(list_t      *list,
                                list_item_t *item);

/** 
   Insert item to the start of the list.

   @param list      Pointer to the linked list
   @param item      Item to be inserted

   @retval TRUE     Item has been inserted
   @retval FALSE    List is full
 */
bool_t list_add_to_start(list_t      *list,
                         list_item_t *item);

/** 
   Add item to the end of the list.

   @param list      Pointer to the linked list
   @param item      Item to be inserted

   @retval TRUE     Item has been inserted
   @retval FALSE    List is full
 */
bool_t list_add_to_end(list_t      *list,
                       list_item_t *item);

/** 
   Remove first item from the list

   @param list      Pointer to the linked list

   @return list_item_t* Pointer to the (old) first item;
                        NULL will be returned if the list is empty.
 */
list_item_t *list_remove_first_item(list_t *list);

/** 
   Remove last item from the list

   @param list      Pointer to the linked list

   @return list_item_t* Pointer to the (old) last item;
                        NULL will be returned if the list is empty.
 */
list_item_t *list_remove_last_item(list_t *list);

/** 
   Remove item from the list

   @param list      Pointer to the linked list
   @param item      Item to be removed from the list

 */
void list_remove_item(list_t      *list,
                      list_item_t *item);

/** 
   See if item is in the list

   @param list      Pointer to the linked list
   @param item      Pointer to specified item

   @retval TRUE     Item is in the list
   @retval FALSE    Item is not in the list
 */
bool_t list_item_in_list(list_t      *list,
                         list_item_t *item);

/* _____MACROS_______________________________________________________________ */

/// @}
#ifdef __cplusplus
}
#endif

#endif // #ifndef __LIST_H__
