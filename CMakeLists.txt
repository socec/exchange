cmake_minimum_required(VERSION 3.3)
project(exchange)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -Wfatal-errors -Werror")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)
