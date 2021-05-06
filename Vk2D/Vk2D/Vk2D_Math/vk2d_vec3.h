#ifndef VK2D_VEC3_H
#define VK2D_VEC3_H

#include <Vk2D/Vk2D_Base/vk2d_base.h>

typedef struct vk2d_vec3 vk2d_vec3;
struct vk2d_vec3
{
    f32 x;
    f32 y;
    f32 z;
};

vk2d_vec3 vk2d_vec3_new(f32 x, f32 y, f32 z);
vk2d_vec3 vk2d_vec3_identity();
vk2d_vec3 vk2d_vec3_scalar(f32 scalar);

vk2d_vec3 vk2d_vec3_add(vk2d_vec3 left, vk2d_vec3 right);
vk2d_vec3 vk2d_vec3_sub(vk2d_vec3 left, vk2d_vec3 right);
vk2d_vec3 vk2d_vec3_mul(vk2d_vec3 left, vk2d_vec3 right);
vk2d_vec3 vk2d_vec3_div(vk2d_vec3 left, vk2d_vec3 right);

vk2d_vec3 vk2d_vec3_add_scalar(vk2d_vec3 left, f32 right);
vk2d_vec3 vk2d_vec3_sub_scalar(vk2d_vec3 left, f32 right);
vk2d_vec3 vk2d_vec3_mul_scalar(vk2d_vec3 left, f32 right);
vk2d_vec3 vk2d_vec3_div_scalar(vk2d_vec3 left, f32 right);

f32 vk2d_vec3_magnitude(vk2d_vec3 vec);
vk2d_vec3 vk2d_vec3_normalise(vk2d_vec3 vec);
f32 vk2d_vec3_distance(vk2d_vec3 left, vk2d_vec3 right);
f32 vk2d_vec3_dot(vk2d_vec3 left, vk2d_vec3 right);
vk2d_vec3 vk2d_vec3_cross(vk2d_vec3 left, vk2d_vec3 right);

i32 vk2d_vec3_eq(vk2d_vec3 left, vk2d_vec3 right);
i32 vk2d_vec3_not_eq(vk2d_vec3 left, vk2d_vec3 right);
i32 vk2d_vec3_greater(vk2d_vec3 left, vk2d_vec3 right);
i32 vk2d_vec3_greater_equal(vk2d_vec3 left, vk2d_vec3 right);
i32 vk2d_vec3_less(vk2d_vec3 left, vk2d_vec3 right);
i32 vk2d_vec3_less_equal(vk2d_vec3 left, vk2d_vec3 right);


#endif