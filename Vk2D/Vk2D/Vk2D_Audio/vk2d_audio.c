#include "vk2d_audio.h"

#include <Vk2D/Vk2D_Base/vk2d_log.h>
#include <Vk2D/Vk2D_Audio/Vk2D_Private/vk2d_audio_context.h>

i32 check_al_error()
{
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		switch (error)
		{
		case AL_INVALID_NAME:
			vk2d_log_fatal("Vk2D Audio", "OpenAL Error (AL_INVALID_NAME) : A bad name (ID) was passed to an OpenAL function.");
			break;
		case AL_INVALID_ENUM:
			vk2d_log_fatal("Vk2D Audio", "OpenAL Error (AL_INVALID_ENUM) : An invalid enum was passed to an OpenAL function.");
			break;
		case AL_INVALID_VALUE:
			vk2d_log_fatal("Vk2D Audio", "OpenAL Error (AL_INVALID_VALUE) : An invalid value was passed to an OpenAL function.");
			break;
		case AL_INVALID_OPERATION:
			vk2d_log_fatal("Vk2D Audio", "OpenAL Error (AL_INVALID_OPERATION) : The requested OpenAL operation is not valid.");
			break;
		case AL_OUT_OF_MEMORY:
			vk2d_log_fatal("Vk2D Audio", "OpenAL Error (AL_OUT_OF_MEMORY) : OpenAL memory is full!");
			break;
		default:
			vk2d_log_fatal("Vk2D Audio", "OpenAL Error (Unknow OpenAL Error)");
		}
		return 0;
	}
	return 1;
}

i32 check_alc_error()
{
	ALCcontext* ctx = alcGetCurrentContext();
	ALCdevice* device = alcGetContextsDevice(ctx);

	ALCenum error = alcGetError(device);
	if (error != ALC_NO_ERROR)
	{
		switch (error)
		{
		case ALC_INVALID_ENUM:
			vk2d_log_fatal("Vk2D Audio", "OpenAL Error (ALC_INVALID_ENUM) : An invalid enum was passed to an OpenAL function.");
			break;
		case ALC_INVALID_VALUE:
			vk2d_log_fatal("Vk2D Audio", "OpenAL Error (ALC_INVALID_VALUE) : An invalid value was passed to an OpenAL function.");
			break;
		case ALC_INVALID_DEVICE:
			vk2d_log_fatal("Vk2D Audio", "OpenAL Error (ALC_INVALID_DEVICE) : An invalid device was passed to an OpenAL function.");
			break;
		case ALC_INVALID_CONTEXT:
			vk2d_log_fatal("Vk2D Audio", "OpenAL Error (ALC_INVALID_CONTEXT) : An invalid context was passed to an OpenAL function.");
			break;
		case ALC_OUT_OF_MEMORY:
			vk2d_log_fatal("Vk2D Audio", "OpenAL Error (ALC_OUT_OF_MEMORY) : OpenAL memory is full!");
			break;
		default:
			vk2d_log_fatal("Vk2D Audio", "OpenAL Error (Unknow OpenALC Error)");
		}

		return 0;
	}

	return 1;
}

i32 vk2d_audio_init()
{
    vk2d_new(_audio_ctx, sizeof(vk2d_audio_ctx));

	if (_audio_ctx)
	{
		const ALCchar* defaultDeviceString = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

		_audio_ctx->device = alcOpenDevice(defaultDeviceString);
		vk2d_assert(_audio_ctx->device != NULL);

		_audio_ctx->alc_ctx = alcCreateContext(_audio_ctx->device, NULL);

		if (!alcMakeContextCurrent(_audio_ctx->alc_ctx))
		{
			vk2d_log_fatal("Vk2D Audio", "Failed to make the OpenAL context current!");
			return 0;
		}
	}

	return check_alc_error();
}

void vk2d_audio_destroy()
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(_audio_ctx->alc_ctx);
	alcCloseDevice(_audio_ctx->device);

	vk2d_free(_audio_ctx);
}