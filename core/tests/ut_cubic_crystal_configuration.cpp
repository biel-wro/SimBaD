#include "configurations/cubic_crystal_configuration.hpp"
#include "interface/attribute.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include <boost/optional.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <iostream>

using namespace simbad::core;
BOOST_AUTO_TEST_SUITE(test_cubic_crystal)

BOOST_AUTO_TEST_CASE(number_check)
{
  cubic_crystal_configuration configuration(2, 5, 0.46);

  constexpr std::size_t side_len = 2 * 10 + 1;
  constexpr std::size_t nparticles = side_len * side_len;

  BOOST_REQUIRE_EQUAL(configuration.configuration_size(), nparticles);

  std::size_t visited = 0;
  configuration.visit_configuration(
      [&visited](attribute_list const &p) { visited++; });

  BOOST_REQUIRE_EQUAL(visited, nparticles);
}

BOOST_AUTO_TEST_CASE(single_check_3d)
{
  cubic_crystal_configuration configuration(3, 0, 1);
  BOOST_REQUIRE_EQUAL(configuration.configuration_size(), 1);
  std::size_t idx = configuration.new_attr_map()
                        .get_descriptor(ATTRIBUTE_KIND::POSITION)
                        .get()
                        .attribute_idx();

  configuration.visit_configuration([=](attribute_list const &al) {
    BOOST_REQUIRE_EQUAL(al[idx].get_real_ref(0), 0);
    BOOST_REQUIRE_EQUAL(al[idx].get_real_ref(1), 0);
    BOOST_REQUIRE_EQUAL(al[idx].get_real_ref(2), 0);
  });
}

BOOST_AUTO_TEST_CASE(grid_check)
{
  constexpr std::size_t dim = 3;
  constexpr double radius = 2;
  constexpr double spacing = 1;
  constexpr std::size_t side = std::size_t(radius / spacing) * 2 + 1;

  cubic_crystal_configuration configuration(dim, radius, spacing);
  std::size_t nparticles = configuration.configuration_size();

  std::vector<bool> visited(nparticles, false);

  std::size_t idx = configuration.new_attr_map()
                        .get_descriptor(ATTRIBUTE_KIND::POSITION)
                        .get()
                        .attribute_idx();
  configuration.visit_configuration([=, &visited](attribute_list const &p) {
    std::size_t id0 = p[idx].get_real_ref(0) + radius;
    std::size_t id1 = side * (p[idx].get_real_ref(1) + radius);
    std::size_t id2 = side * side * (p[idx].get_real_ref(2) + radius);

    std::size_t id = id0 + id1 + id2;
    BOOST_REQUIRE_LT(id, visited.size());
    BOOST_REQUIRE(!visited[id]);
    visited[id] = true;
  });

  for(bool v : visited)
    BOOST_REQUIRE(v);
}

BOOST_AUTO_TEST_SUITE_END()
