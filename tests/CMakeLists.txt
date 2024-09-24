find_package(Matplot++ REQUIRED)

add_executable(test_bspline_1d test_bspline_1d.cpp)
add_executable(test_knots test_knots.cpp)

target_link_libraries(test_bspline_1d PUBLIC bsplinex Matplot++::matplot)
target_link_libraries(test_knots PUBLIC bsplinex)

