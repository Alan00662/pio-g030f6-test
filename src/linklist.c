/* =============================================================================

    Copyright (c) 2008 Pieter Conradie <http://piconomix.com>
 
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
    
    Title:          list.h : Link List
    Author(s):      Pieter Conradie
    Creation Date:  2008-11-27

============================================================================= */

/* _____STANDARD INCLUDES____________________________________________________ */

/* _____PROJECT INCLUDES_____________________________________________________ */
#include "linklist.h"
#include "driver/usart2.h"
/* _____LOCAL DEFINITIONS____________________________________________________ */

/* _____MACROS_______________________________________________________________ */

/* _____GLOBAL VARIABLES_____________________________________________________ */

/* _____LOCAL VARIABLES______________________________________________________ */

/* _____LOCAL FUNCTION DECLARATIONS__________________________________________ */

/* _____LOCAL FUNCTIONS______________________________________________________ */

/* _____GLOBAL FUNCTIONS_____________________________________________________ */
void list_init(list_t *list,
               size_t max_nr_of_items)
{
    list->first_item      = NULL;
    list->last_item       = NULL;
    list->nr_of_items     = 0;
    list->max_nr_of_items = max_nr_of_items;
}

void list_item_init(list_t      *list,
                    list_item_t *item)
{
    item->previous_item = NULL;
    item->next_item     = NULL;
    item->sensor_buf     = NULL;
}

bool_t list_is_empty(list_t *list)
{
    if(list->first_item == NULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

bool_t list_is_full(list_t *list)
{
    if(list->max_nr_of_items == 0)
    {
        return FALSE;
    }

    if(list->nr_of_items < list->max_nr_of_items)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

size_t list_nr_of_items(list_t *list)
{
    // Return item count
    return (list->nr_of_items);
}

list_item_t *list_first_item(list_t *list)
{
    return list->first_item;
}

list_item_t *list_last_item(list_t *list)
{
    return list->last_item;
}

list_item_t *list_next_item(list_t      *list,
                            list_item_t *item)
{
    return item->next_item;
}

list_item_t *list_previous_item(list_t      *list,
                                list_item_t *item)
{
    return item->previous_item;
}

/**
 * Add an item to the start of the list.
 */
bool_t list_add_to_start(list_t      *list,
                         list_item_t *item)
{
    if(list_is_full(list))
    {
        return FALSE;
    }

    if(list_is_empty(list))
    {
        // Add first item
        list->first_item    = item;
        list->last_item     = item;
#if !USE_Double_Circular_Linked_List 
        item->next_item     = NULL;
        item->previous_item = NULL;
#else
        item->next_item     = item;
        item->previous_item = item;
#endif                                
    }
    else
    {
        // Insert new item before first item
        #if !USE_Double_Circular_Linked_List 
        item->previous_item             = NULL;
        #else
        item->previous_item             = list->last_item;
        #endif  
        item->next_item                 = list->first_item;
        list->first_item->previous_item = item;
        list->first_item                = item;
    }

    // Increment item count
    (list->nr_of_items)++;

    return TRUE;
}

/**
 * 
 */
bool_t list_add_to_end(list_t      *list,
                       list_item_t *item)
{
    if(list_is_full(list))
    {
        // debug_tx2("list is full\r\n");
        return FALSE;
    }

    if(list_is_empty(list))
    {
        // Add first item
        list->first_item    = item;
        list->last_item     = item;
#if !USE_Double_Circular_Linked_List 
        item->next_item     = NULL;
        item->previous_item = NULL;
#else
        item->next_item     = item;
        item->previous_item = item;
        // debug_tx2("list is empty\r\n");
#endif  
    }
    else
    {
        // debug_tx2("Append new item to last item\r\n");
        // Append new item to last item
        item->previous_item        = list->last_item;
#if !USE_Double_Circular_Linked_List 
        item->next_item            = NULL;
#else
        item->next_item            = list->first_item;
#endif 
        list->last_item->next_item = item;
        list->last_item            = item;
                debug_tx2("Append1\r\n");
    }

    // Increment item count
    (list->nr_of_items)++;
        // debug_tx2("Append2\r\n");
    return TRUE;
}

list_item_t *list_remove_first_item(list_t *list)
{
    list_item_t *item = list->first_item;

    // See if list is empty
    if(list_is_empty(list))
    {
        return NULL;
    }

    // See if there is only one item
    if(list->first_item == list->last_item)
    {
        list->first_item = NULL;
        list->last_item  = NULL;
    }
    else
    {
        // The next item become the first one in the list
        list->first_item          = item->next_item;
#if !USE_Double_Circular_Linked_List        
        item->next_item->previous_item = NULL;
#else
        list->first_item->previous_item = list->last_item;
        list->last_item->next_item = list->first_item;
#endif
    }

    // Clear links of removed item
    item->previous_item = NULL;
    item->next_item     = NULL;

    // Decrement item count
    (list->nr_of_items)--;

    return item;
}

list_item_t *list_remove_last_item(list_t *list)
{
    list_item_t *item = list->last_item;

    // See if list is empty
    if(list_is_empty(list))
    {
        return NULL;
    }

    // See if there is only one item
    if(list->first_item == list->last_item)
    {
        list->first_item = NULL;
        list->last_item  = NULL;
    }
    else
    {
        // The previous item become the last one in the list
        list->last_item                = item->previous_item;
#if !USE_Double_Circular_Linked_List              
        item->previous_item->next_item = NULL;    
#else
        list->last_item->next_item     = list->first_item;
        list->first_item->previous_item = list->last_item;
#endif    
    }

    // Clear links of removed item
    item->previous_item = NULL;
    item->next_item     = NULL;

    // Decrement item count
    (list->nr_of_items)--;

    return item;
}

void list_remove_item(list_t      *list,
                      list_item_t *item)
{
    // Extra sanity check
    if(list_is_empty(list))
    {
        return;
    }

    // See if this is the first item in the list
    if(item == list->first_item)
    {
        list_remove_first_item(list);
        return;
    }

    // See if this is the last item in the list
    if(item == list->last_item)
    {
        list_remove_last_item(list);
        return;
    }

    // Link previous and next item to each other
    item->previous_item->next_item = item->next_item;
    item->next_item->previous_item = item->previous_item;
    
    // Clear links of item
    item->previous_item = NULL;
    item->next_item     = NULL;

    // Decrement item count
    (list->nr_of_items)--;
}

bool_t list_item_in_list(list_t      *list,
                         list_item_t *item)
{
    // Start at first item in the list
    list_item_t *item_in_list = list->first_item;
#if !USE_Double_Circular_Linked_List     
    // Search all items in the list
    while(item_in_list != NULL)
    {
        if(item_in_list == item)
        {
            // Item is in the list
            return TRUE;
        }
        // Next item
        item_in_list = item_in_list->next_item;
    }
#else
    // Search all items in the list
    if (item_in_list != NULL) {
        do {
            if(item_in_list == item)
            {
                // Item is in the list
                return TRUE;
            }
            // Next item
            item_in_list = item_in_list->next_item;
        } while (item_in_list != list->first_item);
    }
#endif
    // Item is not in the list
    return FALSE;
}

