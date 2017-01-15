#ifndef CORE_CONFIGURATION_VIEW_HPP
#define CORE_CONFIGURATION_VIEW_HPP
#include "core_fwd.hpp"

#include <cstddef>
#include <functional>
#include <unordered_map>
#include <vector>

BEGIN_NAMESPACE_CORE
class configuration_view
{
public:
  using particle_attributes = attribute_list;
  using particle_visitor = std::function<void(particle_attributes const &)>;
  using mapping = attribute_descriptor;

  using size_type = std::size_t;

  virtual size_type configuration_size() const = 0;
  virtual void visit_configuration(particle_visitor visitor) const = 0;

  virtual std::size_t position_attr_idx() const final;

  virtual size_type dimension() const final;
  virtual bool has_unique_id() const final;

  virtual attribute_descriptor const &new_attr_map() const = 0;

  virtual ~configuration_view();

protected:
};

END_NAMESPACE_CORE
#endif // CONFIGURATION_VIEW_HPP
