# BSplineX

The B-Spline library you have always dreamed of were performance and easy-to-use interfaces live in harmony.

## Features

This is a non-exhaustive list of the currently supported features.

- 1D B-Splines
  - Uniform/Non-uniform knots
  - Open/Clamped/Periodic boundary conditions
  - None/Constant/Periodic extrapolation
  - Least-squares fitting of the control points

## Installation

### Quick and dirty

`BSplineX` is a header-only library that depends only on `Eigen`, so the quick and dirty way of installing it is by simply copying the `include` directory to your project and make sure to have `Eigen` available however you see fit. Alternatively, you can do things properly and use `CMake`.

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

`BSplineX` is extremely easy to use, everything that is complex about B-Splines is hidden behind the curtain. Hereafter some quick examples.

### Creating a 1D B-Spline

```cpp
#include <iostream>
#include "bsplinex.hpp"

using namespace bsplinex;

int main()
{
  // Define the B-Spline degree
  constexpr size_t degree{3};

  // Create a uniform knot sequence in [0.0, 10.0] of 10 knots
  // Remember that knots must be ordered in ascending order
  constexpr size_t num_knots{10};
  constexpr double min_knot{0.0};
  constexpr double max_knot{10.0};
  knots::Data<double, Curve::UNIFORM> knots{min_knot, max_knot, num_knots);
  
  // Create some control points
  // There must be knots.size() - degree - 1 control points
  control_points::Data<double> ctrl_points{{1.0, 0.3, 0.6, 0.9, 0.28, 9.32}};
  
  // We can now create our uniform B-Spline
  // For example let's make it clamped and with constant extrapolation
  UniformClamped<
    double,
    Extrapolation::CONSTANT
  > bspline{knots, control_points};

  // Obviously you can directly create the B-Spline by simply nesting the list initialisers
  // (e.g., bspline{{knots_data}, {ctrl_points_data}})

  // Now you can simply evaluate the B-Spline
  double x{3.26};
  std::cout << "B-Spline value at " << x << " is " << bspline.evaluate(x) << std::endl;

  return 0;
}
```

If you want to make it periodic or open, just change the template parameter, easy. Want to pass in a non-uniform knot sequence? Piece of cake, just set the `Curve` template parameter to `Curve::NON_UNIFORM` and pass in your knots vector. Boom, done, simple.

Now I know what you are thinking: "Dude, cool, but how on earth do I know which control points to use?". Fear not, and follow along ;).

### Fitting a 1D B-Spline

In the previous example, the knots are easy to understand: they are you discretisation grid, so you just have to choose how you want to discretise along that dimension. Control points on the other hand are not easily graspable. Fortunately you don't really need to know them a priori, you can just interpolate (coming soon!) or fit some data points. Let's see how, assuming we start from the previous example.

```cpp
...
 
  // Create control points that are all zeros
  control_points::Data<double> ctrl_points(knots.size() - degree - 1, 0.0);
  
  // Create the B-Spline
  UniformClamped<
    double,
    Extrapolation::CONSTANT
  > bspline{knots, control_points};

  // Assume somewhere above you have loaded x- and y-values to fit you B-Spline to

  // Fit the B-Spline
  bspline.fit(x, y);

  // Now the B-Spline magically contains the best control points 
  // (in a least-squares sense) that fit your data

...
```

### More examples

See the [examples](examples) folder for some more examples on how to use the library.

## Performance

`BSplineX` is all about performance. In fact, aside from being a cool and useful project, it is our playground to understand modern code optimisation techniques. Our goal is to be as fast as humanly possible (albeit without having to re-write the library by hand in assembly). Hereafter a non-exhaustive quick table with some performance results. If you want to can run the benchmarks on your own PC.
