#include "vk2d_pipeline.h"

#include <stdlib.h>
#include <string.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_vbuffer.h>

vk2d_pipeline* vk2d_create_pipeline(vk2d_shader* shader, u32 width, u32 height, vk2d_renderpass* renderpass, VkDescriptorSetLayout descriptor)
{
    vk2d_pipeline* result = malloc(sizeof(vk2d_pipeline));

    VkPipelineShaderStageCreateInfo vertShaderStageInfo;
    vk2d_zero_memory(vertShaderStageInfo, sizeof(VkPipelineShaderStageCreateInfo));
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = shader->vertex_shader;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo;
    vk2d_zero_memory(fragShaderStageInfo, sizeof(VkPipelineShaderStageCreateInfo));
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = shader->fragment_shader;
    fragShaderStageInfo.pName = "main";

    // TODO : Add other shaders

    VkVertexInputBindingDescription description = vk2d_get_binding_description();
    VkVertexInputAttributeDescription* descriptions = vk2d_get_attribute_descriptions();

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    vk2d_zero_memory(vertexInputInfo, sizeof(VkPipelineVertexInputStateCreateInfo));
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = 4;
    vertexInputInfo.pVertexBindingDescriptions = &description;
	vertexInputInfo.pVertexAttributeDescriptions = descriptions;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly;
    vk2d_zero_memory(inputAssembly, sizeof(VkPipelineInputAssemblyStateCreateInfo));
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)width;
    viewport.height = (float)height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkExtent2D extent;
    extent.width = width;
    extent.height = height;

    VkOffset2D offset;
    offset.x = 0;
    offset.y = 0;

    VkRect2D scissor;
    scissor.offset = offset;
    scissor.extent = extent;

    VkPipelineViewportStateCreateInfo viewportState;
    vk2d_zero_memory(viewportState, sizeof(VkPipelineViewportStateCreateInfo));
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // TODO: Multiple fill modes

    VkPipelineRasterizationStateCreateInfo rasterizer;
    vk2d_zero_memory(rasterizer, sizeof(VkPipelineRasterizationStateCreateInfo));
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling;
    vk2d_zero_memory(multisampling, sizeof(VkPipelineMultisampleStateCreateInfo));
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    vk2d_zero_memory(colorBlendAttachment, sizeof(VkPipelineColorBlendAttachmentState));
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending;
    vk2d_zero_memory(colorBlending, sizeof(VkPipelineColorBlendStateCreateInfo));
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPushConstantRange range;
    vk2d_zero_memory(range, sizeof(VkPushConstantRange));
    range.offset = 0;
    range.size = sizeof(vk2d_scene_uniforms);
    range.stageFlags = VK_SHADER_STAGE_ALL;

    VkDescriptorSetLayout layouts[2] = { descriptor, descriptor };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    vk2d_zero_memory(pipelineLayoutInfo, sizeof(VkPipelineLayoutCreateInfo));
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 2;
    pipelineLayoutInfo.pSetLayouts = layouts;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &range;

    VkDevice device = volkGetLoadedDevice();
    {
        VkResult res = vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, &result->pipeline_layout);
        vk2d_assert(res == VK_SUCCESS);
    }

    VkGraphicsPipelineCreateInfo pipelineInfo;
    vk2d_zero_memory(pipelineInfo, sizeof(VkGraphicsPipelineCreateInfo));
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = result->pipeline_layout;
    pipelineInfo.renderPass = renderpass->render_pass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    {
        VkResult res = vkCreateGraphicsPipelines(device, NULL, 1, &pipelineInfo, NULL, &result->pipeline);
        vk2d_assert(res == VK_SUCCESS);
    }

    vk2d_free(descriptions);

    return result;
}

void vk2d_free_pipeline(vk2d_pipeline* pipeline)
{
    VkDevice device = volkGetLoadedDevice();

    if (pipeline->pipeline != VK_NULL_HANDLE)
        vkDestroyPipeline(device, pipeline->pipeline, NULL);

    vkDestroyPipelineLayout(device, pipeline->pipeline_layout, NULL);
    
    vk2d_free(pipeline);
}