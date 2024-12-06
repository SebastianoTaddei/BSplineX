// Standard includes

// Third-party includes
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// BSplineX includes
#include "control_points/c_padder.hpp"

using namespace Catch::Matchers;
using namespace bsplinex;
using namespace bsplinex::control_points;

TEST_CASE(
    "control_points::Padder<T, BC> padder{control_points::Data<T> data, degree}", "[c_padder]"
)
{
  std::vector<double> data_vec{0.1, 1.3, 2.2, 4.9, 13.2};
  Data<double> data{data_vec};
  Padder<double, BoundaryCondition::OPEN> padder{data, 3};

  SECTION("padder.size()") { REQUIRE(padder.size() == 0); }
  SECTION("padder.size_right()") { REQUIRE(padder.size_right() == 0); }
  SECTION("padder.right()") { REQUIRE_THROWS_AS(padder.right(0), std::runtime_error); }
}

TEST_CASE(
    "control_points::Padder<T, BoundaryCondition::PERIODIC>
    "padder{control_points::Data<T> data, degree}",
    "[c_padder]"
)
{
  std::vector<double> data_vec{0.1, 1.3, 2.2, 4.9, 13.2};
  Data<double> data{data_vec};
  Padder<double, BoundaryCondition::PERIODIC> padder{data, 3};

  SECTION("padder.size()") { REQUIRE(padder.size() == 3); }
  SECTION("padder.size_right()") { REQUIRE(padder.size_right() == 3); }
  SECTION("padder.right(...)")
  {
    REQUIRE_THAT(padder.right(0), WithinRel(0.1));
    REQUIRE_THAT(padder.right(1), WithinRel(1.3));
    REQUIRE_THAT(padder.right(2), WithinRel(2.2));
  }
}
