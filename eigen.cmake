include(ExternalProject)

set(EIGEN_INSTALL_DIR "${THIRD_PARTY_DIR}/eigen/")

cmake_policy(SET CMP0135 NEW)
externalproject_add(
        eigen
        URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
        SOURCE_DIR "${THIRD_PARTY_DIR}/eigen-src"
        BINARY_DIR "${THIRD_PARTY_DIR}/eigen-build"
        INSTALL_DIR "${EIGEN_INSTALL_DIR}"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DCMAKE_BUILD_TYPE=Release
)

file(MAKE_DIRECTORY "${EIGEN_INSTALL_DIR}/include")  # avoid race condition

add_library(eigenlib INTERFACE IMPORTED GLOBAL)
add_dependencies(eigenlib eigen)
target_compile_features(eigenlib INTERFACE cxx_std_17)

set_target_properties(eigenlib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${EIGEN_INSTALL_DIR}/include/eigen3
)

