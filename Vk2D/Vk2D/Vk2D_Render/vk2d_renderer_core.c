#include "vk2d_renderer_core.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>

#include <vulkan/vulkan.h>
#include <volk.h>

#include <GLFW/glfw3.h>

#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <vulkan/vulkan_win32.h>
#endif
#include <GLFW/glfw3native.h>

#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_renderer_data.h>

#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])

static VkBool32 demo_check_layers(u32 check_count, char **check_names,
                                  u32 layer_count,
                                  VkLayerProperties *layers) {
    for (u32 i = 0; i < check_count; i++) {
        VkBool32 found = 0;
        for (u32 j = 0; j < layer_count; j++) {
            if (strcmp(check_names[i], layers[j].layerName) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            fprintf(stderr, "Cannot find layer: %s", check_names[i]);
            return 0;
        }
    }
    return 1;
}

i32 vk2d_init_renderer(vk2d_window* window, i32 enableDebug)
{
    _debug_enabled = enableDebug;

    // INSTANCE

    VkResult res = volkInitialize();

    _data = malloc(sizeof(vk2d_renderer_data));

    _data->instance_data.enable_extension_count = 0;
    _data->instance_data.enable_layer_count = 0;

    {
        u32 instance_extension_count = 0;
        u32 instance_layer_count = 0;
        u32 validation_layer_count = 0;
        char **instance_validation_layers = NULL;

        char *instance_validation_layers_alt1[] = {
            "VK_LAYER_KHRONOS_validation"
        };

        VkBool32 validation_found = 0;
        res = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
        vk2d_assert(!res);

        instance_validation_layers = instance_validation_layers_alt1;

        if (instance_layer_count > 0) {
            VkLayerProperties *instance_layers =
                    malloc(sizeof (VkLayerProperties) * instance_layer_count);
            res = vkEnumerateInstanceLayerProperties(&instance_layer_count,
                    instance_layers);
            vk2d_assert(!res);

            validation_found = demo_check_layers(
                    ARRAY_SIZE(instance_validation_layers_alt1),
                    instance_validation_layers, instance_layer_count,
                    instance_layers);

            if (validation_found) {
                _data->instance_data.enable_layer_count = ARRAY_SIZE(instance_validation_layers);
                _data->instance_data.enabled_layers[0] = "VK_LAYER_KHRONOS_validation";
                validation_layer_count = 1;
            }

            free(instance_layers);
        }

        VkBool32 surfaceExtFound = 0;
        VkBool32 platformSurfaceExtFound = 0;
        memset(_data->instance_data.extension_names, 0, sizeof(_data->instance_data.extension_names));
        res = vkEnumerateInstanceExtensionProperties(NULL, &instance_extension_count, NULL);
        vk2d_assert(!res);

        if (instance_extension_count > 0) {
            VkExtensionProperties *instance_extensions = malloc(sizeof(VkExtensionProperties) * instance_extension_count);
            res = vkEnumerateInstanceExtensionProperties(NULL, &instance_extension_count, instance_extensions);
            vk2d_assert(!res);

            for (u32 i = 0; i < instance_extension_count; i++) {
                if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
                    _data->instance_data.extension_names[_data->instance_data.enable_extension_count++] = VK_KHR_SURFACE_EXTENSION_NAME;
                }

                if (!strcmp(VK_KHR_WIN32_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
                    _data->instance_data.extension_names[_data->instance_data.enable_extension_count++] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
                }

                if (!strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, instance_extensions[i].extensionName)) {
                    _data->instance_data.extension_names[_data->instance_data.enable_extension_count++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
                }

                if (!strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, instance_extensions[i].extensionName)) {
                    _data->instance_data.extension_names[_data->instance_data.enable_extension_count++] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
                }

                vk2d_assert(_data->instance_data.enable_extension_count < 64);
            }

            free(instance_extensions);
        }

        VkApplicationInfo appInfo;
        memset(&appInfo, 0, sizeof(VkApplicationInfo));
        appInfo.pNext = NULL;
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vk2d App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Vk2d";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo;
        memset(&createInfo, 0, sizeof(VkInstanceCreateInfo));
        createInfo.pNext = NULL;
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = 1;
        createInfo.enabledExtensionCount = _data->instance_data.enable_extension_count;
        createInfo.ppEnabledExtensionNames = (const char *const *)_data->instance_data.extension_names;
        createInfo.ppEnabledLayerNames = (const char *const *)_data->instance_data.enabled_layers;

        res = vkCreateInstance(&createInfo, NULL, &_data->instance_data.instance);

        volkLoadInstance(_data->instance_data.instance);

        if (_debug_enabled)
        {
            vk2d_log_info("Vk2D Debug Messenger", "Created instance");
        }
    }

    // SURFACE
   
    {
        // This is probably the most dumb shit i have ever written, please end my life
        // Why does this work jesus
        #if defined(_WIN32)
        PFN_vkCreateWin32SurfaceKHR test = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(_data->instance_data.instance, "vkCreateWin32SurfaceKHR");
        vk2d_assert(test != NULL);

        VkWin32SurfaceCreateInfoKHR surface_create_info = {
            VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            NULL,
            0,
            GetModuleHandle(NULL),
            glfwGetWin32Window(window->window_pointer)
        };

        res = test(_data->instance_data.instance, &surface_create_info, NULL, &_data->surface);
        #endif

        if (_debug_enabled)
        {
            vk2d_log_info("Vk2D Debug Messenger", "Created window surface");
        }
    }

    _data->physical_device = malloc(sizeof(vk2d_gpu));
    vk2d_init_gpu(_data->physical_device, _data->instance_data.instance, _data->surface);

    if (_debug_enabled)
    {
        char dest[50] = "Using GPU with name: ";
        strcat(dest, _data->physical_device->gpu_props.deviceName);
        vk2d_log_info("Vk2D Debug Messenger", dest);
    }
    
    _data->logical_device = malloc(sizeof(vk2d_device));
    vk2d_init_device(_data->logical_device, _data->instance_data, _data->physical_device->gpu, _data->physical_device->indices);

    if (_debug_enabled)
    {
        vk2d_log_info("Vk2D Debug Messenger", "Created logical device with 2 queues");
    }

    // SWAPCHAIN
    {
        _data->swap_chain = vk2d_create_swapchain(_data->physical_device, _data->surface, window->width, window->height, 2);
    }

    if (_debug_enabled)
    {
        vk2d_log_info("Vk2D Debug Messenger", "Created swap chain with 2 buffers");
    }

    // SPRITE RENDERPASS
    {
        _data->sprite_renderpass = vk2d_create_renderpass("Sprite Render Pass", _data->swap_chain->swap_chain_image_format);
    }

    if (_debug_enabled)
    {
        vk2d_log_info("Vk2D Debug Messenger", "Created sprite render pass");
    }

    // FRAMEBUFFERS
    {
        vk2d_construct_framebuffers(_data->swap_chain, _data->sprite_renderpass);
    }

    if (_debug_enabled)
    {
        vk2d_log_info("Vk2D Debug Messenger", "Created swap chain framebuffers");
    }

    // SPRITE PIPELINE
    {
        vk2d_shader* shader = vk2d_create_shader("vk2d_shaders/vertex.spv", "vk2d_shaders/fragment.spv");
        _data->sprite_pipeline = vk2d_create_pipeline(shader, window->width, window->height, _data->sprite_renderpass);
        vk2d_free_shader(shader);
    }

    if (_debug_enabled)
    {
        vk2d_log_info("Vk2D Debug Messenger", "Created sprite graphics pipeline");
    }

    // RENDER COMMAND
    {
        _data->render_command = vk2d_create_command(_data->physical_device);
    }

    if (_debug_enabled)
    {
        vk2d_log_info("Vk2D Debug Messenger", "Created command pool and command buffers");
    }

    // SYNC OBJECTS
    {
        VkSemaphoreCreateInfo semaphoreInfo;
        memset(&semaphoreInfo, 0, sizeof(VkSemaphoreCreateInfo));
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo;
        memset(&fenceInfo, 0, sizeof(VkFenceCreateInfo));
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(_data->logical_device->device, &semaphoreInfo, NULL, &_data->image_available_semaphore) != VK_SUCCESS ||
			vkCreateSemaphore(_data->logical_device->device, &semaphoreInfo, NULL, &_data->render_finished_semaphore) != VK_SUCCESS ||
			vkCreateFence(_data->logical_device->device, &fenceInfo, NULL, &_data->fence) != VK_SUCCESS) {
			vk2d_assert(1);
		}
    }

    if (_debug_enabled)
    {
        vk2d_log_info("Vk2D Debug Messenger", "Created semaphores and fences");
    }

    int is_good = res == VK_SUCCESS;
    return is_good;
}

