set(EIGEN_REQUIRED_VERSION 3.4)
cmake_policy(SET CMP0135 NEW)

if(TARGET eigenlib)
  return()
endif()

add_library(eigenlib INTERFACE)
target_compile_features(eigenlib INTERFACE cxx_std_17)

set(FETCHCONTENT_BASE_DIR "${BSPLINEX_THIRD_PARTY_DIR}")
find_package(
  Eigen3
  ${EIGEN_REQUIRED_VERSION}
  NO_MODULE
  QUIET
)

if(NOT TARGET Eigen3::Eigen)
  message("Did not find Eigen ${EIGEN_REQUIRED_VERSION} installed, downloading to ${BSPLINEX_THIRD_PARTY_DIR}")
  include(FetchContent)

  fetchcontent_declare(
      Eigen3
      URL https://gitlab.com/libeigen/eigen/-/archive/${EIGEN_REQUIRED_VERSION}/eigen-${EIGEN_REQUIRED_VERSION}.tar.gz
  )

  fetchcontent_makeavailable(Eigen3)
else()
  get_target_property(EIGEN_INCLUDE_DIRS Eigen3::Eigen INTERFACE_INCLUDE_DIRECTORIES)
  message("Found Eigen installed in ${EIGEN_INCLUDE_DIRS}")
endif()

target_link_libraries(eigenlib INTERFACE Eigen3::Eigen)

