#include "vk2d_window.h"
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>

vk2d_window* vk2d_create_window(u32 width, u32 height, const char* title)
{
    vk2d_window* result = malloc(sizeof(vk2d_window));
    result->width = width;
    result->height = height;
    result->title = title;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    result->window_pointer = glfwCreateWindow(width, height, title, NULL, NULL);

    vk2d_assert(result != NULL);

    return result;
}

i32 vk2d_window_should_close(vk2d_window* window)
{
    return glfwWindowShouldClose(window->window_pointer);
}

void vk2d_window_update(vk2d_window* window)
{
    glfwPollEvents();
}

void vk2d_quit_window(vk2d_window* window)
{
    glfwDestroyWindow(window->window_pointer);
    free(window);
}