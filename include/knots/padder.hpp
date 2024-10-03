#ifndef PADDER_HPP
#define PADDER_HPP

// Standard includes
#include <vector>

// BSplineX includes
#include "defines.hpp"
#include "knots/data.hpp"
#include "types.hpp"

namespace bsplinex
{
namespace knots
{

template <typename T, Curve C, BoundaryCondition BC> class Padder
{
public:
  virtual T left(size_t index) const  = 0;
  virtual T right(size_t index) const = 0;
};

template <typename T, Curve C> class Padder<T, C, BoundaryCondition::OPEN>
{
private:
  std::vector<T> pad_left{};
  std::vector<T> pad_right{};

public:
  Padder(Data<T, C> &data, size_t degree)
  {
    this->pad_left  = data.slice(0, degree);
    this->pad_right = data.slice(data.size() - degree, data.size());

    data.reduce_domain(degree, data.size() - degree);
  }

  T left(size_t index) const
  {
    assertm(index <= this->pad_left.size(), "Out of bounds");
    return this->pad_left[index];
  }

  T right(size_t index) const
  {
    assertm(index <= this->pad_right.size(), "Out of bounds");
    return this->pad_right[index];
  }
};

template <typename T, Curve C> class Padder<T, C, BoundaryCondition::CLAMPED>
{
private:
  T pad_left{};
  T pad_right{};
  size_t degree{0};

public:
  Padder(Data<T, C> &data, size_t degree)
  {
    this->pad_left  = data.at(0);
    this->pad_right = data.at(data.size() - 1);
    this->degree    = degree;
  }

  T left(size_t index) const
  {
    assertm(index <= this->degree, "Out of bounds");
    return this->pad_left;
  }

  T right(size_t index) const
  {
    assertm(index <= this->degree, "Out of bounds");
    return this->pad_right;
  }
};

template <typename T, Curve C> class Padder<T, C, BoundaryCondition::PERIODIC>
{
private:
  std::vector<T> pad_left{};
  std::vector<T> pad_right{};

public:
  Padder(Data<T, C> &data, size_t degree)
  {
    T period        = data.at(data.size() - 1) - data.at(0);
    this->pad_left  = data.slice(data.size() - degree - 1, data.size() - 1);
    this->pad_right = data.slice(1, degree + 1);
    for (size_t i{0}; i < degree; i++)
    {
      this->pad_left[i]  -= period;
      this->pad_right[i] += period;
    }
  }

  T left(size_t index) const
  {
    assertm(index <= this->pad_left.size(), "Out of bounds");
    return this->pad_left[index];
  }

  T right(size_t index) const
  {
    assertm(index <= this->pad_right.size(), "Out of bounds");
    return this->pad_right[index];
  }
};

} // namespace knots
} // namespace bsplinex

#endif
