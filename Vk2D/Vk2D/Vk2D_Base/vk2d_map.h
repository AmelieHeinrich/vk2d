#ifndef VK2D_MAP
#define VK2D_MAP

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <stdlib.h>

typedef struct vk2d_map_item vk2d_map_item;
struct vk2d_map_item
{
    char* key;
    void* value;
    vk2d_map_item* next;
};

typedef struct vk2d_map vk2d_map;
struct vk2d_map
{
    vk2d_map_item* items;
    size_t size;
    i32 max_len;
};

vk2d_map* vk2d_map_new(i32 max_len, size_t size);
void vk2d_map_set(vk2d_map* map, const char* key, void* value);
void* vk2d_map_get(vk2d_map* map, const char* key);
void vk2d_map_remove(vk2d_map* map, const char* key);
void vk2d_map_free(vk2d_map* map);

#endif