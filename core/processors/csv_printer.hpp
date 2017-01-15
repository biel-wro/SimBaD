#ifndef CSV_PRINTER_HPP
#define CSV_PRINTER_HPP
#include "processors/stream_printer.hpp"

#include "core_fwd.hpp"

#include <string>
#include <vector>
BEGIN_NAMESPACE_CORE

class csv_printer : public stream_printer
{
public:
  csv_printer(std::ostream *ostream, property_tree const &pt);
  csv_printer(std::ostream *ostream, std::string delimiter = ",");

  void write_header(configuration_view const &conf) override;
  void write_data(configuration_view const &conf) override;
  void write_footer(configuration_view const &conf) override;

private:
  std::string m_delimiter;
  std::vector<std::size_t> m_dimensions;
};

END_NAMESPACE_CORE
#endif // CSV_PRINTER_HPP
