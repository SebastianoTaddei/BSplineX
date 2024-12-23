cmake_minimum_required(VERSION 3.14)
project(BSplineX_Python)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
find_package(Python COMPONENTS Interpreter Development REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_BINARY_DIR}/bsplinex)

include_directories(${Python_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

set_property(SOURCE BSplineX.i PROPERTY CPLUSPLUS ON)
#set_property(SOURCE BSplineX.i PROPERTY SWIG_MODULE_NAME BSplineX)

swig_add_library(BSplineXpy
        TYPE MODULE
        LANGUAGE python
        SOURCES BSplineX.i
)

swig_link_libraries(BSplineXpy
        Python::Python
        BSplineX::BSplineX
)

install(TARGETS BSplineXpy DESTINATION .)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/BSplineX.py DESTINATION .)