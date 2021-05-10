#include "vk2d_renderer.h"

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
#include <Vk2D/Vk2D_Render/Vk2D_Private/vk2d_texture_handler.h>

#define max_quads 20000

static const u32 max_vertices = max_quads * 4;
static const u32 max_indices  = max_quads * 6;

typedef struct vk2d_batch_data vk2d_batch_data;
struct vk2d_batch_data
{
    vk2d_vbuffer* quad_vertex_buffer;
    vk2d_ibuffer* quad_index_buffer;

    u32 quad_index_count;

    vk2d_vertex* quad_vertex_buffer_base;
    vk2d_vertex* quad_vertex_buffer_ptr;

    vk2d_vec3 quad_vertex_positions[4];
    vk2d_scene_uniforms uniforms;

    VkDescriptorPool batch_descriptor_pool;
    VkDescriptorSetLayout batch_dset_layout;
    VkDescriptorSet batch_dsets[2];

    vk2d_texture* white_texture;

    vk2d_texture* texture_slots[32];
    u32 texture_slot_index;
    i32 max_slots;

    u32 image_index;
};

static vk2d_batch_data* batch_data;

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
    VkResult res = volkInitialize();

    vk2d_new(_data, sizeof(vk2d_renderer_data));
    vk2d_new(batch_data, sizeof(vk2d_batch_data));

    // CORE
    {
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

            VkBool32 validation_found;
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
            vk2d_zero_memory_ptr(_data->instance_data.extension_names, sizeof(_data->instance_data.extension_names))
            res = vkEnumerateInstanceExtensionProperties(NULL, &instance_extension_count, NULL);
            vk2d_assert(res == VK_SUCCESS);

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
            vk2d_zero_memory(appInfo, sizeof(VkApplicationInfo));
            appInfo.pNext = NULL;
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Vk2d App";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "Vk2d";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_0;

            VkInstanceCreateInfo createInfo;
            vk2d_zero_memory(createInfo, sizeof(VkInstanceCreateInfo));
            createInfo.pNext = NULL;
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;
            
            createInfo.enabledLayerCount = 0; //1;
            createInfo.enabledExtensionCount = _data->instance_data.enable_extension_count;
            createInfo.ppEnabledExtensionNames = (const char *const *)_data->instance_data.extension_names;
            createInfo.ppEnabledLayerNames = NULL; //(const char *const *)_data->instance_data.enabled_layers;

            res = vkCreateInstance(&createInfo, NULL, &_data->instance_data.instance);
            vk2d_assert(res == VK_SUCCESS);

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

            if (test)
                res = test(_data->instance_data.instance, &surface_create_info, NULL, &_data->surface);

            vk2d_assert(res == VK_SUCCESS);
            #endif

            if (_debug_enabled)
            {
                vk2d_log_info("Vk2D Debug Messenger", "Created window surface");
            }
        }

        vk2d_new(_data->physical_device, sizeof(vk2d_gpu));
        vk2d_init_gpu(_data->physical_device, _data->instance_data.instance, _data->surface);

        if (_debug_enabled)
        {
            char dest[50] = "Using GPU with name: ";
            strcat_s(dest, sizeof(dest), _data->physical_device->gpu_props.deviceName);
            vk2d_log_info("Vk2D Debug Messenger", dest);
        }

        vk2d_new(_data->logical_device, sizeof(vk2d_device));
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

        // Descriptors
        {
            VkDescriptorPoolSize sampler_size;
            sampler_size.type = VK_DESCRIPTOR_TYPE_SAMPLER;
            sampler_size.descriptorCount = 1 * _data->swap_chain->num_buffers;

            VkDescriptorPoolSize textures_size;
            textures_size.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            textures_size.descriptorCount = 32 * _data->swap_chain->num_buffers;

            VkDescriptorPoolSize sizes[] = { sampler_size, textures_size };

            VkDescriptorPoolCreateInfo poolInfo;
            vk2d_zero_memory(poolInfo, sizeof(VkDescriptorPoolCreateInfo));
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            poolInfo.poolSizeCount = 2;
            poolInfo.pPoolSizes = sizes;
            poolInfo.maxSets = 512 * _data->swap_chain->num_buffers;        

            vk2d_assert(vkCreateDescriptorPool(_data->logical_device->device, &poolInfo, NULL, &batch_data->batch_descriptor_pool) == VK_SUCCESS);

            VkDescriptorSetLayoutBinding samplerLayoutBinding;
            vk2d_zero_memory(samplerLayoutBinding, sizeof(VkDescriptorSetLayoutBinding));
            samplerLayoutBinding.binding = 0;
            samplerLayoutBinding.descriptorCount = 1;
            samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
            samplerLayoutBinding.pImmutableSamplers = NULL;
            samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;   
            
            VkDescriptorSetLayoutBinding textureLayoutBinding;
            vk2d_zero_memory(textureLayoutBinding, sizeof(VkDescriptorSetLayoutBinding));
            textureLayoutBinding.binding = 1;
            textureLayoutBinding.descriptorCount = 32;
            textureLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            textureLayoutBinding.pImmutableSamplers = NULL;
            textureLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;   

            VkDescriptorSetLayoutBinding bindings[] = { samplerLayoutBinding, textureLayoutBinding };

            VkDescriptorSetLayoutCreateInfo layoutInfo;
            vk2d_zero_memory(layoutInfo, sizeof(VkDescriptorSetLayoutCreateInfo));
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = 2;
            layoutInfo.pBindings = bindings;

            vk2d_assert(vkCreateDescriptorSetLayout(_data->logical_device->device, &layoutInfo, NULL, &batch_data->batch_dset_layout) == VK_SUCCESS);      

            VkDescriptorSetLayout layouts[2];
            layouts[0] = batch_data->batch_dset_layout;
            layouts[1] = batch_data->batch_dset_layout;

            VkDescriptorSetAllocateInfo allocInfo;
            vk2d_zero_memory(allocInfo, sizeof(VkDescriptorSetAllocateInfo))
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = batch_data->batch_descriptor_pool;
            allocInfo.descriptorSetCount = _data->swap_chain->num_buffers;
            allocInfo.pSetLayouts = layouts;

            vk2d_assert(vkAllocateDescriptorSets(_data->logical_device->device, &allocInfo, batch_data->batch_dsets) == VK_SUCCESS);
        }

        // SPRITE PIPELINE
        {
            vk2d_shader* shader = vk2d_create_shader("vk2d_shaders/vertex.spv", "vk2d_shaders/fragment.spv");
            _data->sprite_pipeline = vk2d_create_pipeline(shader, window->width, window->height, _data->sprite_renderpass, batch_data->batch_dset_layout);
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
            vk2d_zero_memory(semaphoreInfo, sizeof(VkSemaphoreCreateInfo));
	    	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	    	VkFenceCreateInfo fenceInfo;
            vk2d_zero_memory(fenceInfo, sizeof(VkFenceCreateInfo));
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
    }

    // BATCH
    {
        batch_data->quad_vertex_buffer = vk2d_create_vbuffer_empty(_data->physical_device, _data->logical_device, _data->render_command, 
                                                             max_quads * sizeof(vk2d_vertex));

        vk2d_new(batch_data->quad_vertex_buffer_base, sizeof(vk2d_vertex) * max_vertices);    
        vk2d_new(u32* quadIndices, sizeof(u32) * max_indices);

        u32 offset = 0;
        if (quadIndices)
        {
            for (u32 i = 0; i < max_indices; i += 6)
            {
                quadIndices[i + 0] = offset + 0;
                quadIndices[i + 1] = offset + 1;
                quadIndices[i + 2] = offset + 2;

                quadIndices[i + 3] = offset + 2;
                quadIndices[i + 4] = offset + 3;
                quadIndices[i + 5] = offset + 0;

                offset += 4;
            }
        }

        batch_data->quad_index_buffer = vk2d_create_ibuffer(_data->physical_device, _data->logical_device, _data->render_command, 
                                                            max_indices, quadIndices);      

        vk2d_free(quadIndices);    

        batch_data->quad_vertex_positions[0] = vk2d_vec3_new(-0.5f, -0.5f, 0.0f);     
        batch_data->quad_vertex_positions[1] = vk2d_vec3_new( 0.5f, -0.5f, 0.0f);   
        batch_data->quad_vertex_positions[2] = vk2d_vec3_new( 0.5f,  0.5f, 0.0f);   
        batch_data->quad_vertex_positions[3] = vk2d_vec3_new(-0.5f,  0.5f, 0.0f);    

        batch_data->max_slots = 32;

        u32 white = 0xffffffff;
        batch_data->white_texture = vk2d_texture_init_from_raw_data(&white);

        batch_data->texture_slots[0] = batch_data->white_texture;
        batch_data->texture_slot_index = 1;
    }

    int is_good = res == VK_SUCCESS;
    return is_good;
}



