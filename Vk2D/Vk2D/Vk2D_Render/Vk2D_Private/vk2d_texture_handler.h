#ifndef VK2D_TEXTURE_HANDLER
#define VK2D_TEXTURE_HANDLER

#include <volk.h>
#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef struct vk2d_texture_handler vk2d_texture_handler;
struct vk2d_texture_handler
{
    VkImage texture_image;
    VkImageView texture_image_view;
    VkSampler texture_sampler;
};



#endif