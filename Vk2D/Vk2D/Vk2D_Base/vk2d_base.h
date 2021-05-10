#ifndef VK2D_BASE_H
#define VK2D_BASE_H

#include <Vk2D/Vk2D_Base/vk2d_mem.h>
#include <stdint.h>

// typedefs
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef float f32;
typedef double d32;

typedef enum vk2d_init_flags
{
    vk2d_init_video, // Initialises only the renderer subsystem
    vk2d_init_audio, // Initialises only the audio subsystem
    vk2d_init_everything // Initialises everything
} vk2d_init_flags;

/// Initialises Vk2D
/// \param flags Subsystems to initialise
i32 vk2d_init(vk2d_init_flags flags);

/// Quits all the initialised subsystems
void vk2d_quit();

#endif