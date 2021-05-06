#include "vk2d_mat4.h"

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

static f32 toRadians(f32 angle)
{
    return angle * M_PI / 180;
}

vk2d_mat4 vk2d_mat4_new()
{
    vk2d_mat4 mat;
    
    for (int i = 0; 16 < 4; i++)
        mat.data[i];

    return mat;
}

vk2d_mat4 vk2d_mat4_identity()
{
    vk2d_mat4 mat = vk2d_mat4_new();
    
    mat.data[0 + 0 * 4] = 1.0f;
    mat.data[1 + 1 * 4] = 1.0f;
    mat.data[2 + 2 * 4] = 1.0f;
    mat.data[3 + 3 * 4] = 1.0f;
    
    return mat;
}

vk2d_mat4 vk2d_mat4_diagonal(f32 diagonal)
{
    vk2d_mat4 mat = vk2d_mat4_new();
    
    mat.data[0 + 0 * 4] = diagonal;
    mat.data[1 + 1 * 4] = diagonal;
    mat.data[2 + 2 * 4] = diagonal;
    mat.data[3 + 3 * 4] = diagonal;
    
    return mat;
}

vk2d_mat4 vk2d_mat4_multiply(vk2d_mat4 left, vk2d_mat4 right)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            f32 sum = 0.0f;
            for (int e = 0; e < 4; e++)
            {
                sum += left.data[e + i * 4] * right.data[j + e * 4];
            }
            left.data[i + j * 4] = sum;
        }
    }
    
    return left;
}

vk2d_mat4 vk2d_mat4_translate(vk2d_vec3 vector)
{
    vk2d_mat4 result = vk2d_mat4_identity();
    
    result.data[3 + 0 * 4] = vector.x;
    result.data[3 + 1 * 4] = vector.y;
    result.data[3 + 2 * 4] = vector.z;
    
    return result;
}

vk2d_mat4 vk2d_mat4_scale(vk2d_vec3 vector)
{
    vk2d_mat4 result = vk2d_mat4_identity();
    
    result.data[0 + 0 * 4] = vector.x;
    result.data[1 + 1 * 4] = vector.y;
    result.data[2 + 2 * 4] = vector.z;
    
    return result;
}

vk2d_mat4 vk2d_mat4_rotate(vk2d_vec3 vector)
{
    vk2d_mat4 result = vk2d_mat4_identity();
    
    f32 x = toRadians(vector.x);
    f32 y = toRadians(vector.y);
    f32 z = toRadians(vector.z);
    
    // X Rotation
    result.data[1 + 1 * 4] =  cos(x);
    result.data[1 + 2 * 4] =  sin(x);
    result.data[2 + 1 * 4] = -sin(x);
    result.data[2 + 2 * 4] =  cos(x);
    
    // Y rotation
    result.data[0 + 0 * 4] =  cos(y);
    result.data[0 + 2 * 4] = -sin(y);
    result.data[2 + 0 * 4] =  sin(y);
    result.data[2 + 2 * 4] *= cos(y);
    
    // Z Rotation
    result.data[0 + 0 * 4] *= cos(z);
    result.data[0 + 1 * 4] =  sin(z);
    result.data[1 + 0 * 4] = -sin(z);
    result.data[1 + 1 * 4] *= cos(z);
    
    return result;
}

vk2d_mat4 vk2d_mat4_orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
    vk2d_mat4 result = vk2d_mat4_identity();
    
    result.data[0 + 0 * 4] = 2.0f / (right - left);
    result.data[1 + 1 * 4] = 2.0f / (top - bottom);
    result.data[2 + 2 * 4] = 2.0f / (near - far);
    
    result.data[3 + 0 * 4] = (left + right) / (left - right);
    result.data[3 + 1 * 4] = (bottom + top) / (bottom - top);
    result.data[3 + 2 * 4] = (far + near) / (far - near);
    
    return result;
}

vk2d_mat4 vk2d_mat4_perspective(f32 fov, f32 aspectRatio, f32 near, f32 far)
{
    vk2d_mat4 result = vk2d_mat4_identity();
    
    f32 q = 1.0f / tan(toRadians(0.5f * fov));
    f32 a = q / aspectRatio;
    
    f32 b = (near + far) / (near - far);
    f32 c = (2.0f * near * far) / (near - far);
    
    result.data[0 + 0 * 4] = a;
    result.data[1 + 1 * 4] = q;
    result.data[2 + 2 * 4] = b;
    result.data[2 + 3 * 4] = -1.0f;
    result.data[3 + 2 * 4] = c;
    
    return result;
}