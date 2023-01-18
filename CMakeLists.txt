cmake_minimum_required(VERSION 3.22.1)
project(Bake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(common_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
include_directories(${common_SRC_INCLUDE_DIR})
include_directories(BEFORE src)

add_subdirectory(src)
add_subdirectory(cli)
