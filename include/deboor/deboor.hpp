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
  control_points::ControlPoints<T, BC> const &control_points;
  size_t degree{0};
  std::vector<T> support{};

public:
  DeBoor(
      knots::Knots<T, C, BC, EXT> const &knots,
      control_points::ControlPoints<T, BC> const &control_points,
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

  std::vector<T> basis_functions(size_t index, T value)
  {
    std::vector<T> basis(this->control_points.size(), 0.0);

    basis.at(index) = 1.0;
    for (size_t d{1}; d <= this->degree; d++)
    {
      basis.at(index - d) = (knots.at(index + 1) - value) /
                            (knots.at(index + 1) - knots.at(index - d + 1)) *
                            basis.at(index - d + 1);
      for (size_t i{index - d + 1}; i < index; i++)
      {
        basis.at(i) = (value - knots.at(i)) / (knots.at(i + d) - knots.at(i)) *
                          basis.at(i) +
                      (knots.at(i + d + 1) - value) /
                          (knots.at(i + d + 1) - knots.at(i + 1)) *
                          basis.at(i + 1);
      }
      basis.at(index) = (value - knots.at(index)) /
                        (knots.at(index + d) - knots.at(index)) *
                        basis.at(index);
    }

    return basis;
  }
};

} // namespace bsplinex::deboor

#endif
