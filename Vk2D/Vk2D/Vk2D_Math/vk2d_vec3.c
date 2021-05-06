#include "vk2d_vec3.h"

#include <math.h>

vk2d_vec3 vk2d_vec3_new(f32 x, f32 y, f32 z)
{
    vk2d_vec3 res;
    res.x = x;
    res.y = y;
    res.z = z;
    return res;
}

vk2d_vec3 vk2d_vec3_identity() 
{ 
    return vk2d_vec3_new(0, 0, 0); 
}

vk2d_vec3 vk2d_vec2_scalar(f32 scalar) 
{ 
    return vk2d_vec3_new(scalar, scalar, scalar); 
}

vk2d_vec3 vk2d_vec3_add(vk2d_vec3 left, vk2d_vec3 right)
{
    return vk2d_vec3_new(left.x + right.x, left.y + right.y, left.z + right.z);
}

vk2d_vec3 vk2d_vec3_sub(vk2d_vec3 left, vk2d_vec3 right)
{
    return vk2d_vec3_new(left.x - right.x, left.y - right.y, left.z - right.z);
}

vk2d_vec3 vk2d_vec3_mul(vk2d_vec3 left, vk2d_vec3 right)
{
    return vk2d_vec3_new(left.x * right.x, left.y * right.y, left.z * right.z);
}

vk2d_vec3 vk2d_vec3_div(vk2d_vec3 left, vk2d_vec3 right)
{
    return vk2d_vec3_new(left.x / right.x, left.y / right.y, left.z / right.z);
}

vk2d_vec3 vk2d_vec3_add_scalar(vk2d_vec3 left, f32 right)
{
    return vk2d_vec3_new(left.x + right, left.y + right, left.z + right);
}

vk2d_vec3 vk2d_vec3_sub_scalar(vk2d_vec3 left, f32 right)
{
    return vk2d_vec3_new(left.x - right, left.y - right, left.z - right);
}

vk2d_vec3 vk2d_vec3_mul_scalar(vk2d_vec3 left, f32 right)
{
    return vk2d_vec3_new(left.x * right, left.y * right, left.z * right);
}

vk2d_vec3 vk2d_vec3_div_scalar(vk2d_vec3 left, f32 right)
{
    return vk2d_vec3_new(left.x / right, left.y / right, left.z / right);
}


f32 vk2d_vec3_magnitude(vk2d_vec3 vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}  

vk2d_vec3 vk2d_vec3_normalise(vk2d_vec3 vec)
{
    f32 length = vk2d_vec3_magnitude(vec);
    return vk2d_vec3_new(vec.x / length, vec.y / length, vec.z / length);
}

f32 vk2d_vec3_distance(vk2d_vec3 left, vk2d_vec3 right)
{   
    f32 a = left.x - right.x;
    f32 b = left.y - right.y;
    f32 c = left.z - right.z;

    return sqrt(a * a + b * b + c * c);
}

f32 vk2d_vec3_dot(vk2d_vec3 left, vk2d_vec3 right)
{
    return (left.x * right.x + left.y * right.y + left.z * right.z);
}

vk2d_vec3 vk2d_vec3_cross(vk2d_vec3 left, vk2d_vec3 right)
{
    return vk2d_vec3_new(left.y * right.z - left.z * right.y, left.z * right.x - left.x * right.z, left.x * right.y - left.y * right.x);
}

// BOOLEAN SHIT

i32 vk2d_vec3_eq(vk2d_vec3 left, vk2d_vec3 right)
{
    return left.x == right.x && left.y == right.y && left.z == right.z;
}

i32 vk2d_vec3_not_eq(vk2d_vec3 left, vk2d_vec3 right)
{
    return !vk2d_vec3_eq(left, right);
}

i32 vk2d_vec3_greater(vk2d_vec3 left, vk2d_vec3 right)
{
    return left.x > right.x && left.y > right.y && left.z > right.z;
}

i32 vk2d_vec3_greater_equal(vk2d_vec3 left, vk2d_vec3 right)
{
    return left.x >= right.x && left.y >= right.y && left.z >= right.z;
}

i32 vk2d_vec3_less(vk2d_vec3 left, vk2d_vec3 right)
{
    return left.x < right.x && left.y < right.y && left.z < right.z;
}

i32 vk2d_vec3_less_equal(vk2d_vec3 left, vk2d_vec3 right)
{
    return left.x <= right.x && left.y <= right.y && left.z <= right.z;
}
