#ifndef VK2D_VEC4_H
#define VK2D_VEC4_H

#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef struct vk2d_vec4 vk2d_vec4;
struct vk2d_vec4
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

vk2d_vec4 vk2d_vec4_new(f32 x, f32 y, f32 z, f32 w);
vk2d_vec4 vk2d_vec4_identity();
vk2d_vec4 vk2d_vec4_scalar(f32 scalar);

vk2d_vec4 vk2d_vec4_add(vk2d_vec4 left, vk2d_vec4 right);
vk2d_vec4 vk2d_vec4_sub(vk2d_vec4 left, vk2d_vec4 right);
vk2d_vec4 vk2d_vec4_mul(vk2d_vec4 left, vk2d_vec4 right);
vk2d_vec4 vk2d_vec4_div(vk2d_vec4 left, vk2d_vec4 right);

vk2d_vec4 vk2d_vec4_add_scalar(vk2d_vec4 left, f32 right);
vk2d_vec4 vk2d_vec4_sub_scalar(vk2d_vec4 left, f32 right);
vk2d_vec4 vk2d_vec4_mul_scalar(vk2d_vec4 left, f32 right);
vk2d_vec4 vk2d_vec4_div_scalar(vk2d_vec4 left, f32 right);

f32 vk2d_vec4_dot(vk2d_vec4 left, vk2d_vec4 right);

i32 vk2d_vec4_eq(vk2d_vec4 left, vk2d_vec4 right);
i32 vk2d_vec4_not_eq(vk2d_vec4 left, vk2d_vec4 right);
i32 vk2d_vec4_greater(vk2d_vec4 left, vk2d_vec4 right);
i32 vk2d_vec4_greater_equal(vk2d_vec4 left, vk2d_vec4 right);
i32 vk2d_vec4_less(vk2d_vec4 left, vk2d_vec4 right);
i32 vk2d_vec4_less_equal(vk2d_vec4 left, vk2d_vec4 right);


#endif