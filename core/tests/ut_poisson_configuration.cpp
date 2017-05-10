#include "configurations/poisson_configuration.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_list.hpp"
#include "interface/particle.hpp"

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
  BOOST_REQUIRE_EQUAL(config.size(), size);
  int cnt = 0;
  std::size_t idx = config.position_attr_idx();
  config.visit_records([=, &cnt](attribute_list const &p) {
    BOOST_REQUIRE_LE(p[idx].get_real_ref(0), +halflen);
    BOOST_REQUIRE_GE(p[idx].get_real_ref(0), -halflen);
    BOOST_REQUIRE_LE(p[idx].get_real_ref(1), +halflen);
    BOOST_REQUIRE_GE(p[idx].get_real_ref(1), -halflen);
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
  BOOST_REQUIRE_EQUAL(config.size(), size);
  int cnt = 0;
  std::size_t idx = config.position_attr_idx();
  config.visit_records([=,&cnt](attribute_list const &p) {
    double x = p[idx].get_real_ref(0);
    double y = p[idx].get_real_ref(1);
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

  std::size_t size = config.size();

  int cnt = 0;
  config.visit_records([&cnt](attribute_list const &) { ++cnt; });

  BOOST_REQUIRE_EQUAL(cnt, size);
}

BOOST_AUTO_TEST_SUITE_END()
