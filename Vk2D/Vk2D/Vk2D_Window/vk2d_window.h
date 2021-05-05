#ifndef VK2D_WINDOW_H
#define VK2D_WINDOW_H

#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef struct GLFWwindow GLFWwindow;

// Abstract object that wrapps a GLFWwindow.
typedef struct vk2d_window
{
    u32 width; // The width of the window
    u32 height; // The height of the window
    const char* title; // The title of the window
    GLFWwindow* window_pointer; // The GLFW window pointer
} vk2d_window;

/// Initialises a vk2d_window
/// \param width The desired width
/// \param height The desired height
/// \param title The desired title
vk2d_window* vk2d_create_window(u32 width, u32 height, const char* title);

/// Checks if the window is open
/// \param window The target window
i32 vk2d_window_should_close(vk2d_window* window);

/// Updates the window
/// \param window The target window
void vk2d_window_update(vk2d_window* window);

/// Destroys the window
/// \param window The target window
void vk2d_quit_window(vk2d_window* window);

#endif