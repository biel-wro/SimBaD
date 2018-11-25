#ifndef SIMBAD_PROCESSORS_CHRONICLE_BUILDER_HPP
#define SIMBAD_PROCESSORS_CHRONICLE_BUILDER_HPP

#include "processors/dataframe_tracker.hpp"

BEGIN_NAMESPACE_CORE

class abstract_builder
{
public:
  abstract_builder(attribute_description const &event_descriptor,
                   std::string const &key_name,
                   std::vector<std::string> const &nonkey_attribute_names);
  virtual ~abstract_builder();

  void set_configuration(finite_dataframe const &configuration);
  void on_event(attribute_list const &event);

protected:

  virtual void on_create_event(attribute const &key,
                               attribute_list const &event) = 0;
  virtual void on_update_event(attribute const &key,
                               attribute_list const &event) = 0;
  virtual void on_remove_event(attribute const &key,
                               attribute_list const &event) = 0;

private:
  dataframe_tracker m_alive_particles;
  std::size_t m_event_kind_idx;
  std::size_t m_key_idx;
};

END_NAMESPACE_CORE
#endif // SIMBAD_PROCESSORS_CHRONICLE_BUILDER_HPP
