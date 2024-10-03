#ifndef CONTROL_POINTS_HPP
#define CONTROL_POINTS_HPP

// Standard includes
#include <cstddef>

// BSplineX includes
#include "control_points/cp_atter.hpp"
#include "types.hpp"

/**
 * Naming convention:
 * - `n` -> number on control points
 * - `m` -> number of knots
 * - `p` -> degree of the curve
 * - `c` -> control points vector
 *
 * Control points:
 * - If the curve is open, the number of control points must be `n = m - p - 1`
 * - If the curve is periodic, the number of control points must be `n = m - 1`
 *   since they will need to be padded for periodicity by repeating the first
 *   `p` control points at the end
 * - If the curve is clamped, the number of control points must be
 *   `n = m + p - 1`
 *
 */

namespace bsplinex::control_points
{

template <typename T, Curve C, BoundaryCondition BC> class ControlPoints
{
private:
  Atter<T, C, BC> atter;

public:
  ControlPoints(Data<T, C> data, size_t degree) : atter{data, degree} {}

  T at(size_t index) const { return this->atter.at(index); }

  size_t size() const { return this->atter.size(); }
};

} // namespace bsplinex::control_points

#endif
