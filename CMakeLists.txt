project (snake)
cmake_minimum_required (VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#include SDL2
include (FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(src)

add_executable (snake src/game.cpp src/main.cpp)
target_link_libraries(snake ${SDL2_LIBRARIES})

