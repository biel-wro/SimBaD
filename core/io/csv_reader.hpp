#ifndef SIMBAD_CORE_CSV_READER_HPP
#define SIMBAD_CORE_CSV_READER_HPP

#include "io/stream_reader.hpp"

#include "detail/csv_header_parser.hpp"
#include "detail/csv_record_parser.hpp"

#include <string>
#include <vector>

BEGIN_NAMESPACE_CORE
class csv_reader : public stream_reader
{
public:
  class attribute_readout;

  csv_reader(std::istream *istream, property_tree const &pt);
  csv_reader(std::istream *istream, std::string const &delimiter = ",",
             std::string const &numsep = "_");
  ~csv_reader();
  attribute_description read_header() override;
  attribute_list const &
  read_entry(attribute_description const &description) override;
  void read_footer(attribute_description const &description) override;

private:
  std::vector<std::size_t> m_attribute_sizes;

  std::string m_line_readout;
  std::unique_ptr<attribute_readout> m_attribute_readout;

  using iterator_type = std::string::const_iterator;
  csv_header_parser<iterator_type> m_header_parser;
  csv_record_parser<iterator_type> m_record_parser;
};
END_NAMESPACE_CORE

#endif // SIMBAD_CSV_READER_HPP
