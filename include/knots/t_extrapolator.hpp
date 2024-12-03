#ifndef T_EXTRAPOLATOR_HPP
#define T_EXTRAPOLATOR_HPP

// Standard includes
#include <cmath>
#include <cstddef>
#include <stdexcept>

// BSplineX includes
#include "defines.hpp"
#include "knots/t_atter.hpp"
#include "types.hpp"

namespace bsplinex::knots
{

template <typename T, Curve C, BoundaryCondition BC, Extrapolation EXT>
class Extrapolator
{
public:
  virtual size_t extrapolate(T value) const = 0;
};

template <typename T, Curve C, BoundaryCondition BC>
class Extrapolator<T, C, BC, Extrapolation::NONE>
{
public:
  Extrapolator(Atter<T, C, BC> const &, size_t) {}

  T extrapolate(T) const { throw std::runtime_error("Extrapolation explicitly set to NONE"); }
};

template <typename T, Curve C, BoundaryCondition BC>
class Extrapolator<T, C, BC, Extrapolation::CONSTANT>
{
private:
  T value_left{};
  T value_right{};

public:
  Extrapolator(Atter<T, C, BC> const &atter, size_t degree)
      : value_left{atter.at(degree)}, value_right{atter.at(atter.size() - degree - 1)}
  {
  }

  T extrapolate(T value) const
  {
    assertm(
        value < this->value_left || value >= this->value_right, "Value not outside of the domain"
    );
    return value < this->value_left ? this->value_left : this->value_right;
  }
};

template <typename T, Curve C, BoundaryCondition BC>
class Extrapolator<T, C, BC, Extrapolation::PERIODIC>
{
private:
  T value_left{};
  T value_right{};
  T period{};

public:
  Extrapolator(Atter<T, C, BC> const &atter, size_t degree)
      : value_left{atter.at(degree)}, value_right{atter.at(atter.size() - degree - 1)},
        period{this->value_right - this->value_left}
  {
  }

  T extrapolate(T value) const
  {
    assertm(
        value < this->value_left || value >= this->value_right, "Value not outside of the domain"
    );

    // TODO: Figure out how to prevent numerical errors

    if (value < this->value_left)
    {
      value += this->period * (std::floor((this->value_left - value) / this->period) + 1);
    }
    else if (value >= this->value_right)
    {
      value -= this->period * (std::floor((value - this->value_right) / this->period) + 1);
    }

    if (value < this->value_left || value >= this->value_right)
    {
      value = this->value_left;
    }

    return value;
  }
};

} // namespace bsplinex::knots

#endif
