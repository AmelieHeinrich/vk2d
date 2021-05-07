#include "vk2d_input_system.h"
#include <GLFW/glfw3.h>

vk2d_input_system* vk2d_create_input_system(vk2d_window* window)
{
    vk2d_new(vk2d_input_system* result, sizeof(vk2d_input_system));

    result->connected_window = window;

    return result;
}

void vk2d_free_input_system(vk2d_input_system* system)
{
    vk2d_free(system);
}

void vk2d_update_input_system(vk2d_input_system* system)
{
    // TODO: Action system
}   

i32 vk2d_input_key_pressed(vk2d_input_system* system, i32 key)
{
    return glfwGetKey(system->connected_window->window_pointer, key) == GLFW_PRESS;
}

i32 vk2d_input_key_released(vk2d_input_system* system, i32 key)
{
    return !vk2d_input_key_pressed(system, key);
}

i32 vk2d_input_mouse_btn_pressed(vk2d_input_system* system, i32 button)
{
    return glfwGetMouseButton(system->connected_window->window_pointer, button) == GLFW_PRESS;
}

i32 vk2d_input_mouse_btn_released(vk2d_input_system* system, i32 button)
{
    return !vk2d_input_mouse_btn_pressed(system, button);
}

i32 vk2d_input_get_mouse_x(vk2d_input_system* system)
{
    d32 x;
    d32 y;

    glfwGetCursorPos(system->connected_window->window_pointer, &x, &y);

    system->mouse_x = (i32)x;
    system->mouse_y = (i32)y;

    return system->mouse_x;
}

i32 vk2d_input_get_mouse_y(vk2d_input_system* system)
{   
    d32 x;
    d32 y;

    glfwGetCursorPos(system->connected_window->window_pointer, &x, &y);

    system->mouse_x = (i32)x;
    system->mouse_y = (i32)y;

    return system->mouse_y;
}