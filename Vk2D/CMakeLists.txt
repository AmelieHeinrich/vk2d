cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE VK2D_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/Vk2D/*.h)
file(GLOB_RECURSE VK2D_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Vk2D/*.c)

add_library(Vk2D ${VK2D_HEADER} ${VK2D_SOURCE})
target_link_libraries(Vk2D PUBLIC glfw)
target_link_libraries(Vk2D PUBLIC volk)