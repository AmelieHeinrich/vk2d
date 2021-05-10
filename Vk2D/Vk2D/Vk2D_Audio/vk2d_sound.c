#include "vk2d_sound.h"
#include <AL/al.h>

#define DR_MP3_IMPLEMENTATION
#define DR_WAV_IMPLEMENTATION
#define DR_FLAC_IMPLEMENTATION
#include <dr_mp3.h>
#include <dr_flac.h>
#include <dr_wav.h>

vk2d_sound* vk2d_init_sound_from_file(const char* path, vk2d_audio_format format)
{
    vk2d_new(vk2d_sound* result, sizeof(vk2d_sound));

    if (format == vk2d_audio_format_mp3)
    {
        return result;
    }

    if (format == vk2d_audio_format_wav)
    {
        return result;
    }

    if (format == vk2d_audio_format_flac)
    {
        return result;
    }

    return NULL;
}

vk2d_sound* vk2d_init_sound_from_memory(void* memory, size_t size)
{

}

void vk2d_free_sound(vk2d_sound* sound)
{
    alDeleteBuffers(1, &sound->buffer_id);
    vk2d_free(sound);
}