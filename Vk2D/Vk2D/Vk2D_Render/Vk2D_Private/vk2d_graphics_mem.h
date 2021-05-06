#ifndef VK2D_GRAPHICS_MEM
#define VK2D_GRAPHICS_MEM

#include <volk.h>
#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_renderer_data.h>

u32 vk2d_find_memory_type(vk2d_gpu* gpu, u32 typeFilter, VkMemoryPropertyFlags properties);

#endif