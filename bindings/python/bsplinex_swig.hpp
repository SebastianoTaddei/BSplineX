#ifndef BSPLINEX_SWIG_HPP
#define BSPLINEX_SWIG_HPP

#include "bsplinex.hpp"

inline double evaluate(double x) {
  return bsplinex::factory::open_uniform<double>(3, 0.0, 10.0, 10).evaluate(x);
}

inline bsplinex::types::OpenUniform<double> get() {
  return bsplinex::factory::open_uniform<double>(3, 0.0, 10.0, 10);
}

#endif
