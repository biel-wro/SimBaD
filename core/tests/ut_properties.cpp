#include <boost/test/auto_unit_test.hpp>

#include <iostream>

#include "properties.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace pt = boost::property_tree;

BOOST_AUTO_TEST_CASE(ptree_experiments) {
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

    tree.add("test.array","element1");
    tree.add("test.array","element2");

    pt::write_xml( std::cout, tree, pt::xml_writer_make_settings<std::string>(' ',2) );

    size_t counter = 0;

    for( auto const &node : tree.get_child("array") )
    {
        if( counter== 1)
            BOOST_REQUIRE_EQUAL(node.second.data(),"element2");
        else if (counter == 2)
            BOOST_REQUIRE_EQUAL(node.second.data(),"element1");

        counter++;
    }
    BOOST_REQUIRE_EQUAL(counter, 2 );
}


BOOST_AUTO_TEST_CASE(tree_merging) {
  pt::ptree tree1, tree2;

  tree1.put("model.class", "model name");
  tree1.put("model.parameters.k", 4.0);

  tree2.put("model.parameters.k", 3.5);

  simbad::core::properties::merge_trees(tree1, tree2);

  // pt::write_xml( std::cout, tree1,
  // pt::xml_writer_make_settings<std::string>(' ',2) );
  BOOST_REQUIRE_CLOSE(tree2.get<double>("model.parameters.k"), 3.5, 0.001);
}
