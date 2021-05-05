#ifndef VK2D_LOG_H
#define VK2D_LOG_H

#include <Vk2D/Vk2D_Base/vk2d_base.h>

/// Assertion util
/// \param condition The condition to assert.
void vk2d_assert(i32 condition);

/// Displays a green message in the console
void vk2d_log_info(const char* tag, const char* message);

/// Displays a yellow message in the console
void vk2d_log_warn(const char* tag, const char* message);

/// Displays a red message in the console
void vk2d_log_error(const char* tag, const char* message);

/// Displays a red message in the console
void vk2d_log_fatal(const char* tag, const char* message);

/// Flushes the console
void vk2d_log_flush();

/// Flushes the log color buffer
void vk2d_log_reset();

#endif