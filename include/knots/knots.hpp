#ifndef KNOTS_HPP
#define KNOTS_HPP

// Standard includes
#include <utility>

// BSplineX includes
#include "knots/t_atter.hpp"
#include "knots/t_extrapolator.hpp"
#include "knots/t_finder.hpp"
#include "types.hpp"

/**
 * Naming convention:
 * - `m` -> number of knots
 * - `p` -> degree of the curve
 * - `t` -> knots vector
 *
 * Curve domain:
 * - If the curve is open, the domain is [t_p, t_{end - p}]
 * - If the curve is periodic, the domain is [t_0, t_{end}] but appropiate
 *   padding is needed
 * - If the curve is clamped, the domain is [t_0, t_{end}] but the start and end
 *   knots must have multiplicity `p+1`
 *
 * Knots padding:
 * - If the curve is open, no padding is needed, the full `n + p + 1` knots have
 *   to be provided
 * - If the curve is periodic, we need to add `p` knots at the left and right
 *   following periodicity: [0, 1, 2, 2.5, 3] with p = 3 ->
 *   [-2.0, -1.0, -0.5, 0, 1, 2, 2.5, 3, 4, 5, 5.5]
 * - If the curve is clamped, we must repeat the first an last knots `p` times:
 *   [0, 1, 2, 2.5, 3] with p = 3 -> [0, 0, 0, 0, 1, 2, 2.5, 3, 3, 3, 3]
 *
 */

namespace bsplinex::knots
{

template <typename T, Curve C, BoundaryCondition BC, Extrapolation EXT>
class Knots
{
private:
  Atter<T, C, BC> atter;
  Extrapolator<T, C, BC, EXT> extrapolator;
  Finder<T, C, BC, EXT> finder;
  T value_left{};
  T value_right{};

public:
  Knots() = default;

  Knots(Data<T, C> data, size_t degree)
      : atter{data, degree}, extrapolator{this->atter, degree}, finder{this->atter, degree},
        value_left{this->atter.at(degree)},
        value_right{this->atter.at(this->atter.size() - degree - 1)}
  {
  }

  std::pair<size_t, T> find(T value) const
  {
    if (value < this->value_left || value >= this->value_right)
    {
      value = this->extrapolator.extrapolate(value);
    }

    return {this->finder.find(value), value};
  }

  std::pair<T, T> domain() { return {value_left, value_right}; }

  T at(size_t index) const { return this->atter.at(index); }

  [[nodiscard]] size_t size() const { return this->atter.size(); }
};

/*

Knots
  - Finder
    - Atter
    - Extrapolator
  - Atter
    - Data
    - Padder
  - Padder
    - Data
  - Extrapolator
    - Atter
  - Data

*/

} // namespace bsplinex::knots

#endif
