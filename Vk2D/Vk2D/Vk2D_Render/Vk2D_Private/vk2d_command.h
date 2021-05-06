#ifndef VK2D_COMMAND
#define VK2D_COMMAND

#include <volk.h>
#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_gpu.h>

// Structs that represents draw command objects
typedef struct vk2d_command vk2d_command;
struct vk2d_command
{
    VkCommandPool command_pool; // The command pool
    VkCommandBuffer* command_buffers; // One command buffer per frame
    VkCommandBuffer active_command_buffer; // The current command buffer that is being used
};

vk2d_command* vk2d_create_command(vk2d_gpu* gpu);
void vk2d_free_command(vk2d_command* command);

#endif