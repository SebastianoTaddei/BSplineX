#ifndef BSPLINE_HPP
#define BSPLINE_HPP

// Standard includes
#include <sstream>
#include <vector>

// Third-party includes
#include <Eigen/Dense>

// BSplineX includes
#include "Eigen/src/Core/util/Constants.h"
#include "control_points/control_points.hpp"
#include "knots/knots.hpp"
#include "types.hpp"

namespace bsplinex::bspline
{

template <typename T, Curve C, BoundaryCondition BC, Extrapolation EXT>
class BSpline
{
private:
  knots::Knots<T, C, BC, EXT> knots;
  control_points::ControlPoints<T, BC> control_points;
  size_t degree{0};
  std::vector<T> support{};

public:
  BSpline(
      knots::Data<T, C> knots_data,
      control_points::Data<T> control_points_data,
      size_t degree
  )
      : knots{knots_data, degree}, control_points{control_points_data, degree},
        degree{degree}
  {
    this->check_sizes();
    this->support.resize(this->degree + 1);
  }

  T evaluate(T value)
  {
    auto index_value_pair = this->knots.find(value);
    return this->deboor(index_value_pair.first, index_value_pair.second);
  }

  std::vector<T> basis(T value)
  {
    std::vector<T> basis_functions(this->control_points.size(), 0.0);

    this->compute_basis(value, basis_functions.begin(), basis_functions.end());

    return basis_functions;
  }

  void fit(std::vector<T> const &x, std::vector<T> &y)
  {
    if (x.size() != y.size())
    {
      throw std::runtime_error("x and y must have the same size");
    }

    std::vector<T> raw_matrix(this->control_points.size() * x.size(), 0.0);

    for (size_t i{0}; i < x.size(); i++)
    {
      this->compute_basis(
          x.at(i),
          raw_matrix.begin() + i * this->control_points.size(),
          raw_matrix.begin() + (i + 1) * this->control_points.size()
      );
    }

    Eigen::Map<
        Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>
        A(raw_matrix.data(), x.size(), this->control_points.size());
    Eigen::Map<Eigen::VectorX<T>> b(y.data(), x.size());

    // Eigen::MatrixX<T> A{};
    //
    // if constexpr (BC == BoundaryCondition::PERIODIC)
    //{
    //   A.resize(y.size(), control_points.size() - this->degree);
    // }
    // else
    //{
    //   A.resize(y.size(), control_points.size());
    // }
    // Eigen::VectorX<T> b{};
    // b.resize(y.size());

    // for (size_t i{0}; i < y.size(); i++)
    //{
    //   auto basis = this->basis(x.at(i));
    //   b(i)       = y.at(i);
    //   for (size_t j{0}; j < control_points.size(); j++)
    //   {
    //     if constexpr (BC == BoundaryCondition::PERIODIC)
    //     {
    //       A(i, j % (control_points.size() - this->degree)) += basis.at(j);
    //     }
    //     else
    //     {
    //       A(i, j) = basis.at(j);
    //     }
    //   }
    // }

    Eigen::VectorX<T> res;
    if constexpr (BC == BoundaryCondition::PERIODIC)
    {
      A.leftCols(this->degree) += A.rightCols(this->degree);
      res = A.leftCols(this->control_points.size() - this->degree)
                .colPivHouseholderQr()
                .solve(b);
    }
    else
    {
      res = A.colPivHouseholderQr().solve(b);
    }

    this->control_points.set_data(
        {res.data(), res.data() + res.rows() * res.cols()}
    );

    return;
  }

  control_points::ControlPoints<T, BC> const &get_control_points()
  {
    return this->control_points;
  }

private:
  void check_sizes()
  {
    if (this->control_points.size() == this->knots.size() - this->degree - 1)
    {
      return;
    }

    std::stringstream ss{};
    ss << "Found control_points.size() != knots.size() - degree - 1 ("
       << this->control_points.size()
       << " != " << this->knots.size() - this->degree - 1 << "). ";

    // clang-format off

    if constexpr (BC == BoundaryCondition::OPEN)
    {
      ss << "With BoundaryCondition::OPEN no padding is added, therefore you need to respect: control_points_data.size() = knots_data.size() - degree - 1";
    }
    else if constexpr (BC == BoundaryCondition::CLAMPED)
    {
      ss << "With BoundaryCondition::CLAMPED padding is added to the knots, therefore you need to respect: control_points_data.size() = knots_data.size() + degree - 1";
    }
    else if constexpr (BC == BoundaryCondition::PERIODIC)
    {
      ss << "With BoundaryCondition::PERIODIC padding is added to the knots and control points, therefore you need to respect: control_points_data.size() = knots_data.size() - 1";
    }
    else {
     ss << "Unknown BoundaryCondition, you should not have arrived here ever!";
    }

    // clang-format on

    throw std::runtime_error(ss.str());
  }

  T deboor(size_t index, T value)
  {
    for (size_t j = 0; j <= this->degree; j++)
    {
      this->support[j] = this->control_points.at(j + index - this->degree);
    }

    T alpha = 0;
    for (size_t r = 1; r <= this->degree; r++)
    {
      for (size_t j = this->degree; j >= r; j--)
      {
        alpha = (value - this->knots.at(j + index - this->degree)) /
                (this->knots.at(j + 1 + index - r) -
                 this->knots.at(j + index - this->degree));
        this->support[j] =
            (1.0 - alpha) * this->support[j - 1] + alpha * this->support[j];
      }
    }

    return this->support[this->degree];
  }

  template <typename It>
  void compute_basis(T value, It begin, [[maybe_unused]] It end)
  {
    assertm(
        (end - begin) == (long long)this->control_points.size(),
        "Unexpected number of basis asked"
    );

    assertm(
        std::all_of(begin, end, [](T i) { return (T)0 == i; }),
        "Initial basis must be initialised to zero"
    );

    auto [index, val] = this->knots.find(value);

    assertm(begin + index < end, "Index outside of boundaries");

    *(begin + index) = 1.0;
    for (size_t d{1}; d <= this->degree; d++)
    {
      *(begin + index - d) = (knots.at(index + 1) - val) /
                             (knots.at(index + 1) - knots.at(index - d + 1)) *
                             *(begin + index - d + 1);
      for (size_t i{index - d + 1}; i < index; i++)
      {
        *(begin + i) = (val - knots.at(i)) / (knots.at(i + d) - knots.at(i)) *
                           *(begin + i) +
                       (knots.at(i + d + 1) - val) /
                           (knots.at(i + d + 1) - knots.at(i + 1)) *
                           *(begin + i + 1);
      }
      *(begin + index) = (val - knots.at(index)) /
                         (knots.at(index + d) - knots.at(index)) *
                         *(begin + index);
    }
  }
};

} // namespace bsplinex::bspline

#endif
