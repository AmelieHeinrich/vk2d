#ifndef VK2D_INSTANCE_DATA
#define VK2D_INSTANCE_DATA

#include <vulkan/vulkan.h>
#include <volk.h>

#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef struct vk2d_instance_data vk2d_instance_data;
struct vk2d_instance_data
{
    VkInstance instance;
    char *extension_names[64];
    char *enabled_layers[64];
    u32 enable_extension_count;
    u32 enable_layer_count;
};

#endif