#ifndef CORE_MODEL_HPP
#define CORE_MODEL_HPP

#include "configuration_reader.hpp"
#include "configuration_view.hpp"
#include "event_source.hpp"

BEGIN_NAMESPACE_CORE

class model : public event_source,
              public configuration_view,
              public configuration_reader
{
public:
  void generate_events(event_visitor, size_t) override = 0;
  size_type configuration_size() const override = 0;
  void visit_configuration(particle_visitor) const override = 0;
};

END_NAMESPACE_CORE

#endif // MODEL_HPP
