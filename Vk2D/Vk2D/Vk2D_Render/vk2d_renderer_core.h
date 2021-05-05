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

/// Initialises the Vk2D Renderer
/// \param window The window that should be attached to the renderer
/// \param enableDebug If enabled, a debug messenger will be activated
i32 vk2d_init_renderer(vk2d_window* window, i32 enableDebug);

/// Destroys the Vk2D Renderer
void vk2d_shutdown_renderer();

static i32 _debug_enabled;

#endif