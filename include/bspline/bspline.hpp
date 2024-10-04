#ifndef BSPLINE_HPP
#define BSPLINE_HPP

// Standard includes
#include <sstream>

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
  knots::Knots<T, C, BC, EXT> knots;
  control_points::ControlPoints<T, C, BC> control_points;
  size_t degree{0};
  deboor::DeBoor<T, C, BC, EXT> deboor;

public:
  BSpline(
      knots::Data<T, C> knots_data,
      control_points::Data<T, C> control_points_data,
      size_t degree
  )
      : knots{knots_data, degree}, control_points{control_points_data, degree},
        degree{degree}, deboor{this->knots, this->control_points, this->degree}
  {
    if (this->control_points.size() != this->knots.size() - this->degree - 1)
    {
      std::stringstream os;
      os << "Found control_points_data.size() != " << 3;
      throw std::runtime_error(os.str());
    }
  }

  T evaluate(T value)
  {
    return this->deboor.deboor(this->knots.find(value), value);
  }
};

} // namespace bsplinex::bspline

#endif
