#include "BSplineX/bsplinex.hpp"
#include <iostream>

int main()
{
  auto bspline = bsplinex::factory::periodic_nonuniform(3, {0, 1, 2, 3, 4});

  std::cout << bspline.evaluate(3.0) << std::endl;

  return 0;
}
