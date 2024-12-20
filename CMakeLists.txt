cmake_minimum_required(VERSION 3.10)

project(BSplineX
  VERSION 0.0.0
  DESCRIPTION "A header-only library to harness the power of BSplines"
  HOMEPAGE_URL "https://github.com/SebastianoTaddei/BSplineX"
)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(BSPLINEX_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")
set(BSPLINEX_CMAKE_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(BSPLINEX_DEBUG_LOG_CALL "Debug class contructors/destructors" OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
  message("CMAKE_BUILD_TYPE not specified, defaulting to ${CMAKE_BUILD_TYPE}")
endif()

if(NOT CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/lib")
  message("CMAKE_INSTALL_PREFIX not specified, defaulting to ${CMAKE_CURRENT_SOURCE_DIR}")
endif()

include("${BSPLINEX_CMAKE_MODULES_DIR}/eigen.cmake")

add_library(BSplineX INTERFACE)

target_include_directories(BSplineX INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
target_sources(BSplineX INTERFACE ${HEADER_FILES})
target_link_libraries(BSplineX INTERFACE eigenlib)

if(BSPLINEX_DEBUG_LOG_CALL)
  target_compile_definitions(BSplineX INTERFACE -DBSPLINEX_DEBUG_LOG_CALL)
endif()

if(BUILD_TESTS)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if(BUILD_BENCHMARKS)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/benchmarks)
endif()

if(BUILD_EXAMPLES)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif()

if(MSVC)
  target_compile_options(BSplineX INTERFACE /W4 /WX)
else()
  target_compile_options(BSplineX INTERFACE -Wall -Wextra -pedantic -Werror)
endif()


install(TARGETS BSplineX
  EXPORT BSplineXTargets
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT BSplineXTargets
  FILE BSplineXTargets.cmake
  NAMESPACE BSplineX::
  DESTINATION lib/cmake/BSplineX
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  BSplineXConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/BSplineXConfig.cmake
  INSTALL_DESTINATION lib/cmake/BSplineX
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/BSplineXConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/BSplineXConfigVersion.cmake
  DESTINATION lib/cmake/BSplineX
)

