#ifndef VK2D_RENDERER_DATA
#define VK2D_RENDERER_DATA

#include <vulkan/vulkan.h>
#include <volk.h>

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_instance_data.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_gpu.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_device.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_swapchain.h>

typedef struct vk2d_renderer_data vk2d_renderer_data;
struct vk2d_renderer_data
{
    vk2d_instance_data instance_data;
    VkSurfaceKHR surface;
    vk2d_gpu* physical_device;
    vk2d_device* logical_device;
    vk2d_swapchain* swap_chain;
};

static vk2d_renderer_data* _data;

#endif