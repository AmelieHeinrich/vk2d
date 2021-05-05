# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
project("Vk2D")

cmake_minimum_required (VERSION 3.8)

set(GLFW_VULKAN_STATIC ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(VK2D_INCLUDE_DIRS "Vk2D")

include_directories("ext/glfw/include")
include_directories("ext/volk")
include_directories("Vk2D")

add_subdirectory (example_app)
add_subdirectory (Vk2D)
add_subdirectory (ext/glfw)
add_subdirectory (ext/volk)

link_libraries(Vk2D glfw)
link_libraries(Vk2D volk)
link_libraries(example_app Vk2D)

target_include_directories(Vk2D PUBLIC "ext/glfw/include")
target_include_directories(Vk2D PUBLIC "ext/volk")
target_include_directories(Vk2D PUBLIC "ext/glslang/SPIRV" "ext/glslang/glslang/Public")
target_include_directories(example_app PUBLIC "Vk2D")
