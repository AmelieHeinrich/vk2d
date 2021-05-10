#include "vk2d_base.h"

#include <GLFW/glfw3.h>
#include <Vk2D/Vk2D_Render/vk2d_renderer.h>
#include <Vk2D/Vk2D_Audio/vk2d_audio.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>

struct vk2d_initialised_subsystems
{
    i32 video;
    i32 audio;
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

    if (flags & vk2d_init_audio)
    {  
        result = vk2d_audio_init();
        _init.video = 0;
        _init.audio = 1;
        _init.everything = 0;
    }

    if (flags & vk2d_init_everything)
    {
        _init.video = 1;
        _init.audio = 1;
        _init.everything = 1;
        result = glfwInit();
        result = vk2d_audio_init();
        return result;
    }

    return 0;
}

void vk2d_quit()
{
    if (_init.audio)
    {
        vk2d_audio_destroy();
    }
    if (_init.video)
    {
        glfwTerminate();
        return;
    }
    if (_init.everything)
    {
        vk2d_audio_destroy();
        glfwTerminate();
        return;
    }

    vk2d_log_reset();
}