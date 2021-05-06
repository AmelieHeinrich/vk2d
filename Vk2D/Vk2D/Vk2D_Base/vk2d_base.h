#ifndef VK2D_BASE_H
#define VK2D_BASE_H

#include <Vk2D/Vk2D_Base/vk2d_mem.h>

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
    vk2d_init_video, // Initialises only the renderer subsystem
    vk2d_init_everything // Initialises everything
} vk2d_init_flags;

/// Initialises Vk2D
/// \param flags Subsystems to initialise
i32 vk2d_init(vk2d_init_flags flags);

/// Quits all the initialised subsystems
void vk2d_quit();

#endif