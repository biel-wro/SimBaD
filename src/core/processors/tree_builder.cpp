#include "tree_builder.hpp"

#include "interface/attribute_description.hpp"
#include "interface/event_kind.hpp"
#include "interface/property_tree.hpp"
#include "utils/attribute_exceptions.hpp"

BEGIN_NAMESPACE_CORE

tree_builder::tree_builder(attribute_description const &event_description,
                           std::vector<std::string> const &key_names,
                           std::vector<std::string> const &observable_names)
    : m_indices(), // initialized in-body
      m_key_size(key_names.size()),
      m_mutations(key_names.size(), key_names.size() + observable_names.size()),
      m_event_kind_idx(
          event_description.get_attribute_idx(ATTRIBUTE_KIND::EVENT_KIND)),
      m_mutation_description() // initialized in-body
{
  std::vector<std::string> all_names(key_names);
  all_names.insert(all_names.end(), observable_names.begin(),
                   observable_names.end());

  m_indices.reserve((key_names.size() + observable_names.size()));

  for(std::string const &attribute_name : all_names)
  {
    boost::optional<attribute_descriptor const &> descriptor_opt =
        event_description.get_descriptor(attribute_name);

    if(!descriptor_opt)
      throw unrecognized_attribute_name(attribute_name);

    std::size_t output_index =
        m_mutation_description.copy_attribute_auto_idx(descriptor_opt.get());
    m_indices.push_back(output_index);
  }
}

void tree_builder::push_event(attribute_list const &event) {}
// const dataframe &tree_builder::mutations() const { return m_mutations; }
END_NAMESPACE_CORE
