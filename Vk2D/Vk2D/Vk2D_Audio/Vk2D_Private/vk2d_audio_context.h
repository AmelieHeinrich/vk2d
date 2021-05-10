#ifndef VK2D_AUDIO_CTX
#define VK2D_AUDIO_CTX

#include <AL/al.h>
#include <AL/alc.h>

typedef struct vk2d_audio_ctx vk2d_audio_ctx;
struct vk2d_audio_ctx
{
    ALCdevice* device;
    ALCcontext* alc_ctx;
};  

static vk2d_audio_ctx* _audio_ctx;

#endif