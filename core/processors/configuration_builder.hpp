#ifndef SIMBAD_CORE_CONFIGURATION_BUILDER_HPP
#define SIMBAD_CORE_CONFIGURATION_BUILDER_HPP
#include "core_fwd.hpp"

#include "interface/attribute_description.hpp"

#include <cstddef>
#include <memory>
#include <unordered_map>

BEGIN_NAMESPACE_CORE
class configuration_builder
{
public:
  configuration_builder(property_tree const &pt);

  configuration_builder(event_source &source,
                        std::vector<std::string> const &key_attributes,
                        std::vector<std::string> const &value_attributes);

  ~configuration_builder();
  void operator()(event_source &source, std::size_t size);

private:
  std::size_t m_key_size;
  struct particle_set;
  std::unique_ptr<particle_set> m_particle_set_ptr;

  attribute_description m_attribute_description;
  std::vector<std::size_t> m_attribute_mapping;
};

END_NAMESPACE_CORE
#endif
