#include "tree_builder.hpp"

#include "interface/property_tree.hpp"
#include "interface/event_kind.hpp"

BEGIN_NAMESPACE_CORE

tree_builder::tree_builder(attribute_description const &description,
                           std::vector<std::string> const &keynames,
                           std::vector<std::string> const &observable_names)
    : m_mutations(description, keynames, observable_names),
      m_event_kind_idx(description.get_descriptor(ATTRIBUTE_KIND::EVENT_KIND)
                           .get()
                           .attribute_idx())
{
}

void tree_builder::push_event(attribute_list const &event) {
    EVENT_KIND event_kind = event[m_event_kind_idx].get_event_kind_val();
    switch(event_kind)
    {
    case EVENT_KIND::CREATED:
    case EVENT_KIND::TRANSFORMED:

    }

}
const dataframe &tree_builder::mutations() const { return m_mutations; }
END_NAMESPACE_CORE
