#include "vk2d_audio.h"

#include <Vk2D/Vk2D_Base/vk2d_log.h>
#include <Vk2D/Vk2D_Audio/Vk2D_Private/vk2d_audio_context.h>
#include <stdio.h>

i32 check_al_error()
{
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		vk2d_log_fatal("Vk2D Audio", "OpenAL Error");
		return 0;
	}
	return 1;
}

i32 check_alc_error()
{
	ALCenum error = alcGetError(_audio_ctx->device);
	if (error != ALC_NO_ERROR)
	{
		vk2d_log_fatal("Vk2D Audio", "OpenALC Error");
		return 0;
	}

	return 1;
}

i32 vk2d_audio_init()
{
    vk2d_new(_audio_ctx, sizeof(vk2d_audio_ctx));

	_audio_ctx->device = alcOpenDevice(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
	vk2d_assert(_audio_ctx->device != NULL);
	check_alc_error();

	_audio_ctx->alc_ctx = alcCreateContext(_audio_ctx->device, NULL);
	check_alc_error();

	if (alcMakeContextCurrent(_audio_ctx->alc_ctx) != ALC_TRUE)
	{
		vk2d_log_fatal("Vk2D Audio", "Failed to make the OpenAL context current!");
		return 0;
	}

	check_alc_error();

	vk2d_assert(_audio_ctx->alc_ctx != NULL);

	return check_alc_error();
}

void vk2d_audio_destroy()
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(_audio_ctx->alc_ctx);
	alcCloseDevice(_audio_ctx->device);

	vk2d_free(_audio_ctx);
}

void vk2d_audio_set_listener_position(vk2d_vec3 pos)
{
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

void vk2d_audio_set_listener_velocity(vk2d_vec3 vel)
{
	alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
}

void vk2d_audio_set_listener_orientation(f32* vectors)
{
	alListenerfv(AL_ORIENTATION, vectors);
}
