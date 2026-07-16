cmake_minimum_required(VERSION 3.10)

project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(OpenGLProject
    src/main.cpp
    src/glad.c
)

target_include_directories(OpenGLProject
    PRIVATE include
)

target_link_libraries(OpenGLProject
    glfw
    OpenGL::GL
)