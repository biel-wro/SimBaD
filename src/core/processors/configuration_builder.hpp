#ifndef SIMBAD_CORE_CONFIGURATION_BUILDER_HPP
#define SIMBAD_CORE_CONFIGURATION_BUILDER_HPP

#include "interface/interface_fwd.hpp"

#include "interface/finite_dataframe.hpp"
#include "processors/dataframe_tracker.hpp"

#include <cstddef>
#include <memory>
#include <unordered_map>

BEGIN_NAMESPACE_CORE
class configuration_builder : public finite_dataframe
{
public:
  configuration_builder(property_tree const &pt);

  configuration_builder(attribute_description const &event_description,
                        std::string const &key_attribute,
                        std::vector<std::string> const &value_attributes);

  ~configuration_builder();

  void push_event(attribute_list const &event);

  void visit_records(record_visitor visitor) const override;
  const attribute_description &description() const override;
  std::size_t size() const override;
protected:
  //void create_on_event(attribute const &key, attribute_list const &event);
  void update_on_event(attribute const &key, attribute_list const &event);
  void remove_on_event(attribute const &key);

private:
  attribute_description m_configuration_description;
  std::vector<std::size_t> m_index_mapping;
  dataframe_tracker m_configuration;
  std::size_t const m_event_kind_idx;
  std::size_t const m_key_idx;
  static constexpr std::size_t key_size = 1;
};

END_NAMESPACE_CORE
#endif
