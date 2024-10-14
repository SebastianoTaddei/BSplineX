// Standard includes

// Third-party includes
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// BSplineX includes
#include "knots/t_finder.hpp"

using namespace Catch::Matchers;
using namespace bsplinex;
using namespace bsplinex::knots;

TEST_CASE("knots::Finder<T, C, BC, EXT> finder{atter}", "[t_finder]")
{
  std::vector<double> data_vec{0.1, 1.3, 2.2, 2.2, 4.9, 6.3, 6.3, 6.3, 13.2};
  Data<double, Curve::NON_UNIFORM> data{data_vec};
  size_t degree{3};
  Atter<double, Curve::NON_UNIFORM, BoundaryCondition::PERIODIC> atter{
      data, degree
  };
  Finder<
      double,
      Curve::NON_UNIFORM,
      BoundaryCondition::PERIODIC,
      Extrapolation::PERIODIC>
      finder{atter};

  SECTION("finder.find()")
  {
    REQUIRE(finder.find(-1.0) == 10);
    REQUIRE(finder.find(0.1) == 3);
    REQUIRE(finder.find(2.0) == 4);
    REQUIRE(finder.find(2.2) == 6);
    REQUIRE(finder.find(6.3) == 10);
    REQUIRE(finder.find(13.2) == 3);
    REQUIRE(finder.find(14.0) == 3);
  }
}