void vk2d_renderer_resize(u32 width, u32 height)
{
    vkDeviceWaitIdle(_data->logical_device->device);

    vk2d_free_swapchain(_data->swap_chain);
    vk2d_free_command(_data->render_command);
    vk2d_free_pipeline(_data->sprite_pipeline);
    vk2d_free_renderpass(_data->sprite_renderpass);
    
     // SWAPCHAIN
    {
        _data->swap_chain = vk2d_create_swapchain(_data->physical_device, _data->surface, width, height, 2);
    }

    if (_debug_enabled)
    {
        vk2d_log_info("Vk2D Debug Messenger", "Recreated swap chain with 2 buffers");
    }

    // SPRITE RENDERPASS
    {
        _data->sprite_renderpass = vk2d_create_renderpass("Sprite Render Pass", _data->swap_chain->swap_chain_image_format);
    }

    if (_debug_enabled)
    {
        vk2d_log_info("Vk2D Debug Messenger", "Recreated sprite render pass");
    }

    // FRAMEBUFFERS
    {
        vk2d_construct_framebuffers(_data->swap_chain, _data->sprite_renderpass);
    }

    if (_debug_enabled)
    {
        vk2d_log_info("Vk2D Debug Messenger", "Recreated swap chain framebuffers");
    }

    // SPRITE PIPELINE
    {
        vk2d_shader* shader = vk2d_create_shader("vk2d_shaders/vertex.spv", "vk2d_shaders/fragment.spv");
        _data->sprite_pipeline = vk2d_create_pipeline(shader, width, height, _data->sprite_renderpass);
        vk2d_free_shader(shader);
    }

    if (_debug_enabled)
    {
        vk2d_log_info("Vk2D Debug Messenger", "Recreated sprite graphics pipeline");
    }

    // RENDER COMMAND
    {
        _data->render_command = vk2d_create_command(_data->physical_device);
    }
}

