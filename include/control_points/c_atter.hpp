#ifndef C_ATTER_HPP
#define C_ATTER_HPP

// BSplineX includes
#include "control_points/c_data.hpp"
#include "control_points/c_padder.hpp"
#include "defines.hpp"
#include "types.hpp"

namespace bsplinex::control_points
{

template <typename T, BoundaryCondition BC> class Atter
{
private:
  Data<T> data;
  Padder<T, BC> padder;
  size_t degree{0};

public:
  Atter(Data<T> data, size_t degree)
      : data{data}, padder{this->data, degree}, degree{degree}
  {
  }

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

  [[nodiscard]] size_t size() const
  {
    return this->data.size() + this->padder.size();
  }

  [[nodiscard]] size_t get_degree() const { return this->degree; }
};

} // namespace bsplinex::control_points

#endif
