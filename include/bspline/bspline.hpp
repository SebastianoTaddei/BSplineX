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
  control_points::ControlPoints<T, BC> control_points;
  size_t degree{0};
  deboor::DeBoor<T, C, BC, EXT> deboor;

public:
  BSpline(
      knots::Data<T, C> knots_data,
      control_points::Data<T> control_points_data,
      size_t degree
  )
      : knots{knots_data, degree}, control_points{control_points_data, degree},
        degree{degree}, deboor{this->knots, this->control_points, this->degree}
  {
    this->check_sizes();
  }

  T evaluate(T value)
  {
    auto index_value_pair = this->knots.find(value);
    return this->deboor.deboor(index_value_pair.first, index_value_pair.second);
  }

private:
  void check_sizes()
  {
    if (this->control_points.size() == this->knots.size() - this->degree - 1)
    {
      return;
    }

    std::stringstream ss{};
    ss << "Found control_points.size() != knots.size() - degree - 1 ("
       << this->control_points.size()
       << " != " << this->knots.size() - this->degree - 1 << "). ";

    // clang-format off

    if constexpr (BC == BoundaryCondition::OPEN)
    {
      ss << "With BoundaryCondition::OPEN no padding is added, therefore you need to respect: control_points_data.size() = knots_data.size() - degree - 1";
    }
    else if constexpr (BC == BoundaryCondition::CLAMPED)
    {
      ss << "With BoundaryCondition::CLAMPED padding is added to the knots, therefore you need to respect: control_points_data.size() = knots_data.size() + degree - 1";
    }
    else if constexpr (BC == BoundaryCondition::PERIODIC)
    {
      ss << "With BoundaryCondition::PERIODIC padding is added to the knots and control points, therefore you need to respect: control_points_data.size() = knots_data.size() - 1";
    }
    else {
     ss << "Unknown BoundaryCondition, you should not have arrived here ever!";
    }

    // clang-format on

    throw std::runtime_error(ss.str());
  }
};

} // namespace bsplinex::bspline

#endif
