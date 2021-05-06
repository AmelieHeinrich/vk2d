#include <Vk2D/Vk2d.h>
#include <stdio.h>

void resize_cb(int l, int h)
{
    vk2d_log_info("Vk2D Sandbox", "Resized window!");
}

int main()
{
    i32 result = vk2d_init(vk2d_init_everything);
    vk2d_assert(result != 0);

    vk2d_window* window = vk2d_create_window(1280, 720, "Hello, Vk2D!");
    vk2d_event_bind("window_resize", resize_cb);
    result = vk2d_init_renderer(window, 0);
    vk2d_assert(result != 0);

    while (!vk2d_window_should_close(window))
    {
        vk2d_window_update(window);

        vk2d_debug_draw();
    }

    vk2d_shutdown_renderer();

    vk2d_quit_window(window);
    vk2d_quit();
    return 0;
}