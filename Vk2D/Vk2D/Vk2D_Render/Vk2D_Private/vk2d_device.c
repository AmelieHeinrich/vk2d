#include "vk2d_device.h"
#include <string.h>
#include <stdlib.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>

void vk2d_init_device(vk2d_device* dst, vk2d_instance_data data, VkPhysicalDevice gpu, vk2d_queue_family_indices indices)
{
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo;
    queueCreateInfo.flags = 0;
    queueCreateInfo.pNext = NULL;
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphics_family;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(gpu, &features);

    VkDeviceCreateInfo create_info;
    memset(&create_info, 0, sizeof(VkDeviceCreateInfo));
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = 1;
    create_info.pQueueCreateInfos = &queueCreateInfo;
    create_info.pEnabledFeatures = &features;
    create_info.enabledExtensionCount = 0;
    create_info.ppEnabledExtensionNames = NULL;
    create_info.enabledLayerCount = data.enable_layer_count;
    create_info.ppEnabledLayerNames = (const char *const *)data.enabled_layers;

    VkResult err = vkCreateDevice(gpu, &create_info, NULL, &dst->device);
    vk2d_assert(err == VK_SUCCESS);

    volkLoadDevice(dst->device);

    vkGetDeviceQueue(dst->device, indices.graphics_family, 0, &dst->graphics_queue);
}

void vk2d_free_device(vk2d_device* device)
{
    vkDestroyDevice(device->device, NULL);
    free(device);
}