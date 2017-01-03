#include "argument_parser.hpp"
#include "builtin_models.hpp"
#include "launcher.hpp"

#include "configurations/cubic_crystal_configuration.hpp"
#include "configurations/poisson_configuration.hpp"
#include "configurations/stacked_view_configuration.hpp"
#include "interface/event.hpp"
#include "interface/model.hpp"
#include "interface/model_factory.hpp"
#include "interface/model_register.hpp"
#include "interface/property_tree.hpp"
#include "processors/csv_printer.hpp"
#include "processors/launch_utils.hpp"
#include "processors/snapshotter.hpp"
#include "processors/stream_printer.hpp"
#include "processors/text_configuration_printer.hpp"
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

static std::unique_ptr<stream_printer>
get_configuration_printer(std::ostream *os, property_tree const &pt)
{
  std::string const &class_name = pt.get<std::string>("class");
  property_tree const &parameters =
      pt.get_child("parameters", property_tree::get_empty());

  if(class_name == "csv_printer")
    return std::unique_ptr<stream_printer>(new csv_printer(os, parameters));
  throw std::logic_error(std::string("unrecognized printer ") + class_name);
}

static std::unique_ptr<model> get_model(property_tree const &pt)
{
  std::string model_name = pt.get<std::string>("class");

  model_register const &reg = simbad::models::common::get_builtin_models();
  model_factory const &factory = reg.get_model_factory(model_name);

  property_tree parameters_props = pt.get_child("parameters");

  return factory.create_instance(parameters_props);
}

static std::unique_ptr<configuration_view>
get_initial_config(property_tree const &pt)
{
  std::unique_ptr<configuration_view> p_configuraiton;
  std::string config_name = pt.get<std::string>("class");
  property_tree const &parameters = pt.get_child("parameters");
  if("poisson_configuration" == config_name)
    p_configuraiton.reset(new poisson_configuration(parameters));
  else if("cubic_crystal_configuration" == config_name)
    p_configuraiton.reset(new cubic_crystal_configuration(parameters));
  else
    throw std::runtime_error("unrecognized configuration " + config_name);
  return p_configuraiton;
}

void snapshots(property_tree const &pt)
{
  std::unique_ptr<model> p_model = get_model(pt.get_child("model"));

  auto opt_defattr = pt.get_child_optional("model.default_attributes");
  property_tree empty_properties;

  property_tree const &default_attr =
      opt_defattr ? opt_defattr.get() : empty_properties;

  std::unique_ptr<configuration_view> p_init_config =
      get_initial_config(pt.get_child("model.initial_configuration"));
  p_model->set_configuration(*p_init_config, default_attr);

  snapshotter processor(p_model.get(), pt.get<double>("time_step"));
  std::size_t nsteps = pt.get<std::size_t>("max_steps");

  std::unique_ptr<stream_printer> printer = get_configuration_printer(
      &std::cout, pt.get_child("configuration_printer"));

  // text_configuration_printer printer(pt.get_child("reader"));
  stacked_view_configuration stacked_view(*p_model,
                                          pt.get_child("stacked_view"));

  for(std::size_t i = 0; i < nsteps; ++i)
  {
    configuration_view const &raw_configuration = processor();
    stacked_view.set_base(raw_configuration);

    std::cout << "time = " << processor.get_current_time() << std::endl;
    printer->set_configuration(stacked_view);
  }
}

static void simulation(property_tree const &pt)
{
  std::unique_ptr<model> p_model = get_model(pt.get_child("model"));

  auto opt_defattr = pt.get_child_optional("model.default_attributes");
  property_tree empty_properties;

  property_tree const &default_attr =
      opt_defattr ? opt_defattr.get() : empty_properties;

  std::unique_ptr<configuration_view> p_init_config =
      get_initial_config(pt.get_child("model.initial_configuration"));
  p_model->set_configuration(*p_init_config, default_attr);

  size_t nevents = pt.get<size_t>("nevents");
  auto event_visitor = [](event const &e) { std::cout << e << std::endl; };
  p_model->run(event_visitor, nevents);
}
static void final_snapshot(property_tree const &pt)
{
  std::unique_ptr<model> p_model = get_model(pt.get_child("model"));

  auto opt_defattr = pt.get_child_optional("model.default_attributes");
  property_tree empty_properties;

  property_tree const &default_attr =
      opt_defattr ? opt_defattr.get() : empty_properties;

  std::unique_ptr<configuration_view> p_init_config =
      get_initial_config(pt.get_child("model.initial_configuration"));

  p_model->set_configuration(*p_init_config, default_attr);

  std::unique_ptr<stream_printer> configuration_printer =
      get_configuration_printer(&std::cout,
                                pt.get_child("configuration_printer"));

  stacked_view_configuration stacked_view(*p_model,
                                          pt.get_child("stacked_view"));

  size_t nevents = pt.get<size_t>("nevents");

  size_t chunk = pt.get<size_t>("event_chunk_size", 0);
  if(0 == chunk)
    chunk = nevents;

  for(std::size_t i = 0; i < nevents; i += chunk)
  {
    std::cerr << "[" << double(100 * i) / double(nevents) << "%] processed "
              << i << " events out of " << nevents << std::endl;

    std::size_t current_chunk = std::min(chunk, nevents - i);
    p_model->run(current_chunk);
  }

  configuration_printer->set_configuration(stacked_view);
}

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

/*  if(tree.count("print_properties"))
    xml_parser::write_xml(std::cout, static_cast<property_tree::super>(tree),
                          xml_writer_make_settings<ptree::key_type>(' ', 2));
  else*/ if(tree.count("simulation"))
    simulation(tree.get_child("simulation"));
  else if(tree.count("snapshots"))
    snapshots(tree.get_child("snapshots"));
  else if(tree.count("final_snapshot"))
    final_snapshot(tree.get_child("final_snapshot"));
  else
    throw std::runtime_error("unrecognized mode");
}
