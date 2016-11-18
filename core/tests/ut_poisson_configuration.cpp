#include "poisson_configuration.hpp"
#include <boost/test/auto_unit_test.hpp>
#include <iostream>

using namespace simbad::core;

BOOST_AUTO_TEST_SUITE(ut_poisson_configuration)

BOOST_AUTO_TEST_CASE(cube_fixed_size)
{
  const int dimension = 2;
  const std::size_t size = 100;
  const double halflen = 2.34;
  simbad::core::poisson_configuration config(dimension, size, halflen);
  BOOST_REQUIRE_EQUAL(config.configuration_size(), size);
  int cnt = 0;
  config.visit_configuration([&cnt, halflen](particle const &p) {
    BOOST_REQUIRE_LE(p.coord(0), +halflen);
    BOOST_REQUIRE_GE(p.coord(0), -halflen);
    BOOST_REQUIRE_LE(p.coord(1), +halflen);
    BOOST_REQUIRE_GE(p.coord(1), -halflen);
    ++cnt;
  });

  BOOST_REQUIRE_EQUAL(cnt, size);
}

BOOST_AUTO_TEST_CASE(ball_fixed_size)
{
  const int dimension = 2;
  const std::size_t size = 100;
  const double radius = 17.3;
  simbad::core::poisson_configuration config(dimension, size, radius, true);
  BOOST_REQUIRE_EQUAL(config.configuration_size(), size);
  int cnt = 0;
  config.visit_configuration([&cnt, radius](particle const &p) {
    double x = p.coord(0);
    double y = p.coord(1);
    double r = std::sqrt(x * x + y * y);
    BOOST_REQUIRE_LE(r, radius);
    ++cnt;
  });

  BOOST_REQUIRE_EQUAL(cnt, size);
}

BOOST_AUTO_TEST_CASE(construct_from_property_tree)
{
  property_tree pt;
  pt.put("dimension", 2);
  pt.put("lambda", 10.0);
  pt.put("radius", 13.3);
  pt.put("ball", true);
  pt.put("seed", 0);

  simbad::core::poisson_configuration config(pt);

  std::size_t size = config.configuration_size();

  int cnt = 0;
  config.visit_configuration([&cnt](particle const &p) { ++cnt; });

  BOOST_REQUIRE_EQUAL(cnt, size);
}

BOOST_AUTO_TEST_SUITE_END()
