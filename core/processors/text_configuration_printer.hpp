#ifndef TEXT_CONFIGURATION_PRINTER_HPP
#define TEXT_CONFIGURATION_PRINTER_HPP

#include "interface/configuration_reader.hpp"

#include "core_fwd.hpp"

#include <iosfwd>

BEGIN_NAMESPACE_CORE
class text_configuration_printer : public configuration_reader
{
public:
  text_configuration_printer();
  text_configuration_printer(std::ostream &ostream);
  text_configuration_printer(property_tree const &pt);

  void set_ostream(std::ostream &ostream);
  void read_configuration(const configuration_view &conf) override;


private:
  std::ostream *m_ostream_ptr;
};

END_NAMESPACE_CORE
#endif // TEXT_CONFIGURATION_PRINTER_HPP
