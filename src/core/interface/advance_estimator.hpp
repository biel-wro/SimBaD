#ifndef SIMBAD_CORE_INTERFACE_ADVANCER_HPP
#define SIMBAD_CORE_INTERFACE_ADVANCER_HPP

#include "core_fwd.hpp"
#include "interface/configuration_reader.hpp"
#include "interface/event_sink.hpp"
#include "interface/event_source.hpp"

BEGIN_NAMESPACE_CORE

class advance_estimator
{
public:

  /**
   * @brief estimate the amount of events which the event source can progress
   *   without reaching the target
   */
  virtual std::size_t estimate() const = 0;

  /**
   * @brief assumes the given target was met and asks to estimate steps for
   *   the next target
   */
  virtual bool next_target() = 0;

  /**
   * @brief give an estimate of how much work towards reaching the target is
   *   already done.
   *
   * @return a value between 0 and 1 that denotes the progress or NaN if the
   *   progress is not possible to estimate
   */
  virtual float progress() const;

  virtual ~advance_estimator();
};

END_NAMESPACE_CORE
#endif // SIMBAD_CORE_INTERFACE_ADVANCER_HPP