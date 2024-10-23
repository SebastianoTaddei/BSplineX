#ifndef T_FINDER_HPP
#define T_FINDER_HPP

// Standard includes
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <utility>

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
  Finder(Atter<T, C, BC> const &atter, size_t degree)
      : atter{atter}, extrapolator{this->atter, degree}, index_left{degree},
        index_right{this->atter.size() - degree - 1},
        value_left{this->atter.at(this->index_left)},
        value_right{this->atter.at(this->index_right)}
  {
  }

  std::pair<size_t, T> find(T value) const
  {
    if (value < this->value_left || value >= this->value_right)
    {
      value = this->extrapolator.extrapolate(value);
    }

    auto upper = std::upper_bound(
        this->atter.begin() + this->index_left,
        this->atter.begin() + this->index_right,
        value
    );

    return {upper - this->atter.begin() - 1, value};
  }
};

template <typename T, BoundaryCondition BC, Extrapolation EXT>
class Finder<T, Curve::UNIFORM, BC, EXT>
{
private:
  Extrapolator<T, Curve::UNIFORM, BC, EXT> extrapolator;
  T value_left{};
  T value_right{};
  T step_size{};
  size_t degree{};

public:
  Finder(Atter<T, Curve::UNIFORM, BC> const &atter, size_t degree)
      : extrapolator{atter, degree}, value_left{atter.at(degree)},
        value_right{atter.at(atter.size() - degree - 1)},
        step_size{atter.at(degree + 1) - atter.at(degree)}, degree{degree}
  {
  }

  std::pair<size_t, T> find(T value) const
  {
    if (value < this->value_left || value >= this->value_right)
    {
      value = this->extrapolator.extrapolate(value);
    }

    return {
        std::floor((value - this->value_left) / this->step_size) + this->degree,
        value
    };
  }
};

} // namespace bsplinex::knots

#endif
