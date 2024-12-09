#ifndef C_ATTER_HPP
#define C_ATTER_HPP

// BSplineX includes
#include "BSplineX/control_points/c_data.hpp"
#include "BSplineX/control_points/c_padder.hpp"
#include "BSplineX/defines.hpp"
#include "BSplineX/types.hpp"

namespace bsplinex::control_points
{

template <typename T, BoundaryCondition BC>
class Atter
{
private:
  Data<T> data;
  Padder<T, BC> padder;

public:
  Atter() = default;

  Atter(Data<T> data, size_t degree) : data{data}, padder{this->data, degree} {}

  T at(size_t index) const
  {
    assertm(index < this->size(), "Out of bounds");
    if (index < this->data.size())
    {
      return this->data.at(index);
    }
    else
    {
      return this->padder.right(index - this->data.size());
    }
  }

  [[nodiscard]] size_t size() const { return this->data.size() + this->padder.size(); }
};

} // namespace bsplinex::control_points

#endif
