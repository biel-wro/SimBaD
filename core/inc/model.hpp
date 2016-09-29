#ifndef MODEL_HPP
#define MODEL_HPP

#include "core_fwd.hpp"

#include <functional>

BEGIN_NAMESPACE_CORE

class model
{
public:
  virtual ~model();
  using event_visitor = std::function<void(event const &)>;
  using particle_visitor = std::function<void(particle const &)>;
  virtual void generate_events(event_visitor, size_t) = 0;
  virtual std::size_t configuration_size() const = 0;
  virtual void visit_configuration(particle_visitor) const = 0;
};

END_NAMESPACE_CORE

#endif // MODEL_HPP
