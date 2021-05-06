#include "vk2d_renderpass.h"

#include <stdlib.h>
#include <string.h>

vk2d_renderpass* vk2d_create_renderpass(const char* name, VkFormat format)
{
    vk2d_new(vk2d_renderpass* result, sizeof(vk2d_renderpass));

    VkAttachmentDescription colorAttachment;
    vk2d_zero_memory(colorAttachment, sizeof(VkAttachmentDescription));
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef;
    vk2d_zero_memory(colorAttachmentRef, sizeof(VkAttachmentReference));
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass;
    vk2d_zero_memory(subpass, sizeof(VkSubpassDescription));
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency;
    vk2d_zero_memory(dependency, sizeof(VkSubpassDependency));
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo;
    vk2d_zero_memory(renderPassInfo, sizeof(VkRenderPassCreateInfo));
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VkDevice device = volkGetLoadedDevice();
    VkResult res = vkCreateRenderPass(device, &renderPassInfo, NULL, &result->render_pass);

    result->render_pass_format = format;
    result->name = name;

    return result;
}

void vk2d_free_renderpass(vk2d_renderpass* renderpass)
{
    VkDevice device = volkGetLoadedDevice();

    if (renderpass->render_pass != VK_NULL_HANDLE)
        vkDestroyRenderPass(device, renderpass->render_pass, NULL);

    vk2d_free(renderpass);
}