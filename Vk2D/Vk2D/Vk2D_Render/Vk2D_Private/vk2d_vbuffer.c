#include "vk2d_vbuffer.h"
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_graphics_mem.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>
#include <Vk2D/Vk2D_Base/vk2d_list.h>

VkVertexInputBindingDescription vk2d_get_binding_description()
{
    VkVertexInputBindingDescription bindingDescription;
    vk2d_zero_memory(bindingDescription, sizeof(VkVertexInputBindingDescription));
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(vk2d_vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

VkVertexInputAttributeDescription* vk2d_get_attribute_descriptions()
{
    vk2d_new(VkVertexInputAttributeDescription* attributeDescriptions, sizeof(VkVertexInputAttributeDescription) * 4);

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(vk2d_vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(vk2d_vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(vk2d_vertex, tex_coords);

    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32_SINT;
    attributeDescriptions[3].offset = offsetof(vk2d_vertex, tex_index);

    return attributeDescriptions;
}

vk2d_vbuffer* vk2d_create_vbuffer(vk2d_gpu* gpu, vk2d_device* device, vk2d_command* command, i32 listSize, void* vertices)
{
    vk2d_new(vk2d_vbuffer* result, sizeof(vk2d_vbuffer));

    VkDeviceSize bufferSize = listSize * sizeof(vk2d_vertex);

    VkBuffer stagingBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingBufferMemory = 0;

    // Staging buffer
    {
        VkBufferCreateInfo bufferInfo;
        vk2d_zero_memory(bufferInfo, sizeof(VkBufferCreateInfo));
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = bufferSize;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device->device, &bufferInfo, NULL, &stagingBuffer) != VK_SUCCESS) {
            vk2d_log_fatal("Vk2D Renderer", "Failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device->device, stagingBuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo;
        vk2d_zero_memory(allocInfo, sizeof(VkMemoryAllocateInfo));
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = vk2d_find_memory_type(gpu, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(device->device, &allocInfo, NULL, &stagingBufferMemory) != VK_SUCCESS) {
            vk2d_log_fatal("Vk2D Renderer", "Failed to allocate buffer memory!");
        }

        vkBindBufferMemory(device->device, stagingBuffer, stagingBufferMemory, 0);
    }

    void* data;
    vkMapMemory(device->device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices, (size_t)bufferSize);
    vkUnmapMemory(device->device, stagingBufferMemory);

    // Vertex buffer creation
    {
        VkBufferCreateInfo bufferInfo;
        vk2d_zero_memory(bufferInfo, sizeof(VkBufferCreateInfo));
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = bufferSize;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device->device, &bufferInfo, NULL, &result->buffer) != VK_SUCCESS) {
            vk2d_log_fatal("Vk2D Renderer", "Failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device->device, result->buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo;
        vk2d_zero_memory(allocInfo, sizeof(VkMemoryAllocateInfo));
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = vk2d_find_memory_type(gpu, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (vkAllocateMemory(device->device, &allocInfo, NULL, &result->buffer_memory) != VK_SUCCESS) {
            vk2d_log_fatal("Vk2D Renderer", "Failed to allocate buffer memory!");
        }

        vkBindBufferMemory(device->device, result->buffer, result->buffer_memory, 0);
    }

    // Copy buffer
    {
        VkCommandPool commandPool = command->command_pool;
        VkQueue graphicsQueue = device->graphics_queue;

        VkCommandBufferAllocateInfo allocInfo;
        vk2d_zero_memory(allocInfo, sizeof(VkCommandBufferAllocateInfo));
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device->device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo;
        vk2d_zero_memory(beginInfo, sizeof(VkCommandBufferBeginInfo));
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion;
        vk2d_zero_memory(copyRegion, sizeof(VkBufferCopy));
        copyRegion.size = bufferSize;
        vkCmdCopyBuffer(commandBuffer, stagingBuffer, result->buffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo;
        vk2d_zero_memory(submitInfo, sizeof(VkSubmitInfo));
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);

        vkFreeCommandBuffers(device->device, commandPool, 1, &commandBuffer);
    }

    vk2d_assert(result->buffer != VK_NULL_HANDLE);

    vkDestroyBuffer(device->device, stagingBuffer, NULL);
    vkFreeMemory(device->device, stagingBufferMemory, NULL);

    result->vertex_count = listSize;

    return result;
}

vk2d_vbuffer* vk2d_create_vbuffer_empty(vk2d_gpu* gpu, vk2d_device* device, vk2d_command* command, i32 listSize)
{
    vk2d_new(vk2d_vbuffer* result, sizeof(vk2d_vbuffer));

    VkDeviceSize bufferSize = listSize * sizeof(vk2d_vertex);

    VkBufferCreateInfo bufferInfo;
    vk2d_zero_memory(bufferInfo, sizeof(VkBufferCreateInfo));
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage =  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device->device, &bufferInfo, NULL, &result->buffer) != VK_SUCCESS) {
        vk2d_log_fatal("Vk2D Renderer", "Failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device->device, result->buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo;
    vk2d_zero_memory(allocInfo, sizeof(VkMemoryAllocateInfo));
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = vk2d_find_memory_type(gpu, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    if (vkAllocateMemory(device->device, &allocInfo, NULL, &result->buffer_memory) != VK_SUCCESS) {
        vk2d_log_fatal("Vk2D Renderer", "Failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device->device, result->buffer, result->buffer_memory, 0);

    vk2d_assert(result->buffer != VK_NULL_HANDLE);

    result->vertex_count = listSize;

    return result;
}

void vk2d_set_vbuffer_data(VkCommandBuffer cbuf, vk2d_vbuffer* buffer, i32 listSize, void* vertices)
{
    vk2d_assert(buffer != VK_NULL_HANDLE);

    VkDevice device = volkGetLoadedDevice();

    vkCmdUpdateBuffer(cbuf, buffer->buffer, 0, listSize, vertices);
}

void vk2d_free_vbuffer(vk2d_vbuffer* buffer)
{
    VkDevice device = volkGetLoadedDevice();

    vkDestroyBuffer(device, buffer->buffer, NULL);
	vkFreeMemory(device, buffer->buffer_memory, NULL);
    vk2d_free(buffer);
}