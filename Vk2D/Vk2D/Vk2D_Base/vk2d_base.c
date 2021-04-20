#include "vk2d_base.h"

#include <GLFW/glfw3.h>
#include <Vk2D/Vk2D_Render/vk2d_renderer_core.h>

struct vk2d_initialised_subsystems
{
    i32 video;
    i32 everything;
};

static struct vk2d_initialised_subsystems _init;

i32 vk2d_init(vk2d_init_flags flags)
{
    i32 result;

    if (flags & vk2d_init_video)
    {
        result = glfwInit();
        _init.video = 1;
        _init.everything = 0;
        return result;
    }

    if (flags & vk2d_init_everything)
    {
        _init.video = 1;
        _init.everything = 1;
        result = glfwInit();
        return result;
    }
}

void vk2d_quit()
{
    if (_init.video)
    {
        glfwTerminate();
        return;
    }
    if (_init.everything)
    {
        glfwTerminate();
        return;
    }
}