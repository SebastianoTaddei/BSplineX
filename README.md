# BSplineX
The BSplineX C++ library allows you to create and manipulate B-Spline curves. 

It is a header-only library, so just include the header files in your project and you are ready to go.

## Features
Currently, the library supports 1D B-Spline curves.

The following B-Spline curve types are supported:
- Uniform and non-uniform knot vectors
- Open and clamped curves with no or constant extrapolation
- Periodic curves

The following operations are supported:
- Evaluation of the curve
- Evaluation of the derivatives of the curve
- Fitting of a curve to a set of points

## Include the library in your project
### CMake
If you are using CMake, you can add the library as a subdirectory in your project. 
```cmake
add_subdirectory(path/to/BSplineX)
target_link_libraries(your_target BSplineX)
```
You can use `FetchContent` to download the library from GitHub.
```cmake
include(FetchContent)
FetchContent_Declare(
  BSplineX
  GIT_REPOSITORY https://github.com/SebastianoTaddei/BSplineX.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(BSplineX)
target_link_libraries(your_target BSplineX)
```

## Usage
See the [examples](examples) folder for some examples on how to use the library.


