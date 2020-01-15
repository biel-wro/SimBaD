#include "cli/argument_parser.hpp"
#include "cli/launcher.hpp"

#include "utils/properties.hpp"

#include "plugin_common.hpp"

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



  boost::optional<std::string> stdin_format =
      tree.get_optional<std::string>("stdin");
  if(stdin_format)
  {
    std::string format = stdin_format.get();
    if("simbad" == format || "info" == format)
      prop.add_properties_info(std::cin);
    else if("json" == format)
      prop.add_properties_json(std::cin);
    else if("xml" == format)
      prop.add_properties_xml(std::cin);
    else if("ini" == format)
      prop.add_properites_ini(std::cin);
    else
      throw std::runtime_error("unrecognized stdin format:" + format);
  }

  return tree;
}

int main(int argc, const char **argv)
{
  simbad::plugins::initialize_plugins();

  core::property_tree tree = get_args(argc, argv);
  pt::json_parser::write_json(
      std::cerr, static_cast<core::property_tree::super>(tree), true);

  simbad::cli::launcher l(tree);
  l.launch();
}
