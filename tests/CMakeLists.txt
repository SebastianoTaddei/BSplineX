include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tmp/_deps)
fetchcontent_declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)
fetchcontent_makeavailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

file(GLOB_RECURSE KNOTS_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/knots/*.cpp")
add_executable(test_knots ${KNOTS_TESTS})
target_link_libraries(test_knots PRIVATE bsplinex Catch2::Catch2WithMain)
catch_discover_tests(test_knots)

file(GLOB_RECURSE
  CONTROL_POINTS_TESTS
  "${CMAKE_CURRENT_SOURCE_DIR}/control_points/*.cpp"
)
add_executable(test_control_points ${CONTROL_POINTS_TESTS})
target_link_libraries(test_control_points PRIVATE
  bsplinex Catch2::Catch2WithMain
)
catch_discover_tests(test_control_points)

file(GLOB_RECURSE
  DEBOOR_TESTS
  "${CMAKE_CURRENT_SOURCE_DIR}/deboor/*.cpp"
)
add_executable(test_deboor ${DEBOOR_TESTS})
target_link_libraries(test_deboor PRIVATE
  bsplinex Catch2::Catch2WithMain
)
catch_discover_tests(test_deboor)

file(GLOB_RECURSE
  BSPLINE_TESTS
  "${CMAKE_CURRENT_SOURCE_DIR}/bspline/*.cpp"
)
add_executable(test_bspline ${BSPLINE_TESTS})
target_link_libraries(test_bspline PRIVATE
  bsplinex Catch2::Catch2WithMain
)
catch_discover_tests(test_bspline)

