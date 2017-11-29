#ifndef CONFIGURATION_READER_HPP
#define CONFIGURATION_READER_HPP
#include "interface/interface_fwd.hpp"

BEGIN_NAMESPACE_CORE
class configuration_reader
{
public:
  virtual void set_configuration(configuration_view const &conf) final;
  virtual void set_configuration(configuration_view const &conf,
                                 property_tree const &default_properties) final;

protected:
  virtual void read_configuration(configuration_view const &conf) = 0;
};

END_NAMESPACE_CORE
#endif // CONFIGURATION_READER_HPP
