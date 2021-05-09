#ifndef VK2D_RENDERER_CORE
#define VK2D_RENDERER_CORE

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Window/vk2d_window.h>
#include <Vk2D/Vk2D_Math/vk2d_mat4.h>
#include <Vk2D/Vk2D_Math/vk2d_vec4.h>
#include <Vk2D/Vk2D_Render/vk2d_texture.h>

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

/// Resizes the renderer
void vk2d_renderer_resize(u32 width, u32 height);

void vk2d_renderer_begin_scene(vk2d_mat4 projection, vk2d_mat4 view);
void vk2d_renderer_draw_quad_mat4(vk2d_mat4 transform, vk2d_vec4 color);
void vk2d_renderer_draw_quad(vk2d_vec3 position, vk2d_vec3 scale, vk2d_vec3 rotation, f32 angle, vk2d_vec4 color);
void vk2d_renderer_end_scene();
void vk2d_renderer_draw();

// Asset managing
vk2d_texture* vk2d_texture_init_from_file(const char* path);
void vk2d_texture_free(vk2d_texture* texture);

static i32 _debug_enabled;

#endif