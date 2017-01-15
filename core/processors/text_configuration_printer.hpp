#ifndef TEXT_CONFIGURATION_PRINTER_HPP
#define TEXT_CONFIGURATION_PRINTER_HPP

#include "processors/stream_printer.hpp"

#include "core_fwd.hpp"

#include <string>

BEGIN_NAMESPACE_CORE
class text_configuration_printer : public stream_printer
{
public:
  text_configuration_printer();
  text_configuration_printer(std::ostream *ostream, std::string delimiter = ", ");
  text_configuration_printer(std::ostream *ostream, property_tree const &pt);


  void write_header(const configuration_view &conf) override;
  void write_data(const configuration_view &conf) override;
  void write_footer(const configuration_view &conf) override;

private:
  std::string m_delimiter;
};

END_NAMESPACE_CORE
#endif // TEXT_CONFIGURATION_PRINTER_HPP
