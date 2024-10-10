include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tmp/_deps)
fetchcontent_declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)
fetchcontent_makeavailable(Catch2)

file(GLOB_RECURSE KNOTS_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/knots/*.cpp")
add_executable(test_knots ${KNOTS_TESTS})
target_link_libraries(test_knots PRIVATE bsplinex Catch2::Catch2WithMain)

