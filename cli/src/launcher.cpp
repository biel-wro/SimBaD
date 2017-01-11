#include "launcher.hpp"
#include "builtin_models.hpp"

#include "configurations/cubic_crystal_configuration.hpp"
#include "configurations/poisson_configuration.hpp"
#include "configurations/stacked_view_configuration.hpp"
#include "interface/event.hpp"
#include "interface/model.hpp"
#include "interface/model_factory.hpp"
#include "interface/model_register.hpp"
#include "interface/property_tree.hpp"
#include "processors/csv_printer.hpp"
#include "processors/snapshotter.hpp"

#include <boost/property_tree/ptree.hpp>

#include <iostream>
#include <string>
using namespace simbad::core;

BEGIN_NAMESPACE_CLI
launcher::launcher(const property_tree &pt)
    : m_model_ptr(make_model(pt.get_child("model")))
{
  set_initial_configuration(pt.get_child("initial_configuration"));
}

cli::launcher::~launcher() {}
void launcher::launch(const property_tree &pt)
{
  /*  if(tree.count("print_properties"))
    xml_parser::write_xml(std::cout, static_cast<property_tree::super>(tree),
                          xml_writer_make_settings<ptree::key_type>(' ', 2));
  else*/

  std::string mode = pt.get<std::string>("mode");

  if("simulation" == mode)
    launch_simulation(pt.get_child("simulation"));
  else if("snapshots" == mode)
    launch_snapshots(pt);
  else if("final_snapshot" == mode)
    launch_final_snapshot(pt.get_child("final_snapshot"));
  else
    throw std::runtime_error("unrecognized mode");
}
void launcher::launch_snapshots(const property_tree &pt)
{
  std::unique_ptr<stream_printer> printer = make_configuration_printer(
      &std::cout, pt.get_child("configuration_printer"));

  snapshotter processor(m_model_ptr.get(), pt.get<double>("run.time_step"));
  std::size_t nsteps = pt.get<std::size_t>("run.max_steps");

  stacked_view_configuration stacked_view(*m_model_ptr,
                                          pt.get_child("stacked_view"));
  for(std::size_t i = 0; i < nsteps; ++i)
  {
    std::cerr << "processing..." << std::endl;
    configuration_view const &raw_configuration = processor();
    stacked_view.set_base(raw_configuration);
    std::cerr << "[" << double(100 * i) / double(nsteps) << "%] processed " << i
              << " snapshots out of " << nsteps << std::endl;
    std::cout << "time = " << processor.get_current_time() << std::endl;
    printer->set_configuration(stacked_view);
  }
}

void launcher::launch_final_snapshot(const property_tree &pt)
{
  std::unique_ptr<stream_printer> printer = make_configuration_printer(
      &std::cout, pt.get_child("configuraiton_printer"));
  stacked_view_configuration stacked_view(*m_model_ptr,
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
    m_model_ptr->run(current_chunk);
  }

  printer->set_configuration(stacked_view);
}

void launcher::launch_simulation(const property_tree &pt)
{
  size_t nevents = pt.get<size_t>("nevents");
  auto event_visitor = [](event const &e) { std::cout << e << std::endl; };
  m_model_ptr->run(event_visitor, nevents);
}

model &launcher::model() { return *m_model_ptr; }
std::unique_ptr<model> launcher::make_model(const property_tree &pt) const
{
  std::string model_name = pt.get<std::string>("class");

  model_register const &reg = simbad::models::common::get_builtin_models();
  model_factory const &factory =
      dynamic_cast<model_factory const &>(reg.get_factory(model_name));

  property_tree parameters_props = pt.get_child("parameters");

  return factory.create_instance(parameters_props);
}

std::unique_ptr<stream_printer>
launcher::make_configuration_printer(std::ostream *os,
                                     const property_tree &pt) const
{
  std::string const &class_name = pt.get<std::string>("class");
  property_tree const &parameters =
      pt.get_child("parameters", property_tree::get_empty());

  if(class_name == "csv_printer")
    return std::unique_ptr<stream_printer>(new csv_printer(os, parameters));
  throw std::logic_error(std::string("unrecognized printer ") + class_name);
}

std::unique_ptr<configuration_view>
launcher::make_initial_configuration(const property_tree &pt) const
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

void launcher::set_initial_configuration(property_tree const &pt)
{
  std::unique_ptr<configuration_view> ptr_raw_init_config =
      make_initial_configuration(pt);

  std::string path = "default_attributes";

  if(pt.count(path))
    m_model_ptr->set_configuration(*ptr_raw_init_config, pt.get_child(path));
  else
    m_model_ptr->set_configuration(*ptr_raw_init_config);
}

END_NAMESPACE_CLI
