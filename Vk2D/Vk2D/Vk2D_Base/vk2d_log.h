#ifndef VK2D_LOG_H
#define VK2D_LOG_H

#include <Vk2D/Vk2D_Base/vk2d_base.h>

void vk2d_assert(i32 condition);
void vk2d_log_info(const char* tag, const char* message);
void vk2d_log_warn(const char* tag, const char* message);
void vk2d_log_error(const char* tag, const char* message);
void vk2d_log_fatal(const char* tag, const char* message);
void vk2d_log_flush();

#endif