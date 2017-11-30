#include "tree_builder.hpp"

#include "interface/attribute_description.hpp"
#include "interface/event_kind.hpp"
#include "interface/property_tree.hpp"
#include "utils/attribute_exceptions.hpp"

BEGIN_NAMESPACE_CORE

tree_builder::tree_builder(attribute_description const &event_description,
                           std::vector<std::string> const &key_names,
                           std::vector<std::string> const &observable_names)
    : m_indices(),
      m_key_size(key_names.size()),
      // m_mutations(description, key_names, observable_names),
      m_event_kind_idx(
          event_description.get_descriptor(ATTRIBUTE_KIND::EVENT_KIND)
              .get()
              .attribute_idx())
{
  attribute_description output_description;
  m_indices.reserve((key_names.size() + observable_names.size()));

  for(std::string const &key_name : key_names)
  {
    boost::optional<attribute_descriptor const &> descriptor_opt =
        event_description.get_descriptor(key_name);

    if(!descriptor_opt)
      throw unrecognized_attribute_name(key_name);
    std::size_t output_index =
        output_description.copy_attribute_auto_idx(descriptor_opt.get());
    m_indices.push_back(output_index);
  }
}

void tree_builder::push_event(attribute_list const &event) {}
//const dataframe &tree_builder::mutations() const { return m_mutations; }
END_NAMESPACE_CORE
