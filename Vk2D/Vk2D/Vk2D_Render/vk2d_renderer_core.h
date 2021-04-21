#ifndef VK2D_RENDERER_CORE
#define VK2D_RENDERER_CORE

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Window/vk2d_window.h>

typedef struct vk2d_gpu_info vk2d_gpu_info;
struct vk2d_gpu_info
{
    u32 apiVersion;
    u32 driverVersion;
    u32 vendorID;
    u32 deviceID;
    char deviceName[256];
    u32 pipelineCacheUUID[16];
};

i32 vk2d_init_renderer(vk2d_window* window);
void vk2d_shutdown_renderer();

#endif