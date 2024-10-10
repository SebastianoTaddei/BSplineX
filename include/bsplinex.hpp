#ifndef BSPLINEX_H
#define BSPLINEX_H

// Standard includes
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <string>
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
 * Control points:
 * - If the curve is open, the number of control points must be `n = m - p - 1`
 * - If the curve is periodic, the number of control points must be `n = m - 1`
 *   since they will need to be padded for periodicity
 * - If the curve is clamped, the number of control points must be
 *   `n = m + p - 1`
 */

namespace bsplinex
{

enum BoundaryCondition
{
  OPEN     = 0,
  PERIODIC = 1,
  CLAMPED  = 2
};

enum Extrapolation
{
  NONE     = 0,
  CONSTANT = 1
};

template <typename T> class BSpline
{
private:
  size_t degree = 0;
  std::vector<T> knots{};
  std::vector<T> control_points{};
  BoundaryCondition bc_type   = OPEN;
  Extrapolation extrapolation = NONE;
  T period                    = 0;

public:
  BSpline() = default;

  void setup(
      size_t degree,
      std::vector<T> const &knots,
      std::vector<T> const &control_points,
      BoundaryCondition bc_type   = OPEN,
      Extrapolation extrapolation = NONE
  )
  {
    if (0 == degree)
    {
      throw std::invalid_argument("Degree must be greater than 0");
    }

    // The knots must be at least two and monotonic increasing
    if (knots.size() < 2)
    {
      throw std::invalid_argument("Knots must have at least two elements");
    }
    if (!std::is_sorted(knots.begin(), knots.end(), std::less_equal<T>{}))
    {
      throw std::invalid_argument("Knots must be monotonic increasing");
    }

    // The control points change length depending on the boundary condition
    switch (bc_type)
    {
    case OPEN:
      if (control_points.size() != knots.size() - degree - 1)
      {
        throw std::invalid_argument(
            "For the OPEN boundary condition, the control points must be "
            "knots.size() - degree - 1, found: " +
            std::to_string(control_points.size()) +
            " != " + std::to_string(knots.size() - degree - 1)
        );
      }
      break;
    case PERIODIC:
      if (control_points.size() != knots.size() - 1)
      {
        throw std::invalid_argument(
            "For the PERIODIC boundary condition, the control points must be "
            "knots.size() - 1, found: " +
            std::to_string(control_points.size()) +
            " != " + std::to_string(knots.size() - 1)
        );
      }
      break;
    case CLAMPED:
      if (control_points.size() != knots.size() + degree - 1)
      {
        throw std::invalid_argument(
            "For the CLAMPED boundary condition, the control points must be "
            "knots.size() + degree - 1, found: " +
            std::to_string(control_points.size()) +
            " != " + std::to_string(knots.size() + degree - 1)
        );
      }
      break;
    default:
      throw std::invalid_argument(
          "Invalid boundary condition" + std::to_string(bc_type)
      );
      break;
    }

    // Assign the values
    this->knots          = knots;
    this->control_points = control_points;
    this->bc_type        = bc_type;
    this->extrapolation  = extrapolation;

    // Apply the boundary conditions
    this->apply_boundary_conditions();

    for (size_t i = 0; i < this->knots.size(); i++)
    {
      std::cout << this->knots[i] << " ";
    }
    std::cout << std::endl;
    for (size_t i = 0; i < this->control_points.size(); i++)
    {
      std::cout << this->control_points[i] << " ";
    }
    std::cout << std::endl;
  }

  T evaluate(T point, size_t derivative = 0) const
  {
    // If the curve is periodic, we need to map the point to the domain
    if (PERIODIC == this->bc_type)
    {
      if (point < this->knots[this->degree])
      {
        point +=
            this->period *
            (std::floor((this->knots[this->degree] - point) / this->period) + 1
            );
      }
      else if (point >= this->knots[this->knots.size() - this->degree - 1])
      {
        point -=
            this->period *
            (std::floor(
                 (point - this->knots[this->knots.size() - this->degree - 1]) /
                 this->period
             ) +
             1);
      }
    }

    // Binary search to find the knot interval
    auto upper = std::upper_bound(
        this->knots.begin() + this->degree,
        this->knots.end() - this->degree,
        point
    );

    // Find the index of the knot interval
    size_t k = upper - this->knots.begin() - 1;

    // If the point is outside the domain, extrapolate
    if (upper == this->knots.begin() + this->degree)
    {
      this->apply_extrapolation(1, 0, k, point);
    }
    else if (upper == this->knots.end() - this->degree)
    {
      this->apply_extrapolation(0, 1, k, point);
    }

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
               (this->knots[j + this->degree + 1] - this->knots[j + n]);
      }
    }

    // Modify the knot vector
    std::vector<T> new_t(this->knots.begin() + d, this->knots.end() - d);

    return this->deboor(k - d, x, new_t, q, this->degree - d);
  }

  void apply_boundary_conditions()
  {
    switch (this->bc_type)
    {
    case PERIODIC:
    {
      // Save the period
      this->period = this->knots.back() - this->knots.front();

      // Pad the knots
      std::vector<T> periodic_knots(this->knots.size() + 2 * this->degree);
      for (size_t i = 0; i < this->degree; i++)
      {
        periodic_knots[this->degree - 1 - i] =
            this->knots.front() -
            (this->knots.back() - this->knots[this->knots.size() - 1 - (i + 1)]
            );
      }
      for (size_t i = 0; i < this->knots.size(); i++)
      {
        periodic_knots[this->degree + i] = this->knots[i];
      }
      for (size_t i = 0; i < this->degree; i++)
      {
        periodic_knots[this->degree + this->knots.size() + i] =
            this->knots.back() + (this->knots[i + 1] - this->knots.front());
      }
      this->knots = periodic_knots;

      // Pad the control points
      this->control_points.insert(
          this->control_points.end(),
          this->control_points.begin(),
          this->control_points.begin() + this->degree
      );

      break;
    }
    case CLAMPED:
    {
      this->knots.insert(
          this->knots.begin(), this->degree, this->knots.front()
      );
      this->knots.insert(this->knots.end(), this->degree, this->knots.back());
      break;
    }
    case OPEN:
    default:
      break;
    }
  }

  void apply_extrapolation(bool left, bool right, size_t &k, T &point) const
  {
    switch (this->extrapolation)
    {
    case NONE:
      throw std::invalid_argument("Point outside the domain");
    case CONSTANT:
      if (left)
      {
        k     = this->degree;
        point = this->knots[k];
      }
      else if (right)
      {
        k     = this->knots.size() - this->degree - 1;
        point = this->knots[k];
      }
      break;
    default:
      throw std::invalid_argument("Invalid extrapolation type");
    }
  }
};

} // namespace bsplinex

#endif
