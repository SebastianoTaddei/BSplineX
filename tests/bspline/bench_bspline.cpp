// Standard includes
#include <cmath>
#include <string>

// Third-party includes
#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>

// BSplineX includes
#include "bspline/bspline.hpp"

using namespace bsplinex;
using namespace bsplinex::bspline;

/**
 * Rerefence data can be generated using the `reference_sbpline.py` file in the
 * tests folder
 */

TEST_CASE(
    "benchmark bspline::BSpline<double, Curve::UNIFORM, "
    "BoundaryCondition::OPEN, Extrapolation::NONE>",
    "[bspline]"
)
{
  size_t degree{3};

  knots::Data<double, Curve::UNIFORM> t_data{0.1, 13.2, (size_t)9};

  std::vector<double> c_data_vec{0.1, 1.3, 2.2, 4.9, 13.2};
  control_points::Data<double> c_data{c_data_vec};

  BENCHMARK_ADVANCED("bspline.construct")(Catch::Benchmark::Chronometer meter)
  {
    std::vector<Catch::Benchmark::storage_for<BSpline<
        double,
        Curve::UNIFORM,
        BoundaryCondition::OPEN,
        Extrapolation::NONE>>>
        storage(meter.runs());
    meter.measure([&](int i) { storage[i].construct(t_data, c_data, degree); });
  };

  BENCHMARK_ADVANCED("bspline.destroy")(Catch::Benchmark::Chronometer meter)
  {
    std::vector<Catch::Benchmark::destructable_object<BSpline<
        double,
        Curve::UNIFORM,
        BoundaryCondition::OPEN,
        Extrapolation::NONE>>>
        storage(meter.runs());
    for (auto &&o : storage)
      o.construct(t_data, c_data, degree);
    meter.measure([&](int i) { storage[i].destruct(); });
  };

  BSpline<double, Curve::UNIFORM, BoundaryCondition::OPEN, Extrapolation::NONE>
      bspline{t_data, c_data, degree};

  auto fill =
      [](double start, double stop, size_t steps, std::vector<double> &vec)
  {
    double step = (stop - start) / (double)steps;
    vec.resize(steps);
    for (size_t i{0}; i < steps; i++)
    {
      vec.at(i) = start + (double)i * step;
    }
  };

  std::vector<double> x_data{};
  size_t elem_start{1};
  double res{0.0};
  double power{0.0};
  double start{5.2};
  double stop{7.8};
  for (size_t i{0}; i < 6; i++)
  {
    power = std::pow(10.0, i);
    fill(start, stop, elem_start * power, x_data);
    res = 0.0;
    BENCHMARK("bspline.evaluate - " + std::to_string(std::floor(power)))
    {
      for (auto &x : x_data)
      {
        res = bspline.evaluate(x);
      }
      return res;
    };
  }
}

TEST_CASE(
    "benchmark bspline::BSpline<double, Curve::NON_UNIFORM, "
    "BoundaryCondition::OPEN, Extrapolation::NONE>",
    "[bspline]"
)
{
  size_t degree{3};

  std::vector<double> t_data_vec{0.1, 1.3, 2.2, 2.2, 4.9, 6.3, 6.3, 6.3, 13.2};
  knots::Data<double, Curve::NON_UNIFORM> t_data{t_data_vec};

  std::vector<double> c_data_vec{0.1, 1.3, 2.2, 4.9, 13.2};
  control_points::Data<double> c_data{c_data_vec};

  BENCHMARK_ADVANCED("bspline.construct")(Catch::Benchmark::Chronometer meter)
  {
    std::vector<Catch::Benchmark::storage_for<BSpline<
        double,
        Curve::NON_UNIFORM,
        BoundaryCondition::OPEN,
        Extrapolation::NONE>>>
        storage(meter.runs());
    meter.measure([&](int i) { storage[i].construct(t_data, c_data, degree); });
  };

  BENCHMARK_ADVANCED("bspline.destroy")(Catch::Benchmark::Chronometer meter)
  {
    std::vector<Catch::Benchmark::destructable_object<BSpline<
        double,
        Curve::NON_UNIFORM,
        BoundaryCondition::OPEN,
        Extrapolation::NONE>>>
        storage(meter.runs());
    for (auto &&o : storage)
      o.construct(t_data, c_data, degree);
    meter.measure([&](int i) { storage[i].destruct(); });
  };

  BSpline<
      double,
      Curve::NON_UNIFORM,
      BoundaryCondition::OPEN,
      Extrapolation::NONE>
      bspline{t_data, c_data, degree};

  auto fill =
      [](double start, double stop, size_t steps, std::vector<double> &vec)
  {
    double step = (stop - start) / (double)steps;
    vec.resize(steps);
    for (size_t i{0}; i < steps; i++)
    {
      vec.at(i) = start + (double)i * step;
    }
  };

  std::vector<double> x_data{};
  size_t elem_start{1};
  double res{0.0};
  double power{0.0};
  double start{2.2};
  double stop{6.3};
  for (size_t i{0}; i < 6; i++)
  {
    power = std::pow(10.0, i);
    fill(start, stop, elem_start * power, x_data);
    res = 0.0;
    BENCHMARK("bspline.evaluate - " + std::to_string(std::floor(power)))
    {
      for (auto &x : x_data)
      {
        res = bspline.evaluate(x);
      }
      return res;
    };
  }
}
