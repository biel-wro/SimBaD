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
  using particle_visitor = std::function<void(particle const &)>;
  using size_type = std::size_t;


  virtual size_type configuration_size() const = 0;
  virtual void visit_configuration(particle_visitor visitor) const = 0;

  virtual size_type dimension() const = 0;
  virtual bool has_unique_id() const;
  virtual attribute_mapping const &attr_map() const;

  virtual ~configuration_view();

protected:
};

END_NAMESPACE_CORE
#endif // CONFIGURATION_VIEW_HPP
