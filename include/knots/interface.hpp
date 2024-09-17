#ifndef BSPLINEX_H
#define BSPLINEX_H

// Standard includes
#include <cstddef>
#include <string>
#include <vector>

// BSplineX includes
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

namespace bsplinex
{
namespace knots
{

template <typename T, BoundaryCondition BC, Curve C, Extrapolation EXT>
class Knots
{
private:
  size_t p{0};

public:
  virtual void test_virtual() = 0;
  void test_non_virtual();

  template<BC>
  bool check_bounds(x);

  template<BoundaryCondition::PERIODIC>
  bool check_bounds(x);
};

template <typename T, Curve C, Extrapolation EXT>
class Knots<T, BoundaryCondition::OPEN, C, EXT>
{
};

} // namespace knots
} // namespace bsplinex

#endif
