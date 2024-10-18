cmake_minimum_required(VERSION 3.29.2 FATAL_ERROR)

set(PROJECT_NAME Battle)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/main.cpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

include_directories("X:/Programming/c++ and openGL/first game openGL/Battle/external/glfw/include")
link_directories("X:/Programming/c++ and openGL/first game openGL/Battle/external/glfw/build/src/Debug/glfw3.lib")


target_link_libraries(${PROJECT_NAME} glfw)



