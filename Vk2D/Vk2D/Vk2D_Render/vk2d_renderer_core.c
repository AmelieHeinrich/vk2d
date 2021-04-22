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
            fprintf(stderr, "Cannot find layer: %s\n", check_names[i]);
            return 0;
        }
    }
    return 1;
}

i32 vk2d_init_renderer(vk2d_window* window)
{
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
    }

    _data->physical_device = malloc(sizeof(vk2d_gpu));
    vk2d_init_gpu(_data->physical_device, _data->instance_data.instance, _data->surface);
    
    _data->logical_device = malloc(sizeof(vk2d_device));
    vk2d_init_device(_data->logical_device, _data->instance_data, _data->physical_device->gpu, _data->physical_device->indices);

    int is_good = res == VK_SUCCESS;
    return is_good;
}

void vk2d_shutdown_renderer()
{
    vk2d_free_device(_data->logical_device);
    free(_data->physical_device);
    vkDestroySurfaceKHR(_data->instance_data.instance, _data->surface, NULL);
    vkDestroyInstance(_data->instance_data.instance, NULL);
    free(_data);
}