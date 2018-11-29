#ifndef SIMBAD_PROCESSORS_CHRONICLE_BUILDER_HPP
#define SIMBAD_PROCESSORS_CHRONICLE_BUILDER_HPP

#include "processors/dataframe_tracker.hpp"

BEGIN_NAMESPACE_CORE

class abstract_builder
{
public:
  abstract_builder(attribute_description const &event_description,
                   std::string const &key_name,
                   std::vector<std::string> const &nonkey_attribute_names);
  virtual ~abstract_builder();

  virtual void set_configuration(finite_dataframe const &configuration);
  void push_event(attribute_list const &event);

protected:
  virtual void on_event_group_start(std::size_t num_subevents);
  virtual void on_event_group_end();

  virtual void on_create(attribute const &key, attribute_list const &event);
  virtual void on_remove(attribute const &key, attribute_list const &event);
  virtual void on_transform(attribute const &key, attribute_list const &event);

  std::size_t key_idx() const;
  std::size_t event_kind_idx() const;
  std::size_t delta_time_idx() const;

  attribute_description m_dataframe_description;
  std::vector<std::size_t> m_event_to_particle_mapping;
  dataframe_tracker m_alive_particles;
  bool m_event_group_open;
  std::size_t m_current_delta_time;

  std::size_t const m_event_kind_idx;
  std::size_t const m_key_idx;
  std::size_t const m_delta_time_idx;
};

END_NAMESPACE_CORE
#endif // SIMBAD_PROCESSORS_CHRONICLE_BUILDER_HPP
