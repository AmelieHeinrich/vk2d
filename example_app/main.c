#include <Vk2D/Vk2d.h>

#ifdef _DEBUG
static int debug = 1;
#else
static int debug = 0;
#endif

int main()
{
    vk2d_init(vk2d_init_everything);

    vk2d_window* window = vk2d_create_window(1280, 720, "Hello, Vk2D!");
    vk2d_input_system* input = vk2d_create_input_system(window);

    vk2d_init_renderer(window, debug);

    vk2d_texture* c_language = vk2d_texture_init_from_file("assets/fawful.png");
    
    int i = 0;
    while (!vk2d_window_should_close(window))
    {
        vk2d_window_update(window);

        vk2d_mat4 projection = vk2d_mat4_identity();
        vk2d_mat4 view = vk2d_mat4_identity();

        vk2d_renderer_begin_scene(projection, view);
        
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                vk2d_vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
                vk2d_vec3 pos = { x, y, 0.0f };
                vk2d_vec3 scale = { 0.15f, 0.15f, 1.0f };

                vk2d_renderer_draw_textured_quad(pos, scale, vk2d_vec3_new(0.0f, 0.0f, 1.0f), i * 0.2f, c_language, color);
            }
        }
        i++;

        vk2d_renderer_end_scene();

        vk2d_renderer_draw();
    }

    vk2d_texture_free(c_language);
    vk2d_shutdown_renderer();

    vk2d_free_input_system(input);
    vk2d_quit_window(window);
    vk2d_quit();
    return 0;
}