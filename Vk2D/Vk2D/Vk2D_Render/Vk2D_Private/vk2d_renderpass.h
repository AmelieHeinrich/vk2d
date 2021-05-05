#ifndef VK2D_RENDERPASS
#define VK2D_RENDERPASS

#include <volk.h>
#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef struct vk2d_renderpass vk2d_renderpass;
struct vk2d_renderpass
{
    const char* name;
    VkFormat render_pass_format;
    VkRenderPass render_pass;
};

vk2d_renderpass* vk2d_create_renderpass(const char* name, VkFormat format);
void vk2d_free_renderpass(vk2d_renderpass* renderpass);

#endif