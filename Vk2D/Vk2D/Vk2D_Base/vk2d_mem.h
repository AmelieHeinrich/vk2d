#ifndef VK2D_MEM
#define VK2D_MEM

#include <string.h>
#include <stdlib.h>

#define vk2d_zero_memory(dst, size) memset(&dst, 0, size);
#define vk2d_zero_memory_ptr(dst, size) memset(dst, 0, size);
#define vk2d_free(dst) free(dst);
#define vk2d_new(dst, size) dst = malloc(size);

#endif