#include "launcher.hpp"

#include <iostream>
#include <memory>

#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "argument_parser.hpp"
#include "builtin_models.hpp"

#include "event.hpp"
#include "launch_utils.hpp"
#include "model.hpp"
#include "model_factory.hpp"
#include "model_register.hpp"
#include "properties.hpp"

int main(int argc, const char **argv)
{

  using namespace std;
  using namespace boost::property_tree;

  using namespace simbad::cli;
  using namespace simbad::core;
  using namespace simbad::models::common;

  model_register const &reg = get_builtin_models();

  properties prop;
  prop.add_properties_cmd(argc, argv);

  ptree const &tree = prop.get_property_tree();
  boost::optional<std::string> param_fname =
      tree.get_optional<std::string>("positional_cmd_arg.0");
  if (param_fname)
    prop.add_properties_auto(param_fname.get());

  std::string mode = tree.get<std::string>("mode");

  if (mode == "print")
  {
    info_parser::write_info(std::cerr, tree);
    return 0;
  }
  else if (mode != "run")
    throw std::runtime_error("unrecognized mode \"" + mode + "\"");

  unique_ptr<model> p_model = load_model(tree.get_child("model"), reg);

  size_t nevents = tree.get<size_t>("nevents");

  auto event_visitor = [](event const &e) {
    std::cout << e.time() << " " << e.coord(0, 0) << std::endl;
  };

  p_model->generate_events(event_visitor, nevents);
}
