#ifndef C_PADDER_HPP
#define C_PADDER_HPP

// Standard includes
#include <vector>

// BSplineX includes
#include "control_points/c_data.hpp"
#include "defines.hpp"
#include "types.hpp"

namespace bsplinex::control_points
{

template <typename T, Curve C, BoundaryCondition BC> class Padder
{
private:
  std::vector<T> pad_right{};

public:
  Padder(Data<T, C> &data, size_t degree)
  {
    this->pad_right = data.slice(data.size() - degree, data.size());

    data.reduce_domain(0, data.size() - degree);
  }

  T right(size_t index) const
  {
    assertm(index <= this->pad_right.size(), "Out of bounds");
    return this->pad_right[index];
  }

  [[nodiscard]] size_t size() const { return this->pad_right.size(); }
};

template <typename T, Curve C> class Padder<T, C, BoundaryCondition::PERIODIC>
{
private:
  std::vector<T> pad_right{};

public:
  Padder(Data<T, C> &data, size_t degree)
  {
    this->pad_right = data.slice(0, degree);
  }

  T right(size_t index) const
  {
    assertm(index <= this->pad_right.size(), "Out of bounds");
    return this->pad_right[index];
  }

  [[nodiscard]] size_t size() const { return this->pad_right.size(); }
};

} // namespace bsplinex::control_points

#endif
