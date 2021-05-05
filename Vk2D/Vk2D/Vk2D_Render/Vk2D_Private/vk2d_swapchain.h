#ifndef VK2D_SWAPCHAIN
#define VK2D_SWAPCHAIN

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <volk.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_gpu.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_renderpass.h>

typedef struct vk2d_swapchain vk2d_swapchain;
struct vk2d_swapchain
{
    VkSwapchainKHR handle;
    VkImage* swap_chain_images;
    VkImageView* swap_chain_image_views;
    VkFramebuffer* swap_chain_framebuffers;
    VkFormat swap_chain_image_format;
    VkExtent2D swap_chain_extent;
    i32 num_buffers;
};

vk2d_swapchain* vk2d_create_swapchain(vk2d_gpu* gpu, VkSurfaceKHR surface, i32 width, i32 height, i32 num_buffers);
void vk2d_construct_framebuffers(vk2d_swapchain* sc, vk2d_renderpass* renderpass);
void vk2d_free_swapchain(vk2d_swapchain* swapchain);

#endif