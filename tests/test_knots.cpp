#include "knots/atter.hpp"
#include "knots/extrapolator.hpp"
#include "knots/finder.hpp"
#include "knots/knots.hpp"
#include <iostream>

using namespace bsplinex;

int main()
{
  knots::Data<double, Curve::UNIFORM> data{0.0, 10.0, 1.0};
  knots::Atter<double, Curve::UNIFORM, BoundaryCondition::CLAMPED> atter{
      data, 3
  };
  knots::Extrapolator<
      double,
      Curve::UNIFORM,
      BoundaryCondition::CLAMPED,
      Extrapolation::NONE>
      extrapolator{};
  knots::Finder<
      double,
      Curve::UNIFORM,
      BoundaryCondition::CLAMPED,
      Extrapolation::NONE>
      finder{atter, extrapolator};
  knots::Knots<
      double,
      Curve::UNIFORM,
      BoundaryCondition::CLAMPED,
      Extrapolation::NONE>
      knots{atter, finder};

  for (size_t i{0}; i < 17; i++)
  {
    std::cout << knots.at(i) << std::endl;
  }

  std::cout << knots.find(3.0) << std::endl;

  return 0;
}
