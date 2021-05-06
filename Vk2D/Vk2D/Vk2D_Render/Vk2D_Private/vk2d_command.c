#include "vk2d_command.h"

#include <string.h>
#include <stdlib.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>

vk2d_command* vk2d_create_command(vk2d_gpu* gpu)
{
    vk2d_new(vk2d_command* result, sizeof(vk2d_command));
    vk2d_new(result->command_buffers, sizeof(VkCommandBuffer) * 2);

    VkDevice device = volkGetLoadedDevice();

    VkCommandPoolCreateInfo poolInfo;
    vk2d_zero_memory(poolInfo, sizeof(VkCommandPoolCreateInfo));
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = gpu->indices.graphics_family;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    {
        VkResult res = vkCreateCommandPool(device, &poolInfo, NULL, &result->command_pool);
        vk2d_assert(res == VK_SUCCESS);
    }

    VkCommandBufferAllocateInfo allocInfo;
    vk2d_zero_memory(allocInfo, sizeof(VkCommandBufferAllocateInfo));
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = result->command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 2;

    {
        VkResult res = vkAllocateCommandBuffers(device, &allocInfo, result->command_buffers);
        vk2d_assert(res == VK_SUCCESS);
    }

    return result;
}

void vk2d_free_command(vk2d_command* command)
{
    VkDevice device = volkGetLoadedDevice();

    vkDestroyCommandPool(device, command->command_pool, NULL);

    vk2d_free(command->command_buffers);
    vk2d_free(command);
}