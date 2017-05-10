#ifndef CORE_MODEL_HPP
#define CORE_MODEL_HPP

#include "configuration_reader.hpp"
#include "configuration_view.hpp"
#include "event_source.hpp"

BEGIN_NAMESPACE_CORE

class model : public event_source, public configuration_reader
{
public:
  virtual configuration_view const &current_configuration() const = 0;

protected:
  void generate_events(event_visitor, std::size_t) override = 0;
};

END_NAMESPACE_CORE

#endif // MODEL_HPP
