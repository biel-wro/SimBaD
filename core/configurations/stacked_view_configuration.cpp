#include "stacked_view_configuration.hpp"

#include "configurations/default_attributed_configuration.hpp"
#include "configurations/selected_attributes_configuration.hpp"
#include "configurations/slice_configuration.hpp"
#include <stdexcept>
BEGIN_NAMESPACE_CORE

stacked_view_configuration::stacked_view_configuration(
    const configuration_view &base)
    : stacked_view_configuration(base, property_tree())
{
}

stacked_view_configuration::stacked_view_configuration(
    const configuration_view &base, const property_tree &pt)
    : enriched_configuration(base)
{
  for(std::pair<std::string, property_tree> const &child :
      pt.get_child("layers", property_tree::get_empty()))
  {
    property_tree const &subtree = child.second;
    push_layer(subtree.get<std::string>("class"),
               subtree.get_child("parameters"));
  }
}

configuration_view const &stacked_view_configuration::last_layer() const
{
  return m_layers.empty() ? get_base() : *m_layers.back();
}

configuration_view::size_type
stacked_view_configuration::size() const
{
  return last_layer().size();
}

void stacked_view_configuration::visit_records(
    configuration_view::particle_visitor visitor) const
{
  return last_layer().visit_records(visitor);
}

const attribute_descriptor &stacked_view_configuration::descriptor() const
{
  return last_layer().descriptor();
}

void stacked_view_configuration::push_layer(
    std::unique_ptr<enriched_configuration> layer)
{
  configuration_view const &prev = last_layer();
  m_layers.push_back(std::move(layer));
  m_layers.back()->set_base(prev);
}

void stacked_view_configuration::push_layer(const std::string &layer_name,
                                            property_tree const &parameters)
{
  std::unique_ptr<enriched_configuration> ptr;
  if("slice" == layer_name)
    ptr.reset(new slice_configuration(last_layer(), parameters));
  else if("selected_attributes" == layer_name)
    ptr.reset(new selected_attributes_configuration(last_layer(), parameters));
  else if("default_attributes" == layer_name)
    ptr.reset(new default_attributed_configuration(last_layer(), parameters));
  else
    throw std::logic_error(
        std::string("unrecognized configuration layer " + layer_name));

  m_layers.push_back(std::move(ptr));
}

std::unique_ptr<enriched_configuration> stacked_view_configuration::pop_layer()
{
  std::unique_ptr<enriched_configuration> p = std::move(m_layers.back());
  m_layers.pop_back();
  return p;
}
END_NAMESPACE_CORE
