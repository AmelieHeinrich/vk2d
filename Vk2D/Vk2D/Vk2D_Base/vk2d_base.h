#ifndef VK2D_BASE_H
#define VK2D_BASE_H

// typedefs
typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef float f32;
typedef double d32;

typedef enum vk2d_init_flags
{
    vk2d_init_video,
    vk2d_init_everything
} vk2d_init_flags;

i32 vk2d_init(vk2d_init_flags flags);
void vk2d_quit();

#endif