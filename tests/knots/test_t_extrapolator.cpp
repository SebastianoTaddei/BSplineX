// Standard includes

// Third-party includes
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// BSplineX includes
#include "BSplineX/knots/t_extrapolator.hpp"

using namespace Catch::Matchers;
using namespace bsplinex;
using namespace bsplinex::knots;

TEST_CASE(
    "knots::Extrapolator<T, C, BC, Extrapolation::NONE> extrapolator{atter}", "[t_extrapolator]"
)
{
  std::vector<double> data_vec{0.1, 1.3, 2.2, 4.9, 13.2};
  Data<double, Curve::NON_UNIFORM> data{data_vec};
  size_t degree{3};
  Atter<double, Curve::NON_UNIFORM, BoundaryCondition::CLAMPED> atter{data, degree};
  Extrapolator<double, Curve::NON_UNIFORM, BoundaryCondition::CLAMPED, Extrapolation::NONE>
      extrapolator{atter, degree};

  SECTION("extrapolator.extrapolate()")
  {
    REQUIRE_THROWS_AS(extrapolator.extrapolate(-1.0), std::runtime_error);
    REQUIRE_THROWS_AS(extrapolator.extrapolate(15.0), std::runtime_error);
  }
}

TEST_CASE(
    "knots::Extrapolator<T, C, BC, Extrapolation::CONSTANT> "
    "extrapolator{atter}",
    "[t_extrapolator]"
)
{
  std::vector<double> data_vec{0.1, 1.3, 2.2, 4.9, 13.2};
  Data<double, Curve::NON_UNIFORM> data{data_vec};
  size_t degree{3};
  Atter<double, Curve::NON_UNIFORM, BoundaryCondition::CLAMPED> atter{data, degree};
  Extrapolator<double, Curve::NON_UNIFORM, BoundaryCondition::CLAMPED, Extrapolation::CONSTANT>
      extrapolator{atter, degree};

  SECTION("extrapolator.extrapolate()")
  {
    REQUIRE(extrapolator.extrapolate(-1.0) == atter.at(0));
    REQUIRE(extrapolator.extrapolate(15.0) == atter.at(atter.size() - 1));
  }
}

TEST_CASE(
    "knots::Extrapolator<T, C, BC, Extrapolation::PERIODIC> "
    "extrapolator{atter}",
    "[t_extrapolator]"
)
{
  std::vector<double> data_vec{0.1, 1.3, 2.2, 4.9, 13.2};
  Data<double, Curve::NON_UNIFORM> data{data_vec};
  size_t degree{3};
  Atter<double, Curve::NON_UNIFORM, BoundaryCondition::PERIODIC> atter{data, degree};
  Extrapolator<double, Curve::NON_UNIFORM, BoundaryCondition::PERIODIC, Extrapolation::PERIODIC>
      extrapolator{atter, degree};
  double period = data.at(data.size() - 1) - data.at(0);

  SECTION("extrapolator.extrapolate()")
  {
    REQUIRE_THAT(extrapolator.extrapolate(-1.0 - 2 * period), WithinRel(12.1));
    REQUIRE_THAT(extrapolator.extrapolate(-1.0 - period), WithinRel(12.1));
    REQUIRE_THAT(extrapolator.extrapolate(-1.0), WithinRel(12.1));
    REQUIRE_THAT(extrapolator.extrapolate(14.0), WithinRel(0.9));
    REQUIRE_THAT(extrapolator.extrapolate(14.0 + period), WithinRel(0.9));
    REQUIRE_THAT(extrapolator.extrapolate(14.0 + 2 * period), WithinRel(0.9));
  }
}
