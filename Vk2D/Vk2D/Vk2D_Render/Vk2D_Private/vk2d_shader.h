#ifndef VK2D_SHADER_H
#define VK2D_SHADER_H

#include <volk.h>

typedef struct vk2d_shader vk2d_shader;
struct vk2d_shader
{
    VkShaderModule vertex_shader;
    VkShaderModule fragment_shader;
    VkShaderModule geometry_shader;
    VkShaderModule compute_shader;
    VkShaderModule tessellation_control_shader;
    VkShaderModule tessellation_evaluation_shader;
};

vk2d_shader* vk2d_create_shader(const char* vertexPath, const char* fragmentPath);
void vk2d_make_geometry_shader(vk2d_shader* shader, const char* geometryPath);
void vk2d_make_compute_shader(vk2d_shader* shader, const char* computeShader);
void vk2d_make_tessellation_control_shader(vk2d_shader* shader, const char* tescFile);
void vk2d_make_tessellation_evaluation_shader(vk2d_shader* shader, const char* teseFile);

void vk2d_free_shader(vk2d_shader* shader);

#endif