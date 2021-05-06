#include "vk2d_list.h"

#include <Vk2d/Vk2D_Base/vk2d_log.h>

#include <stdlib.h>
#include <string.h>

vk2d_list* vk2d_list_new(size_t size)
{
    vk2d_new(vk2d_list* list, sizeof(vk2d_list));
    list->size = size;
    list->count = 0;
    list->max = 0;
    list->items = NULL;
    return list;
}

void* vk2d_list_add(vk2d_list* list, void* value)
{
    char* pos = NULL;

    if (list->items == NULL) {
        vk2d_new(list->items, list->size);
        list->max = 1; 
    } else if (list->count == list->max) {
        size_t size = list->size * list->max * 2;
        void* items = realloc(list->items, size);
        if (items == NULL) {
            vk2d_log_fatal("Vk2D Container List", "Failed to realloc list items!");
        }
        list->items = items;
        list->max *= 2;
    }

    pos = (char*)list->items+(list->count * list->size);

    if (memcpy(pos, value, list->size) == NULL) {
        vk2d_log_fatal("Vk2D Container List", "Failed to copy list value!");
    }

    list->count++;
    return pos;
}

void vk2d_list_remove(vk2d_list* list, void* value)
{
    i32 i = 0;
    char* pos = (char*)list->items;
    i32 count = 0;
    size_t reminder = 0;

    for (i = 0; i < list->count; ++i) 
    {
        if (memcmp(pos, value, list->size) == 0)
            goto rm;

        pos += list->size;
    }

rm:
    count = list->max - i - 1;
    reminder = count * list->size;
    if (memcpy(pos, pos + list->size, reminder) == NULL) {
        vk2d_log_fatal("Vk2D Container List", "Failed to remove element from list!");
    }
    list->count--;
}

void vk2d_list_clear(vk2d_list* list, void* value)
{
	i32 count = 0;
	char* pos = (char*)list->items;

    for (i32 i = 0; i < list->count; ++i) {
		if (memcmp(pos, value, list->size) == 0)
			count++;

		pos += list->size;
	}

	for (i32 i = 0; i < count; ++i)
		vk2d_list_remove(list, value);
}

i32 vk2d_list_has(vk2d_list* list, void* value)
{
	char* pos = (char*)list->items;

	for (i32 i = 0; i < list->count; ++i) {
		if (memcmp(pos, value, list->size) == 0)
			return 1;

		pos += list->size;
	}

	return 0;
}

void vk2d_list_free(vk2d_list* list)
{
    if (list->count > 0)
        vk2d_free(list->items);
    vk2d_free(list);
}