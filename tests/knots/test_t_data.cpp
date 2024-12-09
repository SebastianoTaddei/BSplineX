// Standard includes

// Third-party includes
#include <catch2/catch_test_macros.hpp>

// BSplineX includes
#include "BSplineX/knots/t_data.hpp"

using namespace bsplinex;
using namespace bsplinex::knots;

TEST_CASE("knots::Data<double, Curve::UNIFORM> data{begin, end, step}", "[t_data]")
{
  Data<double, Curve::UNIFORM> data{0.0, 10.0, 2.3};

  SECTION("data.size()") { REQUIRE(data.size() == 5); }
  SECTION("data.at(...)")
  {
    for (size_t i{0}; i < data.size(); i++)
    {
      REQUIRE(data.at(i) == i * 2.3);
    }
  }
  SECTION("data.slice(...)")
  {
    std::vector<double> slice = data.slice(1, 4);
    REQUIRE(slice.size() == 3);
    for (size_t i{0}; i < slice.size(); i++)
    {
      REQUIRE(slice.at(i) == data.at(i + 1));
    }
  }
}

TEST_CASE("knots::Data<double, Curve::UNIFORM> data{begin, end, num_elems}", "[t_data]")
{
  Data<double, Curve::UNIFORM> data{0.0, 10.0, (size_t)5};

  SECTION("data.size()") { REQUIRE(data.size() == 5); }
  SECTION("data.at(...)")
  {
    for (size_t i{0}; i < data.size(); i++)
    {
      REQUIRE(data.at(i) == i * 2.5);
    }
  }
  SECTION("data.slice(...)")
  {
    std::vector<double> slice = data.slice(1, 4);
    REQUIRE(slice.size() == 3);
    for (size_t i{0}; i < slice.size(); i++)
    {
      REQUIRE(slice.at(i) == data.at(i + 1));
    }
  }
}

TEST_CASE("knots::Data<double, Curve::NON_UNIFORM> data{data_vec}", "[t_data]")
{
  std::vector<double> data_vec{0.0, 1.3, 2.2, 4.9, 13.2};
  Data<double, Curve::NON_UNIFORM> data{data_vec};

  SECTION("data.size()") { REQUIRE(data.size() == 5); }
  SECTION("data.at(...)")
  {
    for (size_t i{0}; i < data.size(); i++)
    {
      REQUIRE(data.at(i) == data_vec.at(i));
    }
  }
  SECTION("data.slice(...)")
  {
    std::vector<double> slice = data.slice(1, 4);
    REQUIRE(slice.size() == 3);
    for (size_t i{0}; i < slice.size(); i++)
    {
      REQUIRE(slice.at(i) == data.at(i + 1));
    }
  }
}
