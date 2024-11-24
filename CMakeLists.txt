cmake_minimum_required(VERSION 3.10)

project(OpenglProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(tri triangle.cpp)

target_link_libraries(tri ${SDL2_LIBRARIES} GLEW GL)
