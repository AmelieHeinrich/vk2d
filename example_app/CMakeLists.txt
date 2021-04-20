# CMakeList.txt : CMake project for example_app, include source
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (example_app "main.c")

target_include_directories(example_app PUBLIC "Vk2D")
target_link_directories(example_app PUBLIC Vk2D)
target_link_libraries(example_app PUBLIC Vk2D)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
