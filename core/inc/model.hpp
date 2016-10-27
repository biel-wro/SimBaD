#ifndef MODEL_HPP
#define MODEL_HPP

#include "core_fwd.hpp"

#include <cstddef>
#include <functional>

BEGIN_NAMESPACE_CORE

class model
{
public:
  using size_type = std::size_t;
  using event_visitor = std::function<void(event const &)>;
  using particle_visitor = std::function<void(particle const &)>;

  virtual void generate_events(event_visitor, size_t) = 0;
  virtual size_type configuration_size() const = 0;
  virtual void visit_configuration(particle_visitor) const = 0;
  virtual ~model();
};

END_NAMESPACE_CORE

#endif // MODEL_HPP
