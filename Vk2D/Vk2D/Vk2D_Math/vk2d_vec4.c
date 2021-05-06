#include "vk2d_vec4.h"

#include <math.h>

vk2d_vec4 vk2d_vec4_new(f32 x, f32 y, f32 z, f32 w)
{
    vk2d_vec4 res;
    res.x = x;
    res.y = y;
    res.z = z;
    res.w = w;
    return res;
}

vk2d_vec4 vk2d_vec4_identity() 
{ 
    return vk2d_vec4_new(0, 0, 0, 0); 
}

vk2d_vec4 vk2d_vec4_scalar(f32 scalar) 
{ 
    return vk2d_vec4_new(scalar, scalar, scalar, scalar); 
}

vk2d_vec4 vk2d_vec4_add(vk2d_vec4 left, vk2d_vec4 right)
{
    return vk2d_vec4_new(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}

vk2d_vec4 vk2d_vec4_sub(vk2d_vec4 left, vk2d_vec4 right)
{
    return vk2d_vec4_new(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
}

vk2d_vec4 vk2d_vec4_mul(vk2d_vec4 left, vk2d_vec4 right)
{
    return vk2d_vec4_new(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
}

vk2d_vec4 vk2d_vec4_div(vk2d_vec4 left, vk2d_vec4 right)
{
    return vk2d_vec4_new(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
}

vk2d_vec4 vk2d_vec4_add_scalar(vk2d_vec4 left, f32 right)
{
    return vk2d_vec4_new(left.x + right, left.y + right, left.z + right, left.w + right);
}

vk2d_vec4 vk2d_vec4_sub_scalar(vk2d_vec4 left, f32 right)
{
    return vk2d_vec4_new(left.x - right, left.y - right, left.z - right, left.w - right);
}

vk2d_vec4 vk2d_vec4_mul_scalar(vk2d_vec4 left, f32 right)
{
    return vk2d_vec4_new(left.x * right, left.y * right, left.z * right, left.w * right);
}

vk2d_vec4 vk2d_vec4_div_scalar(vk2d_vec4 left, f32 right)
{
    return vk2d_vec4_new(left.x / right, left.y / right, left.z / right, left.w / right);
}

f32 vk2d_vec4_dot(vk2d_vec4 left, vk2d_vec4 right)
{
    return (left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w);
}

// BOOLEAN SHIT

i32 vk2d_vec4_eq(vk2d_vec4 left, vk2d_vec4 right)
{
    return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
}

i32 vk2d_vec4_not_eq(vk2d_vec4 left, vk2d_vec4 right)
{
    return !vk2d_vec4_eq(left, right);
}

i32 vk2d_vec4_greater(vk2d_vec4 left, vk2d_vec4 right)
{
    return left.x > right.x && left.y > right.y && left.z > right.z && left.w > right.w;
}

i32 vk2d_vec4_greater_equal(vk2d_vec4 left, vk2d_vec4 right)
{
    return left.x >= right.x && left.y >= right.y && left.z >= right.z && left.w >= right.w;
}

i32 vk2d_vec4_less(vk2d_vec4 left, vk2d_vec4 right)
{
    return left.x < right.x && left.y < right.y && left.z < right.z && left.w < right.w;
}

i32 vk2d_vec4_less_equal(vk2d_vec4 left, vk2d_vec4 right)
{
    return left.x <= right.x && left.y <= right.y && left.z <= right.z && left.w <= right.w;
}