void vk2d_renderer_begin_scene(vk2d_mat4 projection, vk2d_mat4 view)
{
    batch_data->uniforms.projection = projection;
    batch_data->uniforms.projection.data[1 + 1 * 4] *= -1;
    batch_data->uniforms.view = view;

    batch_data->quad_index_count = 0;
	batch_data->quad_vertex_buffer_ptr = batch_data->quad_vertex_buffer_base;
    batch_data->texture_slot_index = 1;
}

void vk2d_renderer_end_scene()
{
    vkAcquireNextImageKHR(_data->logical_device->device, _data->swap_chain->handle, 1000000000, _data->image_available_semaphore, VK_NULL_HANDLE, &batch_data->image_index);

    vkWaitForFences(_data->logical_device->device, 1, &_data->fence, VK_TRUE, 1000000000);
    vkResetFences(_data->logical_device->device, 1, &_data->fence);

    for (int i = 0; i < _data->swap_chain->num_buffers; i++)
    {
        vkResetCommandBuffer(_data->render_command->command_buffers[i], 0);
    
        VkCommandBuffer cbuf = _data->render_command->command_buffers[i];

        VkCommandBufferBeginInfo beginInfo;
        vk2d_zero_memory(beginInfo, sizeof(VkCommandBufferBeginInfo));
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pInheritanceInfo = NULL;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vk2d_assert(vkBeginCommandBuffer(cbuf, &beginInfo) == VK_SUCCESS);

        VkClearValue clear;

        VkClearColorValue clear_color;
        clear_color.float32[0] = 0.1f;
        clear_color.float32[1] = 0.1f;
        clear_color.float32[2] = 0.1f;
        clear_color.float32[3] = 1.0f;
        
        VkOffset2D offset;
        offset.x = 0;
        offset.y = 0;

        vk2d_zero_memory(clear, sizeof(VkClearValue))
        clear.color = clear_color;

        VkRenderPassBeginInfo renderPassInfo;
        vk2d_zero_memory(renderPassInfo, sizeof(VkRenderPassBeginInfo))
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _data->sprite_renderpass->render_pass;
		renderPassInfo.framebuffer = _data->swap_chain->swap_chain_framebuffers[batch_data->image_index];
		renderPassInfo.renderArea.offset = offset;
		renderPassInfo.renderArea.extent = _data->swap_chain->swap_chain_extent;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clear;

        vk2d_assert(batch_data->quad_vertex_buffer_ptr != NULL);
        vk2d_assert(batch_data->quad_vertex_buffer_base != NULL);

        u32 dataSize = (u32)((u8*)batch_data->quad_vertex_buffer_ptr - (u8*)batch_data->quad_vertex_buffer_base);
        vk2d_set_vbuffer_data(cbuf, batch_data->quad_vertex_buffer, dataSize, batch_data->quad_vertex_buffer_base);

        VkDeviceSize size = { 0 };
        VkBuffer buffers[] = { batch_data->quad_vertex_buffer->buffer };

        for (i32 i = 0; i < _data->swap_chain->num_buffers; i++)
        {
            VkDescriptorImageInfo descriptorImageInfos[32];
            vk2d_zero_memory_ptr(descriptorImageInfos, sizeof(VkDescriptorImageInfo) * 32);
    
            for (u32 j = 0; j < batch_data->texture_slot_index; j++)
            {
                if (batch_data->texture_slots[j] != NULL)
                {
                    descriptorImageInfos[j].sampler = batch_data->texture_slots[0]->private_handler->texture_sampler;
                    descriptorImageInfos[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    descriptorImageInfos[j].imageView = batch_data->texture_slots[j]->private_handler->texture_image_view;
                }
            }
    
            VkWriteDescriptorSet setWrites[2];
            vk2d_zero_memory_ptr(setWrites, sizeof(VkWriteDescriptorSet) * 2)
    
            setWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            setWrites[0].dstBinding = 0;
            setWrites[0].dstArrayElement = 0;
            setWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
            setWrites[0].descriptorCount = 1;
            setWrites[0].pBufferInfo = 0;
            setWrites[0].dstSet = batch_data->batch_dsets[i];
            setWrites[0].pImageInfo = &descriptorImageInfos[0];
    
            setWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            setWrites[1].dstBinding = 1;
            setWrites[1].dstArrayElement = 0;
            setWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            setWrites[1].descriptorCount = batch_data->texture_slot_index;
            setWrites[1].pBufferInfo = 0;
            setWrites[1].dstSet = batch_data->batch_dsets[i];
            setWrites[1].pImageInfo = descriptorImageInfos;
    
            vkUpdateDescriptorSets(_data->logical_device->device, 2, setWrites, 0, NULL);
        }

        vkCmdBeginRenderPass(cbuf, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(cbuf, VK_PIPELINE_BIND_POINT_GRAPHICS, _data->sprite_pipeline->pipeline);
        vkCmdBindVertexBuffers(cbuf, 0, 1, buffers, &size);
        vkCmdBindIndexBuffer(cbuf, batch_data->quad_index_buffer->buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(cbuf, VK_PIPELINE_BIND_POINT_GRAPHICS, _data->sprite_pipeline->pipeline_layout, 0, 1, &batch_data->batch_dsets[i], 0, NULL);

        vk2d_scene_uniforms uniforms = {0};
        uniforms.projection = batch_data->uniforms.projection;
        uniforms.view = batch_data->uniforms.view;

        vkCmdPushConstants(cbuf, _data->sprite_pipeline->pipeline_layout, VK_SHADER_STAGE_ALL, 0, sizeof(vk2d_scene_uniforms), &uniforms);
        vkCmdDrawIndexed(cbuf, batch_data->quad_index_count, 1, 0, 0, 0);

        vkCmdEndRenderPass(cbuf);

        vk2d_assert(vkEndCommandBuffer(cbuf) == VK_SUCCESS);
    }
}

void vk2d_renderer_draw()
{
    VkSubmitInfo submitInfo;
    vk2d_zero_memory(submitInfo, sizeof(VkSubmitInfo));
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
    vk2d_zero_memory(presentInfo, sizeof(VkPresentInfoKHR));
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { _data->swap_chain->handle };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &batch_data->image_index;

	vkQueuePresentKHR(_data->logical_device->present_queue, &presentInfo);
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
        _data->sprite_pipeline = vk2d_create_pipeline(shader, width, height, _data->sprite_renderpass, batch_data->batch_dset_layout);
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

void vk2d_renderer_draw_quad_mat4(vk2d_mat4 transform, vk2d_vec4 color)
{
    if (batch_data->quad_index_count >= max_indices)
    {
        vk2d_renderer_end_scene();
        batch_data->quad_index_count = 0;
	    batch_data->quad_vertex_buffer_ptr = batch_data->quad_vertex_buffer_base;
        batch_data->texture_slot_index = 1;
    }

    size_t quadVertexCount = 4;
    vk2d_vec2 texture_coords[] = {
        vk2d_vec2_new(0.0f, 0.0f), vk2d_vec2_new(1.0f, 0.0f), vk2d_vec2_new(1.0f, 1.0f), vk2d_vec2_new(0.0f, 1.0f)
    };
    i32 tex_index = 0;

    for (size_t i = 0; i < quadVertexCount; i++)
    {
        batch_data->quad_vertex_buffer_ptr->position = vk2d_mat4_multiply_v3(transform, batch_data->quad_vertex_positions[i]);
        batch_data->quad_vertex_buffer_ptr->color = color;
        batch_data->quad_vertex_buffer_ptr->tex_coords = texture_coords[i];
        batch_data->quad_vertex_buffer_ptr->tex_index = tex_index;
        batch_data->quad_vertex_buffer_ptr++;
    }

    batch_data->quad_index_count += 6;
}

void vk2d_renderer_draw_quad(vk2d_vec3 position, vk2d_vec3 scale, vk2d_vec3 rotation, f32 angle, vk2d_vec4 color)
{
    vk2d_mat4 transform = vk2d_mat4_translate(position);
    vk2d_mat4 scale_mat = vk2d_mat4_scale(scale);
    vk2d_mat4 rotation_mat = vk2d_mat4_rotate(rotation, angle);

    transform = vk2d_mat4_multiply(transform, scale_mat);
    transform = vk2d_mat4_multiply(transform, rotation_mat);

    vk2d_renderer_draw_quad_mat4(transform, color);
}

void vk2d_renderer_draw_textured_quad_mat4(vk2d_mat4 transform, vk2d_texture* texture, vk2d_vec4 color)
{
    if (batch_data->quad_index_count >= max_indices)
    {
        vk2d_renderer_end_scene();
        batch_data->quad_index_count = 0;
	    batch_data->quad_vertex_buffer_ptr = batch_data->quad_vertex_buffer_base;
        batch_data->texture_slot_index = 1;
    }

    size_t quadVertexCount = 4;
    vk2d_vec2 texture_coords[] = {
        vk2d_vec2_new(0.0f, 0.0f), vk2d_vec2_new(1.0f, 0.0f), vk2d_vec2_new(1.0f, 1.0f), vk2d_vec2_new(0.0f, 1.0f)
    };
    i32 tex_index = 0;

    for (u32 i = 1; i < batch_data->texture_slot_index; i++)
    {
        if (batch_data->texture_slots[i] == texture)
        {
            tex_index = i;
            break;
        }
    }

    if (tex_index == 0)
	{
		if (batch_data->texture_slot_index == batch_data->max_slots)
		{
            vk2d_renderer_end_scene();
            batch_data->quad_index_count = 0;
	        batch_data->quad_vertex_buffer_ptr = batch_data->quad_vertex_buffer_base;
            batch_data->texture_slot_index = 1;
        }

		tex_index = batch_data->texture_slot_index;
		batch_data->texture_slots[batch_data->texture_slot_index] = texture;
		batch_data->texture_slot_index++;
	}

    for (size_t i = 0; i < quadVertexCount; i++)
    {
        batch_data->quad_vertex_buffer_ptr->position = vk2d_mat4_multiply_v3(transform, batch_data->quad_vertex_positions[i]);
        batch_data->quad_vertex_buffer_ptr->color = color;
        batch_data->quad_vertex_buffer_ptr->tex_coords = texture_coords[i];
        batch_data->quad_vertex_buffer_ptr->tex_index = tex_index;
        batch_data->quad_vertex_buffer_ptr++;
    }

    batch_data->quad_index_count += 6;
}

void vk2d_renderer_draw_textured_quad(vk2d_vec3 position, vk2d_vec3 scale, vk2d_vec3 rotation, f32 angle, vk2d_texture* texture, vk2d_vec4 color)
{
    vk2d_mat4 transform = vk2d_mat4_translate(position);
    vk2d_mat4 scale_mat = vk2d_mat4_scale(scale);
    vk2d_mat4 rotation_mat = vk2d_mat4_rotate(rotation, angle);

    transform = vk2d_mat4_multiply(transform, scale_mat);
    transform = vk2d_mat4_multiply(transform, rotation_mat);

    vk2d_renderer_draw_textured_quad_mat4(transform, texture, color);
}

vk2d_texture* vk2d_texture_init_from_file(const char* path)
{
    vk2d_new(vk2d_texture* result, sizeof(vk2d_texture));
    if (result)
    {
        result->private_handler = vk2d_init_texture_handler(_data->physical_device, _data->logical_device, _data->render_command, path);
        result->width = result->private_handler->width;
        result->height = result->private_handler->height;
        result->path = path;
        return result;
    }
    return NULL;
}

vk2d_texture* vk2d_texture_init_from_raw_data(void* data)
{
    vk2d_new(vk2d_texture* result, sizeof(vk2d_texture));
    if (result)
    {
        result->private_handler = vk2d_init_texture_handler_raw(_data->physical_device, _data->logical_device, _data->render_command, data);
        result->width = result->private_handler->width;
        result->height = result->private_handler->height;
        result->path = "NULL_PATH";
        return result;
    }

    return NULL;
}

void vk2d_texture_free(vk2d_texture* texture)
{
    vkDeviceWaitIdle(_data->logical_device->device);

    if (texture != NULL)
    {
        vk2d_free_texture_handler(_data->logical_device, texture->private_handler);
        vk2d_free(texture);
    }
}

void vk2d_shutdown_renderer()
{
    vkDeviceWaitIdle(_data->logical_device->device);

    // BATCH
    {
        vk2d_texture_free(batch_data->white_texture);

        vk2d_free_vbuffer(batch_data->quad_vertex_buffer);
        vk2d_free_ibuffer(batch_data->quad_index_buffer);
        vk2d_free(batch_data->quad_vertex_buffer_base);
        vkFreeDescriptorSets(_data->logical_device->device, batch_data->batch_descriptor_pool, 2, batch_data->batch_dsets);
    }

    // CORE
    {
        vk2d_free_command(_data->render_command);

        vkDestroyFence(_data->logical_device->device, _data->fence, NULL);
	    vkDestroySemaphore(_data->logical_device->device, _data->render_finished_semaphore, NULL);
	    vkDestroySemaphore(_data->logical_device->device, _data->image_available_semaphore, NULL);

        vk2d_free_pipeline(_data->sprite_pipeline);
        vkDestroyDescriptorSetLayout(_data->logical_device->device, batch_data->batch_dset_layout, NULL);
        vkDestroyDescriptorPool(_data->logical_device->device, batch_data->batch_descriptor_pool, NULL);
        vk2d_free_renderpass(_data->sprite_renderpass);
        vk2d_free_swapchain(_data->swap_chain);
        vk2d_free_device(_data->logical_device);
        vk2d_free(_data->physical_device);
        vkDestroySurfaceKHR(_data->instance_data.instance, _data->surface, NULL);
        vkDestroyInstance(_data->instance_data.instance, NULL);
    }

    vk2d_free(batch_data);
    vk2d_free(_data);
}