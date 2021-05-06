#ifndef VK2D_EVENT
#define VK2D_EVENT

#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef void (*vk2d_event_callback)(i32, i32);

void vk2d_event_emit(const char* name, i32 lowParam, i32 highParam);
void vk2d_event_bind(const char* name, vk2d_event_callback callback);

#endif