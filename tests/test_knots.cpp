#include "control_points/control_points.hpp"
#include "knots/knots.hpp"
#include "types.hpp"
#include <iostream>

using namespace bsplinex;

int main()
{
  knots::Knots<
      double,
      Curve::NON_UNIFORM,
      BoundaryCondition::OPEN,
      Extrapolation::NONE>
      knots{{{0.0, 1.0, 2.0, 2.5, 3.4, 3.9, 4.6, 9.0}}, 3};

  control_points::
      ControlPoints<double, Curve::NON_UNIFORM, BoundaryCondition::OPEN>
          ctrl{{{0.0, 1.0, 2.0, 2.5, 3.4, 3.9, 4.6, 9.0}}, 3};

  for (size_t i{0}; i < knots.size(); i++)
  {
    std::cout << knots.at(i) << std::endl;
  }

  std::cout << knots.find(31.0) << std::endl;

  return 0;
}
