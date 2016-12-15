#include "utils/generic_particle.hpp"
#include "interface/attribute_descriptor.hpp"
#include <boost/test/auto_unit_test.hpp>
using namespace simbad::core;
BOOST_AUTO_TEST_SUITE(test_generic_particle)
BOOST_AUTO_TEST_CASE(load_from_property_tree)
{
  property_tree pt;
  pt.put("a", 1.0);
  pt.put("b", 2);
  pt.put("c.1", 3.0);
  pt.put("c.2", 4);
  pt.put("d", "5");

  std::vector<attribute_descriptor> descriptors;
  descriptors.emplace_back("a", 0);
  descriptors.emplace_back("b", 1);
  descriptors.emplace_back("c.1", 2);
  descriptors.emplace_back("c.2", 3);
  descriptors.emplace_back("d", 4);

  //generic_particle gp(pt, descriptors);
  //attribute const &attr = gp.find_attribute(0);



}

BOOST_AUTO_TEST_SUITE_END()
