#ifndef T_FINDER_HPP
#define T_FINDER_HPP

// Standard includes
#include <algorithm>
#include <cstddef>
#include <iostream>

// BSplineX includes
#include "BSplineX/defines.hpp"
#include "BSplineX/knots/t_atter.hpp"
#include "BSplineX/macros.h"
#include "BSplineX/types.hpp"

namespace bsplinex::knots
{

template <typename T, Curve C, BoundaryCondition BC, Extrapolation EXT>
class Finder
{
private:
  Atter<T, C, BC> const *atter{nullptr};
  size_t index_left{0};
  size_t index_right{0};

public:
  Finder() { DEBUG_LOG_CALL(); }

  Finder(Atter<T, C, BC> const &atter, size_t degree)
      : atter{&atter}, index_left{degree}, index_right{this->atter->size() - degree - 1}
  {
    DEBUG_LOG_CALL();
  }

  Finder(const Finder &other)
      : atter(other.atter), index_left(other.index_left), index_right(other.index_right)
  {
    DEBUG_LOG_CALL();
  }

  Finder(Finder &&other) noexcept
      : atter(other.atter), index_left(other.index_left), index_right(other.index_right)
  {
    DEBUG_LOG_CALL();
  }

  ~Finder() noexcept { DEBUG_LOG_CALL(); }

  Finder &operator=(const Finder &other)
  {
    DEBUG_LOG_CALL();
    if (this == &other)
      return *this;
    atter       = other.atter;
    index_left  = other.index_left;
    index_right = other.index_right;
    return *this;
  }

  Finder &operator=(Finder &&other) noexcept
  {
    DEBUG_LOG_CALL()
    if (this == &other)
      return *this;
    atter       = other.atter;
    index_left  = other.index_left;
    index_right = other.index_right;
    return *this;
  }

  size_t find(T value) const
  {
    // TODO: we have to decide if we want to include the right value of the
    // domain or not
    assertm(
        value >= this->atter->at(this->index_left) && value <= this->atter->at(this->index_right),
        "Value outside of the domain"
    );

    auto upper = std::upper_bound(
        this->atter->begin() + this->index_left, this->atter->begin() + this->index_right, value
    );

    return upper - this->atter->begin() - 1;
  }
};

template <typename T, BoundaryCondition BC, Extrapolation EXT>
class Finder<T, Curve::UNIFORM, BC, EXT>
{
private:
  T value_left{};
  T value_right{};
  T step_size_inv{};
  size_t degree{};

public:
  Finder() { DEBUG_LOG_CALL(); }

  Finder(Atter<T, Curve::UNIFORM, BC> const &atter, size_t degree)
      : value_left{atter.at(degree)}, value_right{atter.at(atter.size() - degree - 1)},
        step_size_inv{T(1) / (atter.at(degree + 1) - atter.at(degree))}, degree{degree}
  {
    DEBUG_LOG_CALL();
  }

  Finder(const Finder &other)
      : value_left(other.value_left), value_right(other.value_right),
        step_size_inv(other.step_size_inv), degree(other.degree)
  {
    DEBUG_LOG_CALL();
  }

  Finder(Finder &&other) noexcept
      : value_left(other.value_left), value_right(other.value_right),
        step_size_inv(other.step_size_inv), degree(other.degree)
  {
    DEBUG_LOG_CALL();
  }

  Finder &operator=(const Finder &other)
  {
    DEBUG_LOG_CALL();
    if (this == &other)
      return *this;
    value_left    = other.value_left;
    value_right   = other.value_right;
    step_size_inv = other.step_size_inv;
    degree        = other.degree;
    return *this;
  }

  Finder &operator=(Finder &&other) noexcept
  {
    DEBUG_LOG_CALL();
    if (this == &other)
      return *this;
    value_left    = other.value_left;
    value_right   = other.value_right;
    step_size_inv = other.step_size_inv;
    degree        = other.degree;
    return *this;
  }

  size_t find(T value) const
  {
    assertm(value >= this->value_left && value <= this->value_right, "Value outside of the domain");

    return static_cast<size_t>((value - this->value_left) * this->step_size_inv) + this->degree;
  }
};

} // namespace bsplinex::knots

#endif
