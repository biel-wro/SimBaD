#include "stream_as_model.hpp"

#include "interface/attribute_description.hpp"
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
          *m_event_description_ptr, key_attribute, nonkey_observables)}
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
  auto meta_visitor = [&visitor, &builder(*m_configuration_builder_ptr)](
      attribute_list const &event)
  {
    builder.push_event(event);
    visitor(event);
  };

  return m_stream_reader_ptr->visit_entries(meta_visitor, num_events);
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