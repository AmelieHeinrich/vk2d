#ifndef VK2D_INPUT_SYSTEM
#define VK2D_INPUT_SYSTEM

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Window/vk2d_window.h>

typedef struct vk2d_input_system vk2d_input_system;
struct vk2d_input_system
{
    vk2d_window* connected_window;
    i32 mouse_x;
    i32 mouse_y;
};

vk2d_input_system* vk2d_create_input_system(vk2d_window* window);
void vk2d_free_input_system(vk2d_input_system* system);
void vk2d_update_input_system(vk2d_input_system* system);

// Fetch input
i32 vk2d_input_key_pressed(vk2d_input_system* system, i32 key);
i32 vk2d_input_key_released(vk2d_input_system* system, i32 key);
i32 vk2d_input_mouse_btn_pressed(vk2d_input_system* system, i32 button);
i32 vk2d_input_mouse_btn_released(vk2d_input_system* system, i32 button);
i32 vk2d_input_get_mouse_x(vk2d_input_system* system);
i32 vk2d_input_get_mouse_y(vk2d_input_system* system);

#endif