#include "vk2d_sound.h"
#include <AL/al.h>

#include <Vk2D/Vk2D_Audio/vk2d_audio.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>

#define DR_MP3_IMPLEMENTATION
#define DR_WAV_IMPLEMENTATION
#define DR_FLAC_IMPLEMENTATION
#include <dr_mp3.h>
#include <dr_flac.h>
#include <dr_wav.h>

vk2d_sound* vk2d_init_sound_from_file(const char* path, vk2d_audio_format format)
{
    vk2d_new(vk2d_sound* result, sizeof(vk2d_sound));
    vk2d_zero_memory_ptr(result, sizeof(vk2d_sound));

    result->path = path;

    if (format == vk2d_audio_format_mp3)
    {
        drmp3_config config;
        i16* samples = drmp3_open_file_and_read_pcm_frames_s16(path, &config, NULL, NULL);

        if (!samples)
            vk2d_log_fatal("Vk2D Audio", "Failed to load audio file!");

        i32 al_format;
        if (config.channels == 1)
            al_format = AL_FORMAT_MONO16;
        else
            al_format = AL_FORMAT_STEREO16;

        alGenSources((ALuint)1, &result->source_id);
        vk2d_assert(check_al_error());
        alSourcef(result->source_id, AL_PITCH, 1.0f);
        alSourcef(result->source_id, AL_GAIN, 1.0f);
        alSource3f(result->source_id, AL_POSITION, 0, 0, 0);
        alSource3f(result->source_id, AL_VELOCITY, 0, 0, 0);
        alSourcei(result->source_id, AL_LOOPING, AL_FALSE);

        alGenBuffers((ALuint)1, &result->buffer_id);
        vk2d_assert(check_al_error());
        alBufferData(result->buffer_id, al_format, samples, sizeof(samples) * config.channels, config.sampleRate);
        vk2d_assert(check_al_error());

        alSourcei(result->source_id, AL_BUFFER, result->buffer_id);

        drmp3_free(samples, NULL);
    }

    if (format == vk2d_audio_format_wav)
    {
        u32 channels;
        u32 sampleRate;
        i16* samples = drwav_open_file_and_read_pcm_frames_s16(path, &channels, &sampleRate, NULL, NULL);

        i32 al_format;
        if (channels == 1)
            al_format = AL_FORMAT_MONO16;
        else
            al_format = AL_FORMAT_STEREO16;

        alGenSources(1, &result->source_id);
        vk2d_assert(check_al_error());
        alSourcef(result->source_id, AL_PITCH, 1.0f);
        alSourcef(result->source_id, AL_GAIN, 1.0f);
        alSource3f(result->source_id, AL_POSITION, 0, 0, 0);
        alSource3f(result->source_id, AL_VELOCITY, 0, 0, 0);
        alSourcei(result->source_id, AL_LOOPING, AL_FALSE);

        alGenBuffers(1, &result->buffer_id);
        vk2d_assert(check_al_error());
        alBufferData(result->buffer_id, al_format, samples, (sizeof(samples) / sizeof(samples[0])) * 2, sampleRate);
        vk2d_assert(check_al_error());

        alSourcei(result->source_id, AL_BUFFER, result->buffer_id);
        vk2d_assert(check_al_error());

        drwav_free(samples, NULL);
    }

    if (format == vk2d_audio_format_flac)
    {
        u32 channels;
        u32 sampleRate;
        i16* samples = drflac_open_file_and_read_pcm_frames_s16(path, &channels, &sampleRate, NULL, NULL);

        i32 al_format;
        if (channels == 1)
            al_format = AL_FORMAT_MONO16;
        else
            al_format = AL_FORMAT_STEREO16;

        alGenSources((ALuint)1, &result->source_id);
        vk2d_assert(check_al_error());
        alSourcef(result->source_id, AL_PITCH, 1.0f);
        alSourcef(result->source_id, AL_GAIN, 1.0f);
        alSource3f(result->source_id, AL_POSITION, 0, 0, 0);
        alSource3f(result->source_id, AL_VELOCITY, 0, 0, 0);
        alSourcei(result->source_id, AL_LOOPING, AL_FALSE);

        alGenBuffers((ALuint)1, &result->buffer_id);
        vk2d_assert(check_al_error());
        alBufferData(result->buffer_id, al_format, samples, (sizeof(samples) / sizeof(samples[0])) * 2, sampleRate);
        vk2d_assert(check_al_error());

        alSourcei(result->source_id, AL_BUFFER, result->buffer_id);

        drflac_free(samples, NULL);
    }

    return result;
}

void vk2d_update_sound_attributes(vk2d_sound* sound)
{
    alSourcef(sound->source_id, AL_PITCH, sound->pitch);
    alSourcef(sound->source_id, AL_GAIN, sound->gain);
    alSource3f(sound->source_id, AL_POSITION, sound->position.x, sound->position.y, sound->position.z);
    alSource3f(sound->source_id, AL_VELOCITY, sound->velocity.x, sound->velocity.y, sound->velocity.z);
    alSourcei(sound->source_id, AL_LOOPING, sound->looping);
}

void vk2d_free_sound(vk2d_sound* sound)
{
    alDeleteSources(1, &sound->source_id);
    alDeleteBuffers(1, &sound->buffer_id);
    vk2d_free(sound);
}

void vk2d_play_sound(vk2d_sound* sound)
{
    alSourcePlay(sound->source_id);
}

void vk2d_stop_sound(vk2d_sound* sound)
{
    alSourceStop(sound->source_id);
}

void vk2d_pause_sound(vk2d_sound* sound)
{
    alSourcePause(sound->source_id);
}

void vk2d_rewind_sound(vk2d_sound* sound)
{
    alSourceRewind(sound->source_id);
}