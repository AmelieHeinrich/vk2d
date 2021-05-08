#include <Vk2D/Vk2d.h>
#include <stdio.h>

void resize_cb(int l, int h)
{
    vk2d_log_info("Vk2D Sandbox", "Resized window!");
}

#ifdef _DEBUG
static int debug = 1;
#else
static int debug = 0;
#endif

int main()
{
    vk2d_init(vk2d_init_everything);

    vk2d_window* window = vk2d_create_window(1280, 720, "Hello, Vk2D!");
    vk2d_event_bind("window_resize", resize_cb);

    vk2d_input_system* input = vk2d_create_input_system(window);

    vk2d_init_renderer(window, debug);

    vk2d_vec3 position = vk2d_vec3_identity();

    while (!vk2d_window_should_close(window))
    {
        vk2d_window_update(window);

        vk2d_mat4 projection = vk2d_mat4_identity();
        vk2d_mat4 view = vk2d_mat4_identity();

        if (vk2d_input_key_pressed(input, VK2D_KEY_UP))
            position.y += 0.02f;
        else if (vk2d_input_key_pressed(input, VK2D_KEY_DOWN))
            position.y -= 0.02f;
        if (vk2d_input_key_pressed(input, VK2D_KEY_RIGHT))
            position.x += 0.02f;
        else if (vk2d_input_key_pressed(input, VK2D_KEY_LEFT))
            position.x -= 0.02f;

        vk2d_renderer_begin_scene(projection, view);
        
        vk2d_renderer_draw_quad(position, vk2d_vec3_new(1.0f, 1.0f, 1.0f), vk2d_vec3_identity(), 0.0f, vk2d_vec4_new(0.8f, 0.2f, 0.3f, 1.0f));

        vk2d_renderer_end_scene();

        vk2d_renderer_draw();
    }

    vk2d_shutdown_renderer();

    vk2d_free_input_system(input);
    vk2d_quit_window(window);
    vk2d_quit();
    return 0;
}