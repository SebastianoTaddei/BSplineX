cmake_minimum_required(VERSION 3.15)
project(BSplineX_Python)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_BINARY_DIR}/bsplinex)

include_directories(${Python_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

set_property(SOURCE BSplineX.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE BSplineX.i PROPERTY SWIG_MODULE_NAME BSplineX)

swig_add_library(BSplineXpy
        TYPE MODULE
        LANGUAGE python
        SOURCES BSplineX.i
)

target_link_libraries(BSplineXpy PRIVATE
        Python::Module
        BSplineX::BSplineX
)

if (MSVC)
    # Disable some warnings for Visual Studio.
    # Most of them are related to the SWIG generated code.
    target_compile_options(BSplineXpy PRIVATE
            /wd4127 # conditional expression is constant
            /wd4456 # declaration of 'identifier' hides previous local declaration
            /wd4267 # conversion from 'size_t' to 'type', possible loss of data
    )
endif()

install(TARGETS BSplineXpy DESTINATION .)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/BSplineX.py DESTINATION .)