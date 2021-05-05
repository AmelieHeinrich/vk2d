#ifndef VK2D_FILE
#define VK2D_FILE

#include <stdio.h>
#include <Vk2D/Vk2D_Base/vk2d_base.h>

/// Reads a file and return its content into a char buffer
/// \param path The filepath
char* vk2d_read_file(const char* path);

/// Reads a spirv binary shader and return its content into a uint32_t buffer
/// \param path The filepath
/// \param psize The shader code size
u32* vk2d_read_spirv(const char* path, u32* psize);

#endif