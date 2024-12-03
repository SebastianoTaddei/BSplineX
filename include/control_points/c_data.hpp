#ifndef C_DATA_HPP
#define C_DATA_HPP

// Standard includes
#include <cstddef>
#include <vector>

// BSplineX includes
#include "defines.hpp"

namespace bsplinex::control_points
{

template <typename T>
class Data
{
private:
  std::vector<T> raw_data{};

public:
  Data(std::vector<T> const &data) : raw_data{data} {}

  T at(size_t index) const
  {
    assertm(index < this->raw_data.size(), "Out of bounds");
    return this->raw_data[index];
  }

  [[nodiscard]] size_t size() const { return this->raw_data.size(); }

  std::vector<T> slice(size_t first, size_t last)
  {
    assertm(first <= last, "Invalid range");
    assertm(last <= this->raw_data.size(), "Out of bounds");

    return std::vector<T>{this->raw_data.begin() + first, this->raw_data.begin() + last};
  }
};

} // namespace bsplinex::control_points

#endif
