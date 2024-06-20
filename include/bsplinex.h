#ifndef BSPLINEX_H
#define BSPLINEX_H

// Standard includes
#include <algorithm>
#include <functional>
#include <vector>

/**
 * Naming convention:
 * - `n` -> number on control points
 * - `m` -> number of knots
 * - `p` -> degree of the curve
 * - `t` -> knots vector
 * - `c` -> control points vector
 * - `B` -> BSpline basis
 * - `S` -> BSpline curve
 *
 * Curve domain:
 * - If the curve is open, the domain is [t_p, t_{end - p}]
 * - If the curve is periodic, the domain is [t_0, t_{end}] but appropiate
 * padding is needed
 * - If the curve is clamped, the domain is [t_0, t_{end}] but the start and end
 * knots must have multiplicity `p+1`
 *
 * Knots padding:
 * - If the curve is open, no padding is needed, the full `n + p + 1` knots have
 * to be provided
 * - If the curve is periodic, we need to add `p` knots at the left and right
 * following periodicity: [0, 1, 2, 2.5, 3] with p = 3 -> [-1.0, -0.5, 0, 1,
 * 2, 2.5, 3, 4] the knots are added following the pattern [left, right, left,
 * right, left, ...]
 * - If the curve is clamped, we must repeat the first an last knots `p` times:
 *     [0, 1, 2, 2.5, 3] with p = 3 -> [0, 0, 0, 0, 1, 2, 2.5, 3, 3, 3, 3]
 */

namespace bsplinex
{

enum BoundaryCondition
{
  OPEN     = 0,
  PERIODIC = 1,
  CLAMPED  = 2
};

template <typename T> class BSpline
{
private:
  size_t degree = 0;
  std::vector<T> vector{};
  std::vector<T> control_points{};
  BoundaryCondition bc_type = OPEN;

public:
  BSpline() = default;

  void setup(
      size_t degree,
      std::vector<T> const &knots,
      std::vector<T> const &control_points,
      BoundaryCondition bc_type = OPEN
  )
  {
    if (0 == degree)
    {
      throw std::invalid_argument("Degree must be greater than 0");
    }
    this->degree = degree;

    // The knots must be at least two and monotonic increasing
    if (knots.size() < 2)
    {
      throw std::invalid_argument("Knots must have at least two elements");
    }
    if (!std::is_sorted(knots.begin(), knots.end(), std::less_equal<T>{}))
    {
      throw std::invalid_argument("Knots must be monotonic increasing");
    }
    this->vector  = knots;
    this->bc_type = bc_type;
    this->pad_knots();

    // The control points must be knots - p - 1
    if (control_points.size() != knots.size() - degree - 1)
    {
      throw std::invalid_argument(
          "Control points must be knots.size() - degree - 1"
      );
    }
    this->control_points = control_points;

    // If the curve is periodic, the knots and control points must be circular
    if (PERIODIC == bc_type)
    {
      this->circular_condition(this->knots);
      this->circular_condition(this->control_points);
    }
  }

  T evaluate(T point, size_t derivative = 0) const
  {
    // For now the point must in one knot interval (later we will implement
    // extrapolation)
    if (point < this->vector.front() || point > this->vector.back())
    {
      throw std::invalid_argument("Point must be within the knot interval");
    }

    // Binary search to find the knot interval
    size_t k =
        std::upper_bound(this->vector.begin(), this->vector.end(), point) -
        this->vector.begin() - 1;

    return this->deboor_with_derivatives(k, point, derivative);
  }

private:
  T deboor(int k, T x, std::vector<T> const &t, std::vector<T> const &c, int p)
      const
  {
    std::vector<T> d(p + 1);
    for (int j = 0; j <= p; j++)
    {
      d[j] = c[j + k - p];
    }

    T alpha = 0;
    for (int r = 1; r <= p; r++)
    {
      for (int j = p; j >= r; j--)
      {
        alpha = (x - t[j + k - p]) / (t[j + 1 + k - r] - t[j + k - p]);
        d[j]  = (1.0 - alpha) * d[j - 1] + alpha * d[j];
      }
    }

    return d[p];
  }

  T deboor_with_derivatives(int k, T x, int d) const
  {
    std::vector<T> q = this->control_points;
    for (int n = 1; n <= d; n++)
    {
      for (int j = 0; j <= this->control_points.size() - n; j++)
      {
        q[j] = (this->degree - n + 1) * (q[j + 1] - q[j]) /
               (this->vector[j + this->degree + 1] - this->vector[j + n]);
      }
    }

    // Modify the knot vector
    std::vector<T> new_t(this->vector.begin() + d, this->vector.end() - d);

    return this->deboor(k - d, x, new_t, q, this->degree - d);
  }

  void pad_knots()
  {
    switch (this->bc_type)
    {
    case PERIODIC:
    {
      size_t knots_right = this->degree / 2;
      size_t knots_left  = this->degree - knots_right;
      std::vector<T> periodic_knots(this->vector.size() + this->degree);
      for (size_t i = 0; i < knots_left; i++)
      {
        periodic_knots[knots_left - 1 - i] =
            this->vector.front() -
            (this->vector.back() -
             this->vector[this->vector.size() - 1 - (i + 1)]);
      }
      for (size_t i = 0; i < this->vector.size(); i++)
      {
        periodic_knots[knots_left + i] = this->vector[i];
      }
      for (size_t i = 0; i < knots_right; i++)
      {
        periodic_knots[knots_left + this->vector.size() + i] =
            this->vector.back() + (this->vector[i + 1] - this->vector.front());
      }
      this->vector = periodic_knots;
      break;
    }
    case CLAMPED:
    {
      this->vector.insert(
          this->vector.begin(), this->degree, this->vector.front()
      );
      this->vector.insert(
          this->vector.end(), this->degree, this->vector.back()
      );
      break;
    }
    case OPEN:
    default:
      break;
    }
  }

  void circular_condition(std::vector<T> &vector)
  {
    size_t elem_left  = this->degree / 2;
    size_t elem_right = this->degree - elem_left;
    std::vector<T> circular_vector(vector.size() + this->degree);
    for (size_t i = 0; i < elem_left; i++)
    {
      circular_vector[elem_left - 1 - i] = vector[vector.size() - 1 - i];
    }
    for (size_t i = 0; i < vector.size(); i++)
    {
      circular_vector[elem_left + i] = vector[i];
    }
    for (size_t i = 0; i < elem_right; i++)
    {
      circular_vector[elem_left + vector.size() + i] = vector[i];
    }
    vector = circular_vector;
  }
};

} // namespace bsplinex

#endif
