#ifndef BSPLINEX_TYPES_HPP
#define BSPLINEX_TYPES_HPP

namespace bsplinex
{
enum BoundaryCondition
{
  CLAMPED  = 0,
  OPEN     = 1,
  PERIODIC = 2
};

enum Curve
{
  NON_UNIFORM = 0,
  UNIFORM     = 1
};

enum Extrapolation
{
  CONSTANT = 0,
  NONE
};
} // namespace bsplinex

#endif
