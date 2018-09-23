#include "configuration_builder.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include "interface/event_kind.hpp"
#include "interface/event_source.hpp"

#include <boost/intrusive/set.hpp>
#include <boost/intrusive/unordered_set.hpp>
#include <boost/optional.hpp>
#include <boost/range/join.hpp>

#include <cstddef>
#include <limits>
#include <stdexcept>

BEGIN_NAMESPACE_CORE

configuration_builder::configuration_builder(
    attribute_description const &event_description,        //
    std::string const &key_name,                           //
    std::vector<std::string> const &nonkey_attribute_names //
    )
    : m_configuration_description{attribute_description::mapped_from(
          event_description,
          [&]() {
            std::vector<std::string> names{key_name};
            names.insert(names.end(), nonkey_attribute_names.begin(),
                         nonkey_attribute_names.end());
            return names;
          }())},
      m_index_mapping(
          m_configuration_description.lin_mapping_from(event_description)),
      m_configuration(m_configuration_description.size()),
      m_event_kind_idx(
          event_description.get_descriptor(ATTRIBUTE_KIND::EVENT_KIND, true)
              .value()
              .attribute_idx()),
      m_key_idx(
          event_description.get_descriptor(key_name).value().attribute_idx())
{
  if(nonkey_attribute_names.end() != std::find(nonkey_attribute_names.begin(),
                                               nonkey_attribute_names.end(),
                                               key_name))
    throw std::logic_error("key `" + key_name +
                           "` found in non-key attribute names");
}

configuration_builder::~configuration_builder() = default;

void configuration_builder::set_configuration(
    finite_dataframe const &configuration)
{
  std::vector<std::size_t> mapping =
      m_configuration_description.lin_mapping_from(configuration.description());

  auto visitor = [this, &mapping](attribute_list const &entry) {

    dataframe_tracker::iterator it;
    bool ok;
    std::tie(it, ok) =
        m_configuration.insert(entry, mapping.begin(), mapping.end());
    if(!ok)
      throw std::runtime_error(
          "initial configuration has two particles with same"
          " key attribute");
  };

  configuration.visit_records(visitor);
}

void configuration_builder::push_event(attribute_list const &event)
{
  EVENT_KIND event_kind = event[m_event_kind_idx].get_event_kind_val();

  attribute const &id = event[m_key_idx];

  switch(event_kind)
  {
  case EVENT_KIND::NONE:
    break; // no need to do anything

  case EVENT_KIND::CREATED: //
    create_on_event(id, event);
    break;

  case EVENT_KIND::MODIFIED: //
    update_on_event(id, event);
    break;

  case EVENT_KIND::REMOVED: //
    remove_on_event(id);
    break;

  case EVENT_KIND::TRANSFORMED: //
    remove_on_event(id);
    create_on_event(id, event);

    break;
  case EVENT_KIND::JUMPED_IN:  //
  case EVENT_KIND::JUMPED_OUT: //
    std::logic_error("not supported yet");

  default: throw std::logic_error("unexpected enum value");
  }
}

void configuration_builder::visit_records(
    dataframe::record_visitor visitor) const
{
  return m_configuration.visit_records(visitor);
}
const attribute_description &configuration_builder::description() const
{
  return m_configuration_description;
}
std::size_t configuration_builder::size() const
{
  return m_configuration.size();
}
void configuration_builder::create_on_event(attribute const &key,
                                            attribute_list const &event)
{
  dataframe_tracker::iterator it;
  bool ok;
  std::tie(it, ok) = m_configuration.insert(event, m_index_mapping.begin(),
                                            m_index_mapping.end());
  if(!ok)
    throw std::runtime_error("attempted to create a particle with a key that "
                             "is already present");
}
void configuration_builder::update_on_event(attribute const &id,
                                            attribute_list const &event)
{
  bool ok = m_configuration.update(event, m_index_mapping.begin(),
                                   m_index_mapping.end());
  if(!ok)
    throw std::runtime_error("there was an update event for a particle which "
                             "does not exist yet");
}

void configuration_builder::remove_on_event(attribute const &key)
{
  m_configuration.erase(key);
}

END_NAMESPACE_CORE
