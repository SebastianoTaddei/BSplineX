#ifndef T_DATA_HPP
#define T_DATA_HPP

// Standard includes
#include <cstddef>
#include <vector>

// BSplineX includes
#include "defines.hpp"
#include "types.hpp"

namespace bsplinex::knots
{

template <typename T, Curve C> class Data
{
public:
  virtual T at(size_t index)                              = 0;
  virtual size_t size()                                   = 0;
  virtual std::vector<T> slice(size_t first, size_t last) = 0;
};

template <typename T> class Data<T, Curve::UNIFORM>
{
private:
  T begin{};
  T end{};
  size_t num_elems{0};
  T step_size{};

public:
  // Specifying the step-size means the domain will be [begin, end[
  Data(T begin, T end, T step)
  {
    assertm(step > 0, "Negative step-size");
    assertm(begin < end, "Wrong interval");

    this->begin     = begin;
    this->step_size = step;
    this->num_elems = (end - begin) / step + 1;
    this->end       = begin + (this->num_elems - 1) * step;
  }

  // Specifying the num-elems means the domain will be [begin, end]
  Data(T begin, T end, size_t num_elems)
      : begin{begin}, end{end}, num_elems{num_elems},
        step_size{(end - begin) / num_elems}
  {
    assertm(begin < end, "Wrong interval");

    this->begin     = begin;
    this->end       = end;
    this->num_elems = num_elems;
    this->step_size = (end - begin) / (num_elems - 1);
  }

  T at(size_t index) const
  {
    assertm(index < this->num_elems, "Out of bounds");
    return this->begin + index * this->step_size;
  }

  [[nodiscard]] size_t size() const { return this->num_elems; }

  std::vector<T> slice(size_t first, size_t last) const
  {
    assertm(first <= last, "Invalid range");
    assertm(last <= this->num_elems, "Out of bounds");

    std::vector<T> tmp{};
    tmp.reserve(last - first);

    for (size_t i{first}; i < last; i++)
    {
      tmp.push_back(this->at(i));
    }

    return tmp;
  }
};

template <typename T> class Data<T, Curve::NON_UNIFORM>
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

    return std::vector<T>{
        this->raw_data.begin() + first, this->raw_data.begin() + last
    };
  }
};

} // namespace bsplinex::knots

#endif
