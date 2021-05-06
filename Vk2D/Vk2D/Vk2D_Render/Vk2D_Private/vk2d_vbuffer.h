#ifndef VK2D_VBUFFER
#define VK2D_VBUFFER

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <volk.h>

typedef struct vk2d_vbuffer vk2d_vbuffer;
struct vk2d_vbuffer
{
    VkBuffer buffer;
    VkDeviceMemory buffer_memory;
};

#endif