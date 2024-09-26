#ifndef FINDER_HPP
#define FINDER_HPP

// Standard includes
#include <cstddef>

// BSplineX includes
#include "knots/atter.hpp"
#include "knots/extrapolator.hpp"
#include "types.hpp"

namespace bsplinex
{
namespace knots
{

template <typename T, Curve C, BoundaryCondition BC, Extrapolation EXT>
class Finder
{
private:
  Atter<T, C, BC> const &atter;
  Extrapolator<T, C, BC, EXT> const &extrapolator;
  size_t index_left{0};
  size_t index_right{0};
  T value_left{};
  T value_right{};

public:
  Finder(
      Atter<T, C, BC> const &atter,
      Extrapolator<T, C, BC, EXT> const &extrapolator
  )
      : atter{atter}, extrapolator{extrapolator},
        index_left{this->atter.get_degree()},
        index_right{this->atter.size() - this->index_left - 1},
        value_left{this->atter.at(this->index_left)},
        value_right{this->atter.at(this->index_right)}
  {
  }

  size_t find(T value) const
  {
    if (value < this->value_left || value > this->value_right)
    {
      return this->extrapolator.extrapolate(value);
    }

    return this->binary_search(value);
  }

  size_t binary_search(T value) const
  {
    size_t left   = this->index_left;
    size_t right  = this->index_right;
    size_t middle = 0;
    while (left < right)
    {
      middle = (right + left) / 2;
      if (value < this->atter.at(middle))
      {
        right = middle - 1;
      }
      else
      {
        left = middle + 1;
      }
    }
    return left;
  }
};

} // namespace knots
} // namespace bsplinex

#endif
