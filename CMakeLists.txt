cmake_minimum_required(VERSION 3.10)

project(bsplinex
  VERSION 0.0.0
  DESCRIPTION "A header-only library to harness the power of BSplines"
  HOMEPAGE_URL "https://github.com/SebastianoTaddei/BSplineX"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

option(BUILD_TESTS "Build tests" OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

add_library(bsplinex INTERFACE)

target_include_directories(bsplinex INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
target_sources(bsplinex INTERFACE ${HEADER_FILES})

if(BUILD_TESTS)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if(MSVC)
  target_compile_options(bsplinex INTERFACE /W4 /WX)
else()
  target_compile_options(bsplinex INTERFACE -Wall -Wextra -pedantic -Werror)
endif()

