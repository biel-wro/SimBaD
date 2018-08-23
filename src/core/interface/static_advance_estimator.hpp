#ifndef SIMBAD_CORE_STATIC_ADVANCER_HPP
#define SIMBAD_CORE_STATIC_ADVANCER_HPP

#include "interface/advance_estimator.hpp"
#include "interface/configuration_reader.hpp"

BEGIN_NAMESPACE_CORE
class static_advance_estimator : public advance_estimator
{
public:
  virtual void initialize(std::size_t events_before, configuration_view const
  &) = 0;
  virtual void configuration_update(std::size_t event_number,
                                    configuration_view const &) = 0;
};

END_NAMESPACE_CORE

#endif // SIMBAD_CORE_STATIC_ADVANCER_HPP
