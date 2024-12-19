include(FetchContent)

set(FETCHCONTENT_BASE_DIR "${THIRD_PARTY_DIR}")
set(CATCH2_REQUIRED_VERSION 3.7.1)


add_library(catchlib INTERFACE)


find_package(
  Catch2
  ${CATCH2_REQUIRED_VERSION}
  NO_MODULE
)

if(NOT TARGET Catch2::Catch2WithMain)
  message("Did not find Catch2 ${CATCH2_REQUIRED_VERSION} installed on the system, downloading to ${THIRD_PARTY_DIR}")

  fetchcontent_declare(
      Catch2
      URL https://github.com/catchorg/Catch2/archive/refs/tags/v${CATCH2_REQUIRED_VERSION}.tar.gz
  )

  fetchcontent_makeavailable(Catch2)
endif()

target_link_libraries(catchlib INTERFACE Catch2::Catch2WithMain)

