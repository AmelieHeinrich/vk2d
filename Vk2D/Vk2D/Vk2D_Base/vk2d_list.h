#ifndef VK2D_LIST
#define VK2D_LIST

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <stdlib.h>

typedef struct vk2d_list vk2d_list;
struct vk2d_list
{
    i32 count;
    i32 max;
    size_t size;
    void* items;
};

vk2d_list* vk2d_list_new(size_t size);
void* vk2d_list_add(vk2d_list* list, void* value);
void vk2d_list_remove(vk2d_list* list, void* value);
void vk2d_list_clear(vk2d_list* list, void* value);
i32 vk2d_list_has(vk2d_list* list, void* value);
void vk2d_list_free(vk2d_list* list);

#endif