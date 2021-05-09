#ifndef VK2D_TEXTURE_H
#define VK2D_TEXTURE_H

#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef struct vk2d_texture_handler vk2d_texture_handler;

typedef struct vk2d_texture vk2d_texture;
struct vk2d_texture
{
    i32 width;
    i32 height;
    const char* path;

    vk2d_texture_handler* private_handler;
};

#endif