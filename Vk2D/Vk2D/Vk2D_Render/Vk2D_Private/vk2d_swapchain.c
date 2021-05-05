#include "vk2d_swapchain.h"
#include <stdlib.h>
#include <string.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>

vk2d_swapchain* vk2d_create_swapchain(vk2d_gpu* gpu, VkSurfaceKHR surface, i32 width, i32 height, i32 num_buffers)
{
    vk2d_assert(gpu != NULL);
    vk2d_assert(surface != VK_NULL_HANDLE);

    vk2d_swapchain* result = malloc(sizeof(vk2d_swapchain));   
    result->num_buffers = num_buffers;

    VkExtent2D extent;
    extent.width = width;
    extent.height = height;

    u32 queueFamilyIndices[] = { gpu->indices.graphics_family, gpu->indices.present_family };

    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu->gpu, surface, &capabilities);

    VkSwapchainCreateInfoKHR createInfo;
    memset(&createInfo, 0, sizeof(VkSwapchainCreateInfoKHR));
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = num_buffers;
    createInfo.imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    
    VkDevice device = volkGetLoadedDevice();
    vk2d_assert(device != VK_NULL_HANDLE);

    u32 formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu->gpu, surface, &formatCount, NULL);
    VkSurfaceFormatKHR* formats = malloc(sizeof(VkSurfaceFormatKHR) * formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu->gpu, surface, &formatCount, formats);

    createInfo.imageFormat = formats[0].format;
    createInfo.imageColorSpace = formats[0].colorSpace;

    VkResult res = vkCreateSwapchainKHR(device, &createInfo, NULL, &result->handle);
    vk2d_assert(res == VK_SUCCESS);

    i32 imageCount;
    vkGetSwapchainImagesKHR(device, result->handle, &imageCount, NULL);
    result->swap_chain_images = malloc(sizeof(VkImage) * imageCount);
    vkGetSwapchainImagesKHR(device, result->handle, &imageCount, result->swap_chain_images);

    result->swap_chain_extent = extent;
    result->swap_chain_image_format = createInfo.imageFormat;

    result->swap_chain_image_views = malloc(sizeof(VkImageView) * num_buffers);
    for (i32 i = 0; i < num_buffers; i++) {
        VkImageViewCreateInfo iv_createInfo;
        memset(&iv_createInfo, 0, sizeof(VkImageViewCreateInfo));
        iv_createInfo.flags = 0;
        iv_createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        iv_createInfo.image = result->swap_chain_images[i];
        iv_createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        iv_createInfo.format = result->swap_chain_image_format;
        iv_createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        iv_createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        iv_createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        iv_createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        iv_createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        iv_createInfo.subresourceRange.baseMipLevel = 0;
        iv_createInfo.subresourceRange.levelCount = 1;
        iv_createInfo.subresourceRange.baseArrayLayer = 0;
        iv_createInfo.subresourceRange.layerCount = 1;

        res = vkCreateImageView(device, &iv_createInfo, NULL, &result->swap_chain_image_views[i]);
        vk2d_assert(res == VK_SUCCESS);
    }

    free(formats);

    return result;
}

void vk2d_construct_framebuffers(vk2d_swapchain* sc, vk2d_renderpass* renderpass)
{
    VkDevice device = volkGetLoadedDevice();

    sc->swap_chain_framebuffers = malloc(sizeof(VkFramebuffer) * sc->num_buffers);

    for (size_t i = 0; i < sc->num_buffers; i++) {
        VkImageView attachments[] = {
            sc->swap_chain_image_views[i]
        };

        VkFramebufferCreateInfo framebufferInfo;
        memset(&framebufferInfo, 0, sizeof(VkFramebufferCreateInfo));
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderpass->render_pass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = sc->swap_chain_extent.width;
        framebufferInfo.height = sc->swap_chain_extent.height;
        framebufferInfo.layers = 1;

        VkResult res = vkCreateFramebuffer(device, &framebufferInfo, NULL, &sc->swap_chain_framebuffers[i]);
        vk2d_assert(res == VK_SUCCESS);
    }
}

void vk2d_free_swapchain(vk2d_swapchain* swapchain)
{
    VkDevice device = volkGetLoadedDevice();

    for (i32 i = 0; i < swapchain->num_buffers; i++) {
        vkDestroyFramebuffer(device, swapchain->swap_chain_framebuffers[i], NULL);
        vkDestroyImageView(device, swapchain->swap_chain_image_views[i], NULL);
    }

    free(swapchain->swap_chain_framebuffers);
    free(swapchain->swap_chain_image_views);
    free(swapchain->swap_chain_images);
    vkDestroySwapchainKHR(device, swapchain->handle, NULL);
    free(swapchain);
}