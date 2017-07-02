#ifndef CSV_PRINTER_HPP
#define CSV_PRINTER_HPP
#include "processors/stream_printer.hpp"

#include "core_fwd.hpp"

#include <string>
#include <vector>
BEGIN_NAMESPACE_CORE

class csv_printer final : public stream_printer
{
public:
  csv_printer(std::ostream *ostream, property_tree const &pt);
  csv_printer(std::ostream *ostream, std::string delimiter = ",");

  void write_header(attribute_description const &desc) override;
  void write_entry(attribute_list const &entry) override;
  void write_data(dataframe const &conf) override;
  void write_footer() override;

private:
  std::string m_delimiter;
  std::vector<std::size_t> m_dimensions;
  std::vector<std::size_t> m_indices;
};

END_NAMESPACE_CORE
#endif // CSV_PRINTER_HPP
