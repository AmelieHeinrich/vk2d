#include "vk2d_graphics_mem.h"
#include <Vk2D/Vk2D_Base/vk2d_log.h>

u32 vk2d_find_memory_type(vk2d_gpu* gpu, u32 typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(gpu->gpu, &memProperties);

    for (u32 i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

    return 0;
}