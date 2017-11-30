#ifndef SIMBAD_CORE_CONFIGURATION_BUILDER_HPP
#define SIMBAD_CORE_CONFIGURATION_BUILDER_HPP
#include "core_fwd.hpp"

#include "core_fwd.hpp"
#include "processors/dataframe_tracker.hpp"

#include <cstddef>
#include <memory>
#include <unordered_map>

BEGIN_NAMESPACE_CORE
class configuration_builder
{
public:
  configuration_builder(property_tree const &pt);

  configuration_builder(attribute_description const &event_description,
                        std::vector<std::string> const &key_attributes,
                        std::vector<std::string> const &value_attributes);

  ~configuration_builder();

  void push_event(attribute_list const &event);

  dataframe const &configuration() const;

private:
  std::size_t const m_event_kind_idx;
  std::size_t const m_key_size;
  std::vector<std::size_t> m_input_indices;
  dataframe_tracker m_configuration;
};

END_NAMESPACE_CORE
#endif
