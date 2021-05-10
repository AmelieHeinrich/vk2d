#ifndef VK2D_SOUND
#define VK2D_SOUND

#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef enum vk2d_audio_format vk2d_audio_format;
enum vk2d_audio_format
{
    vk2d_audio_format_flac,
    vk2d_audio_format_mp3,
    vk2d_audio_format_wav
    // OGG coming soon I hope
};

typedef struct vk2d_sound vk2d_sound;
struct vk2d_sound
{
    i32 buffer_id;
    const char* path;
};

vk2d_sound* vk2d_init_sound_from_file(const char* path, vk2d_audio_format format);
vk2d_sound* vk2d_init_sound_from_memory(void* memory, size_t size);
void vk2d_free_sound(vk2d_sound* sound);

#endif