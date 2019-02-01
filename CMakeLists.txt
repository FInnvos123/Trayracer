cmake_minimum_required(VERSION 3.12)
project(Raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wpedantic -O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RAYTRACER_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.2 REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    src/
)

add_subdirectory(src)
