#ifndef SIMBAD_CORE_DYNAMIC_ADVANCER_HPP
#define SIMBAD_CORE_DYNAMIC_ADVANCER_HPP

#include "interface/advancer.hpp"
#include "interface/event_sink.hpp"

BEGIN_NAMESPACE_CORE
class dynamic_advancer : public advancer, public event_sink
{
public:
  virtual void event_update(std::size_t event_number, event const &);
};
END_NAMESPACE_CORE

#endif // SIMBAD_DYNAMIC_ADVANCER_HPP
