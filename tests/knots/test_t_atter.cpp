// Standard includes

// Third-party includes
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// BSplineX includes
#include "knots/t_atter.hpp"

using namespace Catch::Matchers;
using namespace bsplinex;
using namespace bsplinex::knots;

TEST_CASE(
    "knots::Atter<T, C, BC> atter{knots::Data<T, C> data, degree}", "[t_atter]"
)
{
  std::vector<double> data_vec{0.1, 1.3, 2.2, 4.9, 13.2};
  Data<double, Curve::NON_UNIFORM> data{data_vec};
  size_t degree{3};
  Atter<double, Curve::NON_UNIFORM, BoundaryCondition::PERIODIC> atter{
      data, degree
  };

  SECTION("atter.size()") { REQUIRE(atter.size() == data.size() + 2 * degree); }
  SECTION("atter.get_degree()") { REQUIRE(atter.get_degree() == degree); }
  SECTION("atter.at(...)")
  {
    REQUIRE_THAT(atter.at(0), WithinRel(-11.8));
    REQUIRE_THAT(atter.at(1), WithinRel(-10.9));
    REQUIRE_THAT(atter.at(2), WithinRel(-8.2));
    for (size_t i{0}; i < data.size(); i++)
    {
      REQUIRE(atter.at(i + degree) == data.at(i));
    }
    REQUIRE_THAT(atter.at(data.size() + degree), WithinRel(14.4));
    REQUIRE_THAT(atter.at(data.size() + degree + 1), WithinRel(15.3));
    REQUIRE_THAT(atter.at(data.size() + degree + 2), WithinRel(18.0));
  }
  SECTION("atter.begin()")
  {
    auto it = atter.begin();
    REQUIRE_THAT(*it, WithinRel(-11.8));
  }
  SECTION("atter.end()")
  {
    auto it = atter.end();
    it--;
    REQUIRE_THAT(*it, WithinRel(18.0));
  }
}

TEST_CASE("knots::Atter::iterator", "[t_atter]")
{
  std::vector<double> data_vec{0.1, 1.3, 2.2, 4.9, 13.2};
  Data<double, Curve::NON_UNIFORM> data{data_vec};
  size_t degree{3};
  Atter<double, Curve::NON_UNIFORM, BoundaryCondition::PERIODIC> atter{
      data, degree
  };

  SECTION("*iterator")
  {
    auto it = atter.begin();
    REQUIRE_THAT(*it, WithinRel(-11.8));
  }
  SECTION("iterator++")
  {
    auto it = atter.begin();
    REQUIRE_THAT(*(it++), WithinRel(-11.8));
    REQUIRE_THAT(*it, WithinRel(-10.9));
  }
  SECTION("++iterator")
  {
    auto it = atter.begin();
    REQUIRE_THAT(*(++it), WithinRel(-10.9));
    REQUIRE_THAT(*it, WithinRel(-10.9));
  }
  SECTION("iterator--")
  {
    auto it = atter.end();
    it--;
    REQUIRE_THAT(*(it--), WithinRel(18.0));
    REQUIRE_THAT(*it, WithinRel(15.3));
  }
  SECTION("--iterator")
  {
    auto it = atter.end();
    REQUIRE_THAT(*(--it), WithinRel(18.0));
    REQUIRE_THAT(*it, WithinRel(18.0));
  }
  SECTION("iterator += 1")
  {
    auto it = atter.begin();
    REQUIRE_THAT(*(it += 1), WithinRel(-10.9));
    REQUIRE_THAT(*it, WithinRel(-10.9));
  }
  SECTION("iterator + 1")
  {
    auto it = atter.begin();
    REQUIRE_THAT(*(it + 1), WithinRel(-10.9));
    REQUIRE_THAT(*it, WithinRel(-11.8));
  }
  SECTION("iterator -= 1")
  {
    auto it = atter.end();
    REQUIRE_THAT(*(it -= 1), WithinRel(18.0));
    REQUIRE_THAT(*it, WithinRel(18.0));
  }
  SECTION("iterator - 1")
  {
    auto it = atter.end();
    it--;
    REQUIRE_THAT(*(it - 1), WithinRel(15.3));
    REQUIRE_THAT(*it, WithinRel(18.0));
  }
  SECTION("iterator - iterator")
  {
    auto it1 = atter.begin();
    auto it2 = atter.end();
    REQUIRE((it2 - it1) == (int)atter.size());
  }
  SECTION("iterator == iterator")
  {
    auto it1 = atter.begin();
    auto it2 = atter.begin();
    REQUIRE(it1 == it2);
  }
  SECTION("iterator != iterator")
  {
    auto it1 = atter.begin();
    auto it2 = atter.end();
    REQUIRE(it1 != it2);
  }
  SECTION("iterator[]")
  {
    auto it = atter.begin();
    REQUIRE_THAT(it[atter.size() - 2], WithinRel(15.3));
  }
  SECTION("iterator > iterator")
  {
    auto it1 = atter.begin();
    auto it2 = atter.end();
    auto it3 = atter.begin();
    REQUIRE(it2 > it1);
    REQUIRE_FALSE(it1 > it2);
    REQUIRE_FALSE(it3 > it1);
  }
  SECTION("iterator < iterator")
  {
    auto it1 = atter.begin();
    auto it2 = atter.end();
    auto it3 = atter.begin();
    REQUIRE(it1 < it2);
    REQUIRE_FALSE(it2 < it1);
    REQUIRE_FALSE(it1 < it3);
  }
  SECTION("iterator >= iterator")
  {
    auto it1 = atter.begin();
    auto it2 = atter.end();
    auto it3 = atter.begin();
    REQUIRE(it2 >= it1);
    REQUIRE(it3 >= it1);
    REQUIRE_FALSE(it1 >= it2);
  }
  SECTION("iterator <= iterator")
  {
    auto it1 = atter.begin();
    auto it2 = atter.end();
    auto it3 = atter.begin();
    REQUIRE(it1 <= it2);
    REQUIRE(it1 <= it3);
    REQUIRE_FALSE(it2 <= it1);
  }
}
