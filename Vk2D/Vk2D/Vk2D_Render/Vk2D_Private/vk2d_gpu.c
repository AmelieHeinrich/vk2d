#include "vk2d_gpu.h"
#include <stdlib.h>
#include <string.h>
#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])
#include "vk2d_renderer_data.h"
#include <Vk2D/Vk2D_Base/vk2d_log.h>
#include <stdio.h>

vk2d_queue_family_indices vk2d_find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    vk2d_queue_family_indices indices;

    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

    vk2d_assert(queueFamilyCount >= 2);

    VkQueueFamilyProperties* queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    i32 j = 0;
    for (i32 i = 0; i < queueFamilyCount; i++)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family = j;
        }

        VkBool32 presentSupport = 0;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, j, surface, &presentSupport);

        if (presentSupport) {
            indices.present_family = j;
        }

        if (indices.graphics_family && indices.present_family) {
            break;
        }

        j++;
    }

    free(queueFamilies);

    return indices;
}

void vk2d_init_gpu(vk2d_gpu* dst, VkInstance instance, VkSurfaceKHR surface)
{
    u32 deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

    if (deviceCount == 0) {
        vk2d_log_fatal("Vk2D", "Failed to find suitable device!\n");
    }

    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);
    dst->gpu = devices[0];

    if (dst->gpu == VK_NULL_HANDLE) {
        vk2d_log_fatal("Vk2D", "Failed to find suitable device!\n");
    }

    dst->indices = vk2d_find_queue_families(dst->gpu, surface);
    vkGetPhysicalDeviceProperties(dst->gpu, &dst->gpu_props);
    vkGetPhysicalDeviceFeatures(dst->gpu, &dst->gpu_features);

    char dest[50] = "Using GPU with name: ";
    strcat(dest, dst->gpu_props.deviceName);
    vk2d_log_info("Vk2D", dest);

    free(devices);
}