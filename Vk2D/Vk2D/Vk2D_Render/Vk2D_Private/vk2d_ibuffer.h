#ifndef VK2D_IBUFFER
#define VK2D_IBUFFER

#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_renderer_data.h>
#include <Vk2D/Vk2D_Base/vk2d_list.h>
#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Math/vk2d_vec2.h>
#include <Vk2D/Vk2D_Math/vk2d_vec3.h>
#include <Vk2D/Vk2D_Math/vk2d_vec4.h>
#include <volk.h>

typedef struct vk2d_ibuffer vk2d_ibuffer;
struct vk2d_ibuffer
{
    VkBuffer buffer;
    VkDeviceMemory buffer_memory;
    u32 index_count;
};

// The actual vertex buffer code
vk2d_ibuffer* vk2d_create_ibuffer(vk2d_gpu* gpu, vk2d_device* device, vk2d_command* command, i32 listSize, void* vertices);
vk2d_ibuffer* vk2d_create_ibuffer_empty(vk2d_gpu* gpu, vk2d_device* device, vk2d_command* command, i32 listSize);
void vk2d_free_ibuffer(vk2d_ibuffer* buffer);
void vk2d_set_ibuffer_data(VkCommandBuffer cbuf, vk2d_ibuffer* buffer, i32 listSize, void* vertices);

#endif