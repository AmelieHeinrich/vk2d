#include "vk2d_texture_handler.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void transition_image_layout(vk2d_command* command, vk2d_device* device, VkImage* image, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBufferAllocateInfo allocInfo;
    vk2d_zero_memory(allocInfo, sizeof(VkCommandBufferAllocateInfo));
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = command->command_pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device->device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo;
    vk2d_zero_memory(beginInfo, sizeof(VkCommandBufferBeginInfo));
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkImageMemoryBarrier barrier;
    vk2d_zero_memory(barrier, sizeof(VkImageMemoryBarrier));
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = *image;
    barrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
    barrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (barrier.newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    else {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    if (barrier.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && barrier.newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (barrier.oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && barrier.newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (barrier.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && barrier.newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else {
        vk2d_log_fatal("Vk2D Renderer", "Unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, NULL,
        0, NULL,
        1, &barrier
    );

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo;
    vk2d_zero_memory(submitInfo, sizeof(VkSubmitInfo));
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device->graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device->graphics_queue);

    vkFreeCommandBuffers(device->device, command->command_pool, 1, &commandBuffer);
}

void copy_image(vk2d_command* command, vk2d_device* device, u32 w, u32 h, VkBuffer* buffer, VkImage* dst)
{
    VkCommandBufferAllocateInfo allocInfo;
    vk2d_zero_memory(allocInfo, sizeof(VkCommandBufferAllocateInfo));
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = command->command_pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device->device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo;
    vk2d_zero_memory(beginInfo, sizeof(VkCommandBufferBeginInfo));
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkExtent3D extent;
    extent.width = w;
    extent.height = h;
    extent.depth = 1;

    VkOffset3D offset;
    offset.x = 0;
    offset.y = 0;
    offset.z = 0;

    VkBufferImageCopy region;
    vk2d_zero_memory(region, sizeof(VkBufferImageCopy));
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = offset;
    region.imageExtent = extent;

    vkCmdCopyBufferToImage(commandBuffer, *buffer, *dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo;
    vk2d_zero_memory(submitInfo, sizeof(VkSubmitInfo));
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device->graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device->graphics_queue);

    vkFreeCommandBuffers(device->device, command->command_pool, 1, &commandBuffer);
}

void create_image(vk2d_device* device, vk2d_gpu* gpu, u32 w, u32 h, VkImage* img, VkFormat format, VkDeviceMemory* mem)
{
    VkImageCreateInfo imageInfo;
    vk2d_zero_memory(imageInfo, sizeof(VkImageCreateInfo));
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = w;
    imageInfo.extent.height = h;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult res = vkCreateImage(device->device, &imageInfo, NULL, img);
    vk2d_assert(res == VK_SUCCESS);

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device->device, *img, &memRequirements);

    VkMemoryAllocateInfo allocInfo;
    vk2d_zero_memory(allocInfo, sizeof(VkMemoryAllocateInfo));
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = vk2d_find_memory_type(gpu, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    res = vkAllocateMemory(device->device, &allocInfo, NULL, mem);
    vk2d_assert(res == VK_SUCCESS);

    vkBindImageMemory(device->device, *img, *mem, 0);
}

void create_img_staging_buffer(VkDeviceSize image_size, vk2d_device* device, vk2d_gpu* gpu, VkBuffer* stagingBuffer, VkDeviceMemory* stagingBufferMemory)
{
    VkBufferCreateInfo bufferInfo;
    vk2d_zero_memory(bufferInfo, sizeof(VkBufferCreateInfo));
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = image_size;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device->device, &bufferInfo, NULL, stagingBuffer) != VK_SUCCESS) {
        vk2d_log_fatal("Vk2D Renderer", "Failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device->device, *stagingBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo;
    vk2d_zero_memory(allocInfo, sizeof(VkMemoryAllocateInfo));
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = vk2d_find_memory_type(gpu, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(device->device, &allocInfo, NULL, stagingBufferMemory) != VK_SUCCESS) {
        vk2d_log_fatal("Vk2D Renderer", "Failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device->device, *stagingBuffer, *stagingBufferMemory, 0);
}

void create_img_view(VkImage* img, VkImageView* view, VkFormat format, vk2d_device* device)
{
    VkImageViewCreateInfo viewInfo;
    vk2d_zero_memory(viewInfo, sizeof(VkImageViewCreateInfo));
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = *img;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    VkResult res = vkCreateImageView(device->device, &viewInfo, NULL, view);
    vk2d_assert(res == VK_SUCCESS);
}

void create_sampler(vk2d_gpu* gpu, vk2d_device* device, VkSampler* dst)
{
    VkPhysicalDeviceProperties properties;
    vk2d_zero_memory(properties, sizeof(VkPhysicalDeviceProperties));
    vkGetPhysicalDeviceProperties(gpu->gpu, &properties);

    VkSamplerCreateInfo samplerInfo;
    vk2d_zero_memory(samplerInfo, sizeof(VkSamplerCreateInfo));
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    VkResult res = vkCreateSampler(device->device, &samplerInfo, NULL, dst);
    vk2d_assert(res == VK_SUCCESS);
}

vk2d_texture_handler* vk2d_init_texture_handler(vk2d_gpu* gpu, vk2d_device* device, vk2d_command* command, const char* filepath)
{
    vk2d_new(vk2d_texture_handler* result, sizeof(vk2d_texture_handler));

    result->texture_image = VK_NULL_HANDLE;
    result->texture_image_view = VK_NULL_HANDLE;
    result->texture_sampler = VK_NULL_HANDLE;
    result->texture_memory = 0;

    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(filepath, &result->width, &result->height, &result->channels, 4);
    if (!data)
    {
        vk2d_log_fatal("Vk2D Renderer", "Failed to load texture!");
        return NULL;
    }

    switch (result->channels)
    {
        case 3:
            result->format = VK_FORMAT_R8G8B8_UNORM;
            break;
        case 4:
            result->format = VK_FORMAT_R8G8B8A8_UNORM;
            break;
        default:
            vk2d_log_fatal("Vk2D Renderer", "Unsupported channels!");
            break;
    }

    VkDeviceSize image_size = result->width * result->height * 4;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    create_img_staging_buffer(image_size, device, gpu, &stagingBuffer, &stagingBufferMemory);

    void* the_data;
    vkMapMemory(device->device, stagingBufferMemory, 0, image_size, 0, &the_data);
    memcpy(the_data, data, (size_t)image_size);
    vkUnmapMemory(device->device, stagingBufferMemory);

    stbi_image_free(data);

    // Image
    create_image(device, gpu, result->width, result->height, &result->texture_image, result->format, &result->texture_memory);

    // Heap upload
    transition_image_layout(command, device, &result->texture_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copy_image(command, device, result->width, result->height, &stagingBuffer, &result->texture_image);
    transition_image_layout(command, device, &result->texture_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Image view
    create_img_view(&result->texture_image, &result->texture_image_view, result->format, device);

    // Sampler
    create_sampler(gpu, device, &result->texture_sampler);
    
    vkDestroyBuffer(device->device, stagingBuffer, NULL);
    vkFreeMemory(device->device, stagingBufferMemory, NULL);

    return result;
}

vk2d_texture_handler* vk2d_init_texture_handler_raw(vk2d_gpu* gpu, vk2d_device* device, vk2d_command* command, void* data)
{
    vk2d_new(vk2d_texture_handler* result, sizeof(vk2d_texture_handler));

    result->texture_image = VK_NULL_HANDLE;
    result->texture_image_view = VK_NULL_HANDLE;
    result->texture_sampler = VK_NULL_HANDLE;
    result->texture_memory = 0;
    result->width = 1;
    result->height = 1;
    result->channels = 4;
    result->format = VK_FORMAT_R8G8B8A8_UNORM;

    VkDeviceSize image_size = result->width * result->height * 4;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    create_img_staging_buffer(image_size, device, gpu, &stagingBuffer, &stagingBufferMemory);

    void* raw_data;
    vkMapMemory(device->device, stagingBufferMemory, 0, image_size, 0, &raw_data);
    memcpy(raw_data, data, (size_t)image_size);
    vkUnmapMemory(device->device, stagingBufferMemory);

    // Image creation
    create_image(device, gpu, 1, 1, &result->texture_image, result->format, &result->texture_memory);

    // Heap upload
    transition_image_layout(command, device, &result->texture_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copy_image(command, device, result->width, result->height, &stagingBuffer, &result->texture_image);
    transition_image_layout(command, device, &result->texture_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Image view
    create_img_view(&result->texture_image, &result->texture_image_view, result->format, device);

    // Sampler
    create_sampler(gpu, device, &result->texture_sampler);
    
    vkDestroyBuffer(device->device, stagingBuffer, NULL);
    vkFreeMemory(device->device, stagingBufferMemory, NULL);

    return result;
}

void vk2d_free_texture_handler(vk2d_device* device, vk2d_texture_handler* handler)
{
    if (handler->texture_memory != 0)
        vkFreeMemory(device->device, handler->texture_memory, NULL);
    if (handler->texture_sampler != VK_NULL_HANDLE)
        vkDestroySampler(device->device, handler->texture_sampler, NULL);
    if (handler->texture_image_view != VK_NULL_HANDLE)
        vkDestroyImageView(device->device, handler->texture_image_view, NULL);
    if (handler->texture_image != VK_NULL_HANDLE)
        vkDestroyImage(device->device, handler->texture_image, NULL);

    vk2d_free(handler);
}