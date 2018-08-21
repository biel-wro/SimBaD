#ifndef SIMBAD_CORE_INTERFACE_ADVANCER_HPP
#define SIMBAD_CORE_INTERFACE_ADVANCER_HPP

#include "core_fwd.hpp"
#include "interface/event_source.hpp"
#include "interface/configuration_reader.hpp"
#include "interface/event_sink.hpp"

BEGIN_NAMESPACE_CORE

class advance_estimator
{
public:
  virtual std::size_t estimate() const = 0;
  /**
   * @brief assumes the given target was met and asks to estimate steps for
   * the next target
   */
  virtual bool next_target() = 0;

  virtual ~advance_estimator();
};


END_NAMESPACE_CORE
#endif // SIMBAD_CORE_INTERFACE_ADVANCER_HPP