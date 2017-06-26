#include "argument_parser.hpp"
#include "launcher.hpp"

#include "utils/properties.hpp"

#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <iostream>
#include <memory>

namespace core = simbad::core;
namespace pt = boost::property_tree;

core::property_tree get_args(int argc, const char **argv)
{
  core::properties prop;
  core::argument_parser parser(argc, argv);
  prop.add_properties(parser.get_property_tree());

  pt::ptree const &tree = prop.get_property_tree();
  boost::optional<std::string> param_fname =
      tree.get_optional<std::string>("positional_cmd_arg.0");

  if(param_fname)
    prop.add_properties_auto(param_fname.get());

  return tree;
}

int main(int argc, const char **argv)
{
  core::property_tree tree = get_args(argc, argv);
  pt::json_parser::write_json(
      std::cerr, static_cast<core::property_tree::super>(tree), true);

  simbad::cli::launcher l(tree);
  l.launch(tree);
}
