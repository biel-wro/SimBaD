#include <boost/test/auto_unit_test.hpp>

#include "interface/attribute_description.hpp"

#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>

#include <iostream>
using namespace boost;
using namespace boost::multi_index;
using namespace simbad::core;

BOOST_AUTO_TEST_SUITE(test_attribute_descriptor)

BOOST_AUTO_TEST_CASE(instantiation)
{
  attribute_description c;
  c.insert(attribute_descriptor(1, "prop1"));
  c.insert(attribute_descriptor(2, "prop2"));
  c.insert(attribute_descriptor(3, "prop3"));
  c.insert(attribute_descriptor(4, "prop4"));
  c.insert(attribute_descriptor(5, "prop5"));

  std::size_t cnt = 0;
  for(attribute_description::index_iterator it = c.begin_indices(),
                                            end = c.end_indices();
      it != end; ++it)
  {
    attribute_descriptor const &desc = *it;
    (void)desc;
    ++cnt;
  }

  BOOST_REQUIRE_EQUAL(cnt, 5);
}

BOOST_AUTO_TEST_CASE(next_unused_idx)
{
  attribute_description c;
  c.insert(attribute_descriptor(1, "prop1"));
  c.insert(attribute_descriptor(2, "prop2"));
  c.insert(attribute_descriptor(4, "prop4"));
  c.insert(attribute_descriptor(5, "prop5"));

  BOOST_REQUIRE_EQUAL(c.next_unused_idx(0), 0);
  BOOST_REQUIRE_EQUAL(c.next_unused_idx(1), 3);
  BOOST_REQUIRE_EQUAL(c.next_unused_idx(2), 3);
  BOOST_REQUIRE_EQUAL(c.next_unused_idx(3), 3);
  BOOST_REQUIRE_EQUAL(c.next_unused_idx(4), 6);
  BOOST_REQUIRE_EQUAL(c.next_unused_idx(5), 6);
  BOOST_REQUIRE_EQUAL(c.next_unused_idx(6), 6);
}

BOOST_AUTO_TEST_CASE(add_attributes)
{
  // using index_map_type = boost::bimap<std::size_t, std::string>;
  // index_map_type im;
}

BOOST_AUTO_TEST_SUITE_END()
