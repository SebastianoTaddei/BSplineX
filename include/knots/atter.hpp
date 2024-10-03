#ifndef ATTER_HPP
#define ATTER_HPP

// BSplineX includes
#include "defines.hpp"
#include "knots/data.hpp"
#include "knots/padder.hpp"
#include "types.hpp"

namespace bsplinex::knots
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

  [[nodiscard]] size_t size() const
  {
    return this->data.size() + 2 * this->degree;
  }

  [[nodiscard]] size_t get_degree() const { return this->degree; }

  class iterator
  {
  private:
    Atter<T, C, BC> const *atter{nullptr};
    size_t index{0};

  public:
    iterator(Atter<T, C, BC> const *atter, size_t index)
        : atter{atter}, index{index}
    {
    }

    iterator(iterator const &b) : atter{b.atter}, index{b.index} {}

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

    iterator &operator--()
    {
      --(this->index);
      return *this;
    }

    iterator operator--(int)
    {
      iterator retval = *this;
      --(*this);
      return retval;
    }

    iterator &operator+=(int n)
    {
      this->index += n;
      return *this;
    }

    iterator operator+(int n)
    {
      iterator retval  = *this;
      retval          += n;
      return retval;
    }

    iterator &operator-=(int n)
    {
      this->index -= n;
      return *this;
    }

    iterator operator-(int n)
    {
      iterator retval  = *this;
      retval          -= n;
      return retval;
    }

    int operator-(iterator const &b) { return this->index - b.index; }

    bool operator==(iterator const &other) const
    {
      return this->index == other.index;
    }

    iterator &operator=(iterator const &b)
    {
      if (this == &b)
      {
        return *this;
      }

      this->atter = b.atter;
      this->index = b.index;
      return *this;
    };

    bool operator!=(iterator const &other) const { return !(*this == other); }

    T operator*() { return this->atter->at(this->index); }

    T operator[](int n) { return *(*this + n); }

    bool operator<(iterator const &b) { return this->index < b.index; }

    bool operator>(iterator const &b) { return this->index > b.index; }

    bool operator<=(iterator const &b) { return !(*this > b); }

    bool operator>=(iterator const &b) { return !(*this < b); }

    // iterator traits
    using difference_type   = int;
    using value_type        = T;
    using pointer           = const T *;
    using reference         = const T &;
    using iterator_category = std::random_access_iterator_tag;
  };

  iterator begin() const { return {this, 0}; }

  iterator end() const { return {this, this->size()}; }
};

} // namespace bsplinex::knots

#endif