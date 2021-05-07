#ifndef VK2D_SPRITE
#define VK2D_SPRITE

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Math/vk2d_vec3.h>
#include <Vk2D/Vk2D_Math/vk2d_mat4.h>

typedef struct vk2d_rect vk2d_rect;
struct vk2d_rect
{
    f32 x;
    f32 y;
    f32 width;
    f32 height;
};

typedef struct vk2d_sprite vk2d_sprite;
struct vk2d_sprite
{
    vk2d_mat4 transform;
    vk2d_rect collider;
};

#endif