#ifndef T_PADDER_HPP
#define T_PADDER_HPP

// Standard includes
#include <stdexcept>
#include <vector>

// BSplineX includes
#include "BSplineX/defines.hpp"
#include "BSplineX/knots/t_data.hpp"
#include "BSplineX/types.hpp"

namespace bsplinex::knots
{

template <typename T, Curve C, BoundaryCondition BC>
class Padder
{
public:
  virtual T left(size_t index) const              = 0;
  virtual T right(size_t index) const             = 0;
  [[nodiscard]] virtual size_t size() const       = 0;
  [[nodiscard]] virtual size_t size_left() const  = 0;
  [[nodiscard]] virtual size_t size_right() const = 0;
};

template <typename T, Curve C>
class Padder<T, C, BoundaryCondition::OPEN>
{
public:
  Padder()
  {
    DEBUG_LOG_CALL("bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::OPEN>::Padder()");
  }

  Padder(Data<T, C> &, size_t)
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::OPEN>::Padder(bsplinex::knots::Data<T, C>&, size_t)"
    );
  }

  Padder(const Padder &other)
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::OPEN>::Padder(const "
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::OPEN>& other)"
    );
  }

  Padder(Padder &&other) noexcept
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::OPEN>::Padder(bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::OPEN>&& other) noexcept"
    );
  }

  Padder &operator=(const Padder &other)
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::OPEN>& "
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::OPEN>::operator=(const "
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::OPEN>& other)"
    );
    if (this == &other)
      return *this;
    return *this;
  }

  Padder &operator=(Padder &&other) noexcept
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::OPEN>& "
        "bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::OPEN>::operator=(bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::OPEN>&& other) noexcept"
    );
    if (this == &other)
      return *this;
    return *this;
  }

  T left(size_t) const
  {
    throw std::runtime_error(
        "OPEN knots padder has zero length, this function is here only for compatibility reasons."
    );
  }

  T right(size_t) const
  {
    throw std::runtime_error(
        "OPEN knots padder has zero length, this function is here only for compatibility reasons."
    );
  }

  [[nodiscard]] size_t size() const { return 0; }

  [[nodiscard]] size_t size_left() const { return 0; }

  [[nodiscard]] size_t size_right() const { return 0; }
};

template <typename T, Curve C>
class Padder<T, C, BoundaryCondition::CLAMPED>
{
private:
  T pad_left{};
  T pad_right{};
  size_t degree{0};

public:
  Padder()
  {
    DEBUG_LOG_CALL("bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::CLAMPED>::Padder()");
  }

  Padder(Data<T, C> &data, size_t degree)
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::CLAMPED>::Padder(bsplinex::knots::Data<T, C> &data, size_t "
        "degree)"
    );
    this->pad_left  = data.at(0);
    this->pad_right = data.at(data.size() - 1);
    this->degree    = degree;
  }

  Padder(const Padder &other)
      : pad_left(other.pad_left), pad_right(other.pad_right), degree(other.degree)
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::CLAMPED>::Padder(const "
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::CLAMPED>& other)"
    );
  }

  Padder(Padder &&other) noexcept
      : pad_left(other.pad_left), pad_right(other.pad_right), degree(other.degree)
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::CLAMPED>::Padder(bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::CLAMPED>&& other) noexcept"
    );
  }

  Padder &operator=(const Padder &other)
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::CLAMPED>& "
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::CLAMPED>::operator=(const "
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::CLAMPED>& other)"
    );
    if (this == &other)
      return *this;
    pad_left  = other.pad_left;
    pad_right = other.pad_right;
    degree    = other.degree;
    return *this;
  }

  Padder &operator=(Padder &&other) noexcept
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::CLAMPED>& "
        "bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::CLAMPED>::operator=(bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::CLAMPED>&& other) noexcept"
    );
    if (this == &other)
      return *this;
    pad_left  = other.pad_left;
    pad_right = other.pad_right;
    degree    = other.degree;
    return *this;
  }

  T left([[maybe_unused]] size_t index) const
  {
    assertm(index < this->degree, "Out of bounds");
    return this->pad_left;
  }

  T right([[maybe_unused]] size_t index) const
  {
    assertm(index < this->degree, "Out of bounds");
    return this->pad_right;
  }

  [[nodiscard]] size_t size() const { return this->size_left() + this->size_right(); }

  [[nodiscard]] size_t size_left() const { return this->degree; }

  [[nodiscard]] size_t size_right() const { return this->degree; }
};

template <typename T, Curve C>
class Padder<T, C, BoundaryCondition::PERIODIC>
{
private:
  std::vector<T> pad_left{};
  std::vector<T> pad_right{};

public:
  Padder()
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::PERIODIC>::Padder()"
    );
  }

  Padder(Data<T, C> &data, size_t degree)
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::PERIODIC>::Padder(bsplinex::knots::Data<T, C>& data, size_t "
        "degree)"
    );
    T period        = data.at(data.size() - 1) - data.at(0);
    this->pad_left  = data.slice(data.size() - degree - 1, data.size() - 1);
    this->pad_right = data.slice(1, degree + 1);
    for (size_t i{0}; i < degree; i++)
    {
      this->pad_left[i]  -= period;
      this->pad_right[i] += period;
    }
  }

  Padder(const Padder &other) : pad_left(other.pad_left), pad_right(other.pad_right)
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::PERIODIC>::Padder(const "
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::PERIODIC>& other)"
    );
  }

  Padder(Padder &&other) noexcept
      : pad_left(std::move(other.pad_left)), pad_right(std::move(other.pad_right))
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::PERIODIC>::Padder(bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::PERIODIC>&& other) noexcept"
    );
  }

  Padder &operator=(const Padder &other)
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::PERIODIC>& "
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::PERIODIC>::operator=(const "
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::PERIODIC>& other)"
    );
    if (this == &other)
      return *this;
    pad_left  = other.pad_left;
    pad_right = other.pad_right;
    return *this;
  }

  Padder &operator=(Padder &&other) noexcept
  {
    DEBUG_LOG_CALL(
        "bsplinex::knots::Padder<T, C, bsplinex::BoundaryCondition::PERIODIC>& "
        "bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::PERIODIC>::operator=(bsplinex::knots::Padder<T, C, "
        "bsplinex::BoundaryCondition::PERIODIC>&& other) noexcept"
    );
    if (this == &other)
      return *this;
    pad_left  = std::move(other.pad_left);
    pad_right = std::move(other.pad_right);
    return *this;
  }

  T left(size_t index) const
  {
    assertm(index < this->pad_left.size(), "Out of bounds");
    return this->pad_left[index];
  }

  T right(size_t index) const
  {
    assertm(index < this->pad_right.size(), "Out of bounds");
    return this->pad_right[index];
  }

  [[nodiscard]] size_t size() const { return this->size_left() + this->size_right(); }

  [[nodiscard]] size_t size_left() const { return this->pad_left.size(); }

  [[nodiscard]] size_t size_right() const { return this->pad_right.size(); }
};

} // namespace bsplinex::knots

#endif
