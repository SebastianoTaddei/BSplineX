#ifndef T_FINDER_HPP
#define T_FINDER_HPP

// Standard includes
#include <algorithm>
#include <cstddef>

// BSplineX includes
#include "defines.hpp"
#include "knots/t_atter.hpp"
#include "types.hpp"

namespace bsplinex::knots
{

template <typename T, Curve C, BoundaryCondition BC, Extrapolation EXT>
class Finder
{
private:
  Atter<T, C, BC> const &atter;
  size_t index_left{0};
  size_t index_right{0};

public:
  Finder(Atter<T, C, BC> const &atter, size_t degree)
      : atter{atter}, index_left{degree},
        index_right{this->atter.size() - degree - 1}
  {
  }

  size_t find(T value) const
  {
    // TODO: we have to decide if we want to include the right value of the
    // domain or not
    assertm(
        value >= this->atter.at(this->index_left) &&
            value <= this->atter.at(this->index_right),
        "Value outside of the domain"
    );

    auto upper = std::upper_bound(
        this->atter.begin() + this->index_left,
        this->atter.begin() + this->index_right,
        value
    );

    return upper - this->atter.begin() - 1;
  }
};

template <typename T, BoundaryCondition BC, Extrapolation EXT>
class Finder<T, Curve::UNIFORM, BC, EXT>
{
private:
  T value_left{};
  T value_right{};
  T step_size{};
  size_t degree{};

public:
  Finder(Atter<T, Curve::UNIFORM, BC> const &atter, size_t degree)
      : value_left{atter.at(degree)},
        value_right{atter.at(atter.size() - degree - 1)},
        step_size{atter.at(degree + 1) - atter.at(degree)}, degree{degree}
  {
  }

  size_t find(T value) const
  {
    assertm(
        value >= this->value_left && value <= this->value_right,
        "Value outside of the domain"
    );

    return (size_t)((value - this->value_left) / this->step_size) +
           this->degree;
  }
};

} // namespace bsplinex::knots

#endif
