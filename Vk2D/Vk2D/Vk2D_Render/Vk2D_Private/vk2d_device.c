#include "vk2d_device.h"
#include <string.h>
#include <stdlib.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>

#define array_size(arr) (sizeof(arr) / sizeof(arr[0]))

void vk2d_init_device(vk2d_device* dst, vk2d_instance_data data, VkPhysicalDevice gpu, vk2d_queue_family_indices indices)
{
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo graphicsQueueCreateInfo;
    graphicsQueueCreateInfo.flags = 0;
    graphicsQueueCreateInfo.pNext = NULL;
    graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    graphicsQueueCreateInfo.queueFamilyIndex = indices.graphics_family;
    graphicsQueueCreateInfo.queueCount = 1;
    graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceQueueCreateInfo presentQueueCreateInfo;
    presentQueueCreateInfo.flags = 0;
    presentQueueCreateInfo.pNext = NULL;
    presentQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    presentQueueCreateInfo.queueFamilyIndex = indices.present_family;
    presentQueueCreateInfo.queueCount = 1;
    presentQueueCreateInfo.pQueuePriorities = &queuePriority;

    i32 seperatePresentQueue = 0;

    vk2d_new(VkDeviceQueueCreateInfo * createInfos, sizeof(VkDeviceQueueCreateInfo) * 2);
    
    if (graphicsQueueCreateInfo.queueFamilyIndex == presentQueueCreateInfo.queueFamilyIndex)
    {
        createInfos = realloc(createInfos, sizeof(VkDeviceQueueCreateInfo));
        createInfos[0] = graphicsQueueCreateInfo;
    }
    else
    {
        createInfos[0] = graphicsQueueCreateInfo;
        createInfos[1] = presentQueueCreateInfo;
        seperatePresentQueue = 1;
    }

    VkPhysicalDeviceFeatures features = {0};
    features.samplerAnisotropy = 1;

    char* extensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo create_info;
    vk2d_zero_memory(create_info, sizeof(VkDeviceCreateInfo));
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = 1;
    create_info.pQueueCreateInfos = createInfos;
    create_info.pEnabledFeatures = &features;
    create_info.enabledExtensionCount = 1;
    create_info.ppEnabledExtensionNames = (const char *const *)extensions;

#ifdef _DEBUG
    create_info.enabledLayerCount = data.enable_layer_count;
    create_info.ppEnabledLayerNames = (const char *const *)data.enabled_layers;
#else
    create_info.enabledLayerCount = 0;
    create_info.ppEnabledLayerNames = NULL;
#endif

    if (seperatePresentQueue)
    {
        create_info.queueCreateInfoCount = 2;
    }

    VkResult err = vkCreateDevice(gpu, &create_info, NULL, &dst->device);
    vk2d_assert(err == VK_SUCCESS);

    volkLoadDevice(dst->device);

    vkGetDeviceQueue(dst->device, indices.graphics_family, 0, &dst->graphics_queue);
    vkGetDeviceQueue(dst->device, indices.present_family, 0, &dst->present_queue);

    vk2d_free(createInfos);
}

void vk2d_free_device(vk2d_device* device)
{
    vkDestroyDevice(device->device, NULL);
    vk2d_free(device);
}