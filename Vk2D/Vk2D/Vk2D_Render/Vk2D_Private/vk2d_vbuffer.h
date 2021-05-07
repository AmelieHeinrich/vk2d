#ifndef VK2D_VBUFFER
#define VK2D_VBUFFER

#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_renderer_data.h>
#include <Vk2D/Vk2D_Base/vk2d_list.h>
#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Math/vk2d_vec2.h>
#include <Vk2D/Vk2D_Math/vk2d_vec3.h>
#include <Vk2D/Vk2D_Math/vk2d_vec4.h>
#include <volk.h>

typedef struct vk2d_vertex vk2d_vertex;
struct vk2d_vertex
{
    vk2d_vec3 position;
    vk2d_vec4 color;
    vk2d_vec2 tex_coords;
    i32 tex_index;
};

typedef struct vk2d_vbuffer vk2d_vbuffer;
struct vk2d_vbuffer
{
    VkBuffer buffer;
    VkDeviceMemory buffer_memory;
    u32 vertex_count;
};

VkVertexInputBindingDescription vk2d_get_binding_description();
VkVertexInputAttributeDescription* vk2d_get_attribute_descriptions();

// The actual vertex buffer code
vk2d_vbuffer* vk2d_create_vbuffer(vk2d_gpu* gpu, vk2d_device* device, vk2d_command* command, i32 listSize, void* vertices);
vk2d_vbuffer* vk2d_create_vbuffer_empty(vk2d_gpu* gpu, vk2d_device* device, vk2d_command* command, i32 listSize);
void vk2d_free_vbuffer(vk2d_vbuffer* buffer);
void vk2d_set_vbuffer_data(VkCommandBuffer cbuf, vk2d_vbuffer* buffer, i32 listSize, void* vertices);

#endif