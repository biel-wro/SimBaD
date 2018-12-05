#include "abstract_builder.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_kind.hpp"
#include "interface/event_kind.hpp"
#include "interface/finite_dataframe.hpp"

#include <boost/numeric/conversion/cast.hpp>
#include <boost/optional.hpp>

BEGIN_NAMESPACE_CORE

abstract_builder::abstract_builder(
    attribute_description const &event_description, std::string const &key_name,
    std::vector<std::string> const &nonkey_attribute_names)
    : m_dataframe_description{attribute_description::mapped_from(
          event_description,
          [&]() {
            std::vector<std::string> names{key_name};
            names.insert(names.end(), nonkey_attribute_names.begin(),
                         nonkey_attribute_names.end());
            return names;
          }())},
      m_event_to_particle_mapping(
          m_dataframe_description.lin_mapping_from(event_description)),
      m_alive_particles(m_dataframe_description.size()),
      m_event_group_open(false),
      m_current_delta_time{0},

      m_event_kind_idx{event_description.get_attribute_idx(
          ATTRIBUTE_KIND::EVENT_KIND, true)},
      m_key_idx{event_description.get_attribute_idx(key_name)},
      m_delta_time_idx{event_description.get_attribute_idx(
          ATTRIBUTE_KIND::EVENT_DELTA_TIME, true)}
{
}

abstract_builder::~abstract_builder() = default;

void abstract_builder::set_configuration(finite_dataframe const &configuration)
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
  };

  configuration.visit_records(visitor);
}

void abstract_builder::push_event(attribute_list const &event)
{
  EVENT_KIND event_kind = event[event_kind_idx()].get_event_kind_val();
  attribute key = event[key_idx()];
  std::size_t const delta_time =
      boost::numeric_cast<std::size_t>(event[delta_time_idx()].get_int_val());

  assert(delta_time >= 0);

  if(delta_time < m_current_delta_time)
  {
    // this is another sub-event in the same event group
    assert(delta_time == m_current_delta_time - 1);
  }
  else
  {
    // its the first event in the stream or the event group has just ended
    if(m_event_group_open)
      on_event_group_end();
    else
      m_event_group_open = true;

    on_event_group_start(delta_time + 1);
  }

  m_current_delta_time = delta_time;

  switch(event_kind)
  {
  case EVENT_KIND::CREATED: return on_create(key, event);
  case EVENT_KIND::REMOVED: return on_remove(key, event);
  case EVENT_KIND::TRANSFORMED: return on_transform(key, event);

  case EVENT_KIND::MODIFIED:
  case EVENT_KIND::JUMPED_IN:
  case EVENT_KIND::JUMPED_OUT:
    throw std::logic_error("event not supported yet");

  default:
    throw std::logic_error("unexpected value of EVENT_KIND:" +
                           std::to_string(to_numeric(event_kind)));
  }
}

void abstract_builder::on_event_group_start(std::size_t nm) {}
void abstract_builder::on_event_group_end() {}

void abstract_builder::on_create(attribute const &key,
                                 attribute_list const &event)
{
  dataframe_tracker::iterator it;
  bool ok;
  std::tie(it, ok) =
      m_alive_particles.insert(event, m_event_to_particle_mapping.begin(),
                               m_event_to_particle_mapping.end());
  if(!ok)
    throw std::runtime_error("attempted to create a particle with a key " +
                             key.nice_string() + " that is already present");
}
void abstract_builder::on_remove(attribute const &key,
                                 attribute_list const &event)
{
  m_alive_particles.erase(key);
}

void abstract_builder::on_transform(attribute const &key,
                                    attribute_list const &event)
{
}
std::size_t abstract_builder::key_idx() const { return m_key_idx; }
std::size_t abstract_builder::event_kind_idx() const
{
  return m_event_kind_idx;
}
std::size_t abstract_builder::delta_time_idx() const
{
  return m_delta_time_idx;
}

END_NAMESPACE_CORE