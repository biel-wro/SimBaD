#include "cli/launcher.hpp"

#include "builtin_models.hpp"

#include "interface/model.hpp"
#include "interface/model_factory.hpp"
#include "interface/model_register.hpp"
#include "interface/property_tree.hpp"
#include "processors/final_snapshot.hpp"
#include "processors/snapshoter.hpp"
#include "processors/streamer.hpp"
#include "repositories/create_from_property_tree.hpp"
#include "repositories/factory.hpp"
#include "utils/stream_as_model.hpp"

#include <iostream>

using namespace simbad::core;

BEGIN_NAMESPACE_CLI

static std::unique_ptr<core::model> make_model(property_tree const &pt)
{
  std::string model_name = pt.get<std::string>("class");

  model_register const &reg = simbad::models::common::get_builtin_models();
  model_factory const &factory =
      dynamic_cast<model_factory const &>(reg.get_factory(model_name));

  property_tree parameters_props = pt.get_child("parameters");

  return factory.create_instance(parameters_props);
}

static std::unique_ptr<core::model>
make_model_or_replay(property_tree const &pt)
{
  if(pt.count("replay"))
    return std::unique_ptr<core::model>(
        new core::stream_as_model(pt.get_child("replay")));
  return make_model(pt.get_child("model"));
}

static void set_initial_configuration(core::model &model,
                                      property_tree const &pt)
{
  std::unique_ptr<configuration_view> ptr_raw_init_config =
      core::factory_create_from_property_tree<configuration_view>(pt);

  std::string path = "default_attributes";

  if(pt.count(path))
    model.set_configuration(*ptr_raw_init_config, pt.get_child(path));
  else
    model.set_configuration(*ptr_raw_init_config);
}

launcher::launcher(launcher::property_tree pt)
    : m_property_tree(std::move(pt)),
      m_model_ptr(make_model_or_replay(m_property_tree))
{
  set_initial_configuration(*m_model_ptr,
                            m_property_tree.get_child("initial_configuration"));
}
launcher::~launcher() = default;

void launcher::launch()
{
  if(m_property_tree.count("snapshots"))
    launch_snapshots(m_property_tree.get_child("snapshots"));

  else if(m_property_tree.count("stream"))
    launch_stream(m_property_tree.get_child("stream"));

  else if(m_property_tree.count("final_snapshot"))
    launch_final_snapshot(m_property_tree.get_child("final_snapshot"));

  else
    throw std::runtime_error("unrecognized mode");
}

void launcher::launch_snapshots(property_tree const &pt)
{
  snapshoter processor(*m_model_ptr, pt);
  processor.launch();
}
void launcher::launch_stream(property_tree const &pt)
{
  streamer processor(*m_model_ptr, pt);
  processor.launch();
}
void launcher::launch_final_snapshot(property_tree const &pt)
{
  final_snapshot processor(*m_model_ptr, pt);
  processor.launch();
}

END_NAMESPACE_CLI
