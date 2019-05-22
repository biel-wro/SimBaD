#include <boost/test/auto_unit_test.hpp>

#include "interface/attribute_tree.hpp"
#include "utils/properties.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <iostream>

namespace pt = boost::property_tree;

BOOST_AUTO_TEST_SUITE(test_properties)

BOOST_AUTO_TEST_CASE(ptree_experiments)
{
  pt::ptree tree;

  tree.put("model.class", "test model name");

  tree.put("model.parameters.alpha", 3.5);
  tree.put("model.parameters.k", 10);
  tree.put("model.time", 30);

  tree.put("output.format", "text");
  tree.put("output.file", "test.txt");
  tree.put("output.compression", false);

  BOOST_REQUIRE_CLOSE(tree.get<double>("model.parameters.alpha"), 3.5, 0.001);
  BOOST_REQUIRE_EQUAL(tree.get<int>("model.parameters.k"), 10);
  BOOST_REQUIRE_EQUAL(tree.get<int>("model.time"), 30);
}

BOOST_AUTO_TEST_CASE(ptree_experiments_arrays)
{
  pt::ptree tree;

  tree.add("test.array", "element1");
  tree.add("test.array", "element2");

  // pt::write_xml( std::cout, tree, pt::xml_writer_make_settings<char>(' ',2)
  // );

  size_t counter = 0;

  for(auto const &node : tree.get_child("test"))
  {
    if(counter == 1)
      BOOST_REQUIRE_EQUAL(node.second.data(), "element2");
    else if(counter == 2)
      BOOST_REQUIRE_EQUAL(node.second.data(), "element1");

    counter++;
  }
  BOOST_REQUIRE_EQUAL(counter, std::size_t(2));
}

BOOST_AUTO_TEST_CASE(tree_merging)
{
  pt::ptree tree1, tree2;

  tree1.put("model.class", "model name");
  tree1.put("model.parameters.k", 4.0);

  tree2.put("model.parameters.k", 3.5);

  simbad::core::properties::merge_trees(tree1, tree2);

  // pt::write_xml( std::cout, tree1,
  // pt::xml_writer_make_settings<std::string>(' ',2) );
  BOOST_REQUIRE_CLOSE(tree2.get<double>("model.parameters.k"), 3.5, 0.001);
}

BOOST_AUTO_TEST_CASE(new_property_tree)
{
  using namespace simbad::core;
  attribute_tree pt;

  /*
  pt.put("key1", "val1");
  pt.put("key2", "2.0");
  pt.put("key3", 3.0);
  pt.put("key4", 4);

  std::cout << pt.get<std::string>("key1") << std::endl;
  std::cout << pt.get<double>("key2") << std::endl;
  std::cout << pt.get<int>("key3") << std::endl;
  std::cout << pt.get<double>("key4") << std::endl;
  */
}
BOOST_AUTO_TEST_CASE(const_visit)
{
  using namespace simbad::core;
  property_tree pt;
  pt.put("prop1", 1.0);
  pt.put("prop2", 2.0);
  pt.put("prop3.prop4", 4.0);
  pt.put("prop3.prop5", "5.0");

  std::size_t cnt = 0;
  pt.visit([&](std::string const &name, std::string const &val) { ++cnt; },
           true);
  BOOST_REQUIRE_EQUAL(cnt, 4);
}

BOOST_AUTO_TEST_SUITE_END()
