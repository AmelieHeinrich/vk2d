#include "vk2d_shader.h"
#include <Vk2D/Vk2D_Base/vk2d_file.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>

#include <stdlib.h>
#include <string.h>

vk2d_shader* vk2d_create_shader(const char* vertexPath, const char* fragmentPath)
{
    vk2d_shader* result = malloc(sizeof(vk2d_shader));

    u32 vertexSize;
    u32* vertexSource = vk2d_read_spirv(vertexPath, &vertexSize);

    u32 fragmentSize;
    u32* fragmentSource = vk2d_read_spirv(fragmentPath, &fragmentSize);

    VkDevice device = volkGetLoadedDevice();

    // Vertex shader module
    {
        VkShaderModuleCreateInfo createInfo;
        memset(&createInfo, 0, sizeof(VkShaderModuleCreateInfo));

        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = vertexSize;
        createInfo.pCode = vertexSource;

        VkResult res = vkCreateShaderModule(device, &createInfo, NULL, &result->vertex_shader);
        vk2d_assert(res == VK_SUCCESS);
    }

    // Fragment shader module
    {
        VkShaderModuleCreateInfo createInfo;
        memset(&createInfo, 0, sizeof(VkShaderModuleCreateInfo));

        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = fragmentSize;
        createInfo.pCode = fragmentSource;

        VkResult res = vkCreateShaderModule(device, &createInfo, NULL, &result->fragment_shader);
        vk2d_assert(res == VK_SUCCESS);
    }

    result->geometry_shader = VK_NULL_HANDLE;
    result->compute_shader = VK_NULL_HANDLE;
    result->tessellation_control_shader = VK_NULL_HANDLE;
    result->tessellation_evaluation_shader = VK_NULL_HANDLE;

    free(vertexSource);
    free(fragmentSource);

    return result;
}

void vk2d_make_geometry_shader(vk2d_shader* shader, const char* geometryPath)
{
    char* source = vk2d_read_file(geometryPath);

    VkDevice device = volkGetLoadedDevice();

    VkShaderModuleCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkShaderModuleCreateInfo));

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = sizeof(source) / sizeof(char);
    createInfo.pCode = (const uint32_t*)source;

    VkResult res = vkCreateShaderModule(device, &createInfo, NULL, &shader->geometry_shader);
    vk2d_assert(res == VK_SUCCESS);
}

void vk2d_make_compute_shader(vk2d_shader* shader, const char* computeShader)
{
    char* source = vk2d_read_file(computeShader);

    VkDevice device = volkGetLoadedDevice();

    VkShaderModuleCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkShaderModuleCreateInfo));

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = sizeof(source) / sizeof(char);
    createInfo.pCode = (const uint32_t*)source;

    VkResult res = vkCreateShaderModule(device, &createInfo, NULL, &shader->compute_shader);
    vk2d_assert(res == VK_SUCCESS);
}

void vk2d_make_tessellation_control_shader(vk2d_shader* shader, const char* tescFile)
{
    char* source = vk2d_read_file(tescFile);

    VkDevice device = volkGetLoadedDevice();

    VkShaderModuleCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkShaderModuleCreateInfo));

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = sizeof(source) / sizeof(char);
    createInfo.pCode = (const uint32_t*)source;

    VkResult res = vkCreateShaderModule(device, &createInfo, NULL, &shader->tessellation_control_shader);
    vk2d_assert(res == VK_SUCCESS);
}

void vk2d_make_tessellation_evaluation_shader(vk2d_shader* shader, const char* teseFile)
{
    char* source = vk2d_read_file(teseFile);

    VkDevice device = volkGetLoadedDevice();

    VkShaderModuleCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkShaderModuleCreateInfo));

    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = sizeof(source) / sizeof(char);
    createInfo.pCode = (const uint32_t*)source;

    VkResult res = vkCreateShaderModule(device, &createInfo, NULL, &shader->tessellation_evaluation_shader);
    vk2d_assert(res == VK_SUCCESS);
}

void vk2d_free_shader(vk2d_shader* shader)
{
    VkDevice device = volkGetLoadedDevice();

    if (shader->vertex_shader != VK_NULL_HANDLE)
        vkDestroyShaderModule(device, shader->vertex_shader, NULL);
    if (shader->fragment_shader != VK_NULL_HANDLE)
        vkDestroyShaderModule(device, shader->fragment_shader, NULL);
    if (shader->geometry_shader != VK_NULL_HANDLE)
        vkDestroyShaderModule(device, shader->geometry_shader, NULL);
    if (shader->compute_shader != VK_NULL_HANDLE)
        vkDestroyShaderModule(device, shader->compute_shader, NULL);
    if (shader->tessellation_control_shader != VK_NULL_HANDLE)
        vkDestroyShaderModule(device, shader->tessellation_control_shader, NULL);
    if (shader->tessellation_evaluation_shader != VK_NULL_HANDLE)
        vkDestroyShaderModule(device, shader->tessellation_evaluation_shader, NULL);

    free(shader);
}