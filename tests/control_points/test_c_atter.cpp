// Third-party includes
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// BSplineX includes
#include "control_points/c_atter.hpp"

using namespace Catch::Matchers;
using namespace bsplinex;
using namespace bsplinex::control_points;

TEST_CASE(
    "control_points::Atter<T, BC> atter{control_points::Data<T> data, degree}",
    "[c_atter]"
)
{
  std::vector<double> data_vec{0.1, 1.3, 2.2, 4.9, 13.2};
  Data<double> data{data_vec};
  size_t degree{3};
  Atter<double, BoundaryCondition::PERIODIC> atter{data, degree};

  SECTION("atter.size()") { REQUIRE(atter.size() == data.size() + degree); }
  SECTION("atter.get_degree()") { REQUIRE(atter.get_degree() == degree); }
  SECTION("atter.at(...)")
  {
    for (size_t i{0}; i < data.size(); i++)
    {
      REQUIRE(atter.at(i) == data.at(i));
    }
    REQUIRE(atter.at(data.size()) == 0.1);
    REQUIRE(atter.at(data.size() + 1) == 1.3);
    REQUIRE(atter.at(data.size() + 2) == 2.2);
  }
}
