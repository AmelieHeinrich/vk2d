#include <Vk2D/Vk2d.h>

#ifdef _DEBUG
static int debug = 1;
#else
static int debug = 0;
#endif

int main()
{
    // Initialises Vk2D
    vk2d_init(vk2d_init_everything);

    // Creates a window with the given width, height, and title
    vk2d_window* window = vk2d_create_window(1280, 720, "Hello, Vk2D!");
    // Creates an input system attached to a window
    vk2d_input_system* input = vk2d_create_input_system(window);

    // Initialises the Vk2D renderer
    vk2d_init_renderer(window, debug);

    // Loads a texture
    vk2d_texture* fawful_image = vk2d_texture_init_from_file("assets/fawful.png");

    int i = 0;
    while (!vk2d_window_should_close(window))
    {
        // Polls the window's events
        vk2d_window_update(window);

        // Create camera matrices
        vk2d_mat4 projection = vk2d_mat4_identity();
        vk2d_mat4 view = vk2d_mat4_identity();

        // Begins the rendering
        vk2d_renderer_begin_scene(projection, view);
        
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                vk2d_vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
                vk2d_vec3 pos = { x, y, 0.0f };
                vk2d_vec3 scale = { 0.15f, 0.15f, 1.0f };

                // Draw a grid of quads with a texture, position, scale, rotation and color
                vk2d_renderer_draw_textured_quad(pos, scale, vk2d_vec3_new(0.0f, 0.0f, 1.0f), i * 0.2f, fawful_image, color);
            }
        }
        i++;

        // End the scene
        vk2d_renderer_end_scene();

        // Present
        vk2d_renderer_draw();
    }

    // Frees the fawful texture and shutdown the renderer
    vk2d_texture_free(fawful_image);
    vk2d_shutdown_renderer();

    // Frees the input system and the window
    vk2d_free_input_system(input);
    vk2d_quit_window(window);

    // Terminate Vk2D
    vk2d_quit();
    return 0;
}