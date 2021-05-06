#include "vk2d_window.h"
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>
#include <Vk2D/Vk2D_Render/vk2d_renderer_core.h>
#include <Vk2D/Vk2D_Base/vk2d_event.h>

static void close_callback(struct GLFWwindow* win)
{
    vk2d_event_emit("window_close", 0, 0);
}
static void resize_callback(struct GLFWwindow* win, i32 w, i32 h)
{  
    vk2d_event_emit("window_resize", w, h);
    vk2d_renderer_resize(w, h);
}
static void focus_callback(struct GLFWwindow* win, i32 thing)
{
    vk2d_event_emit("window_focus", 0, 0);
}
static void pos_callback(struct GLFWwindow* win, i32 x, i32 y)
{
    vk2d_event_emit("window_moved", x, y);
}
static void key_callback(struct GLFWwindow* win, i32 key, i32 scancode, i32 action, i32 mods)
{
    vk2d_event_emit("key_pressed", key, action);
}
static void char_callback(struct GLFWwindow* win, u32 codepoint)
{
    vk2d_event_emit("char_pressed", codepoint, 0);
}

static void close(int l, int h) {}
static void resize(int l, int h) {}
static void focus(int l, int h) {}
static void pos(int l, int h) {}
static void key(int l, int h) {}
static void char_c(int l, int h) {}

vk2d_window* vk2d_create_window(u32 width, u32 height, const char* title)
{
    vk2d_new(vk2d_window* result, sizeof(vk2d_window));
    result->width = width;
    result->height = height;
    result->title = title;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    result->window_pointer = glfwCreateWindow(width, height, title, NULL, NULL);

    vk2d_assert(result != NULL);

    vk2d_event_bind("window_close", close);
    vk2d_event_bind("window_resize", resize);
    vk2d_event_bind("window_focus", focus);
    vk2d_event_bind("window_moved", pos);
    vk2d_event_bind("key_pressed", key);
    vk2d_event_bind("char_pressed", char_c);

    glfwSetWindowCloseCallback(result->window_pointer, close_callback);
    glfwSetWindowSizeCallback(result->window_pointer, resize_callback);
    glfwSetWindowFocusCallback(result->window_pointer, focus_callback);
    glfwSetWindowPosCallback(result->window_pointer, pos_callback);
    glfwSetKeyCallback(result->window_pointer, key_callback);
    glfwSetCharCallback(result->window_pointer, char_callback);

    return result;
}

i32 vk2d_window_should_close(vk2d_window* window)
{
    return glfwWindowShouldClose(window->window_pointer);
}

void vk2d_window_update(vk2d_window* window)
{
    glfwPollEvents();
}

void vk2d_quit_window(vk2d_window* window)
{
    glfwDestroyWindow(window->window_pointer);
    vk2d_free(window);
}