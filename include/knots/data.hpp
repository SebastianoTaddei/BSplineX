#ifndef DATA_HPP
#define DATA_HPP

// Standard includes
#include <cassert>
#include <cstddef>
#include <vector>

// BSplineX includes
#include "defines.hpp"
#include "types.hpp"

/**
 * Naming convention:
 * - `m` -> number of knots
 * - `p` -> degree of the curve
 * - `t` -> knots vector
 *
 * Curve domain:
 * - If the curve is open, the domain is [t_p, t_{end - p}]
 * - If the curve is periodic, the domain is [t_0, t_{end}] but appropiate
 *   padding is needed
 * - If the curve is clamped, the domain is [t_0, t_{end}] but the start and end
 *   knots must have multiplicity `p+1`
 *
 * Knots padding:
 * - If the curve is open, no padding is needed, the full `n + p + 1` knots have
 *   to be provided
 * - If the curve is periodic, we need to add `p` knots at the left and right
 *   following periodicity: [0, 1, 2, 2.5, 3] with p = 3 ->
 *   [-2.0, -1.0, -0.5, 0, 1, 2, 2.5, 3, 4, 5, 5.5]
 * - If the curve is clamped, we must repeat the first an last knots `p` times:
 *   [0, 1, 2, 2.5, 3] with p = 3 -> [0, 0, 0, 0, 1, 2, 2.5, 3, 3, 3, 3]
 *
 */

namespace bsplinex
{
namespace knots
{

template <typename T, Curve C> class Data
{
public:
  virtual T at(size_t index)                                     = 0;
  virtual size_t size()                                          = 0;
  virtual std::vector<T> slice(size_t first, size_t last)        = 0;
  virtual void reduce_domain(size_t from_begin, size_t from_end) = 0;
};

template <typename T> class Data<T, Curve::UNIFORM>
{
private:
  T begin{};
  T end{};
  T step_size{};
  size_t num_elems{0};

public:
  // Specifying the step-size means the domain will be [begin, end[
  Data(T begin, T end, T step)
  {
    assertm(step > 0, "Negative step-size");
    assertm(begin < end, "Wrong interval");

    this->begin     = begin;
    this->step_size = step;
    this->num_elems = (end - begin) / step;
    this->end       = begin + this->num_elems * step;
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
    this->step_size = (end - begin) / num_elems;
  }

  T at(size_t index)
  {
    assertm(index >= 0 && index < this->end, "Out of bounds");
    return this->begin + index * this->step_size;
  }

  size_t size() { return this->num_elems; }

  std::vector<T> slice(size_t first, size_t last)
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

  void reduce_domain(size_t first, size_t last)
  {
    assertm(first <= last, "Invalid domain");
    assertm(last <= this->num_elems, "Out of bounds");

    this->end    = this->begin + last * this->step_size;
    this->begin += first * this->step_size;
  }
};

template <typename T> class Data<T, Curve::NON_UNIFORM>
{
private:
  std::vector<T> raw_data{};

public:
  Data(std::vector<T> const &data) : raw_data{data} {}

  T at(size_t index)
  {
    assertm(index >= 0 && index < this->raw_data.size(), "Out of bounds");
    return this->raw_data[index];
  }

  size_t size() { return this->raw_data.size(); }

  std::vector<T> slice(size_t first, size_t last)
  {
    assertm(first <= last, "Invalid range");
    assertm(last <= this->raw_data.size(), "Out of bounds");

    return std::vector<T>{
        this->raw_data.begin() + first, this->raw_data.begin() + last
    };
  }

  void reduce_domain(size_t first, size_t last)
  {
    assertm(first <= last, "Invalid domain");
    assertm(last <= this->raw_data.size(), "Out of bounds");

    this->raw_data = std::vector<T>(
        this->raw_data.begin() + first, this->raw_data.begin() + last
    );
  }
};

/*

Knots
  - Finder
    - Atter
  - Atter
    - Data
    - Padder
  - Padder
    - Data
  - Extrapolator
    - Atter
  - Data

*/

} // namespace knots
} // namespace bsplinex

#endif
