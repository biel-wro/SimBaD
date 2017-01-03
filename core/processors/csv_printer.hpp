#ifndef CSV_PRINTER_HPP
#define CSV_PRINTER_HPP
#include "processors/stream_printer.hpp"

#include "core_fwd.hpp"

#include <string>
BEGIN_NAMESPACE_CORE

class csv_printer : public stream_printer
{
public:
  csv_printer(std::ostream *ostream, property_tree const &pt);
  csv_printer(std::ostream *ostream, std::string delimiter=",");
  void read_configuration(const configuration_view &conf) override;
private:
  std::string m_delimiter;
};

END_NAMESPACE_CORE
#endif // CSV_PRINTER_HPP
