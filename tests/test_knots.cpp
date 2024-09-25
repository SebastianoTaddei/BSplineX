#include "knots/atter.hpp"
#include <iostream>

using namespace bsplinex;

int main()
{
  knots::Data<double, Curve::UNIFORM> data{0.0, 10.0, 1.0};
  knots::Atter<double, Curve::UNIFORM, BoundaryCondition::CLAMPED> atter{
      data, 3
  };

  for (size_t i{0}; i < 11; i++)
  {
    std::cout << atter.at(i) << std::endl;
  }

  return 0;
}
