#include <boost/test/auto_unit_test.hpp>

#include "interface/attribute.hpp"
#include "utils/attribute_converter.hpp"

#include "coordinates/coordinates.hpp"

#include <unordered_set>
#include <vector>

using namespace boost;
using namespace simbad::core;

void serialize(attribute const &attr) {}
BOOST_AUTO_TEST_SUITE(test_attribute_serialization)

BOOST_AUTO_TEST_CASE(hash)
{
  std::unordered_set<attribute> set;
  set.emplace("string");
}

BOOST_AUTO_TEST_SUITE_END()
