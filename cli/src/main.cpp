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

  boost::optional<const ptree&> simtree_opt = tree.get_child_optional("simulation");

  if( !simtree_opt)
    throw std::runtime_error("no simulation specified");

  ptree const &simtree = simtree_opt.get();
  unique_ptr<model> p_model = load_model(simtree.get_child("model"), reg);

  size_t nevents = simtree.get<size_t>("nevents");

  auto event_visitor = [](event const &e) {
    std::cout << e.time() << " " << e.coord(0, 0) << std::endl;
  };

  p_model->generate_events(event_visitor, nevents);
}
