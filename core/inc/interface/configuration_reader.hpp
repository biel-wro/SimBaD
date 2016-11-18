#ifndef CONFIGURATION_READER_HPP
#define CONFIGURATION_READER_HPP
#include "core_fwd.hpp"

BEGIN_NAMESPACE_CORE
class configuration_reader
{
public:
  virtual void read_configuration(configuration_view const&conf) = 0;
};

END_NAMESPACE_CORE
#endif // CONFIGURATION_READER_HPP
