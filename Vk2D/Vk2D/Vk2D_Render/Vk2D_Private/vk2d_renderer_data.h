#ifndef VK2D_RENDERER_DATA
#define VK2D_RENDERER_DATA

#include <vulkan/vulkan.h>
#include <volk.h>

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_gpu.h>

typedef struct vk2d_renderer_data vk2d_renderer_data;
struct vk2d_renderer_data
{
    VkInstance instance;
    char *extension_names[64];
    char *enabled_layers[64];
    u32 enable_extension_count;
    u32 enable_layer_count;

    VkSurfaceKHR surface;

    vk2d_gpu physical_device;
};

static vk2d_renderer_data _data;

#endif