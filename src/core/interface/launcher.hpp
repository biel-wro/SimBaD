#ifndef SIMBAD_CORE_INTERFACE_LAUNCHER_HPP
#define SIMBAD_CORE_INTERFACE_LAUNCHER_HPP

#include "core_fwd.hpp"
#include "interface/event_source.hpp"

BEGIN_NAMESPACE_CORE

class launcher
{
public:
  virtual ~launcher();
  virtual void set_model(core::model &) = 0;
  virtual bool run(event_source::event_visitor) = 0;
};

END_NAMESPACE_CORE
#endif // SIMBAD_CORE_INTERFACE_LAUNCHER_HPP