#ifndef DEBOOR_HPP
#define DEBOOR_HPP

// Standard includes
#include <vector>

// BSplineX includes
#include "control_points/control_points.hpp"
#include "knots/knots.hpp"
#include "types.hpp"

namespace bsplinex::deboor
{

template <typename T, Curve C, BoundaryCondition BC, Extrapolation EXT>
class DeBoor
{
private:
  knots::Knots<T, C, BC, EXT> const &knots;
  control_points::ControlPoints<T, C, BC> const &control_points;
  size_t degree{0};
  std::vector<T> support{};

public:
  DeBoor(
      knots::Knots<T, C, BC, EXT> const &knots,
      control_points::ControlPoints<T, C, BC> const &control_points,
      size_t degree
  )
      : knots{knots}, control_points{control_points}, degree{degree}
  {
    this->support.resize(this->degree + 1);
  }

  T deboor(size_t index, T value)
  {
    for (size_t j = 0; j <= this->degree; j++)
    {
      this->support[j] = this->control_points.at(j + index - this->degree);
    }

    T alpha = 0;
    for (size_t r = 1; r <= this->degree; r++)
    {
      for (size_t j = this->degree; j >= r; j--)
      {
        alpha = (value - this->knots.at(j + index - this->degree)) /
                (this->knots.at(j + 1 + index - r) -
                 this->knots.at(j + index - this->degree));
        this->support[j] =
            (1.0 - alpha) * this->support[j - 1] + alpha * this->support[j];
      }
    }

    return this->support[this->degree];
  }
};

} // namespace bsplinex::deboor

#endif
