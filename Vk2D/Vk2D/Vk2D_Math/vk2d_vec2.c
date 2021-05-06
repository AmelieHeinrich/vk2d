#include "vk2d_vec2.h"

#include <math.h>

vk2d_vec2 vk2d_vec2_new(f32 x, f32 y)
{
    vk2d_vec2 res;
    res.x = x;
    res.y = y;
    return res;
}

vk2d_vec2 vk2d_vec2_identity() 
{ 
    return vk2d_vec2_new(0, 0); 
}

vk2d_vec2 vk2d_vec2_scalar(f32 scalar) 
{ 
    return vk2d_vec2_new(scalar, scalar); 
}

vk2d_vec2 vk2d_vec2_add(vk2d_vec2 left, vk2d_vec2 right)
{
    return vk2d_vec2_new(left.x + right.x, left.y + right.y);
}

vk2d_vec2 vk2d_vec2_sub(vk2d_vec2 left, vk2d_vec2 right)
{
    return vk2d_vec2_new(left.x - right.x, left.y - right.y);
}

vk2d_vec2 vk2d_vec2_mul(vk2d_vec2 left, vk2d_vec2 right)
{
    return vk2d_vec2_new(left.x * right.x, left.y * right.y);
}

vk2d_vec2 vk2d_vec2_div(vk2d_vec2 left, vk2d_vec2 right)
{
    return vk2d_vec2_new(left.x / right.x, left.y / right.y);
}

vk2d_vec2 vk2d_vec2_add_scalar(vk2d_vec2 left, f32 right)
{
    return vk2d_vec2_new(left.x + right, left.y + right);
}

vk2d_vec2 vk2d_vec2_sub_scalar(vk2d_vec2 left, f32 right)
{
    return vk2d_vec2_new(left.x - right, left.y - right);
}

vk2d_vec2 vk2d_vec2_mul_scalar(vk2d_vec2 left, f32 right)
{
    return vk2d_vec2_new(left.x * right, left.y * right);
}

vk2d_vec2 vk2d_vec2_div_scalar(vk2d_vec2 left, f32 right)
{
    return vk2d_vec2_new(left.x / right, left.y / right);
}


f32 vk2d_vec2_magnitude(vk2d_vec2 vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}  

vk2d_vec2 vk2d_vec2_normalise(vk2d_vec2 vec)
{
    f32 length = vk2d_vec2_magnitude(vec);
    return vk2d_vec2_new(vec.x / length, vec.y / length);
}

f32 vk2d_vec2_distance(vk2d_vec2 left, vk2d_vec2 right)
{   
    f32 a = left.x - right.x;
    f32 b = left.y - right.y;

    return sqrt(a * a + b * b);
}

f32 vk2d_vec2_dot(vk2d_vec2 left, vk2d_vec2 right)
{
    return (left.x * right.x + left.y * right.y);
}

// BOOLEAN SHIT

i32 vk2d_vec2_eq(vk2d_vec2 left, vk2d_vec2 right)
{
    return left.x == right.x && left.y == right.y;
}

i32 vk2d_vec2_not_eq(vk2d_vec2 left, vk2d_vec2 right)
{
    return !vk2d_vec2_eq(left, right);
}

i32 vk2d_vec2_greater(vk2d_vec2 left, vk2d_vec2 right)
{
    return left.x > right.x && left.y > right.y;
}

i32 vk2d_vec2_greater_equal(vk2d_vec2 left, vk2d_vec2 right)
{
    return left.x >= right.x && left.y >= right.y;
}

i32 vk2d_vec2_less(vk2d_vec2 left, vk2d_vec2 right)
{
    return left.x < right.x && left.y < right.y;
}

i32 vk2d_vec2_less_equal(vk2d_vec2 left, vk2d_vec2 right)
{
    return left.x <= right.x && left.y <= right.y;
}
