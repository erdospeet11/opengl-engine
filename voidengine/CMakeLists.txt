cmake_minimum_required(VERSION 3.15)
project(VoidEngine VERSION 0.1.0 LANGUAGES CXX)

#set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

#include directories
include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

#engine library
add_library(voidengine STATIC ${SOURCES} ${HEADERS})

#link libraries
target_link_libraries(voidengine
    ${OPENGL_LIBRARIES}
    glfw
    glm::glm
    ${FREETYPE_LIBRARIES}
)

#examples
add_subdirectory(examples) 