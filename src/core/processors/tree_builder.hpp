#ifndef SIMBAD_PROCESSORS_TREE_BUILDER_HPP
#define SIMBAD_PROCESSORS_TREE_BUILDER_HPP

#include "interface/interface_fwd.hpp"

#include "processors/dataframe_tracker.hpp"

#include <vector>

BEGIN_NAMESPACE_CORE
class tree_builder
{
public:
  tree_builder(attribute_description const &event_description,
               std::string const &particle_key_name,
               std::string const &mutation_key_name,
               std::vector<std::string> const &observable_names);

  void push_event(attribute_list const &event);

  // dataframe const &mutations() const;
private:
  std::size_t const m_event_kind_idx;
  std::size_t const m_particle_key_idx;
  std::size_t const m_mutation_key_idx;

  dataframe_tracker m_mutations;
  std::vector<std::size_t> const m_indices;
  attribute_description const m_mutation_description;
};
END_NAMESPACE_CORE
#endif
