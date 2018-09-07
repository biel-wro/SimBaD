#include "stream_as_model.hpp"

#include "interface/attribute_description.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/configuration_reader.hpp"
#include "interface/configuration_view.hpp"
#include "interface/property_tree.hpp"
#include "interface/stream_reader.hpp"
#include "processors/configuration_builder.hpp"
#include "repositories/create_from_property_tree.hpp"

BEGIN_NAMESPACE_CORE

stream_as_model::stream_as_model(
    std::unique_ptr<stream_reader> stream_reader_ptr,
    std::string const &key_attribute,
    std::vector<std::string> const &nonkey_observables)
    : m_stream_reader_ptr(std::move(stream_reader_ptr)),
      m_event_description_ptr(
          new attribute_description{m_stream_reader_ptr->read_header()}),
      m_configuration_builder_ptr{new configuration_builder(
          *m_event_description_ptr, key_attribute, nonkey_observables)},
      m_maybe_delta_time_idx([this] {
        boost::optional<std::size_t> result;
        boost::optional<attribute_descriptor const &> maybe_descriptor =
            m_event_description_ptr->get_descriptor(
                ATTRIBUTE_KIND::EVENT_DELTA_TIME);
        if(maybe_descriptor)
          result = maybe_descriptor.get().attribute_idx();
        return result;
      }())
{
}
stream_as_model::stream_as_model(property_tree const &pt)
    : stream_as_model(factory_create_from_property_tree<stream_reader>(
                          pt.get_child("reader")),
                      pt.get("key_attribute", "position"), [&pt] {
                        std::vector<std::string> result;
                        for(auto const &key_child_pair :
                            pt.get_child("nonkey_attributes"))
                          result.push_back(key_child_pair.second.data());
                        return result;
                      }())
{
}
stream_as_model::~stream_as_model() = default;

configuration_view const &stream_as_model::current_configuration() const
{
  return *m_configuration_builder_ptr;
}
bool stream_as_model::generate_events(event_source::event_visitor visitor,
                                      std::size_t num_events)
{
  if(!m_maybe_delta_time_idx)
  {
    auto meta_visitor = [&visitor, &builder(*m_configuration_builder_ptr)](
        attribute_list const &event)
    {
      builder.push_event(event);
      visitor(event);
    };

    return m_stream_reader_ptr->visit_entries(meta_visitor, num_events);
  }

  std::size_t full_event_count = 0;

  auto meta_visitor = [
    &full_event_count, delta_time_idx(m_maybe_delta_time_idx.get()), &visitor,
    &builder(*m_configuration_builder_ptr)
  ](attribute_list const &event)
  {
    builder.push_event(event);
    visitor(event);
    if(0 == event[delta_time_idx].get_int_val())
      ++full_event_count;
  };

  while(full_event_count < num_events)
  {
    bool ok = m_stream_reader_ptr->visit_entries(meta_visitor, 1);
    if(!ok)
      return false;
  }
  return true;
}
attribute_description const &
simbad::core::stream_as_model::event_description() const
{
  return *m_event_description_ptr;
}
void stream_as_model::read_configuration(
    configuration_view const &configuration)
{
  m_configuration_builder_ptr->set_configuration(configuration);
}
END_NAMESPACE_CORE