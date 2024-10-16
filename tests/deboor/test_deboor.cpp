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

  // To generate this reference data use the `reference.py` script in this
  // folder
  // clang-format off
  std::vector<double> x_values{2.2, 2.3000000000000003, 2.4000000000000004, 2.5000000000000004, 2.6000000000000005, 2.7000000000000006, 2.8000000000000007, 2.900000000000001, 3.000000000000001, 3.100000000000001, 3.200000000000001, 3.300000000000001, 3.4000000000000012, 3.5000000000000013, 3.6000000000000014, 3.7000000000000015, 3.8000000000000016, 3.9000000000000017, 4.000000000000002, 4.100000000000001, 4.200000000000002, 4.3000000000000025, 4.400000000000002, 4.500000000000002, 4.600000000000002, 4.700000000000003, 4.8000000000000025, 4.900000000000002, 5.000000000000003, 5.100000000000003, 5.200000000000003, 5.3000000000000025, 5.400000000000003, 5.5000000000000036, 5.600000000000003, 5.700000000000003, 5.800000000000003, 5.900000000000004, 6.0000000000000036, 6.100000000000003, 6.200000000000004};
  std::vector<double> y_values{0.4, 0.4987905929445725, 0.5953834608104188, 0.6901102371457323, 0.7833025554987059, 0.8752920494175335, 0.9664103524504085, 1.0569890981455239, 1.1473599200510731, 1.2378544517152497, 1.3288043266862466, 1.420541178512258, 1.5133966407414765, 1.6077023469220952, 1.7037899306023085, 1.8019910253303089, 1.9026372646542904, 2.0060602821224456, 2.112591711282968, 2.2225631856840518, 2.3363063388738903, 2.4541528044006755, 2.5764342158126015, 2.7034822066578617, 2.83562841048465, 2.9732044608411603, 3.1165419912755823, 3.2659726353361123, 3.4243850625148546, 3.604896086079547, 3.8231795552418366, 4.094909319213373, 4.435759227205808, 4.861403128430789, 5.387514872099959, 6.029768307424969, 6.8038372836174785, 7.725395649889126, 8.810117255451555, 10.073675949516419, 11.53174558129538};
  // clang-format on

  SECTION("deboor.deboor(...)")
  {
    for (size_t i{0}; i < x_values.size(); i++)
    {
      REQUIRE_THAT(
          deboor.deboor(knots.find(x_values.at(i)), x_values.at(i)),
          WithinRel(y_values.at(i))
      );
    }
  }
}
