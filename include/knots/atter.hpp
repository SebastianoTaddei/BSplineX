#ifndef ATTER_HPP
#define ATTER_HPP

// BSplineX includes
#include "defines.hpp"
#include "knots/data.hpp"
#include "knots/padder.hpp"
#include "types.hpp"
#include <iterator>

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

  T at(size_t index) const
  {
    assertm(index < this->data.size() + 2 * this->degree, "Out of bounds");
    if (index < this->degree)
    {
      return this->padder.left(index);
    }
    else if (index > this->data.size() - 1 + this->degree)
    {
      return this->padder.right(index - this->data.size() - this->degree);
    }
    else
    {
      return this->data.at(index - this->degree);
    }
  }

  size_t size() const { return this->data.size() + 2 * this->degree; }

  size_t get_degree() const { return this->degree; }

  class iterator
  {
  private:
    Atter<T, C, BC> const &atter;
    size_t index{0};

  public:
    iterator(Atter<T, C, BC> const &atter, size_t index)
        : atter{atter}, index{index}
    {
    }

    iterator &operator++()
    {
      ++(this->index);
      return *this;
    }

    iterator operator++(int)
    {
      iterator retval = *this;
      ++(*this);
      return retval;
    }

    bool operator==(iterator other) const { return this->index == other.index; }

    bool operator!=(iterator other) const { return !(*this == other); }

    T operator*() { return this->atter.at(this->index); }

    // iterator traits
    using difference_type   = size_t;
    using value_type        = T;
    using pointer           = const T *;
    using reference         = const T &;
    using iterator_category = std::random_access_iterator_tag;
  };

  iterator begin() { return {*this, 0}; }

  iterator end() { return {*this, this->size()}; }
};

} // namespace knots
} // namespace bsplinex

#endif
