#ifndef VK2D_COMMAND
#define VK2D_COMMAND

#include <volk.h>
#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_gpu.h>

typedef struct vk2d_command vk2d_command;
struct vk2d_command
{
    VkCommandPool command_pool;
    VkCommandBuffer* command_buffers;
    VkCommandBuffer active_command_buffer;
};

vk2d_command* vk2d_create_command(vk2d_gpu* gpu);
void vk2d_free_command(vk2d_command* command);

#endif