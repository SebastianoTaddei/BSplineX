cmake_minimum_required(VERSION 3.10)

project(bsplinex)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_library(bsplinex INTERFACE)

target_include_directories(bsplinex INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
