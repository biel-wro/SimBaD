#ifndef CORE_CONFIGURATION_VIEW_HPP
#define CORE_CONFIGURATION_VIEW_HPP
#include "interface/interface_fwd.hpp"

#include <cstddef>
#include <functional>
#include <unordered_map>
#include <vector>

#include "finite_dataframe.hpp"

BEGIN_NAMESPACE_CORE
class configuration_view : public finite_dataframe
{
public:
  using particle_attributes = attribute_list;
  using particle_visitor = std::function<void(particle_attributes const &)>;
  using mapping = attribute_description;
  using size_type = std::size_t;

  std::size_t position_attr_idx() const;
  std::size_t dimension() const;
  bool has_unique_id() const;

  virtual ~configuration_view();
};

END_NAMESPACE_CORE
#endif // CONFIGURATION_VIEW_HPP
