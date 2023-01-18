cmake_minimum_required(VERSION 3.22.1)
project(bake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_subdirectory(src)
add_subdirectory(cli)
