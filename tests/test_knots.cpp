#include <iostream>

#include "bspline/bspline.hpp"

using namespace bsplinex;

int main()
{
  bspline::BSpline<
      double,
      Curve::NON_UNIFORM,
      BoundaryCondition::PERIODIC,
      Extrapolation::PERIODIC>
      bspline{
          {{{0.0, 1.0, 2.0, 2.5, 3.4, 3.9, 4.6, 9.0}}, 3},
          {{{0.0, 1.0, 2.0, 2.5, 3.4, 3.9, 4.6, 9.0}}, 3},
          3
      };

  for (size_t i{0}; i < 10; i++)
  {
    std::cout << bspline.evaluate((double)i) << std::endl;
  }

  return 0;
}
