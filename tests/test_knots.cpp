#include "knots/interface.hpp"

using namespace bsplinex;

int main()
{
  knots::Knots<double, BoundaryCondition::OPEN, Curve::UNIFORM, Extrapolation::NONE> tmp{};
  tmp.test_virtual();

  return 0;
}
