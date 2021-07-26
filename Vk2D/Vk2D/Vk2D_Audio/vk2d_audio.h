#ifndef VK2D_AUDIO
#define VK2D_AUDIO

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Math/vk2d_vec3.h>

i32 check_al_error();
i32 check_alc_error();

i32 vk2d_audio_init();
void vk2d_audio_destroy();

void vk2d_audio_set_listener_position(vk2d_vec3 pos);
void vk2d_audio_set_listener_velocity(vk2d_vec3 vel);
void vk2d_audio_set_listener_orientation(f32* vectors);

#endif