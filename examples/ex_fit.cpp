#include "bspline/bspline.hpp"
#include <iostream>

int main()
{
  constexpr std::size_t degree{3};
  const std::vector<double> control_points(11); // dummy control points

  // uniform, periodic B-spline where knots are [0.1, 1.1, ..., 10.1, 11.1]
  bsplinex::bspline::BSpline<
      double,
      bsplinex::Curve::UNIFORM,
      bsplinex::BoundaryCondition::PERIODIC,
      bsplinex::Extrapolation::PERIODIC>
      bspline{{0.1, 12.0, 1.0}, {control_points}, degree};

  // fit some points
  const std::vector<double> x_fit{2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9};
  std::vector<double> y_fit{11.2, 22.3, 13.4, 14.5, 25.6, 36.7, 17.8};
  bspline.fit(x_fit, y_fit);

  // evaluate some points. Since the curve is periodic, the evaluation can be
  // done at any point
  const std::vector<double> eval_x{3.0, 3.4, 5.1, 6.2, 15.0, 22.0};
  for (const double x : eval_x)
  {
    std::cout << "bspline.evaluate(" << x << ") = " << bspline.evaluate(x)
              << std::endl;
  }
}