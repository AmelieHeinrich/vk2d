#ifndef VK2D_DEVICE
#define VK2D_DEVICE

#include <vulkan/vulkan.h>
#include <volk.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_instance_data.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_gpu.h>

typedef struct vk2d_device vk2d_device;
struct vk2d_device
{
    VkDevice device;
    VkQueue graphics_queue;
};

void vk2d_init_device(vk2d_device* dst, vk2d_instance_data data, VkPhysicalDevice gpu, vk2d_queue_family_indices indices);
void vk2d_free_device(vk2d_device* device);


#endif