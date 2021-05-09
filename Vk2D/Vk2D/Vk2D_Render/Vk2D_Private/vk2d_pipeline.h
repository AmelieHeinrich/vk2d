#ifndef VK2D_PIPELINE
#define VK2D_PIPELINE

#include <volk.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_shader.h>
#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_renderpass.h>

typedef struct vk2d_pipeline vk2d_pipeline;
struct vk2d_pipeline
{
    VkPipelineLayout pipeline_layout; // The Vulkan Pipeline Layout handle
    VkPipeline pipeline; // The Graphics Pipeline itself
    u32 width; // The pipeline width
    u32 height; // The pipeline height
};

vk2d_pipeline* vk2d_create_pipeline(vk2d_shader* shader, u32 width, u32 height, vk2d_renderpass* renderpass, VkDescriptorSetLayout descriptor);
void vk2d_free_pipeline(vk2d_pipeline* pipeline);

#endif