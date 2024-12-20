set(CATCH2_REQUIRED_VERSION 3.7.1)
cmake_policy(SET CMP0135 NEW)

if(TARGET catchlib)
  return()
endif()

add_library(catchlib INTERFACE)


set(FETCHCONTENT_BASE_DIR "${BSPLINEX_THIRD_PARTY_DIR}")
find_package(
  Catch2
  ${CATCH2_REQUIRED_VERSION}
  NO_MODULE
  QUIET
)

if(NOT TARGET Catch2::Catch2)
  # We have to do this manually since the install process of
  # Catch2 does not make the target directly discoverable by find_package
  if(EXISTS "${FETCHCONTENT_BASE_DIR}/catch2-src")
    message("Found Catch2 installed in ${FETCHCONTENT_BASE_DIR}/catch2-src")
    add_subdirectory("${FETCHCONTENT_BASE_DIR}/catch2-src"
                     "${FETCHCONTENT_BASE_DIR}/catch2-build")
  else()
    message("Did not find Catch2 ${CATCH2_REQUIRED_VERSION} installed, "
            "downloading to ${FETCHCONTENT_BASE_DIR}")
    include(FetchContent)

    fetchcontent_declare(
      Catch2
      URL https://github.com/catchorg/Catch2/archive/refs/tags/v${CATCH2_REQUIRED_VERSION}.tar.gz
    )

    fetchcontent_makeavailable(Catch2)
  endif()
else()
  get_target_property(CATCH2_INCLUDE_DIRS Catch2::Catch2 INTERFACE_INCLUDE_DIRECTORIES)
  message("Found Catch2 installed in ${Catch2_INCLUDE_DIRS}")
endif()

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

target_link_libraries(catchlib INTERFACE Catch2::Catch2WithMain)
