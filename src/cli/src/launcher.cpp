#include "cli/launcher.hpp"

#include "builtin_models.hpp"

#include "interface/model.hpp"
#include "interface/model_factory.hpp"
#include "interface/model_register.hpp"
#include "interface/property_tree.hpp"
#include "interface/stream_reader.hpp"
#include "processors/chronicler.hpp"
#include "processors/final_snapshot.hpp"
#include "processors/snapshoter.hpp"
#include "processors/streamer.hpp"
#include "repositories/create_from_property_tree.hpp"
#include "repositories/factory.hpp"
#include "utils/stream_as_model.hpp"

#include <iostream>

using namespace simbad::core;
#include "processors/printing_chronicle_builder.hpp"
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

launcher::launcher(launcher::property_tree pt) : m_property_tree(std::move(pt))
{
}
launcher::~launcher() = default;

void launcher::launch()
{
  if(m_property_tree.count("model"))
    load_model();
  else if(m_property_tree.count("replay"))
    load_replay();

  if(m_property_tree.count("stream") ||    //
     m_property_tree.count("snapshots") || //
     m_property_tree.count("final_snapshot"))
    launch_model();
  else if(m_property_tree.count("chronicler"))
    launch_nonmodel();

  else
    throw std::runtime_error("no run mode specified");
}
void launcher::load_model()
{
  m_model_ptr = make_model(m_property_tree.get_child("model"));
  set_initial_configuration(*m_model_ptr);
}

void launcher::load_replay()
{
  m_model_ptr = std::unique_ptr<core::model>(
      new core::stream_as_model(m_property_tree.get_child("replay")));
  set_initial_configuration(*m_model_ptr);
}

void launcher::load_reader()
{
  m_stream_reader_ptr = core::factory_create_from_property_tree<stream_reader>(
      m_property_tree.get_child("reader"));
}

void launcher::set_initial_configuration(core::configuration_reader &acceptor)
{
  core::property_tree child =
      m_property_tree.get_child("initial_configuration");

  std::unique_ptr<configuration_view> ptr_raw_init_config =
      core::factory_create_from_property_tree<configuration_view>(child);

  std::string path = "default_attributes";

  if(child.count(path))
    acceptor.set_configuration(*ptr_raw_init_config, child.get_child(path));
  else
    acceptor.set_configuration(*ptr_raw_init_config);
}

void launcher::launch_model()
{
  if(m_property_tree.count("snapshots"))
    launch_snapshots(*m_model_ptr, m_property_tree.get_child("snapshots"));

  else if(m_property_tree.count("stream"))
    launch_stream(*m_model_ptr, m_property_tree.get_child("stream"));

  else if(m_property_tree.count("final_snapshot"))
    launch_final_snapshot(*m_model_ptr,
                          m_property_tree.get_child("final_snapshot"));
  else
    throw std::runtime_error("unrecognized operating mode for model");
}

void launcher::launch_nonmodel()
{
  load_reader();
  if(m_property_tree.count("chronicler"))
    launch_chronicler(*m_stream_reader_ptr,
                      m_property_tree.get_child("chronicler"));
  else
    throw std::runtime_error("unrecognized stream processing");
}

void launcher::launch_snapshots(core::model &model_ref, property_tree const &pt)
{
  snapshoter processor(model_ref, pt);
  processor.launch();
}
void launcher::launch_stream(core::model &model_ref, property_tree const &pt)
{
  streamer processor(model_ref, pt);
  processor.launch();
}
void launcher::launch_final_snapshot(core::model &model_ref,
                                     property_tree const &pt)
{
  final_snapshot processor(model_ref, pt);
  processor.launch();
}



void launcher::launch_chronicler(core::stream_reader &source_ref,
                                 property_tree const &pt)
{
  chronicler processor(source_ref, pt);
  set_initial_configuration(processor.reader());
  processor.launch();
}
END_NAMESPACE_CLI
