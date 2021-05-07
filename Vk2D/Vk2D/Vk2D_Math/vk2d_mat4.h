#ifndef VK2D_MAT4_H
#define VK2D_MAT4_H

#include <Vk2D/Vk2D_Base/vk2d_base.h>
#include <Vk2D/Vk2D_Math/vk2d_vec3.h>

typedef struct vk2d_mat4 vk2d_mat4;
struct vk2d_mat4
{
    f32 data[16];
};

vk2d_mat4 vk2d_mat4_new();
vk2d_mat4 vk2d_mat4_identity();
vk2d_mat4 vk2d_mat4_diagonal(f32 diagonal);

vk2d_mat4 vk2d_mat4_multiply(vk2d_mat4 left, vk2d_mat4 right);
vk2d_vec3 vk2d_mat4_multiply_v3(vk2d_mat4 left, vk2d_vec3 right);

// Transform stuff
vk2d_mat4 vk2d_mat4_translate(vk2d_vec3 vector);
vk2d_mat4 vk2d_mat4_scale(vk2d_vec3 vector);
vk2d_mat4 vk2d_mat4_rotate(vk2d_vec3 vector, f32 angle);

// Camera stuff
vk2d_mat4 vk2d_mat4_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
vk2d_mat4 vk2d_mat4_perspective(f32 fov, f32 aspectRatio, f32 near, f32 far);

#endif