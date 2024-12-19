include(FetchContent)

set(FETCHCONTENT_BASE_DIR "${THIRD_PARTY_DIR}")
set(EIGEN_REQUIRED_VERSION 3.4)


add_library(eigenlib INTERFACE)
target_compile_features(eigenlib INTERFACE cxx_std_17)

find_package(
  Eigen3
  ${EIGEN_REQUIRED_VERSION}
  NO_MODULE
)

if(NOT TARGET Eigen3::Eigen)
  message("Did not find Eigen ${EIGEN_REQUIRED_VERSION} installed on the system, downloading to ${THIRD_PARTY_DIR}")

  fetchcontent_declare(
      Eigen3
      URL https://gitlab.com/libeigen/eigen/-/archive/${EIGEN_REQUIRED_VERSION}/eigen-${EIGEN_REQUIRED_VERSION}.tar.gz
  )

  fetchcontent_makeavailable(Eigen3)
endif()

target_link_libraries(eigenlib INTERFACE Eigen3::Eigen)

