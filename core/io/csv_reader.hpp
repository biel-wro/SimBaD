#ifndef SIMBAD_CORE_CSV_READER_HPP
#define SIMBAD_CORE_CSV_READER_HPP

#include "io/stream_reader.hpp"

#include <string>
#include <vector>

#include "detail/csv_header_parser.hpp"

BEGIN_NAMESPACE_CORE
class csv_reader : public stream_reader
{
public:
  csv_reader(std::istream *istream, property_tree const &pt);
  csv_reader(std::istream *istream, std::string const &delimiter = ",",
             std::string const &numsep = "_");

  attribute_description read_header() override;
  attribute_list read_entry(attribute_description const &description) override;
  void read_footer(attribute_description const &description) override;

private:
  std::vector<std::pair<std::string, std::size_t>> m_attribute_sizes;

  using iterator_type = std::string::const_iterator;
  csv_header_parser<iterator_type> m_parser;
};
END_NAMESPACE_CORE

#endif // SIMBAD_CSV_READER_HPP
