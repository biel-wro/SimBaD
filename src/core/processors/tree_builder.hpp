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
               std::vector<std::string> const &keynames,
               std::vector<std::string> const &observable_names);

  void push_event(attribute_list const &event);

  // dataframe const &mutations() const;
private:
  std::size_t m_key_size;
  std::size_t const m_event_kind_idx;
  dataframe_tracker m_mutations;
  std::vector<std::size_t> m_indices;
  attribute_description m_mutation_description;
  std::vector<std::size_t> m_mutation_counts;
};
END_NAMESPACE_CORE
#endif
