#include "argument_parser.hpp"
#include "launcher.hpp"

#include "utils/properties.hpp"

#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <iostream>
#include <memory>

using namespace simbad::core;
using namespace simbad::cli;
using namespace boost::property_tree;


int main(int argc, const char **argv)
{
  using namespace boost::property_tree;

  properties prop;
  argument_parser parser(argc, argv);
  prop.add_properties(parser.get_property_tree());

  ptree const &tree = prop.get_property_tree();
  boost::optional<std::string> param_fname =
      tree.get_optional<std::string>("positional_cmd_arg.0");

  if(param_fname)
    prop.add_properties_auto(param_fname.get());

  json_parser::write_json(std::cerr, static_cast<property_tree::super>(tree),
                          true);
  launcher l(tree);
  l.launch(tree);
}
