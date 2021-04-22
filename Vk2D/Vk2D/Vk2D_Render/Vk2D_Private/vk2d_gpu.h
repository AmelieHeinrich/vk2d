#ifndef VK2D_GPU
#define VK2D_GPU

#include <vulkan/vulkan.h>
#include <volk.h>

#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef struct vk2d_queue_family_indices vk2d_queue_family_indices;
struct vk2d_queue_family_indices
{
    u32 graphics_family;
    u32 present_family;
};

typedef struct vk2d_gpu vk2d_gpu;
struct vk2d_gpu
{
    VkPhysicalDevice gpu;
    vk2d_queue_family_indices indices;
    VkPhysicalDeviceProperties gpu_props;
    VkPhysicalDeviceFeatures gpu_features;
};

void vk2d_init_gpu(vk2d_gpu* dst, VkInstance instance, VkSurfaceKHR surface);

vk2d_queue_family_indices vk2d_find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);

#endif