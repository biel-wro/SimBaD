#ifndef STREAM_PRINTER_HPP
#define STREAM_PRINTER_HPP
#include "core_fwd.hpp"
#include "interface/configuration_reader.hpp"
#include <iosfwd>
BEGIN_NAMESPACE_CORE

class stream_printer : public configuration_reader
{
public:
  stream_printer(std::ostream *ostream);
  stream_printer(std::ostream &ostream);
  void set_ostream(std::ostream &ostream);
  std::ostream &ostream();
private:
  std::ostream *m_ostream_ptr;
};

END_NAMESPACE_CORE
#endif // STREAM_PRINTER_HPP
