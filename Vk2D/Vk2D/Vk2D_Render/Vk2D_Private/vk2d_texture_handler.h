#ifndef VK2D_TEXTURE_HANDLER
#define VK2D_TEXTURE_HANDLER

#include <volk.h>
#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_renderer_data.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_graphics_mem.h>

typedef struct vk2d_texture_handler vk2d_texture_handler;
struct vk2d_texture_handler
{
    VkImage texture_image;
    VkDeviceMemory texture_memory;
    VkImageView texture_image_view;
    VkSampler texture_sampler;

    i32 width;
    i32 height;
    i32 channels;
    i32 format;
};

vk2d_texture_handler* vk2d_init_texture_handler(vk2d_gpu* gpu, vk2d_device* device, vk2d_command* command, const char* filepath);
void vk2d_free_texture_handler(vk2d_device* device, vk2d_texture_handler* handler);

#endif