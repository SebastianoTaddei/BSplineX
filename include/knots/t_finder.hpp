#ifndef T_FINDER_HPP
#define T_FINDER_HPP

// Standard includes
#include <algorithm>
#include <cstddef>

// BSplineX includes
#include "knots/t_atter.hpp"
#include "knots/t_extrapolator.hpp"
#include "types.hpp"

namespace bsplinex::knots
{

template <typename T, Curve C, BoundaryCondition BC, Extrapolation EXT>
class Finder
{
private:
  Atter<T, C, BC> const &atter;
  Extrapolator<T, C, BC, EXT> extrapolator;
  size_t index_left{0};
  size_t index_right{0};
  T value_left{};
  T value_right{};

public:
  Finder(Atter<T, C, BC> const &atter)
      : atter{atter}, extrapolator{this->atter},
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
      value = this->extrapolator.extrapolate(value);
    }

    auto upper = std::upper_bound(
        this->atter.begin() + this->index_left,
        this->atter.begin() + (this->index_right + 1),
        value
    );

    return upper - this->atter.begin() - 1;
  }
};

} // namespace bsplinex::knots

#endif