void vk2d_debug_draw()
{
    vkWaitForFences(_data->logical_device->device, 1, &_data->fence, VK_TRUE, 1000000000);
	vkResetFences(_data->logical_device->device, 1, &_data->fence);

    u32 imageIndex;
    vkAcquireNextImageKHR(_data->logical_device->device, _data->swap_chain->handle, 1000000000, _data->image_available_semaphore, VK_NULL_HANDLE, &imageIndex);

    for (int i = 0; i < 2; i++)
    {
        vkResetCommandBuffer(_data->render_command->command_buffers[i], 0);
    
        VkCommandBuffer cbuf = _data->render_command->command_buffers[i];

        VkCommandBufferBeginInfo beginInfo;
        memset(&beginInfo, 0, sizeof(VkCommandBufferBeginInfo));
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pInheritanceInfo = NULL;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vk2d_assert(vkBeginCommandBuffer(cbuf, &beginInfo) == VK_SUCCESS);

        VkClearValue clear;

        VkClearColorValue clear_color;
        clear_color.float32[0] = 0.1;
        clear_color.float32[1] = 0.1;
        clear_color.float32[2] = 0.1;
        clear_color.float32[3] = 1.0;
        
        VkOffset2D offset;
        offset.x = 0;
        offset.y = 0;

        memset(&clear, 0, sizeof(VkClearValue));
        clear.color = clear_color;

        VkRenderPassBeginInfo renderPassInfo;
        memset(&renderPassInfo, 0, sizeof(VkRenderPassBeginInfo));
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _data->sprite_renderpass->render_pass;
		renderPassInfo.framebuffer = _data->swap_chain->swap_chain_framebuffers[imageIndex];
		renderPassInfo.renderArea.offset = offset;
		renderPassInfo.renderArea.extent = _data->swap_chain->swap_chain_extent;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clear;

        vkCmdBeginRenderPass(cbuf, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(cbuf, VK_PIPELINE_BIND_POINT_GRAPHICS, _data->sprite_pipeline->pipeline);
        vkCmdDraw(cbuf, 3, 1, 0, 0);
        vkCmdEndRenderPass(cbuf);

        vk2d_assert(vkEndCommandBuffer(cbuf) == VK_SUCCESS);
    }

    VkSubmitInfo submitInfo;
    memset(&submitInfo, 0, sizeof(VkSubmitInfo));
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {_data->image_available_semaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = _data->render_command->command_buffers;

	VkSemaphore signalSemaphores[] = { _data->render_finished_semaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(_data->logical_device->device, 1, &_data->fence);

	vk2d_assert(vkQueueSubmit(_data->logical_device->graphics_queue, 1, &submitInfo, _data->fence) == VK_SUCCESS);

	VkPresentInfoKHR presentInfo;
    memset(&presentInfo, 0, sizeof(VkPresentInfoKHR));
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { _data->swap_chain->handle };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(_data->logical_device->present_queue, &presentInfo);
}

void vk2d_shutdown_renderer()
{
    vkDeviceWaitIdle(_data->logical_device->device);

    vk2d_free_command(_data->render_command);

    vkDestroyFence(_data->logical_device->device, _data->fence, NULL);
	vkDestroySemaphore(_data->logical_device->device, _data->render_finished_semaphore, NULL);
	vkDestroySemaphore(_data->logical_device->device, _data->image_available_semaphore, NULL);

    vk2d_free_pipeline(_data->sprite_pipeline);
    vk2d_free_renderpass(_data->sprite_renderpass);
    vk2d_free_swapchain(_data->swap_chain);
    vk2d_free_device(_data->logical_device);
    free(_data->physical_device);
    vkDestroySurfaceKHR(_data->instance_data.instance, _data->surface, NULL);
    vkDestroyInstance(_data->instance_data.instance, NULL);
    free(_data);
}