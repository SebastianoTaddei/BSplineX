#ifndef BSPLINE_HPP
#define BSPLINE_HPP

// BSplineX includes
#include "control_points/control_points.hpp"
#include "deboor/deboor.hpp"
#include "knots/knots.hpp"
#include "types.hpp"

namespace bsplinex::bspline
{

template <typename T, Curve C, BoundaryCondition BC, Extrapolation EXT>
class BSpline
{
private:
  knots::Knots<T, C, BC, EXT> const &knots;
  control_points::ControlPoints<T, C, BC> const &control_points;
  size_t degree{0};
  deboor::DeBoor<T, C, BC, EXT> deboor;

public:
  BSpline(
      knots::Knots<T, C, BC, EXT> const &knots,
      control_points::ControlPoints<T, C, BC> const &control_points,
      size_t degree
  )
      : knots{knots}, control_points{control_points}, degree{degree},
        deboor{this->knots, this->control_points, this->degree}
  {
  }

  T evaluate(T value)
  {
    return this->deboor.deboor(this->knots.find(value), value);
  }
};

} // namespace bsplinex::bspline

#endif
