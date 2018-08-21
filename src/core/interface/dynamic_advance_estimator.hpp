#ifndef SIMBAD_CORE_DYNAMIC_ADVANCER_HPP
#define SIMBAD_CORE_DYNAMIC_ADVANCER_HPP

#include "interface/advance_estimator.hpp"
#include "interface/event_sink.hpp"

BEGIN_NAMESPACE_CORE
class dynamic_advance_estimator : public advance_estimator
{
public:
  virtual void set_description(attribute_description const &desc) = 0;
  virtual void event_update(std::size_t events_since_last_update,
                            event const &) = 0;
};
END_NAMESPACE_CORE

#endif // SIMBAD_DYNAMIC_ADVANCER_HPP
