#ifndef CONFIGURATION_READER_HPP
#define CONFIGURATION_READER_HPP
#include "core_fwd.hpp"
#include "property_tree.hpp"
BEGIN_NAMESPACE_CORE
class configuration_reader
{
public:
  virtual void set_configuration(configuration_view const &conf) final
  {
    property_tree empty_tree;
    read_configuration(conf, empty_tree);
  }
  virtual void set_configuration(configuration_view const &conf,
                                 property_tree const &default_properties) final
  {
    read_configuration(conf, default_properties);
  }

protected:
  virtual void read_configuration(configuration_view const &conf,
                                  property_tree const &default_properties) = 0;
};

END_NAMESPACE_CORE
#endif // CONFIGURATION_READER_HPP
