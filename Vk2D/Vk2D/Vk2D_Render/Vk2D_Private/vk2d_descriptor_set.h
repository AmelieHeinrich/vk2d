#ifndef VK2D_DESCRIPTOR_SET
#define VK2D_DESCRIPTOR_SET

#include <volk.h>
#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef struct vk2d_dset vk2d_dset;
struct vk2d_dset
{
    VkDescriptorSet descriptor_set;
    VkDescriptorSetLayout descriptor_layout;
};

#endif