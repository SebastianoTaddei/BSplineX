#ifndef BSPLINEX_HPP
#define BSPLINEX_HPP

#include "bspline/bspline.hpp"

namespace bsplinex
{

template <typename T = double, Extrapolation EXT = Extrapolation::NONE>
using UniformOpen =
    bspline::BSpline<T, Curve::UNIFORM, BoundaryCondition::OPEN, EXT>;

template <typename T = double, Extrapolation EXT = Extrapolation::NONE>
using NonUniformOpen =
    bspline::BSpline<T, Curve::NON_UNIFORM, BoundaryCondition::OPEN, EXT>;

template <typename T = double, Extrapolation EXT = Extrapolation::NONE>
using UniformClamped =
    bspline::BSpline<T, Curve::UNIFORM, BoundaryCondition::CLAMPED, EXT>;

template <typename T = double, Extrapolation EXT = Extrapolation::NONE>
using NonUniformClamped =
    bspline::BSpline<T, Curve::NON_UNIFORM, BoundaryCondition::CLAMPED, EXT>;

template <typename T = double>
using UniformPeriodic = bspline::BSpline<
    T,
    Curve::UNIFORM,
    BoundaryCondition::PERIODIC,
    Extrapolation::PERIODIC>;

template <typename T = double>
using NonUniformPeriodic = bspline::BSpline<
    T,
    Curve::NON_UNIFORM,
    BoundaryCondition::CLAMPED,
    Extrapolation::PERIODIC>;

} // namespace bsplinex

#endif

