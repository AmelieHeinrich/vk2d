#ifndef VK2D_VEC2_H
#define VK2D_VEC2_H

#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef struct vk2d_vec2 vk2d_vec2;
struct vk2d_vec2
{
    f32 x;
    f32 y;
};

vk2d_vec2 vk2d_vec2_new(f32 x, f32 y);
vk2d_vec2 vk2d_vec2_identity();
vk2d_vec2 vk2d_vec2_scalar(f32 scalar);

vk2d_vec2 vk2d_vec2_add(vk2d_vec2 left, vk2d_vec2 right);
vk2d_vec2 vk2d_vec2_sub(vk2d_vec2 left, vk2d_vec2 right);
vk2d_vec2 vk2d_vec2_mul(vk2d_vec2 left, vk2d_vec2 right);
vk2d_vec2 vk2d_vec2_div(vk2d_vec2 left, vk2d_vec2 right);

vk2d_vec2 vk2d_vec2_add_scalar(vk2d_vec2 left, f32 right);
vk2d_vec2 vk2d_vec2_sub_scalar(vk2d_vec2 left, f32 right);
vk2d_vec2 vk2d_vec2_mul_scalar(vk2d_vec2 left, f32 right);
vk2d_vec2 vk2d_vec2_div_scalar(vk2d_vec2 left, f32 right);

f32 vk2d_vec2_magnitude(vk2d_vec2 vec);
vk2d_vec2 vk2d_vec2_normalise(vk2d_vec2 vec);
f32 vk2d_vec2_distance(vk2d_vec2 left, vk2d_vec2 right);
f32 vk2d_vec2_dot(vk2d_vec2 left, vk2d_vec2 right);

i32 vk2d_vec2_eq(vk2d_vec2 left, vk2d_vec2 right);
i32 vk2d_vec2_not_eq(vk2d_vec2 left, vk2d_vec2 right);
i32 vk2d_vec2_greater(vk2d_vec2 left, vk2d_vec2 right);
i32 vk2d_vec2_greater_equal(vk2d_vec2 left, vk2d_vec2 right);
i32 vk2d_vec2_less(vk2d_vec2 left, vk2d_vec2 right);
i32 vk2d_vec2_less_equal(vk2d_vec2 left, vk2d_vec2 right);


#endif