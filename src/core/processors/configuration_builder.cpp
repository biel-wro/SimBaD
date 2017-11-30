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

configuration_builder::configuration_builder(
    attribute_description const &event_description,
    std::vector<std::string> const &key_names,
    std::vector<std::string> const &value_names)
    : m_key_size(key_names.size()),
      m_event_kind_idx(
          event_description.get_descriptor(ATTRIBUTE_KIND::EVENT_KIND)
              .get()
              .attribute_idx()),
      m_configuration(event_description, key_names, value_names)
{
}

configuration_builder::~configuration_builder() {}
void configuration_builder::push_event(attribute_list const &event)
{
  EVENT_KIND event_kind = event[m_event_kind_idx].get_event_kind_val();

  dataframe_tracker::iterator it;
  dataframe_tracker::insert_commit_data commit_data;
  bool already_exists;
  std::tie(it, already_exists) =
      m_configuration.insert_check(event, commit_data);
}
const dataframe &configuration_builder::configuration() const
{
  return m_configuration;
}

END_NAMESPACE_CORE
