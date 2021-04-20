#ifndef VK2D_WINDOW_H
#define VK2D_WINDOW_H

#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef struct GLFWwindow GLFWwindow;

typedef struct vk2d_window
{
    u32 width;
    u32 height;
    const char* title;
    GLFWwindow* window_pointer;
} vk2d_window;

vk2d_window* vk2d_create_window(u32 width, u32 height, const char* title);
i32 vk2d_window_should_close(vk2d_window* window);
void vk2d_window_update(vk2d_window* window);
void vk2d_quit_window(vk2d_window* window);

#endif