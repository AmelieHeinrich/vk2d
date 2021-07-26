#ifndef VK2D_SOUND
#define VK2D_SOUND

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Math/vk2d_vec3.h>

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
    i32 source_id;
    const char* path;

    i32 pitch;
    i32 gain;
    vk2d_vec3 position;
    vk2d_vec3 velocity;
    i32 looping;
};

vk2d_sound* vk2d_init_sound_from_file(const char* path, vk2d_audio_format format);
void vk2d_update_sound_attributes(vk2d_sound* sound);
void vk2d_free_sound(vk2d_sound* sound);

void vk2d_play_sound(vk2d_sound* sound);
void vk2d_stop_sound(vk2d_sound* sound);
void vk2d_pause_sound(vk2d_sound* sound);
void vk2d_rewind_sound(vk2d_sound* sound);

#endif