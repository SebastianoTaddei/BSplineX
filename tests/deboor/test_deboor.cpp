// Third-party includes
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// BSplineX includes
#include "deboor/deboor.hpp"

using namespace Catch::Matchers;
using namespace bsplinex;
using namespace bsplinex::deboor;

TEST_CASE(
    "deboor::Deboor<T, C, BC, EXT> deboor{knots::Data<T, C> t_data, "
    "control_points::Data<T> c_data, degree}",
    "[deboor]"
)
{
  size_t degree{3};

  std::vector<double> t_data_vec{0.1, 1.3, 2.2, 2.2, 4.9, 6.3, 6.3, 6.3, 13.2};
  knots::Data<double, Curve::NON_UNIFORM> t_data{t_data_vec};
  knots::Knots<
      double,
      Curve::NON_UNIFORM,
      BoundaryCondition::OPEN,
      Extrapolation::NONE>
      knots{t_data, degree};

  std::vector<double> c_data_vec{0.1, 1.3, 2.2, 4.9, 13.2};
  control_points::Data<double> c_data{c_data_vec};
  control_points::ControlPoints<double, BoundaryCondition::OPEN> control_points{
      c_data, degree
  };

  DeBoor<
      double,
      Curve::NON_UNIFORM,
      BoundaryCondition::OPEN,
      Extrapolation::NONE>
      deboor{knots, control_points, degree};

  SECTION("deboor.deboor(...)")
  {
    REQUIRE_THAT(deboor.deboor(3, 0.2), WithinRel(-2.5480497896417282));
  }
}
