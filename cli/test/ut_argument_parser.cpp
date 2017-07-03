#include <boost/test/auto_unit_test.hpp>

#include "argument_parser.hpp"

#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

using namespace simbad::core;
using namespace boost::property_tree;

BOOST_AUTO_TEST_CASE(simple_test)
{
  (void)1;
  const char *argv[] = {"myprogram", "positional1", "--arg1=bla",
                        "--arg.arg2=ble", "positional 2"};
  const int argc = sizeof(argv) / sizeof(char *);

  argument_parser arg_parser;
  arg_parser.parse(argc, argv);

  const ptree &tree = arg_parser.get_property_tree();

  // xml_parser::write_xml(std::cout,
  // tree,xml_writer_make_settings<ptree::key_type>(' ', 4));

  BOOST_REQUIRE_NO_THROW(tree.get<std::string>("arg1"));
  BOOST_REQUIRE_EQUAL(tree.get<std::string>("arg1"), "bla");

  BOOST_REQUIRE_NO_THROW(tree.get<std::string>("arg.arg2"));
  BOOST_REQUIRE_EQUAL(tree.get<std::string>("arg.arg2"), "ble");

  BOOST_REQUIRE_NO_THROW(tree.get<std::string>("positional_cmd_arg.0"));
  BOOST_REQUIRE_EQUAL(tree.get<std::string>("positional_cmd_arg.0"),
                      "positional1");

  BOOST_REQUIRE_NO_THROW(tree.get<std::string>("positional_cmd_arg.1"));
  BOOST_REQUIRE_EQUAL(tree.get<std::string>("positional_cmd_arg.1"),
                      "positional 2");
}
