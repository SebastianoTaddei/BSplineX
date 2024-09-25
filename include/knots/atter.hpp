#ifndef ATTER_HPP
#define ATTER_HPP

// BSplineX includes
#include "defines.hpp"
#include "knots/data.hpp"
#include "knots/padder.hpp"
#include "types.hpp"

namespace bsplinex
{
namespace knots
{

template <typename T, Curve C, BoundaryCondition BC> class Atter
{
private:
  Data<T, C> data;
  Padder<T, C, BC> padder;
  size_t degree{0};

public:
  Atter(Data<T, C> data, size_t degree)
      : data{data}, padder{this->data, degree}, degree{degree}
  {
  }

  T at(size_t index)
  {
    assertm(index < data.size() + 2 * degree, "Out of bounds");
    if (index < degree)
    {
      return padder.left(index);
    }
    else if (index > data.size() - 1 + degree)
    {
      return padder.right(index - data.size() - degree);
    }
    else
    {
      return data.at(index - degree);
    }
  }
};

} // namespace knots
} // namespace bsplinex

#endif
