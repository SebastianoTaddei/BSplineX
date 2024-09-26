#ifndef EXTRAPOLATOR_HPP
#define EXTRAPOLATOR_HPP

// Standard includes
#include <cstddef>
#include <stdexcept>

// BSplineX includes
#include "defines.hpp"
#include "knots/atter.hpp"
#include "types.hpp"

namespace bsplinex
{
namespace knots
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
  size_t extrapolate([[maybe_unused]] T value) const
  {
    throw std::runtime_error("Extrapolation explicitly set to NONE");
  }
};

template <typename T, Curve C, BoundaryCondition BC>
class Extrapolator<T, C, BC, Extrapolation::CONSTANT>
{
private:
  size_t index_left{0};
  size_t index_right{0};
  T value_left{};
  T value_right{};

public:
  Extrapolator(Atter<T, C, BC> const &atter)
      : index_left{atter.get_degree()},
        index_right{atter.size() - this->index_left - 1},
        value_left{atter.at(this->index_left)},
        value_right{atter.at(this->index_right)}
  {
  }

  size_t extrapolate(T value) const
  {
    assertm(
        value < this->value_left || value > this->value_right,
        "Value not outside of the domain"
    );
    return value < this->value_left ? this->index_left : this->index_right;
  }
};

} // namespace knots
} // namespace bsplinex

#endif
