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

#include <cstddef>
#include <limits>

BEGIN_NAMESPACE_CORE
//TODO: key name must be added to description or value_names
configuration_builder::configuration_builder(
    attribute_description const &event_description, //
    std::string const &key_name,                    //
    std::vector<std::string> const &value_names     //
    )
    : m_configuration_description(
          attribute_description::mapped_from(event_description, value_names)),
      m_index_mapping(
          m_configuration_description.lin_mapping_from(event_description)),
      m_configuration(key_size + value_names.size(), key_size),
      m_event_kind_idx(
          event_description.get_descriptor(ATTRIBUTE_KIND::EVENT_KIND)
              .get()
              .attribute_idx()),
      m_key_idx(
          event_description.get_descriptor(key_name).get().attribute_idx())
{
}

configuration_builder::~configuration_builder() {}
void configuration_builder::push_event(attribute_list const &event)
{
  EVENT_KIND event_kind = event[m_event_kind_idx].get_event_kind_val();

  attribute const &id = event[m_key_idx];

  switch(event_kind)
  {
  case EVENT_KIND::NONE:
    break; // no need to do anything

  case EVENT_KIND::CREATED:  //
  case EVENT_KIND::MODIFIED: //
    update_on_event(id, event);
    break;

  case EVENT_KIND::REMOVED: //
    remove_on_event(id);
    break;

  case EVENT_KIND::JUMPED_IN:   //
  case EVENT_KIND::JUMPED_OUT:  //
  case EVENT_KIND::TRANSFORMED: //
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

void configuration_builder::update_on_event(attribute const &id,
                                            attribute_list const &event)
{
  dataframe_tracker::iterator it;
  bool already_exists;
  std::tie(it, already_exists) = m_configuration.insert(id);
  it->update(event,                                        //
             std::next(m_index_mapping.begin(), key_size), //
             m_index_mapping.end(),                        //
             key_size                                      //
             );
}

void configuration_builder::remove_on_event(attribute const &key)
{
  m_configuration.erase(key);
}

END_NAMESPACE_CORE
