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
        return result;
    }

    if (flags & vk2d_init_everything)
    {
        _init.video = 1;
        _init.audio = 1;
        _init.everything = 1;
        result = glfwInit();
        vk2d_assert(result == GLFW_TRUE);
        vk2d_audio_init();

        vk2d_audio_set_listener_position(vk2d_vec3_identity());
        vk2d_audio_set_listener_velocity(vk2d_vec3_identity());
        f32 forwardAndUpVectors[] = {
			/*forward = */ 1.f, 0.f, 0.f,
			/* up = */ 0.f, 1.f, 0.f
		};
        vk2d_audio_set_listener_orientation(forwardAndUpVectors);

        check_al_error();

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