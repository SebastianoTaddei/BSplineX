include(ExternalProject)

set(EIGEN_INSTALL_DIR "${THIRD_PARTY_DIR}/eigen/")
set(EIGEN_REQUIRED_VERSION 3.4)

cmake_policy(SET CMP0135 NEW)

add_library(eigenlib INTERFACE)
target_compile_features(eigenlib INTERFACE cxx_std_17)

# Option 1: Try to find Eigen installed on the system
find_package(
  Eigen3
  ${EIGEN_REQUIRED_VERSION}
  NO_MODULE
  PATHS "${EIGEN_INSTALL_DIR}"
)

if(NOT TARGET Eigen3::Eigen)
  message("Did not find Eigen ${EIGEN_REQUIRED_VERSION} installed on the system, downloading to ${EIGEN_INSTALL_DIR}")
  externalproject_add(
      Eigen3
      URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
      PREFIX "${EIGEN_INSTALL_DIR}"
      DOWNLOAD_EXTRACT_TIMESTAMP TRUE
      CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
      -DCMAKE_BUILD_TYPE=Release
      -DBUILD_TESTING=OFF
  )
  file(MAKE_DIRECTORY "${EIGEN_INSTALL_DIR}/include") # avoid race condition
  find_package(
    Eigen3
    ${EIGEN_REQUIRED_VERSION}
    NO_MODULE
    PATHS "${EIGEN_INSTALL_DIR}"
  )
endif()

target_link_libraries(eigenlib INTERFACE Eigen3::Eigen)

