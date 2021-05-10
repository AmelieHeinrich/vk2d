cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE VK2D_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/Vk2D/*.h)
file(GLOB_RECURSE VK2D_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Vk2D/*.c)

if (NOT CMAKE_VERSION VERSION_LESS 3.7.0)
	message(STATUS "Using module to find Vulkan")
	find_package(Vulkan REQUIRED FATAL_ERROR)
endif()

IF(WIN32)
	set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
	add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
	IF (NOT Vulkan_FOUND)
		find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS ${CMAKE_SOURCE_DIR}/libs/vulkan)
		IF (Vulkan_LIBRARY)
			set(Vulkan_FOUND ON)
			MESSAGE("Using bundled Vulkan library version")
		ENDIF()
	ENDIF()
ENDIF()

IF (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
	message(STATUS ${Vulkan_LIBRARY})
ENDIF()

include_directories(${Vulkan_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext/stb_image)

add_definitions(-DVK_NO_PROTOTYPES)
add_definitions(-DGLFW_INCLUDE_NONE)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_library(Vk2D ${VK2D_HEADER} ${VK2D_SOURCE})
target_link_libraries(Vk2D PUBLIC glfw)
target_link_libraries(Vk2D PUBLIC volk)
target_link_libraries(Vk2D PUBLIC OpenAL)