include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../third_party)
fetchcontent_declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
  GIT_PROGRESS   TRUE
)
fetchcontent_makeavailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

# Benchmarks

file(GLOB_RECURSE
  BSPLINE_BENCHS
  "${CMAKE_CURRENT_SOURCE_DIR}/bspline/bench_*.cpp"
)
add_executable(bench_bspline ${BSPLINE_BENCHS})
target_link_libraries(bench_bspline PRIVATE
  bsplinex Catch2::Catch2WithMain
)
catch_discover_tests(bench_bspline)

