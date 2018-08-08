#ifndef SIMBAD_CORE_STATIC_ADVANCER_HPP
#define SIMBAD_CORE_STATIC_ADVANCER_HPP

#include "interface/advancer.hpp"
#include "interface/configuration_reader.hpp"

BEGIN_NAMESPACE_CORE
class static_advancer : public advancer{
public:
  virtual void configuration_update(std::size_t event_number,
                                    configuration_view const &) = 0;

};

END_NAMESPACE_CORE


#endif //SIMBAD_CORE_STATIC_ADVANCER_HPP
