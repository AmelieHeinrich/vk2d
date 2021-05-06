#include "vk2d_map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static i32 vk2d_map_hash(const char* key)
{
    i32 hash = 0;

    for (i32 i = 0; i < strlen(key); ++i)
        hash += key[i] * 23;
    
    return hash;
}

vk2d_map* vk2d_map_new(i32 max_len, size_t size)
{
    vk2d_new(vk2d_map* map, sizeof(vk2d_map))

    vk2d_map_item item;
    vk2d_zero_memory(item, sizeof(vk2d_map_item));

    map->max_len = max_len;
    map->size = size;
    vk2d_new(map->items, max_len * sizeof(vk2d_map_item));

    for (i32 i = 0; i < max_len; ++i)
        map->items[i] = item;

    return map;
}

void vk2d_map_set(vk2d_map* map, const char* key, void* value)
{
    i32 hash = vk2d_map_hash(key);
    vk2d_map_item* item = map->items + (hash % map->max_len);
    vk2d_map_item* new_item = NULL;

    if (item->value == NULL) {
        char* k = calloc(sizeof(char), strlen(key) + 1);
		void* v = calloc(map->size, 1);

        strcpy(k, key);
		memcpy(v, value, map->size);

        item->key = k;
		item->value = v;
		return;
    }

    for (;;) {
        if (strcmp(item->key, key) == 0) {
			void *v = calloc(map->size, 1);

			memcpy(v, value, map->size);
			vk2d_free(item->value);
			item->value = v;
			return;
		}
		if (item->next == NULL)
			break;

		item = item->next;
    }

    new_item = calloc(sizeof(vk2d_map_item), 1);
	new_item->key = calloc(sizeof(char), strlen(key) + 1);
	new_item->value = calloc(map->size, 1);
	strcpy(new_item->key, key);
	memcpy(new_item->value, value, map->size);
	item->next = new_item;
}

void* vk2d_map_get(vk2d_map* map, const char* key)
{
    i32 hash = vk2d_map_hash(key);
	vk2d_map_item* item = map->items + (hash % map->max_len);

	while (item != NULL && item->key != NULL) {
		if (strcmp(key, item->key) == 0)
			return item->value;
		item = item->next;
	}

	return NULL;
}

void vk2d_map_remove(vk2d_map* map, const char* key)
{
    i32 hash = vk2d_map_hash(key);
	vk2d_map_item* bucket = map->items+(hash % map->max_len); 
	vk2d_map_item* curr = bucket; 
	vk2d_map_item* prev = NULL;
	vk2d_map_item* next = curr->next; 

	while (curr != NULL) {
		next = curr->next;
		if (strcmp(curr->key, key) == 0)
			break;
		prev = curr;
		curr = next;
	}

	vk2d_free(curr->key);
	vk2d_free(curr->value);

	if (next == NULL) {
		curr->key = NULL;
		curr->value = NULL;
		if (prev == NULL)
			return;

		if (prev->next != NULL) {
			vk2d_free(prev->next);
			prev->next = NULL;
		}
	} else {
		curr->key = next->key;
		curr->value = next->value;
		curr->next = next->next;
		vk2d_free(next);
	}
}
