#pragma once

#include <stdlib.h>
#include <assert.h>

#ifndef LIST_ASSERT
	#define LIST_ASSERT assert
#endif
#ifndef LIST_MALLOC
	#define LIST_MALLOC malloc
#endif
#ifndef LIST_FREE
	#define LIST_FREE free
#endif

#define new_item(item) (typeof(item)*)LIST_MALLOC(sizeof(typeof(item)))

#define list_push(list, item) 												\
	do {																	\
		typeof(item)* new_node = new_item(item);							\
		LIST_ASSERT(new_node != NULL && "Buy more RAM...");					\
		*new_node = item;													\
		new_node->next = (list)->head;										\
		(list)->head = new_node;											\
		(list)->length++;			 										\
	} while (0)

#define list_insert_after(list, prev, item) 								\
	do {																	\
		LIST_ASSERT(prev != NULL && "prev is NULL");						\
		typeof(item)* new_node = new_item(item);							\
		LIST_ASSERT(new_node != NULL && "Buy more RAM...");					\
		*new_node = item;													\
		new_node->next = prev->next;										\
		prev->next = new_node;												\
		(list)->length++;													\
	} while (0)

#define list_append(list, item)												\
	do {																	\
		typeof(item)* new_node = new_item(item);							\
		LIST_ASSERT(new_node != NULL && "Buy more RAM...");					\
		*new_node = item;													\
		new_node->next = NULL;												\
		if ((list)->head == NULL) {											\
			(list)->head = new_node;										\
		} else {															\
			typeof(item)* current = (list)->head;							\
			while (current->next != NULL) {									\
				current = current->next;									\
			}																\
			current->next = new_node;										\
		}																	\
		(list)->length++;													\
	} while (0)

#define create_list(list)													\
	do {																	\
		typeof(list) new_list = (typeof(list))LIST_MALLOC(sizeof(typeof(*list)));	\
		LIST_ASSERT(new_list != NULL && "Buy more RAM...");					\
		list = new_list;													\
		(list)->length = 0;													\
		(list)->head = NULL;												\
	} while (0)

#define free_list(list)						 								\
	do {																	\
		if (list->head != NULL) {											\
			typeof(list->head) item = list->head;							\
			typeof(list->head) item_tmp;									\
			while ((list->length--) > 0) {						\
				item_tmp = item->next;										\
				LIST_FREE(item);											\
				item = item_tmp;											\
			}																\
		}																	\
		LIST_FREE(list);													\
	} while (0)
