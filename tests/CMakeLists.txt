cmake_minimum_required(VERSION 3.10)

project(tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(Matplot++ REQUIRED)

add_executable(test_bspline_1d test_bspline_1d.cpp)

target_include_directories(test_bspline_1d PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(test_bspline_1d PUBLIC Matplot++::matplot)
