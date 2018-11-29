#include "chronicles_builder.hpp"

#include "interface/attribute_description.hpp"
#include "interface/configuration_view.hpp"

#include <boost/container/small_vector.hpp>
#include <boost/optional.hpp>

BEGIN_NAMESPACE_CORE

chronicles_builder::chronicles_builder(
    attribute_description const &event_description, std::string const &key_name,
    std::vector<std::string> const &nonkey_attribute_names)
    : abstract_builder{event_description, key_name, nonkey_attribute_names},

      m_event_time_idx{event_description.get_attribute_idx(
          ATTRIBUTE_KIND::EVENT_TIME, true)},
      m_chronicle_data{},
      m_next_id{1}
{
}

chronicles_builder::~chronicles_builder() = default;

void chronicles_builder::set_configuration(
    finite_dataframe const &configuration)
{
  std::vector<std::size_t> mapping =
      m_dataframe_description.lin_mapping_from(configuration.description());

  auto visitor = [this, &mapping](attribute_list const &entry) {
    dataframe_tracker::iterator it;
    bool ok;
    std::tie(it, ok) =
        m_alive_particles.insert(entry, mapping.begin(), mapping.end());
    if(!ok)
      throw std::runtime_error(
          "initial configuration has two particles with same"
          " key attribute");
    attribute const &key = entry[*mapping.begin()];
    chronicle_datum datum{
        generate_next_id(),                      //
        0,                                       //
        -std::numeric_limits<double>::infinity() //
    };
    m_chronicle_data.insert({key, datum});
  };

  configuration.visit_records(visitor);
}

std::size_t chronicles_builder::generate_next_id() { return m_next_id++; }

void chronicles_builder::interpreter_context::reset()
{
  m_maybe_time = boost::none;
  m_maybe_parent = boost::none;
  m_keys.clear();
}

void chronicles_builder::on_event_group_start(std::size_t num_subevents) {}

void chronicles_builder::on_event_group_end()
{
  std::size_t parent_id = m_interpreter_context.m_maybe_parent
                              ? *m_interpreter_context.m_maybe_time
                              : 0;
  double birth_time = m_interpreter_context.m_maybe_time
                          ? *m_interpreter_context.m_maybe_time
                          : -std::numeric_limits<double>::infinity();

  for(attribute const &key : m_interpreter_context.m_keys)
  {
    chronicle_datum datum{generate_next_id(), parent_id, birth_time};
    m_chronicle_data.insert({key, datum});
  }
  m_interpreter_context.reset();
}

void chronicles_builder::on_create(attribute const &key,
                                   attribute_list const &event)
{
  dataframe_tracker::iterator it;
  bool ok;

  std::tie(it, ok) =
      m_alive_particles.insert(event, m_event_to_particle_mapping.begin(),
                               m_event_to_particle_mapping.end());

  if(!ok)
    throw std::runtime_error("attempted to create a particle with key" +
                             key.nice_string() + " that is already present");

  fetch_time_if_not_set(m_interpreter_context, event);
  m_interpreter_context.m_keys.push_back(key);
}

void chronicles_builder::on_remove(attribute const &key,
                                   attribute_list const &event)
{
  chronicle_datum &datum = get_chronicle_or_throw(key);
  tracker_record &record = get_record_or_throw(key);
  double event_time = event[m_event_time_idx].get_real_val();

  emit_particle(record, datum, event_time);

  m_chronicle_data.erase(key);
  m_alive_particles.erase(key);
}

void chronicles_builder::on_transform(attribute const &key,
                                      attribute_list const &event)
{
  chronicle_datum &datum = get_chronicle_or_throw(key);
  tracker_record &record = get_record_or_throw(key);

  double event_time = event[m_event_time_idx].get_real_val();

  emit_particle(record, datum, event_time);

  m_alive_particles.update(event, m_event_to_particle_mapping.begin(),
                           m_event_to_particle_mapping.end());

  fetch_time_if_not_set(m_interpreter_context, event);
  m_interpreter_context.m_keys.push_back(key);
}

chronicles_builder::chronicle_datum &
chronicles_builder::get_chronicle_or_throw(attribute const &key)
{
  chronicles_builder::chronicle_map::iterator chronicle_it =
      m_chronicle_data.find(key);

  if(m_chronicle_data.end() == chronicle_it)
    throw std::runtime_error("chronicler for particle with key " +
                             key.nice_string() + " not found");

  return chronicle_it->second;
}

tracker_record &chronicles_builder::get_record_or_throw(attribute const &key)
{
  dataframe_tracker::iterator dataframe_it = m_alive_particles.find(key);

  if(m_alive_particles.end() == dataframe_it)
    throw std::runtime_error("record not found for particle with key " +
                             key.nice_string());
  return *dataframe_it;
}

void chronicles_builder::fetch_time_if_not_set(
    chronicles_builder::interpreter_context &context,
    attribute_list const &event)
{
  if(context.m_maybe_time)
    return;
  context.m_maybe_time = event[m_event_time_idx].get_real_val();
}

END_NAMESPACE_CORE