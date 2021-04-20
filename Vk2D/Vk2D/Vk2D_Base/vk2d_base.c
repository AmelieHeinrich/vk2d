#include "vk2d_base.h"

#include <GLFW/glfw3.h>

i32 vk2d_init(vk2d_init_flags flags)
{
    int result;

    if (flags & vk2d_init_video)
    {
        result = glfwInit();
        return result;
    }

    if (flags & vk2d_init_everything)
    {
        result = glfwInit();
        return result;
    }
}

void vk2d_quit()
{
    glfwTerminate();
}