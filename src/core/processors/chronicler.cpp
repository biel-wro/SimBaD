#include "chronicler.hpp"

#include "interface/advance_estimator.hpp"

#include "interface/event_source.hpp"
#include "interface/property_tree.hpp"
#include "interface/stream_printer.hpp"
#include "interface/stream_reader.hpp"
#include "processors/printing_chronicle_builder.hpp"
#include "repositories/create_from_property_tree.hpp"

#include <algorithm>

BEGIN_NAMESPACE_CORE

chronicler::chronicler(stream_reader &reader_ref,
                       std::unique_ptr<stream_printer> stream_printer_ptr,
                       std::string const &key_name,
                       std::vector<std::string> const &nonkey_names)
    : m_reader(reader_ref),
      m_stream_printer_ptr(std::move(stream_printer_ptr)),

      m_builder_ptr(std::make_unique<printing_chronicle_builder>(
          reader_ref.read_header(), key_name, nonkey_names,
          *m_stream_printer_ptr))
{
}

chronicler::chronicler(stream_reader &reader_ref, property_tree const &pt)
    : chronicler{reader_ref, factory_create_from_property_tree<stream_printer>(
                                 pt.get_child("printer")),
                 pt.get<std::string>("key_name", "position"),
                 pt.get_vector<std::string>("nonkey_attributes")}
{
}

chronicler::~chronicler() = default;

void chronicler::launch()
{
  bool can_continue = true;

  auto event_visitor = [this](attribute_list const &event) {
    m_builder_ptr->push_event(event);
  };

  m_builder_ptr->write_header();
  do
    can_continue =
        m_reader.visit_entries(event_visitor, default_advancement_step);
  while(can_continue);

  m_builder_ptr->write_footer();
}
void chronicler::set_initial_configuration(configuration_view const &conf)
{
  m_builder_ptr->set_configuration(conf);
}
configuration_reader &chronicler::reader()
{
  return *m_builder_ptr;
}

printing_chronicle_builder &chronicler::builder() { return *m_builder_ptr; }
END_NAMESPACE_CORE
